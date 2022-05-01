# mks-memory-loop
MKS Distributed (UDP) Memory Loop Library


This C/CXX-11 library enables the simple configuration and execution of applications which share a distributed memory environment such that each host participant (federate) can provide a UDP network frame of 508 bytes to each other federate in a federation with high frequency. 
The format and contents of these frames must be handled by the application and fit into the buffer size of 508 bytes. 
Possible use cases are multiplayer games which each player provides its data in a frame to the federation.


This library is being developed and tested using Linux (Ubuntu 20,04 LTS) and BSD (FreeBSD 13.0).

## Using The Library
The library consists of three files, the header (mksmemoryloop.h) and the library archives (mksmemoryloop.a/mksmemoryloop.so). 

First, build the library invoking make.

Include the header in your project and link with the shared/static library file. 

The library can be installed to/uninstalled from the /usr/local/include (header) and /usr/local/lib (static and shared library) folders by invoking  make [install/uninstall] as root or sudo. 

## Sample Configuration
3               - Number of federates/hosts

192.168.2.121   - host 0 IP

13201           - host 0 port

192.168.2.121   - host 1 IP

13202           - host 1 port

192.168.2.121   - host 2 IP

13203           - host 2 port

0               - platform/own federate index

20              - thread network sleep time in milliseconds per frames handling


## Examples

### (simple_example) Simple example using the frame pointer with a simple structure overlaid
This example uses the configuration files simple_example_[0|1|2].cfg and is intended to be lauched with 3 applications. The parameters of each application should use one of the three config files as shown below.

In three shells, go to the simple_example directory and run 1 for each command line:

./simple_example ./simple_example_0.cfg

./simple_example ./simple_example_1.cfg

./simple_example ./simple_example_2.cfg


You will have 3 counters running in printout and you can quit by pressing any key. The application can be restarted anytime and resumes communication with any other federates running.

### (simple_example_raw) Simple example using a raw pointer of the frame
This example uses the configuration files simple_example_raw_[0|1|2].cfg and is intended to be lauched with 3 applications. The parameters of each application should use one of the three config files as shown below.

In three shells, go to the simple_example directory and run 1 for each command line:

./simple_example_raw ./simple_example_raw_0.cfg

./simple_example_raw ./simple_example_raw_1.cfg

./simple_example_raw ./simple_example_raw_2.cfg


You will have 3 counters running in printout and you can quit by pressing any key. The application can be restarted anytime and resumes communication with any other federates running.

### (sfml_example) Simple Example using the SFML library for windowing and display and a custom structure from the pointer of the frame
This example uses the configuration files sfml_example_[0|1|2].cfg and is intended to be lauched with 3 applications. The parameters of each application should use one of the three config files as shown below.

In three shells, go to the simple_example directory and run 1 for each command line:

./sfml_example ./sfml_example_0.cfg

./sfml_example ./sfml_example_1.cfg

./sfml_example ./sfml_example_2.cfg


You will have 3 circles, red, green and blue and you can quit by closing the window. The position and color data for each circle is synchronized throughout the federates on the network. The application can be restarted anytime and resumes communication with any other federates running.


