#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "game/item.h"

u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);

struct ShamanVillageObject *Func_02001fec(void) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Import_ActorCreate(489, 0x780000, 0x400000,
                                                        0x1080000));
  u8 *buffer = NULL;

  if (object == NULL)
    return NULL;
  {
    struct FieldSprite *sprite = object->sprite;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = 0;
    object->field5c = 1;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(65);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  return object;
}

void DriftScene_SpawnObject(s32 x, s32 y, s32 z, u16 value);

extern const u8 gData_02004cfc[];
extern const u8 gData_02004d40[];
extern const u8 gData_02004d84[];
extern const u8 gData_02004dc8[];
extern const u8 gData_02004e0c[];
extern const u8 gData_02004e5c[];
extern const u8 gData_02004eac[];
extern const u8 gData_02004f08[];

void Func_02002068(void) {
  struct ShamanVillageObject *rod;
  struct ShamanVillageObject *object;
  s32 angle;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  Sound_Play(78);
  angle = 0xc000;
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->facing = angle;
  wait(1);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(120), 0x280000, PIXELS(246), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  rod = Func_02001fec();
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 84 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 20)
  Actor_SetSpeed(15, 0x26666, 0x13333);
  Actor_SetPositionAndReset(15, 120, 242);
  Sound_Play(29);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(20);
  if (rod != NULL)
    Func_0200481c(rod);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, angle);
  Func_02004944(18, gMainRoutineWork.currentOwner);
  Func_02004944(7, gMainRoutineWork.currentOwner);
  Func_02004944(5, gMainRoutineWork.currentOwner);
  Func_02004944(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(18, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(18, gData_02004cfc);
  Actor_SetMovementTypeById(7, gData_02004d40);
  Actor_SetMovementTypeById(5, gData_02004d84);
  Actor_SetMovementTypeById(6, gData_02004dc8);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 14 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  Actor_SetSpeed(17, 0x13333, 0x9999);
  Actor_SetPositionAndReset(17, 136, 232);
  Event_ShowMessageAndWait(17, 0);
  Actor_SetSpeed(16, 0x13333, 0x9999);
  Actor_SetPositionAndReset(16, 104, 232);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 28 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(18, 0);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(15, 120, 252);
  Event_Wait(40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_SetPositionAndReset(15, 136, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 22 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_SetPositionAndReset(15, 104, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_SetPositionAndReset(15, 152, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 16 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_SetPositionAndReset(15, 88, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 80)
  Actor_SetPositionAndReset(15, 120, 252);
  Actor_SetPositionAndReset(15, 120, 242);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetModeById(15, 4);
  Event_ShowMessageAndWait(15, 0);
  Actor_ArmCallback(6, 0, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Func_080d47b4(18, 256, 20);
  Actor_Jump(18, 4, 0);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 20)
  Event_ShowMessageAndWait(15, 0);
  Actor_ArmCallback(6, FACING_NORTH, 0);
  Actor_ArmCallback(5, FACING_NORTH, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(15, 4);
  Event_ShowMessageAndWait(15, 0);
  Func_080d47b4(6, 259, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(15, FACING_SOUTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 257, 0)
  Event_ShowMessageAndWait(5, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(7, 265, 20)
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_ShowMessageAndWait(18, 0);
  Func_080d47b4(15, 265, 40);
  Event_ShowMessageAndWait(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 45 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 262, 20)
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(17, 0);
  Func_080d47b4(15, 257, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 20);
  Actor_SetModeById(17, 3);
  Event_ShowMessageAndWait(17, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Event_Wait(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Event_Wait(30);
    Func_080d47b4(16, 262, 20);
    Actor_ArmCallbackAndRefresh(16, 0);
    Event_ShowMessageAndWait(16, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Motion_SetModeAndWaitAnimation(17, 3);
    Func_080d47b4(15, 264, 40);
    Motion_SetModeAndWaitAnimation(15, 3);
    Actor_FaceDirection(17, FACING_SOUTH, 0);
    Actor_FaceDirection(16, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(15, FACING_SOUTH);
    Event_ShowMessageAndWait(15, 0);
    Actor_SetModeById(15, 3);
    Event_ShowMessageAndWait(15, 0);
    Camera_SetSpeedParameters(0x6666, 0xccc);
    Camera_MoveWithinMapBounds(PIXELS(180), 0x200000, z, 1);
  }
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->unk63 = 0;
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_SetSpeed(15, 0x19999, 0xcccc);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(15, gData_02004e0c);
  Event_Wait(20);
  Actor_SetMovementTypeById(17, gData_02004e5c);
  Actor_SetMovementTypeById(16, gData_02004eac);
  Sound_Play(78);
  do {
    wait(1);
  } while (object->unk63 == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(7);
  Actor_Stop(5);
  Actor_Stop(6);
  wait(1);
  Func_02004a2c();
  Camera_SetSpeedParameters(0x33333, 0x6666);
  Camera_MoveWithinMapBounds(PIXELS(120), -1, PIXELS(260), 1);
  Func_020049f4();
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_ArmCallbackAndRefresh(6, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirectionAndWait(5, FACING_WEST);
  Event_ShowMessageAndWait(0x2005, 0);
  Event_ShowMessageAndWait(18, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessageAndWait(18, 0);
  Actor_ArmCallback(7, 0, 0);
  Actor_ArmCallback(5, 0, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetMovementTypeById(6, gData_02004f08);
  Actor_SetMovementTypeById(5, gData_02004f08);
  Actor_SetMovementTypeById(7, gData_02004f08);
  Actor_SetMovementTypeAndWaitById(18, gData_02004f08);
  Event_Wait(20);
  GameFlag_SetBit(514);
  GameFlag_SetBit(0x9e1);
  Func_020048cc();
}

void Func_0200279c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  Event_ShowMessageAndWait(15, 0);
  Func_020048cc();
}

void Func_020027c0(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  Event_ShowMessageAndWait(16, 0);
  Func_020048cc();
}

void Func_020027e4(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  Event_ShowMessageAndWait(17, 0);
  Func_020048cc();
}

void Func_02002808(void) {
  if (GameFlag_Test(0x9fe)) {
    Sound_Play(0);
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetModeById(17, 5);
    Event_ShowMessageAndWait(17, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
    Func_020011f8();
    Actor_SetModeById(17, 1);
    GameFlag_SetBit(0x203);
    Func_020048cc();
  }
}

void Func_02002878(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  GameFlag_SetBit(0x205);
}

void Func_0200289c(void) {
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 304);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
  GameFlag_ClearBit(0x201);
}

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

u8 *Unit_Get(s32 owner);
struct ItemDefinition *Func_02004894(s32 item);

// Forwards to Party_ListActiveOwners; tagged where it is used.
void Func_0200290c(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindOwner(65) == -1)
    goto refuse;
  /* FAKEMATCH(wrapper): the count read through ListOwners; as a direct
   * Party_ListActiveOwners call with owner = owners before it, 4 of 119
   * instructions differ in tla-en. */
  count = FieldParty_ListMembers(owners);
  owner = owners;
  remaining = 8;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = (u16 *)(Unit_Get(slot) + 216);
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Func_02004894(item)->type - 1) <= 3)
        remaining--;
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->field1c4++;
  if (remaining != 0) {
  refuse:
    /* FAKEMATCH(pin): the emote pinned to r1 by SHAMAN_VILLAGE_EMOTE, so it and
     * the frames load before the actor; as a plain Func_080d47b4(15, 257, 20),
     * 2 of 119 instructions differ in tla-en. */
    SHAMAN_VILLAGE_EMOTE(15, 257, 20)
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    GameFlag_SetBit(0x9e0);
    GameFlag_SetBit(0x201);
  }
}

// The portrait mode task: reads its command script, eases the portrait's scale,
// blend and position, and queues its sprites.
#include "affine.h"
#include "vram_tab.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "edition.h"

struct Sprite {
  u32 words[3];
};

extern u32 gData_020064d0[];
extern s16 gData_02005158[];
extern s16 gData_020064bc;
extern s16 *gData_020064c0;
extern s32 gData_020064a0;
extern s16 gData_02006504;
extern s16 gData_0200649c;
extern s16 gData_02006500;
extern s16 gData_020064a8;
extern s16 gData_020064a4;
extern s16 gData_02006498;
extern s16 gData_02006490;
extern s16 gData_02006508;
extern s16 gData_020064b4;
extern s16 gData_020064b8;
extern s16 gData_020064c4;
extern s16 gData_020064ac;
extern s16 gData_020064b0;
extern s16 gData_02006494;

void Resource_ResetEntry(s32 entry);
void Func_020047cc(void *entry, s32 slot);

void Func_02002a20(void) {
  u32 *write = gData_020064d0;
  struct Sprite *sprite = (struct Sprite *)write;
  s32 tile = gResourceTableEntries[gData_02005158[0]].offset >> 5;
  u32 flags;
  s32 blend, scale, pos;
  s32 matrix, i, x, y, left;
  u32 work[2];

commands:
  if (gData_020064bc == 0) {
    switch (*gData_020064c0++) {
    case 0x4000:
      gData_020064a0 = *gData_020064c0++ << 8;
      gData_02006504 = *gData_020064c0++;
      gData_0200649c = 0;
      break;
    case 0x3000:
      gData_02006500 = gData_02006504;
      gData_02006504 = *gData_020064c0++;
      gData_0200649c = *gData_020064c0++;
      gData_020064a8 = 0;
      break;
    case 0x1000:
      gData_02006498 = gData_020064a4;
      gData_020064a4 = *gData_020064c0++;
      gData_02006490 = *gData_020064c0++;
      gData_02006508 = 0;
      break;
    case 0x2000:
      gData_020064b8 = gData_020064b4;
      gData_020064b4 = *gData_020064c0++;
      gData_020064c4 = *gData_020064c0++;
      gData_020064ac = 0;
      break;
    case 0x7fff:
      gData_020064bc = *gData_020064c0++;
      break;
    case -1:
      Task_RemoveFar(Func_02002a20);
      Resource_ResetEntry(gData_02005158[0]);
      return;
    }
    goto commands;
  }
  {
    s32 duration;
    gData_020064bc--;
    duration = gData_02006490;
    if (duration == 0) {
      scale = gData_020064a4;
    } else {
      s32 start, progress, target;
      start = gData_02006498;
      target = gData_020064a4;
      progress = ++gData_02006508;
      scale = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_02006490 = 0;
    }
  }
  {
    s32 duration;
    duration = gData_020064c4;
    if (duration == 0) {
      blend = gData_020064b4;
    } else {
      s32 start, progress, target;
      start = gData_020064b8;
      target = gData_020064b4;
      progress = ++gData_020064ac;
      blend = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_020064c4 = 0;
    }
  }
  {
    u32 packed;
    u32 *t;
    s32 duration;
    duration = gData_0200649c;
    if (duration == 0) {
      pos = gData_02006504;
    } else {
      s32 start, progress, target;
      start = gData_02006500;
      target = gData_02006504;
      progress = ++gData_020064a8;
      pos = start + (target - start) * progress / duration;
      if (progress >= duration)
        gData_0200649c = 0;
    }
    t = work;
    packed = scale << 16;
    {
      u32 low, word;
      t[1] &= 0xffff0000;
      low = packed >> 16;
      word = work[0];
      word &= 0xffff0000;
      word |= low;
      word &= 0xffff;
      word |= low << 16;
      work[0] = word;
    }
    matrix = (s16)AffineMatrix_BuildForEffect((struct AffineTransform *)t);
  }
  gData_020064a0 += pos;
  pos = gData_020064a0 / 256;
  switch (gData_020064b0) {
  case 1: {
    for (i = 0; i < 4; i++) {
      y = 56;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 48) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | tile;
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
  case 3: {
    for (i = 0; i < 2; i++) {
      y = 48;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gData_02006494);
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#if LANGUAGE == LANGUAGE_FRENCH
  case 4: {
    for (i = 0; i < 2; i++) {
      y = 64;
      flags = 0x80004000;
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
        *write++ = 0xf400 | (tile + gData_02006494);
        Func_020047cc(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#else
  case 4:
    y = 48;
    flags = 0xc0004000;
    left = pos + 56;
    if ((u32)(pos + 120) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gData_02006494);
      Func_020047cc(sprite, 236);
    }
    break;
#endif
  case 2:
    y = 48;
    flags = 0x80000000;
    left = pos + 88;
    if ((u32)(pos + 152) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gData_02006494);
      Func_020047cc(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
