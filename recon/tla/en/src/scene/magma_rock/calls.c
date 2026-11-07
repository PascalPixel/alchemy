#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 gData_02003dba[];
struct Actor *Actor_GetById(s32 id);
void Func_02003bcc(s32 value);
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02001ba4(void) { Func_02003bcc(0); }

void Func_02001bb0(void) { Func_02002e30(gData_02003dba); }

#include "call.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02003ad4(s32 a, s32 x, s32 z, s32 frames);

void Func_02001bc0(void) {
  Call4((void (*)())Func_02003ad4, 0, 0x1080000, 0x2280000, 30);
  GameFlag_SetBit(0x212);
}

void Func_02001be0(void) {
  Call4((void (*)())Func_02003ad4, 0, 0x1080000, 0x2280000, 0);
  GameFlag_ClearBit(0x212);
}
