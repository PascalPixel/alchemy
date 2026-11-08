// Matches: differ 0 (tla-en, 2026-11-08)
// Plain C, no devices; differ 0 in all six editions (tla-en, tla-ja, tla-de, tla-es, tla-fr, tla-it).
// What closed it (from 15): the second and third racers are looked up with Actor_GetById(id++) after id = second,
// and the fourth with Actor_GetById(id). The post-increment puts id = second + 1 before the second racer's lookup,
// so reload loads r6 from [sp] beside the lookup's own load and sched2 sinks the add between that racer's calls,
// as the listing has it. With the set-up as plain statements (the once-only block gone), the third racer's heading
// 2 comes from the second racer's halfword 2 and is rebuilt (movs r2, #2) rather than reusing r5, and local-alloc
// gives r5 = 2, r8 = 1, r10 = 7, r9 = 0 as the listing does.
// Places four racers, runs them, and compares the times of the two teams.
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
  racer = Actor_GetById(id++);
  racer->unk64 = 2;
  racer->unk66 = 7;
  racer->unk62 = 0;
  racer->state = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
  racer = Actor_GetById(id++);
  racer->unk64 = 8;
  racer->unk66 = 7;
  racer->unk62 = 2;
  racer->state = 1;
  racer->unk4c = 0;
  Func_02005654();
  Func_02005624(racer, 2);
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
