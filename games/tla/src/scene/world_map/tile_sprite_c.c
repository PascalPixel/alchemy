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

s32 Func_020035e0(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}
