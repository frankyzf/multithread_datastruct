//
// Created by feng on 10/11/15.
//

#ifndef MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_H
#define MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_H

namespace multithread{

    template <typename  T>
    class queue{
    private:
        mutable std::mutex mu;
        std::condition_variable cv;
        std::queue<T> data;
    public:
        bool empty()const{
            std::lock_guard<std::mutex> lk(mu);
            return data.empty();
        }
        void push(T value){
            std::lock_guard <std::mutex> lk(mu);
            data.push(move(value));
            cv.notify_one();
        }
        T pop_and_wait(){
            std::unique_lock <std::mutex> ul(mu);
            cv.wait(ul, [this]{return !data.empty();});
            T r = move(data.front());
            data.pop();
            return r;
        }
        T try_pop(){
            std::lock_guard <std::mutex> lk(mu);
            if(data.empty()) {
                throw logic_error("empty container");
            }
            T r = data.front();
            data.pop();
            return r;
        }
    };
};

#endif //MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_H
