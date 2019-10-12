#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
///

using namespace cv;
namespace enc = sensor_msgs::image_encodings;
image_transport::Subscriber image_sub;
image_transport::Publisher image_pub;

cv_bridge::CvImage pimg;
sensor_msgs::Image pmsg;


Mat src, src_gray;
Mat dst, detected_edges;

int threshold_1=40;
int threshold_2=150;
int kernel_size = 3;

void imageCallback(const sensor_msgs::ImageConstPtr img)
{

    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(img, enc::BGR8);
    }
    catch (cv_bridge::Exception &e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    src=cv_ptr->image;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    blur( src_gray, detected_edges, Size(3,3) );

    Canny( detected_edges, detected_edges, 50, 100, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);

    ros::Time time = ros::Time::now();


    pimg.header.stamp = time;
    pimg.header.frame_id = "image";
    pimg.encoding = "bgr8";
    pimg.image=dst;


    pimg.toImageMsg(pmsg);
    image_pub.publish(pmsg);

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    image_transport::ImageTransport it(n);

    image_sub=it.subscribe("/cv_camera/image_raw",1,imageCallback);
    image_pub=it.advertise("canny_image",10);

    ros::spin();

    return 0;
}