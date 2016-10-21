import paraFEM

vertices = []
triangles = []

with open("half_sphere.obj") as fo:
    for i, line in enumerate(fo):
        if i < 2:
            continue
        if line[0] == "v":
            vertices.append([float(p) for p in line[2:].split()])
        if line[0] == "f":
            triangles.append([int(p) - 1 for p in line[2:].split()])

unwrapper = paraFEM.LscmRelax(vertices, triangles, [])
unwrapper.lscm()
print(unwrapper.flat_vertices)