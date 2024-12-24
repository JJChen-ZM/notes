#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> dataQueue;

void producer()
{
	for(int i=0;i<100;i++)
	{
		std::unique_lock<std::mutex> lck(mtx);
        dataQueue.push(i);
        std::cout<<"producer: "<<i<<std::endl;
	}
    cv.notify_all(); // 唤醒所有等待的消费者线程
}

void consumer(int id)
{   
    while(true){
        {
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, []{return !dataQueue.empty();}); // 等待队列非空
            int data = dataQueue.front();
            dataQueue.pop();
            std::cout << "Consumer " << id << " consumed: " << data << std::endl;
            if(data == 99)
                break;
        }
    }
    
}

int main(){
    std::thread producerThread(producer);
    std::thread consumerThread1(consumer,1);
    std::thread consumerThread2(consumer,3);

    producerThread.join();
    consumerThread1.join();
    consumerThread2.join();
    return 0;
}