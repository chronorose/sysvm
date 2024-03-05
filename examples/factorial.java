class Factorial {
  public static void main(String[] args) {
    System.exit(factorial(5));
  }
  public static int factorial(int n) {
    if (n <= 2) {
      return n;
    }
    return n * factorial(n - 1);
  }
}
