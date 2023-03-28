#include "Threads.h"


using namespace std;


bool Threads_worker::checker(char *msg)
{
    int len = strlen(msg);
    if (len > 64)
    {
        return false;
    }
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(msg[i]))
        {
            return false;
        }
    }
    return true;
}

bool cmp(char a, char b)
{
    return a > b;
}

void Threads_worker::sort_desc(char *msg)
{
    int len = strlen(msg);
    sort(msg, msg + len, cmp);
}

void Threads_worker::replaceKB(char *msg)
{

    int len = strlen(msg);

    for (int i = 0; i < len; i++)
    {
        if (!(msg[i] % 2))
        {

            len = strlen(msg);
            for (int r = len; r > i; r--)
            {
                msg[r + 1] = msg[r];
            }
            msg[i] = 'K';
            msg[i + 1] = 'B';
            i++;
        }
    }
}

void Threads_worker::entermsg(char *msg)
{
    cout << "Введите сообщение: " << endl;
    cin >> msg;
}

void Threads_worker::thread1(char *msg)
{

    entermsg(msg);

    while(!checker(msg)){ 
        cout << "Not correct input, need to proceed only digits." << endl;
        entermsg(msg);
    }

    sort_desc(msg);
    replaceKB(msg);

    cv.notify_one();
}

void Threads_worker::prepare(char *msg)
{
    memset(msg2, '\0', sizeof(msg2));
    int number = 0;

    for (int i = 0; i < strlen(msg);i++){
        if (isdigit(msg[i])) {
            number += msg[i] - '0';
        }
    }

    sprintf(msg2,"%d", number);
    cout << "Data from thread2: " << msg << endl;
    cout << "Sum of all elements: " << msg2 << endl;
    memset(msg, '\0', sizeof(msg));

}

void Threads_worker::thread2(char *msg)
{
    unique_lock<mutex> lck2(mtx);
    cv.wait(lck2);

    prepare(msg);

}

void Threads_worker::runThreads()
{

    thread th1 = this->spawn1();
    thread th2 = this->spawn2();

    th1.join();
    th2.join();
    
}

