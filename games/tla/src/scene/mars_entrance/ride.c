#include "ride.h"

void Func_02004be0(void *handle);

void Func_020022c4(void) {
  struct RideWork *work = RIDE_WORK;

  Func_02004be0(work->handle);
  work->handle = NULL;
}

void Func_020022d8(s16 value) { RIDE_WORK->value0e = value; }
