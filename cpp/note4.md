### lock_guard 和 unique_lock
`std::lock_guard` 和 `std::unique_lock` 都是 C++ 标准库中用于管理互斥锁的 RAII（Resource Acquisition Is Initialization）类，它们在不同场景下提供了不同的功能和灵活性。

1. `std::lock_guard`：
   - `std::lock_guard` 是一个简单的互斥锁管理类，用于自动管理互斥锁的上锁和解锁。它适用于简单的锁定情况，即在作用域结束时自动释放锁。一旦 `std::lock_guard` 对象被创建，它会立即锁定互斥锁，并在其作用域结束时自动解锁。

   - `std::lock_guard` 的构造函数接受一个互斥锁作为参数，并在构造时对该互斥锁上锁，析构时自动解锁。因此，`std::lock_guard` 不能手动解锁，它的生命周期控制了锁的持有时间。

   - 由于 `std::lock_guard` 自动上锁和解锁，所以更适合用于需要在较小作用域内使用锁的情况，使得代码更简洁和安全。

   以下是使用 `std::lock_guard` 的示例：

   ```cpp
   #include <iostream>
   #include <thread>
   #include <mutex>

   std::mutex mtx; // 互斥锁

   void worker() {
       std::lock_guard<std::mutex> lock(mtx); // 自动上锁和解锁

       // 互斥锁作用域内的代码
       // ...
   }

   int main() {
       std::thread t(worker);
       t.join();
       return 0;
   }
   ```

2. `std::unique_lock`：
   - `std::unique_lock` 是一个更灵活的互斥锁管理类，它提供了更多的功能，包括手动上锁和解锁，条件变量支持，以及延迟上锁等。因为它提供了手动上锁和解锁的功能，所以在需要更复杂锁定和解锁逻辑的情况下，`std::unique_lock` 更为适用。

   - `std::unique_lock` 的构造函数接受一个互斥锁作为参数，并可以选择在构造时锁定（`std::defer_lock`）或不锁定（`std::try_to_lock`）。还可以在构造后通过 `lock()` 手动上锁，`unlock()` 手动解锁。

   - `std::unique_lock` 还可以与条件变量（`std::condition_variable`）结合使用，更方便地实现等待和通知线程。

   以下是使用 `std::unique_lock` 的示例：

   ```cpp
   #include <iostream>
   #include <thread>
   #include <mutex>

   std::mutex mtx; // 互斥锁

   void worker() {
       std::unique_lock<std::mutex> lock(mtx); // 不自动上锁

       // 手动上锁
       lock.lock();

       // 互斥锁作用域内的代码
       // ...

       // 手动解锁
       lock.unlock();
   }

   int main() {
       std::thread t(worker);
       t.join();
       return 0;
   }
   ```

总结来说，`std::lock_guard` 适用于简单的自动上锁和解锁场景，而 `std::unique_lock` 则适用于需要更灵活锁定和解锁操作，或与条件变量一起使用的场景。根据具体需求，选择合适的互斥锁管理类可以更好地管理线程间的互斥和同步。

### std::condition_variable notify_one()
`std::condition_variable` 是 C++ 标准库中用于线程间同步的类，它可以配合互斥锁（`std::mutex`）来实现线程间的等待和唤醒。`std::condition_variable` 提供了 `notify_one()` 方法，用于通知等待在条件变量上的一个线程继续执行。

`notify_one()` 方法的作用是唤醒一个等待在条件变量上的线程。如果没有线程在条件变量上等待，调用 `notify_one()` 不会有任何效果。如果有多个线程等待在条件变量上，`notify_one()` 只会随机选择一个线程进行唤醒。

以下是一个简单示例，演示了如何使用 `std::condition_variable` 的 `notify_one()` 方法：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;  // 互斥锁
std::condition_variable cv; // 条件变量
bool ready = false; // 全局标志变量

void worker() {
    std::unique_lock<std::mutex> lock(mtx);

    // 等待条件满足
    cv.wait(lock, [] { return ready; });

    // 条件满足后执行的操作
    std::cout << "Worker thread: Condition satisfied." << std::endl;
}

int main() {
    std::thread t(worker);

    // 等待一段时间，模拟其他任务的执行
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 设置条件为 true，并通知一个等待线程
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();

    // 等待 worker 线程执行完成
    t.join();

    return 0;
}
```

在上述示例中，我们创建了一个条件变量 `cv` 和一个互斥锁 `mtx`，以及一个全局标志变量 `ready`。在 `worker()` 函数中，线程会等待条件变量满足，如果条件不满足，则线程会阻塞在 `cv.wait()` 调用处。在 `main()` 函数中，我们先启动了一个线程 `t`，然后等待一段时间模拟其他任务的执行。接着，我们设置条件变量为 `true` 并调用 `cv.notify_one()` 来唤醒一个等待线程（如果有的话）。

请注意，在调用 `cv.wait()` 之前必须获取互斥锁 `mtx`，这样才能保证等待的过程是线程安全的。在设置条件变量和调用 `cv.notify_one()` 之前，同样也要获取互斥锁，以保证条件变量的修改是原子的。

使用 `std::condition_variable` 可以更加灵活地实现线程间的通信和同步，以满足不同的场景需求。