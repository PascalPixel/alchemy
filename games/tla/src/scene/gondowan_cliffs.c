/// @file
/// The scene code of overlay 0x66b that its scene tables belong to and the routine after it, in ROM order, then its data; the rest of the overlay is in gondowan_cliffs/.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

extern const u8 gData_02001698[];
extern const u8 gData_02001728[];

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Func_02000038(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

void Func_02000058(void) { Func_080d2b4c(21, 49); }

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66bRoom2)
    return gData_02001728;
  return gData_02001698;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_0200178c[];
extern const u8 gData_0200193c[];

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66bRoom2)
    return gData_0200193c;
  return gData_0200178c;
}


#include "objcmd.h"

// clang-format off
u32 gData_02001324[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02001360[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_0200139c[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};
// clang-format on

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

struct Actor *Func_02000160(s32 a0, s32 a1, s32 a2, s32 a3) {
  struct Actor *t0;

  t0 = FieldActor_Create(a3, a0, a1, a2);
  if (t0 != 0) {
    Scene_SetActorSpritePriority(t0, 1);
    t0->motionFlags = 0;
    t0->collisionFlags = 8;
    Actor_SetSingleChildFlags(t0, 0);
    Actor_SetSpriteColor(t0, 0xf);
    t0->priorityFlags = 0x22 | t0->priorityFlags;
    return t0;
  }
  return 0;
}
