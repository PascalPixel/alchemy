#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_ClearBit(s32 flag);
void Func_02000a98(s32 a, s32 b);
void Func_02001d68(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02000e50(void) { GameFlag_ClearBit(518); }
