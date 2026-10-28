#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

extern const u8 gData_02000f10[];
extern const u8 gData_02000ebc[];
u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
void Func_02000724(void);
void Func_020006e4(void);
void Func_020006ec(void);
void Func_0200072c(s32 value);
void Func_020006fc(s32 id, s32 x, s32 z);
void Func_020006a4(void);
void Func_020006ac(void);

void Func_02000624(void) { Func_020006a4(); }

void Func_0200062c(void) { Func_020006ac(); }
