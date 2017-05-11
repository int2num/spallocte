//
// Created by root on 17-5-9.
//
#ifndef CSPALLOC_PATHALG_H
#include"limits.h"
#define CSPALLOC_PATHALG_H
#define INFCOST INT_MAX/2
#include<bits/stdc++.h>
#include"edge.h"
using namespace std;
class pairless {
    public:
        bool operator()(pair<int,int>a,pair<int,int>b){
            return a.second>b.second;
        }
};
class algbase {
    protected:
        vector<int> getrout(int &s, int &t, vector<edge> &edges, vector<int> &pre) {
            vector<int> rout;
            int pp = pre[t];
            while (pp >= 0) {
                rout.push_back(pp);
                pp = pre[edges[pp].s];
            }
            reverse(rout.begin(), rout.end());
            return rout;
        }
    public:
        algbase(){};
        virtual vector<int>  routalg(int s,int t,vector<edge>&edges,int nodenum)=0;
};
class dijkstor:public algbase{
    public:
		vector<vector<int>>neibour;
        dijkstor(){};
        void init(vector<edge>&edges,int nodenum)
        {
			vector<vector<int>>nei(nodenum,vector<int>());
			neibour=nei;
			for(int i=0;i<edges.size();i++)
			{
				int from=edges[i].s;
				neibour[from].push_back(i);
			}
		}
        virtual vector<int> routalg(int s,int t,vector<edge>&edges,int nodenum)
        {
        	init(edges,nodenum);
            vector<int>dist(nodenum,INFCOST);
            vector<int>pre(nodenum,-1);
            priority_queue<pair<int,int>,vector<pair<int,int>>,pairless> que;
            dist[s]=0;
            que.push(make_pair(s,dist[s]));
            while(!que.empty())
            {
                int node=que.top().first;
                que.pop();
                if(node==t)
                {
                    return getrout(s,t,edges,pre);
                }
                for(int i=0;i<neibour[node].size();i++)
                {
                    int id=neibour[node][i];
                    edge e=edges[id];
                    //if(e.bw==0)continue;
                    int to=e.t;
                    if(dist[node]+e.w<dist[to])
                    {
                        dist[to]=dist[node]+e.w;
                        que.push(make_pair(to,dist[to]));
                        pre[to]=id;
                    }
                }
            }
            return vector<int>();
        }
};
class parallelor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m,*m,*dev_pre,*pre,*dev_d,*d;
		void allocate(int s,vector<edge>&edges,int nodenum);
		void dellocate();
	public:
	 	 parallelor();
	 	 virtual vector<int> routalg(int s,int t,vector<edge>&edges,int nodenum);
};
#endif //CSPALLOC_PATHALG_H
