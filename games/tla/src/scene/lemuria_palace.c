/// @file
/// Lemuria's palace, where King Hydros and his council receive the party and
/// show it two maps, the ancient world and the one Lunpa had.

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "module/game/game.h"
#include "system/task.h"
#include "call.h"
#include "system/io_write.h"
#include "macros.h"
#include "text/msg_ids.h"

extern s32 gLemuriaPalace_Doors[];
extern s32 gLemuriaPalace_InnerDoor[];

void LemuriaPalace_DrawMaps(void);

void Field_StartColorTransition();

extern s16 gLemuriaPalace_DoorOpening[];

/// The sprite each of the two maps is drawn as.
extern s32 gLemuriaPalace_MapTime;
extern u8 gLemuriaPalace_MapFadeLevels[];
extern s32 gLemuriaPalace_FirstMapShown;
extern s32 gLemuriaPalace_SecondMapShown;
extern struct EffectSprite gLemuriaPalace_FirstMapSprite;
extern struct EffectSprite gLemuriaPalace_SecondMapSprite;
extern const u8 gLemuriaPalace_FirstMapTiles[];
extern const u8 gLemuriaPalace_SecondMapTiles[];
extern s32 gLemuriaPalace_SecondMapVram;
extern s32 gLemuriaPalace_FirstMapTile;
extern s32 gLemuriaPalace_SecondMapTile;

