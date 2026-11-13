/// @file
/// Scene code of overlay 0x679, in ROM order.

// ---- games/tla/src/scene/apojii_islands.c ----

#include "../shared/routines.h"
#include "../shared/tables.h"
#include "../apojii_islands.h"
#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"
#include "event_inline.h"
#include "event.h"
#include "event_work.h"
#include "field.h"
#include "field_event.h"
#include "flag_call.h"
#include "ground_mask.h"
#include "module/field/field.h"
#include "module/scene/apojii_islands.h"
#include "module/scene/jupiter_lighthouse_aerie.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system.h"
#include "task.h"

void Func_0200033c(void) { Djinn_JoinParty(10, 3, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov679Room1)
    return gData_02002218;
  if (scene == (s32)&SceneId_Ov679Room3)
    return gData_02002320;
  if (scene == (s32)&SceneId_Ov679Room2)
    return gData_02002458;
  return gData_02002200;
}

void Func_020003b0(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020003cc(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

/* VIEW(apojii_islands): variable gData_020024c4 as extern s32 gData_020024c4;
 * the definition has s32 gData_020024c4[] = { - 1, }; tried as the definition's
 * extern s32 gData_020024c4[], it does not compile in tla-ja:
 * games/tla/src/scene/apojii_islands.c:76: `gData_020024c4' has an incomplete
 * type. */
extern s32 gData_020024c4__apojii_islands __asm__("gData_020024c4");
extern u16 Data_02002958[], Data_02002978[], gData_02002998[], gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 phase, k, offset;
  u16 *firstCopy, *secondCopy;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  if (*(s8 *)(work + 0xca4) != 0)
    return;
  if (gData_020024c4__apojii_islands == -1) {
    Call3((void (*)())Iwram_CopyWordsEntry, Data_02002958, (void *)0x05000060,
          32);
    Iwram_CopyWordsEntry(Data_02002978, Data_02002958, 32);
    Iwram_CopyWordsEntry(gData_02002998, (void *)0x05000080, 32);
    Iwram_CopyWordsEntry(gData_020029b8, gData_02002998, 32);
    gData_020024c4__apojii_islands = 0;
  }
  phase = gData_020024c4__apojii_islands / 8 % 4;
  k = 0;
  firstCopy = Data_02002978;
  secondCopy = gData_020029b8;
  for (; k <= 3; k++) {
    offset = (phase + k) % 4 * 2 + 24;
    *(u16 *)((u8 *)firstCopy + offset) = Data_02002958[12 + k];
    *(u16 *)((u8 *)secondCopy + offset) = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)firstCopy, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000080, (u32)secondCopy, 0x84000008);
  gData_020024c4__apojii_islands++;
}

/* VIEW(apojii_islands): variable gData_020024c8 as extern s32 gData_020024c8;
 * the definition has s32 gData_020024c8[] = { - 1, }; tried as the definition's
 * extern s32 gData_020024c8[], it does not compile in tla-ja:
 * games/tla/src/scene/apojii_islands.c:110: `gData_020024c8' has an incomplete
 * type. */
extern s32 gData_020024c8__apojii_islands __asm__("gData_020024c8");
extern u16 Data_020029d8[], Data_020029f8[], Data_02002a18[], Data_02002a38[];

void Func_02000540(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c8__apojii_islands == -1) {
    Call3((void (*)())Iwram_CopyWordsEntry, (s32)Data_020029d8, 0x05000060, 32);
    Iwram_CopyWordsEntry(Data_020029f8, Data_020029d8, 32);
    Iwram_CopyWordsEntry(Data_02002a18, (void *)0x050001a0, 32);
    Iwram_CopyWordsEntry(Data_02002a38, Data_02002a18, 32);
    gData_020024c8__apojii_islands = busy;
  }
  phase = gData_020024c8__apojii_islands / 8 % 4;
  for (k = 0; k <= 3; k++) {
    /* FAKEMATCH(dead): the phase and the first colour in a once-only block,
     * whose end holds the order around it; as plain statements, 6 of 152
     * instructions differ in tla-en. */
    do {
      j = (phase + k) % 4;
      Data_020029f8[12 + j] = Data_020029d8[12 + k];
    } while (0);
    Data_02002a38[12 + j] = Data_02002a18[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)Data_020029f8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)Data_02002a38, 0x84000008);
  gData_020024c8__apojii_islands++;
}

