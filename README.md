# pepper_control_itd

## Installation

* Get sound_grabber
* Install itpp:
  $ sudo apt-get install libitpp-dev
* Configure qibuild workspace and create toolchain: see [here](http://jokla.me/robotics/install-sdk-c-naoqi/)
* Build [visp_naoqi](https://github.com/lagadic/visp_naoqi): see [here](http://jokla.me/robotics/visp_naoqi/) instruction
* Clone `pepper_control_itd` in your qibuild workspace:
  $ git clone https://github.com/lagadic/pepper_control_itd.git
* Configure project:
  $ `configure --release -c  toolchain_2_4 -Dvisp_naoqi_DIR=/home/aly/workspace/visp_naoqi/build-pepper_2_4-release/sdk/cmake -DVISP_DIR=/home/gclaudio/software/visp-build`
  $ Go in the build folder and `make`



## Usage

$  ./sdk/bin/soundprocessing --pip $PEPPER_IP --pport 9559
