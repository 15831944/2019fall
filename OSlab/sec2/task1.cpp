#include<iostream>
#include<fstream>
#include<ctime>
#include<unistd.h>
#include<thread>
#include<stdio.h>
#include<time.h>
#include<string>
#include<string.h>
#include<pthread.h>
#include<queue>
using namespace std;

struct sem
{
	int counter;
	queue<int> Q;
};

typedef struct sem S;

int intervals[20];
S barber_ready;
S cust_ready;
S mutex;


void Down(S *a)
{
    if((a->counter) > 0)
    	(a->counter)--;
}
void Up(S *b)
{
    (b->counter)++;
}

void customers(int *chairs)
{
	time_t mytime = time(NULL);
	char * time_str = ctime(&mytime);
	time_str[strlen(time_str)-1] = '\0';
	Down(&mutex);
	if((*chairs) > 0)
	{
		(*chairs)--;
		printf("%s: One customer comes, now there are %d seats left now.\n", time_str, *chairs);
		Up(&cust_ready);
		Up(&mutex);
		Down(&barber_ready);
	}else{
		printf("%s: One customer leaves, now there are %d seats left now.\n", time_str, *chairs);
		Up(&mutex);
	}
}

void barber(int *chairs)
{
	do
	{
		Down(&cust_ready);
		Down(&mutex);
		(*chairs)++;
		time_t mytime0 = time(NULL);
		char * time_str0 = ctime(&mytime0);
		time_str0[strlen(time_str0)-1] = '\0';
		printf("%s: Barber works, there are %d seats left now.\n", time_str0, *chairs);
		Up(&barber_ready);
		Up(&mutex);
		usleep(500000);
		time_t mytime = time(NULL);
		char * time_str = ctime(&mytime);
		time_str[strlen(time_str)-1] = '\0';
		printf("%s: Barber has cut hair, there are %d seats left now.\n", time_str, *chairs);
	}while((*chairs) < 6);
}

int main()
{
	mutex.counter = 1;
	barber_ready.counter = 0;
	cust_ready.counter = 0;
	ifstream fin("sleeping_data.txt");
	int j = 0;
	do{
		int buffer;
		fin >> buffer;
        intervals[j] = buffer;
        j++;
	}while(j < 20);
	int chairs = 6;
	printf("There are %d seats left.\n", chairs);
	thread c(customers, &chairs);
	sleep(0.5);
	thread b(barber, &chairs);
	for (int i = 0; i < 20; i++)
	{
		usleep(intervals[i]);
		thread c(customers, &chairs);
		c.join();
	}
	c.join();
	b.join();
	return 0;
}
