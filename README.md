# Intel_IXN_SightPP
MSc IXN Group Thesis in collaboraiton with Intel
## Introduction
Digital health has become an increasingly important and cutting edge field, opening a wealth of innovations and research into medical assistive devices (MADs) with assistance from advancing technologies such as Artificial Intelligence(AI). Broadly speaking AI has two dominating categories, heuristic or rule-based Expert Systems (ES) and Machine Learning (ML) systems. Development and improvements of power and performance in ML algorithms has further driven its significant relevance in medical assistance. Models trained with supervised learning are being used in medical imaging (such as cancer detection), predictive diagnosis (such as for chronic diseases) and IV drip automation. Likewise, ES's are also found in healthcare such as in differential diagnosis. ES’s are also often used in navigation such as in autonomous cars. For example, the Responsibility-Sensitive Safety net by Intels’ Mobileye.  MADs are tools that help a person carry out day-to-day activities. These include devices such as hearing aids, walking canes, and glasses. This project conducts novel development of an assistive device in collaboration with Intel, AstraZeneca, UCL Computer Science, the UCL Interaction Centre (UCLIC) and the UCL Global Disability Innovation Hub. The project finalised two objective groups to outline the requirements of the project:

To research, design, and develop a new modular service platform, seeking simplicity, customisabiliy, and extensibility. This uses an open architecture consisting of components that: Process RealSense camera input, run inference classification, analyse the data in an ES module, and push the resultant data to compatible interfaces via an interface controller module. The system has two external components, including front-end interfaces and the input camera. This system input is from the supplied Intel D435 Camera after research on the most effective camera.

Following this we utilise this new platform to build a proof of concept (PoC) system and create a new MAD  to assist a visually impaired user. This system would use the platform to classify and analyse objects in front of the blind user and relay that environmental information, while also integrating obstacle avoidance. The PoC is designed to work as an alternative to other assistive systems such as canes or guide dogs. There are a few products that are similar to this PoC. However, this project aims to remove limitations from other products such as allowing novel route exploration and reducing cognitive loads on the user. This PoC aims to use the novel architecture to create an assistive system, giving the user awareness of their environment, increasing independence in known environments and allowing for exploration of unknown environments. This proof of concept has been named \textbf{Sight++}.
## Overview


The platform is modular and uses interfaces for standardised communication. The interfaces are, in C++ terms, abstract classes with virtual functions. This leads to independent implementations where the implementations do the work as needed. Their only requirement is to fulfil the interface contract and register as an interface user with the relevant controller.

OpenCV is a central part of the system. Each frame is received from the RealSense camera, transformed to an OpenCV matrix. While the current system only supports Intel RealSense cameras, it would be trivial to implement connections to other cameras, given that they support a frame to OpenCV matrix transformation.

The classification frameworks must work with OpenCV matrices as input and output their results as standardised classification items containing information such as the name, its location, and the distance. These classification items get sent to the prioritiser modules. The prioritiser interface expects the implementing classes to assign a priority to each item. These prioritised items are sent to the output controller which sends the information to all connected output API users.

This type of interface architecture with independent implementation does have drawbacks. It can be detrimental to the performance to the system as there are no limitations or extra requirements other than implementing the relevant interface. This can be worrisome with regards to performance and processing time, as a single badly performing or incorrectly configured service can bring the whole system to a standstill.

Due to technical limitations, the main parts of the system are run in a linear fashion, from getting input from the cameras to machine learning classification to prioritising and outputting the data to the interfaces. Only certain processes and functions are run in parallel or in separate threads. Computationally-heavy tasks, such as the machine learning classification, are done on the main-thread, though the classification models do work in parallel. This is done to make sure that each frame is worked on from start to finish before a new one is retrieved, to make sure that the information that is sent to the user is as current as can be. Another way to perform these steps is to perform the work in such a way that a new frame is fetched every x milliseconds and sent to the machine learning classifiers. The classifiers then perform their calculations independently and send their results to the user. However, such an architecture could lead to cases where classifiers compute at different speeds leading to situations where data from different frames are being sent out of order, leading to erroneous assistance being rendered.

