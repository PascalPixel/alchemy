/// @file
/// Scene code of overlay 0x6b0, in ROM order.

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
#include "module/scene/anemos_inner_sanctum.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"

extern struct ActorPlacement gData_02003b38[];
extern struct ActorPlacement gData_02003b50[];
extern struct ActorPlacement gData_02003b68[];
extern struct ActorPlacement gData_02003bc8[];
extern const u8 gData_02003c88[];
extern struct ActorPlacement gData_02003ce8[];

const struct ActorPlacement *Scene_GetPlacements(void) {
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

void Func_020006f8();
void Actor_SelectAnimation(struct Actor *, s32);
void Actor_StartScript();
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
  Actor_SetBlendMode(actor, 1);
  FieldActor_SetSpriteColor(actor, (u16)part);
  Scene_SetActorSpritePriority(actor, 2);
  Actor_SelectAnimation(actor, 7);
  Actor_SetSpriteFlags(actor, 0);
  FieldVector_AddPolarOffset(actor->unk4c, actor->unk64, &actor->position);
  Actor_StartScript(actor, gData_02003684);
  Func_020006f8(actor);
}

#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_020009b8(s32 x, s32 y, s16 part, s16 a3) {
  s32 i;

  for (i = 0; i != 16;) {
    struct Actor *object = (FieldActor_Create(0x2a0, x, y, 0x980000));
    s32 angle = i << 12;

    FieldVector_AddPolarOffset(0x200000, angle, &object->x);
    object->timer = angle;
    object->unk66 = a3;
    BYTE_AT(object, 98) = i;
    BYTE_AT(object, 99) = part;
    object->bounce = x;
    object->unk4c = 0x200000;
    object->gravity = y;
    object->motionFlags = 0;
    Actor_SetBlendMode(object, 1);
    FieldActor_SetSpriteColor(object, (u16)part);
    Scene_SetActorSpritePriority(object, 2);
    Actor_SelectAnimation(object, 7);
    Actor_SetSpriteFlags(object, 0);
    Actor_StartScript(object, gData_02003584);
    i += 2;
  }
}

void Func_02000a78(s32 a0, s32 a1, s16 a2) {
  struct Actor *t0;

  t0 = FieldActor_Create(0x2a0, (a0 << 16), ((a1 + 3) << 16), 0x980000);
  t0->motionFlags = 4;
  FieldActor_SetSpriteColor(t0, (u16)a2);
  Scene_SetActorSpritePriority(t0, 2);
  Actor_SelectAnimation(t0, 7);
  Actor_SetSpriteFlags(t0, 0);
}
