#include "types.h"
#include "actor.h"

extern s16 gData_02001cde;
extern struct Actor *gData_02001ce0[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02000ba4(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->action = value;
    /* FAKEMATCH: ⚓️ stores the action before it addresses the list */
    asm volatile("");
    gData_02001ce0[gData_02001cde] = object;
    gData_02001cde++;
  }
}

void Func_02000bd4(u16 value) { gData_02001de2 = value; }

void Func_02000be0(u16 value) { gData_02001de4 = value; }
