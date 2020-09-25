# 项目介绍

### 1、文件结构

目前完成的内容包含了6种节点，仿真环境以及需要的ros message

6种节点包含：控制节点、决策节点、感知节点、服务节点、资源节点和任务节点

仿真环境基于ros stage实现。

ros message包含通讯所需msg，service and action三种通讯消息。

### 2、目前可实现功能

可以运行决策树，结合规划服务、路径跟随以及方针，能够实现四车的集结调度。

### 3、节点情况

###### （1）资源节点

​		已经完成资源节点编写，均未测试。

###### （2）控制节点

​		遥控节点目前可以遥控仿真小车，实车未测试。

​		路径跟踪节点目前可以实现仿真小车控制，实车未测试。

###### （3）服务节点

​		可以实现基于A star算法的路径规划。	

​		可以实现简单的散点算法	

###### （4）决策节点

​		实现基于ICRA框架的决策树，目前只有集结和等待任务。		

###### （5）任务节点

​		可以实现仿真小车的集结任务。

###### （6）感知节点

​		完成定位节点编写，未测试。

###### （7）通讯节点

​		目前通讯节点完成了上位机交互初步功能。

### 4、下一步工作

###### （1）加入感知功能

###### （2）继续完善决策任务

### 5、代码提交记录

###### 2020年9月11日提交     

（1）加入散点算法 （2）加入上位机交互的通讯节点 （3）精简仿真代码

###### 2020年9月21日提交

（1）加入costmap  （2）加入icra中基于teb实现的local_planner，但是目前存在问题





