#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "iwram_call.h"
#include "object_runtime.h"

extern u8 Data_02006c08[];
extern const u8 Data_02005920[];
extern u8 Data_02006f20[];
extern u8 Data_02006e0e[];
extern u8 Data_02006e0c[];
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
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
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_02004d40(s32 value);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_020036a8(void);
void Func_020048f4(void);
s32 Func_02004e10(void);
void Func_02003bb8(struct ObjectRuntime *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

struct TileSprite {
    u8 unknown_00[18];
    u16 tile;
    u8 unknown_14[6];
    u8 frame;
};

s32 Func_02003418(struct ObjectRuntime *object)
{
    if (Random16Far() < 0x4000)
        Animation_ApplyChildValues(object, Random16Far() * 8 >> 16);
    return 0;
}
