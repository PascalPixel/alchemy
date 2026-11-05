// Draft, differ 126 of 556, skeleton gap 26 (tla-en, 2026-11-01)
// The portrait mode task, ☀️'s Colosso_UpdateLettering
// (scene/shared/colosso_task.h) with overlay 696's variables and the French
// four-graphic case. Left: only one allocation. The native spills sprite and
// scale (frame 24, here 20), which frees r9 for packed and fp for the third
// duration address; here sprite is r9, scale fp, packed sits at sp+0 and the
// third address is reloaded. Every skeleton line still different follows from
// that, the branch shapes included (their distances). global.c's order is
// already right below these two: packed .069, then the duration addresses
// .065, as the native's r9 and fp show. sprite is 4 * 18 / 680 = .106 and scale
// 2 * 7 / 169 = .083, and both must fall below .069. Their refs are flow's
// loop-weighted counts (depth + 1); unweighted they are 12 and 5, which gives
// .053 and .059. sprite has no constant note (three different sets, no_equiv),
// so its live length cannot double, packed cannot reach .083 under 24 insns,
// and no scale form was found with fewer refs. That weighting is off only with
// optimize_size, and hoist_code, which would also explain the target, t and
// packed loads ahead of each duration test, runs only then: a compiler question
// for Pascal (K3), out of a draft's reach. blend is declared before scale for
// the native's slots (scale 0, blend 4, sprite 8, write 12, pseudo order
// downward); it changes nothing until they spill.
// Forms this rests on: each duration address is a one-constant local set before
// the timer store, with the duration read into a local before the target
// address is set; that closed the first interpolation exactly (149 to 132) and
// the same form in the other two gave 128. In the third, duration, then target,
// then t, then packed gave 126; the other orders gave 128 or 132. The target
// pointers, t and packed stand for what the native loads before each duration
// test; plain reads there do not hoist them (without targetVar, differ 258).
// work is two words because bitfields read through t became strh. ({ ... })
// around the last script read of each case makes the pointer store come first,
// kept in HImode: plain *p++ is 202 this round, a local promotes and loads
// ldrsh, an inline reader is worse. This is the form a reviewer should question
// first. The case 1 and 3 loops are goto loops so y and flags stay in the body:
// as a real loop loop.c moves them (nothing sets maybe_never before them, and a
// second set in the if is gone by cse). limit is a one-constant local: cse puts
// its 303 straight into the compare, so combine never rewrites it to 304 with
// bcs (without it, 136, gap 32). A real loop gets the same 303 from
// reg_single_usage, not from a hoist. Set per case instead of at the top, limit
// is worse. volatile cannot stand in for the spills: a volatile local sits
// above the spill slots, and write must sit above scale.
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
  s32 blend, scale, pos;
  s32 matrix, i, x, y, left;
  u32 work[2];
  u32 limit = 303;

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
  {
    s16 *durationVar = &gData_02006490;
    s16 *targetVar;
    s32 duration;
    gData_020064bc--;
    duration = *durationVar;
    targetVar = &gData_020064a4;
    if (duration == 0) {
      scale = *targetVar;
    } else {
      s32 start, progress, target;
      start = gData_02006498;
      progress = ++gData_02006508;
      target = *targetVar;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        *durationVar = 0;
    }
  }
  {
    s16 *durationVar = &gData_020064c4;
    s16 *targetVar;
    s32 duration;
    targetVar = &gData_020064b4;
    duration = *durationVar;
    if (duration == 0) {
      blend = *targetVar;
    } else {
      s32 start, progress, target;
      start = gData_020064b8;
      target = *targetVar;
      progress = ++gData_020064ac;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        *durationVar = 0;
    }
  }
  {
    s16 *durationVar = &gData_0200649c;
    s16 *targetVar;
    u32 packed;
    u32 *t;
    s32 duration;
    duration = *durationVar;
    targetVar = &gData_02006504;
    packed = scale << 16;
    t = work;
    if (duration == 0) {
      pos = *targetVar;
    } else {
      s32 start, progress, target;
      start = gData_02006500;
      target = *targetVar;
      progress = ++gData_020064a8;
      pos = start + (target - start) * progress / duration;
      if (progress >= duration)
        *durationVar = 0;
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
      if ((u32)(x + 152) <= limit) {
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
      if ((u32)(x + 152) <= limit) {
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
      if ((u32)(x + 152) <= limit) {
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
