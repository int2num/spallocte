
/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include"pathalg.h"
static const int WORK_SIZE = 128;
__global__ void bellmanHigh(edge *edges,int*d,int*m,int edgesize)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if (i >=edgesize)return;
	int head = edges[i].s;
	int tail = edges[i].t;
	int w=edges[i].w;
	int val = d[head]+w;
	if (d[tail] >val){
		*m = 1;
		d[tail] = val;
	}
};
__global__ void color(edge *edges,int* d,int*pre,int edgesize)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if (i >=edgesize)return;
	int head = edges[i].s;
	int tail = edges[i].t;
	int w=edges[i].w;
	int val=d[head]+w;
	if (d[tail]==val&&d[tail]<INT_MAX/3)
		pre[tail]=i;
};
void parallelor::allocate(int s,vector<edge>&edges,int nodenum){
	cudaMalloc(&dev_edges, sizeof(edge)*edges.size());
	cudaMalloc((void**)&dev_d,nodenum*sizeof(int));
	cudaMalloc((void**)&dev_pre,nodenum*sizeof(int));
	cudaMalloc((void**)&dev_m,sizeof(int));
	m=new int;
	*m=0;
	d=new int[nodenum],pre=new int[nodenum];
	memset(pre,-1,sizeof(int)*nodenum);
	for(int i=0;i<nodenum;i++)
		d[i]=INT_MAX/2;
	d[s]=0;
	aedges=new edge[edges.size()];
	for(int i=0;i<edges.size();i++)
		aedges[i]=edges[i];
	cudaMemcpy(dev_edges,aedges,edges.size()* sizeof(edge),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m,m,sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_d,d,sizeof(int)*nodenum,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_pre,pre,sizeof(int)*nodenum,cudaMemcpyHostToDevice);
};
void parallelor::dellocate(){
	delete[]d;
	delete[]pre;
	delete[]aedges;
	delete m;
	cudaFree(dev_edges);
	cudaFree(dev_m);
	cudaFree(dev_d);
	cudaFree(dev_pre);
};
parallelor::parallelor(){};
vector<int> parallelor:: routalg(int s,int t,vector<edge>&edges,int nodenum)
{
	dijkstor plo=dijkstor();
	allocate(s,edges,nodenum);
	int zero=0;
	int ck=0;
	do{
		if(ck%5==0)
			cudaMemcpy(dev_m, &zero, sizeof(int), cudaMemcpyHostToDevice);
		bellmanHigh << <(edges.size()/WORK_SIZE)+1, WORK_SIZE >> >(dev_edges,dev_d,dev_m,edges.size());
		if(ck%5==0)
			cudaMemcpy(m, dev_m, sizeof(int), cudaMemcpyDeviceToHost);
		ck++;
	}
	while(*m>0);
	color << <(edges.size()/WORK_SIZE)+1, WORK_SIZE >> >(dev_edges,dev_d,dev_pre,edges.size());
	cudaMemcpy(pre,dev_pre,sizeof(int)*nodenum,cudaMemcpyDeviceToHost);
	cudaMemcpy(d,dev_d,sizeof(int)*nodenum,cudaMemcpyDeviceToHost);
	vector<int>vpree;
	for(int i=0;i<nodenum;i++)
		vpree.push_back(pre[i]);
	dellocate();
	return getrout(s,t,edges,vpree);

};
