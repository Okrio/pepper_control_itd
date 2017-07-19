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

#include "BSSstft.h"
/*
%%% Errors and warnings %%%
if nargin<1, error('Not enough input arguments.'); end
if nargin<2, wlen=1024; end
[nchan,nsampl]=size(x);
if nchan>nsampl, error('The signals must be within rows.'); end
if wlen~=4*floor(wlen/4), error('The window length must be a multiple of 4.');
end

%%% Computing STFT coefficients %%%
% Defining sine window
win=sin((.5:wlen-.5)/wlen*pi).';
% Zero-padding
nfram=ceil(nsampl/wlen*2);
x=[x,zeros(nchan,nfram*wlen/2-nsampl)];
% Pre-processing for edges
x=[zeros(nchan,wlen/4),x,zeros(nchan,wlen/4)];
swin=zeros((nfram+1)*wlen/2,1);
for t=0:nfram-1,
    swin(t*wlen/2+1:t*wlen/2+wlen)=swin(t*wlen/2+1:t*wlen/2+wlen)+win.^2;
end
swin=sqrt(wlen*swin);
nbin=wlen/2+1;
X=zeros(nbin,nfram,nchan);
for i=1:nchan,
    for t=0:nfram-1,
        % Framing
        frame=x(i,t*wlen/2+1:t*wlen/2+wlen).'.*win./swin(t*wlen/2+1:t*wlen/2+wlen);
        % FFT
        fframe=fft(frame);
        X(:,t+1,i)=fframe(1:nbin);
    end
end

return;*/

BSSstft::BSSstft(const int &len, const mat &sig)
{
  /*Defining sine window
  win=sin((.5:wlen-.5)/wlen*pi).';
  % Zero-padding

  % Pre-processing for edges
  x=[zeros(nchan,wlen/4),x,zeros(nchan,wlen/4)];
  swin=zeros((nfram+1)*wlen/2,1);
  for t=0:nfram-1,
     swin(t*wlen/2+1:t*wlen/2+wlen)=swin(t*wlen/2+1:t*wlen/2+wlen)+win.^2;
  end*/
  nchan = sig.rows();
  nsampl = sig.cols();
  wlen = len;
  win = sin(linspace_fixed_step<>(0.5, wlen - .5, 1.) / (1.0 * wlen) * pi);
  /* Zero-padding
   * nfram=ceil(nsampl/wlen*2);
  x=[x,zeros(nchan,nfram*wlen/2-nsampl)];
  */
  nfram = ceil((double)nsampl / (double)wlen * 2.);
  s = concat_horizontal<>(sig, zeros(nchan, nfram * wlen / 2 - nsampl));
  /* % Pre-processing for edges
  swin=sqrt(wlen*swin);
  nbin=wlen/2+1;*/
  s = concat_horizontal<>(zeros(nchan, wlen / 4), s);
  s = concat_horizontal<>(s, zeros(nchan, wlen / 4));
  swin = zeros((nfram + 1) * wlen / 2);
  for (int t = 0; t < nfram; t++)
    swin.set_subvector(t * wlen / 2,
                       swin.get(t * wlen / 2, t * wlen / 2 + wlen - 1) +
                           pow(win, 2));

  swin = sqrt((double)wlen * swin);
  nbin = wlen / 2 + 1;
  // std::cout<<s<<std::endl;
}

const Vec< cmat > BSSstft::compute()
{
  /*X=zeros(nbin,nfram,nchan);
  for i=1:nchan,
    for t=0:nfram-1,
        % Framing
        frame=x(i,t*wlen/2+1:t*wlen/2+wlen).'.*win./swin(t*wlen/2+1:t*wlen/2+wlen);
        % FFT
        fframe=fft(frame);
        X(:,t+1,i)=fframe(1:nbin);
    end
  end
  return;*/
  Vec< cmat > X;
  vec frame;
  cvec fframe;
  X.set_length(nchan);
  for (int i = 0; i < nchan; i++)
    X[i] = to_cmat(zeros(nbin, nfram));
  for (int i = 0; i < nchan; i++)
    for (int t = 0; t < nfram; t++) {
      frame = elem_div(
          elem_mult((s.get_row(i)).get(t * wlen / 2, t * wlen / 2 + wlen - 1),
                    win),
          swin.get(t * wlen / 2, t * wlen / 2 + wlen - 1)); // a revoir
      fft(to_cvec(frame), fframe);
      X[i].set_col(t, fframe.get(0, nbin - 1));
    }
  // std::cout<<X<<std::endl;
  return X;
}
