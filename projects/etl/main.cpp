#include <etl/vector.h>

int main(void) {
  etl::vector<uint8_t, 4> testVector = {1, 2, 3, 4}
  for(uint8_t idx = 0; idx < testVector.size(); idx++) {
    printf("Vector entry %d: %d", idx, testVecor[idx]);
  }
  while(true) {}
  return 0;
}
