# ML modules and Object tracking
## Overview
- Object Detection Module, responsible for detecting objects in each frame, calculating the distance, tracking objects and sending results to proritiser.

## Documentation
### Components
- ml_interface.hpp: object detection network interfaces
- caffe_impl.cpp & yolo_impl.cpp:  Implementation of Caffe-based network (MobileNetSSD) and darknet-based network (YOLOv3) respectively
- model_helper.hpp & model_creator.hpp: Header files containing simple functions for MobileNetSSD network and YOLOv3 network implementation
- object_tracking.hpp & object_tracking.cpp: Object tracking system to track recognised objects and compute their appearance count and speed.

### Design Decisions
The Caffe-based Network (MobileNetSSD) is used as the primary network due in the system to its good performance. The darknet-based network (YOLOv3) is also implemented here as the second option. Harr Cascade Classifiers was considered but not implemented and used because training-related functions are not included in opencv4.

Three algorithms for depth detection are tested. The mean depth is not accurate as it calculates the average depth of both object and background. Contours detection is affected by zero-depth in the depth frame. While the k-means algorithm takes more time, it can distinguish the object from the background and get a more accurate distance.

The increase in resolution results in a lower frame-rate, but the model can detect the objects at longer ranges. The multiple preset themes with different resolution configuration are used for various scenarios.


### Limitations
- Poor performance when the image resolution is very high.
- High requirements for the training dataset for object detection network (a large set of labelled images should be used to train the networks)
- Difficulty when defining the classes of the objects when preparing training dataset (e.g. cups vs. mugs vs. glass cups).
- The zero-depth area in depth frame may affect the accuracy of the depth detection. 
- The depth detection algorithm may use depth of background as object distance for some particular objects, as these objects occupy the small proportion of the object area
- The trade-off between the accuracy and efficiency of the object tracking.
### Visuals and diagrams
![bottle_new](https://user-images.githubusercontent.com/39096904/93452877-db0a6900-f8d0-11ea-909c-4b73c53b1967.png)

## Development Manual
* The object detecting process is in a linear fashion. More research can be done to investigate other approaches that may help improve the performance.

### Requirements
- OpenCV
- TBB
- SPDLOG (for logging)

### Future development
* Improve the performance of the object detection module (executing with GPU; applying OpenVINO etc.).
* Train more object detection models with higher accuracy.
* Design tracking strategies to improve the performance of object tracking.
