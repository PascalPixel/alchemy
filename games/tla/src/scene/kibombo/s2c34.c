#include "kibombo.h"

void Func_02002c34(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);
    Func_02002b18(87, 1, ((Random16Far() * 8) >> 16) - 4);
    Func_02002b18(103, 2, ((Random16Far() * 4) >> 16) - 2);
    Func_02002b18(119, 3, ((Random16Far() * 4) >> 16) - 2);
  }
}

void Func_02002ccc(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);
    Func_02002b18(7, 1, ((Random16Far() * 6) >> 16) - 3);
    Func_02002b18(6, 2, ((Random16Far() * 4) >> 16) - 2);
    Func_02002ba4(76, 3, ((Random16Far() * 8) >> 16) - 4);
  }
}

void Func_02002d64(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(169, 0, ((Random16Far() * 4) >> 16) - 2);
    Func_02002b18(167, 1, ((Random16Far() * 2) >> 16) - 1);
    Func_02002b18(166, 2, Random16Far() >> 16);
  }
}