void Func_02000694(void) {
  Func_02001bf4(Func_02000540);
  System_Wait(3);
}

void Func_020006a8(void) {
  Func_02001bfc(Func_02000540);
  System_Wait(3);
}

extern u16 gData_02002a78[16];

s32 __modsi3(s32 a, s32 b);

void Func_020006bc(void) {
  s32 base;
  s32 i;

  if (gData_020024cc[0] == -1) {
    Iwram_CopyWordsEntry(gData_02002a58, (void *)0x05000080, 32);
    gData_020024cc[0] = 0;
  }
  base = (gData_020024cc[0] / 2) % 15;
  for (i = 0; i < 15; i++)
    gData_02002a78[(base + i) % 15 + 1] = gData_02002a58[i + 1];
  IO_WRITE_QUEUE_PUSH(0x050003c0, (u32)gData_02002a78, 0x84000008);
  gData_020024cc[0]++;
}

void Func_02000770(struct Actor *object) {
  Actor_SetSpriteColorMode(object, 0);
  object->update = 0;
}

DEFINE_FUNC_02000780(Func_02000780, 7)

// ---- games/tla/src/scene/apojii_islands/s07a4.c ----
#include "scene_tables.h"
#include "actor.h"
#include "task.h"

/* VIEW(s07a4): routine Random16Far as u16 Random16Far(void); system.h has u32
 * Random16Far(void); tried as the rest of the object declares it, Func_020007a4
 * moves: 1 of 188 instructions in tla-ja. */
extern u16 Random16Far__s07a4(void) __asm__("Random16Far");
/* VIEW(s07a4): routine BattleEffect_ApplyColorToTargetBuffer as s32
 * BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b); event.h has void
 * BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode); tried as the
 * definition's void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode),
 * which the object already declares, Func_020007a4 moves: 1 of 188 instructions
 * in tla-ja. */
extern s32 BattleEffect_ApplyColorToTargetBuffer__s07a4(s32 a, s32 b) __asm__(
    "BattleEffect_ApplyColorToTargetBuffer");

static inline void ApojiiIslands_Flash(s32 strength, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer__s07a4(strength, mode);
}

static inline void ApojiiIslands_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

/// An Apojii Islands scene inside an event: the actor takes Func_02000780 as
/// its update, cues 78 and 21 play, a sparkle setup (Func_02001ad8) runs and
/// four flashes of random strength follow; then the screen blends in over six
/// steps while the palette cycle gives way to Func_020006bc and the cell
/// markers are rebuilt (Func_020017f8). Flags 0x95f and 0x95e are set and the
/// camera pans through three points before cue 77.
void Func_020007a4(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = Func_02000780;
  Sound_Play(78);
  wait(30);
  Sound_Play(21);
  Func_02001ad8();
  Func_02001ba0(17);
  Func_02001dec(170);
  i = 3;
  /* FAKEMATCH(wrapper): both flashes forwarded through ApojiiIslands_Flash,
   * which keeps each call's own constants; as direct calls, 10 of 188
   * instructions differ in tla-en. */
  /* FAKEMATCH(signature): BattleEffect_ApplyColorToTargetBuffer declared
   * returning s32, though its target returns nothing; as void, 2 of 188
   * instructions differ in tla-en. */
flash:
  ApojiiIslands_Flash(Random16Far__s07a4() & 0x7fff, 0);
  PaletteFade_StartInterpolation(15);
  wait(10);
  ApojiiIslands_Flash(0x10000, 0);
  PaletteFade_StartInterpolation(10);
  i--;
  wait(10);
  if (i >= 0)
    goto flash;
  ObjectTable_Snapshot();
  IO_WRITE_QUEUE_PUSH(0x04000050, 0xf00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000, 0x20000);
  wait(1);
  Task_RemoveFar(Func_02000540);
  Task_Add(Func_020006bc, TASK_PASS_VBLANK);
  Func_020017bc();
  Func_020017f8(33, 0, 0);
  i = 1;
blend:
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
  wait(4);
  i++;
  if (i <= 6)
    goto blend;
  GameFlag_SetBit(0x95f);
  GameFlag_SetBit(0x95e);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  /* FAKEMATCH(wrapper): the camera moves forwarded through
   * ApojiiIslands_CamMove, so each builds its own -1; as direct calls, 25 of
   * 188 instructions differ in tla-en. */
  ApojiiIslands_CamMove(PIXELS(360), -1, PIXELS(328), 1);
  Actor_ShowEmoteById(16, 256, 20);
  Func_02001d8c();
  ApojiiIslands_CamMove(PIXELS(440), -1, PIXELS(376), 1);
  Func_02001d8c();
  ApojiiIslands_CamMove(PIXELS(472), -1, PIXELS(408), 1);
  Import_EventSetExitRequest(77);
}

