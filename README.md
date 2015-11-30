# LinuxHeartbeat
A heartbeat module for dmesg

This module serves as an example of how to build a kernel module as well as how to interact with timers and dmesg. It was never submitted for formal acceptance so I presume that it would not be fit for that purpose. If you are planning on building a kernel module for mainline acceptance, please read https://www.kernel.org/doc/Documentation/SubmittingPatches for the proper procedures to follow.

## Building / Installing on Debian
Install the required packages for building modules for your local kernel:
```
sudo apt-get install linux-headers-$(uname -r) build-essential
```

Check out the code via git:
```
git clone git@github.com:metiscus/LinuxHeartbeat.git
```

Build and insert the module for testing:
```
cd LinuxHeartbeat
make
sudo insmod heartbeat.ko
```
