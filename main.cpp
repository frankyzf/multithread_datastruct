#include "common.h"
//not checked yet

#include "queue_onemutex_sharedptr.h"

int main() {

    multithread::queue_sharedptr<int> q;
    q.push(1);
    cout << *q.try_pop() << endl;
    return 1;
}