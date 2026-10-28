#include "types.h"
#include "system.h"

struct BusyObject {
  u8 unk00[0x28];
  s32 busy;
};

void OverlayObject_WaitUntilIdle(struct BusyObject *object) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    wait(1);
    busy = object->busy;
    count--;
    if (busy == 0) {
      break;
    }
  }
}
