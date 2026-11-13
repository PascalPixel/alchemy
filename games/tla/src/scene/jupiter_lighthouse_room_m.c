/// @file
/// The scene code of overlay 0x6a3, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "types.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"

void Leader_CenterInCell(void);
#include "system/io_reg.h"
#include "system/dma.h"
#include "system/io_write.h"
#include "call.h"
#include "field_sprite.h"
#include "system/frame.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"
#include "macros.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

struct CellTarget;
struct CellTargetEntry;

void PaletteCycle_SetPausesInEvents(s32 value);

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_020056ec[];
extern u32 gData_020057d0[];
extern u32 gData_02005800[];
extern u32 gData_0200d830[];
extern u32 gData_020058d8[];
extern u32 gData_02005920[];

extern u32 gData_020049cc[];
extern u32 gData_020049ec[];
extern u32 gData_020049fc[];
extern u32 gData_02004a18[];
extern u32 gData_02004a34[];
extern u32 gData_02004a50[];

extern u32 gData_0200599c[];

extern u32 gData_02004a6c[];
extern u32 gData_02004a90[];
extern u32 gData_02004ab4[];
extern u32 gData_02004ae8[];
extern u32 gData_02004b1c[];

extern u32 gData_020059a4[];
extern u32 gData_02005a00[];

void JupiterLighthouse_LoadPalettes(s32 mode);

void Func_02000060(void);

void Func_02000080(void);

void Func_0200060c(void);

void Func_020008c8(void);

void Whirlwind_CarryLeader(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

extern struct CellTarget gCrackedTiles;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry);

s32 Event_ClearInvalidPackedValues();
void HangingVine_Place(s32 id, s32 mode);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

#include "text/msg_ids.h"

s32 Func_02000038(s32 object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gWhirlwindRiding;

/* VIEW(jupiter_lighthouse_room_m): routine Whirlwind_SpawnUpper as void
 * Whirlwind_SpawnUpper(); scene/jupiter_lighthouse_room_m/s3474 has
 * DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper,
 * Import_ActorSelectAnimation, Whirlwind_Update__s3474, FieldEffect_Spawn)
 * DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)
 * DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
 * JupiterLighthouse_PlaceRaisedActors) extern s16 gLightSpritesLarge; tried as
 * the one declaration struct GustObject *Whirlwind_SpawnUpper(), it does not
 * compile in tla-ja (make modern): games/tla/include/field/field.h:25:41: note:
 * expected 'struct Actor *' but argument is of type 's32' {aka 'int'}. */
void Whirlwind_SpawnUpper__jupiter_lighthouse_room_m() __asm__(
    "Whirlwind_SpawnUpper");
#define Whirlwind_SpawnUpper Whirlwind_SpawnUpper__jupiter_lighthouse_room_m

void Func_02000060(void) {
  if (gWhirlwindRiding == 0) {
    Whirlwind_SpawnUpper(Actor_Get(24));
    gWhirlwindRiding = 1;
  }
}

void Func_02000080(void) {
  if (gWhirlwindRiding == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 10) {
      struct Actor *object = Actor_Get(party->currentOwner);

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Whirlwind_SpawnUpper(Actor_Get(25));
      gWhirlwindRiding = 1;
    }
  }
}

/* VIEW(jupiter_lighthouse_room_m): variable gBgScroll as s16 gBgScroll[];
 * bg_scroll.h has struct BgScroll gBgScroll[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:222: incompatible types in
 * assignment. */
extern s16 gBgScroll__jupiter_lighthouse_room_m[] __asm__("gBgScroll");

/* VIEW(jupiter_lighthouse_room_m): routine Actor_EmitRisingSparks as void
 * Actor_EmitRisingSparks(void); scene/jupiter_lighthouse_room_m/s3474 has
 * DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)
 * DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
 * JupiterLighthouse_PlaceRaisedActors) extern s16 gLightSpritesLarge; tried as
 * the definition's DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn,
 * Actor_SelectAnimation, Whirlwind_Update, FieldEffect_Spawn) UNUSED s32
 * Actor_EmitRisingSparks(struct..., it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:238: `Actor_EmitRisingSparks'
 * undeclared (first use in this function). */
