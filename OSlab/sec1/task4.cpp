#include<iostream>
#include<ctime>
#include<unistd.h>
#include<thread>
using namespace std;


int func(int *p)
{
	while(1)
	{
        sleep(1);
        (*p)--;
        cout<<"child"<<*p<<endl;
	}
	return 0;
}

int main() 
{
	int shared_var = 0;
    thread t(func, &shared_var);
    while(1)
    {
    	sleep(1);
    	shared_var++;
        cout<<"main:"<<shared_var<<endl;
    }
    t.join();
	return 0;
}