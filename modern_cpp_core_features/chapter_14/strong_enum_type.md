# 强枚举类型（C++11 C++17 C++20)

## 14.1 枚举类型的弊端

虽然枚举类型存在一定的安全检查功能，一个枚举类型不允许分配到另外一种枚举类型，而且整型也无法隐式转换成枚举类型。但是枚举类型却可以却可以隐式转换整型。

## 14.2 使用强枚举类型

强枚举类型的的新特性：

1. 枚举标识符属于强枚举类型的作用域
2. 枚举标识符不会隐式转换为整型
3. 能指定强枚举类型的底层类型，底层类型默认为int类型

强枚举类型不仅实现起来非常简单，而且还是POD类型。

虽然强调了强枚举类型的枚举标识符是无法隐式转换为整形的，但还是可以通过 `static_cast` 进行强制转换。

最后说明强枚举类型不可以匿名，必须要赋予一个名称。

## 14.3 列表初始化有底层类型枚举对象

从 C++17 标准库开始，对有底层类型的枚举类型对象可以使用初始化列表初始化。

例如：

```c++
enum class Color {
	Red,
    Green,
    Blue
};

int main() {
    Color c{5};		// compile successfully
    Color c1 = 5;	// compile failed
    Color c2 = {5};	// compile failed
    Color c3(5);	// complie failed
}
```

## 14.4 使用 using 代开强枚举类型

C++ 20标准扩展了 using 功能，它可以打开强枚举类型的命名空间。

例如：

```c++
enum class Color {
    Red,
    Green,
    Blue
};

const char* ColorToString(Color c) {
	switch(c) {
	using enum Color;
        case Red: return "Red";
        case Green: return "Green";
        case Blue: return "Blue";
        default:
            return "None";
    }
}
```

注意，switch-case作用域之外还是需要指定枚举biao'shi