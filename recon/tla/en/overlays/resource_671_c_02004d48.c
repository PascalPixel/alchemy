// Draft, differ 63 of 230 (tla-en, 5 November): places four racers, runs them, and compares the times of the two teams.
// Left: the native copies second into r6 before the second racer's lookup and steps that copy (ldr r6, [sp]; adds r6, #1); here
// CSE folds id onto second, so it reloads after the call. r8 holds one and r10 seven in the native, swapped here (0.079 to 0.083), and the first racer's
// address stays in r3 while its constants are set. Tried: every order of the constants' sets, assignment expressions (69 to 73), separators, types of two.
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

  racer = Actor_GetById(first);
  zero = 0;
  seven = 7;
  one = 1;
  two = 2;
  racer->unk64 = zero;
  racer->unk66 = seven;
  racer->unk62 = two;
  racer->state = one;
  racer->unk4c = zero;
  Func_02005654();
  Func_02005624(racer, 2);
  id = second;
  PLACE(second, two, seven, 0, one, zero);
  id++;
  PLACE(id, 8, seven, 2, one, zero);
  id++;
  PLACE(id, 10, seven, 0, one, zero);
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
    {
    struct Racer *r = Actor_GetById(first);
    if (r->x != 0)
      a = r->unk4c;
    }
    {
    struct Racer *r = Actor_GetById(second);
    if (r->x != 0) {
      u32 value = r->unk4c;
      if (a == 0 || a > value)
        a = value;
    }
    }
    {
    struct Racer *r = Actor_GetById(first + 2);
    if (r->x != 0)
      b = r->unk4c;
    }
    {
    struct Racer *r = Actor_GetById(first + 3);
    if (r->x != 0) {
      u32 value = r->unk4c;
      if (b == 0 || b > value)
        b = value;
    }
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
