# UROSWorldControl
Unreal World Control from ROS using UROSBridge.

Supported engine version: **UE 4.27**

## Features
Currently there are the following ROS services supported:
* spawn_model - Spawns a StaticMeshComponent in the world with given material at given location, rotation and with given UTags. Note: The uasset files are expected to have the Prefixs 'SM_' (StaticMesh) and 'M_' (Material)
* set_model_pose - Sets an Actor to given location and rotation.
* delete_model - Destroys an Actor with from Unreal-World.
* highlight_model - Highlights an Actor in the Unreal-World.
* set_material - Sets the Materials of an Actor.
* set_physics_properties - Set the physics properties of an Actor.
* spawn_physics_constraint - Spawns a physics constraint. (will probably be reworked in the future)

## Installation
### Unreal
For this Plugin to work on the unreal side of things, you also need to the Plugins [UROSBridge](https://github.com/robcog-iai/UROSBridge) and [UUTils/UTags](https://github.com/robcog-iai/UUtils). Copy all three plugins into your project Plugin folder and activate them.

For the Highlighting to work, you need to setup three things in your unreal project.
1) Got to `Edit` -> `Project Settings` under `Rendering` -> `Postprocessing` you need to set `Custom Depht-Stencil Pass` to `Enable with Stencil`.
2) Place a `PostProcessVolume` in your unreal world. You need to scale it so it big enough to cover everything, or check the Infinite Extend box in its settings.
3) In the `PostProcessVolume` you need to add the `Outline_M_Inst` to the `PostProcessMaterials`.

### ROS
On the ROS side you need the srv and msg files, for that you need to get the package `unreal_msgs`, which can be found in the [unreal_ros_pkgs](https://github.com/robcog-iai/unreal_ros_pkgs) repository.

## Usage
After starting the editor should have an extra mode tab, within this tab the server address of the ROSBridgeServer needs to be set. In order to register the services of this plugin, also add 'RWCManager' to the Publisher List in the 'Callbacks' section of this mode tab.  After setting the adress and the publisher list acordingly, pressing the `ConnectToRosBridge`-Button will connect and publish the services listed above.
