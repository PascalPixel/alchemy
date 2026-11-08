#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
s32 Func_020018bc(s32 value);
void Func_02001ba4(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_02000ea0(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->action = value;
    /* FAKEMATCH: ⚓️ stores the action before it addresses the list */
    asm volatile("");
    gData_0200242c[gData_0200242a] = object;
    gData_0200242a++;
  }
}

void Func_02000ed0(u16 value) { gData_0200252e = value; }
