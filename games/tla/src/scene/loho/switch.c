#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_02000f38(void);

void Func_02000e0c(void) {
  Actor_SetMovementTypeById(8, gData_020017d8);
  GameFlag_SetBit(800);
}

void Func_02000e24(void) {
  Actor_SetMovementTypeById(8, gData_020017a4);
  GameFlag_SetBit(800);
}

void Func_02000e3c(void) {
  GameFlag_ClearBit(800);
  Actor_SetMovementTypeById(8, gData_0200171c);
}
