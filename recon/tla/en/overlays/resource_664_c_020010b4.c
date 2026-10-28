// Draft, differ 129 of 211 (was 141): the update Func_02001290 gives its
// shots: while the event work is busy it stops; on a cell of kind 212 it ends
// (cue 539, its end script gData_020061cc); otherwise, the first time it
// reaches the leader (within 16 pixels on the same height when the leader
// climbs, else within 8 on the same level) and flag 0x104 is clear, it
// carries the leader along (counter 0x16a 200, movement 0); it moves 1.5
// pixels a frame along its angle, upward when its angle is 0xc000. The
// counter's 200 goes through an s32 (as a halfword constant it comes from the
// pool). Left: the native keeps the shot in r6 and reuses the busy counter's
// zero in r7 for the stores and the near test; this draft swaps them, which
// moves most rows. Measured: no hit pointer, the fourth counter read into
// near first (154).
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"

extern u8 gData_020061cc[];
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
s32 Func_02000690(s32 layer, s32 x, s32 z);
void Func_02005f8c(struct ObjectRuntime *object, s32 cue);
void Func_02005d6c(struct ObjectRuntime *object, const u8 *script);
s32 Func_02000d18(s32 *a, s32 *b);
s32 Func_02000cec(s32 *a, s32 *b);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_020010b4(struct ObjectRuntime *shot) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  u8 *hit;
  s32 near;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      *(u16 *)(work + 0x1b2) == 0) {
    ObjectDispatch_SetAnimationSpeed(shot, 0);
    return;
  }
  ObjectDispatch_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    Func_02005f8c(shot, 539);
    ((u8 *)shot)[0x63] = 0;
    shot->update = 0;
    Func_02005d6c(shot, gData_020061cc);
    return;
  }
  hit = (u8 *)shot + 0x62;
  if (*hit == 0) {
    near = 0;
    if (gAdventure.renderMode == 2) {
      if (Func_02000d18(&leader->x, &shot->x) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(&leader->x, &shot->x) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->linkedObject = shot;
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
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  }
}
