#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "iwram_call.h"
#include "actor.h"

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02004b50(struct Actor *object, s32 x, s32 y, s32 z);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void GameFlag_ClearBit(s32 flag);
u32 Random16Far(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004db8(s32 value);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Func_02004c88(s32 id, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_02004d40(s32 value);
s32 Task_Add(void (*func)(void), s32 order);
void Task_RemoveFar(void (*func)(void));
void Func_020036a8(void);
void Func_020048f4(void);
s32 Func_02004e10(void);
void Func_02003bb8(struct Actor *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Func_02000388(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

s32 Func_020003a8(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

/// With flag 0x200 set the object is released, takes acceleration and speed
/// limit 0x1000 and is sent to an offset of its position.
s32 Func_020003b4(struct Actor *object) {
  s32 x, z;

  if (!GameFlag_Test(0x200))
    return 1;
  Actor_SetSingleChildFlags(object, 0);
  x = object->x;
  object->acceleration = 0x1000;
  object->speedLimit = 0x1000;
  z = object->z;
  /* FAKEMATCH: the empty asm ties z to x, so z loads before x's offset is
   * added, as in the native (plain: differ 2; a one-pass block before the call:
   * 4; alchemy permute, 3 minutes: nothing closer). */
  asm("" : "+r"(z) : "r"(x));
  Func_02004b50(object, x - 0x1e0000, object->y, z + 0x80000);
  return 0;
}
