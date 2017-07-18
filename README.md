# pepper_control_itd

## Installation

* Get and build [sound grabber](https://github.com/lagadic/al_sound_grabber)

* Install `itpp`:

        $ sudo apt-get install libitpp-dev

* Build [visp_naoqi](https://github.com/lagadic/visp_naoqi): libqi branch
* Clone `pepper_control_itd` in your workspace:

        $ git clone https://github.com/lagadic/pepper_control_itd.git

* Checkout libqi branch and build

        $ source /opt/ros/kinetic/setup.bash
        $ cd pepper_control_itd
        $ git checkout libqi
        $ mkdir build; cd build
        $ ccmake ../ -DVISP_DIR=$HOME/soft/visp/visp-build -Dvisp_naoqi_DIR=$HOME/soft/visp_naoqi/visp-naoqi-build
        $ make -j4

## Usage

* In one terminal run the sound grabber:

        $ cd $HOME/robot_ws/al_sound_grabber
        $ ./sdk/bin/al_sound_grabber --pip 192.168.0.24 --pport 9559`

* In a second terminal run the robot controller

        $ ./src/itd_control --ip 192.168.0.24

