#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include "std_msgs/String.h"
#include <sstream>
#include <nav_msgs/Path.h> 

  // PATH PLANNER CALLBACK
 /*
nav_msgs::Path plannedPath;

    void planned_pathCallback(nav_msgs::Path::ConstPtr& msg)
    {
         int i=0;
          for(std::vector<geometry_msgs::PoseStamped>::const_iterator it= msg->poses.begin(); it!= msg->poses.end);  ++it)  
          {

              //gPlan.poses[i] = *it;
              i++;
           }
      }
*/
int main (int argc, char** argv){
  ros::init(argc, argv, "ros_run");
  ros::NodeHandle n;
  
  // SUBSCRIBER PATH PLANNER
 //ros::Subscriber planSub = n.subscribe("planned_path", 1000, planned_pathCallback);
 
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50); 
  tf::TransformBroadcaster odom_broadcaster;

  //POSE START
  double x = 0.0;
  double y = 0.0;
  double th = 0.0;
  //VELOCITY  
  double vx = -0.1;
  double vy = -0.1;
  double vth = 0.0;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();
  
   //REFRESH RATE
  ros::Rate r(10.0);
  while(n.ok()){


// CHECK IF MESSAGE SENT
    ros::spinOnce();               
    current_time = ros::Time::now();
	
    //COMPUTE DISTANCE BY VELOCITY
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

	
    //QUATERION FROM YAW
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //PUBLISH TRANSFORM OVER TF
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time; 

	//SET ODOM TRANSFORM
    odom_trans.header.frame_id = "map";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //SEND TRANSFORM
    odom_broadcaster.sendTransform(odom_trans);

    //PUBLISH ODOMETRY
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
	

   /*
/// PATH EXTRACTION
	nav_msgs::Path Planner2D::extractPath(ob::ProblemDefinition* pdef){
    nav_msgs::Path plannedPath;
    plannedPath.header.frame_id = "/map";
    // get the obtained path
    ob::PathPtr path = pdef->getSolutionPath();
    // print the path to screen
    path->print(std::cout);
    // convert to geometric path
    const auto *path_ = path.get()->as<og::PathGeometric>();
    // iterate over each position
    for(unsigned int i=0; i<path_->getStateCount(); ++i){
    // get state
     const ob::State* state = path_->getState(i);
    // get x coord of the robot
     const auto *coordX =
    state->as<ob::CompoundState>()->as<ob::RealVectorStateSpace::StateType>(0);
        // get y coord of the robot
    const auto *coordY =
    state->as<ob::CompoundState>()->as<ob::RealVectorStateSpace::StateType>(1);
    const auto *coordZ =
    state->as<ob::CompoundState>()->as<ob::RealVectorStateSpace::StateType>(2);
	
	*/

       
    //SET POSE
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //SET VELOCITY
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //PUBLISH MESSAGE
    odom_pub.publish(odom);

    last_time = current_time;
	
    r.sleep();
  }
}