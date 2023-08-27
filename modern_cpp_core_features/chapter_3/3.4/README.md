# 返回类型推导

C++14 标准支持对返回类型声明为 auto 的推导

```c++
auto sum(int a, int b) { return a + b; }
```

注意：如果有多重返回值，那么需要保证返回值类型是相同的

```c++
auto sum(long a, long b) {
    if(a < 0) {
        return 0;	// 返回 int 类型
    } else {
        return a + b;	// 返回 long 类型
    }
}
```

以上代码有两处返回，return 0 返回的是 int 类型，而 return a + b 返回的是 long 类型，这种不同返回类型会导致编译失败。