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

#include <itpp/itbase.h>
#include <itpp/signal/transforms.h>
#include <itpp/stat/misc_stat.h>
#include <itpp/itsignal.h>
#include <itpp/fixed/cfix.h>
#include <complex>
#include <cmath>
#include "BSSstft.h"
using namespace itpp;

/*
 * 
% BSS_LOCATE_SPEC Estimation of the source TDOAs in a stereo convolutive
% mixture using an angular spectrum
%
% [tau, spec] = bss_locate_spec(x, fs, d, nsrc, local, pooling, tau_grid)
%
% Inputs:S
% x: nsampl x 2 matrix containing a stereo mixture signal
% fs: sampling frequency in Hz
% d: microphone spacing in meters
% nsrc: number of sources
% local: local angular spectrum function: 'GCC-PHAT', 'GCC-NONLIN'
%     (default), 'MUSIC', 'DS', 'MVDR', 'DNM', 'DSW' or 'MVDRW'
% pooling: pooling function: 'max' (default) or 'sum'
% tau_grid: 1 x ngrid vector of possible TDOAs in seconds (default: 181
%     values linearly spaced between -d/343 and d/343)
%
% Outputs:
% tau: 1 x nsrc vector of estimated TDOAs in seconds
% spec: ngrid x 1 angular spectrum
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * 
 */

class BSSlocate{
private:
int  nchan, nsampl, wlen, nsrc,fs;
double d, c;
vec f,grid,crossCorr;
Vec<cmat> X;

public:
  BSSlocate(const mat &sig, const double &dist, const int &freq);
  vec compute();
  vec getCC();
 private:
  Vec<mat> nonLinSpec(const Vec<cmat>& X, const double & alpha,const vec & tau_grid);
  const void findpeaks(const vec &x, vec *ind, vec *peak);
  mat fastCSqrt(const mat & in);
  vec getSpec();
};
