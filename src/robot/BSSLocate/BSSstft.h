#include <itpp/itbase.h>
#include <itpp/base/sort.h>
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
class BSSstft{
private:
int  nchan, nsampl, wlen, nfram, nbin;
vec win, swin;
mat s;
public:
  BSSstft(const int & len,const mat & sig);
  const Vec<cmat> compute();
  
};
