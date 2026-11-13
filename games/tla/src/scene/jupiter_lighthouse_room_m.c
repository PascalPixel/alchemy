/// @file
/// The scene code of overlay 0x6a3, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "types.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "event_inline.h"
#include "jupiter_lighthouse_room_m.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write.h"
#include "call.h"
#include "field_sprite.h"
#include "frame.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/scene/jupiter_lighthouse_room_m.h"
#include "module/scene/scene.h"
#include "module/scene/world_map.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "resource.h"
#include "text/msg_ids.h"

s32 Func_02000038(s32 object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_02005a3c;

void Whirlwind_SpawnUpper();

void Func_02000060(void) {
  if (gData_02005a3c == 0) {
    Whirlwind_SpawnUpper(Actor_Get(24));
    gData_02005a3c = 1;
  }
}

void Func_02000080(void) {
  if (gData_02005a3c == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 10) {
      struct Actor *object = Actor_Get(party->currentOwner);

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Whirlwind_SpawnUpper(Actor_Get(25));
      gData_02005a3c = 1;
    }
  }
}

/* VIEW(jupiter_lighthouse_room_m): variable gBgScroll as s16 gBgScroll[]; bg_scroll.h has struct BgScroll gBgScroll[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_m.c:134: incompatible types in assignment. */
extern s16 gBgScroll__jupiter_lighthouse_room_m[] __asm__("gBgScroll");

void Actor_EmitRisingSparks(void);

void Func_020000c4(void) {

  u16 *buffer;
  u16 *p;
  u32 i;

  {
    u8 *enabled = &gCpuLoadDisplayEnabled;

    *enabled = 0;
  }
  REG_DISPCNT &= ~0x100;
  Io_Write16(0x400, &REG_BG0CNT);
  buffer = Heap_Alloc(0x800);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)(0x06000000), 0x84000200, REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = Heap_Alloc(128);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  p = buffer;
  for (i = 0; i != 64; i++)
    *p++ &= 0xfcff;
  p = buffer;
  for (i = 0; i != 16; i++) {
    Dma_Set(p, (void *)(0x06002000 + (i << 6)), 0x84000002, REG_DMA3);
    p += 4;
  }
  Heap_Release(buffer);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll__jupiter_lighthouse_room_m[0] = -104;
  gBgScroll__jupiter_lighthouse_room_m[1] = 16;
  System_Wait(30);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= 0x100;
  Event_PlaySound(144);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  FieldActor_Jump(4, 6, 0);
  Actor_Get(4)->actionFlags &= ~1;
  Actor_WalkBy(4, -16, 0);
  Actor_Get(20)->update = Actor_EmitRisingSparks;
  for (i = 0; i != 32; i++) {
    REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
    System_Wait(1);
  }
  MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
  MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
  MapGrid_SetCellBehavior(130 << 19, 160 << 16, 2, 236);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll__jupiter_lighthouse_room_m[0] = 0;
  gBgScroll__jupiter_lighthouse_room_m[1] = 0;
  System_Wait(1);
  gCpuLoadDisplayEnabled = 1;
  MapGrid_SetShake(0, 0, 0);
}

/* VIEW(jupiter_lighthouse_room_m): routine Func_02004774 as void Func_02004774(s32 x, s32 z, s32 a, s32 b); module/scene/eastern_alhafra.h has void Func_02004774(struct Actor *object); tried as the definition's void Func_02004774(struct Actor *object), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_m.c:183: `Func_02004774__jupiter_lighthouse_room_m' undeclared (first use in this function). */
extern void Func_02004774__jupiter_lighthouse_room_m(s32 x, s32 z, s32 a, s32 b) __asm__("Func_02004774");

void Func_0200039c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  Call4(Func_02004774__jupiter_lighthouse_room_m, object->x, object->z, 0, -8);
}

static __inline__ void Func_02004774_(s32 x, s32 z, s32 a, s32 b) {
  Func_02004774__jupiter_lighthouse_room_m(x, z, a, b);
}

static __inline__ void Func_020048b4_(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline; as plain calls, 2 of 237
   * instructions of Func_020003b8 differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020003b8(s32 unused, s32 id) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(id);
  s32 x;
  s32 i;

  if (object->x >> 20 == 63 && object->z >> 20 == 15) {
    Event_SetFlag(2572);
    object->motionFlags = 3;
    System_Wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    object->motionFlags = 0;
    Event_PlaySound(134);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(254 << 18, -1, 232 << 16, 1);
    Func_020048f4();
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
    x = leader->x >> 20;
    if (x == 64) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1032, 232);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    } else if (x == 63) {
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 232);
    }
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 1000, 248);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(60);
    FieldEvent_Wait(50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x6000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x2000);
    FieldEvent_Wait(20);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0x4000);
    FieldEvent_Wait(20);
    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 257, 50);
    Func_020048b4_(gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
    FieldEvent_Wait(40);
    Func_02002efc(0);
    Func_020000c4();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    FieldActor_StartRepeatedMotion(4, 3);
    Actor_SetAttachedEffect(4, 258);
    FieldEvent_Wait(40);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x30000, 0x6000);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    Func_020048f4();
    FieldEvent_Wait(10);
    object->update = 0;
    Event_PlaySound(80);
    Sound_WaitForJingle();
    FieldEvent_Wait(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    Func_020048f4();
    FieldEvent_Wait(10);
    Func_02002efc(1);
    Event_End();
  }
  Func_02004774_(object->x, object->z, 0, -6);
}

