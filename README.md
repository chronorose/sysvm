# sysvm - java virtual machine implemented in C++

## Milestones and plans:
### First milestone:
  - [x] .class files parser (no verification)
  - [x] simple VM that implements subset of spec(no native methods)

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
 
### Second milestone(first iteration):
  - [ ] Garbage collector(single-threaded)
  - [ ] native methods

#### Criteria(first iteration):
Infinite loop doesn't throw OOM exception;

### Second milestone(plans for second iteration):
  - [ ] native methods
  - [ ] improve on base VM(inline a lot of stuff into parser, support more bytecode commands, support all datatypes)

### Third milestone(first iteration):
  - [ ] Parallel GC
  - [ ] Overall improvements across the board

### Third milestone(plans for second iteration):
  - [ ] GC (either single-threaded or parallel)
