#include <iostream>
#include"Graph.h"
#include<sys/time.h>
int main()
{
    struct timeval starttime, endtime;
    gettimeofday(&starttime,NULL);
    dijkstor plo=dijkstor();
    //parallelor plo=parallelor();
    ERGraph graph(30000,15,plo);
    int count=0;
    while(count<100){graph.cookedge(rand()%10000,rand()%10000,(rand()%5+1)*10),count++;};
    gettimeofday(&endtime,NULL);
    cout<<"time is:"<<endtime.tv_sec- starttime.tv_sec<<endl;


}
