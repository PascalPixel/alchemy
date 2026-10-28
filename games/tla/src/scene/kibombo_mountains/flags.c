#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001144(s32 a, s32 b);
void Func_0200114c(void);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_0200103c(struct ObjectRuntime *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_02000428(void) { GameFlag_SetBit(0x882); }

void Func_02000438(void) { Func_0200114c(); }
