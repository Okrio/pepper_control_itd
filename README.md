# pepper_control_itd

## Installation

* Get sound_grabber 
* Install itpp: 
  `$ sudo apt-get install libitpp-dev` 
* Build [visp_naoqi](https://github.com/lagadic/visp_naoqi): libqi branch
* Clone `pepper_control_itd` in your workspace:  
  $ git clone https://github.com/lagadic/pepper_control_itd.git   
* Checkout libqi branch
* Build project   
* Go in the build folder and `make`

## Usage
`$ cd /home/gclaudio/soft/robot_ws/al_sound_grabber`   
`$./sdk/bin/al_sound_grabber --pip 192.168.0.24 --pport 9559`   
`$ ./sdk/bin/itd_control --ip 192.168.0.24`   

