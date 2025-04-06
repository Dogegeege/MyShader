"""
实验性质的代码，应用于模型格式加载 .obj格式文件。
当然它仅能加载一些形式单一的模型，目前已弃用

EASY Object Format Loder
-------------------------------------------------------------------------------------------------

v: 顶点
vn: 法线
vt: uv坐标
f: 面索引

"""

import os
import csv


def find_obj_files(directory):
    obj_files = []
    for root, dirs, files in os.walk(directory):  # 遍历目录及其子目录
        for file in files:
            if file.lower().endswith(".obj"):  # 检查文件扩展名（忽略大小写）
                obj_files.append(os.path.join(root, file))
    return obj_files


def main():
    # 指定搜索的目录
    directory = "model/vtuber-neuro-sama-v3"
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

            if line.startswith("v "):  # 顶点
                vertices.append(list(map(float, line[2:].strip().split())))
            elif line.startswith("vt "):  # UV 坐标
                uvs.append(list(map(float, line[3:].strip().split())))
            elif line.startswith("vn "):  # 法线
                normals.append(list(map(float, line[3:].strip().split())))
            elif line.startswith("f "):  # 面索引
                face = line[2:].strip().split()
                face_indices = [list(map(int, f.split("/"))) for f in face]
                faces.append(face_indices)

    print("\nLoaded data:")
    print("Vertices:", len(vertices))
    print("UVs:", len(uvs))
    print("Normals:", len(normals))
    print("Faces:", len(faces))

    print("\nfacecount:", faces[:5])

    # 创建唯一的顶点数据和索引数组
    unique_vertices = []
    indices = []
    vertex_map = {}  # 用于存储唯一顶点的映射

    for face in faces:
        for index in face:
            while len(index) < 3:
                index.append(0)

            # 注意：obj文件的索引是从1开始的
            # 获取顶点、UV 和法线
            vertex = vertices[index[0] - 1] if index[0] != 0 else [0.0, 0.0, 0.0]
            uv = uvs[index[1] - 1] if index[1] != 0 else [0.0, 0.0]
            normal = normals[index[2] - 1] if index[2] != 0 else [0.0, 0.0, 0.0]

            complete_vertex = vertex + uv + normal
            vertex_tuple = tuple(complete_vertex)

            if vertex_tuple not in vertex_map:
                vertex_map[vertex_tuple] = len(unique_vertices)
                unique_vertices.append(complete_vertex)

            indices.append(vertex_map[vertex_tuple])

    # 打印结果
    print("\nUnique Vertices:")
    for v in unique_vertices[:5]:
        print(v)

    print("\nIndices:")
    print(indices[:5])

    with open(
        directory + "/source/_unique_vertices.csv", "w", newline="", encoding="utf-8"
    ) as vertex_file:
        writer = csv.writer(vertex_file)
        writer.writerow(["x", "y", "z", "u", "v", "nx", "ny", "nz"])  # 写入表头
        writer.writerows(unique_vertices)

    with open(
        directory + "/source/_indices.csv", "w", newline="", encoding="utf-8"
    ) as index_file:
        writer = csv.writer(index_file)
        writer.writerow(["index"])  # 写入表头
        writer.writerows([[i] for i in indices])

    with open(
        directory + "/source/_prop.csv", "w", newline="", encoding="utf-8"
    ) as prop_file:
        writer = csv.writer(prop_file)
        writer.writerow(["prop"])  # 写入表头
        writer.writerow([3, 2, 3])  # 写入属性数据

    print("\nCSV files have been written: unique_vertices.csv, indices.csv, prop.csv")


if __name__ == "__main__":
    main()
