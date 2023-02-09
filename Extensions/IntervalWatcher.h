//
// Created by 58226 on 2023/2/8.
//

#ifndef INTERVALWATCHER_H
#define INTERVALWATCHER_H
#include<chrono>
class IntervalWatcher
{

public:
    explicit IntervalWatcher(bool isStartAuto = false): t1(res::zero())
        , t2(res::zero()) {
        if(isStartAuto)
            start();
    }

    void start()
    {
        t1 = clock::now();
    }

    void stop() {
        t2 = clock::now();
    }


    double milliSecond()
    {
       return ((double)(std::chrono::duration_cast<res>(t2 - t1).count()) / 1e3);
    }

private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds res;
    clock::time_point t1;
    clock::time_point t2;
};


#endif //INTERVALWATCHER_H