// ---- games/tla/src/scene/apojii_islands/events_d.c ----
// Scene code of overlay 0x679, in ROM order, from events_d's offset up to the
// next object the overlay links.

#include "map.h"
#include "event_work.h"

void Func_02000974(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(17)->update = (void (*)(void))Func_02000770;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(16, FACING_EAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_WEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  FieldActor_ShowEmote(16, 0x102, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(16, 2, 25);
  Event_ShowMessage(16, 0);
  Actor_RunRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Event_End();
  Event_ClearFlag(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

DEFINE_FUNC_02000A68(Func_02000a68, 68, 38, 21, 4, 0xf9b, 65, 376, 376, 23, 23)

DEFINE_FUNC_02000A68(Func_02000af8, 68, 38, 21, 4, 0xf9a, 64, 600, 408, 37, 25)

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

DEFINE_FUNC_02000A68(Func_02000ba8, 67, 11, 6, 8, 0xf9e, 64, 200, 152, 12, 9)

void Func_02000c38(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Event_PlaySound(123);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn_(gMainRoutineWork.currentOwner, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

s32 Func_02000cb4(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    Event_ShowMessage(9, 0);
    Actor_ArmCallback_(9, FACING_SOUTHEAST, 0);
  }
  Event_End();
  return 0;
}

s32 Func_02000d20(void) {
  u16 facing;

  facing = (s16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
                 0xc000);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    Event_ShowMessage(9, 0);
  }
  Event_End();
  return 0;
}

void Func_02000d74(void) {
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 1, 5);
}

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov679Room1)
    return gData_020024d0;
  if (scene == (s32)&SceneId_Ov679Room3)
    return gData_02002698;
  if (scene == (s32)&SceneId_Ov679Room2)
    return gData_0200280c;
  return gData_020024b8;
}

void Func_02000de0(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e24(8, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgWowYouLook);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Func_02000e40(s32 actor) {
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)&MsgYouGuysAren;
  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Func_02000e98(s32 actor) {
  if ((u16)((Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_02001e1c(23, actor);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgItDBe);
  Event_ShowMessage(actor, 0);
  Event_End();
}

void Func_02000ef8(void) {
  u32 value;

  if (REG_VCOUNT < gData_02002a98) {
    value = gData_02002a9c;
    REG_BG1HOFS = value;
    REG_BLDCNT = 0;
  } else {
    value = gData_02002a9e;
    /* FAKEMATCH(dead): the once-only block orders this statement; as a plain
     * statement, 9 of 35 instructions differ in tla-en. */
    do {
      REG_BG1HOFS = value;
    } while (0);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

DEFINE_FUNC_02000E90(Func_02000f58, gData_02002a98, gData_02002a9c,
                     gData_02002a9e)

DEFINE_FUNC_02000ED0(Func_02000f98, Func_02000ef8, Func_02000f58)

/// The overlay's entry: sets the map mode and screen effect 0x202, then starts
/// each room's own setup.
s32 Scene_Initialize(void) {
  s32 *effect;
  s16 scene;
  struct MainRoutineWork *party;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  *effect = 0x202;
  party = &gMainRoutineWork;
  scene = party->scene;
  if (scene == (s32)&SceneId_Ov679Room2) {
    Func_02001054();
  } else if (scene == (s32)&SceneId_Ov679Room1) {
    FieldTask_AddFrame(Func_02000540, 0x480);
    if (Event_GetFlag(0x95e))
      Func_02000974();
  } else if (scene == (s32)&SceneId_Ov679Room3) {
    *effect = 0x100;
    if (party->entrance == 6)
      Func_020010b4();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001054(void) {
  Task_AddFrame(Func_020003e8, TASK_PASS_VBLANK);
  Func_02001dfc();
  Func_02001e04(0, 0x200, 8, 9);
  Func_02001dec(170);
  Func_02000f98();
  if (gMainRoutineWork.entrance == 3)
    Func_02001500();
  if (gMainRoutineWork.entrance == 4)
    Func_02001500();
  if (gMainRoutineWork.entrance == 5)
    Func_02001500();
}

void Func_020010b4(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_ClearFlag(0x12f);
}

void Func_020010d0(void) { Func_02001d9c(); }

#include "field_sprite.h"

void Func_020010d8(struct Actor *object) {
  struct Actor *child;
  struct FieldSprite *parent;
  struct FieldSprite *sprite;

  child = (FieldActor_Create(14, object->x, object->y + 0x40000,
                             object->z + 0x40000));
  parent = object->sprite;
  if (child != NULL) {
    sprite = child->sprite;
    child->terrainHeight = object->terrainHeight;
    Actor_StartScript(child, gData_02001ee8);
    child->motionFlags = 0;
    if (sprite != NULL) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parent->priority;
    }
  }
}

// ---- games/tla/src/scene/apojii_islands/events_h.c ----
// Scene code of overlay 0x679, in ROM order, from events_h's offset up to the
// next object the overlay links.

DEFINE_FUNC_02001140(Func_02001140, gData_02001ef4)

void Func_020011a8(void) {
  struct EventWork1cc *work = Ram_HeapSlots->eventWork;
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *anchor = work->object;
  struct Actor *leader = Actor_Get(owner);
  s32 position[3];
  s32 result;

  if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1 ||
      leader->terrainHeight != 0) {
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z + 0x20000;
    result = Func_02001c9c(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_020010d8(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner, 0x102);
      FieldActor_SetMoveTarget(leader, leader->x, leader->y,
                               leader->z + 0x80000);
      Func_02001c6c(leader, 49);
      FieldActor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y != leader->terrainHeight);
      Func_020010d8(leader);
      Func_02001c6c(leader, 49);
      System_Wait(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = Func_02001c9c(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = Func_02001c9c(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = Func_02001c9c(leader, position);
          if (result <= 0) {
            anchor->z += 0x20000;
            leader->z += 0x20000;
            leader->facing = 0x4000;
          }
        }
      }
    }
  }
}

DEFINE_FUNC_02001308(Func_02001308, Func_02001c6c)

DEFINE_FUNC_02001338(Func_02001338, Func_02001c6c, Func_020010d8)

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
DEFINE_FUNC_020013D8(Func_020013d8)

DEFINE_FUNC_02001464(Func_02001464, Func_020013d8)

DEFINE_FUNC_02001500(Func_02001500, Func_020013d8)

#include "graphics/vram_tab.h"
#include "system/dma.h"

#define COPY_PICTURE(from, to)                                                 \
  Dma_Set((void *)(from), (void *)(offset + (to)), DMA_COPY_WORDS(16), REG_DMA3)

void Func_020015f8(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // viewZ read through a pointer to viewX, its neighbour; as map->viewZ, 30
  // of 189 instructions differ in tla-en.
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  // DEAD: viewY is subtracted from both height and z and cancels in dy; the
  // source keeps its load and a store to the stack that nothing reads.
  s32 viewY = map->origin[1];
  struct CellMarkers *table = &gData_02002aa0;
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[table->block];
  u32 tile = entry->offset >> 5;
  u32 offset = entry->offset;
  struct CellMarker *record;
  s32 i;

  COPY_PICTURE(0x0600f000, 0x06010000);
  COPY_PICTURE(0x0600f400, 0x06010040);
  COPY_PICTURE(0x0600f040, 0x06010080);
  COPY_PICTURE(0x0600f440, 0x060100c0);
  COPY_PICTURE(0x0600f080, 0x06010100);
  COPY_PICTURE(0x0600f480, 0x06010140);
  COPY_PICTURE(0x0600f080, 0x06010180);
  COPY_PICTURE(0x0600f480, 0x060101c0);
  record = table->markers;
  for (i = 127; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    left &= 0x1ff;
    /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapWork after left's
     * mask; as nothing, 82 of 189 instructions differ in tla-en. */
    Ram_HeapSlots->mapWork += 0;
    top &= 255;
    attr = record->oam;
    kind = record->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Oam_AddSpriteAtDepth(record->oam, 0);
  }
}

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  Iwram_ClearWordsEntry(&gData_02002aa0, 0x1004);
  gData_02002aa0.block = VramCache_FindFreeEntry();
  Vram_Load(gData_02002aa0.block, 0x200, 0);
  FieldTask_AddFrame(Func_020015f8, 0x480);
}

#include "system.h"
#include "heap_release.h"
#include "io_reg.h"
#include "resource.h"

void Func_020017f8(u32 base, s32 a, s32 b) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *second = &map->layers[1];
  struct MapLayer *layer = &map->layers[0];
  struct MapCell *cells = second->cells;
  s32 left = *(s32 *)&layer->unk08[0] + map->minX;
  s32 top = *(s32 *)&layer->unk08[4] + map->minZ;
  struct CellMarkers *markers = &gData_02002aa0;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  struct CellMarker *entry = &markers->markers[markers->count];
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s32 origin = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = a << 8;
  cells += origin;
  z = 0;
  if (z < height) {
  outer:
    cell = cells + ((u16)z << 7);
    x = 0;
    if (x < width) {
    inner:
      value = cell->collisionCode;
      if (value != 0 && value >= base && value < base + 4) {
        entry->x = ((u16)x << 20) + left + 0x80000;
        entry->z = ((u16)z << 20) + top + 0x80000;
        entry->rest = b << 19;
        entry->y = a << 19;
        entry->kind = value - base;
        entry->state = 1;
        entry++;
        markers->count++;
        // Layer 0's row base in its own local (written inline, 10 of 159
        // differ).
        {
          u8 *terrain = map->layers[0].terrainIds + origin;

          terrain[(u16)x + ((u16)z << 7)] = 120;
        }
      }
      // The step spelled as a halfword sum (as x++, 40 of 159 instructions
      // differ in tla-en).
      x = ((x << 16) + 0x10000) >> 16;
      cell++;
      if ((u16)x < width)
        goto inner;
    }
    // The same halfword-sum spelling (as z++, 16 of 159 differ).
    z = ((z << 16) + 0x10000) >> 16;
    if ((u16)z < height)
      goto outer;
  }
}

extern s16 gData_02001f18[];

DEFINE_FUNC_02001940(Func_02001940, gData_02001f18)

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and Func_02001940 scheduled at priority 0x480 to move
/// them; the overlay's copy of FieldEffect_InitializeSparkles.
void Func_02001ad8(void) {
  struct DustWork *work = HeapSlot_AllocEwram(116, sizeof(struct DustWork));
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  /* FAKEMATCH(temporary): one local is the fill word, the end word and each
   * particle's attribute cursor, so it lives on the stack and every step of
   * the cursor is stored there, as the native's; with a separate cursor, 55 of
   * 81 instructions differ in tla-en. */
  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 517, REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Func_02001c24(gField_Tiles679, buf);
  work->vramEntry = VramCache_FindFreeEntry();
  work->tileBase = Vram_Load(work->vramEntry, 0x300, buf);
  HeapSlot_Release(56);
  work->field808 = size;
  i = 0;
loop:
  size = (u32)p;
  *(u32 *)size = 0;
  size += 4;
  *(u32 *)size = 0x40000400;
  size += 4;
  *(u32 *)size = 0xb400;
  p->posX = 0;
  p->posZ = 0;
  p->posY = Map_GetTerrainHeight(0, 0, 0) << 16;
  p->timer = (i & 15) + 1;
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_AddFrame(Func_02001940, TASK_PASS_VBLANK);
}

void Func_02001ba0(s32 id) {
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_Get(id);
  s32 *slot = (s32 *)(work + 0x808);

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}

#include "text/msg_ids.h"
#include "version.h"

#if !TLA_PATCH_1
// The Japanese table holds 131 where later editions hold MsgFieldOff's number,
// which the Japanese text numbers alike: a changed value, from patch 1.
// XXX: is it a message at all; what reads gData_02002044 would settle it.
#define UNK_02002044_VALUE 131
#else
#define UNK_02002044_VALUE MSGNO(MsgFieldOff)
#endif

void Func_0200033c(void);
void Func_02000694(void);
void Func_02000a48(void);
void Func_02000a68(void);
void Func_02000af8(void);

// clang-format off
u32 gData_02001e34[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001e70[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001eac[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001ee8[] = {
    0, 20, 38,
};

u32 gData_02001ef4[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};

DEFINE_G_DATA_02001F18(gData_02001f18)

s32 gData_02002038[] = {
    (s32)gData_02001e34,
    (s32)gData_02001e70,
    (s32)gData_02001eac,
};

s32 gData_02002044[] = {
    22,
    30,
    UNK_02002044_VALUE,
    22,
    16,
    0x3333,
    22,
    15,
    0x9999,
    (s32)0x80010000,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1280000,
    0,
    0x680000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1180000,
    0,
    0x880000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    13,
    0xffff,
    (s32)0xc0010000,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(154),
    SCENE_EXIT_IF(1, 2, 36, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 155, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 155, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 156, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 156, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 156, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 156, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 156, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 58, 5, CONDITION_ALWAYS),
    SCENE_EXIT(77, 2, 78),
    SCENE_EXITS(156),
    SCENE_EXIT_IF(1, 154, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 154, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 154, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 154, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 154, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 135, 2, CONDITION_ALWAYS),
    SCENE_EXITS(155),
    SCENE_EXIT_IF(1, 154, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 154, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 157, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 157, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 157, 5, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

u32 gData_02002200[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002218[] = {
    0xffff0073, 1, 0x380000, 0, 0xa80000, 0x1a000,
    0xffff0074, 1, 0x1c80000, 0, 0x580000, 0x3e000,
    0xffff0072, 1, 0x2380000, 0, 0x1380000, 0x1e000,
    0xffff0080, 2, 0x1f80000, 0, 0xc80000, 0xc000,
    0xffff0070, 1, 0x2680000, 0, 0x1080000, 0x12000,
    0xffff0071, 2, 0xb80000, 0, 0x980000, 0x1e000,
    0xffff0088, 3, 0xd80000, 0, 0x1780000, 0x4000,
    0xffff008a, 3, 0x680000, 0, 0x1580000, 0x2000,
    0xffff0073, 1, 0x1580000, 0, 0x1180000, 0x1c000,
    0xffff017f, 1, 0x1600000, 0, 0xe80000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02002320[] = {
    (s32)0xffff0074,
    1,
    0x980000,
    0,
    0x980000,
    0x10000,
    (s32)0xffff0088,
    1,
    0x780000,
    0,
    0x680000,
    0x22000,
    (s32)0xffff0072,
    1,
    0x1080000,
    0,
    0x680000,
    0x12000,
    (s32)0xffff0080,
    (s32)gData_02002044,
    0x1180000,
    0,
    0x880000,
    0x10000,
    (s32)0xffff0088,
    1,
    0x1980000,
    0,
    0x680000,
    0x14000,
    (s32)0xffff008a,
    2,
    0x1580000,
    0,
    0x780000,
    0x1e000,
    (s32)0xffff0070,
    3,
    0x2500000,
    0,
    0x780000,
    0x12000,
    (s32)0xffff0071,
    1,
    0x2280000,
    0,
    0x880000,
    0x14000,
    (s32)0xffff006e,
    1,
    0x1a00000,
    0,
    0x1680000,
    0x14000,
    (s32)0xffff0086,
    1,
    0x900000,
    0,
    0x1680000,
    0x14000,
    (s32)0xffff0087,
    1,
    0xd80000,
    0,
    0x1780000,
    0x16000,
    (s32)0xffff0070,
    3,
    0x800000,
    0,
    0x26c0000,
    0x4000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02002458[] = {
    0xffff0197, 1, 0x1080000, 0, 0x1180000, 0x2c000,
    0xffff0197, 1, 0xd80000, 0, 0x1180000, 0x2c000,
    0x7700f6, 1, 0x1780000, 0, 0xc80000, 0x8000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020024b8[] = {
    -1,
    0,
    0,
};

s32 gData_020024c4[] = {
    -1,
};

s32 gData_020024c8[] = {
    -1,
};

s32 gData_020024cc[] = {
    -1,
};

s32 gData_020024d0[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    2,
    (s32)0xffff0004,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0005,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0006,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0007,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0008,
    (s32)Func_02000c38,
    1,
    (s32)0xffff0009,
    9,
    0xc602,
    (s32)0xffff0021,
    (s32)Func_020010d0,
    0xc602,
    (s32)0xffff0015,
    (s32)Func_020010d0,
    0,
    (s32)0xffff0008,
    MSGNO(MsgThisIsApojii),
    0,
    (s32)0xffff0009,
    MSGNO(MsgWhateverYouDo),
    0,
    (s32)0xffff000a,
    MSGNO(MsgTheHugeWaterfall),
    0,
    (s32)0xffff000b,
    MSGNO(MsgOnAquaIsland),
    0,
    (s32)0xffff000c,
    MSGNO(MsgHaveYouAny),
    0,
    (s32)0xffff000d,
    MSGNO(MsgIDonTKnowWhat),
    0,
    (s32)0xffff000e,
    MSGNO(MsgWhenTheyCome),
    0,
    (s32)0xffff000f,
    MSGNO(MsgDoYouThink),
    0,
    (s32)0xffff0010,
    MSGNO(MsgIfThereS),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgLookingOverGaia),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgTheCurrentsNear),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgAllOfThe),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgWhyWouldAnyone),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgWellAtLeast),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgApojiiHasSo),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgAquaIslandIs),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgIWonderWhere),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgAquaRockMust),
    0x2115,
    0x95f0011,
    (s32)Func_020007a4,
    0x8b85,
    -0x10000,
    (s32)Func_020003b0,
    0x40008b85,
    -0x10000,
    (s32)Func_020003cc,
    0x50009085,
    -0x10000,
    (s32)Func_02000694,
    0x40009085,
    -0x10000,
    (s32)Func_020006a8,
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_02000a48,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_02000a68,
    0x50008905,
    (s32)0xffff0017,
    (s32)Func_02000af8,
    -1,
    0,
    0,
};

s32 gData_02002698[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0,
    (s32)0xffff0008,
    MSGNO(MsgThereSThis),
    0,
    (s32)0xffff0009,
    (s32)Func_02000cb4,
    0,
    (s32)0xffff000a,
    MSGNO(MsgIAbsolutelyRefuse),
    0,
    (s32)0xffff000b,
    MSGNO(MsgTheCurrentsAround),
    0,
    (s32)0xffff000c,
    MSGNO(MsgIMNotAllowed),
    0,
    (s32)0xffff000d,
    MSGNO(MsgHeyMisterDid),
    0,
    (s32)0xffff000e,
    MSGNO(MsgThatOldAqua),
    0,
    (s32)0xffff000f,
    MSGNO(MsgHeSToo),
    0,
    (s32)0xffff0010,
    (s32)Func_02000e98,
    0,
    (s32)0xffff0011,
    (s32)Func_02000de0,
    0,
    (s32)0xffff0012,
    (s32)Func_02000e40,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgIJustWish),
    0x8d15,
    (s32)0xffff0009,
    (s32)Func_02000d20,
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgISinkLike),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgPoorBoyHis),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgItSStupid),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgWowHeSwam),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgYouUnlockThe),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgHeAlwaysWanted),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgNoOneShips),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgIMSure),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgThoseGuysOn),
    0,
    (s32)0xffff0013,
    MSGNO(MsgTheTemperatureOf),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgLongAgoPoseidon),
    -1,
    0,
    0,
};

s32 gData_0200280c[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0003,
    (s32)Func_02001464,
    2,
    (s32)0xffff0004,
    (s32)Func_02001464,
    2,
    (s32)0xffff0005,
    (s32)Func_02001464,
    0x4602,
    (s32)0xffff0028,
    (s32)Func_02000d74,
    0,
    (s32)0xffff000a,
    (s32)Func_0200033c,
    0x8515,
    0x2000008,
    0,
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_02000b88,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_02000ba8,
    -1,
    0,
    0,
};
#undef COPY_PICTURE
#undef UNK_02002044_VALUE
