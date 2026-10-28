#include "types.h"
#include "iwram_call.h"
#include "actor.h"

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct Actor *gData_02003600[];
extern u16 gData_02003702;
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Func_02002be4(s32 a, s32 b);
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c0c(s32 value);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_02000038(void) { Func_02002be4(21, 74); }
