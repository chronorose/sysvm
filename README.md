# sysvm - java virtual machine implemented in C++

## Milestones and plans:
### First milestone:
  - [x] .class files parser
  - [ ] simple VM that implements subset of spec(no native methods)

#### Criteria:
Examples of programs that should be executed correctly:

```Java
class HelloVM {
  public static void main(String[] args) {}
}
```

```Java
class Factorial {
  public static void main(String[] args) {
    factorial(5);
  }
  public int factorial(int n) {
    if (n <= 2) {
      return n;
    }
    return n * factorial(n - 1);
  }
}
```
 
### Second milestone:
  - [ ] Garbage collector(single-threaded)
  - [ ] native methods

#### Criteria:
Infinite loop doesn't throw OOM exception;

### Third milestone:
  - [ ] Parallel GC
  - [ ] Overall improvements across the board
