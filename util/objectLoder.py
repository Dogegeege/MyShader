"""
实验性质的代码，应用于模型格式加载 .obj格式文件。
Object Format Loder
-------------------------------------------------------------------------------------------------

v: 顶点
vn: 法线
vt: uv坐标
f: 面索引

"""

import os


def find_obj_files(directory):
    obj_files = []
    for root, dirs, files in os.walk(directory):  # 遍历目录及其子目录
        for file in files:
            if file.lower().endswith(".obj"):  # 检查文件扩展名（忽略大小写）
                obj_files.append(os.path.join(root, file))
    return obj_files


def cpp_string():
    cpp_string = """
    #include <iostream>
    #include <vector>
    #include <string>

    int main() {
        std::vector<std::string> vertices;
        std::vector<std::string> uvs;
        std::vector<std::string> normals;
        std::vector<std::string> faces;

        // 读取文件并填充 vertices, uvs, normals, faces 向量

        return 0;
    }
    """
    return cpp_string


def main():
    # 指定搜索的目录
    directory = "model/sculpture-bust-of-roza-loewenfeld"
    obj_files = find_obj_files(directory)

    # 打印找到的 .OBJ 文件
    for obj_file in obj_files:
        print(obj_file)

    vertices = []
    uvs = []
    normals = []
    faces = []

    with open(obj_files[0], "r") as file:
        for line in file:

            if len(line) == 0:
                continue

            if line[:2] == "v ":
                vertices.append(line[2:].strip().split())
            elif line[:3] == "vt ":
                uvs.append(line[3:].strip().split())
            elif line[:3] == "vn ":
                normals.append(line[3:].strip().split())
            elif line[:2] == "f ":
                face = line[2:].strip().split()
                for i in range(len(face)):
                    face[i] = face[i].split("/")
                faces.append(face)
    for face in faces:
        for index in face:
            if len(index) == 1:
                index.append("0")
                index.append("0")
            elif len(index) == 2:
                index.append("0")

            completeVertex = []

            # 注意：obj文件的索引是从1开始的
            if index[0] != "0":
                vertex = vertices[int(index[0]) - 1]
                completeVertex.append(vertex)

            if index[1] != "0":
                uv = uvs[int(index[1]) - 1]
                completeVertex.append(uv)

            if index[2] != "0":
                normal = normals[int(index[2]) - 1]
                completeVertex.append(normal)

            completeVertex = [item for sublist in completeVertex for item in sublist]
            print("vertex:", completeVertex)


if __name__ == "__main__":
    main()
