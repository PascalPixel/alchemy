// Draft, differ 185 of 230: runs a race of the four objects from `first` (two
// teams of two): sets each one's starting place and stage, steps them all
// with Func_02004938 sixteen frames at a time until all have finished
// (stage 0 or 7) or 128 rounds pass, then, when judged, compares each
// team's best time (unk4c): returns 3 with no times, 1 or 2 for the team
// with the better time (setting flag 0x9eb), 0 for a tie, or 2 or 1 when
// only one team finished. Left: the native keeps 0, 1, 2 and 7 in high registers across the four set-ups, its race loops are goto loops entered at their tests with the bound first + 4 in r8, and it orders the judging's branches otherwise.
#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 id);
void Func_02005654(void);
void Func_02005624(struct Actor *object, s32 mode);
void Func_02004938(struct Actor *object, s32 frame, s32 id);
void wait(s32 frames);
void GameFlag_SetBit(s32 flag);

static __inline__ void Racer(s32 id, s32 start, s32 lane) {
  struct Actor *racer = Actor_GetById(id);

  racer->action = start;
  racer->unk66 = 7;
  ((u8 *)racer)[0x62] = lane;
  ((u8 *)racer)[0x63] = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
}

s32 Func_02004d48(s32 first, s32 judge) {
  struct Actor *racer;
  s32 round, frame, id, done, stage;
  u32 a, b, t;

  Racer(first, 0, 2);
  Racer(first + 1, 2, 0);
  Racer(first + 2, 8, 2);
  Racer(first + 3, 10, 0);
  for (round = 0; round <= 127; round++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
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
  a = 0;
  b = 0;
  racer = Actor_GetById(first);
  if (racer->x != 0)
    a = racer->unk4c;
  racer = Actor_GetById(first + 1);
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
  if (a == 0)
    return b != 0 ? 2 : 3;
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
