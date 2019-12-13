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
#include<mutex>
using namespace std;


int reader_count = 0, writer_count = 0;

mutex fmtx;
mutex rdcntmtx;
mutex wtcntmtx;
mutex queue;


void reader(){
    while(1){
        queue.lock();
        rdcntmtx.lock();
        if( reader_count == 0 ) { fmtx.lock(); }
        reader_count = reader_count + 1;
        rdcntmtx.unlock();
        queue.unlock();
        //Do read operation ...
        sleep(1);
        printf("read\n");
        rdcntmtx.lock();
        reader_count = reader_count - 1;
        if( reader_count == 0 ) { fmtx.unlock(); }
        rdcntmtx.unlock();
    }
}

void writer(){
    while(1){
        queue.lock();
        fmtx.lock();
        queue.unlock();
        sleep(6);
        printf("write\n");
        fmtx.unlock();
    }
}

int main(){
    thread t1(reader);
    thread t2(writer);
    t1.join();
    t2.join();
    return 0;
}