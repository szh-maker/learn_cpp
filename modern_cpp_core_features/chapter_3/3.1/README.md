# 重新定义的 auto 的关键字

使用 auto 占位符声明变量的时候必须初始化变量。

1. 当用一个 auto 关键字声明多个变量的时候，编译器遵守由左往右的推导规则，以最左边的表达式推断 auto 的具体类型

   ```c++
   int n = 5;
   auto *pn = &n, m = 10;
   ```

2. 当条件表达式初始化 auto 声明的变量时，编译器总是使用表达能力更强的类型

   ```c++
   auto i = true ? 5 : 8.0;	// i 的数据类型为 double
   ```

3. C++11 标准已经支持在声明成员变量时初始化，但是 auto 却无法在这种情况下声明非静态成员变量。在 C++11 中静态成员变量时可以用 auto 声明并且初始化的，不过前提是 auto 必须使用 const 限定符。在 C++17 标准中，对于静态成员变量，auto 可以在没有 const 的情况下使用

   ```c++
   struct sometype {
       auto i = 5;	// 错误，编译不过
   };
   
   // C++11
   struct sometype {
   	static const auto i = 5;
   };
   
   // C++17
   struct sometype {
       static inline auto i = 5;
   }
   ```

4. 按照 C++20 之前的标准，无法在函数参数列表中使用 auto 声明形参（注意，C++14 中，auto 可以为 lambda表达式声明形参）