The platform allows testing windows through flagging, however this testing system runs on the same thread. This is due to a lack of expertise with multi-threading in C++ and OpenCV, and more importantly, due to multiple responses on the OpenCV forums advising against such efforts. We decided that the potential performance gains were not worth the effort required to implement it.

Only one part in the current release runs in its own separate thread. The WebSocket implementation of the interface API runs in and requires a thread for itself. This is done to make sure that connections between interfaces, like the example implementation we developed, and the main system, is kept alive. This is a result of how the WebSocket++ implements its server components.

## Core modules
Below are links to the readme documentation and manuals of each component:
- [Interface Controller](Intel_SighPP_CMake/Intel_SighPP_CMake/src/interface_controller)
- [Prioritiser](Intel_SighPP_CMake/Intel_SighPP_CMake/src/priority_lib)
- [ML and Object tracking](Intel_SighPP_CMake/Intel_SighPP_CMake/src/ml_lib)
- [Main Class and Service Controller](Intel_SighPP_CMake/Intel_SighPP_CMake/src/)
## Installation and Development
### Packages required
This project uses [Vcpkg](https://github.com/Microsoft/vcpkg) for library dependencies. Install the following libraries with Vcpkg to run the solution (make sure to install the 64-bit versions, as this system is being developed with 64-bit systems in mind):

* realsense2
* opencv3
* opencv3[contrib]
* tbb
* websocketpp
* spdlog
* GTest


### Installation and Run Manual of CMAKE
This section is an instruction guide on how to install and run CMake. This will include a guide for building in windows power shell as well as within Microsofts Visual Studio 2019. Running the system with CMake has the same requirements as with the original version. This guide includes the use of vcpkg, and it is assumed that CMake is installed on your system.

#### Open with VS
- Startup visual studio and select "Open local Folder".
- Navigate inside /Intel_SighPP_CMake and click select folder.
- Rightclick on the top-level CMakeLists.txt and select CMake Settings. This will open a new JSON page to instruct VS on how to build the system.
- Following, Add the path to your vcpkg.cmake file to "CMake Toolchain file". This is roughly "<vcpkg path>/scritps/buildsystems/vcpkg.cmake".
- You will now be able to compile the system given you have installed everything correctly.
- Rightclick on the top-level CMakeLists.txt and click Generate cache. This will build the base build instructions for the CMake system.
- Finally, to build the executable under the build tab select rebuild all.
- The executable files will now be within the ./build directory.

#### Running the software within VS
- Now that you have built the CMake, you can run the system in debug mode from VS.
- Note: You may need to copy the models folder to the location of the executable as this is needed, especially if you changed the build directory.
- There are a few options.
- Option 1, target view:
  - In this method, first, navigate to the solution explorer to the right to the home icon.
  - Right-click the "switch view" icon and select the CMake target view.
  - You will now see the executables of the project.
  - To run the main project, navigate to "Intel_SightPP"
  - At this point, if you would like to edit the run arguments right-click the executable and select "add debug configuration."
    - Within the JSON object add the attribute args:[ \"\" ] and add the arguments you required
  - Finally: Either select Debug within the executable or set the item as Startup item and run from the top panel.
- Option 2,
  - To set the run arguments, right-click on the top-level CMake and select add build configuration, select default.
    - within the JSON object add the attribute args:[ \"\" ] and add the arguments you required
  -  Next, either, you can right-click on the top .txt file and select debug, or you can set the text file as the Startup Item and run the software from the top control panel as usual.

#### Building Cmake in terminal
- To build the CMake in the terminal, navigate to the top file of the CMake repo.
- Create a new folder that you would like to build the repo in
  - Please use a name from this list so it is ignored from the repo: output,build,out,compile,debug or test
- Run the following command to build the repo into a visual studio source. Change the vcpkg location to your own path. Note: one could also remove the "Visual Studio 16 2019" to build it into your systems default Packages.
```bash
cmake .. -G "Visual Studio 16 2019" "-DCMAKE_TOOLCHAIN_FILE=<Path to vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake"
```
-Note that it may not work the first time and a second run may be required as sometimes vcpkg may not be found at first.
- This should build the system similar to how it builds within the VS version.
- Finally to create the executable run the following command:
```bash
Windows:
camke --build .
```
```bash
OS/linux:
make .
```
- The executables will now be found within your desired build folder. bin/debug by default.
- Note: You will also be able to open the VS project file via the *.sln* file within the build folder.
- Note: You may need to copy the models folder to the location of the executable as this is needed, especially if you changed the build directory.
- Following you can run the executable such as Inter_SightPP.exe with the flags following.

###flags
Flags for running the system include:
- "realsense" = run the system using a realsense camera
- "-color" = view the rgb output from the service for testing
- "-depth" = view the deoth output from the service for testing
- "-rec \<filename\>" = record input to a file
- "-play \<filename\>" = set imput to a file
- "-port \<port number\>" = sets the websocket port, default is 7979


## User Tutorials

Links to tutorials will are linked below. Please follow them for startup guides on using this system.

There is a link to the youtube playlist of tutorital, videos are to be added by 25/09/2020:

Here is the [playlist](https://www.youtube.com/playlist?list=PLdIlpeMVPz67gisf5Q0S8F_MMb9EodiW9)


## Future work

While we have improved the efficiency of the object detection networks significantly, we can explore more possibilities to further improve the processing time. For example, the openVINO utilises the model optimiser and back engine to facilitate faster inference of deep learning models, which is two times faster than our current approach. Another possible improvement is the GPU utilisation when training and using the object recognition networks. An example of practicable approach to use GPU to speed up the performance is CUDA, which is a parallel computing platform that enables the GPU for model training and inference.


One possible method to improve object tracking is to design tracking strategies. For example, the trackers should be assigned based on the size and position of the objects. Also, we can set targets for tracking rather than track all identified objects. These strategies can reduce the tracked objects and put limited resources to accurate calculation of important ones.


The prioritiser module has an incredible amount of potential. Understanding the data classified by the object detection module has many opportunities such as processing other use cases for navigation or heuristic inference classification for new MADs. The simplicity of the prioritiser architecture allows for rapid development of priority experiments and allows for setting up new heuristic AIs. Improvements of the current system could include further increase in the sophistication of the rule based system such as priority by object size or implementing a full historical knowledge base for future priority prediction. Finally, improved user to system communication would allow for a tailored experience.

The next step for the interface would be using a more adaptable STT API to satisfy various browsers. Although using speech APIs with buttons clicks can interact with users efficiently, it would be more convenient for users to use an application without buttons. Therefore, the next step could be to a more adaptable speech recognition APIs which could identify what the user speaks continuously and make corresponding interactions.

## Contrubuting
All future Contributions should be submitted to the Intel Copy of this repo hosted by the Intel Company. The link to said repo will be posted here when it is up.
## Acknowledgement
Extending thanks to the team at Intel UK, comprising Costas Stylianou, Philippa Chick, Chris Feltham along with Jo Baker from AstraZeneca for assisting and mentoring the team throughout the project. We would also like to thank Prof. Emmanuel Letier for his guidance and contribution as the supervisor of the project. Following, we would like to thank Sheena Visram at UCLIC and Albert Higgins from the UCL International Disability Innovation Hub for their assistance in understanding the user interaction and medical component of this project. Additional thanks go to Fredrick Odhiambo and Shailen Sobhee for their input and feedback in the final phase of the project. Finally, we would like to thank Dr. Dean Mohamedally for leading the IXN initiative and UCL for making these project possible.
