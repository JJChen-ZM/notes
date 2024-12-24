## C++多线程编程
C++ 多线程编程允许程序在同一时间执行多个任务，从而提高程序的性能和响应性。C++11 引入了标准库 `<thread>`，提供了对多线程编程的支持。以下是 C++ 多线程编程的基本概念和示例：

### 1. 线程创建与启动
要创建一个线程，需要包含 `<thread>` 头文件，并使用 `std::thread` 类。可以通过向 `std::thread` 的构造函数传递一个可调用对象（如函数、函数对象或 lambda 表达式）来指定线程要执行的任务。

```cpp
#include <iostream>
#include <thread>

// 线程函数
void hello() {
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::thread t(hello); // 创建并启动线程
    std::cout << "Main thread " << std::this_thread::get_id() << std::endl;

    t.join(); // 等待线程 t 完成

    return 0;
}
```

在这个例子中，`std::thread t(hello);` 创建了一个新线程，并让它执行 `hello` 函数。`t.join()` 用于等待线程 `t` 完成执行，防止主线程在子线程之前结束。

### 2. 传递参数给线程函数
可以向线程函数传递参数，就像调用普通函数一样。

```cpp
#include <iostream>
#include <thread>

// 带参数的线程函数
void print_id(int id) {
    std::cout << "Thread " << id << std::endl;
}

int main() {
    std::thread threads[5];

    // 创建并启动 5 个线程
    for (int i = 0; i < 5; ++i) {
        threads[i] = std::thread(print_id, i);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
```

### 3. 线程同步
当多个线程访问共享资源时，可能会出现数据竞争和不一致的问题。为了解决这些问题，需要使用线程同步机制，如互斥锁（`std::mutex`）、条件变量（`std::condition_variable`）和信号量（`std::counting_semaphore`）等。

#### 互斥锁（`std::mutex`）
互斥锁用于保护共享资源，确保同一时间只有一个线程可以访问。

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; // 创建一个互斥锁
int shared_variable = 0;

// 线程函数
void increment() {
    for (int i = 0; i < 1000000; ++i) {
        mtx.lock(); // 锁定互斥锁
        shared_variable++;
        mtx.unlock(); // 解锁互斥锁
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Final value: " << shared_variable << std::endl;

    return 0;
}
```

在这个例子中，`mtx.lock()` 和 `mtx.unlock()` 用于保护 `shared_variable` 的访问，防止多个线程同时修改它。

#### 条件变量（`std::condition_variable`）
条件变量用于线程间的同步，一个线程可以等待某个条件满足，而另一个线程可以通知这个条件已经满足。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// 线程函数 1
void print_id(int id) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck); // 等待条件变量
    std::cout << "thread " << id << '\n';
}

// 线程函数 2
void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    std::cout << "Notifying...\n";
    cv.notify_all(); // 通知所有等待的线程
}

int main() {
    std::thread threads[10];
    for (int i = 0; i < 10; ++i) {
        threads[i] = std::thread(print_id, i);
    }

    std::cout << "10 threads ready to race...\n";
    go();

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
```

### 4. 线程安全的对象
可以通过封装互斥锁和条件变量，创建线程安全的对象。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        cv_.notify_one();
    }

    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return!queue_.empty(); });
        if (queue_.empty()) return false;
        value = queue_.front();
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

