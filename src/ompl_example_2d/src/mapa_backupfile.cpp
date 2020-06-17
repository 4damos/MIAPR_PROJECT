#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl/point_types.h>
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud2.h"
#include <fstream>
#include "shape_msgs/Mesh.h"
#include "shape_msgs/MeshTriangle.h"
#include "visualization_msgs/Marker.h"

// std_msgs int8
#include "std_msgs/Int8MultiArray.h"

#include <pcl_conversions/pcl_conversions.h> //powoduje blad
#include <pcl/octree/octree_pointcloud.h>



int main(int argc, char** argv)
{
  //direct path to .by map file
  std::string inputFilename = "/home/adam/catkin_ws_projekt/src/ompl_example_2d/src/fr_078_tidyup.bt";

  //mapa zewnetrzna res = 0.05m
  //inputFilename = "/home/ubuntu/MAPR_projekt_catkin/src/projekt_4/src/fr_078_tidyup.bt";

  std::cout << "\nReading OcTree file\n===========================\n";
  std::ifstream file(inputFilename.c_str(), std::ios_base::in |std::ios_base::binary);

  if (!file.is_open()){
    OCTOMAP_ERROR_STR("Filestream to "<< inputFilename << " not open, nothing read.");
    std::cout << "error -1" << std::endl;
    exit(-1);
  }

  //mapa zewnetrzna
  octomap::OcTree tree(0.05); //res = 0.05 m
  double map_res = 0.05;
  tree.readBinary(file); //wczytaj mape z pliku

  //inicjalizacja node'a
  ros::init(argc, argv, "PROJEKT");
  ros::NodeHandle nh;
  ros::Rate loop_rate(10);


  //create new pointCloud - PointXYZ
  pcl::PointCloud<pcl::PointXYZI>::Ptr pointCloud(new pcl::PointCloud <pcl::PointXYZI>);
  std::cout << "pointCloud.size() = " << pointCloud->size() << std::endl;


  // wczytaj mape z pliku do pointCloud formatu PCL
  for(octomap::OcTree::leaf_iterator it = tree.begin(), end=tree.end(); it!= end; ++it)
  {
    if(tree.isNodeOccupied(*it)){

      double size = it.getSize();
      //write position to PointXYZ
      pcl::PointXYZI punkt;
      punkt.x = it.getX();
      punkt.y = it.getY();
      punkt.z = it.getZ();
      punkt.intensity = it.getDepth();

      //push pointXYZ to PointCloud
      pointCloud->push_back(punkt);
    }
  }


  // Zamiana PCL -> ROS MSG
  // Declare message to publish (http://docs.ros.org/melodic/api/sensor_msgs/html/msg/PointCloud2.html)
  sensor_msgs::PointCloud2 cloudToPublish;

  // Convert created point cloud to ROS message
  pcl::toROSMsg(*pointCloud , cloudToPublish);

   //nazwa frejma
  cloudToPublish.header.frame_id = "map";

  // Advertise topic with filtered data - used to publish on a topic
  ros::Publisher filteredPointClouddPub;

  filteredPointClouddPub = nh.advertise<sensor_msgs::PointCloud2>("/octomap", 1000);
  // Publish message

  ros::Rate r(10); // 10 hz

  while (ros::ok())
  {


    filteredPointClouddPub.publish(cloudToPublish);

    r.sleep();
  }

  return 0;
}
