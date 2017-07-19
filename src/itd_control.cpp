/****************************************************************************
 *
 * Copyright (C) 2005 - 2017 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using this software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a Professional
 * Edition License.
 *
 * See http://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, or concerning the
 * license please contact Inria at Fabien.Spindler@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Authors:
 * Fabien Spindler, Giovanni Claudio, Aly Magassouba
 *
 ****************************************************************************/

/*! \example speech_recognition.cpp */
#include <iostream>
#include <string>
#include "./robot/BSSLocate/BSSlocate.h"
#include "./robot/Misc/misc.h"

#include <alproxies/almemoryproxy.h>
#include <qi/session.hpp>
#include <qi/applicationsession.hpp>
#include <qi/anymodule.hpp>

#include <visp/vpPlot.h>
#include <visp/vpDisplayX.h>

#include <visp_naoqi/vpNaoqiRobot.h>


const std::string currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d_%H.%M.%S", &tstruct);

  return buf;
}

double getITD(const mat stereoSignal, float d_microphone, int freq){

  BSSlocate bss(stereoSignal, d_microphone,freq);
  vec tau_grid= bss.compute();
  vec crossCorr=bss.getCC();
  if (fabs(min(crossCorr)>0.5)) // too uncertain measurements(probably spurious): tau<- previous tau
    return -1;
  else
    return (tau_grid[0]);
}

/*!

   Connect toRomeo robot, and apply some motion.
   By default, this example connect to a robot with ip address: 198.18.0.1.
   If you want to connect on an other robot, run:

   ./motion --ip <robot ip address>

   Example:

   ./motion --ip 169.254.168.230
 */
int main(int argc, const char* argv[])
{
  try
  {
    std::string opt_ip = "198.18.0.1";;
    if (argc == 3) {
      if (std::string(argv[1]) == "--ip")
        opt_ip = argv[2];
    }

    vpNaoqiRobot robot;
    if (! opt_ip.empty()) {
      std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
      robot.setRobotIp(opt_ip);
    }

    robot.open();

    std::string jointName;

    if (robot.getRobotType() == vpNaoqiRobot::Pepper)
      jointName = "HeadYaw";
    else if (robot.getRobotType() == vpNaoqiRobot::Romeo)
      jointName = "NeckYaw";
    else
    {
      std::cout << "Type of robot not valid" << std::endl;
      return 0;
    }

    bool servoing = true;

    // Define values
    //float lambda = 0.03;
    float lambda = 1.5; // 0.050;
    mat recordS;
    vpColVector vel(1);
    AL::ALMemoryProxy memProxy(opt_ip, 9559);

    vpImage<unsigned char> I(320, 320);
    vpDisplayX dd(I);
    vpDisplay::setTitle(I, "ViSP viewer");

    vpPlot plotter(3, 250*2, 500*2, 100, 200, "Real time curves plotter");
    plotter.initGraph(0, 1);
    plotter.initGraph(1, 1);
    plotter.initGraph(2, 1);
    plotter.setTitle(0,  "C");
    plotter.setTitle(1,  "Velocity");
    plotter.setTitle(2,  "Cross-Correlation");

    std::vector<float> vel_ (2);
    std::vector<std::string> names = robot.getBodyNames("Head");
    vpMatrix L;
    unsigned long loop_iter = 0;
    double tau=0;
    double tauStar=0;
    double oldtau=0;
    int fs=16000;
    float d_micro=0.1;
    float c=343.;
    float A=d_micro/c;
    float ell=1;
    double t;
    while (1)
    {
      std::cout << "----------------------------------" << std::endl;
      t = vpTime::measureTimeMs();
      std::vector<float> left_= memProxy.getData("ALSoundProcessing/leftVec");
      std::vector<float> right_= memProxy.getData("ALSoundProcessing/rightVec");
      mat stereoSignal=zeros(2,right_.size());
      for(int i=0;i<left_.size();i++){
        stereoSignal(0,i)=left_[i];
        stereoSignal(1,i)=right_[i];
      }

      //std::cout << "stereoSignal:" << stereoSignal << std::endl;
      tau=getITD( stereoSignal.transpose(), d_micro, fs);
      if (tau==-1)
        tau=oldtau;
      else
        oldtau=tau;
      /*Interaction matrix*/
      L.resize(1,6);
      L[0][0]=-(A*A-tau*tau)/(A*ell);
      L[0][1]=tau*sqrt(A*A-tau*tau)/(A*ell);
      L[0][5]=sqrt(A*A-tau*tau);

      std::cout << "ITD: " << tau << std::endl;

      // Compute Interaction matrix

      //std::cout << "Loop time_Mat: " << vpTime::measureTimeMs() - t << " ms" << std::endl;
      //std::cout << "matrix:" << L << std::endl;
      //Compute joint velocity NeckYaw: Head only
      vel[0] = -lambda* 1./L[0][5]*(tau-tauStar);

      vel_[0] = -vel[0];

      plotter.plot(0, 0, loop_iter, tau);
      plotter.plot(1, 0, loop_iter,vel[0]);

      std::cout << "vel: " << vel << std::endl;

      if (servoing)
      {
        if (robot.getRobotType() == vpNaoqiRobot::Pepper)
          robot.setVelocity(names, vel_);
        else if (robot.getRobotType() == vpNaoqiRobot::Romeo)
          robot.setVelocity(jointName, vel);
      }

      // Save current values
      loop_iter ++;
      std::cout << "Loop time_tot: " << vpTime::measureTimeMs() - t << " ms" << std::endl;

      if (vpDisplay::getClick(I, false))
        break;
    }

    //   plotter->saveData(0, "ratio.dat");

    if (robot.getRobotType() == vpNaoqiRobot::Pepper)
    {
      robot.stop(jointName);
      robot.stopPepperControl();
    }
    else if (robot.getRobotType() == vpNaoqiRobot::Romeo)
      robot.stop(jointName);

    vpDisplay::getClick(I, true);
    //plotter.saveData(0, "itdDense_C.dat");
    //plotter.saveData(1, "itdDense_vel.dat");
    //misc::writemat("itdDense_corr.dat", recordS.transpose());

  }
  catch (const vpException &e)
  {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }
  catch (const AL::ALError &e)
  {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

  return 0;
}
