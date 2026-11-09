#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
u32 Random16Far(void);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020036a8(void);
void Func_020048f4(void);
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

s32 Func_02003418(struct Actor *object) {
  if (Random16Far() < 0x4000)
    Animation_ApplyChildValues(object, Random16Far() * 8 >> 16);
  return 0;
}
