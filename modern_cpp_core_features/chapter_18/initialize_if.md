# 支持初始化语句的if和switch（C++17）

## 18.1 支持初始化语句的if

在C++17标准中，if控制结构可以在执行条件语句之前执行一个初始化语句，语法如下：

```c++
if(init; condition) {}
```

其中 init 是初始化语句，condition 是条件语句，它们之间使用分号分割。

注意：
如果else if中有初始化语句，生命周期只在此处及以后。

本质上来说初始化语句就是在执行条件判断之前先执行一个初始化语句，并且语句中的变量将拥有与if结构相同的生命周期。

例子：

```c++
#include <cstdio>
#include <string>

int main() {
	std::string str;
    if(char buf[10]{0}; std::fgets(buf, 10, stdin)) {
        str += buf;
    }
}
```

## 18.2 支持初始化语句的switch

与if结构一样。

例子：

```c++
#include <conditional_variable>
#include <chrono>
using namespace std::chrono_literals;

std::condition_variable cv;
std::mutex cv_m;

int main() {
	switch(std::unique_lock<std::mutex> lk(cv_m); cv.wait(lk, 100ms)) {
        case std::cv_status::timeout:
            break;
     	case: std::cv_status::no_timeout:
            break;
    }
}
```

