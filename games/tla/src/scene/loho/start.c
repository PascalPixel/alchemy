#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitTicks(s32 ticks);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const u8 *callback);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 value);
void Func_02001400(s32 a, s32 b, s32 c);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_020013f8(s32 value);
void Func_02000f38(void);

void Func_02000038(void) { Func_02001400(19, 3, 16); }
