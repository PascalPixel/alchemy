/// @file
/// The scene code of overlay 0x6a3 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_m/.

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
#include "jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write.h"
#include "call.h"
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"
#include "field_sprite.h"
#include "frame.h"

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

void Func_02003474();

void Func_02000060(void) {
  if (gData_02005a3c == 0) {
    Func_02003474(((struct FieldActor *)Actor_Get(24)));
    gData_02005a3c = 1;
  }
}

void Func_02000080(void) {
  if (gData_02005a3c == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 10) {
      struct FieldActor *object =
          ((struct FieldActor *)Actor_Get(party->currentOwner));

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Func_02003474(((struct FieldActor *)Actor_Get(25)));
      gData_02005a3c = 1;
    }
  }
}

extern u8 gCpuLoadDisplayEnabled;
extern char ResourceId_LightningTiles;
extern char ResourceId_LightningMap;
extern s16 gBgScroll[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200468c(void *resource, void *destination);
void Func_020046b4(void);
void Func_020046a4(void);
void Func_0200354c(void);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 8 of 281
   * instructions differ in tla-en. */
  Func_0802b450(a, b, c, d, e, f);
}

static __inline__ void Func_0200475c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

static __inline__ void Func_0200491c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

static __inline__ void Func_02004924_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  BattleEffect_ApplyColorToSourceBuffer(a, b);
}

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
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)(0x06000000), 0x84000200, REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = Heap_Alloc(128);
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
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
  gBgScroll[0] = -104;
  gBgScroll[1] = 16;
  System_Wait(30);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  Func_02004924_(0x7fff, 0);
  Func_0200491c_(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= 0x100;
  Event_PlaySound(144);
  Func_0200475c_(0x50000, 0x70000, 0x10000);
  Func_0200475c_(-1, -1, 0xe666);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  FieldActor_Jump(4, 6, 0);
  Actor_Get(4)->actionFlags &= ~1;
  Actor_WalkBy(4, -16, 0);
  Actor_Get(20)->update = Func_0200354c;
  for (i = 0; i != 32; i++) {
    REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
    System_Wait(1);
  }
  Func_0200474c_(104, 42, 5, 11, 61, 42);
  Func_0200474c_(106, 52, 1, 1, 63, 84);
  MapGrid_SetCellBehavior(130 << 19, 160 << 16, 2, 236);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x10000, 0);
  PaletteFade_Interpolate(60);
  Func_020046b4();
  Func_020046a4();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0] = 0;
  gBgScroll[1] = 0;
  System_Wait(1);
  gCpuLoadDisplayEnabled = 1;
  Func_0200475c_(0, 0, 0);
}

void Func_02004774();

void Func_0200039c(s32 unused, s32 id) {
  struct FieldActor *object = (struct FieldActor *)Actor_Get(id);

  Call4(Func_02004774, object->x, object->z, 0, -8);
}

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);
void Func_02002efc(s32 value);
void Func_020000c4(void);
void Sound_WaitForJingle(void);
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

static __inline__ void Func_02004774_(s32 x, s32 z, s32 a, s32 b) {
  Func_02004774(x, z, a, b);
}

