# 新字符类型  char16_t  和  char32_t

## 字符集和编码方法

UTF-32 是最简单的编码方法，该方法使用一个32为内存空间（即4 bytes）存储一个字符编码，由于Unicode字符集的最大个数为0x10FFFF（ISO 10646）， 因此4 bytes（0xFFFFFFFF） 的空间完全能容纳任何一个字符编码。

> 优点：简单，计算字符串长度和查找字符都很方便
>
> 缺点：内存占用量大

UTF-16 编码方法是将所需的内存空间压缩至16位（即2 bytes），所以UTF-16 最多支持 0xFFFF 个字符，显然不能像 UTF-32 一样覆盖全部字符，于是 UTF-16 采用了一种特殊的方法来表达无法表示的字符。

> 0x0000~0xD7FF 以及 0xE000~0xFFFF 直接映射到 Unicode 字符集，而剩下的 0xD800~0xDFFF 则用于映射 0x10000~0x10FFFF 的 Unicode 字符集，映射方法为：字符编码减去 0x1000 后剩下的 20 bit 位分为高位和低位，高十位的映射范围为 0xD800~0xDBFF，低十位的映射范围为 0xDC00~0xDFFF。

## char8_t 字符类型

使用char类型处理UTF-8字符虽然可行，但是也会带来一些困扰，比如库函数需要同时处理多种字符时必须采用不同的函数名称以区分普通字符和 UTF-8 字符。C++20 引入 char8_t 类型解决该问题。

> char8_t 具有和 unsigned char 相同的符号属性、存储大小、对齐方式以及整数转换等级。