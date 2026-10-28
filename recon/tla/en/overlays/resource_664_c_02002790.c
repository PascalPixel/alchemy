// Draft, differ 84 of 101: Func_02002898's twin for scenes 0x55 to 0x57 and
// 0x59 to 0x5b and 0x5d, putting back the cells from (70, 72): when the
// controlled character moves to another cell, saves the 1 by 2 cell
// attributes under it at (73, 72) and copies the spare ones in their place.
// Left: as Func_02002898's draft, the native keeps the saved values'
// addresses in high registers across the two copies.
#include "types.h"
#include "actor.h"
#include "adventure.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;

extern s32 gData_02006c0c;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

s32 Func_02005f84(void);
struct Actor *Actor_GetById(u32 id);
void Func_02005db4(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02002790(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 x = actor->x >> 20;
  s32 y = actor->y >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gData_02006c0c)
    return;
  gData_02006c0c = cell;
  gData_020073dc = 1;
  gData_020073e0 = 2;
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov664Room1 || scene == (s32)&SceneId_Ov664Room2 || scene == (s32)&SceneId_Ov664Room3) {
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
  } else if (scene == (s32)&SceneId_Ov664Room5 || scene == (s32)&SceneId_Ov664Room6 || scene == (s32)&SceneId_Ov664Room7 || scene == (s32)&SceneId_Ov664Room9) {
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
  }
  Func_02005db4(gData_020073e4, gData_020073e8, gData_020073dc, gData_020073e0,
                73, 72);
  Func_02005db4(70, 72, gData_020073dc, gData_020073e0, gData_020073e4,
                gData_020073e8);
}
