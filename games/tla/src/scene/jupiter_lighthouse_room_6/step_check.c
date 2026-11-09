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

void Func_02000054(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Func_020018bc(value))
    FieldEvent_SetExitRequest(13);
}

void Func_02000084(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Func_020018bc(value))
    FieldEvent_SetExitRequest(14);
}
