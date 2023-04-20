openpose --image_dir ./out.png --write_keypoint ./ --display 0 --write_images ./


./examples/openpose/openpose --image_dir /workspaces/FastDash/testFiles/testOpenPose/input --write_images /workspaces/FastDash/testFiles/testOpenPose/output --write_json /workspaces/FastDash/testFiles/testOpenPose/output --display 0


ARG nvidia_binary_version="515.65.01"
ARG nvidia_binary="NVIDIA-Linux-x86_64-${nvidia_binary_version}.run"
RUN wget -q https://us.download.nvidia.com/XFree86/Linux-x86_64/${nvidia_binary_version}/${nvidia_binary} &&
chmod +x ${nvidia_binary} &&
./${nvidia_binary} --accept-license --ui=none --no-kernel-module --no-questions &&
rm -rf ${nvidia_binary}
RUN rmmod nvidia_drm
RUN rmmod nvidia_modeset
RUN rmmod nvidia_uvm
RUN rmmod nvidia
