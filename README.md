# FastDash

FastDash enables users to do the following

- Read delta encoded depth frames from disk
- Reassemble those delta encodings into full depth frame data
- Convert that frame data into a point cloud
- Stitch those point clouds together to form a volumetric video
- Run a pose estimation algorithm on the stitched together point clouds

openpose is located at the /openpose folder. When running an openpose commands make sure that you are in this folder so the runtime enviornment is correct.

Openpose useful flags
-display 0 for no display
-fps_max to tell openpose to process frames as quickly as possible
-image_dir process a directory of images. Specify the directory location.
-profile_speed openpose will show some runtime statistics at a frame number
-write_images directory to write rendered frames to


Command for 3d pose reconstruction - assumes that there is only one person in the frame
./build/examples/openpose/openpose.bin --video examples/media/video.avi -display 0 -write_images ./build/output -3d --number_people_max 1

Command for 2d pose construction
./build/examples/openpose/openpose.bin --video examples/media/video.avi -display 0 -write_images ./build/output

Libpng is included at
/usr/include/libpng

# Makefil notes
Makefile allows you to specify targets that are only created when their dependencies are newer
When you add a new cpp file for a header, make sure to add it to the object files list in the makefile
Also the rule in the makefile is that each hdr file has at most one cpp file that needs to be recompiled