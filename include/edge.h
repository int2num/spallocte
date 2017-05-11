//
// Created by root on 17-5-9.
//
#ifndef CSPALLOC_EDGE_H
#define CSPALLOC_EDGE_H
struct edge{
    int s,t,w;
    double bw;
    edge(int _s=0,int _t=0,int _w=0,double _bw=0):s(_s),t(_t),w(_w),bw(_bw){};
};

#endif //CSPALLOC_EDGE_H
