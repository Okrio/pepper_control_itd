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

#include <fstream>
#include <iostream>
#include <itpp/itbase.h>
#include <sstream>
#include <string>
#include <visp/vpExponentialMap.h>
using namespace itpp;
using namespace std;
class misc
{
public:
  static void writevec(const std::string &filename, const vec &data);
  static void writevec(const std::string &filename, const Vec< vec > &data);
  static void writemat(const std::string &filename, const mat &data);
  static mat vpMatrixtoMat(const vpMatrix &input);
  static vec vpColVectortoVec(const vpColVector &input);
  static vpColVector vectoVpColVec(const vec &input);
  static std::vector< double > vectoStdVec(const vec &input);
  static vec stdVectoVec(std::vector< double > input);
  static vector< float > vecTovFloat(vec input);
  static double closest(vec to_search, double find);
  /** read matrix in text file, useful for microphone and source parameters*/
  static mat readMat(const std::string &path);
  static bool belong(const vec &vlist, const int &val);
  /** read vector in text file, used for the signal*/
  static vec readVec(const string &path);
};
