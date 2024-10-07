# 非受限联合类型（C++11）

## 11.1 联合类型在C++11中的局限性

过去的C++标准规定，联合类型的成员变量的类型不能是一个非平凡类型，也就是它的成员类型不能有自定义构造函数。

例如：

```c++
union U {
	int a;
    float b;
    std::string c;
};
```

上述代码是无法通过编译的，因为c存在自定义的构造函数，所以他是一个非平凡类型。

## 11.2 使用非受限联合类型

在C++11中如果联合类型中存在非平凡类型，那么这个联合类型的特殊成员函数将隐式删除，即至少自定义联合类型的构造函数和析构函数。

例如：

```c++
#include <iostream>
#include <string>
#include <vector>

union U {
    U() {} 	// 存在非平凡类型成员，必须提供构造函数
    ~U() {} // 存在非平凡类型成员，必须提供析构函数
    int x1;
    float x2;
    std::string x3;
    std::vector<int> x4;
};

int main() {
	U u;
    u.x3 = "hello world!";
    std::cout << u.x3;
}
```

上述代码运行时会报错，因为编译器并不知道使用时利用的是哪个构造函数和析构函数。

进行如下修改：

```c++
#include <iostream>
#include <string>
#include <vector>

union U {
    U() {} 	// 存在非平凡类型成员，必须提供构造函数
    ~U() {} // 存在非平凡类型成员，必须提供析构函数
    int x1;
    float x2;
    std::string x3;
    std::vector<int> x4;
};

int main() {
	U u;
    new(&u.x3) std::string("hello world!");
    std::cout << u.x3 << "\n";
    u.x3.~basic_string();
    
    new(&u.x4) std::vector<int>;
    u.x4.push_back(58);
    std::cout << u.x4.front() << "\n";
    
    return 0;
}
```

请注意，上述代码用了 placement new 的技巧来初始化构造x3和x4对象，在使用完之后手动调用对象的析构函数。通过这样的方法保证了联合类型使用的灵活性和正确性。

最后简单说一下非受限联合类型对静态成员变量的支持。联合类型的静态成员不属于联合类型的任何对象，所以对象构造时被定义的，不能在来联合类型内部初始化。