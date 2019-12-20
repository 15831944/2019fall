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
#include<mutex>
using namespace std;

mutex mtx;
mutex wmtx;

void reader(int * rdr)
{
	int reader = 0;
	mtx.lock();
	reader++;
	(*rdr)++;
	time_t mytime0 = time(NULL);
	char * time_str0 = ctime(&mytime0);
	time_str0[strlen(time_str0)-1] = '\0';
	printf("%s: Creating %dth reader.\n", time_str0, *rdr);
	if(reader == 1) wmtx.lock();
	time_t mytime1 = time(NULL);
	char * time_str1 = ctime(&mytime1);
	time_str1[strlen(time_str1)-1] = '\0';
	printf("%s: NO.%d reader requires reading.\n", time_str1, *rdr);
	mtx.unlock();
	sleep(1);
	time_t mytime2 = time(NULL);
	char * time_str2 = ctime(&mytime2);
	time_str2[strlen(time_str2)-1] = '\0';
	printf("%s: End of NO.%d reader for reading.\n", time_str2, *rdr);
	mtx.lock();
	reader--;
	if(reader == 0) wmtx.unlock();
	mtx.unlock();
}

void writer(int * wtr)
{
	(*wtr)++;
	time_t mytime3 = time(NULL);
	char * time_str3 = ctime(&mytime3);
	time_str3[strlen(time_str3)-1] = '\0';
	printf("%s: Creating %dth writer.\n", time_str3, *wtr);
	time_t mytime4 = time(NULL);
	char * time_str4 = ctime(&mytime4);
	time_str4[strlen(time_str4)-1] = '\0';
	printf("%s: NO.%d writer requires writing.\n", time_str4, *wtr);
	wmtx.lock();
	sleep(6);
	time_t mytime5 = time(NULL);
	char * time_str5 = ctime(&mytime5);
	time_str5[strlen(time_str5)-1] = '\0';
	printf("%s: End of NO.%d writer for writing.\n", time_str5, *wtr);
	wmtx.unlock();
}

int main()
{
	ifstream fin("come_seq.txt");
	int j = 0;
	int rdr = 0;
	int wtr = 0;
	int comeseq[20];
	do{
		int buffer;
		fin >> buffer;
		comeseq[j] = buffer;
		j++;
	}while(j < 20);
	for(int i = 0; i < 20; i++)
	{
		sleep(2);
		if(comeseq[i] == 0){
			thread r(reader, &rdr);
			r.join();
		}else{
			thread w(writer, &wtr);
		    w.join();
		}
	}
	return 0;
}
