#include<iostream>
#include<ctime>
#include<unistd.h>
using namespace std;

int main()
{
    time_t now_time;
    int pid = fork();
    if(pid == 0)
    {
        while(1)
        {
    	    sleep(1);
    	    now_time = time(NULL);
            cout<<"The parent is talking at "<<now_time<<endl;
        }
    }
    else if(pid < 0)
    {
        perror("FORK ERROR");
        exit(1);
    }
    else
    {
        while(1)
        {
    	    sleep(1);
    	    now_time = time(NULL);
            cout<<"The child is talking at "<<now_time<<endl;
        }
    }
    
    return 0;
}
