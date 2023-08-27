# lambda 表达式中使用 auto 类型推导

在 C++14 标准中我们还可以把 auto 写到 lambda 表达式的形参中，此时可以得到一个泛型的 lambda 表达式。

```c++
auto l = [](auto a, auto b) { return a * b; };
auto res = l(5, 5.0);
```

上面代码中 a 被推导为 int 类型，b 被推导为 double 类型，返回值 res 被推导为 double 类型。

lambda 表达式返回 auto 引用的方法：

```c++
auto l = [](int& a)->auto& { return a; };
auto x1 = 5;
auto& x2 = l(x1);
assert(&x1 == &x2);	// 具有相同的内存地址
```

起初在后置返回类型中使用 auto 是不允许的，但是后来发现，这是唯一让 lambda 表达式通过推导返回引用类型的方法了。