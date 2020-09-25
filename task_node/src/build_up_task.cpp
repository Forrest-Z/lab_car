#include "build_up_task.h"

BuildUpTask::BuildUpTask()
    :build_up_action(nh,"build_up_action",boost::bind(&BuildUpTask::BuildUpExcuteCB,this,_1),false),
    task_state(ActionState::PENDING),plan_action(nh,"move_base",true)
{
    car_id = 1;
    build_up_action.registerPreemptCallback(std::bind(&BuildUpTask::BuildUpPreemptCB,this));
    statue_sub_ = nh.subscribe("move_base/status",10,&BuildUpTask::MoveBaseStatusCB,this);
    build_up_action.start();
    ROS_INFO("build up task initialized");
}

void BuildUpTask::BuildUpPreemptCB(){
    if(build_up_action.isPreemptRequested()){
        plan_action.cancelGoal();
        result.succeed = false;
        build_up_action.setAborted(result,"goal cancel");
    }
}

void BuildUpTask::BuildUpExcuteCB(const robot_msgs::BuildUpGoalConstPtr &goal){
    //set goal
    ROS_INFO("get goal");
    geometry_msgs::Pose origin_goal;
    goal_point = goal.get()->goal;
    origin_goal = goal_point;
    float temp[4][2] = {{-1,-1},{-1,1},{1,1},{1,-1}};
    goal_point.position.x += temp[car_id-1][0] * 0.5;
    goal_point.position.y += temp[car_id-1][1] * 0.5;
    ROS_INFO("target position is [%f,%f],the car number is %d, build up target position is [%f,%f]",origin_goal.position.x,origin_goal.position.y,car_id,goal_point.position.x,goal_point.position.y);
    move_base_msgs::MoveBaseGoal planning_goal;
    planning_goal.target_pose.header.frame_id = "map";
    planning_goal.target_pose.header.stamp = ros::Time().now();
    planning_goal.target_pose.pose = goal_point;
    plan_action.sendGoal(planning_goal);
    ros::Rate loop(10);
    loop.sleep();
    while(ros::ok() && build_up_action.isActive()){
        ros::spinOnce();
        if(task_state == ActionState::SUCCEEDED){
            result.succeed = true;
            ROS_INFO("build up task finished");
            build_up_action.setSucceeded(result,"goal_reached");
            break;
        }
        else if(task_state == ActionState::ABORTED){
            result.succeed = false;
            ROS_INFO("build up task error");
            build_up_action.setAborted(result,"task failed");
            break;
        }
        else{
            robot_msgs::BuildUpFeedback feedback;
            feedback.error_occured = false;
            build_up_action.publishFeedback(feedback);
        }
        loop.sleep();
    }
    task_state = ActionState::PENDING;
    
}
/* 以下是planning move_base的callback */
void BuildUpTask::MoveBaseStatusCB(const actionlib_msgs::GoalStatusArrayConstPtr &msg){
    if(msg->status_list.empty()){
        task_state = ActionState::PENDING;
        return ;
    }
    int state = msg->status_list.begin()->status;
    switch(state){
        case ActionState::SUCCEEDED : task_state = ActionState::SUCCEEDED;break;
        case ActionState::PENDING : task_state = ActionState::PENDING;break;
        case ActionState::ACTIVE : task_state = ActionState::ACTIVE;break;
        default :task_state = ActionState::ABORTED;break;
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "build_up_task");
    BuildUpTask build_up_task;
    ros::Rate loop(10);
    while(ros::ok()){
        loop.sleep();
        ros::spinOnce();
    }
    return 0;
}
