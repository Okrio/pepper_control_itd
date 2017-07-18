/****************************************************************************
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://team.inria.fr/lagadic/visp for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://team.inria.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * This example demonstrates how to control the robot remotely in position and velocity.
 *
 * Authors:
 * Giovanni Claudio
 *
 *****************************************************************************/

#include <iostream>
#include <string>

//#include <alproxies/almemoryproxy.h>
#include <qi/session.hpp>
#include <qi/applicationsession.hpp>
#include <qi/anymodule.hpp>

#include <visp/vpPlot.h>
#include <visp/vpDisplayX.h>

#include <visp_naoqi/vpNaoqiRobot.h>

#include "./robot/BSSLocate/BSSlocate.h"
#include "./robot/Misc/misc.h"


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

std::vector<float> fromAnyValueToFloatVector(qi::AnyValue& value){
  qi::AnyReferenceVector anyrefs = value.asListValuePtr();
  std::vector<float> result;

  for(int i=0; i<anyrefs.size();i++)
  {
    try
    {
      result.push_back(anyrefs[i].content().toFloat());
    }
    catch(std::runtime_error& e)
    {
      result.push_back(-1.0);
      std::cout << e.what() << "=> set to -1" << std::endl;
    }
  }
  return result;
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

int main(int argc, const char* argv[])
{
  try
  {
    std::string opt_ip = "192.168.0.24";
    bool opt_language_english = true;
    int opt_cam = 0;
    bool opt_record = true;

    for (unsigned int i=0; i<argc; i++) {
      if (std::string(argv[i]) == "--ip")
        opt_ip = argv[i+1];
      else if (std::string(argv[i]) == "--help") {
        std::cout << "Usage: " << argv[0] << "[--ip <robot address>] [--help]" << std::endl;
        return 0;
      }
     }

    // Connection to module to control Pepper in velocity
    qi::SessionPtr session = qi::makeSession();
    std::string ip_port = "tcp://" + opt_ip + ":9559";
    session->connect(ip_port);
    if (! opt_ip.empty()) {
      std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
    }

    // Connect to the robot
    vpNaoqiRobot robot(session);
    robot.open();

    if (robot.getRobotType() != vpNaoqiRobot::Pepper) {
      std::cout << "ERROR: You are not connected to Pepper, but to a different Robot. Check the IP. " << std::endl;
      return 0;
    }

    std::string jointName;

    jointName = "HeadYaw";

    bool servoing = true;

    // Define values
    //float lambda = 0.03;
    float lambda = 1.5; // 0.050;
    mat recordS;
    vpColVector vel(1);
    //AL::ALMemoryProxy memProxy(opt_ip, 9559);

    qi::AnyObject memProxy(session->service("ALMemory"));

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
      //std::vector<float> left_= memProxy.getData("ALSoundProcessing/leftVec");
      //std::vector<float> right_= memProxy.getData("ALSoundProcessing/rightVec");
      qi::AnyValue data_left = memProxy.call<qi::AnyValue>("getData", "ALSoundProcessing/leftVec");
      qi::AnyValue data_right = memProxy.call<qi::AnyValue>("getData", "ALSoundProcessing/rightVec");

      std::vector<float> left_ = fromAnyValueToFloatVector(data_left);
      std::vector<float> right_ = fromAnyValueToFloatVector(data_right);

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
          robot.setVelocity(names, vel_);

      // Save current values
      loop_iter ++;
      std::cout << "Loop time_tot: " << vpTime::measureTimeMs() - t << " ms" << std::endl;

      if (vpDisplay::getClick(I, false))
        break;
    }

    //   plotter->saveData(0, "ratio.dat");

      robot.stop(jointName);
      robot.stopPepperControl();

    vpDisplay::getClick(I, true);
    //plotter.saveData(0, "itdDense_C.dat");
    //plotter.saveData(1, "itdDense_vel.dat");
    //misc::writemat("itdDense_corr.dat", recordS.transpose());

  }
  catch (const vpException &e)
  {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

  return 0;
}
