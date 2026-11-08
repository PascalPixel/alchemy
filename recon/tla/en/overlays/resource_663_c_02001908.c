// Differ 2 of 266 (tla-en, 2026-11-08, -mtune=gba). The counter is now stored
// through struct EventWork's unk16a as Air's Rock's Func_020010b4 does (2, in
// place of the pointer-and-value block); through an s16 or u16 pointer or an
// index, 16; in a once-only block, 35. Also tried 2026-11-08: the sprites in
// locals (either order), value after counter (2); the counter moved after
// renderMode or before the priority copy, or its address taken early (14 to
// 29); near's 0 in a once-only block (8) or the positions declared as
// Func_020010b4 does (98); 8 permuter minutes found nothing better.
// Later 2026-11-08: the sprites through locals, a typed EventWork local, a
// u8 bit-field view of byte 9, a cast 200, every single move of a declaration
// (2); the leader stores reordered (6 to 82); the copy or priorityFlags in a
// once-only block (82); without the asm (4); 8 more permuter minutes.
// One tie in sched2 (both priority 11, RTL order wins): the native computes
// the counter's address (add r2, r8) ahead of the sprite's strb, as in 664's
// Func_02000d48. Func_02000d48's twin, with another ending.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "field/event_work.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"

extern u8 gData_02003de0[], gData_02003dcc[];
struct Actor *Actor_GetById(u32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02000348(s32 layer, s32 x, s32 z);
void Func_02003cd0(struct Actor *object, s32 cue);
void Func_02003b38(struct Actor *object, const u8 *script);
s32 Func_020018d8(s32 *a, s32 *b);
s32 Func_020018ac(s32 *a, s32 *b);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02001908(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 speed = 0x18000;
  s32 cue = 539;
  struct Actor *other, *launcher;
  u8 *hit;
  s32 near, i;
  struct Actor **objects;
  u32 angle;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000348(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
  end:
    Func_02003cd0(shot, cue);
    {
      s32 none = 0;

      ((u8 *)shot)[0x63] = none;
      shot->update = (void *)none;
    }
    if (shot->angle == 0xc000)
      Func_02003b38(shot, gData_02003de0);
    else
      Func_02003b38(shot, gData_02003dcc);
    return;
  }
  hit = (u8 *)shot + 0x62;
  if (*hit == 0) {
    {
      s32 *leaderPosition = &leader->x, *shotPosition = &shot->x;

      near = 0;
      /* FAKEMATCH(barrier): the empty asm holds near's 0 ahead of the position
       * arguments; as no asm, 4 of 266 instructions differ in tla-en. */
      asm("" : "=r"(near) : "0"(near));
      if (gMainRoutineWork.renderMode == 2) {
        if (Func_020018d8(leaderPosition, shotPosition) <= 16 &&
            leader->z >> 20 == shot->z >> 20)
          near = 1;
      } else {
        if (Func_020018ac(leaderPosition, shotPosition) <= 8 &&
            leader->y >> 20 == shot->y >> 20)
          near = 1;
      }
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      *hit = 1;
      ((u8 *)shot)[0x63] = 1;
    }
  }
  objects = (struct Actor **)(work + 0x34);
  for (i = 8; i != 64; i++) {
    other = *objects++;
    if (other->script != 0) {
      launcher = shot->watchedObject;
      if (launcher->x >> 20 != other->x >> 20 ||
          launcher->y >> 20 != other->y >> 20 ||
          launcher->z >> 20 != other->z >> 20) {
        if (Func_020018ac(&other->x, &shot->x) <= 8 &&
            other->y >> 20 == shot->y >> 20)
          goto end;
      }
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), speed);
  }
}
