# CMake generated Testfile for 
# Source directory: /Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/modules/flann
# Build directory: /Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv_build/modules/flann
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_flann "/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv_build/bin/opencv_test_flann" "--gtest_output=xml:opencv_test_flann.xml")
set_tests_properties(opencv_test_flann PROPERTIES  LABELS "Main;opencv_flann;Accuracy" WORKING_DIRECTORY "/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv_build/test-reports/accuracy" _BACKTRACE_TRIPLES "/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/cmake/OpenCVUtils.cmake;1763;add_test;/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/cmake/OpenCVModule.cmake;1352;ocv_add_test_from_target;/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/cmake/OpenCVModule.cmake;1110;ocv_add_accuracy_tests;/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/modules/flann/CMakeLists.txt;2;ocv_define_module;/Users/lukehobeika/Desktop/Desktop/Projects/Harsha_Research_Lab/FastDash/thirdParty/opencv/modules/flann/CMakeLists.txt;0;")