/// Opens the floor under (49, 103) and (49, 106) once (flag 514, until 2591).
/// Object 23, standing on the ground at x cell 49 to 52 with flag 2573 clear,
/// falls through and sets 2573: from cells 50 and 51 it drops, lands with the
/// screen shaken and slides off sideways; from 49 and 52 it simply drops.
void Func_0200060c(void) {
  struct Actor *object = Actor_GetById(23);
  s32 spot = 0;
  s32 x;
  s32 i;

  if (Event_GetFlag(2591))
    return;
  GameFlag_SetBit(514);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(124);
  /* FAKEMATCH(wrapper): forwarded through Camera_MoveWithinMapBounds_ above;
   * as plain calls, 63 of 292 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds_(194 << 18, -1, PIXELS(208), 1);
  Func_020048f4();
  Event_Wait(6);
  Sound_Play(217);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
  x = object->x >> 20;
  if (x == 49)
    spot = 1;
  if (x == 50)
    spot = 2;
  if (x == 51)
    spot = 3;
  if (x == 52)
    spot = 4;
  if (object->y != 0)
    spot = 0;
  if (Event_GetFlag(2573))
    spot = 0;
  if (spot != 0)
    GameFlag_SetBit(2573);
  if ((u32)(spot - 2) <= 1) {
    FieldCamera_SetSpeed(0x40000, 0x8000);
    FieldCamera_FollowActor(23, 1);
    Func_020048f4();
    Sound_Play(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = PIXELS(-80);
    i = 0;
    if (object->y > object->terrainHeight) {
      do {
        System_Wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight);
    }
    MapGrid_SetShake(0x30000, 0x30000, 0xf333);
    Sound_Play(134);
    object->gravity = 0x10000;
    object->velocityY = PIXELS(6);
    Actor_SetSpeed(23, 0x18000, 0xc000);
    if (spot == 2)
      Actor_SetDestinationOffset(23, -32, 0);
    else
      Actor_SetDestinationOffset(23, 32, 0);
    Event_Wait(7);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    object->gravity = 0x8000;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(80);
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    MapGrid_SetShake(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  } else if (spot == 1 || spot == 4) {
    FieldActor_SetSpritePriority(23, 1);
    FieldCamera_SetSpeed(0x40000, 0x8000);
    FieldCamera_FollowActor(23, 1);
    Func_020048f4();
    Sound_Play(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(20);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    Event_Wait(60);
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    MapGrid_SetShake(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  }
  Event_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Event_End();
}

void Func_020008c8(void) {
  if (Event_GetFlag(0xa1f) != 0)
    return;
  Event_ClearFlag(0x202);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(125);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(208), 1);
  Func_020048f4();
  FieldEvent_Wait(6);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(133);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Talking to one of the party: each answers with its own message.
void Func_02000970(s32 actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  switch (actor) {
  case 0:
    Actor_RunRepeatedMotion(0, 2);
    FieldEvent_SetMessageId(MsgNoWhyAre);
    Event_ShowMessage(0, 0);
    break;
  case 2:
    Actor_RunRepeatedMotion(2, 2);
    FieldEvent_SetMessageId(MsgDonTWorry);
    Event_ShowMessage(2, 0);
    break;
  case 28:
    FieldEvent_SetMessageId(MsgWeHaveTo);
    Event_ShowMessage(28, 0);
    break;
  case 5:
    FieldEvent_SetMessageId(MsgIDonT);
    Event_ShowMessage(5, 0);
    break;
  case 6:
    Actor_SetAnimation(6, 4);
    FieldEvent_Wait(33);
    FieldEvent_SetMessageId(MsgYouVeBeen);
    Event_ShowMessage(6, 0);
    break;
  }
  Event_End();
}

s32 Func_02000a78(void) {
  struct Actor *object = Actor_Get(1);

  if (object != NULL) {
    struct FieldSprite *sprite = object->sprite;

    if ((gFrameCount & 127) == 0)
      Actor_SetAttachedEffect(29, 258);
    if (sprite != NULL) {
      // As sprite->unk14[n], the frame count's reload passes these stores.
      *(sprite->unk14 + 3) = 30;
      *(sprite->unk14 + 2) = 252;
      sprite->rotation = Math_Sin(gFrameCount * 728) / 128 - 0x800;
    }
  }
  return 1;
}

s32 Func_02000ad0(void) {
  switch (Random16Far() & 7) {
  case 0:
    Event_PlaySound(126);
    break;
  case 1:
    Event_PlaySound(212);
    break;
  case 2:
    Event_PlaySound(134);
    break;
  case 3:
  case 5:
  case 6:
  default:
    Event_PlaySound(133);
    break;
  case 4:
  case 7:
    Event_PlaySound(154);
    break;
  }
  return 0;
}

s32 Func_02000b38(u8 *object) {
  object[90] &= 0xfe;
  return 0;
}

extern u32 gData_0200558c[];
extern u32 gData_020055ec[];
extern u32 gData_02005644[];
extern u32 gData_0200569c[];

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so each   \
     * run of equal angles is held in r5 (0x2000, then 0xc000), the second     \
     * built while the third call is set up, as the native does; as a plain    \
     * argument, 31 of 57 instructions of Func_02000c08 differ in tla-en, and  \
     * 16 of 69 of Func_02000b48. */                                           \
    s32 angle_ = (angle);                                                      \
    Actor_SetPositionAndFacing(id, (x) << 16, (z) << 16, angle_);                                \
  }

