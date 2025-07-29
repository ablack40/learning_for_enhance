# 18. C++ 指针详解 (面试向)

## 1. 指针基础

**什么是针?**

指针 (Pointer) 是一个变量，其存储的值是另一个变量的**内存地址**。通过指针，我们可以间接访问和修改该内存地址上存储的数据。

**为什么使用指针?**

*   **动态内存管理:** 在堆上分配和释放内存，创建生命周期不限于作用域的对象。
*   **高效的函数参数传递:** 传递大型对象的地址（而不是复制整个对象）可以提高效率，并允许函数修改调用者的数据。
*   **数据结构实现:** 链表、树、图等数据结构的核心是节点之间的链接，通常用指针实现。
*   **硬件交互:** 直接访问内存映射的硬件寄存器。
*   **实现多态:** 通过基类指针调用派生类的虚函数。
*   **C 风格接口:** 与 C 库或底层 API 交互时经常需要使用原始指针。

## 2. 原始指针 (Raw Pointers)

这是 C++ 从 C 继承来的指针形式，需要程序员手动管理内存。

### 2.1 声明与初始化

```cpp
int var = 10;
int* ptr; // 声明一个指向 int 类型的指针 ptr (此时是野指针，未初始化)

ptr = &var; // '&' 取地址运算符，获取 var 的内存地址赋给 ptr

int* ptr_init = &var; // 声明并初始化
int* null_ptr = nullptr; // C++11 推荐：初始化为空指针 (不指向任何有效地址)
int* zero_ptr = 0; // C 风格空指针 (效果同 nullptr，但不推荐在 C++ 中)
int* another_null = NULL; // C 风格宏 (通常定义为 0 或 (void*)0，不推荐)
```

*   **野指针 (Wild Pointer):** 未初始化的指针，指向不确定的内存区域。对其解引用是未定义行为，非常危险。
*   **空指针 (Null Pointer):** 不指向任何有效对象的指针。解引用空指针也是未定义行为，但通常比解引用野指针更容易诊断（程序可能崩溃）。`nullptr` 是类型安全的空指针常量。

### 2.2 解引用与访问

使用解引用运算符 `*` 来访问指针所指向地址上的数据。

```cpp
int var = 20;
int* ptr = &var;

std::cout << "Value via pointer: " << *ptr << std::endl; // 输出 20

*ptr = 30; // 修改指针指向地址上的值
std::cout << "Value of var now: " << var << std::endl; // 输出 30

struct Point { int x, y; };
Point p = {1, 2};
Point* p_ptr = &p;

std::cout << "Point x: " << (*p_ptr).x << std::endl; // 使用 * 和 . 访问成员
std::cout << "Point y: " << p_ptr->y << std::endl;   // 使用 -> 运算符访问成员 (更常用)
p_ptr->x = 5;
```

### 2.3 指针算术

指针可以进行加减运算，移动指向内存中的下一个或上一个元素。移动的步长取决于指针指向的数据类型的大小。

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int* arr_ptr = arr; // 数组名本身就是指向第一个元素的指针

std::cout << "First element: " << *arr_ptr << std::endl; // 输出 10

arr_ptr++; // 指针移动到下一个 int 的位置
std::cout << "Second element: " << *arr_ptr << std::endl; // 输出 20

std::cout << "Third element: " << *(arr_ptr + 1) << std::endl; // 输出 30 (arr_ptr 仍指向第二个)

