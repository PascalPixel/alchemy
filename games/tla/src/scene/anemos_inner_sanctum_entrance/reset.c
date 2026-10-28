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

void Func_02000494(void) {
  s32 i;

  for (i = 9; i < 15; i++) {
    struct Actor *object = Actor_GetById(i);
    u8 *flag = (u8 *)object + 98;
    s32 none = 0;
    s32 on = 1;

    /* FAKEMATCH: ⚓️ builds both values before it stores either */
    asm("" : "+l"(none), "+l"(on));
    *flag = on;
    *(s32 *)((u8 *)object + 108) = none;
  }
}

void Func_020004b4(struct Actor *object) {
  if ((object->terrainHeight = Map_GetTerrainHeight(0, object->x, object->z)) >=
      -0x200000)
    Actor_SetSingleChildFlags(object, 1);
  else
    Actor_SetSingleChildFlags(object, 0);
}