void Actor_EmitRisingSparks__jupiter_lighthouse_room_m(void) __asm__(
    "Actor_EmitRisingSparks");
#define Actor_EmitRisingSparks Actor_EmitRisingSparks__jupiter_lighthouse_room_m

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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)(0x06000000), 0x84000200, REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = Heap_Alloc(128);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
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
  FieldMapGrid_SetCellBehavior(130 << 19, 160 << 16, 2, 236);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
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

/* VIEW(jupiter_lighthouse_room_m): routine MapGrid_SetCellHeight as void
 * MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
 * module/scene/eastern_alhafra.h has void MapGrid_SetCellHeight(struct Actor
 * *object); not tried: MapGrid_SetCellHeight's bound declaration was not found.
 */
extern void Func_02004774__jupiter_lighthouse_room_m(
    s32 x, s32 z, s32 a, s32 b) __asm__("MapGrid_SetCellHeight");

void Func_0200039c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  Call4(Func_02004774__jupiter_lighthouse_room_m, object->x, object->z, 0, -8);
}

static __inline__ void Func_02004774_(s32 x, s32 z, s32 a, s32 b) {
  Func_02004774__jupiter_lighthouse_room_m(x, z, a, b);
}

static __inline__ void Func_020048b4_(s32 id, s32 angle) {
  Actor_FaceDirectionById(id, angle);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline; as plain calls, 2 of 237
   * instructions of Func_020003b8 differ in tla-en. */
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
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
    FieldCamera_WaitForArrival();
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
    PaletteCycle_SetPausesInEvents(0);
    Func_020000c4();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
    FieldActor_StartRepeatedMotion(4, 3);
    Actor_SetAttachedEffect(4, 258);
    FieldEvent_Wait(40);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(0x30000, 0x6000);
    Camera_MoveWithinMapBounds_(131 << 19, 0, 174 << 16, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    object->update = 0;
    Event_PlaySound(80);
    Sound_WaitForJingle();
    FieldEvent_Wait(10);
    Camera_MoveWithinMapBounds_(246 << 18, -0x400000, 248 << 16, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    PaletteCycle_SetPausesInEvents(1);
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
  GameFlag_Set(514);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(124);
  /* FAKEMATCH(wrapper): forwarded through Camera_MoveWithinMapBounds_ above;
   * as plain calls, 63 of 292 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds_(194 << 18, -1, PIXELS(208), 1);
  FieldCamera_WaitForArrival();
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
    GameFlag_Set(2573);
  if ((u32)(spot - 2) <= 1) {
    FieldCamera_SetSpeed(0x40000, 0x8000);
    EventCamera_FollowActor(23, 1);
    FieldCamera_WaitForArrival();
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
    EventCamera_FollowActor(23, 1);
    FieldCamera_WaitForArrival();
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
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
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
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(6);
  MapGrid_CopyCellsLayered(49, 103, 4, 3, 49, 76);
  FieldEvent_Wait(6);
  Event_PlaySound(133);
  MapGrid_CopyCellsLayered(49, 100, 4, 3, 49, 76);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
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
  switch (Random16() & 7) {
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

/* VIEW(first): routine gData_0200558c as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA s32 gData_0200558c[],
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:604: `gData_0200558c'
 * undeclared (first use in this function). */
extern u32 gData_0200558c__first[] __asm__("gData_0200558c");
#define gData_0200558c gData_0200558c__first
/* VIEW(first): routine gData_020055ec as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA s32 gData_020055ec[],
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:606: `gData_020055ec'
 * undeclared (first use in this function). */
extern u32 gData_020055ec__first[] __asm__("gData_020055ec");
#define gData_020055ec gData_020055ec__first
/* VIEW(first): routine gData_02005644 as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA s32 gData_02005644[],
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:605: `gData_02005644'
 * undeclared (first use in this function). */
extern u32 gData_02005644__first[] __asm__("gData_02005644");
#define gData_02005644 gData_02005644__first
/* VIEW(first): routine gData_0200569c as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA s32 gData_0200569c[],
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:607: `gData_0200569c'
 * undeclared (first use in this function). */
extern u32 gData_0200569c__first[] __asm__("gData_0200569c");
#define gData_0200569c gData_0200569c__first

/// Places an actor at (x, z), in pixels, facing a direction.
#define PLACE(id, x, z, facing)                                                \
  {                                                                            \
    s32 facing_ = (facing);                                                    \
    Actor_SetPositionAndFacing(id, (x) << 16, (z) << 16, facing_);             \
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
  Actor_SetSpriteFlags(Actor_Get(1), 0);
  Actor_SetScript(1, gData_02005524);
  {
    s32 priority = 0x480;

    FieldTask_AddFrame((void (*)(void))Func_02000a78, priority);
  }
  Actor_SetAnimation(0, 38);
  Actor_SetAnimation(2, 31);
  Actor_SetAnimation(3, 9);
}

extern u32 gData_02005748[];
/* VIEW(first): routine gData_0200578c as this part declared it; the joined file
 * has another; tried as the definition's extern
 * DEFINE_G_DATA_020056C4(gData_020056c4, 424, 344)
 * DEFINE_G_DATA_020056C4(gData_02005728, 512, 384) s32 gData_0200578c[], it
 * does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:713: `gData_0200578c'
 * undeclared (first use in this function). */
extern u32 gData_0200578c__first[] __asm__("gData_0200578c");
#define gData_0200578c gData_0200578c__first
extern u32 gData_02005860[];
extern u32 gData_02005890[];
/* VIEW(first): routine gData_02005950 as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA
 * DEFINE_G_DATA_020057D0(gData_02005920, 5, 708, 328) LATE_DATA s32
 * gData_02005950[], it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:732: `gData_02005950'
 * undeclared (first use in this function). */
extern u32 gData_02005950__first[] __asm__("gData_02005950");
#define gData_02005950 gData_02005950__first
extern u32 gData_020049dc[];

static __inline__ struct Actor *I_Func_020048cc(s32 actor, s32 emote,
                                                s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; as
   * direct calls at its 42 sites, 171 of 1568 instructions differ in tla-en. */
  return (struct Actor *)Actor_ShowEmoteById(actor, emote, frames);
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
  EventCamera_FollowActor(5, 1);
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
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
  } else {
    Actor_SetAnimation(28, 4);
    FieldEvent_Wait(33);
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
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
    FieldCamera_WaitForArrival();
  } else {
    FieldCamera_MoveTo(PIXELS(950), PIXELS(-64), PIXELS(287), 1);
    FieldCamera_WaitForArrival();
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
  FieldCamera_WaitForArrival();
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
  Actor_SetPositionAndFacing(27, PIXELS(641), PIXELS(128), angle);
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
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(PIXELS(720), 0, PIXELS(210), 1);
  FieldCamera_WaitForArrival();
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
  EventCamera_FollowActor(4, 1);
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
    Actor_SetDestination(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(6, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(6, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(5, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(5, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Actor_SetAnimation(28, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(28, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
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
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
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
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6a3, 77);
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

void Func_02001e38(void) {
  PLACE(0, 761, 219, FACING_NORTHWEST);
  PLACE(2, 741, 232, FACING_NORTHWEST);
  Actor_SetSpriteFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 39);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
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
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

// The Japanese form differs and is still on its listing.

static __inline__ void Walk(s32 id, s32 speed, s32 frames) {
  /* FAKEMATCH(wrapper): the walk forwarded inline, so its constants stay out of
   * CSE; as direct calls, 103 of 1328 instructions differ in tla-en. */
  ((struct Actor *)Actor_ShowEmoteById(id, speed, frames));
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
  Actor_SetSpriteFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 0x27);
  Actor_SetSpriteFlags(Actor_Get(2), 0);
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
  EventCamera_GetActor()->motionFlags = 0;
  EventCamera_FollowActor(4, 1);
  FieldEvent_Wait(0x14);
  FieldActor_LaunchFromLeader(6, 0, 0, 0x4000);
  EventCamera_FollowActor(6, 1);
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
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
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
  EventCamera_FollowActor(6, 1);
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
  EventCamera_FollowActor(4, 1);
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

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 516;
  HangingVine_Place(8, 1);
  HangingVine_Place(9, 1);
  HangingVine_Place(10, 1);
  HangingVine_Place(11, 1);
  HangingVine_Place(12, 1);
  HangingVine_Place(13, 1);
  HangingVine_Place(14, 1);
  HangingVine_Place(15, 1);
  Rope_Initialize();
  Rope_Place(0, 512, 21, 22);
  gWhirlwindRiding = 0;
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
    FieldMapGrid_SetCellBehavior(0x4100000, 0xa00000, 2, 236);
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

#include "shared/scene_set_arrival_flags.c"

/* VIEW(first): routine gPaletteCyclePausesInEvents as this part declared it;
 * the joined file has another; tried as the definition's extern s32
 * gPaletteCyclePausesInEvents, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:1735:
 * `gPaletteCyclePausesInEvents' undeclared (first use in this function). */
extern s32
    gPaletteCyclePausesInEvents__first __asm__("gPaletteCyclePausesInEvents");
#define gPaletteCyclePausesInEvents gPaletteCyclePausesInEvents__first

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

/* VIEW(first): routine gPaletteCycleFrame as this part declared it; the joined
 * file has another; tried as the definition's extern s32 gPaletteCycleFrame, it
 * does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:1744: `gPaletteCycleFrame'
 * undeclared (first use in this function). */
extern s32 gPaletteCycleFrame__first __asm__("gPaletteCycleFrame");
#define gPaletteCycleFrame gPaletteCycleFrame__first

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }
extern u16 gPaletteCycleColors[];

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell,
                             Func_02004774__jupiter_lighthouse_room_m,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

/* VIEW(first): routine gWhirlwindEndScript as this part declared it; the joined
 * file has another; tried as the definition's extern u32 gWhirlwindEndScript[],
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:1755: `gWhirlwindEndScript'
 * undeclared (first use in this function). */
extern const s32 gWhirlwindEndScript__first[] __asm__("gWhirlwindEndScript");
#define gWhirlwindEndScript gWhirlwindEndScript__first

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gWhirlwindEndScript,
                        gWhirlwindRiding)

#undef Whirlwind_SpawnUpper
#undef Actor_EmitRisingSparks
// ---- scene/jupiter_lighthouse_room_m/s3474.c ----

/// Scene code of overlay 0x6a3, in ROM order.

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Whirlwind_Update(struct Actor *self);
/* VIEW(s3474): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another; tried as the definition's void FieldEffect_Spawn(s32 x, s32
 * y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const
 * struct EffectOptions *options), which the object already declares,
 * Whirlwind_SpawnUpper moves: 1 of 90 instructions in tla-ja. */
void FieldEffect_Spawn__s3474(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s3474

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Import_ActorSelectAnimation,
                             Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern s16 gLightSpritesLarge;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef WORD_AT
#undef PLACE
#undef EVENT_WORK
#undef ACTOR_SET_MODE_AND_WAIT
#undef Whirlwind_SpawnUpper
#undef Actor_EmitRisingSparks
#undef gData_0200558c
#undef gData_020055ec
#undef gData_02005644
#undef gData_0200569c
#undef gData_0200578c
#undef gData_02005950
#undef gPaletteCyclePausesInEvents
#undef gPaletteCycleFrame
#undef gWhirlwindEndScript
#undef FieldEffect_Spawn
// @file
// Scene code of overlay 0x6a3, in ROM order.

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

// ---- scene/jupiter_lighthouse_room_m/s41ac.c ----

// Scene code of overlay 0x6a3, in ROM order.

struct CellTargetEntry;

/* VIEW(s41ac): tag struct CellTarget as struct CellTarget__s41ac; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s41ac {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

/* VIEW(s41ac): routine CrackedTile_Fall as void CrackedTile_Fall(struct
 * CellTarget__s41ac *table, struct CellTargetEntry *entry);
 * module/scene/jupiter_lighthouse_room_m.h has void CrackedTile_Fall(struct
 * CellTarget *table, struct CellTargetEntry *entry); tried as the definition's
 * DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell) void
 * CrackedTile_Fall(struct PushEvent *event), it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_m.c:1970: conflicting types for
 * `CrackedTile_Fall'. */
void CrackedTile_Fall__s41ac(
    struct CellTarget__s41ac *table,
    struct CellTargetEntry *entry) __asm__("CrackedTile_Fall");
#define CrackedTile_Fall CrackedTile_Fall__s41ac
DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s41ac,
                         Leader_CenterInCell)

#undef CrackedTile_Fall

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_StartScript
#undef CrackedTile_Fall
// @file
// Scene code of overlay 0x6a3, in ROM order.

/* VIEW(s4344): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another; tried as the definition's void FieldEffect_Spawn(s32 x, s32
 * y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const
 * struct EffectOptions *options), which the object already declares,
 * CrackedTile_Fall (gone) moves: 131 of 131 instructions in tla-ja. */
void FieldEffect_Spawn__s4344(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s4344

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

// clang-format off
u32 gData_020049cc[] = {
    1, 39, 1, 17,
};

u32 gData_020049dc[] = {
    1, 39, 1, 17,
};

u32 gData_020049ec[] = {
    1, 39, 1, 17,
};

u32 gData_020049fc[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gData_02004a18[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

u32 gData_02004a34[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

u32 gData_02004a50[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gData_02004a6c[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004a90[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1),
    SC_END,
};

DEFINE_G_DATA_02004AB4(gData_02004ab4, 699)

DEFINE_G_DATA_02004AB4(gData_02004ae8, 679)

DEFINE_G_DATA_02004AB4(gData_02004b1c, 659)
// clang-format on

#undef FieldEffect_Spawn

/* VIEW(data): routine Func_02000038 as this part declared it; the joined file
 * has another; tried as the definition's void Func_02000038(s16 *list),
 * gData_02004ab4 (gone) moves: 0 of 0 instructions in tla-ja. */
void Func_02000038__data(void) __asm__("Func_02000038");
#define Func_02000038 Func_02000038__data

extern u32 gFieldEffect_LongScript[];
extern s32 gData_02005078[];

// clang-format off
LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

LATE_DATA s32 gData_02005078[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA DEFINE_G_SCENE_EXITS_6(263, 256, 3, 256, 4, 4, 258, 261, 7, 260, 8, 257)

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    {373, CONDITION_ALWAYS, 1, PIXELS(24), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(1464), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(1608), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {317, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(648), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {317, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(680), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(936), 0, PIXELS(232), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(952), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {428, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(968), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(1144), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(1064), 0, PIXELS(184), FACING_SOUTH, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(200), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(200), 0, PIXELS(152), FACING_EAST, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_02005078, PIXELS(1016), 0, PIXELS(168), FACING_EAST, 2, 0},
    {0, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {1, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {2, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {3, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {4, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {33, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {23, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_02005078, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
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
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02000060)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02000080)},
    {TRIGGER_STEP, 32, 0x202, EVENT(Func_0200060c)},
    {TRIGGER_STEP, 33, IF_FLAG_SET(0x202), EVENT(Func_020008c8)},
    {TRIGGER_TALK, 0, IF_FLAG_SET(0xa23), EVENT(Func_02000970)},
    {TRIGGER_TALK, 2, IF_FLAG_SET(0xa23), EVENT(Func_02000970)},
    {TRIGGER_TALK, 6, IF_FLAG_SET(0xa23), EVENT(Func_02000970)},
    {TRIGGER_TALK, 5, IF_FLAG_SET(0xa23), EVENT(Func_02000970)},
    {TRIGGER_TALK, 28, IF_FLAG_SET(0xa23), EVENT(Func_02000970)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(Func_0200039c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT(Func_020003b8)},
    {8, -1, CONDITION_ALWAYS, EVENT(Func_0200039c)},
    {9, -1, CONDITION_ALWAYS, EVENT(Func_020003b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 21, 0x200, 0},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gData_02005524[] = {
#if !TLA_PATCH_1
    39, 14, 22, 26, 0, 23,
#else
    39, 14, 22, 26, 0, 22,
#endif
    6, 0xc0000, 17,
};

LATE_DATA u32 gData_02005548[] = {
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(9),
    SC_WAIT(120),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA s32 gData_0200558c[] = {
    SC_ARG(2),
    SC_CALL(Func_02000b38),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_MOVE_XZ(PIXELS(737), PIXELS(202)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(PIXELS(776), PIXELS(202)),
    SC_CALL(Func_02000ad0),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA s32 gData_020055ec[] = {
    SC_ARG(2),
    SC_CALL(Func_02000b38),
    SC_SYNC,
    SC_ARG(9),
    SC_WAIT(60),
    SC_MOVE_XZ(PIXELS(776), PIXELS(202)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(PIXELS(737), PIXELS(202)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA s32 gData_02005644[] = {
    SC_CALL(Func_02000b38),
    SC_MOVE_XZ(PIXELS(731), PIXELS(213)),
    SC_ARG(2),
    SC_WAIT(4),
    SC_MOVE_XZ(PIXELS(731), PIXELS(232)),
    SC_CALL(Func_02000ad0),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA s32 gData_0200569c[] = {
    SC_ARG(2),
    SC_CALL(Func_02000b38),
    SC_MOVE_XZ(PIXELS(731), PIXELS(232)),
    SC_WAIT(4),
    SC_MOVE_XZ(PIXELS(731), PIXELS(213)),
    SC_SYNC,
    SC_ARG(8),
    SC_WAIT(53),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA DEFINE_G_DATA_0200218C(gData_020056ec, 20, 20, 20, 20)

LATE_DATA u32 gData_02005748[] = {
    SC_ARG(5),
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(744), PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

LATE_DATA s32 gData_0200578c[] = {
    SC_ARG(5),
    SC_MOVE_BY(PIXELS(-8), 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(896), PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA DEFINE_G_DATA_020057D0(gData_020057d0, 2, 740, 312)

LATE_DATA DEFINE_G_DATA_02005800(gData_02005800, 2, 900)

LATE_DATA DEFINE_G_DATA_020057D0(gData_0200d830, 5, 724, 312)

LATE_DATA DEFINE_G_DATA_02005800(gData_02005860, 5, 916)

LATE_DATA u32 gData_02005890[] = {
    SC_ARG(5),
    SC_MOVE_XZ(PIXELS(677), PIXELS(287)),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(PIXELS(740), PIXELS(344)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gData_020058d8[] = {
    SC_ARG(5),
    SC_MOVE_XZ(PIXELS(950), PIXELS(287)),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(PIXELS(900), PIXELS(344)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA DEFINE_G_DATA_020057D0(gData_02005920, 5, 708, 328)

LATE_DATA s32 gData_02005950[] = {
    SC_ARG(5),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_ARG(5),
    SC_MOVE_XZ(PIXELS(924), PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_END,
};

LATE_DATA u32 gData_0200599c[] = {
    1, 38,
};

LATE_DATA u32 gData_020059a4[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(706), PIXELS(172)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(682), PIXELS(151)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(682), PIXELS(120)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(664), PIXELS(106)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(664), PIXELS(100)),
    SC_SYNC,
    SC_END,
};

LATE_DATA u32 gData_02005a00[] = {
    SC_MOVE_XZ(PIXELS(664), PIXELS(80)),
    SC_SYNC,
    SC_FREE,
};

LATE_DATA s32 gPaletteCycleFrame[] = {
    -1,
};

LATE_DATA u32 gPaletteCyclePausesInEvents[] = {
    1,
};

LATE_DATA u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef Func_02000038
