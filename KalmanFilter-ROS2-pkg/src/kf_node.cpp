#include "kf_node.h"

KFNode::KFNode(const std::string & node_name, const std::string & node_namespace) : rclcpp::Node(node_name, node_namespace), bram1(1, 400), bram2(2,400) {

  // Custom code here to initialize BRAM and xkalmanfilterkernel
	XKalmanfilterkernel_Initialize(&kf_kernel, device_id); //to be updated
  // ...

  // Initialize subscribers
  pos_meas_sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
    "/sensor/pos_measurement", 
    10, 
    std::bind(&KFNode::pos_meas_callback, this, std::placeholders::_1)
  );
  control_input_sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
    "/controller/control_input", 
    10, 
    std::bind(&KFNode::control_input_callback, this, std::placeholders::_1)
  );

  // Initialize publishers
  pos_est_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/kf/pos_est", 10);

}

KFNode::~KFNode() {
  // Custom code here to close BRAM and xkalmanfilterkernel
  XKalmanfilterkernel_Release(&kf_kernel);
  // ...
}

void KFNode::pos_meas_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
  pos_t pos_meas;
  pos_meas.x = msg->data[0];
  pos_meas.y = msg->data[1];
  pos_meas.z = msg->data[2];
  pos_meas_queue.push(pos_meas);

  // Custom code here to possibly call Kalman filter if both queues are not empty

  if (!control_input_queue.empty() && XKalmanfilterkernel_IsDone(&kf_kernel)){

    uint32_t *datain = bram1.GetPointer();
    datain[0] = pos_meas_queue[0];
    datain[1] = pos_meas_queue[1];
    datain[2] = pos_meas_queue[2];

    XKalmanfilterkernel_Start(&kf_kernel);
  }
  // ...

}

void KFNode::control_input_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
  acc_t control_input;
  control_input.ax = msg->data[0];
  control_input.ay = msg->data[1];
  control_input.az = msg->data[2];
  control_input_queue.push(control_input);

  // Custom code here to possibly call Kalman filter if both queues are not empty
  if (!pos_meas_queue.empty() && XKalmanfilterkernel_IsDone(&kf_kernel)){

    uint32_t *datain = bram1.GetPointer();
    datain[3] = control_input_queue[0];
    datain[4] = control_input_queue[1];
    datain[5] = control_input_queue[2];

    XKalmanfilterkernel_Start(&kf_kernel);
  }
  // ...

}

void KFNode::publish_pos_est(pos_t pos_est) {
  geometry_msgs::msg::PoseStamped pos_est_msg;
  pos_est_msg.header.stamp = this->get_clock()->now();
  pos_est_msg.header.frame_id = "world";
  pos_est_msg.pose.orientation.w = 1.0;
  pos_est_msg.pose.orientation.x = 0.;
  pos_est_msg.pose.orientation.y = 0.;
  pos_est_msg.pose.orientation.z = 0.;

  uint32_t *dataout = bram2.GetPointer();
  pos_est.x = dataout[0] 
  pos_est.y = dataout[1] 
  pos_est.z = dataout[2] 

  pos_est_msg.pose.position.x = pos_est.x;
  pos_est_msg.pose.position.y = pos_est.y;
  pos_est_msg.pose.position.z = pos_est.z;
  pos_est_pub_->publish(pos_est_msg);
}

int main(int argc, char ** argv)
{
	rclcpp::init(argc, argv);
    auto node = std::make_shared<KFNode>();

    rclcpp::spin(node);

	rclcpp::shutdown();
  return 0;
}
