#ifdef _DEBUG
#include <print>
#endif

int main() {
#ifdef _DEBUG
  std::println("Hello, World!");
#endif
  return 1;
}