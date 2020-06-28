#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Moving the robot to the ball");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the command_robot service and pass the velocities to it to drive the robot
    if (!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    ROS_INFO("Image Height : %d", img.height);
    ROS_INFO("Image Width  : %d", img.width);
    ROS_INFO("Image Step   : %d", img.step);
    uint8_t white_pixel = 255;
    uint32_t white_pos = 0;
    bool is_white = false;

    // Check channels count
    // If the image contains RGB channles, this channel_count shoud be 3.
    uint8_t channel_count = (int)(img.step / img.width);

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    for (uint32_t pixel = 0; pixel < img.height * img.width; pixel++) 
    {
        // Check all channels
        for (uint8_t i = 0; i < channel_count; i++)
        {
            // If the pixel is not 255, exit loop and go to next pixel
            if (img.data[pixel * channel_count + i] != white_pixel) 
            {
                break;
            }
            // If all channels is 255, make is_white flag true
            if (i == channel_count - 1)
            {
                white_pos = pixel % img.width;
                is_white = true;
                ROS_INFO("Found Ball!");
                ROS_INFO("Array   count : %d", pixel * channel_count + i);
                ROS_INFO("Pixel   count : %d", pixel);
                ROS_INFO("i       count : %d", i);
                ROS_INFO("white_pos     : %d", white_pos);
            }
        }
        if (is_white)
        {
            break;
        }
    }
    

    if (is_white)
    {
        // White ball is at left in image
        if (white_pos < img.width / 3.0)
        {
            drive_robot(0, 0.5);
        // White ball is at right in image
        }else if (white_pos > img.width * 2.0 / 3.0)
        {
            drive_robot(0, -0.5);
        }else
        {
            drive_robot(0.5, 0);
        }
    }else
    {
        drive_robot(0, 0);
    }

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}