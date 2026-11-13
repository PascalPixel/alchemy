#include "../shared/routines.h"
#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "game/item.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/scene/shaman_village.h"
#include "task.h"

/// The Shaman's Rod (item 65) drawn as an actor over the leader.
struct ShamanVillageObject *ShamanVillage_CreateRodIcon(void) {
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

extern u32 gData_02004d84[];
extern u32 gData_02004e5c[];

/// ★ Named from the game: "That's the Shaman's Rod!"
/// (MsgShamanRecognizeShamanRod)
void ShamanVillage_ShowShamansRod(void) {
  struct ShamanVillageObject *rod;
  struct ShamanVillageObject *object;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(78);
  angle = 0xc000;
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->facing = angle;
  wait(1);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  ((struct ShamanVillageObject *)Func_080d4568())->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(120), 0x280000, PIXELS(246), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 264);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  rod = ShamanVillage_CreateRodIcon();
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 84 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 20)
  Actor_SetSpeed(15, 0x26666, 0x13333);
  Actor_WalkToAndWait(15, 120, 242);
  Event_PlaySound(29);
  Event_SetMessageId((s32)MsgShamanRecognizeShamanRod);
  Event_ShowMessage(15, 0);
  Event_Wait(20);
  if (rod != NULL)
    Actor_Release(rod);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, angle);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(18, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetScript(18, gData_02004cfc);
  Actor_SetScript(7, gData_02004d40);
  Actor_SetScript(5, gData_02004d84);
  Actor_SetScript(6, gData_02004dc8);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 14 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  Actor_SetSpeed(17, 0x13333, 0x9999);
  Actor_WalkToAndWait(17, 136, 232);
  Event_ShowMessage(17, 0);
  Actor_SetSpeed(16, 0x13333, 0x9999);
  Actor_WalkToAndWait(16, 104, 232);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(15, 3);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 28 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Event_ShowMessage(15, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_SetSpeed(15, 0xcccc, 0x6666);
  Actor_WalkToAndWait(15, 120, 252);
  Event_Wait(40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_WalkToAndWait(15, 136, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 22 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_WalkToAndWait(15, 104, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 25 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  Actor_WalkToAndWait(15, 152, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 20);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 16 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 40)
  Actor_WalkToAndWait(15, 88, 252);
  Actor_FaceDirection(15, FACING_SOUTH, 40);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 257, 80)
  Actor_WalkToAndWait(15, 120, 252);
  Actor_WalkToAndWait(15, 120, 242);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetAnimation(15, 4);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldActor_ShowEmote(18, 256, 20);
  Actor_Jump(18, 4, 0);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 19 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 265, 20)
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessage(15, 0);
  FieldActor_ShowEmote(6, 259, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(15, FACING_SOUTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 257, 0)
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(15, 3);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 8 of 711 instructions differ
   * in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(7, 265, 20)
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST, 0);
  Actor_SetAttachedEffect(18, 258);
  Event_ShowMessage(18, 0);
  FieldActor_ShowEmote(15, 265, 40);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 45 of 711 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 262, 20)
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(15, 257, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 20);
  Actor_SetAnimation(17, 3);
  Event_ShowMessage(17, 0);
  {
    s32 y = 0x500000;
    s32 z = 0xf20000;

    DriftScene_SpawnObject(0x740000, y, z, 0xe000);
    Event_Wait(10);
    DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
    Event_Wait(30);
    FieldActor_ShowEmote(16, 262, 20);
    Actor_FaceDirectionAndWait(16, 0);
    Event_ShowMessage(16, 0);
    Actor_FaceDirectionAndWait(17, FACING_WEST);
    Actor_SetAnimationAndWait(17, 3);
    FieldActor_ShowEmote(15, 264, 40);
    Actor_SetAnimationAndWait(15, 3);
    Actor_FaceDirection(17, FACING_SOUTH, 0);
    Actor_FaceDirection(16, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(15, FACING_SOUTH);
    Event_ShowMessage(15, 0);
    Actor_SetAnimation(15, 3);
    Event_ShowMessage(15, 0);
    FieldCamera_SetSpeed(0x6666, 0xccc);
    FieldCamera_MoveTo(PIXELS(180), 0x200000, z, 1);
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
  Actor_SetScript(15, gData_02004e0c);
  Event_Wait(20);
  Actor_SetScript(17, gData_02004e5c);
  Actor_SetScript(16, gData_02004eac);
  Event_PlaySound(78);
  do {
    wait(1);
  } while (object->unk63 == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(7);
  Actor_Stop(5);
  Actor_Stop(6);
  wait(1);
  Party_PlaySavedMusic();
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(120), -1, PIXELS(260), 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(15, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_FaceDirectionAndWait(6, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(5, FACING_WEST);
  Event_ShowMessage(0x2005, 0);
  Event_ShowMessage(18, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetScript(6, gData_02004f08);
  Actor_SetScript(5, gData_02004f08);
  Actor_SetScript(7, gData_02004f08);
  Actor_RunScriptAndWait(18, gData_02004f08);
  Event_Wait(20);
  Event_SetFlag(514);
  Event_SetFlag(0x9e1);
  Event_End();
}

/// ★ Named from the game: "If you're here offering charity, I won't have it...
/// Or did you want to show me something?" (MsgShamanShowMeSomething)
void ShamanVillage_ShowItemToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  Event_ShowMessage(15, 0);
  Event_End();
}

/// ★ Named from the game: "Did you want to sell it? Because I don't have any
/// money." (MsgShamanCannotBuyGift)
void ShamanVillage_ShowItemToGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  Event_ShowMessage(16, 0);
  Event_End();
}

/// ★ Named from the game: "Show it to Moapa if you must"
/// (MsgShamanShowItToMoapa)
void ShamanVillage_ShowItemToOtherGuard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  Event_ShowMessage(17, 0);
  Event_End();
}

/// ★ Named from the game: "Hoolabaloo!" (MsgShamanFortuneGreeting)
void ShamanVillage_GreetFortuneSeeker(void) {
  if (Event_GetFlag(0x9fe)) {
    Event_PlaySound(0);
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetAnimation(17, 5);
    Event_ShowMessage(17, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
    ShamanVillage_HopFortuneTeller();
    Actor_SetAnimation(17, 1);
    Event_SetFlag(0x203);
    Event_End();
  }
}

/// Growth on the sprout, actor 21: its cells become climbable, toward the
/// Spirit Gloves' chest (flag 0x205).
void ShamanVillage_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  Event_SetFlag(0x205);
}

void ShamanVillage_StepBackFromTrial(void) {
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 304);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
  Event_ClearFlag(0x201);
}

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

// Forwards to Party_ListMembers; tagged where it is used.
/// Moapa takes the challenge only from a party with the Shaman's Rod and eight
/// pieces of equipment.
/// ★ Named from the game: "But what's this? You no longer have the Shaman's
/// Rod?" (MsgShamanMissingShamanRod)
void ShamanVillage_CheckTrialEntry(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindMember(65) == -1)
    goto refuse;
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
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessage(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    Event_SetFlag(0x9e0);
    Event_SetFlag(0x201);
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

extern s16 gData_02006500;

/// The portrait mode task: reads its command script, eases the portrait's
/// scale, blend and position, and queues its sprites.
void ShamanVillage_RunLettering(void) {
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
      Task_RemoveFar(ShamanVillage_RunLettering);
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
        Oam_AddSpriteAtDepth(sprite++, 236);
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
        Oam_AddSpriteAtDepth(sprite++, 236);
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
        Oam_AddSpriteAtDepth(sprite++, 236);
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
      Oam_AddSpriteAtDepth(sprite, 236);
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
      Oam_AddSpriteAtDepth(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

/// Starts the mode task: keeps the mode and its parameter, schedules
/// ShamanVillage_RunLettering and picks the script the mode runs, then rewinds the
/// task's timer and position.
void ShamanVillage_StartLettering(s32 modeArg, s32 paramArg) {
  s32 handler;

  gData_020064b0 = modeArg;
  gData_02006494 = paramArg << 4;

  Task_Add(ShamanVillage_RunLettering, 0x480);

  handler = (s32)gData_0200515a;
  if (modeArg == 2) {
    handler = (s32)gData_02004b58;
  } else if (modeArg == 4) {
    handler = (s32)gData_02005186;
  } else if (modeArg == 3) {
    if (paramArg != 0) {
      handler = (s32)gData_02004b80;
    } else {
      handler = (s32)gData_02005204;
    }
  }

  gData_020064bc = 0;
  gData_020064c0 = (s16 *)handler;
  gData_02006504 = 0;
  gData_0200649c = 0;
  gData_020064a0 = 0;
}

#include "heap_release.h"
#include "resource.h"
#include "dma.h"
#include "types.h"
#include "input.h"

// The European editions' lettering sheet has one more palette row.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

/// The decoded lettering sheet: its palette rows, then eight graphics of 32
/// tiles each.
struct LetteringSheet {
  u8 palettes[LETTERING_PALETTE_ROWS][32];
  u8 graphics[8][1024];
};

/// Decodes the lettering sheet, sends one graphic's palette to OBJ palette 15
/// and loads its tiles into a VRAM block of its own. Graphic 8 has a palette of
/// its own and graphic 4's tiles.
void ShamanVillage_LoadLettering(s32 graphic) {
  // BUG: this holds seven graphics of the sheet's eight, so decoding the sheet
  // writes 1024 bytes past the block.
#ifdef BUGFIX
  struct LetteringSheet *sheet = Heap_AllocEwram(sizeof(struct LetteringSheet));
#else
  struct LetteringSheet *sheet =
      Heap_AllocEwram((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
  struct DmaRegisters volatile *dma;
  s32 palette;

  if (gData_02005158[0] == -1)
    gData_02005158[0] = Resource_FindFreeEntry();
  palette = gData_02004bfe[graphic];
  if (graphic == 8)
    graphic = 4;
  Func_0200479c(Resource_GetTableEntry((s32)&ResourceId_LetteringBanner),
                sheet);
  Dma_Set(sheet->palettes[0] + palette, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 32 / 4, REG_DMA3);
  Vram_Load(gData_02005158[0], sizeof(sheet->graphics[0]),
            sheet->graphics[graphic]);
  dma = (struct DmaRegisters volatile *)REG_DMA3;
  while ((s32)dma->control < 0)
    ;
  Heap_Release(sheet);
}

/// The lettering banner's three counts and the start, before the race up Trial
/// Road.
void ShamanVillage_CountDownToStart(void) {
  gData_0200515a[15] = 180;
  Event_Wait(30);
  Event_PlaySound(78);
  ShamanVillage_LoadLettering(5);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_StartLettering(2, 1);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_LoadLettering(6);
  ShamanVillage_StartLettering(2, 0);
  Event_PlaySound(236);
  Event_Wait(60);
  ShamanVillage_LoadLettering(7);
  ShamanVillage_StartLettering(4, 0);
  Event_PlaySound(237);
  Event_PlaySound(76);
}

/// Moapa's first test: Sheba steps up and casts Whirlwind on the purple rock,
/// and the sand is gone (flag 0x94c).
/// ★ Named from the game: "You can do it, !" (MsgShamanCheerForCompanion)
void ShamanVillage_ShebaClearsSand(void) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Actor_GetById(16));

  s32 result;
  s32 owner;
  s32 angle;
  u8 *done;

  work[34] = 0;
  work[35] = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(528), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 536);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02004f80);
  Actor_SetScript(5, gData_02004fc4);
  Actor_SetScript(18, gData_02004f3c);
  Actor_RunScriptAndWait(7, gData_02005008);
  Actor_FaceDirectionAndWait(5, 0);
  Event_SetMessageId((s32)MsgShamanCheerForCompanion);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(0x2006, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Actor_FaceDirectionAndWait(6, 0);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 168, 524);
  Actor_FaceDirection(15, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  ShamanVillage_CastWhirlwindOnSand();
  Event_PlaySound(29);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetAttachedEffect(15, 258);
  Event_Wait(20);
  Event_ShowMessage(15, 0);
  Actor_SetAttachedEffect(17, 258);
  Event_ShowMessage(0x8011, 0);
  Actor_SetAttachedEffect(16, 258);
  FieldEvent_ShowMessageThenWait(0x8010, 0, 20);
  Actor_RunRepeatedMotion(15, 1);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 91 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 256, 0)
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessage(15, 0);
  Actor_Jump(6, 4, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessage(0x4007, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_SetAnimation(15, 4);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(18, 0, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 88 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(5, 256, 20)
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_ShowMessage(0x200f, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 257, 40)
  Event_ShowMessage(0x4012, 0);
  Actor_SetAnimationAndWait(15, 3);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 256, 0)
  FieldActor_ShowEmote(16, 256, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Actor_StartRepeatedMotion(17, 1);
  Event_ShowMessage(17, 0);
  Actor_StartRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 29 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Event_ShowMessage(15, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 30 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(6, 259, 0)
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(0x8011, 0);
  Actor_FaceDirectionAndWait(16, FACING_NORTHEAST);
  Actor_SetAnimation(16, 4);
  Event_ShowMessage(0x8010, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(560), 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkTo(16, 120, 560);
  Actor_WalkToAndWait(17, 216, 560);
  Actor_SetAnimation(17, 1);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_WalkTo(16, 152, 560);
  Actor_WalkToAndWait(17, 184, 560);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH);
  Actor_SetSpeed(15, 0x10000, 0x8000);
  Actor_SetAnimation(15, 2);
  Actor_SetAnimation(17, 2);
  Actor_SetAnimation(16, 2);
  Actor_SetDestinationOffset(15, 0, 16);
  Actor_SetDestinationOffset(17, 0, 16);
  Actor_SetDestinationOffset(16, 0, 16);
  Actor_WaitForMoveById(15);
  Actor_SetAnimation(15, 1);
  Actor_SetAnimation(17, 1);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirectionAndWait(18, 0x3000);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(15, FACING_NORTHWEST, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 23 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(18, 265, 20)
  Event_ShowMessage(18, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(17, 257, 0)
  FieldActor_ShowEmote(16, 257, 20);
  FieldActor_ShowEmote(5, 264, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(16, 0, 0);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(15, FACING_NORTHEAST, 0);
  /* FAKEMATCH(pin): the emote, pinned to r1 by SHAMAN_VILLAGE_EMOTE, and the
   * frames load before the actor; as a plain call, 83 of 1045 instructions
   * differ in tla-en (see the macro). */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  FieldActor_ShowEmote(6, 259, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(15, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(15, 4);
  Event_Wait(20);
  FieldActor_ShowEmote(17, 262, 0);
  Actor_FaceDirectionAndWait(17, FACING_SOUTHWEST);
  Event_ShowMessage(17, 0);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessage(16, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Actor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST);
  Event_ShowMessage(16, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Event_ShowMessage(5, 0);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(15, 258);
  Actor_FaceDirection(15, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(15, FACING_NORTHEAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTHWEST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(15, FACING_NORTH);
  FieldActor_ShowEmote(15, 265, 40);
  Event_ShowMessage(15, 0);
  Actor_SetAnimation(15, 4);
  Event_OpenMessage(15, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  done = &object->unk63;
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(15, 4);
    Event_ShowMessage(15, 0);
  } else {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimationAndWait(15, 4);
    EVENT_WORK->field1c4++;
    Event_ShowMessage(15, 0);
    Actor_SetAnimation(15, 3);
    Event_ShowMessage(15, 0);
  }
  Actor_WalkToAndWait(15, 212, 582);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(500), 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 15);
  Actor_WatchAndSetScript(18, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(6, 15);
  *done = 0;
  Actor_SetScript(15, gData_0200504c);
  Event_Wait(20);
  Actor_SetScript(17, gData_0200504c);
  Actor_SetScript(16, gData_0200504c);
  Event_PlaySound(78);
  do {
    wait(1);
  } while (*done == 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  Party_PlaySavedMusic();
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(540), 1);
  Event_Wait(20);
  angle = 0x4000;
  Actor_SetPositionAndFacing(15, PIXELS(224), PIXELS(280), angle);
  Actor_SetPositionAndFacing(16, PIXELS(208), PIXELS(264), angle);
  Actor_SetPositionAndFacing(17, PIXELS(240), PIXELS(264), angle);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(18, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(18);
  Actor_Stop(5);
  Actor_Stop(7);
  Actor_Stop(6);
  ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(18))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(5))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(7))->update = NULL;
  ((struct ShamanVillageObject *)Actor_GetById(6))->update = NULL;
  wait(1);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetScript(7, gData_020050bc);
  Actor_SetScript(6, gData_020050bc);
  Actor_SetScript(5, gData_020050bc);
  Actor_RunScriptAndWait(18, gData_020050bc);
  Event_SetFlag(0x94c);
  Event_End();
}

void ShamanVillage_CastWhirlwindOnSand(void) {
  Actor_SetSpritePriority(6, 1);
  Event_PlaySound(131);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(((struct ShamanVillageObject *)Actor_GetById(6)), 2);
  Event_Wait(40);
  BattleEffect_SetObjectUpdateMode(((struct ShamanVillageObject *)Actor_GetById(6)), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  ShamanVillage_BlowAwaySand();
  Actor_SetSpritePriority(6, 2);
}

void ShamanVillage_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;
  struct ShamanVillageObject *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Event_PlaySound(188);
  else
    Event_PlaySound(158);
  MapGrid_CopyCellsTo(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = ((struct ShamanVillageObject *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                        object->z);
    Actor_WaitForMove(object);
    object->facing = 0xc000;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02004c08);
  wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003c58(void) { ShamanVillage_EnterDoor(43, 40, 8, 22, 1, 2); }

void Func_02003c74(void) { ShamanVillage_EnterDoor(43, 42, 4, 26, 1, 2); }

void Func_02003c90(void) { ShamanVillage_EnterDoor(43, 42, 27, 21, 1, 2); }

void Func_02003cac(void) { ShamanVillage_EnterDoor(43, 40, 28, 17, 1, 2); }

void Func_02003cc8(void) { ShamanVillage_EnterDoor(43, 42, 10, 8, 1, 2); }

void Func_02003ce4(void) {
  if (Event_GetFlag(0x202)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(13);
  } else {
    ShamanVillage_EnterDoor(43, 40, 7, 6, 1, 2);
  }
}

/// Back at Trial Road's foot, Moapa offers the trial again.
/// ★ Named from the game: "In order to reach the battle at the summit, you
/// cannot fail on Trial Road." (MsgShamanMustFinishTrialRoad)
void ShamanVillage_RestartTrial(void) {
  s32 angle;

  Event_ClearFlag(0x11b);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(17, 0, 0);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0xc000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 0);
  wait(1);
  Map_CenterOnFollowedPoint();
  wait(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(16, PIXELS(184), PIXELS(128), angle);
  Actor_SetPositionAndFacing(17, PIXELS(264), PIXELS(128), angle);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 1);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_WalkTo(16, 208, 264);
  Actor_WalkToAndWait(17, 240, 264);
  Actor_SetAnimation(16, 1);
  Event_Wait(10);
  Actor_FaceDirection(16, angle, 0);
  Actor_FaceDirection(17, angle, 0);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  Event_ShowMessage(0x200f, 0);
  Actor_SetAnimation(15, 3);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(15, 3);
      Event_PlaySound(76);
      Event_ShowMessage(0x200f, 0);
      Event_SetFlag(0x200);
      ShamanVillage_ExplainTrialRules();
      return;
    }
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4 += 2;
  }
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_StepBackFromTrial();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessage(0x200f, 0);
    ShamanVillage_CheckTrialEntry();
  }
  Party_PlaySavedMusic();
  Event_ClearFlag(0x12f);
  Event_End();
}

/// ★ Named from the game: "It would be wise of you to quit. Even if you reach
/// the top, you cannot beat me." (MsgShamanMoapaAdvisesLeaving)
void ShamanVillage_AdviseQuitting(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanMoapaAdvisesLeaving);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(16))->facing = 0x8000;
  ((struct ShamanVillageObject *)Actor_GetById(17))->facing = 0x8000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(280), 0);
  wait(1);
  Map_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_ShowMessage(0x200f, 0);
  ShamanVillage_StepBackFromTrial();
  Event_ClearFlag(0x12f);
  Event_End();
}

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    ShamanVillage_SetUpVillage();
  else if (scene == (s32)&SceneId_ShamanVillage3)
    ShamanVillage_SetUpRoom3();
  else if (scene == (s32)&SceneId_ShamanVillage2)
    ShamanVillage_SetUpRoom2();
  else if (scene == (s32)&SceneId_ShamanVillage4)
    ShamanVillage_SetUpRoom4();
  else if (scene == (s32)&SceneId_ShamanVillage6)
    ShamanVillage_SetUpRoom6();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void ShamanVillage_SetUpVillage(void) {
  if (Event_GetFlag(0x94c)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    Actor_SetSpriteColor(((struct ShamanVillageObject *)Actor_GetById(14)),
                           6);
  } else if (!Event_GetFlag(0x109)) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(15, PIXELS(168), PIXELS(568), angle);
    Actor_SetPositionAndFacing(17, PIXELS(216), PIXELS(536), angle);
    Actor_SetPositionAndFacing(16, PIXELS(120), PIXELS(536),
                                                   angle);
    wait(1);
  }
  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  Actor_SetAnimation(10, 2);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  Actor_SetAnimation(13, 2);
  if (!Event_GetFlag(0x109)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 11)
      ShamanVillage_RestartTrial();
    else if (entrance == 9)
      ShamanVillage_AdviseQuitting();
  }
  if (Event_GetFlag(0x9fe)) {
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(16, 0, 0);
    MapGrid_CopyCellAttributes(10, 12, 3, 1, 10, 13);
    MapGrid_CopyCellAttributes(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

void ShamanVillage_SetUpRoom3(void) {
  struct ShamanVillageObject *object;

  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  Actor_SetAnimation(10, 2);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  Actor_SetAnimation(13, 2);
  ((struct ShamanVillageObject *)Actor_GetById(14))->motionFlags = 0;
  if (Event_GetFlag(0x94e)) {
    Actor_SetPositionAndResetMotion(
        ((struct ShamanVillageObject *)Actor_GetById(14)), PIXELS(520),
        PIXELS(32), PIXELS(184));
    wait(1);
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(10);
  }
  if (Event_GetFlag(0x9e1)) {
    MapGrid_CopyCellsTo(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (Event_GetFlag(0x94d))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetPosition(17, 0, 0);
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->collisionFlags = 8;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(16)),
                            0);
}

void Func_020042c8(struct ShamanVillageObject *object) {
  s32 step;

  object->x += object->field64 << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->field64 += 2;
  if (--object->field68 == 0)
    Actor_Release(object);
}

void Func_0200430c(s32 x, s32 y, s32 z) {
  struct ShamanVillageObject *object =
      ((struct ShamanVillageObject *)Import_ActorCreate(285, x, y, z));

  if (object != NULL) {
    object->field5c = 2;
    object->motionFlags = 0;
    object->field64 = 0;
    object->priorityFlags &= ~1;
    Actor_SetSpriteColor(object, 7);
    object->sprite->priority = 1;
    object->priorityFlags |= 2;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    object->field68 = 60;
    object->update = Func_020042c8;
    Import_ActorSetMode(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

void Func_02004394(void) {
  if ((gFrameCount & 63) == 0) {
    Func_0200430c(0x480000, 0x420000, 0x1780000);
    Func_0200430c(0x680000, 0x620000, 0x980000);
  }
}

DEFINE_FUNC_020043C8(Func_020043c8, 21, 14)

struct IoReg16 {
  u16 value;
};

void ShamanVillage_SetUpRoom2(void) {
  if (!Event_GetFlag(0x109)) {
    if (Event_GetFlag(0x9fe)) {
      Actor_SetPosition(16, PIXELS(104), PIXELS(200));
      Actor_SetPosition(17, PIXELS(136), PIXELS(200));
    } else if (Event_GetFlag(0x94f) && !Event_GetFlag(0x9e1)) {
      Actor_SetPosition(15, PIXELS(120), PIXELS(208));
      Actor_SetPosition(16, PIXELS(104), PIXELS(212));
      Actor_SetPosition(17, PIXELS(136), PIXELS(212));
      wait(1);
    }
  }
  if (Event_GetFlag(0x202))
    MapGrid_CopyCellsTo(43, 40, 7, 6, 1, 2);
  if (Event_GetFlag(0x94d)) {
    s32 width = 1;
    s32 height = 2;

    MapGrid_CopyCellsTo(48, 40, 77, 21, width, height);
    MapGrid_CopyCellsTo(48, 40, 81, 25, width, height);
  }
  Func_02004a44();
  Func_02004a4c(0, 0x204, 19, 20);
  if (Event_GetFlag(0x205))
    ShamanVillage_GrowVine();
  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    /* FAKEMATCH(barrier): the empty asm holds r0 until the priority is
     * complete, so the callback is loaded last as the native has; as
     * Task_Add(Func_020043c8, 0x480), 8 of 128 instructions differ in
     * tla-en. */
    __asm__("" : "+l"(priority) : : "r0");
    Task_Add(Func_020043c8, priority);
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_02004394, priority);
  }
}

void ShamanVillage_SetUpRoom4(void) {
  u32 i;

  ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      521;
  ((struct ShamanVillageObject *)Actor_GetById(12))->collisionFlags = 8;
  if (Event_GetFlag(0x9fe)) {
    s32 angle = 0x3000;

    Actor_SetPositionAndFacing(8, PIXELS(88), PIXELS(92), angle);
    Actor_SetPosition(11, PIXELS(156), PIXELS(72));
  } else {
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if ((gMainRoutineWork.entrance == 30 || gMainRoutineWork.entrance == 31) &&
      !Event_GetFlag(0x109)) {
    Party_RestoreHpPp(1);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(
                                  gMainRoutineWork.currentOwner)),
                              0);
    wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    i = 0;
    goto test_stand;
  next_stand:
    i++;
  test_stand:
    if (i < 60) {
      wait(1);
      if (gInput.held == 0)
        goto next_stand;
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
    Event_Wait(10);
    i = 0;
    goto test_turn;
  next_turn:
    i++;
  test_turn:
    if (i < 30) {
      wait(1);
      if (gInput.held == 0)
        goto next_turn;
    }
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 813, 120);
    Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(
                                  gMainRoutineWork.currentOwner)),
                              1);
    Event_End();
    if (gMainRoutineWork.entrance == 30) {
      Event_SetFlag(0x9fe);
      Event_SetFlag(0x94d);
    }
  }
}

DEFINE_FUNC_020043C8(Func_020046bc, 9, 10)

void ShamanVillage_SetUpRoom6(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 521;
  if (Event_GetFlag(0x9fe)) {
    Task_Add(Func_020046bc, 0x480);
  } else {
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
  }
}

// clang-format off
u32 gData_02004aa4[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004ae0[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004b1c[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02004b58[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

s16 gData_02004b80[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC7_PALETTE (5 * 32)
#else
#define SHAMAN_GRAPHIC7_PALETTE (2 * 32)
#endif

u8 gData_02004bfe[] = {
    0, 32, 32, 32, 96, 64, 64, SHAMAN_GRAPHIC7_PALETTE, 128, 0,
};
// clang-format on

// clang-format off
s32 gData_02004c08[] = {
    22, 26, 0, 23, 7, -0x8000,
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 24, 0, 17,
};

s32 gData_02004c50[] = {
    (s32)gData_02004aa4,
    (s32)gData_02004ae0,
    (s32)gData_02004b1c,
};

u32 gData_02004c5c[] = {
    39, 2, 4, 0xb80000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gData_02004cac[] = {
    39, 2, 4, 0x1080000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gData_02004cfc[] = {
    39, 2, 4, 0x980000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xb000, 0, 10, 17,
};

u32 gData_02004d40[] = {
    39, 2, 4, 0x880000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004d84[] = {
    39, 2, 4, 0x680000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004dc8[] = {
    39, 2, 4, 0x580000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004e0c[] = {
    39, 2, 4, 0x780000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gData_02004e5c[] = {
    39, 2, 4, 0x880000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gData_02004eac[] = {
    39, 2, 4, 0x680000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 22, 40, 1, 17,
};

u32 gData_02004f08[] = {
    39, 2, 4, 0x780000, 0, 0x1080000,
    1, 39, 1, 0, 1, 3,
    17,
};

u32 gData_02004f3c[] = {
    39, 2, 4, 0x880000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xd000, 0, 10, 17,
};

u32 gData_02004f80[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gData_02004fc4[] = {
    39, 2, 4, 0x980000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xe000, 0, 10, 17,
};

u32 gData_02005008[] = {
    39, 2, 4, 0xc80000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gData_0200504c[] = {
    39, 2, 4, 0xd80000, 0, 0x2340000,
    1, 4, 0xd80000, 0, 0x1d60000, 1,
    4, 0xa80000, 0, 0x1b20000, 1, 4,
    0xa80000, 0, 0x17c0000, 1, 39, 1,
    22, 40, 1, 17,
};

u32 gData_020050bc[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

s32 gData_020050f8[] = {
    23, 9, 0x2000, 23, 10, 0x4000,
    0, 1, 13, 4, 0, (s32)0x80010000,
    23, 9, -4096, 23, 10, -0x2000,
    0, 1, 13, 8, (s32)0xc0010000, 17,
};

s16 gData_02005158[] = {
    -1,
};

// Scale for each edition.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SHAMAN_GRAPHIC1_SCALE 384
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 384
#define SHAMAN_GRAPHIC4_LARGE_SCALE 512
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 384
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#else
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#endif

s16 gData_0200515a[] = {
    0x4000, -188, 0x800, 0x1000, SHAMAN_GRAPHIC1_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 0x800, 20, 0x7fff, 60, -1,
};

s16 gData_02005186[] = {
    0x1000, SHAMAN_GRAPHIC4_START_SCALE, 1, 0x2000, 0, 1,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x2000, 16, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6, 0x2000, 0, 30,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6, -1,
};
// clang-format on

#include "text/msg_ids.h"

void Func_02000280(void);
void ShamanVillage_StopAtLockedEntrance(void);
void ShamanVillage_StopAtClosedEntrance(void);
void ShamanVillage_ReadTrialRoadSign(void);
void ShamanVillage_TalkToOtherGuard(void);

// clang-format off
u32 gData_02005204[] = {
    0x801000, 0x20000001, 0x10000, 0x1001000, 0x20000006, 0x60010,
    0x3c7fff, 0x1801000, 0x7fff0006, 0x10000006, 0x60110, 0x67fff,
    0x1901000, 0x7fff0006, 0x10000006, 0x60120, 0x67fff, 0x2000,
    0x1000001e, 0x601a0, 0x67fff, 0x1301000, 0x7fff0006, 0x10000006,
    0x601b0, 0x67fff, 0x1401000, 0x7fff0006, 0x10000006, 0x601c0,
    0x67fff, 0xffff,
};

struct SceneEntrance gShamanVillage_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillage_Room5Entrances[] = {
    {15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
    {16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
    {17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gShamanVillage_Room6Entrances[] = {
    {6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(241),
    SCENE_EXIT(1, 247, 31),
    SCENE_EXIT(2, 247, 32),
    SCENE_EXIT(3, 243, 1),
    SCENE_EXIT(10, 247, 50),
    SCENE_EXIT(11, 247, 31),
    SCENE_EXIT(12, 247, 32),
    SCENE_EXITS(243),
    SCENE_EXIT(1, 241, 3),
    SCENE_EXIT(2, 242, 8),
    SCENE_EXIT(3, 242, 11),
    SCENE_EXIT(4, 245, 17),
    SCENE_EXIT(5, 242, 14),
    SCENE_EXIT(6, 242, 9),
    SCENE_EXITS(242),
    SCENE_EXIT(1, 2, 58),
    SCENE_EXIT(2, 245, 15),
    SCENE_EXIT(3, 245, 16),
    SCENE_EXIT(4, 244, 6),
    SCENE_EXIT(5, 244, 8),
    SCENE_EXIT(6, 244, 11),
    SCENE_EXIT(7, 244, 13),
    SCENE_EXIT(8, 243, 2),
    SCENE_EXIT(9, 243, 6),
    SCENE_EXIT(10, 246, 6),
    SCENE_EXIT(11, 243, 3),
    SCENE_EXIT(12, 244, 3),
    SCENE_EXIT(13, 244, 1),
    SCENE_EXIT(14, 243, 5),
    SCENE_EXITS(244),
    SCENE_EXIT(1, 242, 13),
    SCENE_EXIT(2, 244, 4),
    SCENE_EXIT(3, 242, 12),
    SCENE_EXIT(4, 244, 2),
    SCENE_EXIT(5, 244, 9),
    SCENE_EXIT(6, 242, 4),
    SCENE_EXIT(7, 244, 10),
    SCENE_EXIT(8, 242, 5),
    SCENE_EXIT(9, 244, 5),
    SCENE_EXIT(10, 244, 7),
    SCENE_EXIT(11, 242, 6),
    SCENE_EXIT(12, 244, 14),
    SCENE_EXIT(13, 242, 7),
    SCENE_EXIT(14, 244, 12),
    SCENE_EXITS(245),
    SCENE_EXIT(15, 242, 2),
    SCENE_EXIT(16, 242, 3),
    SCENE_EXIT(17, 243, 4),
    SCENE_EXITS(246),
    SCENE_EXIT(6, 242, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gShamanVillage_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room1Placements[] = {
    {305, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {382, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_EAST, 2, 2},
    {41, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(280), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(240), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room3Placements[] = {
    {305, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(264), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(264), FACING_EAST, 2, 0},
    {272, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(184), FACING_EAST, 2, 0},
    {112, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(336), FACING_EAST, 1, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(296), FACING_SOUTH, 0, 0},
    {46, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {246, 0x79, 1, PIXELS(680), 0, PIXELS(248), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020056b0[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(428), 0x3000, 3, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(248), FACING_EAST, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(364), 0, PIXELS(248), FACING_WEST, 1, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(254), 0, PIXELS(434), 0x5000, 1, 0},
    {113, CONDITION_ALWAYS, 3, PIXELS(434), 0, PIXELS(446), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(200), 0x5000, 1, 0},
    {138, CONDITION_ALWAYS, 3, PIXELS(412), 0, PIXELS(230), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {299, CONDITION_ALWAYS, 7, PIXELS(40), 0, PIXELS(288), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room4Placements[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(90), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(110), 0, PIXELS(172), FACING_SOUTH, 1, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(126), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 3, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {135, CONDITION_ALWAYS, 3, PIXELS(140), 0, PIXELS(388), 0x3000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(694), FACING_WEST, 0, 0},
    {128, CONDITION_ALWAYS, 3, PIXELS(144), 0, PIXELS(658), FACING_NORTH, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(164), 0, PIXELS(432), 0x5000, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(460), FACING_EAST, 1, 0},
    {211, CONDITION_ALWAYS, 1, PIXELS(114), 0, PIXELS(364), 0xb000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room5Placements[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(678), 0xb000, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(508), 0, PIXELS(656), 0x3000, 0, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(542), 0, PIXELS(628), 0x3000, 0, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(550), 0, PIXELS(640), 0xb000, 0, 0},
    {111, CONDITION_ALWAYS, 1, PIXELS(752), 0, PIXELS(648), 0x5000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(724), 0, PIXELS(628), 0x3000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gShamanVillage_Room6Placements[] = {
    {201, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(466), 0x3000, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(746), 0, PIXELS(516), 0xb000, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(700), 0, PIXELS(524), 0xd000, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gShamanVillage_Triggers[] = {
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 526, 0x94c, EVENT(ShamanVillage_ShebaClearsSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 1038, 0x94c, 0},
    {TRIGGER_STEP, 10, 0x9e0, EVENT(ShamanVillage_ExplainTrialRules)},
    {TRIGGER_STEP, 10, 0x201, EVENT(ShamanVillage_ChallengeMoapa)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(ShamanVillage_OfferToGiveUp)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeLeftPath)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TakeRightPath)},
    {TRIGGER_TALK, 15, 0x94c, MSGNO(MsgShamanYegelosSandChallenge)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanChooseEitherRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x94c, MSGNO(MsgShamanRodWasStolen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaCannotLose)},
    {TRIGGER_TALK, 17, 0x94c, MSGNO(MsgShamanTrialRoadAncientBattle)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x94c, MSGNO(MsgShamanNeedContigoOrShamanPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaGreatestWarrior)},
    {TRIGGER_TALK, 16, 0x94c, MSGNO(MsgShamanAncientHeroesVanishedSand)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToOtherGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x94c, MSGNO(MsgShamanSandChallengeWillFail)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShamanJadeNeedsYegelosHeir)},
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0xce01, 4, 0x194d, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x94e, EVENT(ShamanVillage_DropPillar)},
    {0xc403, 22, 0x94d, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS, EVENT(ShamanVillage_ReadTrialRoadSign)},
    {TRIGGER_STEP, 11, 0x203, EVENT(ShamanVillage_GreetFortuneSeeker)},
    {0xc400, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_TellFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0xa21, MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShamanCloudyFortuneAfterTrial)},
    {TRIGGER_TALK, 15, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x9fe, MSGNO(MsgShamanAncestorsDistrustOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShamanContigoHeroVictorySurprise)},
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xce01, 2, 0x194d, 2},
    {0xce01, 3, 0x194d, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02003c58)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02003c74)},
    {0xc602, 6, 0x194d, EVENT(Func_02003c90)},
    {0xc602, 7, 0x194d, EVENT(Func_02003cac)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xce01, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Func_02003cc8)},
    {0xc602, 13, 0x94f, EVENT(ShamanVillage_MeetMoapa)},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(Func_02003ce4)},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 19, 0x204, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 21, 0x205, EVENT(ShamanVillage_GrowVine)},
    {0xc403, 20, 0x94d, EVENT(ShamanVillage_StopAtLockedEntrance)},
    {0xc403, 22, 0x94d, EVENT(ShamanVillage_StopAtClosedEntrance)},
    {0xc403, 21, 0x94f, EVENT(ShamanVillage_MeetMoapa)},
    {TRIGGER_TALK, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaImpressedByChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe, MSGNO(MsgShamanWaitForOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaPraisesOpponents)},
    {TRIGGER_TALK, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9fe, MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShamanDoubtMoapaDefeat)},
    {TRIGGER_TALK, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9fe, MSGNO(MsgShamanWhoInvitedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShamanHopeForStrongerShamanChild)},
    {TRIGGER_TALK, 11, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9fe, MSGNO(MsgShamanRefuseToTalkToOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
    {TRIGGER_TALK, 12, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaPromiseFulfilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x9fe, MSGNO(MsgShamanTrustContigoFriends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShamanRemainFriendsWithContigo)},
    {TRIGGER_TALK, 13, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(ShamanVillage_AskAboutOtherSide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9fe, MSGNO(MsgShamanDistrustFriendlyOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShamanWishToBreakSandWalls)},
    {TRIGGER_TALK, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShamanChosenFewHavePower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9fe, MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(ShamanVillage_TellOutsidersToLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShamanOutsidersAreNotContigo)},
    {TRIGGER_TALK, 17, 0x9fe, EVENT(ShamanVillage_WarnOfMoapasPower)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgShamanGuardPromisesRematchVictory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x9fe, MSGNO(MsgShamanContigoTempleTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShamanClimbingTrialRoadIsHard)},
    {TRIGGER_TALK, 16, 0x9fe, EVENT(ShamanVillage_SendToMoapa)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgShamanVillagersBlameGuardDefeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9fe, MSGNO(MsgShamanMoapaSparedOutsiders)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShamanGuardBlamesMountainClimb)},
    {0x4114, 15, 0x9e1, EVENT(ShamanVillage_ShowShamansRod)},
    {0x1ff14, 15, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToMoapa)},
    {0x1ff14, 16, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToGuard)},
    {0x1ff14, 17, CONDITION_ALWAYS, EVENT(ShamanVillage_ShowItemToOtherGuard)},
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_TALK, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaWantsToKnowNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe, MSGNO(MsgShamanMoapaLeftDoorOpen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShamanShameNotKnowingVictors)},
    {TRIGGER_TALK, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanFormalBattleNeedsNames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x9fe, MSGNO(MsgShamanFatherNeverLoses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShamanFatherLostToRudeOutsiders)},
    {TRIGGER_TALK, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanNamesDoNotMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x9fe, MSGNO(MsgShamanFirstForeignVisitors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x9ed, MSGNO(MsgShamanVictorsDeserveRespect)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9fe, MSGNO(MsgShamanPayingStrangersMayStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
    {TRIGGER_TALK, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShamanForgiveUnkindVillagers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9fe, MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgShamanVillagersWorryTooMuch)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShamanQuietWhileLockedInside)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgShamanMissedTrialRoadChallenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShamanHidingLawPreventedWatching)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgShamanEnjoyGettingOutside)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShamanChildrenIgnoreTradition)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgShamanPlanPicnicMeal)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgShamanEagerForDinner)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgShamanSaltAndPepperOnly)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room5Triggers[] = {
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanAncientHesperiaAttekaWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShamanNewContigoHeroStronger)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShamanDoubtShamanSuperiority)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGERS_END},
};

struct Trigger gShamanVillage_Room6Triggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(ShamanVillage_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9fe, MSGNO(MsgShamanVisitorsMustFindMoapa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShamanFearRenewedContigoWar)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanThreatenContigoRematch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanPrayForShamanWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShamanPrayForContigoConqueror)},
    {TRIGGERS_END},
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
