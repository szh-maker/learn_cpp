# 使用 auto 的时机建议

auto 推荐使用规则

1. 当一眼就能够看出声明变量的初始化类型的时候可以使用 auto

   ```c++
   std::map<std::string, int> str_to_int;
   // ... 填充 str_to_int 代码
   for(std::map<std::string, int>::const_iterator it = str_to_int.cbegin(); 
       it != str_to_int.cend(); ++it) {}
   // 或者
   for(std::pair<const std::string, int>& it : str_to_int) {}
   
   // 使用 auto
   for(auto it = str_to_int.begin(); it != str_to_int.end(); ++it) {}
   for(auot& it : str_to_int) {}
   ```

2. 对于复杂的类型，例如 lambda 表达式、bind 等直接使用 auto

   ```c++
   // 有时无法写出类型或者过于复杂的类型，或者即使能够写出来，但是别的程序员阅读起来也很费劲
   auto l = [](int a, int b) { return a + b; };
   
   int sum(int a, int b) {
       return a + b;
   }
   auto b = std::bind(sum, 5, std::placeholders::_1);
   ```

   

