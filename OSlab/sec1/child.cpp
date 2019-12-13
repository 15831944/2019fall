#include<iostream>
#include<ctime>
#include<unistd.h>
using namespace std;

int main()
{
    time_t now_time;
    while(1){
    	sleep(1);
    	now_time = time(NULL);
        cout<<"The child is talking at "<<now_time<<endl;
    }
    return 0;
}
