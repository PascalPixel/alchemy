// Draft, differ 177 of 556, skeleton gap 87 (tla-en, 2026-11-01)
// The portrait mode task, ☀️'s Colosso_UpdateLettering
// (scene/shared/colosso_task.h) with overlay 696's variables and the French
// four-graphic case. Left: allocation. The native puts sprite and scale on the
// stack (frame 24, here 20) and gives the third duration address fp, packed r9
// and tile sl. Here sprite takes r9 and scale fp first: global.c ranks sprite 4
// * 18 / 678 and scale 2 * 7 / 169, over the duration addresses (3 / 44) and
// packed (2 / 31), and the third duration address falls out and is rebuilt. No
// sprite or declaration-order form tried changes that rank. Then the case 1 and
// 3 bounds are 304 with bcs where the native has 303 with bhi. Forms this rests
// on: the target pointers, t and packed stand for what the native loads before
// each duration test; plain reads there do not hoist them (gcse has no hoist at
// -O2, and a one-pass block is phony to loop.c and moves the then arm). work is
// two words because bitfields read through t became strh; the native's
// read-modify-write needs word set over and over, or combine folds it.
// ({ ... }) around the last script read of each case makes the pointer store
// come first, kept in HImode: a local promotes and loads ldrsh, an inline
// reader is worse (gap 102). The case 1 and 3 loops are goto loops so y and
// flags stay in the body: as for, while or do-while loop.c hoists them (gap 135
// with the 303 bound). The native has loop.c's hoisted-and-refolded 303, so its
// loops do have notes and something keeps these user variables from moving
// (maybe_never); a test that resists duplication until cse (i < 4 && always)
// did not reproduce that.
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "affine.h"
#include "vram_tab.h"
#include "io_reg.h"
#include "io_write_queue.h"

struct Sprite {
  u32 words[3];
};

extern u32 gData_020064d0[];
extern s16 gData_02005158[];
extern s16 gData_020064bc;
extern s16 *gData_020064c0;
extern s32 gData_020064a0;
extern s16 gData_02006504;
extern s16 gData_0200649c;
extern s16 gData_02006500;
extern s16 gData_020064a8;
extern s16 gData_020064a4;
extern s16 gData_02006498;
extern s16 gData_02006490;
extern s16 gData_02006508;
extern s16 gData_020064b4;
extern s16 gData_020064b8;
extern s16 gData_020064c4;
extern s16 gData_020064ac;
extern s16 gData_020064b0;
extern s16 gData_02006494;

void Resource_ResetEntry(s32 entry);
void Func_020047cc(void *entry, s32 slot);

#define SCRIPT_NEXT() ({ *gData_020064c0++; })

void Func_02002a20(void) {
  u32 *write = gData_020064d0;
  struct Sprite *sprite = (struct Sprite *)write;
  s32 tile = gResourceTableEntries[gData_02005158[0]].offset >> 5;
  u32 flags;
  s32 scale, blend, pos;
  s32 matrix, i, x, y, left;
  u32 work[2];

commands:
  if (gData_020064bc == 0) {
    switch (*gData_020064c0++) {
    case 0x4000:
      gData_020064a0 = *gData_020064c0++ << 8;
      gData_02006504 = SCRIPT_NEXT();
      gData_0200649c = 0;
      break;
    case 0x3000:
      gData_02006500 = gData_02006504;
      gData_02006504 = *gData_020064c0++;
      gData_0200649c = SCRIPT_NEXT();
      gData_020064a8 = 0;
      break;
    case 0x1000:
      gData_02006498 = gData_020064a4;
      gData_020064a4 = *gData_020064c0++;
      gData_02006490 = SCRIPT_NEXT();
      gData_02006508 = 0;
      break;
    case 0x2000:
      gData_020064b8 = gData_020064b4;
      gData_020064b4 = *gData_020064c0++;
      gData_020064c4 = SCRIPT_NEXT();
      gData_020064ac = 0;
      break;
    case 0x7fff:
      gData_020064bc = *gData_020064c0++;
      break;
    case -1:
      Task_RemoveFar(Func_02002a20);
      Resource_ResetEntry(gData_02005158[0]);
      return;
    }
    goto commands;
  }
  gData_020064bc--;
  {
    s16 *targetVar = &gData_020064a4;
    if (gData_02006490 == 0) {
      scale = *targetVar;
    } else {
      s32 duration, start, progress, target;
      duration = gData_02006490;
      start = gData_02006498;
      target = *targetVar;
      progress = ++gData_02006508;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_02006490 = 0;
    }
  }
  {
    s16 *targetVar = &gData_020064b4;
    if (gData_020064c4 == 0) {
      blend = *targetVar;
    } else {
      s32 duration, start, progress, target;
      duration = gData_020064c4;
      start = gData_020064b8;
      target = *targetVar;
      progress = ++gData_020064ac;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_020064c4 = 0;
    }
  }
  {
    s16 *targetVar = &gData_02006504;
    u32 packed = scale << 16;
    u32 *t = work;
    if (gData_0200649c == 0) {
      pos = *targetVar;
    } else {
      s32 duration, start, progress, target;
      duration = gData_0200649c;
      start = gData_02006500;
      target = *targetVar;
      progress = ++gData_020064a8;
      pos = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_0200649c = 0;
    }
    {
      u32 low, word;
      t[1] &= 0xffff0000;
      low = packed >> 16;
      word = work[0];
      word &= 0xffff0000;
      word |= low;
      word &= 0xffff;
      word |= low << 16;
      work[0] = word;
    }
    matrix = (s16)AffineMatrix_BuildForEffect((struct AffineTransform *)t);
  }
  gData_020064a0 += pos;
  pos = gData_020064a0 / 256;
  switch (gData_020064b0) {
  case 1: {
    i = 0;
  titleLoop:
    {
      y = 56;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 48) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | tile;
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
      i++;
      if (i < 4)
        goto titleLoop;
    }
    break;
  }
  case 3: {
    i = 0;
  resultLoop:
    {
      y = 48;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gData_02006494);
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
      i++;
      if (i < 2)
        goto resultLoop;
    }
    break;
  }
#if defined(TLA_EDITION_FR)
  case 4: {
    i = 0;
  goLoop:
    {
      y = 64;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gData_02006494);
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
      i++;
      if (i < 2)
        goto goLoop;
    }
    break;
  }
#else
  case 4:
    y = 48;
    flags = 0xc0004000;
    left = pos + 56;
    if ((u32)(pos + 120) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gData_02006494);
      Func_020047cc(sprite, 236);
    }
    break;
#endif
  case 2:
    y = 48;
    flags = 0x80000000;
    left = pos + 88;
    if ((u32)(pos + 152) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gData_02006494);
      Func_020047cc(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