static __inline__ void Func_020048b4_(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
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
    Func_020047d4();
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
    Func_020047dc();
  }
  Func_02004774_(object->x, object->z, 0, -6);
}

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 2 of 292
   * instructions differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_020048e4_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 26 of 292
   * instructions differ in tla-en. */
  Camera_SetSpeedParameters(a, b);
}

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 21 of 292
   * instructions differ in tla-en. */
  return GameFlag_Test(flag);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarded inline; as a plain call, 2 of 292
   * instructions differ in tla-en. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
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

  if (GameFlag_Test_(2591))
    return;
  GameFlag_SetBit(514);
  Func_020047d4();
  Event_ResolvePendingActions(0);
  Sound_Play(124);
  Camera_MoveWithinMapBounds_(194 << 18, -1, PIXELS(208), 1);
  Func_020048f4();
  Event_Wait(6);
  Sound_Play(217);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  Event_Wait(6);
  Func_0200474c_(49, 106, 4, 3, 49, 76);
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
  if (GameFlag_Test_(2573))
    spot = 0;
  if (spot != 0)
    GameFlag_SetBit(2573);
  if ((u32)(spot - 2) <= 1) {
    Func_020048e4_(0x40000, 0x8000);
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
    Func_0200475c_(0x30000, 0x30000, 0xf333);
    Sound_Play(134);
    object->gravity = 0x10000;
    object->velocityY = PIXELS(6);
    Actor_SetSpeedParameters_(23, 0x18000, 0xc000);
    if (spot == 2)
      Actor_OffsetPositionAndResetMotion_(23, -32, 0);
    else
      Actor_SetDestinationOffset(23, 32, 0);
    Event_Wait(7);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    object->gravity = 0x8000;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(80);
    Func_0200475c_(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    Func_0200475c_(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  } else if (spot == 1 || spot == 4) {
    FieldActor_SetSpritePriority(23, 1);
    Func_020048e4_(0x40000, 0x8000);
    FieldCamera_FollowActor(23, 1);
    Func_020048f4();
    Sound_Play(310);
    object->gravity = 0x8000;
    object->motionFlags = 2;
    object->terrainHeight = PIXELS(-240);
    Event_Wait(20);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    Event_Wait(60);
    Func_0200475c_(0x50000, 0x50000, 0x10000);
    Sound_Play(144);
    Event_Wait(20);
    Func_0200475c_(-1, -1, 0xe666);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(10);
  }
  Event_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);

void Func_020008c8(void) {
  if (Event_GetFlag(0xa1f) != 0)
    return;
  Event_ClearFlag(0x202);
  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(125);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(208), 1);
  Func_020048f4();
  FieldEvent_Wait(6);
  Func_0200474c_(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(133);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020048f4();
  Func_020047dc();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern const u8 MsgNoWhyAre[];
extern const u8 MsgDonTWorry[];
extern const u8 MsgWeHaveTo[];
extern const u8 MsgIDonT[];
extern const u8 MsgYouVeBeen[];

void Func_020047d4(void);
void Func_020047dc(void);

/// Talking to one of the party: each answers with its own message.
void Func_02000970(s32 actor) {
  Func_020047d4();
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
  Func_020047dc();
}

static __inline__ void I_Func_020048d4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its one site, 2 of 37 instructions differ in tla-en. */
  Func_080d489c(actor, a);
}

s32 Func_02000a78(void) {
  struct Actor *object = Actor_Get(1);

  if (object != NULL) {
    struct FieldSprite *sprite = object->animation;

    if ((gFrameCount & 127) == 0)
      I_Func_020048d4(29, 258);
    if (sprite != NULL) {
      // As sprite->unk14[n], the frame count's reload passes these stores.
      *(sprite->unk14 + 3) = 30;
      *(sprite->unk14 + 2) = 252;
      sprite->rotation = Math_Sin(gFrameCount * 728) / 128 - 0x800;
    }
  }
  return 1;
}

s32 Random16Far();

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

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_0200558c[];
extern u32 gData_020055ec[];
extern u32 gData_02005644[];
extern u32 gData_0200569c[];

s32 Func_02000a78(void);

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 30 of 57
   * instructions of Func_02000c08 differ in tla-en, and 25 of 69 of
   * Func_02000b48. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

static __inline__ void ObjSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 32 of 69
   * instructions of Func_02000b48 differ in tla-en, the speeds held in r5 and
   * r6 across the calls. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so each   \
     * run of equal angles is held in r5 (0x2000, then 0xc000), the second     \
     * built while the third call is set up, as the native does; as a plain    \
     * argument, 31 of 57 instructions of Func_02000c08 differ in tla-en, and  \
     * 16 of 69 of Func_02000b48. */                                           \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

/// Sets four actors' speeds, places them and starts their action scripts.
void Func_02000b48(void) {
  ObjSpeed(26, 0x30000, 0x18000);
  ObjSpeed(27, 0x30000, 0x18000);
  ObjSpeed(0, 0x30000, 0x18000);
  ObjSpeed(2, 0x30000, 0x18000);
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
    /* FAKEMATCH(temporary): the priority held in a block local, so 0x480 is
     * built before the task function's address is loaded; as an argument, 2 of
     * 57 instructions differ in tla-en. */
    s32 priority = 0x480;

    FieldTask_AddFrame((void (*)(void))Func_02000a78, priority);
  }
  Actor_SetAnimation(0, 38);
  Actor_SetAnimation(2, 31);
  Actor_SetAnimation(3, 9);
}

