# Application Process Sync Function
This is a Custom Function for dSPACE ConfigurationDesk to synchronize multiple application processes. 
The base for this Custom Function is interprocess communication via System V message queues. 

## Interface of the ConfigurationDesk Custom Function:
![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/902f19ba-36bb-4853-85fd-6c0f0b531aa6)

The function consists of two ports: <i>Send Trigger</i> and <i>Event Port</i>. For the implementation at least two Simulink Models are needed: one that controls the syncronisation and one or more Models that are being syncronised.  
For each Model that should be syncronised, two instances of the Custom Function should be created. Each instance should have the same paramter **Key**. One of the instances should be placed in the Simulink Model, that controls the syncronisation, the second instance should be placed in the Simulink Model that shall be syncronised. The Simulink Model that controls the syncronisation, should only have <i>Send Trigger</i> Ports, the other Simulink Models should only have <i>Event Ports</i>.


## Integration of the Custom Function

1. Create a directory "CustomFunctions" in your ConfigurationDesk Project directory. Paste the includes of this repo in the newly created directory.
With a right click in the Tab **Functions** of ConfigurationDesk, select "Reload Custom Functions". The Custom Function should appear:
   
   ![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/20d9926b-f080-4819-b417-545f4aeb4623)

2. With a right click on the <i>Application Process Sync</i>  create a new instance of the function by selecting "New" (1). In the **Model-Function** View of ConfigurationDesk drag the unconnected function instance on
the Simulink Model in the **Model-Function Mapping** Tab (2). With a right click on the instance function (Application Process Sync (1)) select Propagade to Simulink Model (3).

   ![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/f8e6b56c-b07b-4aac-97b7-c7c55e534b65) -> ![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/d8c9a889-7b45-4186-b9a9-7785d1278f4a) -> ![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/67fb0c60-54a3-4fd4-902f-9ef26065a382)

Note: For each Model that needs to be syncronised, two instances should be created. One instance should be dragged and dropped into the Simulink Model that controls the syncronisation process and the second instance in the Model that shall be syncronised. 

3. For the two corresponding instances created in the step 2, set parameter **Key** in the **Properties** Tab of the Custom Function instances. Corresponding instances should always have the same **Key** parameter.
![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/262b59c4-3277-46ac-b5c1-3c0c4d398a5c)

4a. The Simulink Model that controls the syncronisation should only have a <i>Send Trigger</i> Port from each Custom Function instance. For that in the **Signal Chain** View with a right click on the generated Model Port Block delete the <i>Event Port</i> from Application. Then manually delete it from Simulink Block.

![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/e857dfdb-78e8-4e2d-829f-6e4882b24fb5)

With the propagation of the function to Simulink Model, a Runnable Task was created for the Event Port. As Event Port is not used in this case, the task can be deleted in the **Tasks** View, with a right click by selecting "Delete from Application"
![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/d2cdf2cb-e458-41b8-9f69-d945180f008a)

4b. The Simulink Models that are being syncronised should only have <i>Event Port</i>. Similarly to step 4a, delete <i>Send Trigger</i> Model Port Block and corresponding Simulink Block. Created Runnable Task should not be deleted in this case. 

5a. In the Simulink Model controlling the syncronisation, <i>Send Trigger</i> should be connected to a constant block, which will be set from ControlDesk. Only when the value of the constant block is **1**, the syncronisation is activated. If there are two models that should be syncronised, the same constant block shall be used: 

![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/cce07c69-ddb9-4eb7-8689-96431bb2fd41)

5b. In the Simulink Models that should be syncronised, <i>Event Port</i> should be connected to a **Function-Call Subsystem**, which can be found in the Simulink library. The behavioural model should be placed into this subsystem and will be active only when the <i>Event Port</i> is triggered.

![image](https://github.com/alinamamulat/IPC_Event/assets/163404282/2a5c312b-95ac-4a6f-a94f-eb9fbee1cc2d)




