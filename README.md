# mks-memory-loop
MKS Distributed (UDP) Memory Loop Library

This CXX-11 library enables the simple configuration and execution of applications which share a ditributed memory environment such that each participant (federate) can provide a UDP network frame of 508 bytes to each other federate in a federation with high frequency. 
The format and contents of these frames must be handled by the application and fit into the buffer sioze of 508 bytes. 
Possible use cases are multiplayer games which each player provides it's data in a frame to the federation.

## Using The Library
The library consists of two files, the header (mksmemoryloop.h) and the library archive (mksmemoryloop.a). Includew the header in your project and link with the library file.

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

### Simple Example
This example uses the configruation files simple_example_[0|1|2].cfg and is intended to be lauched with 3 applications. The parameters of each application should use one of the three config files as shown below.

In three shells, go to the simple_example directory and run 1 for each command line:
./simple_example ./simple_example_0.cfg 
./simple_example ./simple_example_1.cfg
./simple_example ./simple_example_2.cfg

You will have 3 counters running in printout and you can quit by pressing any key. The application can be restarted anytime and resumes communication witrh any other federates running.

