// Draft, differ 226 of 266 (was 230): the update Func_02001b54 gives its
// shots, overlay 664's Func_02000d48 with one more ending: it stops while the
// event work is busy or the leader's movement is 5; it ends (cue 539, one of
// two end scripts by its angle) on a cell of kind 212 or when it reaches any
// of objects 8 to 63 (the event work's object table) that is live and not
// where its launcher stands; the first time it reaches the leader with flag
// 0x104 clear it carries the leader along; it moves 1.5 pixels a frame along
// its angle, upward when its angle is 0xc000. The counter's 200 goes through
// an s32 (as a halfword constant it comes from the pool). Left: registers from
// the start (the native keeps the shot in r6, gAdventure in r7 and the event
// work in r8; this draft the shot in r7 and the work in r6), which moves
// nearly every row; the code is the native's otherwise.
#include "types.h"
#include "actor.h"
#include "adventure.h"
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
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor **objects;
  struct Actor *other, *launcher;
  u8 *hit;
  s32 near, i;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gAdventure.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000348(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
  end:
    Func_02003cd0(shot, 539);
    ((u8 *)shot)[0x63] = 0;
    shot->update = 0;
    if (shot->angle == 0xc000)
      Func_02003b38(shot, gData_02003de0);
    else
      Func_02003b38(shot, gData_02003dcc);
    return;
  }
  hit = (u8 *)shot + 0x62;
  if (*hit == 0) {
    near = 0;
    if (gAdventure.renderMode == 2) {
      if (Func_020018d8(&leader->x, &shot->x) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_020018ac(&leader->x, &shot->x) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->watchedObject = shot;
      leader->angle = shot->angle;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      {
        s16 *counter = (s16 *)(work + 0x16a);
        s32 value = 200;

        *counter = value;
      }
      gAdventure.renderMode = 0;
      *hit = 1;
      ((u8 *)shot)[0x63] = 1;
    }
  }
  objects = (struct Actor **)(work + 0x34);
  i = 8;
search:
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
  i++;
  if (i != 64)
    goto search;
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  }
}
