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
#include "ram_buffer.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02000724();
void PartyInventory_Remove();

struct EventWorkLock {
  u8 unk000[0x15a];
  u16 locked;
};

static inline void SetCollisionCode(s32 x, s32 z, s32 layer, s32 value) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's
   * own constants (direct: differ 12) */
  ((u8)Func_080dbdd4(x, z, layer, value));
}

void Func_02000054(void) {
  MapGrid_CopyCellsLayered(17, 10, 1, 4, 19, 10);
  Map_CopyCellAttributeRect(17, 10, 1, 1, 19, 10);
  SetCollisionCode(0x1300000, 0xa00000, 0, 254);
  Actor_SetHorizontalPositionWithTerrain(14, PIXELS(504), PIXELS(168));
  if (GameFlag_Test(0xa00)) {
    Func_02000724(14);
    ((u8)Func_080dbdd4(0x1f00000, 0xa00000, 0, 254));
  }
}

void Func_020000c0(void) {
  Func_0802b450(16, 10, 1, 4, 19, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, 19, 10);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Map_CopyCellAttributeRect(30, 10, 1, 1, 31, 10);
}

void Func_02000108(s32 unused, s32 id) { Actor_SetSpritePriority(id, 2); }

void Func_02000114(s32 unused, s32 id) {
  struct FieldActor *object;
  s32 i;

  object = ((struct FieldActor *)Actor_GetById(id));
  if (object->x >> 20 == 10 && object->z >> 20 == 32) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      wait(1);
    }
    Func_02000724(id);
    GameFlag_SetBit(0xa06);
  }
}

void Func_02000164(void) { Import_EventSetExitRequest(12); }

void Func_02000170(void) { Import_EventSetExitRequest(13); }

void Func_0200017c(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

void Func_020001ac(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

void Func_020001dc(s32 item) {
  GameFlag_SetBit(0xa07);
  Sound_Play(134);
  Func_0200017c();
  PartyInventory_Remove(item);
}

void Func_020001fc(s32 item) {
  GameFlag_SetBit(0xa08);
  Sound_Play(134);
  Func_020001ac();
  PartyInventory_Remove(item);
}

void Func_0200021c(void) {
  ((struct EventWorkLock *)Ram_HeapSlots->eventWork)->locked = 1;
  FieldMessage_Show(MsgFieldDoorLocked, 1);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }
