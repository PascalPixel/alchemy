// Draft, differ 115 of 230 (tla-en, 2026-11-05).
// Draft of overlay 671's Func_02004d48, differ 115 of 230 (tla-en, 5 November 2026), from 134.
// Left: the native has no CSE across the racers (fresh movs r2, #2 and #0 where the
// variables hold them), holds racer + 100 in r3 while it sets the first racer's
// constants, and keeps next in r6, where here next folds onto second. A do-while
// macro or empty asm between racers does not end the block for cse1 (119, 123).
#include "types.h"
#include "call.h"

struct Racer {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 state;
  u16 unk64;
  u16 unk66;
};

struct Racer *Actor_GetById();
void Func_02005654();
void Func_02005624();
void Func_02004938();
void wait();
void GameFlag_SetBit();

#define PLACE(id, a, b, c, d, e)                                               \
  do {                                                                         \
    racer = Actor_GetById(id);                                                 \
    racer->unk64 = a;                                                          \
    racer->unk66 = b;                                                          \
    racer->unk62 = c;                                                          \
    racer->state = d;                                                          \
    racer->unk4c = e;                                                          \
    Func_02005654();                                                           \
    Func_02005624(racer, 2);                                                   \
  } while (0)

s32 Func_02004d48(s32 first, s32 check) {
  struct Racer *racer;
  s32 second = first + 1;
  s32 zero, seven, one;
  s32 two;
  s32 frame, tries, id;
  s32 done;
  s32 next;
  s32 result;

  zero = 0;
  seven = 7;
  one = 1;
  two = 2;
  PLACE(first, zero, seven, two, one, zero);
  next = second;
  PLACE(next, two, seven, 0, one, zero);
  next++;
  PLACE(next, 8, seven, 2, one, zero);
  next++;
  PLACE(next, 10, seven, 0, one, zero);
  for (tries = 0; tries <= 127; tries++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        racer = Actor_GetById(id);
        if (racer->state != 0 && racer->state != 7)
          done = 0;
        Func_02004938(racer, frame, id);
      }
      wait(1);
    }
    if (done)
      break;
  }
  wait(20);
  result = 3;
  if (check) {
    u32 a = 0;
    u32 b = 0;

    racer = Actor_GetById(first);
    if (racer->x != 0)
      a = racer->unk4c;
    racer = Actor_GetById(second);
    if (racer->x != 0) {
      u32 value = racer->unk4c;
      if (a == 0 || a > value)
        a = value;
    }
    racer = Actor_GetById(first + 2);
    if (racer->x != 0)
      b = racer->unk4c;
    racer = Actor_GetById(first + 3);
    if (racer->x != 0) {
      u32 value = racer->unk4c;
      if (b == 0 || b > value)
        b = value;
    }
    if (a != 0) {
      result = 1;
      if (b != 0) {
        result = 0;
        if (a != b) {
          if (a < b) {
            GameFlag_SetBit(0x9eb);
            result = 1;
          } else {
            GameFlag_SetBit(0x9eb);
            result = 2;
          }
        }
      }
    } else {
      result = 2;
      if (b == 0)
        result = 3;
    }
  }
  return result;
}
