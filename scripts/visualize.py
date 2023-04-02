import sys
import pdb
import struct
import open3d as o3d
import numpy as np
from threading import Thread
from multiprocessing import Condition
import time
import math

DATA_SIZE = 4
DATA_AMOUNT = 3
STRUCT_SIZE = DATA_AMOUNT * DATA_SIZE

def getName(prefix, number):
    return prefix + "_" + str(number) + ".pointCloud"

def readFile(filePath):
    b = bytearray()
    with open(filePath, "rb") as f :
        for line in f:
            for byte in line:
                b.append(byte)
    vals = []
    for i in range(0,len(b),STRUCT_SIZE ):
        coord = []
        for j in range(i, i + STRUCT_SIZE, DATA_SIZE):
            f = bytearray(b[j:j+DATA_SIZE])
            coord.append(struct.unpack("f", f)[0])
        vals.append(coord)
    return vals

seeNextFrame = False
def canSeeNextFrame():
    return seeNextFrame
def setSeeNextFrame(val):
    global seeNextFrame 
    seeNextFrame = val

def controlPlayBack(condition, userControlledPlayBack, fps):
    fileLocation_prefix = sys.argv[1]
    curFileNum = 1
    def updateRender(curFileNum):
        condition.acquire()
        if(canSeeNextFrame()):
            condition.release()
            return       
        setSeeNextFrame(True)
        global pcd
        global vis
        pcd.points = o3d.utility.Vector3dVector(readFile(getName(fileLocation_prefix, curFileNum)))
        vis.update_geometry(pcd)    
        condition.notify_all()
        condition.release()
        curFileNum+=1
    
    waitTime = 1.0 / fps
    timeRemaing = 1.0 / fps
    while True:
        if(userControlledPlayBack):
            input("See next frame?")
        else:
            time.sleep(timeRemaing)
            timeRemaing = waitTime
        start = time.time()
        updateRender(curFileNum)
        delta = time.time() - start
        timeRemaing = timeRemaing - delta
        if timeRemaing < 0:
            timeRemaing = 0

        curFileNum+=1
            

condition = Condition()
global vis 
vis = o3d.visualization.Visualizer()
global pcd
pcd = o3d.geometry.PointCloud()


def main():
    fileLocation_prefix = sys.argv[1]
    numberOfFrames = int(sys.argv[2])
    curFileNum = 0

    global vis
    global pcd
    vis.create_window()
    pcd.points = o3d.utility.Vector3dVector(readFile(getName(fileLocation_prefix, curFileNum)))
    vis.add_geometry(pcd)
    vis.poll_events()
    vis.update_renderer()
    thread = Thread(target = controlPlayBack, args = (condition, False, 1))
    thread.start()

    i = 0
    while i < numberOfFrames:
        vis.poll_events()
        vis.update_renderer()  
        condition.acquire()
        if(not canSeeNextFrame()):
            condition.release()
            continue
        setSeeNextFrame(False)
        vis.poll_events()
        vis.update_renderer()          
        i+=1
        condition.release()  
    
    thread.join()
    vis.destroy_window()        
    

if __name__ == "__main__":
    main()
    



