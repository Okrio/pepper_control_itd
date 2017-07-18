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
