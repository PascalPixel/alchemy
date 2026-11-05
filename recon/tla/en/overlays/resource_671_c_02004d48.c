// Draft of overlay 671's Func_02004d48, rewritten from the listing's translation.
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
  s32 zero, seven, one, two;
  s32 frame, tries, id;
  s32 done;

  racer = Actor_GetById(first);
  zero = 0;
  racer->unk64 = zero;
  seven = 7;
  racer->unk66 = seven;
  one = 1;
  two = 2;
  racer->unk62 = two;
  racer->state = one;
  racer->unk4c = zero;
  Func_02005654();
  Call2((void (*)())Func_02005624, racer, 2);
  {
    s32 next = second;

    racer = Actor_GetById(next);
    racer->unk64 = two;
    racer->unk66 = seven;
    racer->unk62 = 0;
    racer->state = one;
    racer->unk4c = zero;
    Func_02005654();
    next++;
    Call2((void (*)())Func_02005624, racer, 2);
    racer = Actor_GetById(next);
    racer->unk64 = 8;
    racer->unk66 = seven;
    {
      s32 k = 2;

      racer->unk62 = k;
    }
    racer->state = one;
    racer->unk4c = zero;
    Func_02005654();
    next++;
    Call2((void (*)())Func_02005624, racer, 2);
    racer = Actor_GetById(next);
    racer->unk64 = 10;
    racer->unk66 = seven;
    racer->unk62 = 0;
    racer->state = one;
    racer->unk4c = zero;
    Func_02005654();
    Func_02005624(racer, 2);
  }
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
  if (check) {
    u32 a = 0;
    u32 b = 0;

    {
      struct Racer *one = Actor_GetById(first);

      if (one->x != 0)
        a = one->unk4c;
    }
    {
      struct Racer *other = Actor_GetById(second);

      if (other->x != 0 && (a == 0 || a > other->unk4c))
        a = other->unk4c;
    }
    {
      struct Racer *one = Actor_GetById(first + 2);

      if (one->x != 0)
        b = one->unk4c;
    }
    {
      struct Racer *other = Actor_GetById(first + 3);

      if (other->x != 0 && (b == 0 || b > other->unk4c))
        b = other->unk4c;
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
  }
  return 3;
}
