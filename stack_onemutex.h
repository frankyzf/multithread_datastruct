//
// Created by feng on 10/11/15.
//

#ifndef MULTITHREAD_DATASTRUCT_STACK_ONEMUTEX_H
#define MULTITHREAD_DATASTRUCT_STACK_ONEMUTEX_H

#include "common.h"

namespace multithread_mutex{
  template <typename T>
  class stack{
    private:
        mutable std::mutex mu;
        std::condition_variable cv;
        std::stack<T> data;
    public:
        bool empty() const {
            std::lock_guard<std::mutex> lock(mu);
            return data.empty();
        }

        void push(T value){
            std::lock_guard<std::mutex> lock(mu);
            data.emplace(std::move(value));
            cv.notify_one();
        }

        T pop_and_wait(){
            std::unique_lock<std::mutex> ul(mu);
            cv.wait(ul, [this]{return !data.empty();});
            T r = move(data.top());
            data.pop();
            return r;
        }

        T try_pop(){
            std::lock_guard<std::mutex> lock(mu);
            if(data.empty()){
                throw logic_error("empty container");
            }
            T r = move(data.top());
            data.pop();
            return r;
        }

  };

};


#endif //MULTITHREAD_DATASTRUCT_STACK_ONEMUTEX_H
