# 线程局部存储（C++11）

## 操作系统和编译器对线程局部存储的支持

线程局部存储是指对象内存在线程开始后分配，线程结束时回收且每个线程有该对象自己的实例，简单说，线程局部存储对象都是独立于各个线程的。

在Windows中可以通过调用API函数 TlsAlloc 来分配一个未使用的线程局部槽索引（TLS slot index），这个索引实际上是 Windows 内部线程环境块（TEB）中线程局部存储数组的索引。通过API函数 TlsGetValue 与 TlsSetValue 可以获取和设置线程局部存储数组对于索引元素的值。API函数 TlsFree 用于释放线程局部存储槽索引。

Linux使用了 pthreads（POSIX threads）作为线程接口，在 pthreads 中可以调用 pthread_key_create 与 pthread_key_delete 创建与删除一个类型为 pthread_key_t 的键。利用这个建可以使用 pthread_setspecific 函数设置线程相关的数据，当然，随后还能通过 pthread_getspecific 函数获取之前设置的内存数据。

在C++11标准之前，各个编译器也用了自定义的方法支持线程局部存储。如 gcc 和 clang 添加关键字 __thread 来声明线程局部存储变量，而 MSVC 则是使用 `__declspec(thread)`。C++标准委员会在C++11标准中添加了新的 thread_local 说明符来声明线程局部存储变量。

## thread_loacl 说明符

thread_local 说明符可以用来声明线程生命周期的对象，它能与 static 或 extern 结合，分别指定内部或外部链接，不过额外的 static 并不影响对象的生命周期。换句话说，static 并不影响其线程局部存储的属性：

```c++
struct X {
    thread_local static int i;
};

thread_local X a;

int main() {
	thread_local X b;
}
```

被 thread_local 生命的变量在行为上非常像静态变量，只不过多了线程属性，当然这也是线程局部存储能出现的关键原因，它能解决全局变量或者静态变量在多线程操作中存在的问题，一个典型的就是 errno。直到C++11之前，errno都是一个静态变量，而从C++11之后errno就是一个线程局部存储变量。

线程局部存储只是定义了对象的生命周期，而没有定义可访问性。也就是说，可以获取线程局部存储变量的地址并将其传递给其他线程，并且其他线程可以在其生命周期内使用变量。不过这样做除了用于诊断功能以外没有实际意义，而且风险性过大，一旦没有掌握好目标线程的生命周期，就很可能导致内存访问异常，造成未定义的程序行为，通常情况下程序崩溃。

> 注意：
>
> 使用取地址 & 取到的线程局部存储变量的地址是运行时被计算出来的，它不是一个常量，也就是无法和 constexpr 结合：
>
> ```c++
> thread_local int tv;
> static int sv;
> 
> int main() {
>     constexpr int* sp = &sv;	// 编译成功
>     constexpr int* tp = &tv;	// 编译1失败
> }
> ```

说明一下线程局部对象的初始化和销毁。在同一个线程中，一个线程局部存储对象只会初始化一次，即使在某个函数中被多次调用。这一点和单线程程序中的静态对象非常相似。相对应的，对象销毁也只会发生一次，通常发生在线程退出的时刻。

```c++
#include <thread>
#include <iostream>
#include <string>
#include <mutex>

std::mutex g_out_mutex;

struct RefCount {
    RefCount(const char* f) : i(0), func(f) {
        std::lock_guard<std::mutex> lock(g_out_mutex);
        std::cout << std::this_thread::get_id() 
            << "|" << func 
            << " : ctor i(" << i << ")" << std::endl;
    }

    ~RefCount() {
        std::lock_guard<std::mutex> lock(g_out_mutex);
        std::cout << std::this_thread::get_id()
            << "|" << func
            << " : dtor i(" << i << ")" << std::endl;
    }

    void inc() {
        std::lock_guard<std::mutex> lock(g_out_mutex);
        std::cout << std::this_thread::get_id()
            << "|" << func
            << " : ref count add 1 to i(" << i << ")" << std::endl;
        ++i;
    }

private:
    int i;
    std::string func;
};

RefCount* lp_ptr = nullptr;

void foo(const char* f) {
    std::string func(f);
    thread_local RefCount tv(func.append("#foo").c_str());
    tv.inc();
}

void bar(const char* f) {
    std::string func(f);
    thread_local RefCount tv(func.append("#bar").c_str());
    tv.inc();
}

void threadfunc1() {
    const char* func = "threadfunc1";
    foo(func);
    foo(func);
    foo(func);
}

void threadfunc2() {
    const char* func = "threadfunc2";
    foo(func);
    foo(func);
    foo(func);
}

void threadfunc3() {
    const char* func = "threadfunc3";
    foo(func);
    bar(func);
    bar(func);
}

int main() {
    std::thread t1(threadfunc1);
    std::thread t2(threadfunc2);
    std::thread t3(threadfunc3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
```

输出结果：

```c++
2|threadfunc1#foo : ctor i(0)
2|threadfunc1#foo : ref count add 1 to i(0)
4|threadfunc3#foo : ctor i(0)
4|threadfunc3#foo : ref count add 1 to i(0)
2|threadfunc1#foo : ref count add 1 to i(1)
2|threadfunc1#foo : ref count add 1 to i(2)
4|threadfunc3#bar : ctor i(0)
4|threadfunc3#bar : ref count add 1 to i(0)
4|threadfunc3#bar : ref count add 1 to i(1)
3|threadfunc2#foo : ctor i(0)
3|threadfunc2#foo : ref count add 1 to i(0)
3|threadfunc2#foo : ref count add 1 to i(1)
3|threadfunc2#foo : ref count add 1 to i(2)
2|threadfunc1#foo : dtor i(3)
4|threadfunc3#bar : dtor i(2)
4|threadfunc3#foo : dtor i(1)
3|threadfunc2#foo : dtor i(3)
```



