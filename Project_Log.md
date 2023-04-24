openpose --image_dir ./out.png --write_keypoint ./ --display 0 --write_images ./


./examples/openpose/openpose --image_dir /workspaces/FastDash/testFiles/testOpenPose/input --write_images /workspaces/FastDash/testFiles/testOpenPose/output --write_json /workspaces/FastDash/testFiles/testOpenPose/output --display 0


# Openpose output explanation
- pose_keypoints_2d: Body part locations (x, y) and detection confidence (c)
- https://cmu-perceptual-computing-lab.github.io/openpose/web/html/doc/md_doc_02_output.html#json-ui-mapping