int* ptr1 = &arr[1];
int* ptr2 = &arr[3];
std::ptrdiff_t diff = ptr2 - ptr1; // 指针相减得到元素间隔，类型是 std::ptrdiff_t
std::cout << "Difference: " << diff << std::endl; // 输出 2
```

**注意:** 指针算术只对指向**同一数组（或内存块）**的指针有意义。对不相关的指针进行算术运算是未定义行为。

### 2.4 指针类型

*   **指向基本类型:** `int*`, `float*`, `char*` 等。
*   **指向自定义类型:** `MyClass*`, `struct Point*` 等。
*   **`void*` (无类型指针):** 可以指向任何类型的对象，但不能直接解引用，必须先强制转换为具体类型指针。常用于需要处理未知类型数据的底层函数（如 `memcpy`, `malloc` 返回值）。
    ```cpp
    void* generic_ptr;
    int i = 10;
    generic_ptr = &i;
    // std::cout << *generic_ptr; // 错误！不能直接解引用 void*
    int* int_ptr = static_cast<int*>(generic_ptr);
    std::cout << *int_ptr << std::endl; // 正确
    ```
*   **函数指针:** 指向函数的指针，可以用来调用函数。
    ```cpp
    int add(int a, int b) { return a + b; }
    int (*func_ptr)(int, int); // 声明一个函数指针
    func_ptr = &add; // 或 func_ptr = add;
    int result = func_ptr(3, 4); // 通过指针调用函数
    std::cout << "Function pointer result: " << result << std::endl; // 输出 7
    ```
*   **指向指针的指针:** `int** ptr_to_ptr;`

### 2.5 `const` 与指针

`const` 可以用来限制指针本身或指针所指向内容的可修改性：

1.  **指向常量的指针 (Pointer to Constant):** 不能通过该指针修改所指向的内容，但指针本身可以指向其他地址。
    ```cpp
    const int val = 10;
    const int* ptr_to_const = &val; // 指向常量
    // *ptr_to_const = 20; // 错误！不能通过指针修改
    int another_val = 30;
    ptr_to_const = &another_val; // 正确，指针本身可以修改
    ```
    (也可写成 `int const* ptr_to_const;` 效果相同)

2.  **常量指针 (Constant Pointer):** 指针本身的值（即它存储的地址）不能被修改，但可以通过指针修改所指向的内容（如果指向的内容不是常量）。
    ```cpp
    int val = 10;
    int* const const_ptr = &val; // 指针本身是常量
    *const_ptr = 20; // 正确，可以修改指向的内容
    int another_val = 30;
    // const_ptr = &another_val; // 错误！不能修改指针指向的地址
    ```

3.  **指向常量的常量指针 (Constant Pointer to Constant):** 指针本身和它所指向的内容都不能被修改。
    ```cpp
    const int val = 10;
    const int* const const_ptr_to_const = &val;
    // *const_ptr_to_const = 20; // 错误！
    // const_ptr_to_const = &another_val; // 错误！
    ```

### 2.6 原始指针的风险与手动内存管理

*   **内存泄漏 (Memory Leak):** `new` 或 `malloc` 分配的内存，如果忘记 `delete` 或 `free`，将一直占用，直到程序结束。
*   **悬垂指针 (Dangling Pointer):** 指针指向的内存已经被释放或回收，但指针本身没有置空，后续解引用该指针会导致未定义行为。
*   **重复释放 (Double Free):** 对同一块内存执行多次 `delete` 或 `free`，导致未定义行为。
*   **野指针 (Wild Pointer):** 未初始化的指针。
*   **内存越界:** 指针算术超出数组或分配的内存范围。

**手动内存管理:**

*   **`new` / `delete`:** C++ 操作符，用于分配/释放单个对象。`new` 会调用构造函数，`delete` 会调用析构函数。
*   **`new[]` / `delete[]`:** C++ 操作符，用于分配/释放对象数组。**必须配对使用！** 用 `delete` 释放 `new[]` 分配的内存，或用 `delete[]` 释放 `new` 分配的内存，都是未定义行为。
*   **`malloc` / `free`:** C 标准库函数，只负责分配/释放原始内存，不调用构造/析构函数。

**规则:** 手动管理内存时，必须确保每次分配都有对应的释放，且只释放一次。这非常容易出错，是现代 C++ 推荐使用智能指针的主要原因。

## 3. 智能指针 (Smart Pointers)

智能指针是 C++11 及以后版本标准库提供的类模板，用于自动管理动态分配的内存（或其他资源），遵循 RAII (Resource Acquisition Is Initialization) 原则。

**RAII:** 资源（如内存、文件句柄、锁）的生命周期与对象的生命周期绑定。在对象构造时获取资源，在对象析构时释放资源。这确保了即使发生异常，资源也能被正确释放。

### 3.1 `std::unique_ptr` (唯一指针)

*   **核心:** 提供对其管理资源的**独占所有权**。
*   **特性:** 轻量级，几乎零开销。不可复制，但可移动 (所有权转移)。
*   **创建:** 推荐使用 `std::make_unique<T>(args...)` (C++14 起)，它更安全（异常安全）且可能更高效。
*   **使用场景:** 默认选择。当你明确知道一个资源只有一个拥有者时。

```cpp
#include <memory>

