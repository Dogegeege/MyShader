#version 330 core

/*
* "Seascape" by Alexander Alekseev aka TDM - 2014
 * License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * Contact: tdmaav@gmail.com
 */
precision mediump float;

in mat4 iProjection;
in mat4 iView;

uniform vec3 iResolution;// viewport resolution (in pixels)
uniform float iTime;// shader playback time (in seconds)
uniform vec4 iMouse;// mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec3 iOrig; 
uniform vec3 iDir;
uniform vec3 iEuler;

uniform float iFov;
uniform vec3 iCamRight;
uniform vec3 iCamUp;
uniform vec3 iCamFront;



out vec4 FragColor;

const int NUM_STEPS=32;
const float PI=3.141592;
const float EPSILON=1e-3;
#define EPSILON_NRM (.1/iResolution.x)
//#define AA

// sea
const int ITER_GEOMETRY=3;
const int ITER_FRAGMENT=5;
const float SEA_HEIGHT=.6;
const float SEA_CHOPPY=4.;
const float SEA_SPEED=.8;
const float SEA_FREQ=.16;
const vec3 SEA_BASE=vec3(0.,.09,.18);
const vec3 SEA_WATER_COLOR=vec3(.8,.9,.6)*.6;
#define SEA_TIME (1.+iTime*SEA_SPEED)
const mat2 octave_m=mat2(1.6,1.2,-1.2,1.6);

// math
//通过欧拉角生成旋转矩阵，用于相机方向控制。
mat3 fromEuler(vec3 ang) {
    vec2 a1=vec2(sin(ang.x),cos(ang.x));
    vec2 a2=vec2(sin(ang.y),cos(ang.y));
    vec2 a3=vec2(sin(ang.z),cos(ang.z));
    mat3 m;
    m[0]=vec3(a1.y*a3.y+a1.x*a2.x*a3.x,a1.y*a2.x*a3.x+a3.y*a1.x,-a2.y*a3.x);
    m[1]=vec3(-a2.y*a1.x,a1.y*a2.y,a2.x);
    m[2]=vec3(a3.y*a1.x*a2.x+a1.y*a3.x,a1.x*a3.x-a1.y*a3.y*a2.x,a2.y*a3.y);
    return m;
}
//生成伪随机噪声，用于波浪细节。
float hash(vec2 p) {
    float h=dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}
float noise(in vec2 p) {
    vec2 i=floor(p);
    vec2 f=fract(p);
    vec2 u=f*f*(3.-2.*f);
    return-1.+2.*mix(mix(hash(i+vec2(0.,0.)),
            hash(i+vec2(1.,0.)),u.x),
        mix(hash(i+vec2(0.,1.)),
            hash(i+vec2(1.,1.)),u.x),u.y);
}

// lighting
float diffuse(vec3 n,vec3 l,float p) {
    return pow(dot(n,l)*.4+.6,p);
}
float specular(vec3 n,vec3 l,vec3 e,float s) {
    float nrm=(s+8.)/(PI*8.);
    return pow(max(dot(reflect(e,n),l),0.),s)*nrm;
}

// sky
vec3 getSkyColor(vec3 e) {
    e.y=(max(e.y,0.)*.8+.2)*.8;
    return vec3(pow(1.-e.y,2.),1.-e.y,.6+(1.-e.y)*.4)*1.1;
}

// sea
float sea_octave(vec2 uv,float choppy) {
    uv+=noise(uv);
    vec2 wv=1.-abs(sin(uv));
    vec2 swv=abs(cos(uv));
    wv=mix(wv,swv,wv);
    return pow(1.-pow(wv.x*wv.y,.65),choppy);
}

float map(vec3 p) {
    float freq=SEA_FREQ;
    float amp=SEA_HEIGHT;
    float choppy=SEA_CHOPPY;
    vec2 uv=p.xz;uv.x*=.75;

    float d,h=0.;
    for(int i=0;i<ITER_GEOMETRY;i++) {
        d=sea_octave((uv+SEA_TIME)*freq,choppy);
        d+=sea_octave((uv-SEA_TIME)*freq,choppy);
        h+=d*amp;
        uv*=octave_m;freq*=1.9;amp*=.22;
        choppy=mix(choppy,1.,.2);
    }
    return p.y-h;
}

