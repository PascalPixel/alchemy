#include "ride.h"

void Func_020012bc(struct ObjectRuntime *object) {
  switch (gFrameCount & 7) {
  case 0:
    Animation_ApplyChildValues(object, 0);
    break;
  case 6:
    Animation_ApplyChildValues(object, 10);
    break;
  }
}
