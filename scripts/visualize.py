import sys
import pdb
import struct
import open3d as o3d
import numpy as np

fileLocation = sys.argv[1]

DATA_SIZE = 4
DATA_AMOUNT = 3
STRUCT_SIZE = DATA_AMOUNT * DATA_SIZE

b = bytearray()
with open(fileLocation, "rb") as f :
    for line in f:
        for byte in line:
            b.append(byte)

vals = []
for i in range(0,len(b),STRUCT_SIZE ):
    location = i
    coord = []
    for j in range(i, i + STRUCT_SIZE, DATA_SIZE):
        f = bytearray(b[j:j+DATA_SIZE])
        coord.append(struct.unpack("f", f)[0])
    vals.append(coord)

pcd = o3d.geometry.PointCloud()
pcd.points = o3d.utility.Vector3dVector(vals)

vis = o3d.visualization.Visualizer()
vis.create_window()

vis.add_geometry(pcd)
opt = vis.get_render_option()
opt.point_size = 1.0

vis.run()

vis.destroy_window()


