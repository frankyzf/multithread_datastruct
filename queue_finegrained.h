//
// Created by feng on 10/11/15.
//

#ifndef MULTITHREAD_DATASTRUCT_QUEUE_FINEGRAINED_H
#define MULTITHREAD_DATASTRUCT_QUEUE_FINEGRAINED_H

namespace multithread{
    template<typename  T>
    class queue_list{
        private:
            struct node{
                shared_ptr<T> data;
                unique_ptr<node> next;
            };
            unique_ptr<node> head;
            node* tail;
            mutable mutex headmu;
            mutable mutex tailmu;
    public:
        queue_list():head(new node()), tail(head.get()){}
        queue_list(const queue_list&) = delete;
        queue_list& operator=(const queue_list&) = delete;

        bool empty() const {
            lock_guard <mutex> headlk(headmu);
            lock_guard <mutex> taillk(tailmu);
            return head.get() == tail;
        }

        void push(T value){
            auto sp = make_shared<T>(move(value));
            unique_ptr<node> up(new node);
            auto p = up.get();
            lock_gurad <mutex> taillk(tailmu);
            tail->data = sp;
            tail->next = move(up);
            tail = p;
        }


        shared_ptr<T> try_pop(){
            lock_guard <mutex> headlk(headmu);
            lock_guard <mutex> taillk(tailmu);
            if(head.get() == tail){
                return nullptr;
            }
            auto up = move(head);
            head = move(up->next);
            return head;
        }

    };
};

#endif //MULTITHREAD_DATASTRUCT_QUEUE_FINEGRAINED_H
