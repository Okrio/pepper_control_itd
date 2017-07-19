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

#include <itpp/base/sort.h>
#include <itpp/itbase.h>
#include <itpp/signal/transforms.h>
using namespace itpp;
/*% STFT_MULTI Multichannel short-time Fourier transform (STFT) using
% half-overlapping sine windows.

% X=stft_multi(x)
% X=stft_multi(x,wlen)
%
% Inputs:
% x: nchan x nsampl matrix containing nchan time-domain mixture signals
% with nsampl samples
% wlen: window length (default: 1024 samples or 64ms at 16 kHz, which is
% optimal for speech source separation via binary time-frequency masking)
%
% Output:
% X: nbin x nfram x nchan matrix containing the STFT coefficients with nbin
% frequency bins and nfram time frames
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Copyright 2008 Emmanuel Vincent
% This software is distributed under the terms of the GNU Public License
% version 3 (http://www.gnu.org/licenses/gpl.txt)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
class BSSstft
{
private:
  int nchan, nsampl, wlen, nfram, nbin;
  vec win, swin;
  mat s;

public:
  BSSstft(const int &len, const mat &sig);
  const Vec< cmat > compute();
};
