#include "BSSlocate.h"


BSSlocate::BSSlocate (const mat &sig, const double &dist, const int &freq) {
    nchan=sig.rows();
    nsampl=sig.cols();
    wlen=1024;
    d=dist;
    fs=freq;
    c=343.; //sound celerity
    int tauGridSize=180;
    f=fs*1.0/wlen*linspace_fixed_step<>(1.,wlen/2.);
    grid=linspace_fixed_step<>(-d/c,d/c,2*d/(c*(tauGridSize-1)));
    if (grid.size()>180)
        grid.del(grid.size()-1);
    /*
     Linear transform
    X = stft_multi(x.',wlen);
    X = X(2:end,:,:); */
    BSSstft stft(wlen,sig.transpose());
    X =stft.compute();
    for(int i=0; i<X.size(); i++)
        X[i].del_row(0);


}









vec BSSlocate::getSpec(){

    int c = 343;
    const double alpha = 10*c/(d*fs);
    Vec<mat> spec= nonLinSpec(X,alpha,grid);
    
    Vec<mat> vspec,gvspec;
    vec mspec;
    
    for(int i=0; i<spec.length(); i++){
        vspec.ins(i,sum(spec[i],1));        
    }
    for(int i=0; i<vspec.length(); i++){        
        mspec.ins(i,sum(vspec[i],1));//alternative solution mspec.ins(i,max(vspec[i],1));
        
    }
        
        return mspec;

}

vec BSSlocate::getCC(){
    return (crossCorr/max(crossCorr));
}


vec BSSlocate::compute() {
    crossCorr=getSpec();
    vec ind, val;
    vec tau;
    int index;
    findpeaks(crossCorr, &ind, &val);
    //std::cout<<"mspec:"<<mspec<<std::endl;
    //std::cout<<"grid:"<<mspec.size()<<" "<<grid.size()<<std::endl;

    vec copyval=val;

    // sort operation
    for(int i=0; i<ind.length(); i++) {
        index=max_index<>(copyval);
        tau.ins(tau.length(),grid.get(ind[index]));
        copyval.del(index);
        ind.del(index);
     }
    return tau;
}


mat BSSlocate::fastCSqrt(const mat  &in){
    cmat out=to_cmat(zeros(in.rows(),in.cols()));
    for (int i=0;i<in.rows();i++){
        for(int j=0;j<in.cols();j++){
           if(in(i,j)<0)
               out(i,j)=std::complex<double>(0.,sqrt(abs(in(i,j))));
           else
               out(i,j)=sqrt(in(i,j));
        }
    }
    return real(out);
}

Vec<mat> BSSlocate::nonLinSpec(const Vec<cmat> &X, const double &alpha, const vec &tau_grid) {
    /*

    [nbin,nfram] = size(X(:,:,1));
    ngrid = length(tau_grid);
    X1 = X(:,:,1);
    X2 = X(:,:,2);

    spec = zeros(nbin,nfram,ngrid);
    P = X1.*conj(X2);
    P = P./abs(P);
    for ind = 1:ngrid,
        EXP = repmat(exp(-2*1i*pi*tau_grid(ind)*f),1,nfram);
        spec(:,:,ind) = ones(nbin,nfram) - tanh(alpha*real(sqrt(2-2*real(P.*EXP))));
    end

    return;
     * */
    //mat Jcoeff,Jcoeff2;
    double nbin=X[0].rows();
    int nfram=X[0].cols();
    int ngrid=tau_grid.length();
    cmat X0=X[0];
    cmat X1=X[1];
    cmat EXP,u;
    mat gEXP;
    Vec<mat> spec;
    spec.set_length(ngrid);
    
    for(int i=0; i<ngrid; i++){
        spec[i]=zeros(nbin,nfram);
        
    }
    
    cmat P=elem_mult(X0,conj(X1));
    //std::cout<<"tau_grid:"<<tau_grid<<std::endl;
    P=elem_div(P,to_cmat(abs(P))); //PHase Transform
    
    for(int ind=0; ind<ngrid; ind++) {
        EXP= repmat(itpp::exp(f*(-2)*std::complex<double>(0.,1.)*pi*tau_grid(ind)),1,nfram);
        u=elem_mult(P,EXP);
        spec[ind]=real(u); //alternative solution:spec[ind]=1-tanh(alpha*fastCSqrt(2-2*u));       
      
    }

    return spec;


}


const void BSSlocate::findpeaks(const vec & x,vec *ind, vec *peak) {
    bool sens=0;
    int i=0;
    int last=-3;
    if(x[0]<x[1])
        sens=1;
    if(x[0]>x[1]) {
        ind->ins(ind->length(),0);
        peak->ins(peak->length(),x[0]);
    }
    if(x[x.length()-1]>x[x.length()-2]) {
        ind->ins(ind->length(),x.length()-1);
        peak->ins(peak->length(),x[x.length()-1]);
    }

    while(i<x.length()-1) {
        if(sens==0 ) {
            while(x[i]>=x[i+1] && i<x.length()-1)
                i++;
            sens=1;
        }
        else {
            if(i>x.length()-1)
            {
                break;
            }
            while(x[i]<=x[i+1] && i<x.length()-1)
                i++;
            if(i<x.length()-1 && i-last>4) {
                ind->ins(ind->length(),i);
                peak->ins(peak->length(),x[i]);
                last=i;
            }
            /*else{
                if(x[i]>x[last]){
                    ind->del(ind->length()-1);
                    peak->del(peak->length()-1);
                    ind->ins(ind->length(),i);
                    peak->ins(peak->length(),x[i]);
                    last=i;
                }
            }*/
            sens=0;
        }
        //std::cout<<i<<std::endl;
        //std::cout<<x[i]<<std::endl;
        //std::cout<<x[i+1]<<std::endl;
        //std::cout<<sens<<std::endl;
    }



    //std::cout<<ind->length()<<std::endl;
    //std::cout<<*peak<<std::endl;
    //std::cout<<*ind<<std::endl;
}




