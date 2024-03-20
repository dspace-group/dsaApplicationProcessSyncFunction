# Application Process Sync Function
This is a Custom Function for dSPACE ConfigurationDesk to synchronize multiple application processes. 
The base for this Custom Function is interprocess communication via System V message queues. 

## Interface of the ConfigurationDesk Custom Function:

![interface](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/25a40040-f030-4c31-9021-10e293a67125)

The function consists of two ports: <i>Send Trigger</i> and <i>Event Port</i>. For the implementation at least two Simulink Models are needed: one that controls the syncronisation and one or more Models that are being syncronised.  
For each Model that should be syncronised, two instances of the Custom Function should be created. Each instance should have the same paramter **Key**. One of the instances should be placed in the Simulink Model, that controls the syncronisation, the second instance should be placed in the Simulink Model that shall be syncronised. The Simulink Model that controls the syncronisation, should only have <i>Send Trigger</i> Ports, the other Simulink Models should only have <i>Event Ports</i>.


## Integration of the Custom Function

1. Create a directory "CustomFunctions" in your ConfigurationDesk Project directory. Paste the includes of this repo in the newly created directory.
With a right click in the Tab **Functions** of ConfigurationDesk, select "Reload Custom Functions". The Custom Function should appear:

![customfunction](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/0ba42e2d-94e2-4436-a5f8-593d65794082)

2. With a right click on the <i>Application Process Sync</i>  create a new instance of the function by selecting "New" (1). In the **Model-Function** View of ConfigurationDesk drag the unconnected function instance on
the Simulink Model in the **Model-Function Mapping** Tab (2). With a right click on the instance function (Application Process Sync (1)) select Propagade to Simulink Model (3).

![applprocess](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/298d8e3a-4299-4c8b-b03b-cca307c70bf8)->![notpropagated](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/d766ef63-2889-4e7c-916a-1d4bf7a51b34)->![propagated](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/6ae2eef6-cc00-4d7f-9bcd-1a8272516b41)

Note: For each Model that needs to be syncronised, two instances should be created. One instance should be dragged and dropped into the Simulink Model that controls the syncronisation process and the second instance in the Model that shall be syncronised. 

3. For the two corresponding instances created in the step 2, set parameter **Key** in the **Properties** Tab of the Custom Function instances. Corresponding instances should always have the same **Key** parameter.
   
![key](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/152b6793-5476-4d4c-a9ee-edea77368fe3)

4a. The Simulink Model that controls the syncronisation should only have a <i>Send Trigger</i> Port from each Custom Function instance. For that in the **Signal Chain** View with a right click on the generated Model Port Block delete the <i>Event Port</i> from Application. Then manually delete it from Simulink Block.

![twoports](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/87dd0038-20e2-4eef-94b1-c38c4074a0c9)

With the propagation of the function to Simulink Model, a Runnable Task was created for the Event Port. As Event Port is not used in this case, the task can be deleted in the **Tasks** View, with a right click by selecting "Delete from Application"

![task](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/d5562ad0-7352-4855-9cb6-d93abcf92c19)


4b. The Simulink Models that are being syncronised should only have <i>Event Port</i>. Similarly to step 4a, delete <i>Send Trigger</i> Model Port Block and corresponding Simulink Block. Created Runnable Task should not be deleted in this case. 

5a. In the Simulink Model controlling the syncronisation, <i>Send Trigger</i> should be connected to a constant block, which will be set from ControlDesk. Only when the value of the constant block is **1**, the syncronisation is activated. If there are two models that should be syncronised, the same constant block shall be used: 

![model_send](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/5492e3e4-4081-4514-b622-e51da0306525)


5b. In the Simulink Models that should be syncronised, <i>Event Port</i> should be connected to a **Function-Call Subsystem**, which can be found in the Simulink library. The behavioural model should be placed into this subsystem and will be active only when the <i>Event Port</i> is triggered.

![model_rcv](https://github.com/dspace-group/dsaApplicationProcessSyncFunction/assets/163404282/1bdcc54c-19d3-44eb-84b6-026c0cb74b62)





