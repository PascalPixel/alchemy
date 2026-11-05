#include "lucky_wheels_lzpack.h"

void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackWork *work = gFlash_Handler3;
  s32 index;

  for (index = 0; index != work->outCnt; index++) {
    destination[work->outTotal] = work->outBuf[index];
    work->outTotal = work->outTotal + 1;
  }
}
