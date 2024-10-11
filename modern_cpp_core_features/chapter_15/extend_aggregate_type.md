# 扩展的聚合类型（C++17  C++20）

## 15.1 聚合类型的新定义

C++17标准对聚合类型的定义做出了大幅的修改，即从基类公开且非虚继承的类也可能是一个聚合。同时聚合类型还需满足常规条件。

1. 没有用户提供的构造函数

2. 没有私有和受保护的非静态数据成员

3. 没有虚函数

   在新的扩展中，如果类存在继承关系，则额外满足以下条件。

4. 必须是公开的基类，不能是私有或者受保护的基类

5. 必须是非虚继承

## 15.2 聚合类型的初始化

过去要想初始化派生类的基类，需要在派生中提供构造函数。

现在这个过程经过化简：

1. 删除派生类中用户提供的构造函数
2. 直接初始化

例如：

```c++
#include <iostream>
#include <string>

class MyStringWithIndex : public string {
public:
    int index_ = 0;
};

std::ostream& operator<<(ostream& out, const MyStringWithIndex& s) {
	out << s.index_ << ":" << s.c_str();
    return out;
}

int main() {
    MyStringWithIndex s{ {"hello world"}, 11 };
    std::cout << s << std::endl;
    return 0;
}
```

## 15.3 扩展聚合类的兼容问题

例子：

```c++
#include <iostream>
#include <string>

class BaseData {
	int data_;
 public:
    int Get() { return data_; }
 protected:
    BaseData() : data_(11) {}
};

class DerivedData : public BaseData {
public:
};

int main() {
	DerivedData d{};
    std::cout << d.Get() << std::endl;
    return 0;
}
```

C++17编译失败，因为DerivedData变成了一个聚合类，，以至于初始化1变为了聚合类初始化，因为基类 BaseData 中获的构造函数是受保护的关系，它不允许在聚合类型初始化被调用，所以编译器会报错。解决方案：自定义提供构造函数。

## 15.4 禁止聚合类使用用户声明的构造函数

不要直接使用=delete来删除复制构造函数，而是通过假如或者继承一个不可复制构造的类型来实现类型的不可复制。

例如：

```c++
struct X {
    std::string s;
    std::vector<int> v;
    [[no_unique_address]] NonCopyable nc;
};

// 或者

struct X : NonCopyable {
    std::string s;
    std::vector<int> v;
};
```

这种做法能让代码看起来更加简洁。

##  15.5 使用带小括号的列表初始化聚合类型对象

C++20中，规定对于聚合类型对象的初始化可以使用小括号列表完成，其最终效果于大括号一致。注意小括号是支持窄化的。