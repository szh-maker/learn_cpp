# 使用函数返回类型后置声明函数

当返回类型比较复杂的时候，比如返回一个函数指针类型，返回类型后置可能会是一个不错的选择，例如：

```c++
int bat_impl(int x) { return x; }

typedef int(*bar)(int);
bar fool() {
    return bar_impl;
}

auto foo2()->int(*)(int) {
    return bar_impl;
}

int main() {
    auto func = foo2();
    func(8);
}
```

上面代码中，函数 foo2 的返回值不再是简单的 int 而是函数指针类型。使用传统的函数声明语法的 fool 无法将函数指针类型作为返回类型直接使用，所以需要使用 typedef 给函数指针类型创建别名 bar，再使用别名作为函数 fool 的返回值类型。而是用函数返回值后值语法的 foo2 则没有这个问题。同样 auto 作为返回值类型占位符，在 -> 后声明返回的函数指针类型 int(*)(int) 即可。