void Event_Start();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// A palace door opens with its cell animation and the leader walks through it.
void LemuriaPalace_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                                    s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void LemuriaPalace_EnterDoor(s32 kind) {
  const u8 *data = gLemuriaPalace_Doors;

  if (kind == 3)
    data += 8;
  if (kind == 16)
    data = gLemuriaPalace_InnerDoor;
  LemuriaPalace_OpenDoorAndEnter(data, kind, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern s16 gLemuriaPalace_GlowColors[];

static inline u32 Frame_Mod6(u32 frame) {
  /* FAKEMATCH(wrapper): the inline keeps gFrameCount's address out of a
   * callee-saved register. */
  return frame % 6;
}

/// Every sixth frame rotates colours 1-14 of background palette 12 by one
/// place; every frame sets its colour 15 from a twelve-step glow.
void LemuriaPalace_CycleGlowColors(void) {
  /* FAKEMATCH(pin): the colour stays in r1. */
  register s32 color asm("r1");

  if (Frame_Mod6(gFrameCount) == 0) {
    volatile u16 *palette;
    /* FAKEMATCH(pin): the saved colour takes r1 and leaves r0 to the index. */
    register s32 saved asm("r1");
    s32 index;

    /* FAKEMATCH(pin): the read stays in r3. */
    register s32 raw asm("r3") = *(volatile u16 *)&BG_PLTT_COLOR(12, 14);

    /* FAKEMATCH(barrier): the barrier keeps the sign extension apart from the
     * read. */
    asm("" : "+r"(raw));
    saved = (s16)raw;
    index = 14;
  loop:
    palette = (volatile u16 *)&BG_PLTT_COLOR(12, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto loop;
    *(volatile u16 *)&BG_PLTT_COLOR(12, 1) = saved;
  }
  color = gLemuriaPalace_GlowColors[(gFrameCount >> 2) % 12];
  *(volatile u16 *)&BG_PLTT_COLOR(12, 15) = color;
}

/// The room's setup: it schedules the palette cycle
/// LemuriaPalace_CycleGlowColors.
s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Task_AddFrame(LemuriaPalace_CycleGlowColors, 0xc80);
  FieldActor_SetSpritePriority(8, 3);
  if (gMainRoutineWork.entrance == 6)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(0x96a)) {
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Actor 13 raises a hand and two sparks fly up, as each map is called up. The
/// caller passes the actor, but the sparks always come from actor 13.
void LemuriaPalace_CastMapSparks(s32 actorId) {
  struct Actor *object = Actor_Get(13);
  struct Actor *first;
  struct Actor *second;

  Actor_SetAnimation(13, 5);
  FieldEvent_Wait(28);
  Event_PlaySound(SE_MENU_CONFIRM);
  first =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  first->speedLimit = 0x20000;
  first->acceleration = 0x20000;
  first->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(first, first->x + PIXELS(6),
                                first->y + PIXELS(8), first->z);
  second =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  second->speedLimit = 0x20000;
  second->acceleration = 0x20000;
  second->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(second, first->x + PIXELS(8),
                                first->y - PIXELS(4), first->z);
  FieldActor_WaitForMove(first);
  Actor_Release(first);
  FieldActor_WaitForMove(second);
  Actor_Release(second);
  FieldEvent_Wait(30);
}

extern s8 gLemuriaPalace_MapMode;
extern u32 gLemuriaPalace_MapCrossfade[];
s32 __modsi3();

/// A task: draws the maps the hall shows, fading between them by the map mode
/// (1, the fade level by frame; 2, the cross-fade that flickers between them).
void LemuriaPalace_DrawMaps(void) {
  struct Vector3 position;
  s8 mode = gLemuriaPalace_MapMode;

  if (mode == 1) {
    s32 *time = &gLemuriaPalace_MapTime;
    s32 t = *time;
    s32 level = gLemuriaPalace_MapFadeLevels[(t >> 3) & 15];

    {
      s32 value = level << 8;
      /* FAKEMATCH(dead): an empty block holds the shift first. */
      do {
      } while (0);
      value |= 16 - level;

      REG_BLDALPHA = value;
    }
    /* FAKEMATCH(dead): the step in a once-only block holds it after the store.
     */
    do {
      t++;
    } while (0);
    *time = t;
  } else if (mode == 2) {
    s32 *time = &gLemuriaPalace_MapTime;
    s32 step = (*time >> 2) % 28;

    {
      u16 *table = (u16 *)gLemuriaPalace_MapCrossfade;
      /* FAKEMATCH(temporary): the offset in a local puts the table first in the
       * load. */
      s32 at = step * 2;
      s32 value = *(u16 *)((u8 *)table + at);

      REG_BLDALPHA = value;
    }
    if (step == 0) {
      gLemuriaPalace_FirstMapShown = 1;
      gLemuriaPalace_SecondMapShown = step;
      Event_PlaySound(SE_HIGH_JUMP);
    }
    if (step == 14) {
      gLemuriaPalace_FirstMapShown = 0;
      gLemuriaPalace_SecondMapShown = 1;
      Event_PlaySound(SE_HIGH_JUMP);
    }
    (*time)++;
  }
  position.x = 0x3400000;
  position.y = 0;
  position.z = 0x1c00000;
  Camera_WorldToScreen(&position);
  if (gLemuriaPalace_FirstMapShown != 0) {
    gLemuriaPalace_FirstMapSprite.x = position.x;
    gLemuriaPalace_FirstMapSprite.y = position.z;
    EffectSprite_Draw(&gLemuriaPalace_FirstMapSprite);
  }
  if (gLemuriaPalace_SecondMapShown != 0) {
    gLemuriaPalace_SecondMapSprite.x = position.x;
    gLemuriaPalace_SecondMapSprite.y = position.z;
    EffectSprite_Draw(&gLemuriaPalace_SecondMapSprite);
  }
}

extern s32 gLemuriaPalace_FirstMapVram;

/// Loads the two maps the council shows the party, the ancient world and
/// Lunpa's, and starts drawing them.
void LemuriaPalace_LoadMaps(void) {
  /* FAKEMATCH(wrapper): forwarded. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x1000);
  s32 *firstSlot;
  s32 *firstTile;
  s32 *secondSlot;
  s32 *secondTile;

  Io_Write16(0x3f40, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Decompress_TaggedLz(gLemuriaPalace_FirstMapTiles, buffer);
  firstSlot = &gLemuriaPalace_FirstMapVram;
  *firstSlot = VramCache_FindFreeEntry();
  firstTile = &gLemuriaPalace_FirstMapTile;
  *firstTile = Vram_Load(gLemuriaPalace_FirstMapVram, 0x1000, buffer);
  EffectSprite_Init(&gLemuriaPalace_FirstMapSprite, 0, 0, 0xc0000000,
                    gLemuriaPalace_FirstMapTile);
  gLemuriaPalace_FirstMapSprite.depth = 1;
  gLemuriaPalace_FirstMapSprite.oam.priority = 2;
  gLemuriaPalace_FirstMapSprite.oam.objectMode = 1;
  gLemuriaPalace_FirstMapSprite.oam.palette256 = 1;
  gLemuriaPalace_FirstMapSprite.oam.palette = 0;
  Decompress_TaggedLz(gLemuriaPalace_SecondMapTiles, buffer);
  secondSlot = &gLemuriaPalace_SecondMapVram;
  *secondSlot = VramCache_FindFreeEntry();
  secondTile = &gLemuriaPalace_SecondMapTile;
  *secondTile = Vram_Load(gLemuriaPalace_SecondMapVram, 0x1000, buffer);
  EffectSprite_Init(&gLemuriaPalace_SecondMapSprite, 0, 0, 0xc0000000,
                    gLemuriaPalace_SecondMapTile);
  gLemuriaPalace_SecondMapSprite.depth = 2;
  gLemuriaPalace_SecondMapSprite.oam.priority = 2;
  gLemuriaPalace_SecondMapSprite.oam.objectMode = 1;
  gLemuriaPalace_SecondMapSprite.oam.palette256 = 1;
  gLemuriaPalace_SecondMapSprite.oam.palette = 0;
  gLemuriaPalace_FirstMapShown = 0;
  gLemuriaPalace_SecondMapShown = 0;
  gLemuriaPalace_MapTime = 0;
  gLemuriaPalace_MapMode = 0;
  Task_AddFrame(LemuriaPalace_DrawMaps, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}

/// Stops drawing the maps and frees their tiles.
void LemuriaPalace_FreeMaps(void) {
  FieldTask_Remove(LemuriaPalace_DrawMaps);
  VramCache_FreeEntry(gLemuriaPalace_FirstMapVram);
  VramCache_FreeEntry(gLemuriaPalace_SecondMapVram);
}

static __inline__ void Blend_WriteAlpha(s32 value) { REG_BLDALPHA = value; }

#define BLEND_SET_ALPHA(value)                                                 \
  do {                                                                         \
    Blend_WriteAlpha(value);                                                   \
  } while (0)

/// Fades the shown map in over 16 steps of four frames.
void LemuriaPalace_FadeMapsIn(void) {
  s32 i;

  BLEND_SET_ALPHA(0x1000);
  gLemuriaPalace_MapMode = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA(((16 - i) << 8) | i);
  System_Wait(4);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(16);
  gLemuriaPalace_MapMode = 1;
}

/// The reverse fade, in steps of three frames.
void LemuriaPalace_FadeMapsOut(void) {
  s32 i;

  BLEND_SET_ALPHA(16);
  gLemuriaPalace_MapMode = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA((i << 8) | (16 - i));
  System_Wait(3);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(0x1000);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_Start();
s32 __modsi3();
void Field_StartPaletteTransition();

/// In the palace the party is stopped by a guard, then brought before King
/// Hydros and his council, who have been waiting for the warriors Piers spoke
/// of.
/// "You are Lord Felix, correct?"
s32 LemuriaPalace_MeetKingHydros(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r7_1, r8_1, r5_3, r5_4, r5_5;
  s32 r0, v17, r2, r3;
  struct Actor *v3, *v6, *v7, *v12, *v13, *v19, *v20, *v22, *v23, *v24, *v25,
      *v26, *v27;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaPalace11);
  Actor_SetSpeed(16, 1.2);
  Actor_WalkByAndWait(16, -24, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH + FACING_STEP);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_1 = party + 532;
  }
  Actor_FaceActor(16, FIELD_AT(r5_1, s32, 0), 0);
  Event_OpenMessage(16, 0);
  r7_1 = Event_AskYesNoWithActorAnimation(4, 0);
  if (r7_1 == 0)
    goto answeredNo;
  Event_Wait(30);
  Actor_SetAnimationAndWait(16, 4);
  Event_ShowMessageThenWait(16, 0, 5);
  Actor_WalkByAndWait(FIELD_AT(r5_1, s32, 0), 0, 16);
  Actor_WalkByAndWait(16, 24, 0);
  Actor_FaceDirectionAndWait(16, FACING_SOUTH + FACING_STEP);
  r0 = Event_End();
  goto done;
answeredNo:
  Actor_SetAnimationAndWait(16, 3);
  Event_Wait(10);
  Actor_WalkByAndWait(16, 24, 0);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_WatchAndSetScript(16, FIELD_AT(r5_1, s32, 0));
  Event_SetFlag(0x96a);
  Event_SetFlag(0x96b);
  Party_AddMember(7);
  v3 = Actor_GetById(64);
  v3->scaleY = r7_1;
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_SetSpeed(13, 0.7);
  Actor_SetSpeed(14, 0.7);
  Actor_SetSpeed(7, 0.7);
  Actor_WalkToAndWait(4, 872, 528);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirectionAndWait(13, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(30);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  FieldCamera_FollowActor(13, 1);
  Actor_WalkTo(13, 864, 444);
  Event_Wait(20);
  Actor_WalkTo(14, 848, 444);
  Event_Wait(20);
  Actor_WalkTo(7, 832, 444);
  Actor_WaitForMoveById(13);
  Actor_SetAnimation(13, 1);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(14);
  Actor_SetAnimation(14, 1);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WaitForMoveById(7);
  Actor_SetAnimation(7, 1);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 496, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(5);
  FieldActor_LaunchFromLeader(17, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_LaunchFromLeader(5, -16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, -32, 0, FACING_NORTH);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_WaitForMoveById(6);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTH + FACING_STEP);
  Actor_WalkByAndWait(6, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(6, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTH + FACING_STEP);
  Actor_WalkByAndWait(5, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Actor_WalkByAndWait(4, 0, -8);
  Event_Wait(10);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  v6 = Actor_GetById(17);
  v6->actionFlags &= ~1;
  Actor_WalkByAndWait(17, 0, -8);
  Event_Wait(1);
  v7 = Actor_GetById(17);
  v7->actionFlags |= 1;
  Event_Wait(10);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_NORTH + FACING_STEP);
  Event_Wait(15);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_RunRepeatedMotion(17, 2);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  LemuriaPalace_CastMapSparks(13);
  Field_StartColorTransition(32, 0x2108);
  Event_Wait(32);
  Event_Wait(20);
  LemuriaPalace_LoadMaps();
  r8_1 = 1;
  gLemuriaPalace_FirstMapShown = r8_1;
  gLemuriaPalace_SecondMapShown = r7_1;
  Event_PlaySound(SE_HIGH_JUMP);
  LemuriaPalace_FadeMapsIn();
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Event_ShowMessageThenWait(15, 0, 10);
  FieldActor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  LemuriaPalace_CastMapSparks(13);
  LemuriaPalace_FadeMapsOut();
  Event_Wait(30);
  gLemuriaPalace_FirstMapShown = r7_1;
  gLemuriaPalace_SecondMapShown = r8_1;
  Event_PlaySound(SE_HIGH_JUMP);
  LemuriaPalace_FadeMapsIn();
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 10);
  Event_ShowMessageThenWait(14, 0, 5);
  v12 = Actor_GetById(17);
  v12->actionFlags &= ~1;
  Actor_WalkByAndWait(17, 0, -16);
  Event_Wait(1);
  v13 = Actor_GetById(17);
  v13->actionFlags |= 1;
  Event_Wait(25);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_WalkByAndWait(17, 0, 16);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  FieldActor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  LemuriaPalace_CastMapSparks(13);
  LemuriaPalace_FadeMapsOut();
  Event_Wait(10);
  gLemuriaPalace_MapTime = r7_1;
  gLemuriaPalace_MapMode = 2;
  Event_Wait(150);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(17, 0, 0);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetSpeed(15, 1);
  Actor_WalkToAndWait(15, 888, 444);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(5);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 472, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  FieldActor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  LemuriaPalace_CastMapSparks(13);
  Event_PlaySound(SE_HIGH_JUMP);
  r5_3 = 0;
  goto waitForCycle;
nextFrame:
  r5_3 = r5_3 + 1;
waitForCycle:
  if (r5_3 <= 119) {
    wait(1);
    v17 = __modsi3((gLemuriaPalace_MapTime) >> 2, 14);
    r0 = v17;
    if (v17 != 0)
      goto nextFrame;
  }
  LemuriaPalace_FreeMaps();
  Field_StartPaletteTransition(32);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_SetAnimationAndWait(15, 3);
  Event_Wait(15);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(13, 0x7fff);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(14, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(20);
  Actor_RunRepeatedMotion(15, 2);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(17, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(25);
  Actor_FaceDirectionAndWait(17, FACING_WEST);
  Event_Wait(20);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST + FACING_STEP);
  Event_Wait(15);
  Actor_RunRepeatedMotion(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 4);
  Event_ShowMessageThenWait(14, 0, 5);
  FieldActor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  FieldActor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  FieldActor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_OpenMessage(15, 0);
  if (!Event_AskYesNoWithActorAnimation(4, 0)) {
    Event_Wait(15);
    Event_ShowMessageThenWait(15, 0, 5);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    r2 = ((s32)Ram_HeapSlots->eventWork) + 452;
    r3 = (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    Event_Wait(35);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    Event_ShowMessageThenWait(15, 0, 5);
  }
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(13, 0, 5);
  Actor_SetAnimationAndWait(15, 4);
  Actor_SetSpeed(15, 0.5);
  v19 = Actor_GetById(15);
  v19->actionFlags &= ~1;
  Actor_WalkByAndWait(15, 32, 0);
  Event_Wait(1);
  v20 = Actor_GetById(15);
  v20->actionFlags = 1 | (v20->actionFlags);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(17, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTHEAST);
  Event_Wait(5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetAnimationAndWait(15, 4);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(7, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_SetAnimationAndWait(15, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_RunRepeatedMotion(15, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_SetSpeed(15, 1.2);
  Actor_WalkByAndWait(15, -50, 24);
  Actor_WatchAndSetScript(13, 15);
  Actor_WatchAndSetScript(14, 15);
  Actor_WatchAndSetScript(7, 15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WatchAndSetScript(6, 15);
  Actor_WatchAndSetScript(17, 15);
  FieldCamera_FollowActor(15, 1);
  Actor_WalkByAndWait(15, -8, 32);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Actor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  Actor_SetSpeed(4, 2.2);
  Actor_SetSpeed(5, 2.2);
  Actor_WalkBy(5, -8, 16);
  Actor_WalkByAndWait(4, 8, 16);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Event_Wait(15);
  Actor_WatchAndSetScript(4, 15);
  Actor_WatchAndSetScript(5, 15);
  Actor_WalkByAndWait(15, 0, 64);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(15, FACING_NORTH + FACING_STEP);
  Event_Wait(10);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Event_ShowMessageThenWait(15, 0, 5);
  Actor_FaceDirectionAndWait(15, FACING_SOUTH + FACING_STEP);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, FACING_WEST);
  Event_Wait(15);
  Actor_FaceDirectionAndWait(16, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(15);
  Actor_SetAnimation(16, 5);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_WalkByAndWait(15, 0, 96);
  Event_Wait(20);
  Actor_SetPosition(15, 0, 0);
  Actor_SetSpeed(4, 1);
  Actor_SetSpeed(5, 1);
  Actor_WalkBy(5, 8, -16);
  Actor_WalkByAndWait(4, -8, -16);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_Stop(16);
  Actor_SetAnimation(16, 1);
  Actor_FaceDirectionAndWait(17, FACING_NORTHWEST + FACING_STEP);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(40);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessageThenWait(6, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(14, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(20);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(14, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  Actor_WalkByAndWait(14, 24, 26);
  Actor_WalkByAndWait(14, 0, 8);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Actor_Jump(14, 4, 20);
  r5_4 = 15;
  goto grow;
grow:
  v22 = Actor_GetById(64);
  v22->scaleY = (v22->scaleY) + 4096;
  r5_4 = r5_4 - 1;
  wait(1);
  if (r5_4 >= 0)
    goto grow;
  v23 = Actor_GetById(64);
  v23->scaleY = 0x10000;
  Event_Wait(5);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(17, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_WalkByAndWait(14, -24, -26);
  Actor_WalkByAndWait(14, 0, -8);
  Actor_FaceDirectionAndWait(14, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(30);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(15);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_Wait(10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Actor_SetAnimationAndWait(13, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 5);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(13, FACING_WEST);
  Event_Wait(30);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_SetAnimationAndWait(13, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 5);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(30);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(20);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 10);
  Actor_FaceDirectionAndWait(13, FACING_NORTH + FACING_STEP);
  Event_Wait(5);
  Actor_WalkToAndWait(13, 864, 392);
  Actor_FaceDirectionAndWait(13, FACING_SOUTHEAST + FACING_STEP);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(14, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirectionAndWait(14, FACING_SOUTHEAST + FACING_STEP);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(14, 0, 5);
  Actor_FaceDirection(17, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  Event_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(17, 3);
  Event_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_SetSpeed(17, 1.2);
  Actor_SetAnimation(17, 2);
  {
    s32 party = (s32)&gMainRoutineWork;

    r5_5 = party + 532;
  }
  v24 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v24 != 0) {
    Actor_SetDestination(17, ((s16 *)&v24->x)[1], ((s16 *)&v24->z)[1]);
  }
  Actor_WaitForMoveById(17);
  Actor_SetPosition(17, 0, 0);
  Actor_SetSpeed(5, 1.2);
  Actor_SetAnimation(5, 2);
  v25 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v25 != 0) {
    Actor_SetDestination(5, ((s16 *)&v25->x)[1], ((s16 *)&v25->z)[1]);
  }
  Actor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 1.2);
  Actor_SetAnimation(6, 2);
  v26 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v26 != 0) {
    Actor_SetDestination(6, ((s16 *)&v26->x)[1], ((s16 *)&v26->z)[1]);
  }
  Actor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(4, -32, -48);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  Event_Wait(10);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(7, 3);
  Event_Wait(10);
  Actor_SetSpeed(7, 1.2);
  Actor_SetAnimation(7, 2);
  v27 = Actor_GetById(FIELD_AT(r5_5, s32, 0));
  if (v27 != 0) {
    Actor_SetDestination(7, ((s16 *)&v27->x)[1], ((s16 *)&v27->z)[1]);
  }
  Actor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(15);
  Actor_WalkToAndWait(14, 840, 408);
  Actor_FaceDirectionAndWait(14, FACING_SOUTHEAST + FACING_STEP);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(10);
  r0 = Event_End();
done:
  return r0;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaPalace),
    SCENE_EXIT_UNLESS(1, Lemuria, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaPalace, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemuriaPalace, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemuriaPalace, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaPalace, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, LemuriaPalace, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LemuriaPalace, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LemuriaPalace, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemuriaPalace, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Lemuria, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaPalace, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, LemuriaPalace, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, LemuriaPalace, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, LemuriaPalace, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemuriaPalace, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, LemuriaPalace, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, Lemuria, 17, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {157, CONDITION_ALWAYS, 1, PIXELS(912), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, 1, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(456), FACING_EAST, 3, 0},
    {165, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(360), FACING_EAST, 1, 0},
    {164, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(136), FACING_NORTH + FACING_STEP, 1, 0},
    {31, CONDITION_ALWAYS, 1, PIXELS(864), 0, PIXELS(392), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {43, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {34, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(392), FACING_WEST, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(560), FACING_SOUTH + FACING_STEP, 1, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(424), FACING_NORTHEAST, 2,
     0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
/// The palace doors' cell animations: a narrow door's, then a wide door's.
s16 gLemuriaPalace_DoorOpening[] = {
    1, 52, 1, 2, 6, 2, 52, 1,
    2, 6, -1, 2, 54, 2, 2, 6,
    4, 54, 2, 2, 6, -1,
};

DEFINE_G_DATA_0200064C(gLemuriaPalace_Doors, gLemuriaPalace_DoorOpening, 0x170005, gLemuriaPalace_DoorOpening,
    0x17001e)

s32 gLemuriaPalace_InnerDoor[] = {
    (s32)gLemuriaPalace_DoorOpening,
    0x40038,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
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
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0xc602, 16, CONDITION_ALWAYS, EVENT(LemuriaPalace_EnterDoor)},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 20, 0x96a, EVENT(LemuriaPalace_MeetKingHydros)},
    {TRIGGER_TALK, 8, 0x196a, MSGNO(MsgLemuriaPalace12)},
    {TRIGGER_TALK, 9, 0x196a, MSGNO(MsgLemuriaPalace13)},
    {TRIGGER_TALK, 10, 0x196a, MSGNO(MsgLemuriaPalace14)},
    {TRIGGER_TALK, 11, 0x196a, MSGNO(MsgLemuriaPalace15)},
    {TRIGGER_TALK, 12, 0x196a, MSGNO(MsgLemuriaPalace16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x196a, MSGNO(MsgLemuriaPalace17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x196a, MSGNO(MsgLemuriaPalace18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x196a, MSGNO(MsgLemuriaPalace19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x196a, MSGNO(MsgLemuriaPalace20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x196a, MSGNO(MsgLemuriaPalace21)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace03)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace04)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace10)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace22)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace23)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace27)},
    {243, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {243, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {243, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {243, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {243, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {243, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {243, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {243, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {451, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {TRIGGER_TABLE_END},
};

s16 gLemuriaPalace_GlowColors[] = {
    0x7b00, 0x76c0, 0x7280, 0x6e40, 0x6a00, 0x65c0,
    0x6180, 0x65c0, 0x6a00, 0x6e40, 0x7280, 0x76c0,
};
// clang-format on
#undef BLEND_SET_ALPHA
#undef FIELD_AT

#undef BLEND_SET_ALPHA
#undef FIELD_AT

// clang-format off
LATE_DATA u32 gLemuriaPalace_MapCrossfade[] = {
    0xc040e02, 0x8080a06, 0x40c060a, 0x100010, 0x60a040c, 0xa060808,
    0xe020c04, 0xc040e02, 0x8080a06, 0x40c060a, 0x100010, 0x60a040c,
    0xa060808, 0xe020c04,
};
// clang-format on
