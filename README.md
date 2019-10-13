# canny_edge_my_face
Subcribes to images from cv_camera node and applies canny edege detector. It then publishes those images on "canny_image" topic

## Execution
```
roscore
rosrun canny_edge_my_face canny_edge_my_face_node 
rosrun cv_camera cv_camera_node 

