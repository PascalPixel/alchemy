#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
s32 Func_020018bc(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_020002dc(s32 value) { gData_020023c0 = value; }
