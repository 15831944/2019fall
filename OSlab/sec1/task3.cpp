#include<iostream>
#include<ctime>
#include<unistd.h>
#include<thread>
using namespace std;


void t_func(){
	time_t now_time;
	while(1)
	{
		sleep(1);
		now_time = time(NULL);
		cout<<"["<<this_thread::get_id()<<"]"<<"+"<<"["<<now_time<<"]"<<endl;
	}
}

int main()
{
	time_t now_time;
    std::thread t(t_func);
    while(1){
    	sleep(1);
    	now_time = time(NULL);
    	cout<<"["<<this_thread::get_id()<<"]"<<"+"<<"["<<now_time<<"]"<<endl;
    }
    t.join();
    return 0;
}
