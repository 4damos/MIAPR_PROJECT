#include <ros/ros.h>
#include <visualization_msgs/Marker.h>


int main( int argc, char** argv )
{
  ros::init(argc, argv, "points");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // SHAPE == CUBE
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // SET FRAME ID
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();


 // FIRST MARKER     ##       START_MARKER
    // SET INDIVIDUAL ID / NAME
    marker.ns = "START_MARKER";
    marker.id = 0;

    // SHAPE SET
	marker.type= shape;

    // ADD MARKER
    marker.action = visualization_msgs::Marker::ADD;

    // SET COORDS OF MARKER
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // SET SCALE -- 1x1x1 = 1m EACH SIDE
    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 8.0;

    // SET COLOR
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;
	marker_pub.publish(marker);
	
	
	 // SECOND MARKER     ##       GOAL MARKER 
    marker.ns = "GOAL_MARKER";
    marker.id = 1;

    marker.type = shape;

  
    marker.action = visualization_msgs::Marker::ADD;

 
    marker.pose.position.x = -6;
    marker.pose.position.y = -3;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;


    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 8.0;


    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // PUBLISH
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);

    r.sleep();
  }
}