void process(std::unique_ptr<MyClass> ptr) {
    ptr->print();
} // ptr 离开作用域，自动 delete 管理的对象

int main() {
    // 创建
    std::unique_ptr<MyClass> p1 = std::make_unique<MyClass>(1, "Unique Obj 1");
    MyClass* raw_ptr = new MyClass(2, "Raw for Unique");
    std::unique_ptr<MyClass> p2(raw_ptr); // 也可以用原始指针构造，但不推荐

    // 访问
    p1->print();

    // 所有权转移
    // std::unique_ptr<MyClass> p3 = p1; // 错误: 不可复制
    std::unique_ptr<MyClass> p4 = std::move(p1); // 正确: p1 变为空，p4 拥有对象
    if (!p1) {
        std::cout << "p1 is now empty." << std::endl;
    }
    process(std::move(p4)); // 将所有权转移给函数 process
    // p4 此时也为空

    // 管理数组
    std::unique_ptr<int[]> arr_ptr = std::make_unique<int[]>(10);
    arr_ptr[0] = 100;

    return 0;
} // p2 (如果未移动) 和 arr_ptr 在 main 结束时自动释放资源
```

### 3.2 `std::shared_ptr` (共享指针)

*   **核心:** 提供对其管理资源的**共享所有权**，通过引用计数机制实现。
*   **特性:** 允许多个 `shared_ptr` 指向同一对象。当最后一个 `shared_ptr` 被销毁时，对象才被删除。有额外开销（控制块）。
*   **创建:** 推荐使用 `std::make_shared<T>(args...)`，它通常更高效（一次内存分配同时创建对象和控制块）且异常安全。
*   **使用场景:** 当你需要多个指针共享同一个对象的所有权，且不确定哪个最后析构时。

```cpp
#include <memory>
#include <vector>

int main() {
    std::shared_ptr<MyClass> sp1 = std::make_shared<MyClass>(3, "Shared Obj");
    std::cout << "Initial use count: " << sp1.use_count() << std::endl; // 1

    std::vector<std::shared_ptr<MyClass>> vec;
    vec.push_back(sp1); // 复制，引用计数增加
    std::cout << "Use count after vector push: " << sp1.use_count() << std::endl; // 2

    std::shared_ptr<MyClass> sp2 = sp1; // 复制，引用计数再次增加
    std::cout << "Use count after sp2 copy: " << sp1.use_count() << std::endl; // 3

    sp1.reset(); // sp1 不再指向对象，引用计数减少
    std::cout << "Use count after sp1 reset: " << sp2.use_count() << std::endl; // 2

    vec.clear(); // vector 中的 shared_ptr 销毁，引用计数减少
    std::cout << "Use count after vector clear: " << sp2.use_count() << std::endl; // 1

    return 0;
} // sp2 离开作用域，引用计数变为 0，对象被 delete
```

### 3.3 `std::weak_ptr` (弱指针)

*   **核心:** **非拥有性**的智能指针，用于观察 `shared_ptr` 管理的对象，但**不影响对象的生命周期**（不增加引用计数）。
*   **特性:** 主要用于**打破 `shared_ptr` 之间的循环引用**。不能直接访问对象，需要通过 `lock()` 获取一个临时的 `shared_ptr`。
*   **使用场景:** 解决循环引用；缓存；观察者模式等需要临时、非拥有性访问共享资源的场景。

```cpp
#include <memory>

struct Parent;
struct Child;

struct Parent {
    std::shared_ptr<Child> child;
    ~Parent() { std::cout << "Parent destroyed\n"; }
};

struct Child {
    // std::shared_ptr<Parent> parent; // 如果这样写，会造成循环引用！
    std::weak_ptr<Parent> parent; // 使用 weak_ptr 打破循环
    ~Child() { std::cout << "Child destroyed\n"; }
};

