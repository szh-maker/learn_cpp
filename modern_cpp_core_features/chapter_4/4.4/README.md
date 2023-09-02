# cv 限定符推导

通常情况下，decltype(e) 所推导的类型会同步 e 的 cv 限定符，比如：

```c++
const int i = 0;
decltype(i);	// decltype(i) 推导类型为 const int
```

但是还有特殊情况，当 e 是未加括号的成员变量时，父对象表达式的 cv 限定符会被忽略，不能同步到推到结果：

```c++
struct A {
    double x;
};
const A* a = new A();
decltype(a->x);	// decltype(a->x) 推导类型为 double，const 属性被忽略
```

在上述代码中，a 被声明为 const 类型，如果想在代码中改变 a 中的 x 的值，则肯定编译会失败。但是 decltype(a->x) 却得到一个没有 const double类型。当然，如果给 a->x 加上括号，则情况会有所不同：

```c++
struct A {
    double x;
};
const A* a = new A();
decltype((a->x));	// decltype((a->x)) 推导类型为 const double&
```

总的来说，当 e 是加括号的数据成员时，父对象表达式的 cv 限定符会同步到推断结果。