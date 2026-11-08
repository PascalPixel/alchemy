// Matches: differ 0 (tla-en, 2026-11-08)
// Differ 0 in all six editions (tla-en, tla-ja, tla-de, tla-es, tla-fr,
// tla-it). One tagged device: the first Func_02005654 call without its actor.
// What closed it (from 15): the second and third actors are looked up with
// Actor_GetById(id++) after id = second, and the fourth with
// Actor_GetById(id). The post-increment puts id = second + 1 before the second
// lookup, so reload loads r6 from [sp] beside the lookup's own load and sched2
// sinks the add between that actor's calls, as the listing has it. With the
// set-up as plain statements, the third actor's 2 comes from the second's
// halfword 2 and is rebuilt (movs r2, #2) rather than reusing r5, and
// local-alloc gives r5 = 2, r8 = 1, r10 = 7, r9 = 0 as the listing does.
// Sets up four actors, steps them with Func_02004938 for up to 128 rounds of
// 16 frames, then compares unk4c of the first pair with the second pair's.
#include "types.h"
#include "call.h"

struct Unk02004d48 {
  u8 unk00[8];
  s32 x;
  u8 unk0c[76 - 12];
  u32 unk4c;
  u8 unk50[98 - 80];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u16 unk66;
};

struct Unk02004d48 *Actor_GetById();
void Func_02005654();
void Func_02005624();
void Func_02004938();
void wait();
void GameFlag_SetBit();

s32 Func_02004d48(s32 first, s32 check) {
  struct Unk02004d48 *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = Actor_GetById(first);
  actor->unk64 = 0;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  /* FAKEMATCH(signature): Func_02005654, Actor_ResetMotion's veneer, called
   * without the actor it reads from r0; as Func_02005654(actor), 73 of 230
   * instructions differ in tla-en. */
  Func_02005654();
  Func_02005624(actor, 2);
  id = second;
  actor = Actor_GetById(id++);
  actor->unk64 = 2;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = Actor_GetById(id++);
  actor->unk64 = 8;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = Actor_GetById(id);
  actor->unk64 = 10;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  for (tries = 0; tries <= 127; tries++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        actor = Actor_GetById(id);
        if (actor->unk63 != 0 && actor->unk63 != 7)
          done = 0;
        Func_02004938(actor, frame, id);
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
      struct Unk02004d48 *r = Actor_GetById(first);
      if (r->x != 0)
        a = r->unk4c;
    }
    {
      struct Unk02004d48 *r = Actor_GetById(second);
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (a == 0 || a > value)
          a = value;
      }
    }
    {
      struct Unk02004d48 *r = Actor_GetById(first + 2);
      if (r->x != 0)
        b = r->unk4c;
    }
    {
      struct Unk02004d48 *r = Actor_GetById(first + 3);
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
