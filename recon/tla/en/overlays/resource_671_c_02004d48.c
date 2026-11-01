// Draft, differ 95 of 230, skeleton gap 41 (tla-en, 2026-11-01)
// Runs a race of the four objects from `first` (two teams of two): sets each
// one's starting place and stage, steps them all with Func_02004938 sixteen
// frames at a time until all have finished (stage 0 or 7) or 128 rounds pass,
// then, when judged, compares each team's best time (unk4c): returns 3 with no
// times, 1 or 2 for the team with the better time (setting flag 0x9eb), 0 for
// a tie, or 2 or 1 when only one team finished.
// The judging's skeleton is closed: early returns, with first + 1 kept in
// `second`. Left, all in the set-ups and the bound:
// - The native computes first + 1 before the first call and keeps it on the
//   stack, with id (r6) a live copy: `ldr r0, [sp]; ldr r6, [sp]`, then
//   adds r6, #1 across the racers. Sharing id with the race loop and leaving
//   the judging's first + 1 to gcse reproduces that top, but a HI pseudo for
//   the 2 pushes racer onto the stack (gap 62). Dropping `second` loses the
//   top (gap 44).
// - The native holds 2 in r5 for racer one's lane and racer two's action, yet
//   loads 2 fresh for racer three's lane. Each strh goes through a HI temporary
//   that cse then finds for the later QI 2, which is the pool load here; `two`
//   is a closer form, but no plain form stopped that lookup. A single start
//   variable reassigned per racer is worse (gap 80).
// - The bound first + 4 is computed inside the frame loop in the native
//   (movs r2, #4; add r2, r11; mov r8, r2). Here PRE in gcse hoists it above
//   the round loop. goto loops and a user-variable bound change nothing.
#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);
void Func_02005654(void);
void Func_02005624(struct Actor *object, s32 mode);
void Func_02004938(struct Actor *object, s32 frame, s32 id);
void wait(s32 frames);
void GameFlag_SetBit(s32 flag);
#define RACER(racer, start, lane)                                              \
  (racer)->action = (start);                                                   \
  (racer)->unk66 = 7;                                                          \
  ((u8 *)(racer))[0x62] = (lane);                                              \
  ((u8 *)(racer))[0x63] = 1;                                                   \
  (racer)->unk4c = 0;                                                          \
  Func_02005654();                                                             \
  Func_02005624((racer), 2)

s32 Func_02004d48(s32 first, s32 judge) {
  s32 second, round, frame, done, stage;
  u32 a, b, t;

  second = first + 1;
  {
    struct Actor *racer;
    s32 id;
    s8 two = 2;

    racer = Actor_GetById(first);
    RACER(racer, 0, two);
    id = second;
    racer = Actor_GetById(id);
    RACER(racer, two, 0);
    id++;
    racer = Actor_GetById(id);
    RACER(racer, 8, 2);
    id++;
    racer = Actor_GetById(id);
    RACER(racer, 10, 0);
  }
  for (round = 0; round <= 127; round++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      struct Actor *racer;
      s32 id;

      for (id = first; id < first + 4; id++) {
        racer = Actor_GetById(id);
        stage = ((u8 *)racer)[0x63];
        if (stage != 0 && stage != 7)
          done = 0;
        Func_02004938(racer, frame, id);
      }
      wait(1);
    }
    if (done)
      break;
  }
  wait(20);
  if (judge == 0)
    return 3;
  {
    struct Actor *racer;

    a = 0;
    b = 0;
    racer = Actor_GetById(first);
    if (racer->x != 0)
      a = racer->unk4c;
    racer = Actor_GetById(second);
    if (racer->x != 0) {
      t = racer->unk4c;
      if (a == 0 || a > t)
        a = t;
    }
    racer = Actor_GetById(first + 2);
    if (racer->x != 0)
      b = racer->unk4c;
    racer = Actor_GetById(first + 3);
    if (racer->x != 0) {
      t = racer->unk4c;
      if (b == 0 || b > t)
        b = t;
    }
    if (a != 0) {
      if (b == 0)
        return 1;
      if (a == b)
        return 0;
      if (a < b) {
        GameFlag_SetBit(0x9eb);
        return 1;
      }
      GameFlag_SetBit(0x9eb);
      return 2;
    }
    if (b != 0)
      return 2;
    return 3;
  }
}
