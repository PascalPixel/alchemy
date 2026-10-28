// Draft, differ 229 of 220: Aqua Rock's current, once flag 0xa68 is set (else
// Func_02001e00): the controlled character is carried back 2 pixels a
// frame (with the object the event work holds at 0x1cc) unless something
// blocks it ahead or both diagonals; with nothing below it falls (cue 258)
// to the ground; in Ov67bRoom1 a pull ahead lifts it out of
// the water for 60 frames, spinning, and runs Func_02002a18. Left: a rough reading: the native keeps gAdventure in r10 (loading it after the flag test), the owner in r8, the carried object in r9 and the scene's address in r0 and 0x9e in r11, and places the flag-clear branch at the end; the alignment is poor.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "system/vector3.h"

extern char SceneId_Ov67bRoom1;
extern u32 gFrameCount;
s32 GameFlag_Test(s32 flag);
void Func_02001e00(void);
struct Actor *Actor_GetById(u32 id);
s32 Func_02002970(struct Actor *object, struct Vector3 *position);
void Func_02001d30(struct Actor *object);
void Func_020029f8(u32 id, s32 cue);
void Func_02002930(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002910(struct Actor *object, s32 mode);
void Func_02002938(struct Actor *object);
void wait(s32 frames);
void Func_02002998(void);
void Func_02002a48(s32 skip);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void FieldEvent_Wait(s32 frames);
u16 Random16Far(void);
void Func_020029a0(void);
void Func_02002a18(s32 mode);

void Func_02000d80(void) {
  struct Actor *leader, *carried;
  struct Vector3 ahead;
  s16 *scene = &gAdventure.scene;
  u32 owner;
  s32 result, i;

  if (!GameFlag_Test(0xa68)) {
    Func_02001e00();
    return;
  }
  owner = gAdventure.currentOwner;
  carried = *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0x1cc);
  leader = Actor_GetById(owner);
  if (*scene == (s32)&SceneId_Ov67bRoom1 && leader->terrainHeight == 0)
    return;
  ahead.x = leader->x;
  ahead.y = leader->y;
  ahead.z = leader->z + 0x20000;
  result = Func_02002970(leader, &ahead);
  if ((gFrameCount & 4) == 0)
    Func_02001d30(leader);
  if (result < 0) {
    Func_020029f8(owner, 258);
    Func_02002930(leader, leader->x, leader->y, leader->z - 0x80000);
    Func_02002910(leader, 7);
    Func_02002938(leader);
    do
      wait(1);
    while (leader->y != leader->terrainHeight);
    Func_02001d30(leader);
    Func_02002910(leader, 6);
    wait(3);
    return;
  }
  ahead.x = leader->x;
  ahead.y = leader->y;
  ahead.z = leader->z - 0x80000;
  result = Func_02002970(leader, &ahead);
  if (result > 0) {
    if (*scene != (s32)&SceneId_Ov67bRoom1)
      return;
    Func_02002998();
    Func_02002a48(0);
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(leader, 0);
    gAdventure.renderMode = 2;
    Func_02001d30(leader);
    FieldEvent_Wait(5);
    for (i = 59; i >= 0; i--) {
      leader->y += 0x20000;
      leader->angle += 0x1000;
      if (Random16Far() <= 0xfff)
        Func_02001d30(leader);
      wait(1);
    }
    gAdventure.renderMode = 0;
    Func_020029a0();
    Func_02002a18(6);
    return;
  }
  ahead.x = leader->x + 0x5b333;
  ahead.y = leader->y;
  ahead.z = leader->z - 0x5b333;
  if (Func_02002970(leader, &ahead) > 0)
    return;
  ahead.x = leader->x - 0x5b333;
  ahead.y = leader->y;
  ahead.z = leader->z - 0x5b333;
  if (Func_02002970(leader, &ahead) > 0)
    return;
  carried->z += -0x20000;
  leader->z += -0x20000;
}
