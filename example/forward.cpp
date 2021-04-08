#include <utility>
#include <cstddef>
#include <string>
#include <iostream>
using namespace std;


// for test1:
class Foo {
public:
    Foo() = default;
    explicit Foo(string name) : name_(name) {
        cout << name_ << " Foo::constructor" << endl;
    }
    Foo(const Foo& foo) {
        cout << name_ << " Foo::constructor copy &" << endl;
    }
    
    Foo( Foo&& foo) {
        cout << name_ << " Foo::constructor copy&&" << endl;
    }
    
    Foo& operator=(const Foo& foo) {
        cout << name_ << " Foo::constructor operator=" << endl;
        return *this;
    }

    ~Foo() {
        cout << name_ << " Foo::destructor" << endl;
    }
    string name_;

};
void bar(Foo f) {
    cout << "start bar" << endl;
    cout << "end   bar" << endl;
}
void test1() {
    cout << "start test1" << endl;
    bar(Foo("bar"));
    Foo f1((Foo("&")));
    Foo f2(std::move(Foo("&&")));
    cout << "end   test1" << endl;
}




// for test2:
class MyString {
public:
    MyString(string name, size_t num = 0)
        : name_(name), size_(num), data_(new char[size_]) {
        log("constructor");
    }
    
    explicit MyString(size_t num = 0)
        : size_(num), data_(new char[size_]) {
        log("constructor");
    }

    ~MyString() {
        log("destructor");
        if (data_) {
            delete [] data_;
            data_ = 0;
        }
    }

    MyString(const MyString& other)
        : name_("copy"), size_(other.size_), data_(new char[size_]) {
        log("copy constructor");
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    // 由于我们不能直接修改 other，所以要先构造一个临时的对象 tmp，然后把 this 和 tmp 交换
    // 此处共用到 1次 other 构造，1次 tmp 构造，还有可忽略的 swap 操作
    // 或者可以采用复制，即 1次 other 构造，1次复制(size_ 和 data_ 都要复制，data_ 会很耗时)
    MyString& operator=(const MyString& other) {
        log("copy assignment operator&");
        MyString tmp(other);        // tmp constructor
        std::swap(size_, tmp.size_);
        std::swap(data_, tmp.data_);
        return *this;               // tmp destructor
    }

    // 这里直接用 1次 other 构造即可，内部通过 swap 把临时对象 other 的内容“偷过来”，要保证 other 后面不再使用。
    MyString& operator=(MyString&& other) {
        log("move assignment operator&&");
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
        return *this;               // other destructor
    }
private:
    void log(const char* msg) {
        cout << "[" << this << "] " << name_ << " " << msg << "\n";
    }

    size_t size_;
    char* data_;
    string name_;
};

void test2() {
    // 普通构造函数，会在 v2 内部生成一个 tmp 临时对象，并在
    MyString v1("v1");          // v1 constructor
    MyString v2("v2");          // v2 constructor

    cout << endl;
    cout << "start assigning lvalue...\n";
    v2  = v1;
    cout << "ended assigning lvalue...\n";
    cout << endl;

    // 把 MyString& operator=(MyString&& other) 注释掉看对比
    cout << endl;
    cout << "start assigning lvalue...\n";
    v2 = MyString("v3");
    cout << "ended assigning lvalue...\n";
    cout << endl;

    // v2 = (std::move(Intvec(3)));
    // v2, v1 destructor
}



// for test3:
template<typename T>
void print(T & t){
    std::cout << "左值" << std::endl;
}

template<typename T>
void print(T && t){
    std::cout << "右值" << std::endl;
}

template<typename T>
void testForward(T && v){
    print(v);
    print(std::forward<T>(v));
    print(std::move(v));
}
void test3() {
    testForward(1);

    std::cout << "======================" << std::endl;

    int x = 1;
    testForward(x);
}


int main() {


    // test1();
    // cout << endl;


    // test2();
    // cout << endl;

    test3();
    cout << endl;
    
    return 0;
}

// g++ forward.cpp -std=c++11 -o forward.exe