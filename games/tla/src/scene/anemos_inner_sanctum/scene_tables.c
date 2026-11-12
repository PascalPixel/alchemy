#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"

extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;
extern const u8 gData_02003b38[];
extern const u8 gData_02003b50[];
extern const u8 gData_02003b68[];
extern const u8 gData_02003bc8[];
extern const u8 gData_02003c88[];
extern const u8 gData_02003ce8[];

const u8 *Func_02000818(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1)
    return gData_02003b50;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2)
    return gData_02003b68;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3)
    return gData_02003bc8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    return gData_02003c88;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room5)
    return gData_02003ce8;
  return gData_02003b38;
}

#include "system/iwram_call.h"
#include "event_inline.h"

struct Work {
  u8 pad0[8];
  struct Vector3 position;
  s32 unk14;
  u8 pad18[0x18];
  s32 unk30;
  s32 unk34;
  u8 pad38[0x0c];
  s32 unk44;
  s32 unk48;
  s32 unk4c;
  u8 pad50[5];
  u8 unk55;
  u8 pad56[0x0c];
  u8 unk62;
  u8 pad63;
  s16 unk64;
  u16 unk66;
};

s32 ArcTan2();
void Func_020006f8();
void Func_020032e0();
void Func_020032e8();
u32 Random16Far();
extern u8 gData_02003684[];

void Func_0200088c(struct Vector3 *from, struct Vector3 *to, s16 part) {
  struct Work *actor;
  s32 x, z, distance;

  actor = ((struct Work *)FieldActor_Create(0x2a0, from->x, from->y, from->z));
  x = (from->x - to->x) >> 16;
  z = (from->z - to->z) >> 16;
  actor->unk64 = ArcTan2(from->z - to->z, from->x - to->x);
  actor->unk66 = (((Random16Far() & 2) - 1) * (Random16Far() & 15)) << 8;
  actor->unk62 = part;
  actor->unk55 = 0;
  actor->unk44 = to->x;
  actor->unk48 = to->z;
  distance = Iwram_Sqrt(x * x + z * z) << 16;
  actor->unk4c = distance;
  actor->unk30 = distance / 16;
  actor->unk34 = from->y;
  actor->unk14 = (from->y - to->y) / 16;
  Animation_SetStateFlags(actor, 1);
  Actor_SetSpriteColor(actor, (u16)part);
  Scene_SetActorSpritePriority(actor, 2);
  Func_020032e0(actor, 7);
  Actor_SetSingleChildFlags(actor, 0);
  FieldVector_AddPolarOffset(actor->unk4c, actor->unk64, &actor->position);
  Func_020032e8(actor, gData_02003684);
  Func_020006f8(actor);
}
