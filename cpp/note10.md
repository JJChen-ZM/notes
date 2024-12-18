## C++ 例子：利用状态机改进代码
好的，我可以提供一个例子来展示如何利用状态机改进代码。假设我们有一个简单的状态机来管理设备的状态。我们将定义一个状态机类，它可以处理不同的状态转换和相应的行为。

### 示例场景
假设我们有一个设备，它可以处于以下几种状态：
- 关机 (`Off`)
- 启动 (`Starting`)
- 运行 (`Running`)
- 关机中 (`Stopping`)

### 状态机实现

首先，定义状态枚举和状态机类：

```cpp
#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>

// 定义状态枚举
enum class State {
    Off,
    Starting,
    Running,
    Stopping
};

// 状态机类
class StateMachine {
public:
    StateMachine() : currentState(State::Off) {}

    void addTransition(State from, State to, std::function<void()> action) {
        transitions[{from, to}] = action;
    }

    void transitionTo(State newState) {
        auto key = std::make_pair(currentState, newState);
        if (transitions.find(key) != transitions.end()) {
            transitions[key]();
            currentState = newState;
            std::cout << "Transitioned to state: " << stateToString(newState) << std::endl;
        } else {
            std::cout << "Invalid transition from " << stateToString(currentState)
                      << " to " << stateToString(newState) << std::endl;
        }
    }

    State getCurrentState() const {
        return currentState;
    }

private:
    std::string stateToString(State state) const {
        switch (state) {
            case State::Off: return "Off";
            case State::Starting: return "Starting";
            case State::Running: return "Running";
            case State::Stopping: return "Stopping";
            default: return "Unknown";
        }
    }

    State currentState;
    std::unordered_map<std::pair<State, State>, std::function<void()>, boost::hash<std::pair<State, State>>> transitions;
};
```

### 使用状态机类

现在我们可以使用这个状态机类来管理设备的状态：

```cpp
int main() {
    StateMachine machine;

    // 添加状态转换和对应的动作
    machine.addTransition(State::Off, State::Starting, []() {
        std::cout << "Starting the device..." << std::endl;
    });
    machine.addTransition(State::Starting, State::Running, []() {
        std::cout << "Device is now running." << std::endl;
    });
    machine.addTransition(State::Running, State::Stopping, []() {
        std::cout << "Stopping the device..." << std::endl;
    });
    machine.addTransition(State::Stopping, State::Off, []() {
        std::cout << "Device is now off." << std::endl;
    });

    // 进行状态转换
    machine.transitionTo(State::Starting);
    machine.transitionTo(State::Running);
    machine.transitionTo(State::Stopping);
    machine.transitionTo(State::Off);

    return 0;
}
```

### 结果
```
Starting the device...
Transitioned to state: Starting
Device is now running.
Transitioned to state: Running
Stopping the device...
Transitioned to state: Stopping
Device is now off.
Transitioned to state: Off
```

在这个示例中，状态机类管理设备的状态，并确保只允许有效的状态转换。这种方法可以使代码更加模块化和易于维护。