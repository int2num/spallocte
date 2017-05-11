//
// Created by root on 17-5-8.
//
#ifndef CSPALLOC_GRAPH_H
#define CSPALLOC_GRAPH_H
#include<bits/stdc++.h>
#include"edge.h"
#include"pathalg.h"
using namespace std;
struct levelGraph {
    vector<int>startnodes;
    vector<int>startedges;
    int nodenum=-1;
    int edgenum=-1;
    vector<edge>edges;
    int levelsize;
};
class Graph
{
    public:
        double remain;
        vector<edge>edges,extenedges;
        unordered_map<int,levelGraph>levels ;
        int n,width,maxnode,maxedge;
        vector<int>etn2n;
        vector<int> exe2e;
        vector<vector<int>>relate;
        algbase&router;
        bool cutcake(int dbw) {
            remain-=dbw;
            if(remain<0)
            {
                remain+=dbw;
                return false;
            }
            levelGraph &lg=levels[dbw];
            int newstart=lg.nodenum+1;
            int newstartedge=lg.edgenum+1;
            lg.startnodes.push_back(newstart);
            lg.startedges.push_back(newstartedge);
            lg.nodenum=newstart+maxnode;
            lg.edgenum=newstartedge+maxedge;
            unordered_set<int>ifhas;
            for(int i=0;i<extenedges.size();i++)
            {
                int s=extenedges[i].s+newstart,t=extenedges[i].t+newstart;
                lg.edges.push_back(edge(s,t,extenedges[i].w,dbw));
            }
            lg.levelsize++;
            return true;
        };
        bool cookedge(int s,int t,int dbw)
        {
            while(!levels.count(dbw))
                if(!cutcake(dbw))
                {
                    cout<<"no bandw to task "<<s<<" "<<t<<" "<<dbw<<endl;
                    return false;
                }
            while(true)
            {
				levelGraph &lg=levels[dbw];
				vector<int>raw2prime(lg.edges.size(),-1);
				int nzsize=0;
				for(int i=0;i<lg.edges.size();i++)
					if(lg.edges[i].bw>0)
						nzsize++;
				vector<edge>rawedge(nzsize+lg.levelsize*(2+width),edge(0,0,0,0));
				for(int i=0;i<lg.edges.size();i++)
					if(lg.edges[i].bw>0)
						rawedge[i]=lg.edges[i];
				int head=lg.nodenum+1,tail=lg.nodenum+2;
				vector<int>lev2s(lg.edges.size()*2,-1);
				for(int i=0;i<lg.levelsize;i++)
				{
					lev2s[nzsize]=lg.startedges[i];
					int rs=s+(lg.levelsize-1)*(maxnode+1);
					int rt=t+(lg.levelsize-1)*(maxnode+1);
					rawedge[nzsize++]=edge(head,rs,0,0);
					rawedge[nzsize++]=edge(rt,tail,0,0);
					for(int k=0;k<width;k++)
						rawedge[nzsize++]=edge(rt+n*(k+1),tail,0,dbw);
				}
				vector<int> rout=router.routalg(head,tail,rawedge,tail+1);
				if(rout.size()>0) {
					int lvs = lev2s[rout[0]];
					for (int i = 1; i < rout.size(); i++) {
						int primee = raw2prime[rout[i]];
						cout<<rawedge[rout[i]].t<<" ";
						vector<int> related = relate[exe2e[primee - lvs]];
						for (int j = 0; j < related.size(); j++)
							lg.edges[related[j] + lvs].bw = 0;
						if (etn2n[rawedge[rout[i]].t%(maxnode+1)] == t)
							break;
					}
					cout<<endl;
				}
				else
					if(!cutcake(dbw))
					{   cout<<"no bandw to task "<<s<<" "<<t<<" "<<dbw<<endl;
						return false;
					}
				return true;
            }
        }
        virtual ~Graph(){};
    protected:
        void addedge(int _s,int _t,int _w,double _bw=500){
            edges.push_back(edge(_s,_t,_w,_bw));
            edges.push_back(edge(_t,_s,_w,_bw));
        };
        virtual void GenGraph()=0;
        Graph(int _n,int _degree,algbase&alg):n(_n),width(_degree),remain(500),etn2n(n*(width+1),-1),maxnode(0),router(alg){};
        void extend()
        {
        	vector<vector<int>>rs(edges.size(),vector<int>());
        	relate=rs;
        	vector<int>es(edges.size()*(width+1),-1);
        	exe2e=es;
            for(int i=0;i<edges.size();i++)
            {
                int k=0;
                while(edges[i].w+k<=width)
                {
                    int s=edges[i].s+n*k;
                    etn2n[s]=edges[i].s;
                    int t=edges[i].t+n*(k+1);
                    etn2n[t]=edges[i].t;
                    exe2e[extenedges.size()]=i;
                    relate[i].push_back(extenedges.size());
                    extenedges.push_back(edge(s,t,edges[i].w,edges[i].bw));
                    if(s>maxnode||t>maxnode)
                        maxnode=max(s,t);
                    k++;
                }
            }
            maxedge=extenedges.size()-1;
        };
};
class ERGraph:public Graph{
public:
    ERGraph(int _n,int _degree,algbase&alg):Graph(_n,_degree,alg){GenGraph();extend();};
private:
    virtual void GenGraph(){
        int count = 0;
        set<pair<int, int>>flag;
        double threhod = log((double)(n));
        for (int i = 0; i <n; i++)
            for (int j = 0; j<n; j++)
                if (i != j)
                {
                    double ran =(double) (rand() % (n));
                    if (ran<threhod)
                    {
                        if (flag.find(make_pair(i, j))==flag.end())
                        {
                            addedge(i,j,rand()%3+1);
                            flag.insert(make_pair(i,j));
                            flag.insert(make_pair(j,i));
                        }
                    }
                }
    };
};
class BAGraph:public Graph{
public:
    BAGraph(int _n,int _degree,algbase&alg):Graph(_n,_degree,alg){GenGraph();extend();};
private:
    virtual void GenGraph(){
        int todu = 0;
        int count = 0;
        int k =3;
        vector<int>du(n,0);
        for (int i = 0; i < 5; i++)
        {
            addedge(i,i+1,rand()%3+1);
            du[i]++;
            du[i + 1]++;
            todu += 2;
        }
        for (int i = 5; i < n; i++)
        {
            int addin = 0;
            while (addin < k)
            {
                for (int j = 0; j < n; j++)
                {
                    if (i!=j&&rand() % todu < du[j])
                    {
                        addedge(i,j,rand()%3+1);
                        du[i]++;
                        du[j]++;
                        todu += 2;
                        addin++;
                    }
                    if (addin >= k)
                        break;
                }
            }
        }
    };
};
#endif //CSPALLOC_GRAPH_H
