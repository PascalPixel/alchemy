// Draft, differ 99 of 230, skeleton gap 19 (tla-en, 2026-11-01)
// Runs a race of the four objects from `first` (two teams of two): sets each
// one's starting place and stage, steps them all with Func_02004938 sixteen
// frames at a time until all have finished (stage 0 or 7) or 128 rounds pass,
// then, when judged, compares each team's best time (unk4c): returns 3 with no
// times, 1 or 2 for the team with the better time (setting flag 0x9eb), 0 for
// a tie, or 2 or 1 when only one team finished.
// Found this round, all kept below:
// - The bound stays in the frame loop because `id` is read after `first`'s
//   last textual use: judging through `id = first` and `id + 1` to `id + 3`
//   makes id the head of cse's class, so `first + 4` is killed by the copy at
//   the loop's top and PRE leaves it; gcse's cprop then rewrites the judging to
//   `first`, and cse2 gives the listing's `first + 4` and `cmp first, limit`.
// - `Actor_GetById(next++)` gives `ldr r0, [sp]; ldr r6, [sp]`: the increment's
//   reload sits before the call and sched2 floats only the adds down. The last
//   call has no increment, as the listing's last racer shows.
// Left, with the three facts that decide it:
// - The native holds 1, 7 and 0 in r8, r10 and r9, the registers of limit,
//   round and done. local-alloc fills r5, r6, r8, r10, r9, r11 by priority, and
//   global-alloc then kicks out poorly used locals (find_reg). Reusing those
//   three loop variables for the constants makes them global and closes the
//   skeleton to gap 10 (5 and 5), differ 72, with frame r5, id r6, racer r7,
//   but that is a local reused for a second value: a `temporary` device, not
//   added here. Whether it may be tagged is a counting question for Pascal
//   (S2). The probe is not in this file.
// - Without it, `next` beats `two` and takes r5 (listing r6, two r5), so r5 and
//   r6 are swapped through the set-ups, and id and frame are swapped in the
//   loop. Sharing `next` with `id` fixes those (differ 77) but the kicked 0
//   then loads from the pool (gap 37). A local `zero` set at the top does not
//   float below the first call (gap 33).
// - Racer one stores the stage before the lane only to bring `two`'s live range
//   under 33, ahead of the constants; in the listing's order 0 lands in a low
//   register (gap 31). limit is `first + 4` written in the test: a variable
//   bound takes r6 and pushes frame to r4 (gap 22), and this form leaves
//   `mov; adds #4; mov` for the listing's `movs #4; add; mov`.
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
  s32 second, round, frame, done, stage, id;
  u32 a, b, t;
  struct Actor *racer;

  second = first + 1;
  {
    s32 next;
    s32 two;

    racer = Actor_GetById(first);
    racer->action = 0;
    racer->unk66 = 7;
    ((u8 *)racer)[0x63] = 1;
    two = 2;
    ((u8 *)racer)[0x62] = two;
    racer->unk4c = 0;
    Func_02005654();
    Func_02005624(racer, 2);
    next = second;
    racer = Actor_GetById(next++);
    RACER(racer, two, 0);
    racer = Actor_GetById(next++);
    RACER(racer, 8, 2);
    racer = Actor_GetById(next);
    RACER(racer, 10, 0);
  }
  for (round = 0; round <= 127; round++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        stage = ((u8 *)racer)[0x63];
        racer = Actor_GetById(id);
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

    id = first;
    a = 0;
    b = 0;
    racer = Actor_GetById(id);
    if (racer->x != 0)
      a = racer->unk4c;
    racer = Actor_GetById(id + 1);
    if (racer->x != 0) {
      t = racer->unk4c;
      if (a == 0 || a > t)
        a = t;
    }
    racer = Actor_GetById(id + 2);
    if (racer->x != 0)
      b = racer->unk4c;
    racer = Actor_GetById(id + 3);
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
