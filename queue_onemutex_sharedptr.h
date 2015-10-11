//
// Created by feng on 10/11/15.
//

#ifndef MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_SHAREDPTR_H
#define MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_SHAREDPTR_H

namespace multithread{
    template <typename T>
    class queue_sharedptr{
    private:
        mutable std::mutex mu;
        std::condition_variable cv;
        queue<shared_ptr<T>> data;
    public:
        bool empty() const {
            std::lock_guard <std::mutex> lk(mu);
            return data.empty();
        }
        void push(T value){
            auto sp = make_shared<T>(value);
            {
                lock_guard<mutex> lk(mu);
                data.push(sp);
            }
            cv.notify_one();
        }

        shared_ptr<T> pop_and_wait(){
            unique_lock <mutex> ul(mu);
            cv.wait(ul, [this]{return !data.empty();});
            auto sp = move(data.front());
            data.pop();
            return sp;
        }

        shared_ptr<T> try_pop(){
            lock_guard <mutex> lk(mu);
            if(data.empty())
                throw logic_error("empty");
            auto sp = data.front();
            data.pop();
            return sp;
        }
    };
};


#endif //MULTITHREAD_DATASTRUCT_QUEUE_ONEMUTEX_SHAREDPTR_H
