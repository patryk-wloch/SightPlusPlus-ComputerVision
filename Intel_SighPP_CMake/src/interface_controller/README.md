# Interface Controller
## Overview

This module contains and handles all outputting needs of the software. It contains both the output stream windows for testing and development and the controller and implementations that connect to the user interfaces.

## Documentation
### Components

* Output Stream Controller
  * Only used for debug and testing. Displays the camera view to the developer. Enabled with `-color` and `-depth` flags on startup.
  * Contains function that determines if the main loop should run again. `should_receive_new_frames()` is true if OpenCV stream output windows can receive frames, it is also true if they are not enabled.
* API Controller
  * Manages connected output API users, not User Interfaces but connectors to the user interfaces.
  * New output API implementations need to be added as a user using the `add_user(ApiUserInterface& user)` method.
  * Items that should be sent to user interfaces are sent to the `new_items` method, where they are sent to each of the implementations `new_item` function.
* API User Interface
  * Has one required method `new_item`. Implementations should send the new items to their connected user interfaces.
* API WebSocket Implementation
  * Based on WebSocket++ Simple Broadcast Server. Runs on seperate thread.
  * Sends items if priority is high enough, the minimum priority is set in the constructor.

### Design Decisions

* Output Stream Windows
  * Currently houses the method for deciding if the program should continue running, `should_receive_new_frames`. 
  * Due to OpenCV limitations they are and must run on the main thread. The performance hit is acceptable due to this only being meant for testing and development.
* API Controller
  * Uses `tbb::parallel_for_each` to send items concurrently to connected API users.
* API WebSocket Implementation
  * Runs in a seperate thread, this is required as the WebSocket++ server blocks the thread with an endless-loop.

### Limitations

* API User Interface
  * Try to limit using implementations that require a seperate thread. 

### Visuals and diagrams

N/A

## Development Manual

* The API User Interface may change in the future. A method of two-way communication is in the works.

### Component Developemnt

* To create new Output API implementations, it is only necessary to implement the API User Interface and register with the API Controller during the program startup, in `Sight++.cpp`

### Requirements

* Currently the Output API uses OpenCV, TBB, and WebSocketPP.

### Future development

* The completion of two-way communication is in the works. That will change the API User Interface and the WebSocket++ implementation.