/// Sets four actors' speeds, places them and starts their action scripts.
void Func_02000b48(void) {
  Actor_SetSpeed(26, 0x30000, 0x18000);
  Actor_SetSpeed(27, 0x30000, 0x18000);
  Actor_SetSpeed(0, 0x30000, 0x18000);
  Actor_SetSpeed(2, 0x30000, 0x18000);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTH);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetScript(0, gData_0200558c);
  Actor_SetScript(2, gData_02005644);
  Actor_SetScript(26, gData_020055ec);
  Actor_SetScript(27, gData_0200569c);
  Actor_SetScript(3, gData_02005548);
}

void Func_02000c08(void) {
  PLACE(2, 757, 216, FACING_SOUTHEAST);
  PLACE(0, 761, 219, FACING_SOUTHEAST);
  PLACE(3, 816, 285, FACING_SOUTHEAST);
  PLACE(1, 824, 302, FACING_NORTH);
  PLACE(29, 821, 302, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_Get(1), 0);
  Actor_SetScript(1, gData_02005524);
  {
    s32 priority = 0x480;

    FieldTask_AddFrame((void (*)(void))Func_02000a78, priority);
  }
  Actor_SetAnimation(0, 38);
  Actor_SetAnimation(2, 31);
  Actor_SetAnimation(3, 9);
}

#undef PLACE

extern u32 gData_02005748[];
extern u32 gData_0200578c[];
extern u32 gData_02005860[];
extern u32 gData_02005890[];
extern u32 gData_02005950[];
extern u32 gData_020049dc[];

static __inline__ struct Actor *I_Func_020048cc(s32 actor, s32 emote,
                                                s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 42 sites, 171 of 1568 instructions differ in tla-en. */
  return (struct Actor *)Func_080d47b4(actor, emote, frames);
}