int main() {
    ThreadSafeQueue<int> tsq;

    std::thread producer([&tsq]() {
        for (int i = 0; i < 10; ++i) {
            tsq.push(i);
        }
    });

    std::thread consumer([&tsq]() {
        int value;
        while (true) {
            if (tsq.pop(value)) {
                std::cout << "Consumed: " << value << std::endl;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
```

以上是 C++ 多线程编程的基本内容，多线程编程需要谨慎处理同步和资源管理问题，以避免死锁和数据竞争等问题。  






## notify_all 和 notify_once
在 C++ 的多线程编程中，`std::condition_variable` 的 `notify_all` 和 `notify_once` 是两个用于通知等待线程的成员函数，它们在功能和使用场景上存在一些区别。

### 1. `notify_all`
- **功能**：`notify_all` 函数会唤醒所有在 `std::condition_variable` 上等待的线程。当调用 `notify_all` 时，所有通过 `wait` 或 `wait_for` 或 `wait_until` 函数阻塞在该条件变量上的线程都会被唤醒，这些线程会竞争获取相关的互斥锁，一旦获得锁，就会继续执行。
- **使用场景**：当某个条件的改变会影响到所有等待线程时，适合使用 `notify_all`。例如，在一个生产者 - 消费者模型中，如果生产者向共享队列中放入了一批数据，并且所有等待的消费者都需要处理这些新数据，那么生产者就可以调用 `notify_all` 来唤醒所有等待的消费者线程。

下面是一个使用 `notify_all` 的简单示例：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> dataQueue;

// 生产者线程函数
void producer() {
    for (int i = 0; i < 5; ++i) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            dataQueue.push(i);
            std::cout << "Produced: " << i << std::endl;
        }
        cv.notify_all(); // 唤醒所有等待的消费者线程
    }
}

// 消费者线程函数
void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return!dataQueue.empty(); });
        int value = dataQueue.front();
        dataQueue.pop();
        std::cout << "Consumer " << id << " consumed: " << value << std::endl;
        if (value == 4) break;
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread1(consumer, 1);
    std::thread consumerThread2(consumer, 2);

    producerThread.join();
    consumerThread1.join();
    consumerThread2.join();

    return 0;
}
```

在这个例子中，生产者线程在向队列中添加数据后，调用 `notify_all` 唤醒所有等待的消费者线程，每个消费者线程在被唤醒后，从队列中取出数据并处理。

### 2. `notify_one`
- **功能**：`notify_one` 函数只会唤醒一个在 `std::condition_variable` 上等待的线程。如果有多个线程在等待，系统会随机选择一个线程进行唤醒。被唤醒的线程会竞争获取相关的互斥锁，获取到锁后继续执行。
- **使用场景**：当某个条件的改变只需要一个等待线程来处理时，适合使用 `notify_one`。例如，在一个单资源的访问控制场景中，只有一个线程可以获取到该资源进行处理，此时使用 `notify_one` 可以避免不必要的线程唤醒和竞争。

以下是一个使用 `notify_one` 的示例：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool resourceAvailable = false;

// 线程函数，尝试获取资源
void worker(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return resourceAvailable; });
    std::cout << "Thread " << id << " acquired the resource." << std::endl;
    resourceAvailable = false;
}

// 主线程，模拟资源可用
int main() {
    std::thread threads[3];
    for (int i = 0; i < 3; ++i) {
        threads[i] = std::thread(worker, i + 1);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::unique_lock<std::mutex> lock(mtx);
        resourceAvailable = true;
        std::cout << "Resource is available. Notifying one thread." << std::endl;
        cv.notify_one(); // 只唤醒一个等待线程
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
```

在这个例子中，只有一个线程会被唤醒并获取到资源，其他线程会继续等待，直到下次资源可用并再次调用 `notify_one`。

### 总结
`notify_all` 和 `notify_one` 都是 `std::condition_variable` 中用于通知等待线程的重要函数，具体使用哪个函数取决于你的应用场景和需求。合理使用这两个函数可以有效地提高多线程程序的性能和资源利用率。  




## cv.wait(lck,[]{return !dataQueue.empty();});
`cv.wait(lck,[]{return!dataQueue.empty();});` 这行代码是 C++ 多线程编程中使用条件变量（`std::condition_variable`）进行线程同步的关键部分。下面详细解释这行代码的含义和作用：

1. **`cv` 和 `lck` 的定义**：
    - `cv` 是一个 `std::condition_variable` 类型的对象，它用于线程间的同步，通常与一个 `std::mutex` 配合使用，以实现线程在某个条件满足时才继续执行。
    - `lck` 是一个 `std::unique_lock<std::mutex>` 类型的对象，它是一个对 `std::mutex` 的智能锁。`std::unique_lock` 提供了更灵活的锁管理方式，允许在需要时锁定和解锁互斥锁，并且在其作用域结束时会自动解锁。

2. **`cv.wait` 函数的基本形式**：
    - `cv.wait` 有两种重载形式。这里使用的是带谓词（predicate）的形式，其原型为 `void wait (unique_lock<mutex>& lock, Predicate pred);`。
    - 不带谓词的 `cv.wait` 调用会释放关联的互斥锁，并将当前线程阻塞，直到另一个线程通过 `notify_one` 或 `notify_all` 唤醒它。唤醒后，线程会重新获取互斥锁并继续执行。
    - 带谓词的 `cv.wait` 调用会先检查谓词 `pred` 的值。如果 `pred` 为 `false`，则会释放关联的互斥锁，并将当前线程阻塞，直到被其他线程唤醒。唤醒后，它会再次检查谓词 `pred` 的值。只有当 `pred` 为 `true` 时，线程才会继续执行；如果 `pred` 仍然为 `false`，线程会再次释放互斥锁并进入阻塞状态，等待下一次唤醒和检查。

3. **谓词 `[]{return!dataQueue.empty();}`**：
    - 这是一个匿名的 lambda 表达式，用作 `cv.wait` 的谓词。它的作用是检查 `dataQueue`（一个共享的队列）是否不为空。
    - `return!dataQueue.empty();` 表示当队列不为空时返回 `true`，否则返回 `false`。

4. **整体作用**：
    - 这行代码的作用是让当前线程在 `dataQueue` 为空时进入等待状态。当 `dataQueue` 为空时，`[]{return!dataQueue.empty();}` 返回 `false`，`cv.wait` 会释放 `lck` 所管理的互斥锁，并将当前线程阻塞。
    - 当其他线程向 `dataQueue` 中添加了数据后，通过调用 `cv.notify_one` 或 `cv.notify_all` 唤醒等待的线程。被唤醒的线程重新获取互斥锁，并再次检查 `[]{return!dataQueue.empty();}` 的值。如果此时 `dataQueue` 不为空，谓词返回 `true`，线程就会继续执行后续的代码，从 `dataQueue` 中取出数据进行处理。

通过这种方式，`cv.wait(lck,[]{return!dataQueue.empty();});` 确保了线程在共享资源（`dataQueue`）满足特定条件（不为空）时才进行操作，避免了线程在资源不可用时的无效操作，从而实现了线程间的同步和对共享资源的安全访问。  