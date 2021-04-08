### 涉及函数及头文件

头文件：`utility`

- std::move
- std::forward

### 左值和右值
[理解 C/C++ 中的左值和右值](https://nettee.github.io/posts/2018/Understanding-lvalues-and-rvalues-in-C-and-C/)

`左值` (lvalue, locator value) 表示了一个占据内存中某个可识别的位置（也就是一个地址）的对象。

`右值` (rvalue) 则使用排除法来定义。一个表达式不是 `左值` 就是 `右值` 。 那么，`右值`是一个“不表示”内存中某个可识别位置的对象的表达式。

例：
~~~cpp
int foo() { return 1; }     // 返回值为右值，若 使用 foo() = 3，则报错，给 “右值” 赋值是错误的。
int& foo() { return 2; }    // 错误，返回值应该是某个左值(即有地址空间的值)，一般用作返回类成员 this 指针。下面是对的：
int globalvar = 20;
int& foo() { return globalvar; }
int main() { foo() = 10; cout << globalvar << ", " << foo() << endl; return 0; }

int var;
var = 4;        // 正确

4 = var;        // 错误！
(var + 1) = 4;  // 错误
~~~

> 右值通常是某个临时变量，或者表达式的中间结果，而没有可识别的内存位置，也就是说，只存在于计算过程中的每个临时寄存器中。

#### 左值引用和右值引用
左值引用的对象必须是一个左值，相对地，右值引用的对象是一个右值。
例：
~~~cpp
class Foo{
    Foo(Foo& f) {}
    Foo(Foo&& f) {}
};
// 分别是左值和右值引用参数，右值引用可以使用这样的表达式初始化：
Foo f(std::move(Foo()));    // 调用 Foo(Foo&& f) {}，因为传入的是一个临时变量
~~~

** CV 限定，即 `const` 和 `volatile `。**

通常左值可以有 CV 限定。而右值，在 C 中是没有 CV 限定的，C++ 中部分有，内置类型没有。

### std::move 和右值引用
使用临时对象初始化某个对象的例子：[test2](../example/forward.cpp)


### std::forward 完美转发
即原来是左值，传递后还是左值，原来是右值，传递后还应是右值。
例子：[test3](../example/forward.cpp)

