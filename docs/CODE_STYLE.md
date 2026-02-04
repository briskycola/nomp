# Coding Style
This document will be the guide for how
you should style your code. The code needs
to look as consistent and readable as possible.
I won't be super crazy with rules, but I'll
put some general ones to make the code look
consistent. Here are some general rules for how
your code should look like:

## Indentation Style
We will use the Allman style for indentation. All
brackets will be on the next line after the control
statement.
```cpp
// Allman Style
void fooBar(int number, boolean &isEven)
{
    if (number % 2 == 0)
    {
        isEven = true;
    }

    else
    {
        isEven = false;
    }
}
```

## Variable and Function Naming Convention
We will use camalCase generally speaking
for naming variables and functions, however
there are some exceptions. Here is the naming
convention for all variables and functions

### Primitive types
```cpp
int foo = 6;
int fooBar = 9;
```

### Primitive types (static)
```cpp
static int FooBar = 9;
```

### Constants
```cpp
const int FOO = 6;
const int FOO_BAR = 9;
```

### Raw pointers
```cpp
int foo = 6;
int *bar = &foo;
```

### Objects
```cpp
class Foo
{
    private:
        int foo;
};

// Allocate on the stack
Foo f;

// Allocate on the heap
std::unique_ptr<Foo> f = std::make_unique<Foo>();

class FooBar
{
    private:
        int foo;
};

// Allocate on the stack
FooBar f;

// Allocate on the heap
std::unique_ptr<FooBar> f = std::make_unique<FooBar>(); // Allocate on the heap
```

### Functions
```cpp
void foo();
void fooBar();

// Functions (static)
static void FooBar();
```

## Other practices
Here are some other practices we will use in
the development process.

### Namespaces
Do not use `using namespace`. We want to know
exactly where a particular function is coming
from. This will make the code much easier to read.

### Tabs and spaces
Tabs will be 4 spaces and we will use spaces
as tabs (someone's going to roast me with this one).
I'm going to assume you know how to configure your
editor for this.

### One liners
If you want, you can also do one-liners
to take up less lines:
```cpp
// Normal
if (a == b)
{
    return true;
}

// One liner
if (a == b) return true;
```
