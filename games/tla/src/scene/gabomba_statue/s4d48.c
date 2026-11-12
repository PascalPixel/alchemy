/// @file
/// Scene code of overlay 0x671, in ROM order, from s4d48's offset up to the next object the overlay links.

#include "types.h"
#include "event_inline.h"

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

void Func_02005654();
void Func_02005624();
void Func_02004938();

s32 Func_02004d48(s32 first, s32 check) {
  struct Unk02004d48 *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = ((struct Unk02004d48 *)Actor_Get(first));
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
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 2;
  actor->unk66 = 7;
  actor->unk62 = 0;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id++));
  actor->unk64 = 8;
  actor->unk66 = 7;
  actor->unk62 = 2;
  actor->unk63 = 1;
  actor->unk4c = 0;
  Func_02005654(actor);
  Func_02005624(actor, 2);
  actor = ((struct Unk02004d48 *)Actor_Get(id));
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
        actor = ((struct Unk02004d48 *)Actor_Get(id));
        if (actor->unk63 != 0 && actor->unk63 != 7)
          done = 0;
        Func_02004938(actor, frame, id);
      }
      System_Wait(1);
    }
    if (done)
      break;
  }
  System_Wait(20);
  result = 3;
  if (check) {
    u32 a = 0;
    u32 b = 0;
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first));
      if (r->x != 0)
        a = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(second));
      if (r->x != 0) {
        u32 value = r->unk4c;
        if (a == 0 || a > value)
          a = value;
      }
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 2));
      if (r->x != 0)
        b = r->unk4c;
    }
    {
      struct Unk02004d48 *r = ((struct Unk02004d48 *)Actor_Get(first + 3));
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
            Event_SetFlag(0x9eb);
            result = 1;
          } else {
            Event_SetFlag(0x9eb);
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

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
};

extern struct SceneState gSceneState;

void Func_02004f40(u32 *x_out, u32 *z_out) {
  u32 z;
  u32 x;

  for (z = 0; z <= 7; z++) {
    for (x = 0; x <= 10; x++) {
      if (gSceneState.cells[z * 11 + x].kind == 10 && !(x == 5 && z == 3) &&
          !(z == 7 && (x == 1 || x == 9))) {
        *x_out = x;
        *z_out = z;
        return;
      }
    }
  }
}

#include "iwram_call.h"

struct ClockWork {
  u8 unk00[132];
  s32 timer;
  u8 ringing;
  u8 mode;
};

extern u8 gData_020023c4[];
s32 Math_Cosine(s32 angle);

/// Converts the remaining frames into minutes, seconds and tenths
/// (`MM:SS:T`).
void Func_02004f84(s32 time, u8 *text) {
  if (time < 0)
    time = 0;
  *text = time / 36000 + '0';
  time %= 36000;
  *++text = time / 3600 + '0';
  time %= 3600;
  *++text = ':';
  *++text = time / 600 + '0';
  time %= 600;
  *++text = time / 60 + '0';
  time %= 60;
  *++text = ':';
  *++text = time / 6 + '0';
  text[1] = 0;
}

s32 Func_02005020(s32 time) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  s32 second = time % 60;
  s32 volume = 512;
  u8 *ringing = &clock->ringing;

  if (second != 0) {
    *ringing = 1;
  } else if (*ringing) {
    if (clock->timer != 0)
      Event_PlaySound(236);
    *ringing = second;
  }
  if (second <= 19) {
    s32 cosine = Math_Cosine((second << 15) / 10);
    s32 (*mul)(s32 a, s32 b);
    s32 scale = 128;

    mul = Iwram_MulQ16;
    volume = mul(scale << 2, cosine);
  }
  if (volume <= 16)
    volume = 16;
  return volume;
}