extern u32 gData_02005548[];

extern const u8 MsgAhhhh2[];
extern u32 gData_020056ec[];
extern u32 gData_02005748[];
extern u32 gData_0200578c[];
extern u32 gData_020057d0[];
extern u32 gData_02005800[];
extern u32 gData_0200d830[];
extern u32 gData_02005860[];
extern u32 gData_02005890[];
extern u32 gData_020058d8[];
extern u32 gData_02005920[];
extern u32 gData_02005950[];
extern u32 gData_020049cc[];
extern u32 gData_020049dc[];
extern u32 gData_020049ec[];
extern u32 gData_020049fc[];
extern u32 gData_02004a18[];
extern u32 gData_02004a34[];
extern u32 gData_02004a50[];
extern u32 gData_0200599c[];

void Func_020047d4(void);
void Func_020047dc(void);
void Func_020048f4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void I_Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                       s32 g, s32 h, s32 i, s32 j, s32 k) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 7 sites, 152 of 1568 instructions differ in tla-en. */
  Battle_ShowPairedUnitWorkAndWait(a, b, c, d, e, f, g, h, i, j, k);
}

static __inline__ void I_Actor_ArmCallback(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 15 sites, 73 of 1568 instructions differ in tla-en. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void I_Func_02004964(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 4 sites, 16 of 1568 instructions differ in tla-en. */
  Func_080d3070(actor, x, z, angle);
}

static __inline__ struct Actor *I_Func_020048cc(s32 actor, s32 emote,
                                                s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 42 sites, 171 of 1568 instructions differ in tla-en. */
  return (struct Actor *)Func_080d47b4(actor, emote, frames);
}

static __inline__ void I_Actor_SetSpeedParameters(s32 actor, s32 speed,
                                                  s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 14 sites, 78 of 1568 instructions differ in tla-en. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void I_Motion_CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void I_Func_0200484c(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 3 sites, 14 of 1568 instructions differ in tla-en. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

static __inline__ void I_Func_020048b4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 20 sites, 85 of 1568 instructions differ in tla-en. */
  Actor_ArmCallbackAndRefresh(actor, a);
}

static __inline__ void I_Actor_OffsetPositionAndResetMotion(s32 id, s32 dx,
                                                            s32 dz) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 6 sites, 26 of 1568 instructions differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void I_Actor_ResetAndSetPositionInMode2(s32 id, s32 x,
                                                          s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void I_Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its one site, 7 of 1568 instructions differ in tla-en. */
  Actor_CommitPositionAndActivate(id, x, z);
}

/// The scene in which actor 4 meets 5, 6, 7 and 28 and the group is placed for
/// the arrival; arg selects one of its two layouts.
void Func_02000cac(s32 arg) {
  struct Actor *object;
  s32 angle;

  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgAhhhh2);
  Event_PlaySound(124);
  FieldEvent_Wait(26);
  Event_PlaySound(217);
  Actor_SetScript(4, gData_020056ec);
  if (arg != 0) {
    I_Func_020048bc(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    I_Func_020048bc(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  FieldActor_Stop(4);
  I_Actor_ArmCallback(4, FACING_SOUTH, 0);
  I_Func_02004964(7, 16, 0, FACING_WEST);
  I_Func_02004964(28, -16, 0, FACING_EAST);
  I_Func_02004964(6, 16, 24, FACING_WEST);
  I_Func_02004964(5, -16, 24, FACING_EAST);
  FieldCamera_FollowActor(5, 1);
  FieldActor_WaitForMoveById(5);
  (struct Actor *)Actor_ShowEmote(28, 0x102, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  I_Actor_ArmCallback(5, FACING_NORTH, 0);
  I_Actor_ArmCallback(6, FACING_NORTHWEST + FACING_STEP, 0);
  (struct Actor *)Actor_ShowEmote(7, 0x101, 30);
  I_Actor_ArmCallback(5, FACING_NORTH + FACING_STEP, 0);
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
  I_Actor_SetSpeedParameters(4, 0x00013333, 0x9999);
  Actor_SetScript(4, arg != 0 ? gData_02005748 : gData_0200578c);
  FieldEvent_Wait(30);
  I_Actor_ArmCallback(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(28, FACING_EAST, 0);
  I_Actor_ArmCallback(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(28, 3);
  FieldEvent_Wait(30);
  I_Actor_SetSpeedParameters(5, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(7, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(6, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(28, 0xcccc, 0x6666);
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
  I_Func_0200484c(2, PIXELS(741), PIXELS(232), angle);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(PIXELS(800), 0, PIXELS(270), 1);
  Func_020048f4();
  Event_PlaySound(39);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimation(3, 0);
  I_Func_020048b4(3, FACING_NORTHWEST);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_FaceDirectionAndWait(3, angle);
  Actor_SetAnimation(3, 9);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  FieldActor_StartRepeatedMotion(3, 3);
  I_Func_020048d4(3, 258);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  Actor_Get(3)->actionFlags &= 0xfe;
  I_Actor_SetSpeedParameters(3, 0x1999, 0xccc);
  Actor_SetDestinationOffset(3, 4, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, 2, 1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, -1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  FieldEvent_Wait(40);
  I_Actor_OffsetPositionAndResetMotion(3, 0, -2);
  FieldActor_WaitForMoveById(3);
  I_Actor_SetSpeedParameters(3, 0x20000, 0x10000);
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
  I_Func_020048b4(3, FACING_NORTHWEST);
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
  I_Func_0200484c(26, PIXELS(665), PIXELS(128), angle);
  Actor_SetPositionAndFacing(27, PIXELS(641), PIXELS(128),
                                                 angle);
  Event_PlaySound(78);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x100, 0);
  I_Func_020048cc(2, 0x100, 30);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Func_020048b4(2, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  I_Motion_CamBounds(PIXELS(720), 0, PIXELS(210), 1);
  I_Actor_SetSpeedParameters(26, 0x4000, angle);
  I_Actor_SetSpeedParameters(27, 0x4000, angle);
  I_Actor_ResetAndSetPositionInMode2(26, 714, 193);
  I_Actor_ResetAndSetPositionInMode2(27, 698, 209);
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
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
  I_Func_020048b4(0, FACING_NORTHWEST);
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
  I_Func_020048b4(26, FACING_NORTHWEST);
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
  I_Motion_CamBounds(PIXELS(784), 0, PIXELS(274), 1);
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
  I_Func_020048b4(27, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  (struct Actor *)Actor_ShowEmote(27, 0x101, 30);
  I_Actor_SetSpeedParameters(27, 0x20000, 0x10000);
  I_Actor_CommitPositionAndActivate(27, -24, -24);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  FieldEvent_Wait(10);
  I_Func_020048b4(27, FACING_NORTH);
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
  I_Func_020048b4(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  I_Func_020048bc(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
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
  I_Actor_SetSpeedParameters(26, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(27, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(0, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(2, 0x30000, 0x18000);
  I_Actor_ResetAndSetPositionInMode2(26, 737, 202);
  I_Actor_ResetAndSetPositionInMode2(27, 731, 213);
  Actor_SetScript(27, gData_020049fc);
  Actor_SetScript(26, gData_02004a18);
  FieldEvent_Wait(5);
  I_Actor_ResetAndSetPositionInMode2(0, 776, 202);
  I_Actor_ResetAndSetPositionInMode2(2, 731, 232);
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
  Func_020047dc();
}
