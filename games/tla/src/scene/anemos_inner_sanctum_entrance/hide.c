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

s32 Func_0200033c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}