int main() {
    std::shared_ptr<Parent> p = std::make_shared<Parent>();
    std::shared_ptr<Child> c = std::make_shared<Child>();

    p->child = c;
    c->parent = p; // p 指向 c (shared), c 指向 p (weak)

    std::cout << "Parent use count: " << p.use_count() << std::endl; // 1 (只有 main 中的 p)
    std::cout << "Child use count: " << c.use_count() << std::endl; // 1 (只有 main 中的 c 和 p->child)

    // 访问 weak_ptr
    if (std::shared_ptr<Parent> locked_parent = c->parent.lock()) {
        // lock() 成功，说明 Parent 对象还存在
        std::cout << "Child can access parent. Parent use count now: " << locked_parent.use_count() << std::endl; // 2 (p 和 locked_parent)
    } else {
        std::cout << "Parent has been destroyed." << std::endl;
    }

    return 0;
} // p 和 c 离开作用域，引用计数都能正常降为 0，对象被销毁
```

### 3.4 智能指针总结

*   **优先使用 `std::unique_ptr`:** 开销小，所有权明确。
*   **需要共享所有权时使用 `std::shared_ptr`:** 注意循环引用问题。
*   **打破 `shared_ptr` 循环引用或需要非拥有性观察时使用 `std::weak_ptr`。**
*   **尽量使用 `std::make_unique` 和 `std::make_shared` 创建智能指针。**
*   **避免混合使用原始指针和智能指针管理同一个对象。**
*   **不要用 `new` 获取的原始指针初始化多个 `shared_ptr` (会导致重复释放)。**

## 4. 面试中的指针考点

面试官考察指针时，通常关注以下方面，目的是评估你对内存管理、指针操作的理解深度以及代码安全性意识：

*   **基本概念:**
    *   什么是指针？为什么要用指针？
    *   指针和引用的区别？(重要)

        #### 指针 vs. 引用：形象化理解与对比

        为了更清晰地理解指针和引用的核心差异，我们可以使用一些比喻：

        *   **引用 (Reference) - 就像一个变量的“绰号”或“别名”**
            *   **本质:** 引用是**已存在变量的一个别名**。它不创建新的独立实体，而是为某个已经存在的变量提供了另一个名字。
            *   **必须初始化:** 绰号必须在起的时候就指定是“谁”的绰号，不能凭空存在，也不能中途改成别人的绰号。
            *   **不能为空:** 不能有一个指向“虚无”的绰号。
            *   **操作即本体:** 对绰号的操作，就是对这个人本身的操作。

        *   **指针 (Pointer) - 就像一张写有地址的“纸条”**
            *   **本质:** 指针是一个**独立的变量**，这张“纸条”本身存在。纸条上记录的内容是另一个东西（变量）的**内存地址**。
            *   **可以不初始化/为空:** 纸条刚拿到手可以是空白的（未初始化，危险！），或者明确写上“无地址”（`nullptr`）。
            *   **可以改变指向:** 纸条上的地址可以擦掉，换成另一个地址。
            *   **需要“按地址查找”:** 想通过纸条找到那个东西，需要一个明确的动作——“根据纸条上的地址去找”（解引用 `*`）。纸条本身 (`ptr`) 和纸条指向的东西 (`*ptr`) 是不同的。

        **代码对比:**

        ```cpp
        #include <iostream>
        #include <string>

        int main() {
            std::string name = "Alice";
            std::string another_name = "Bob";

            // --- 引用 --- 
            std::string& name_ref = name; // 引用必须在声明时初始化，是 name 的别名
            // std::string& empty_ref; // 错误！引用必须初始化
            // name_ref = nullptr; // 错误！引用不能设为空

            std::cout << "Original name: " << name << std::endl;       // Alice
            std::cout << "Name via ref: " << name_ref << std::endl;    // Alice

            name_ref = "Alice Smith"; // 修改引用，就是修改原变量
            std::cout << "Original name now: " << name << std::endl; // Alice Smith

            // 尝试让引用指向其他变量 (实际是赋值操作)
            name_ref = another_name; // 这不是让 name_ref 变成 another_name 的别名！
                                     // 而是将 another_name 的值赋给 name_ref 所引用的变量 (即 name)
            std::cout << "Original name after assign: " << name << std::endl; // Bob
            std::cout << "another_name: " << another_name << std::endl; // Bob
            std::cout << "Name via ref after assign: " << name_ref << std::endl; // Bob (仍然是 name 的别名)
            // 引用一旦绑定，终生不变其引用的对象

            std::cout << "\n--- Pointer ---" << std::endl;

            // --- 指针 --- 
            std::string* name_ptr; // 可以声明时不初始化 (野指针，危险)
            name_ptr = &name;      // 指针可以后续赋值，指向 name 的地址
            std::string* null_ptr = nullptr; // 指针可以为空

            std::cout << "Address stored in ptr: " << name_ptr << std::endl; // 输出 name 的地址
            std::cout << "Value via ptr: " << *name_ptr << std::endl;   // Bob (解引用得到值)

            *name_ptr = "Bob Brown"; // 通过指针修改指向的值
            std::cout << "Original name now: " << name << std::endl; // Bob Brown

            // 让指针指向其他变量
            name_ptr = &another_name; // 正确，指针可以改变指向
            std::cout << "Value via ptr now: " << *name_ptr << std::endl; // Bob (现在指向 another_name)

            return 0;
        }
        ```

        **总结关键区别:**

        | 特性         | 指针 (Pointer)                     | 引用 (Reference)                   |
        | :----------- | :--------------------------------- | :--------------------------------- |
        | **本质**     | 存储地址的变量                     | 已存在变量的别名                   |
        | **初始化**   | 可选 (不初始化是野指针)            | **必须**在声明时初始化             |
        | **空值**     | 可以为 `nullptr`                   | **不能**为 `nullptr` 或空          |
        | **可变性**   | 可以改变指向的地址                 | **不能**改变引用的对象（一旦绑定） |
        | **内存占用** | 通常占用一个指针大小的内存         | 通常不占用额外内存（作为别名）     |
        | **操作**     | 间接访问，需解引用 (`*`, `->`)   | 直接访问，像使用原变量一样         |

    *   `*` 和 `&` 在不同上下文的含义（声明指针 vs 解引用；取地址 vs 声明引用）。
*   **原始指针:**
    *   指针算术的规则和风险。
    *   `void*` 的作用和限制。
    *   函数指针的声明和使用。
    *   `const` 与指针的组合含义（三种情况必须分清）。
    *   常见的指针错误：野指针、悬垂指针、内存泄漏、重复释放、内存越界。如何避免？
    *   `new`/`delete` 与 `malloc`/`free` 的区别？
        *   类型安全：`new`/`delete` 是操作符，`malloc`/`free` 是函数。
        *   构造/析构：`new`/`delete` 调用构造/析构函数，`malloc`/`free` 不调用。
        *   重载：`new`/`delete` 可以被重载。
        *   内存来源：`new` 可能调用 `malloc`，但行为可定制。
    *   `delete` 和 `delete[]` 的区别？为什么必须匹配使用？（`delete[]` 需要知道数组大小以便正确调用每个元素的析构函数和释放整个块）。
*   **智能指针 (现代 C++ 核心):**
    *   为什么推荐使用智能指针取代原始指针？（RAII，自动内存管理，减少错误）。
    *   `std::unique_ptr`：特点（独占所有权、移动语义）、用法、`make_unique`。
    *   `std::shared_ptr`：特点（共享所有权、引用计数）、用法、`make_shared`、控制块开销。
    *   `std::weak_ptr`：作用（打破循环引用、非拥有性观察）、用法 (`lock()`)。
    *   如何选择合适的智能指针？
    *   `make_unique`/`make_shared` 相对于直接使用 `new` 的优势？
        *   异常安全：`make_...` 能保证在复杂表达式中即使构造函数抛异常也不会内存泄漏。
        *   效率 (`make_shared`)：通常只需要一次内存分配（对象+控制块），而 `shared_ptr(new T(...))` 需要两次。
    *   `shared_ptr` 的线程安全性？（控制块的引用计数增减是原子操作，线程安全；但其指向的对象本身的数据访问不是线程安全的，需要额外加锁）。
    *   循环引用是如何产生的？如何用 `weak_ptr` 解决？
*   **实践与场景:**
    *   举例说明指针在特定场景的应用（如链表操作、函数回调）。
    *   如何在 C++ 代码中安全地处理 C API 返回的需要手动管理的资源指针？（通常用 `unique_ptr` 加自定义删除器）。

**回答技巧:**

*   **清晰准确:** 定义清晰，区分不同类型的指针和概念。
*   **强调安全:** 突出原始指针的风险以及智能指针如何解决这些问题。
*   **掌握细节:** 对 `const`、`new`/`delete`/`delete[]`、引用计数、`make_` 函数等细节有深入理解。
*   **现代 C++ 优先:** 表明你熟悉并倾向于使用现代 C++ 的智能指针进行内存管理。
*   **结合示例:** 用简洁的代码片段或场景说明问题。

对指针的深刻理解是 C++ 程序员的核心能力之一，也是面试中展现你基础是否扎实的重点。