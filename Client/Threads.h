#pragma once

#include <thread>
#include <cstring>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <algorithm>

using namespace std;


class Threads_worker {

    public:

        mutex mtx;
        condition_variable cv;

        char a;
        char b;

        char msg[1024];
        char msg2[1024];
        bool checker(char *msg);

        //bool cmp(char a, char b);

        void sort_desc(char *msg);

        void replaceKB(char *msg);

        void entermsg(char *msg);

        void thread1(char *msg);

        void prepare(char *msg);

        void thread2(char *msg);

        void runThreads();

        thread spawn1() {
            return thread( [=] { thread1(msg); } );
        }
        thread spawn2() {
            return thread( [=] { thread2(msg); } );
        }

};