/// The scene in which actor 4 meets 5, 6, 7 and 28 and the group is placed for
/// the arrival; arg selects one of its two layouts.
void Func_02000cac(s32 arg) {
  struct Actor *object;
  s32 angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgAhhhh2);
  Event_PlaySound(124);
  FieldEvent_Wait(26);
  Event_PlaySound(217);
  Actor_SetScript(4, gData_020056ec);
  if (arg != 0) {
    FieldEvent_ShowTwoMessagesAndWait(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    FieldEvent_ShowTwoMessagesAndWait(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  FieldActor_Stop(4);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldActor_LaunchFromLeader(7, 16, 0, FACING_WEST);
  FieldActor_LaunchFromLeader(28, -16, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(6, 16, 24, FACING_WEST);
  FieldActor_LaunchFromLeader(5, -16, 24, FACING_EAST);
  FieldCamera_FollowActor(5, 1);
  FieldActor_WaitForMoveById(5);
  (struct Actor *)Actor_ShowEmote(28, 0x102, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST + FACING_STEP, 0);
  (struct Actor *)Actor_ShowEmote(7, 0x101, 30);
  Actor_FaceDirection(5, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(7, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimation(28, 3);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(28, 0, 5);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_SetAnimation(28, 4);
    FieldEvent_Wait(33);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  }
  Actor_RunRepeatedMotion(5, 2);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  (struct Actor *)Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, 0x102, 30);
  Actor_SetSpeed(4, 0x00013333, 0x9999);
  Actor_SetScript(4, arg != 0 ? gData_02005748 : gData_0200578c);
  FieldEvent_Wait(30);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(28, 3);
  FieldEvent_Wait(30);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(28, 0xcccc, 0x6666);
  Actor_SetScript(28, arg != 0 ? gData_020057d0 : gData_02005800);
  Actor_SetScript(7, arg != 0 ? gData_0200d830 : gData_02005860);
  Actor_SetScript(5, arg != 0 ? gData_02005890 : gData_020058d8);
  Actor_SetScript(6, arg != 0 ? gData_02005920 : gData_02005950);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  if (arg != 0) {
    FieldCamera_MoveTo(PIXELS(677), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  } else {
    FieldCamera_MoveTo(PIXELS(950), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  }
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(28);
  FieldEvent_Wait(20);
  angle = 0x2000;
  Actor_SetPositionAndFacing(2, PIXELS(741), PIXELS(232), angle);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(PIXELS(800), 0, PIXELS(270), 1);
  Func_020048f4();
  Event_PlaySound(39);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimation(3, 0);
  Actor_FaceDirectionAndWait(3, FACING_NORTHWEST);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_FaceDirectionAndWait(3, angle);
  Actor_SetAnimation(3, 9);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  FieldActor_StartRepeatedMotion(3, 3);
  Actor_SetAttachedEffect(3, 258);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  Actor_Get(3)->actionFlags &= 0xfe;
  Actor_SetSpeed(3, 0x1999, 0xccc);
  Actor_SetDestinationOffset(3, 4, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, -2, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, 2, 1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, -2, -1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  FieldEvent_Wait(40);
  Actor_SetDestinationOffset(3, 0, -2);
  FieldActor_WaitForMoveById(3);
  Actor_SetSpeed(3, 0x20000, 0x10000);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(3, 4);
  } while (0);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_Get(3)->actionFlags |= 1;
  Actor_SetDestinationOffset(3, -2, -8);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 1);
  Actor_FaceDirectionAndWait(3, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(0, 0x102, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(2, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetScript(3, gData_02005548);
  Actor_SetPositionAndFacing(26, PIXELS(665), PIXELS(128), angle);
  Actor_SetPositionAndFacing(27, PIXELS(641), PIXELS(128),
                                                 angle);
  Event_PlaySound(78);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x100, 0);
  I_Func_020048cc(2, 0x100, 30);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(720), 0, PIXELS(210), 1);
  Actor_SetSpeed(26, 0x4000, angle);
  Actor_SetSpeed(27, 0x4000, angle);
  Actor_WalkTo(26, 714, 193);
  Actor_WalkTo(27, 698, 209);
  Actor_SetScript(26, gData_020049cc);
  Actor_SetScript(27, gData_020049dc);
  FieldEvent_Wait(20);
  I_Func_020048cc(2, 0x102, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Event_PlaySound(34);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldActor_RefreshSelectorById(26);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldActor_RefreshSelectorById(27);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x100, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x101, 0);
  I_Func_020048cc(0, 0x101, 30);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(0, 0x106, 30);
  Actor_FaceDirection(2, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(2, 0x102, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x109, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x107, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, 0x103, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Actor_SetAnimation(27, 4);
  Actor_SetAnimation(26, 4);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  I_Func_020048cc(2, 0x107, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x100, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, 0x109, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, 0x100, 30);
  Actor_FaceDirectionAndWait(0, angle);
  Actor_SetAnimation(0, 38);
  FieldCamera_MoveTo(PIXELS(784), 0, PIXELS(274), 1);
  Func_020048f4();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(PIXELS(720), 0, PIXELS(210), 1);
  Func_020048f4();
  Actor_SetAnimation(0, 1);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceEachOther(26, 27, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 0x108, 0);
  (struct Actor *)Actor_ShowEmote(26, 0x108, 30);
  (struct Actor *)Actor_ShowEmote(2, 0x101, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  {
    /* FAKEMATCH(pin): the angle pinned to r1, so it is copied there first; as
     * a plain local, 2 of 1568 instructions differ in tla-en. */
    register s32 a1 asm("r1") = angle;
    s32 flags = 0;

    /* FAKEMATCH(barrier): r1 and r2 are set before r0, as the native sets
     * them; without it, 18 of 1568 instructions differ in tla-en. */
    __asm__("" : : "r"(a1), "r"(flags));
    Actor_FaceDirection(26, a1, flags);
  }
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Actor_FaceDirectionAndWait(27, angle);
  } while (0);
  Actor_SetAnimation(26, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x103, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x109, 30);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(27, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  (struct Actor *)Actor_ShowEmote(27, 0x101, 30);
  Actor_SetSpeed(27, 0x20000, 0x10000);
  Actor_WalkByAndWait(27, -24, -24);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_NORTH);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirection(2, FACING_NORTHWEST, 0);
  (struct Actor *)Actor_ShowEmote(27, 0x103, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 0x108, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, 0x108, 30);
  Actor_WalkBy(27, 24, 24);
  Actor_SetScript(27, gData_020049ec);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldActor_RefreshSelectorById(27);
  Actor_FaceEachOther(26, 27, 30);
  Actor_SetAnimation(26, 3);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(27, 3);
  } while (0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(30);
  Actor_SetSpeed(26, 0x30000, 0x18000);
  Actor_SetSpeed(27, 0x30000, 0x18000);
  Actor_SetSpeed(0, 0x30000, 0x18000);
  Actor_SetSpeed(2, 0x30000, 0x18000);
  Actor_WalkTo(26, 737, 202);
  Actor_WalkTo(27, 731, 213);
  Actor_SetScript(27, gData_020049fc);
  Actor_SetScript(26, gData_02004a18);
  FieldEvent_Wait(5);
  Actor_WalkTo(0, 776, 202);
  Actor_WalkTo(2, 731, 232);
  Actor_SetScript(0, gData_02004a34);
  Actor_SetScript(2, gData_02004a50);
  FieldActor_RefreshSelectorById(0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(26);
  FieldActor_RefreshSelectorById(27);
  FieldEvent_Wait(40);
  Func_02000b48();
  FieldEvent_Wait(200);
  FieldCamera_FollowActor(4, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  I_Func_020048cc(6, 0x109, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(7, 0x107, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  (struct Actor *)Actor_ShowEmote(5, 0x109, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH);
  FieldEvent_Wait(10);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  } while (0);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceActor(6, 4, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  /* FAKEMATCH(dead): the statement kept in a once-only block, whose
   * loop note starts a scheduling region here as in the native; as plain
   * statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(6, 3);
  } while (0);
  Actor_SetAnimation(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(6, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(5, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(28, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(28, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetScript(7, gData_0200599c);
  Actor_SetScript(6, gData_0200599c);
  Actor_SetScript(5, gData_0200599c);
  Actor_SetScript(28, gData_0200599c);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(28);
  Event_End();
}

#include "scene_ids.h"
#include "version.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

static __inline__ void JupiterLighthouseRoomM_AddHp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from
   * the pool at each call, as the native does; as direct calls, 23 of 160
   * instructions of Func_02001c7c differ in tla-en, the four uses sharing it
   * in r5. */
  Unit_AddHp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_AddPp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from
   * the pool at each call, as the native does; as direct calls, 23 of 160
   * instructions of Func_02001c7c differ in tla-en, the four uses sharing it
   * in r5. */
  Unit_AddPp(owner, delta);
}

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so it is  \
     * built before x and z, as the native does; as a plain argument, 12 of    \
     * 160 instructions of Func_02001c7c differ in tla-en. */                  \
    s32 angle_ = (angle);                                                      \
    Actor_SetPositionAndFacing(id, (x) << 16, (z) << 16, angle_);                                \
  }

/// Places actors 0, 2, 27 and 26 and shows an apology; then the party is left
/// with owners 2 and 0, whose first values and PP drop by 10000. "I'm sorry"
void Func_02001c7c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTHWEST);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_SetAnimation(26, 1);
  Actor_SetAnimation(27, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgIMSorry);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, 0x100, 30);
  FieldCamera_MoveTo(PIXELS(705), 0, PIXELS(177), 1);
  Func_020048f4();
  FieldEvent_Wait(30);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHWEST);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  (struct Actor *)Actor_ShowEmote(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_WalkBy(26, 24, 0);
  FieldEvent_Wait(20);
  do {
    /* FAKEMATCH(dead): the store kept in a once-only block, whose end ends
     * CSE's block, so the byte store's dead zero is not kept in r8 for the
     * store of the owner below; without it, 31 of 160 instructions differ
     * in tla-en. */
    gMainRoutineWork.battleRule24b = 6;
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6a3, 77);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a3, 77);
  Party_RemoveMember(4);
  Party_RemoveMember(7);
  Party_RemoveMember(5);
  Party_RemoveMember(6);
  Party_RemoveMember(2);
  Party_RemoveMember(0);
  Party_AddMember(2);
  Party_AddMember(0);
  Party_RemoveMember(3);
  Party_RemoveMember(1);
  JupiterLighthouseRoomM_AddHp(2, -10000);
  JupiterLighthouseRoomM_AddHp(0, -10000);
  Unit_Calculate(0);
  Unit_Calculate(2);
  FieldUnit_AddHp(0, 173);
  JupiterLighthouseRoomM_AddPp(2, -10000);
  Unit_AddPp(0, -10000);
  gMainRoutineWork.currentOwner = 0;
  Field_RequestBattleFromEncounterTable(102, 4);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so the    \
     * first two angles are shared in r5 and the later ones built before z,    \
     * as the native does; as a plain argument, 24 of 53 instructions differ   \
     * in tla-en (as one function-wide angle local, 10 to 16). */              \
    s32 angle_ = (angle);                                                      \
    Actor_SetPositionAndFacing(id, (x) << 16, (z) << 16, angle_);                                \
  }

void Func_02001e38(void) {
  PLACE(0, 761, 219, FACING_NORTHWEST);
  PLACE(2, 741, 232, FACING_NORTHWEST);
  Actor_SetSingleChildFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 39);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  PLACE(5, 761, 203, FACING_SOUTH);
  PLACE(6, 725, 232, FACING_EAST);
  PLACE(28, 729, 203, FACING_SOUTHEAST);
  Actor_SetScript(3, gData_02005548);
}

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Inventory_Equip();

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                               \
    FieldEvent_Wait(frames);                                                        \
  } while (0)

// The Japanese form differs and is still on its listing.

static __inline__ void Walk(s32 id, s32 speed, s32 frames) {
  /* FAKEMATCH(wrapper): the walk forwarded inline, so its constants stay out of
   * CSE; as direct calls, 103 of 1328 instructions differ in tla-en. */
  ((struct Actor *)Func_080d47b4(id, speed, frames));
}

void Func_02001ed0(void) {
  s32 index;
  s32 offset;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(0x2e10000, 0, 0xd10000, 0);
  gMainRoutineWork.currentOwner = 4;
  Party_RemoveMember(0);
  Party_RemoveMember(2);
  Party_RemoveMember(5);
  Party_RemoveMember(6);
  Party_RemoveMember(7);
  Party_RemoveMember(3);
  Party_RemoveMember(1);
  Party_AddMember(4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(4, 0x2980000, 0x580000, angle);
  }
  {
    s32 angle = 0x2000;

    Actor_SetPositionAndFacing(0x1b, 0x2cd0000, 0xd00000, angle);
  }
  Actor_SetPositionAndFacing(0x1a, 0x2e10000, 0xc30000, 0x2000);
  {
    s32 angle = 0xa000;

    Actor_SetPositionAndFacing(0, 0x2f90000, 0xdb0000, angle);
  }
  Actor_SetPositionAndFacing(2, 0x2e50000, 0xe80000, 0xa000);
  Actor_SetScript(3, (s32)gData_02005548);
  Actor_SetSingleChildFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 0x27);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 0x20);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItAgatio);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAnimation(0x1a, 8);
  FieldEvent_Wait(0x28);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  FieldActor_StartRepeatedMotion(0, 2);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  Walk(0x1a, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0xe000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAnimation(0x1a, 1);
  FieldEvent_Wait(0x1e);
  Actor_FaceDirectionAndWait(0x1a, 0x5000);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_FollowActor(4, 1);
  FieldEvent_Wait(0x14);
  FieldActor_LaunchFromLeader(6, 0, 0, 0x4000);
  FieldCamera_FollowActor(6, 1);
  FieldActor_LaunchFromLeader(7, 0x10, 0, 0x4000);
  FieldActor_LaunchFromLeader(0x1c, -16, 0x18, 0x2000);
  FieldActor_LaunchFromLeader(5, -16, 0, 0x2000);
  Actor_WalkByAndWait(4, 0x10, 0x18);
  Actor_FaceDirection(4, 0x4000, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x100, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  Actor_FaceDirectionAndWait(0x1c, 0);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(4, 0x8000, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0x23);
  Actor_SetAnimation(4, 3);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_WalkTo(4, 0x2ab, 0xba);
  Actor_WalkTo(0x1c, 0x29b, 0xca);
  Actor_SetScript(4, (s32)gData_02004a6c);
  Actor_SetScript(0x1c, (s32)gData_02004a90);
  Actor_FaceDirection(0x1a, 0x8000, 0);
  Actor_FaceDirection(0x1b, 0x8000, 0);
  Walk(6, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceActor(5, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(7, 5, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  FieldEvent_Wait(0x14);
  Actor_WalkBy(7, 0, 0x28);
  Actor_WalkBy(6, 0, 0x28);
  Actor_WalkBy(5, 0, 0x28);
  Actor_SetScript(7, (s32)gData_02004ab4);
  Actor_SetScript(6, (s32)gData_02004ae8);
  Actor_SetScript(5, (s32)gData_02004b1c);
  FieldCamera_FollowActor(4, 1);
  Func_020048f4();
  FieldEvent_Wait(0x14);
  Walk(0x1a, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceDirectionAndWait(0x1a, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x4000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x103, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(6, 4, 0x21);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1b, 0x107, 0x1e));
  Event_OpenMessage(0x1b, 0);
  Actor_FaceActor(0x1c, 4, 0);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(6, 4, 0);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(0xa);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ((struct Actor *)Actor_ShowEmote(0x1b, 0x102, 0x1e));
    FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ((struct Actor *)Actor_ShowEmote(0x1b, 0x102, 0x1e));
    EVENT_WORK->messageId++;
    FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  }
  Actor_FaceActor(7, 0x1a, 0);
  ((struct Actor *)Actor_ShowEmote(7, 0x107, 0x1e));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceEachOther(0x1a, 0x1b, 0);
  FieldActor_RefreshSelectorById(0x1a);
  FieldActor_RefreshSelectorById(0x1b);
  Walk(0x1a, 0x105, 0);
  ((struct Actor *)Actor_ShowEmote(0x1b, 0x105, 0x1e));
  Actor_FaceActor(0x1c, 0x1b, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 4, 0x21);
  Actor_FaceActor(0x1a, 0x1c, 0);
  Actor_FaceActor(0x1b, 0x1c, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  FieldEvent_Wait(0x3c);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_RunRepeatedMotion(0x1a, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0x1a, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  FieldEvent_Wait(0x14);
  Actor_RunRepeatedMotion(0x1a, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0xa000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceEachOther(6, 5, 0);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(0x1c);
  Walk(4, 0x108, 0);
  Walk(6, 0x108, 0);
  Walk(5, 0x108, 0);
  Walk(7, 0x108, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x108, 0x28));
  Actor_FaceActor(4, 0x1a, 0);
  Actor_FaceActor(5, 0x1a, 0);
  Actor_FaceActor(6, 0x1a, 0);
  Actor_FaceActor(7, 0x1a, 0);
  Actor_FaceActor(0x1c, 0x1a, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  Event_OpenMessage(0x1a, 0);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    EVENT_WORK->messageId++;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  }
  Actor_FaceEachOther(0x1c, 4, 0);
  FieldActor_RefreshSelectorById(0x1c);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldEvent_Wait(0xa);
  ((struct Actor *)Actor_ShowEmote(4, 0x102, 0x1e));
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0x28);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0x1e);
  Actor_FaceEachOther(0x1a, 0x1b, 0);
  FieldActor_RefreshSelectorById(0x1a);
  FieldActor_RefreshSelectorById(0x1b);
  Walk(0x1b, 0x108, 0);
  Walk(0x1a, 0x108, 0x1e);
  Actor_FaceActor(0x1a, 4, 0);
  Actor_FaceActor(4, 0x1b, 0);
  Actor_WalkToAndWait(0x1b, 0x2bb, 0xca);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0, 0x100, 0);
  Walk(4, 0x100, 0);
  Walk(2, 0x100, 0);
  Walk(5, 0x100, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x100, 0x1e));
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x109, 0x1e));
  Actor_FaceDirection(0x1c, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x108, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceEachOther(4, 7, 0);
  Actor_FaceEachOther(6, 5, 0);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldEvent_Wait(0x28);
  Actor_FaceActor(4, 0x1a, 0);
  Actor_FaceActor(7, 0x1a, 0);
  Actor_FaceActor(6, 0x1a, 0);
  Actor_FaceActor(5, 0x1a, 0);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x107, 0x1e));
  Actor_FaceDirection(0x1b, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetSpeed(7, 0x40000, 0x20000);
  Actor_SetSpeed(0x1b, 0x20000, 0x10000);
  Actor_WalkBy(7, 8, 0x18);
  Actor_WalkByAndWait(0x1b, 8, 4);
  FieldActor_WaitForMoveById(7);
  FieldEvent_Wait(5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x103, 0x1e));
  Actor_FaceActor(0x1a, 7, 0);
  Actor_FaceActor(0x1b, 7, 0);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceActor(0x1a, 0, 0);
  Actor_FaceActor(0x1b, 0, 0);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldEvent_Wait(0x28);
  Actor_FaceDirectionAndWait(4, 0x2000);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldActor_WatchAndSetScript(0x1a, 4);
  FieldActor_WatchAndSetScript(0x1b, 4);
  Actor_WalkByAndWait(4, 5, 0x20);
  Actor_WalkByAndWait(4, 0x38, -2);
  FieldEvent_Wait(0x1e);
  FieldActor_StartRepeatedMotion(0, 2);
  FieldEvent_Wait(4);
  Actor_RunRepeatedMotion(4, 2);
  PartyInventory_Remove(0xdf);
  PartyInventory_Add(0xf5);
  Message_ShowCentered((s16)EVENT_WORK->messageId++, 5);
  index = 0;
  offset = 0xd8;
  for (index = 0; index <= 14; offset += 2, index++) {
    if ((*(u16 *)((u8 *)Unit_Get(4) + offset) & 0x1ff) == 245) {
      Inventory_Equip(4, index);
      break;
    }
  }
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceActor(4, 0x1b, 0);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0x14);
  Actor_SetSpeed(7, 0x20000, 0x10000);
  Actor_SetSpeed(0x1a, 0x8000, 0x4000);
  Actor_SetSpeed(0x1b, 0x8000, 0x4000);
  Actor_SetSpeed(4, 0x8000, 0x4000);
  Actor_SetScript(0x1a, (s32)gData_020059a4);
  FieldEvent_Wait(0x28);
  Actor_SetDestinationOffset(7, -16, 0);
  FieldEvent_Wait(0xa);
  Actor_SetScript(0x1b, (s32)gData_020059a4);
  FieldEvent_Wait(0x6e);
  Actor_SetScript(4, (s32)gData_020059a4);
  FieldActor_RefreshSelectorById(0x1a);
  Actor_SetScript(0x1a, (s32)gData_02005a00);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_SetScript(0x1b, (s32)gData_02005a00);
  FieldActor_RefreshSelectorById(4);
  Actor_SetAnimation(4, 1);
  FieldActor_StartRepeatedMotion(7, 2);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(0x1c, 0xc000, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  FieldCamera_FollowActor(6, 1);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ((struct Actor *)Actor_ShowEmote(4, 0x101, 0x1e));
  ((struct Actor *)Actor_ShowEmote(7, 0x109, 0x1e));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ((struct Actor *)Actor_ShowEmote(4, 0x100, 0x1e));
  ((struct Actor *)Actor_ShowEmote(6, 0x106, 0x1e));
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(5, 0xc000, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetScript(7, (s32)gData_020059a4);
  FieldActor_RefreshSelectorById(7);
  Party_AddMember(7);
  FieldCamera_FollowActor(4, 1);
  Actor_SetPosition(7, 0, 0);
  FieldEvent_Wait(0x14);
#if TLA_PATCH_1
  Event_ClearInvalidPackedValues();
#endif
  Actor_WalkToAndWait(4, 0x298, 0x46);
  Event_End();
}

/// Sets up Jupiter Lighthouse room M: objects 8 to 15 take mode 1, objects 24
/// and 25 stand at heights 0 and 32, the cells flags 514 and 2591 opened are
/// stamped again, and with 2591 set the arrival is chosen by flags 2595, 2593
/// and 2596 and the entrance.
s32 Scene_Initialize(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Func_02004994(8, 1);
  Func_02004994(9, 1);
  Func_02004994(10, 1);
  Func_02004994(11, 1);
  Func_02004994(12, 1);
  Func_02004994(13, 1);
  Func_02004994(14, 1);
  Func_02004994(15, 1);
  Func_02004984();
  Func_0200498c(0, 512, 21, 22);
  gData_02005a3c = 0;
  object = Actor_Get(24);
  object->motionFlags = 0;
  object->y = 0;
  object->terrainHeight = 0;
  object = Actor_Get(25);
  object->motionFlags = 0;
  object->y = PIXELS(32);
  object->terrainHeight = PIXELS(32);
  if (Event_GetFlag(514)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
  }
  if (Event_GetFlag(2591)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
    Actor_SetPosition(23, 0, 0);
    if (Event_GetFlag(2595)) {
      if (!Event_GetFlag(2593)) {
        Func_02000c08();
        Func_02001e38();
      }
    } else if (!Event_GetFlag(2596)) {
      if (gMainRoutineWork.entrance == 3) {
        Func_02000c08();
        Event_SetFlag(2596);
        Func_02000cac(gMainRoutineWork.entrance == 3);
      }
    } else {
      Func_02000c08();
      if (gMainRoutineWork.entrance == 5) {
        Func_02001c7c();
      } else if (gMainRoutineWork.entrance == 77) {
        Func_02001ed0();
        Event_SetFlag(2595);
        Event_ClearFlag(2596);
      } else {
        Func_02000b48();
      }
    }
  }
  Actor_Get(23)->motionFlags = 0;
  if (Event_GetFlag(2573))
    Actor_SetPosition(23, 0, 0);
  if (gMainRoutineWork.entrance == 99)
    Func_02001ed0();
  return 0;
}

void LightSprites_InitC();
void LightSprites_Add();

/// The scene's map preparation.
s32 Scene_PrepareMap(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  LightSprites_InitC();
  LightSprites_Add(20, 0);
  object = Actor_Get(20);
  if (Event_GetFlag(0xa0c)) {
    Actor_SetPosition(20, PIXELS(1016), PIXELS(248));
    MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
    MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
    MapGrid_SetCellBehavior(0x4100000, 0xa00000, 2, 236);
  }
  object->motionFlags = 0;
  {
    s32 zero = 0;

    /* FAKEMATCH(barrier): the last call's zero is built before the position
     * loads; as a plain 0 argument, 2 of 62 instructions differ in tla-en. */
    __asm__("" : : "r"(zero));
    Func_02004774__jupiter_lighthouse_room_m(object->x, object->z, zero, -6);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"

extern s32 gData_02005a18;

void Func_02002efc(s32 value) { gData_02005a18 = value; }

extern s32 gData_02005a14;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_02005a14 = value; }
extern u16 gData_02005a40[];

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gData_02005a14, gData_02005a18,
    gData_02005a40)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes, gData_02005a18, gData_02005a40,
    gData_02005a60, PaletteCycle_Step, gData_02005a64)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02004774__jupiter_lighthouse_room_m,
    Func_020049b4)

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern const s32 gData_02005a1c[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, Func_020049a4, Position_FlatDistance,
    gData_02005a1c, gData_02005a3c)
