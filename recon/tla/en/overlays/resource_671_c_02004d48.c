// Differ 15 of 230 (tla-en, 2026-11-08, -mtune=gba): the second racer's Func_02005624 call and id++ moved out of
// its once-only block, id++ before the call (from 17). Left: cse2 still folds id = second into id++ (the native loads
// r6 from [sp] beside the lookup and adds 1 between the calls; once-only blocks around the copy or the increment,
// a goto label, the lookup through id, second as a copy of id, or no second local all stay 15), the first racer's
// stores sit one slot apart, and the third racer's heading 2 reuses r5. An 8-minute permute: only invalid candidates
// (a float cast, id set after its use) below 15.
// Earlier, at 17: id = second + 1 in place of the copy, 17.
// Tried 8 November, all 17 or worse: id = second after the lookup, id++ or id = second + 1 between the second
// racer's calls, id++ in its own once-only block, ++id in the lookup, id = first + 1 or first; id++, second set
// late, declaration orders, once-only blocks around the third racer or its unk62 store (21), racers 2 and 3 in one
// block (101), an inline placement helper (103), no second local (27); an 8-minute permute found nothing below 17.
// 2026-11-06: the native keeps id = second as its own copy (ldr r6, [sp] beside the second racer's lookup, adds r6, #1 between that racer's two calls); here combine folds it into second + 1, and sched2 moves the load and add past the call; the first racer's set-up stores and Func_02005624's two arguments come in a slightly different order, and the third racer's heading 2 reuses r5 where the native builds it again.
// Places four racers, runs them, and compares the times of the two teams.
// 6 November: constants written as literals (21) and only the second racer in a one-pass block (17) give the native's r9 = 0, r10 = 7, r8 = 1, r5 = 2; the result's
// last branch matches once the no-first-time case returns (63 to 45). Tried for the id copy: id initialised at the top, before or inside the block, id++ between the calls,
// the lookup through id, id = first, first + 1/2/3 without id (93 to 119), Func_02005624 prototypes; all 17 or worse.
// Earlier (5 November): every order of the constants' sets as named locals, assignment expressions (69 to 73), separators, types of two.
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

s32 Func_02004d48(s32 first, s32 check) {
  struct Racer *racer;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  racer = Actor_GetById(first);
  racer->unk64 = 0;
  racer->unk66 = 7;
  racer->unk62 = 2;
  racer->state = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
  id = second;
  do {
    // A once-only block: its end holds the order around it (as plain
    // statements, 99 of 230 differ in tla-en).
    racer = Actor_GetById(second);
    racer->unk64 = 2;
    racer->unk66 = 7;
    racer->unk62 = 0;
    racer->state = 1;
    racer->unk4c = 0;
    Func_02005654();
  } while (0);
  id++;
  Func_02005624(racer, 2);
  racer = Actor_GetById(id);
  racer->unk64 = 8;
  racer->unk66 = 7;
  racer->unk62 = 2;
  racer->state = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
  id++;
  racer = Actor_GetById(id);
  racer->unk64 = 10;
  racer->unk66 = 7;
  racer->unk62 = 0;
  racer->state = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
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
      if (b != 0)
        return 2;
      return 3;
    }
  }
  return result;
}