float map_detailed(vec3 p) {
    float freq=SEA_FREQ;
    float amp=SEA_HEIGHT;
    float choppy=SEA_CHOPPY;
    vec2 uv=p.xz;uv.x*=.75;

    float d,h=0.;
    for(int i=0;i<ITER_FRAGMENT;i++) {
        d=sea_octave((uv+SEA_TIME)*freq,choppy);
        d+=sea_octave((uv-SEA_TIME)*freq,choppy);
        h+=d*amp;
        uv*=octave_m;freq*=1.9;amp*=.22;
        choppy=mix(choppy,1.,.2);
    }
    return p.y-h;
}

vec3 getSeaColor(vec3 p,vec3 n,vec3 l,vec3 eye,vec3 dist) {
    float fresnel=clamp(1.-dot(n,-eye),0.,1.);
    fresnel=min(fresnel*fresnel*fresnel,.5);

    vec3 reflected=getSkyColor(reflect(eye,n));
    vec3 refracted=SEA_BASE+diffuse(n,l,80.)*SEA_WATER_COLOR*.12;

    vec3 color=mix(refracted,reflected,fresnel);

    float atten=max(1.-dot(dist,dist)*.001,0.);
    color+=SEA_WATER_COLOR*(p.y-SEA_HEIGHT)*.18*atten;

    color+=specular(n,l,eye,60.);

    return color;
}

// tracing
vec3 getNormal(vec3 p,float eps) {
    vec3 n;
    n.y=map_detailed(p);
    n.x=map_detailed(vec3(p.x+eps,p.y,p.z))-n.y;
    n.z=map_detailed(vec3(p.x,p.y,p.z+eps))-n.y;
    n.y=eps;
    return normalize(n);
}

float heightMapTracing(vec3 ori,vec3 dir,out vec3 p) {
    float tm=0.;
    float tx=1000.;
    float hx=map(ori+dir*tx);
    if(hx>0.) {
        p=ori+dir*tx;
        return tx;
    }
    float hm=map(ori);
    for(int i=0;i<NUM_STEPS;i++) {
        float tmid=mix(tm,tx,hm/(hm-hx));
        p=ori+dir*tmid;
        float hmid=map(p);
        if(hmid<0.) {
            tx=tmid;
            hx=hmid;
        }else {
            tm=tmid;
            hm=hmid;
        }
        if(abs(hmid)<EPSILON)break;
    }
    return mix(tm,tx,hm/(hm-hx));
}



vec3 getPixel(vec2 coord,float time) {
    vec2 uv=coord/iResolution.xy;
    uv=uv*2.0-1.0;
    uv.x*=iResolution.x/iResolution.y;//标准化位置

    
    // ray
    //vec3 ang=vec3(sin(time*3.)*.1,sin(tisme)*.2+.3,time);
    //vec3 ori=vec3(0.,3.5,time*5.);
    //vec3 dir=normalize(vec3(uv.xy,-2.0));  
    //dir.z+=length(uv)*.14;

    vec3 ori=vec3(-iOrig.z ,-iOrig.y,iOrig.x);
    vec3 ang=iEuler;
    vec3 dir=normalize(vec3(uv.xy,-2.0));
    dir.z+=length(uv)*.14;
    dir=normalize(dir)*fromEuler(ang);



    // tracing
    vec3 p;
    heightMapTracing(ori,dir,p);
    vec3 dist=p-ori;
    vec3 n=getNormal(p,dot(dist,dist)*EPSILON_NRM);
    vec3 light=normalize(vec3(0.,1.,.8));

    // color
    return mix(
        getSkyColor(dir),
        getSeaColor(p,n,light,dir,dist),
        pow(smoothstep(0.,-.02,dir.y),.2));
}

// main
void main() {
    float time=iTime*.3+iMouse.x*.01;

#ifdef AA
    vec3 color=vec3(0.);
    for(int i=-1;i<=1;i++) {
        for(int j=-1;j<=1;j++) {
            vec2 uv=gl_FragCoord+vec2(i,j)/3.0;
            color+=getPixel(uv,time);
        }
    }
    color/=9.;
#else
    vec3 color = getPixel(gl_FragCoord.xy,time);
#endif

    // post
    FragColor=vec4(pow(color,vec3(.65)),1.);
}