/// @file
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "version.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/frame.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "field/field_coordinate.h"
#include "owner_state.h"
#include "event_work.h"
#include "field_sprite.h"
#include "call.h"
#include "system/task.h"
#include "fixed_math.h"
#include "system/input.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "module/window/window.h"
#include "module/game/game.h"
#include "scene_ids.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "graphics/resource.h"
#include "bg_scroll.h"
#include "field/sprite.h"
#include "graphics/vram_tab.h"
#include "macros.h"
#include "shared/tables.h"
#include "text/msg_ids.h"

struct Actor;

/// FieldSprite's byte 0x11 read as its bits.
struct SpriteCacheBits {
  u8 retainCacheEntry : 1; ///< keeps the VRAM cache entry when it is released
};

struct WorldMapOam {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

void Func_020048f4(void);

void WorldMap_SetCountdownByZ(s32 a, s32 b, s32 c);

void Actor_SetFlightMode(struct Actor *object, s32 mode);

void Gauge_Init(void);
void Func_02003c3c(void);
void Gauge_End(void);

void Import_ActorSelectAnimation(struct Actor *object, s32 mode);

void WorldMap_FightBanditAndThieves(s32 entrance);

void WorldMap_DeliverLohoCannon(void);

void WorldMap_MeetEcho(void);

void Func_020029a4(void);

void WorldMap_DriftNorthOfContinent(void);

void Func_02002e00(void);

void WorldMap_TakeTheTiller(void);
void Func_0200310c(void);
void Func_02003490(void);

void Func_02003d44(s32 mode);

void Func_0200009c(void);

void Func_02000100(void);

void Func_02000164(void);

void Func_020001c8(void);

void Func_0200022c(void);

void WorldMap_SetUpFlyingShip(void);

void WorldMap_SetUpShip(void);
void Func_02000918(void);

void WorldMap_RefuseToRunAway(void);

void WorldMap_GazeIntoPond(void);

void Func_02000d1c(void);

void WorldMap_TakeOff(void);

extern s32 gData_02006320[];
extern s32 gData_02006578[];
extern s32 gData_02006584[];

extern u32 gData_02004fc8[];

s32 WorldMap_GetBlockPlace(s32 *position);

extern u16 gData_02006e08;

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void Func_02004634(struct Actor *object);

void Actor_Release(struct Actor *object);

extern u8 Data_02006e20[][32];
extern s16 gData_02006e10;
extern u32 Data_02006e14[3];

void Party_DrainPp(s32 value);
s32 Party_GetPpRatio(void);

void WorldMap_WatchVenusLighthouseCollapse(void);

s32 Event_PickUpItem(s32 a, s32 b, s32 c);
void Actor_StartRenderModeScript(void);
void WorldMap_Freeze(void);
void WorldMap_ReloadPendingAreaGraphics(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
s32 WorldMap_IsWalkable(struct Actor *object, s32 *position);
void Party_GivePiersDjinn(void);
void Event_WaitForZoomTransition(void);
void DjinnMenu_Run(void);
void Party_RestoreHpPp(s32 value);
void FieldCamera_WaitForArrival(void);
extern s32 Data_02006be8[8];

// Once the party has walked nine tenths of the encounter threshold, a coin
// toss either requests the event's battle now or makes the party walk the
// whole threshold first. The Japanese release has no render mode test.
#if !TLA_PATCH_1
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct EncounterWork *work;                                                \
    struct MainRoutineWork *party;                                             \
                                                                               \
    work = Ram_HeapSlots->eventWork;                                           \
    party = &gMainRoutineWork;                                                 \
    if (party->encounterSteps >= work->threshold * 9 / 10) {                   \
      if (Random16() < 0x8000) {                                               \
        Event_RequestDjinnBattle(a, b);                                        \
        work->bias = 0;                                                        \
      } else {                                                                 \
        party->encounterSteps = work->threshold;                               \
      }                                                                        \
    }                                                                          \
  }
#else
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct MainRoutineWork *party;                                             \
    struct EncounterWork *work;                                                \
                                                                               \
    party = &gMainRoutineWork;                                                 \
    work = Ram_HeapSlots->eventWork;                                           \
    if (party->renderMode == 1) {                                              \
      if (party->encounterSteps >= work->threshold * 9 / 10) {                 \
        if (Random16() < 0x8000) {                                             \
          Event_RequestDjinnBattle(a, b);                                      \
          work->bias = 0;                                                      \
        } else {                                                               \
          party->encounterSteps = work->threshold;                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }
#endif

void Func_02000038(void) { WORLD_MAP_TRY_ENCOUNTER(0x80a, 8) }

void Func_0200009c(void) { WORLD_MAP_TRY_ENCOUNTER(0x80b, 28) }

void Func_02000100(void) { WORLD_MAP_TRY_ENCOUNTER(0x80c, 31) }

void Func_02000164(void) { WORLD_MAP_TRY_ENCOUNTER(0x80d, 72) }

void Func_020001c8(void) { WORLD_MAP_TRY_ENCOUNTER(0x80e, 13) }

void Func_0200022c(void) { WORLD_MAP_TRY_ENCOUNTER(0x80f, 53) }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/* VIEW(world_map): routine Actor_SetSpriteColorMode as void
 * Actor_SetSpriteColorMode(struct Actor *object, s32 value); actor_view.h has
 * void Actor_SetSpriteColorMode(struct Actor *, u32); tried as the definition's
 * void Actor_SetSpriteColorMode(struct Actor *object, u32 value),
 * WorldMap_MeetEcho moves: 1 of 714 instructions in tla-ja. */
void Actor_SetSpriteColorMode__world_map(
    struct Actor *object, s32 value) __asm__("Actor_SetSpriteColorMode");
#define Actor_SetSpriteColorMode Actor_SetSpriteColorMode__world_map
u32 __umodsi3(u32 a, u32 b);

#include "shared/tile_sprite_reset.h"

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): through the inline the multiply's address loads between
   * the halves of 0x40000. */
  return Iwram_MulQ16(a, b);
}

/// The marker blinks and bobs up and down on a circle.
// As The Broken Seal's WorldMap_UpdateBobbingMarker.
void WorldMap_UpdateBobbingMarker(struct BobbingMarker *marker) {
  s16 *angle;
  s32 y;
  s32 z;
  s32 radius;
  s32 rise;

  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(marker, 10);
  else
    Actor_SetSpriteColorMode(marker, 7);
  if (marker->stopped == 0) {
    angle = &marker->angle;
    marker->x.fixed = px(10120);
    y = MulQ16(Math_Sin(*angle << 3), 0x40000);
    z = 0x20a40000;

    rise = 0x100000;
    marker->y.fixed = y + rise;
    marker->z.fixed = z;
    radius = rise;
    FieldVector_AddPolarOffset(radius, *angle, &marker->x);
    marker->facing = *angle + 0x4000;
    *angle += 0x400;
  }
}

s32 StoryActor_Initialize(struct Actor *actor) {
  if (Event_GetFlag(0x37) != 0) {
    return 0;
  }
  if (Event_GetFlag(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  actor->update = (void (*)(void))WorldMap_UpdateBobbingMarker;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->unk66 = 0;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  return 0;
}

s32 Func_02000388(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

s32 Func_020003a8(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

/// With flag 0x200 set the object is released, takes acceleration and speed
/// limit 0x1000 and is sent to an offset of its position.
s32 Func_020003b4(struct Actor *object) {
  s32 x, z;

  if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
    return 1;
  Actor_SetSpriteFlags(object, 0);
  x = object->x;
  object->acceleration = px(1) / 16;
  object->speedLimit = px(1) / 16;
  z = object->z;
  FieldActor_SetMoveTargetFixed(object, x - 0x1e0000, object->y, z + 0x80000);
  return 0;
}

extern struct ActorPlacement gData_02006338[];
extern struct ActorPlacement gData_02006500[];
extern struct ActorPlacement gData_02006598[];
extern struct ActorPlacement gData_02006700[];
extern struct ActorPlacement gData_02006790[];

void Leader_Search(void);

const struct ActorPlacement *Scene_GetPlacements(void) {
  switch (gMainRoutineWork.entrance) {
  case 64:
    return gData_02006500;
  case 65:
    return gData_02006598;
  case 68:
    if (!Event_GetFlag(0x8de))
      return gData_02006700;
  case 77:
    return gData_02006790;
  default:
    return gData_02006338;
  }
}

/// The world map's render mode 8 start: with flag 0x147 a blank message is
/// shown first, the event work's counters are cleared.
void WorldMap_SetUpFlyingShip(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  gMainRoutineWork.renderMode = 8;
  if (Event_GetFlag(0x147)) {
    WorldMap_Freeze();
    MessageArgument_Push(153, 4);
    Message_ShowCentered((s32)MsgBlank2, 4);
  }
  if (Actor_Get(FieldParty_GetLeaderActor()) != NULL)
    Actor_StartRenderModeScript();
  Leader_Search();
  if (Event_GetFlag(0x147))
    work[3387] = 0;
  else
    work[3387] = 1;
  if (Event_GetFlag(0x147)) {
    Message_CloseFinishedWindows();
    WorldMap_ReloadPendingAreaGraphics();
  }
  Gauge_Init();
  {
    s32 zero;

    /* FAKEMATCH(temporary): the halfword counters take an s32 zero assigned
     * inside the first store. */
    *(u16 *)(work + 346) = zero = 0;
    *(u16 *)(work + 350) = zero;
    *(u16 *)(work + 348) = zero;
    *(u16 *)(work + 356) = zero;
    *(u16 *)(work + 358) = zero;
  }
}

/// Sets render mode 7, clears object 8's child flags, then adds up the PP of
/// the active party; when it is 0, the current owner gets 1 PP back.
void WorldMap_SetUpShip(void) {
  s32 count;
  s32 pp;
  s32 i;

  gMainRoutineWork.renderMode = 7;
  Func_02003c3c();
  Gauge_End();
  Actor_SetSpriteFlags(Actor_Get(8), 0);
  count = Party_CountMembers();
  pp = 0;
  i = 0;
  if (pp < count) {
  loop:
    // One member's PP.
    pp += ((struct BattleUnit *)Unit_Get(gMainRoutineWork.activeOwners[i]))->pp;
    i++;
    if (i < count)
      goto loop;
  }
  if (pp == 0)
    Unit_AddPp(gMainRoutineWork.currentOwner, 1);
}

void Actor_StartIdle(struct Actor *object);

/// The leader jumps aboard the ship, actor 8, when close enough.
void WorldMap_BoardShip(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  struct Actor *ship;
  s32 kind, dx, dz, distance;

  if (GameFlag_Get(0x8de) == 0)
    return;
  kind = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ship = Actor_Get(8);
  dx = (leader->x - ship->x) >> 16;
  dz = (leader->z - ship->z) >> 16;
  distance = dx * dx + dz * dz;
  if (kind != 8 && distance > 255)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(0x176);
  gMainRoutineWork.savedX = ship->x;
  gMainRoutineWork.savedZ = ship->z;
  gMainRoutineWork.unk280 = ship->facing;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Import_ActorSelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SetSpriteFlags(leader, 0);
  Import_ActorSelectAnimation(leader, 7);
  leader->speedLimit = px(3);
  leader->acceleration = px(2);
  leader->velocityY = px(4);
  leader->terrainHeight = px(4);
  Actor_StartIdle(leader);
  FieldActor_SetMoveTargetFixed(leader, ship->x, 0, ship->z);
  FieldActor_WaitForMove(leader);
  Import_ActorSelectAnimation(leader, 6);
  System_Wait(6);
  leader->terrainHeight = 0;
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  Scene_ChooseMusic();
  Scene_PlayMusic();
  Event_End();
  *(s16 *)work->unk15c = 0;
#if TLA_PATCH_1
  work->exit = 0;
#endif
  FieldEffect_ScheduleRatioTransition(0x10000, 30);
}

/// The leader jumps ashore from the ship, where the shore ahead lets them land.
void WorldMap_LeaveShip(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ship = Actor_GetById(8);
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 cell = WorldMap_GetCollision(&ship->x);
  s32 position[3];
  s32 shore = 0;
  s32 blocked;
  u8 *block;
  u8 *blocks = gMapBlocks;
  u8 flags;

  position[0] = ship->x;
  position[1] = ship->y;
  position[2] = ship->z;
  Vector_AddPolarOffset(0x180000, ship->facing, position);
  blocked = 0;
  block = blocks + ((((position[0] / 0x200000) & 31) +
                     ((position[2] / 0x200000) & 31) * 32) *
                    4);
  if (cell > 12) {
    blocked = 1;
  } else {
    if ((u32)(cell - 1) <= 2)
      shore = 1;
    if ((block[3] & 64) == 0 && shore != 0) {
      blocked = 1;
    } else {
      position[0] += 0x60000;
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += -0xc0000;
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += 0x60000;
      position[1] += 0x60000;
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += -0xc0000;
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += 0x60000;
      blocked |= WorldMap_IsWalkable(ship, position);
    }
  }
  if (blocked == 0) {
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEffect_ScheduleRatioTransition(0x10000, 10);
    gMainRoutineWork.renderMode = 1;
    gMainRoutineWork.ownerOverride = 0;
    Event_ClearFlag(0x176);
    gMainRoutineWork.savedX = ship->x;
    gMainRoutineWork.savedZ = ship->z;
    gMainRoutineWork.unk280 = ship->facing;
    flags = leader->motionFlags;
    leader->x = ship->x;
    leader->y = px(4);
    leader->z = ship->z;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(leader, 0);
    Import_ActorSelectAnimation(leader, 6);
    System_Wait(6);
    Event_PlaySound(SE_JUMP);
    Import_ActorSelectAnimation(leader, 7);
    leader->speedLimit = px(3);
    leader->acceleration = px(2);
    leader->velocityY = px(4);
    leader->motionFlags = flags;
    Actor_StartIdle(leader);
    FieldActor_SetMoveTargetFixed(leader, position[0], 0, position[2]);
    FieldActor_WaitForMove(leader);
    Import_ActorSelectAnimation(leader, 6);
    System_Wait(6);
    Event_End();
    Scene_ChooseMusic();
    Scene_PlayMusic();
    *(s16 *)work->unk15c = 0;
  }
}

void Actor_MoveToObject(s32 actor, s32 target);

/// Back to the map: actor 8's position is kept in gMainRoutineWork.
void Func_02000918(void) {
  struct EventDestination *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(8);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.renderMode == 8)
    WorldMap_SetUpShip();
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.savedX = leader->x;
  gMainRoutineWork.savedZ = leader->z;
  gMainRoutineWork.unk280 = 0;
  gMainRoutineWork.ownerOverride = 0;
  Event_ClearFlag(0x176);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(work->scene);
}

/// Brings the actor to the player character's position.
static __inline__ void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetAnimation(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(actor, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetPosition(actor, 0, 0);
}

void WorldMap_RefuseToRunAway(void) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 southwest = FACING_SOUTHWEST;
  s32 west = FACING_WEST;

  if (Event_GetFlag(0xa21) && !Event_GetFlag(0x9d0)) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    x = object->x;
    z = object->z;
    x >>= 20;
    z >>= 20;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0x9b3)) {
      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      Actor_WalkToAndWait(5, (x << 4) + 24, (z << 4) - 12);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                 FACING_NORTHEAST);
      WorldMap_Freeze();
      FieldEvent_SetMessageId((s32)MsgWeShouldnT);
      Event_ShowMessage(5, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    } else {
      s32 besideZ;

      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_WalkToAndWait(5, x + 24, besideZ);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                 FACING_NORTHEAST);
      WorldMap_Freeze();
      FieldEvent_SetMessageId((s32)MsgYouReNot);
      Event_ShowMessage(5, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_FaceDirection(5, west, 0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
      Actor_SetAnimation(5, 4);
      WorldMap_Freeze();
      Event_ShowMessage(5, 0);
      Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
      Event_ShowMessage(5, 0);
      Actor_SetAnimation(5, 3);
      Event_ShowMessage(5, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      Event_SetFlag(0x9b3);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    }
    System_Wait(8);
    Event_End();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(work->scene);
  }
}

void WorldMap_GazeIntoPond(void) {
  u32 roll = (Random16() << 4) >> 16;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgGazedIntoThe, 1);
  if (roll == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    s32 slot = 79;
    struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
    u8 *unit = Unit_Get(gMainRoutineWork.currentOwner);
    s32 amount = (((unit[15] * Random16()) << 1) >> 16) + 0x8001;
    s32 position[3];
    s32 x;
    /* FAKEMATCH(pin): object in r6, which leaves r7 to the amount. */
    register struct Actor *object asm("r6");

    Message_ShowCentered((s32)MsgThereAreSome, 1);
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    FieldVector_AddPolarOffset(px(16), leader->facing, position);
    object = Actor_Get(79);
    x = position[0];
    if (object == NULL) {
      object = FieldActor_Create(0x1e9, x, position[1], position[2]);
      if (object == NULL)
        return;
      work->objects[slot] = object;
    } else {
      object->x = x;
      object->y = position[1];
      object->z = position[2];
    }
    FieldActor_ShowItemIconById(79, amount);
    /* FAKEMATCH(dead): the store and the call in a once-only block. */
    do {
      *(u16 *)((u8 *)work + 0xd34) = amount;
      Scene_TalkToActor(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    MapGrid_SetShake(3, 5, 1);
    MapGrid_SetShakeFixed((s32)(-1), (s32)(-1), (s32)(0xe666));
    FieldEvent_Wait(30);
    Message_ShowCentered((s32)MsgTheBoatS, 1);
  } else {
    Message_ShowCentered((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Event_End();
}

void Func_02000d1c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Value3((s32 (*)())Event_PickUpItem, 19, 0x198, -1) == 0)
    Event_ClearFlag(0xa77);
  Event_End();
}

void Func_02000d4c(void) {
  struct Actor *object;
  u8 *animation;
  s32 a, id;

  FieldActor_ShowItemIconById(19, ITEM_GOLDEN_BOOTS);
  object = Actor_Get(19);
  animation = object->sprite;
  animation[17] |= 1;
  Actor_SetSpriteFlags(Actor_Get(19), 0);
  a = 0x4000;
  id = 19;
  Actor_SetPositionAndFacing(id, px(10168), px(9648), a);
  Event_SetFlag(0xa77);
}

void Func_02000d98(s32 id) { WorldMap_SetCountdownByZ(id, 63, 62); }

void Func_02000da4(s32 id) { WorldMap_SetCountdownByZ(id, 34, 33); }

void Func_02000db0(s32 id) { WorldMap_SetCountdownByZ(id, 3, 2); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern s32 gData_0200632c[];

s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 && GameFlag_Get(0x8de) != 0 &&
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.savedX != 0) {
    struct Actor *ship = Actor_GetById(8);

    ship->x = gMainRoutineWork.savedX;
    ship->z = gMainRoutineWork.savedZ;
    ship->facing = gMainRoutineWork.unk280;
    ship->timer = 0;
    ship->unk66 = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    FieldEffect_ScheduleRatioTransition(0x10000, 1);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  FieldCamera_SetSpeed(8);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  WorldMap_StartPlacedActors();
  if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE))
    Field_SetWorldMapWindowCellTile(0, 240, 336, 288);
  if (GameFlag_Get(0xa21)) {
    Field_SetWorldMapWindowCellTile(0, 224, 208, 256);
    Field_SetWorldMapWindowCellTile(16, 224, 224, 256);
  }
  if (GameFlag_Get(0xa77))
    Func_02000d4c();
  switch (gMainRoutineWork.entrance) {
  case 39:
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      GameFlag_Set(0x9ab);
    break;
  case 9:
  case 73:
  case 74:
    if (GameFlag_Get(0xa76))
      WorldMap_FightBanditAndThieves(gMainRoutineWork.entrance);
    break;
  case 77:
    Func_020029a4();
    break;
  case 78:
    Func_02003490();
    break;
  case 59:
    if (!Event_GetFlag(0x90a) && GameFlag_Get(0xa5f) != 0) {
      GameFlag_Set(0x90a);
      WorldMap_DeliverLohoCannon();
    }
    break;
  case 64:
    WorldMap_WatchVenusLighthouseCollapse();
    break;
  case 65:
    WorldMap_DriftNorthOfContinent();
    break;
  case 70:
    Party_GivePiersDjinn();
    break;
  case 2:
    if (GameFlag_Get(55) == 0) {
      Actor_SetScript(9, gData_0200632c);
      WorldMap_MeetEcho();
    }
    break;
  case 67:
    Func_02002e00();
    break;
  case 68:
    WorldMap_TakeTheTiller();
    break;
  case 69:
    Func_0200310c();
    break;
  case 72:
    Func_02003d44(2);
    FieldActor_SetSpriteColor(Actor_GetById(7), 15);
    Actor_SetPosition(7, 7277, 7300);
    wait(1);
    FieldEvent_SetMessageId((s32)MsgIVeHeard);
    Event_ShowMessage(7, 0);
    FieldEvent_Wait(20);
    Event_ShowMessage(7, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_SetDestination((s32)&SceneId_JupiterLighthouseAerieBeacon, 9);
    break;
  case 98:
    gMainRoutineWork.ownerOverride = 1;
    Event_SetFlag(0x8de);
    Event_SetFlag(FLAG_SHIP_FLIES);
    Event_SetFlag(FLAG_DEBUG_STEPS_OFF);
    Event_SetFlag(0x11d);
    Event_SetDestination((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_Set(FLAG_DEBUG_STEPS_OFF);
    GameFlag_Set(0x11d);
    break;
  }
  return 0;
}

void Menu_AnimateSelectionToEntry(s32 a, s32 b);

s32 Scene_PrepareMap(void) { return 0; }

/// The djinn tutorial. With flag 0x175 set each message is offset by
/// MsgDjinnGiveTutorialB less A.
// As The Broken Seal's WorldMap_ShowDjinnTutorial.
void WorldMap_ExplainGivingDjinn(void) {
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(0x10000, 6);
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_SetMessageId(Event_GetFlag(0x175) * ((s32)MsgDjinnGiveTutorialB -
                                                  (s32)MsgDjinnGiveTutorialA) +
                          (s32)MsgAfterBringingA);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  FieldActor_Jump(9, 4, 30);
  {
    s32 flag = Event_GetFlag(0x175);
    s32 base = (s32)MsgNextILl;

    FieldEvent_SetMessageId(
        flag * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +
        base);
  }
  Event_ShowMessage(9, 0);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  FieldEvent_Wait(30);
  Field_RequestBattleFromEncounterTable(9, 5);
}

#define ARM_WEST(id)                                                           \
  do {                                                                         \
    /* FAKEMATCH(barrier): the native copies the angle before it loads the     \
     * actor id. */                                                            \
    s32 a1 = west;                                                             \
                                                                               \
    asm volatile("" : "+l"(a1));                                               \
    Actor_FaceDirection(id, a1, 0);                                            \
  } while (0)

/// The bandit and the thieves from Vault, after a linked game, want Isaac.
void WorldMap_FightBanditAndThieves(s32 entrance) {
  s32 east, west, east74, eastB, westB;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (entrance == 9) {
    west = 0xc000;
    Actor_SetPositionAndFacing(20, px(10168), px(9000), west);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgHeyWaitA);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(0x18ccc, 16);
    Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_WalkByAndWait(4, 0, 32);
    Actor_FaceDirection(4, west, 0);
    Event_WaitForZoomTransition();
    Actor_WaitForMoveById(4);
    WorldMap_Freeze();
    {
      /* FAKEMATCH(barrier): the native copies the angle before the other
       * arguments. */
      s32 a3 = west;

      asm volatile("" : "+l"(a3));
      Actor_LaunchFromLeader(23, 16, 0, a3);
    }
    Actor_RefreshSelectorById(23);
    Actor_FaceDirectionAndWait(23, west);
    east = 0x4000;
    Event_Wait(10);
    Actor_SetPositionAndFacing(20, px(10168), px(9600), east);
    Actor_SetPositionAndFacing(21, px(10168), px(9600), east);
    Actor_SetPositionAndFacing(22, px(10168), px(9600), east);
    Actor_WalkBy(20, 0, 16);
    Actor_WalkBy(21, -16, 16);
    Actor_WalkByAndWait(22, 16, 16);
    Actor_FaceDirection(20, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(21, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(22, FACING_SOUTH_SOUTHEAST, 0);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_WaitForMoveById(20);
    Actor_WaitForMoveById(21);
    Actor_WaitForMoveById(22);
    WorldMap_Freeze();
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    ARM_WEST(4);
    {
      /* FAKEMATCH(barrier): the native copies the angle, loads the 0 and then
       * the actor. */
      s32 a1 = west, a2;

      asm volatile("" : "+l"(a1));
      a2 = 0;
      Actor_ArmCallback(23, a1, a2);
    }
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    ARM_WEST(4);
    ARM_WEST(23);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_WalkBy(4, 8, -8);
    Actor_WalkBy(23, -8, 8);
    Actor_WaitForMoveById(4);
    Actor_WaitForMoveById(23);
    ARM_WEST(4);
    ARM_WEST(23);
    {
      /* FAKEMATCH(barrier): the native starts 0x102, loads the 30, shifts, and
       * loads the actor last. */
      s32 a1 = 129, a2;

      asm volatile("" : "+l"(a1));
      a2 = 30;
      /* FAKEMATCH(barrier): the asm on the 30, the same call's second. */
      asm volatile("" : "+l"(a2));
      a1 <<= 1;
      Actor_ShowEmote(4, a1, a2);
    }
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(dead): the native loads the scene's id after this store. */
    do {
    } while (0);
    {
      s32 worldMap = (s32)&SceneId_WorldMap;

      FieldEvent_SetAfterBattleDestination(worldMap, 73);
      Party_SetFields1f2And1f4(worldMap, 74);
    }
    WorldMap_ReloadPendingAreaGraphics();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Field_RequestBattleFromEncounterTable(102, 3);
  } else if (entrance == 74) {
    Event_ClearFlag(0xa76);
    east74 = 0x4000;
    Actor_SetPositionAndFacing(20, px(10168), px(9616), east74);
    Actor_SetPositionAndFacing(21, px(10152), px(9616), east74);
    Actor_SetPositionAndFacing(22, px(10184), px(9616), east74);
    {
      s32 a3 = 0xe000;

      Actor_SetPositionAndFacing(23, px(10152), px(9664), a3);
    }
    Actor_SetPositionAndFacing(4, px(10168), px(9648), FACING_NORTHWEST);
    EventCamera_FollowActor(4, 0);
    FieldEffect_ScheduleRatioTransition(0x18ccc, 1);
    Actor_SetAnimation(4, 19);
    Actor_SetSpriteFlags(Actor_Get(4), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgItLooksLike);
    Actor_RunRepeatedMotion(4, 2);
    Event_Wait(5);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(21, east74);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_FaceDirectionAndWait(22, east74);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(20, east74);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Event_Wait(40);
    Event_SetDestination(
        ((struct SavedDestination *)&gMainRoutineWork)->scene,
        ((struct SavedDestination *)&gMainRoutineWork)->entrance);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  } else {
    Event_ClearFlag(0xa76);
    eastB = 0x4000;
    Actor_SetPositionAndFacing(20, px(10168), px(9632), eastB);
    westB = 0xc000;
    Actor_SetPositionAndFacing(21, px(10152), px(9632), eastB);
    Actor_SetPositionAndFacing(22, px(10184), px(9632), eastB);
    Actor_SetPositionAndFacing(23, px(10160), px(9680), westB);
    Actor_SetPositionAndFacing(4, px(10176), px(9680), westB);
    EventCamera_FollowActor(4, 0);
    FieldEffect_ScheduleRatioTransition(0x18000, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgWeLostAgain);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(21, FACING_EAST);
    Event_Wait(10);
    Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_FaceDirectionAndWait(20, FACING_WEST);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(22, FACING_WEST);
    Event_Wait(10);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_FaceDirection(20, eastB, 0);
    Actor_FaceDirection(21, eastB, 0);
    Actor_FaceDirection(22, eastB, 0);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(20, 3);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(10);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_Wait(40);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Actor_FaceDirection(22, FACING_WEST, 0);
    Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_FaceDirectionAndWait(20, eastB);
    Event_Wait(20);
    Event_OpenMessage(20, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldEvent_ShowMessageThenWait(20, 0, 5);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      FieldEvent_ShowMessageThenWait(20, 0, 5);
    }
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(40);
    Actor_FaceDirection(4, FACING_NORTH, 0);
    Actor_FaceDirection(23, FACING_NORTH, 0);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_RunRepeatedMotion(21, 2);
    Actor_FaceDirection(21, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(22, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Func_02000d4c();
    Event_Wait(5);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Event_Wait(40);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(20, 22, 0);
    Event_Wait(40);
    Actor_SetAnimation(22, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Actor_FaceDirection(21, FACING_SOUTH, 0);
    Actor_FaceDirection(22, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(20, FACING_SOUTH);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_WalkBy(20, -32, 0);
    Actor_WalkBy(21, -32, 0);
    Actor_WalkByAndWait(22, -32, 0);
    Actor_WalkBy(20, -64, -64);
    Actor_WalkBy(21, -64, -64);
    Actor_WalkByAndWait(22, -64, -64);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(20, FACING_SOUTH);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(23, FACING_EAST);
    Actor_FaceDirectionAndWait(4, FACING_WEST);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(4, 3);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    Actor_SetSpeed(23, 1.2);
    Actor_SetAnimation(23, 2);
    {
      struct Actor *object = Actor_Get(4);

      if (object != NULL)
        Actor_SetDestination(23, *(s16 *)((u8 *)object + 10),
                             *(s16 *)((u8 *)object + 18));
    }
    Actor_WaitForMoveById(23);
    Actor_SetPosition(23, 0, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(0x10000, 15);
    Event_End();
  }
}

extern u32 gData_02004e38[];
extern u32 gData_02004e58[];
extern u32 gData_02004e88[];

void WorldMap_DeliverLohoCannon(void) {
  s32 angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(16)->motionFlags = 2;
  Actor_Get(17)->motionFlags = 2;
  Actor_Get(18)->motionFlags = 2;
  /* FAKEMATCH(temporary): the facing angle held in a local is built once and
   * shared by the three placements. */
  angle = 0xe000;
  Actor_SetPositionAndFacing(16, px(8913), px(5853), angle);
  Actor_SetPositionAndFacing(17, px(8929), px(5869), angle);
  Actor_SetPositionAndFacing(18, px(8929), px(5885), angle);
  Event_StartScreenTransition();
  Actor_SetSpeed(16, 0.2);
  Actor_SetSpeed(17, 0.2);
  Actor_SetSpeed(18, 0.2);
  Actor_WalkBy(16, 10, 0);
  Actor_WalkBy(17, 10, 0);
  Actor_WalkBy(18, 10, 0);
  Event_WaitTransitionFrames();
  FieldEffect_ScheduleRatioTransition(0x13333, 6);
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  FieldActor_WaitForMoveById(16);
  FieldEvent_Wait(40);
  Actor_SetAnimation(16, 3);
  FieldEvent_SetMessageId((s32)MsgYouDonT);
  Actor_FaceDirection(17, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(60);
  FieldActor_WatchAndSetScript(4, 18);
  Actor_SetSpeed(16, 0.4);
  Actor_SetSpeed(17, 0.4);
  Actor_SetSpeed(18, 0.4);
  Actor_SetScript(16, gData_02004e38);
  FieldEvent_Wait(40);
  Actor_SetScript(17, gData_02004e58);
  Actor_SetScript(18, gData_02004e88);
  FieldActor_RefreshSelectorById(16);
  FieldActor_RefreshSelectorById(17);
  FieldActor_RefreshSelectorById(18);
  WorldMap_ReloadPendingAreaGraphics();
  FieldEvent_Wait(30);
  FieldEffect_ScheduleRatioTransition(0x10000, 6);
  Event_End();
}

/// A tutorial message: the second set (from MsgDjinnGiveTutorialB) when
/// secondSet is 1.
#define TUTORIAL_SET_MESSAGE(secondSet, base)                                  \
  ((secondSet) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +   \
   (s32)(base))

/// A tutorial message: the second set while flag 0x175 is set.
#define TUTORIAL_MESSAGE(base) TUTORIAL_SET_MESSAGE(Event_GetFlag(0x175), base)

/// The first Djinni, Echo, joins south of Daila, however often he is refused.
void WorldMap_MeetEcho(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 x;
  s32 z;
  s32 owner;
  s32 count;
  s32 i;
  s32 flag;
  // Holds 0 for its whole life; see its one use.
  s32 zero = 0;

  djinni = Actor_GetById(9);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  x = (leader->x - px(10120)) / 2 + 0x27880000;
  z = (leader->z - px(8356)) / 2 + 0x20a40000;
  /* FAKEMATCH(temporary): flag holds the id 0x16e, then the test's result, then
   * flag 0x175's result for MsgHey2. */
  flag = 0x16e;
  flag = GameFlag_Get(flag);
  if (flag == 0) {
    if (gInput.held & KEY_UP)
      Event_SetFlag(0x175);
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Party_RemoveMember(0);
    Party_RemoveMember(1);
    Party_RemoveMember(2);
    Party_RemoveMember(3);
    Party_RemoveMember(4);
    Party_RemoveMember(5);
    Party_RemoveMember(6);
    Party_RemoveMember(7);
    Party_AddMember(4);
    Party_AddMember(5);
    Party_AddMember(6);
    owner = gMainRoutineWork.currentOwner;
    Party_RestoreHpPp(1);
    Event_SetFlag(FLAG_DJINN_LESSON);
    Event_Start();
    Event_ResolvePendingActions(0);
    Djinn_Give(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    WorldMap_Freeze();
    Actor_FaceActorById(owner, 9, 0);
    Event_Wait(10);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    {
      s32 one = 1;
      *(u16 *)&djinni->unk66 = one;
    }
    Actor_FaceActorById(9, owner, 0);
    wait(16);
    flag = Event_GetFlag(0x175);
    Event_SetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey2));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(0x13333, 6);
    Event_WaitForZoomTransition();
    WorldMap_Freeze();
    djinni->motionFlags = 2;
    djinni->gravity = px(1) / 4;
    djinni->speedLimit = px(1);
    djinni->acceleration = px(1);
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    /* FAKEMATCH(barrier): an empty asm that reads and writes all of djinni. */
    asm("" : "+m"(*djinni));
    FieldActor_SetMoveTarget(djinni, 10120, 0, 8356);
    while (i >= 0) {
      djinni->scaleX += 0x800;
      djinni->scaleY += 0x800;
      wait(1);
      i--;
    }
    Actor_FaceActorById(9, owner, 0);
    Actor_FaceActorById(owner, 9, 0);
    wait(16);
    djinni->update = 0;
    Actor_SetSpriteColorMode(djinni, 0);
    djinni->gravity = px(1);
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
      Actor_Jump(9, 2, 20);
      Actor_Jump(9, 2, 20);
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren));
      FieldMessage_ShowAtActor(9, 0);
      if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
        Event_ShowMessageAndWait(9, 0);
      } else {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
        Event_ShowMessageAndWait(9, 0);
      }
    } else {
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeYouKnow));
      Event_ShowMessageAndWait(9, 0);
    }
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_ShowMessageAndWait(9, 0);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_ShowMessageAndWait(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(9, 2, 30);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 0;
    FieldActor_SetMoveTargetFixed(djinni, x, px(16), z);
    while (i >= 0) {
      *(u16 *)&djinni->facing += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 1);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 2;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    i = 7;
    while (i >= 0) {
      *(u16 *)&djinni->facing += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 22);
    Event_ShowMessageAndWait(9, 0);
    Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_FaceActorById(9, owner, 0);
    Motion_SetVarCbAndRefresh(9, 2);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(9, 2, 30);
    FieldMessage_ShowAtActor(9, 0);
    count = 0;
  plead:
    {
      i = Event_AskYesNoWithActorAnimation(owner, 0);

      if (count == 7) {
        if (i == 0) {
          Actor_Jump(9, 2, 20);
          Actor_Jump(9, 2, 20);
          Event_SetMessageId(TUTORIAL_MESSAGE(MsgOhAllRight));
          Event_ShowMessageAndWait(9, 0);
          goto joined;
        }
      } else if (i == 1) {
        Actor_Jump(9, 2, 20);
        Actor_Jump(9, 2, 20);
        Event_SetMessageId(count + TUTORIAL_MESSAGE(MsgOhComeOn));
        FieldMessage_ShowAtActor(9, 0);
        count++;
        goto plead;
      }
    }
    Actor_SetModeById(owner, 22);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 4, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgIMSo));
    Event_ShowMessageAndWait(9, 0);
  joined:
    MessageArgument_Push(0x133, 4);
    Sound_Play(MUS_081);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(9, 2, 20);
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Event_GetFlag(0x175))
      Sound_Play(MUS_017);
    else
      Sound_Play(MUS_035);
    goto tutorial;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToObject(9, owner);
  djinni->velocityY = px(10);
  FieldActor_SetMoveTargetFixed(djinni, x, 0, z);
  Event_Wait(30);
  WorldMap_Freeze();
  Actor_FaceActorById(9, owner, 0);
  Actor_FaceActorById(owner, 9, 0);
  Actor_SetModeById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeWeDjinn));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_ShowMessageAndWait(9, 0);
  Sound_Play(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
  FieldActor_SetMoveTarget(djinni, 10120, 0, 8356);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceActorById(9, owner, 0);
  Event_ShowMessageAndWait(9, 0);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) != 1)
    goto refused;
  Actor_SetModeById(owner, 22);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgHmmmmDoYou));
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
    goto refused;
  Event_ShowMessageAndWait(9, 0);
  Actor_WalkToAndWait(9, x >> 16, z >> 16);
tutorial:
  WorldMap_ExplainGivingDjinn();
  WorldMap_ReloadPendingAreaGraphics();
  return;
refused:
  Actor_SetModeById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellThatS));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageAndWait(9, 0);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  WorldMap_ReloadPendingAreaGraphics();
  FieldDjinn_JoinParty(9, zero, 7);
  Scene_PlayMusic();
  Event_End();
  Event_ClearFlag(FLAG_DJINN_LESSON);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
}

extern u32 gData_02006bc8[];

/// Spawns object 285 at a random spot with a random scale and starts its
/// script.
void Func_02002594(void) {
  struct Actor *obj;
  struct FieldSprite *sprite;
  u8 *motionFlags;
  s32 x;
  s32 z;
  s32 scale;

  x = ((Random16() * 40) >> 16) << 16;
  z = ((Random16() * 30) >> 16) << 16;
  obj = (FieldActor_Create(285, x + 0x7b80000, 0, z + 0x1c5c0000));
  if (obj != NULL) {
    sprite = obj->sprite;
    scale = ((Random16() << 15) >> 16) + 0x13333;
    sprite->nextFrame = 0;
    sprite->priority = 2;
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Import_ActorSelectAnimation(obj, 1);
    Actor_StartScript(obj, gData_02006bc8);
  }
}

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, x, z);
}

void Func_02002618(s32 id, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL)
    PosSet(id, *(s16 *)((u8 *)object + 10) + dx,
           *(s16 *)((u8 *)object + 18) + dz);
}

void Func_02002640(s32 a, s32 b, s32 dx, s32 dz) {
  FieldWorldMap_SetWindowCellTileAndDraw(a, b, 48, 224);
  Func_02002618(5, dx, dz);
  Func_02002618(8, dx, dz);
  Func_02002618(9, dx, dz);
  Func_02002618(10, dx, dz);
  FieldWorldMap_SetWindowCellTileAndDraw(a + 16, b, 64, 224);
}

void WorldMap_WatchVenusLighthouseCollapse(void) {
  Func_02003d44(1);
  FieldEvent_SetMessageId((s32)MsgTheyVeDone);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(4, 4, 1);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(SE_CRASH);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(Func_02002594, TASK_PASS_VBLANK);
  MapGrid_SetShake(5, 5, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(7, 6, 1);
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  MapGrid_SetShake(3, 3, 1);
  Event_ShowMessage(9, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(7, 7, 1);
  FieldCamera_MoveTo(1988, CAMERA_KEEP, 7553, 1);
  FieldEffect_ScheduleRatioTransition(39321, 60);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(SE_CRASH);
  Func_02002640(0, 128, 0, 0);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 7);
  System_Wait(4);
  Event_PlaySound(SE_CRASH);
  Func_02002640(32, 128, 32, 0);
  System_Wait(4);
  Func_02002640(64, 128, 32, 0);
  System_Wait(4);
  FieldCamera_MoveTo(2052, CAMERA_KEEP, 7553, 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  MapGrid_SetShake(3, 3, 1);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(5, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(3, 3, 1);
  Func_02002640(96, 128, 32, 0);
  System_Wait(20);
  Func_02002640(0, 160, 32, 0);
  System_Wait(20);
  Func_02002640(32, 160, 32, 0);
  FieldCamera_MoveTo(2148, CAMERA_KEEP, 7553, 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(64);
  FieldEvent_Wait(120);
}

extern s32 gData_02004f10[];

void Func_020028cc(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_Get(8);
  obj =
      FieldActor_Create(285, leader->x - px(32), leader->y, leader->z - px(16));
  if (obj == NULL)
    return;
  obj->scaleX = Q16_ONE / 2;
  obj->scaleY = Q16_ONE / 2;
  spr = obj->sprite;
  if ((Random16() * 2) >> 16) {
    s32 back = ((Random16() * 48) >> 16) << 16;

    obj->x -= back >> 1;
    obj->z -= back;
  } else {
    value = ((Random16() << 5) >> 16) << 16;
    obj->x += value;
    value = (s32)value >> 1;
    obj->z += value;
  }
  spr->nextFrame = 0;
  spr->priority = ((struct FieldSprite *)leader->sprite)->priority;
  obj->priorityFlags |= 2;
  obj->motionFlags = leader->motionFlags;
  FieldActor_SetSpriteColor(obj, 9);
  Import_ActorSelectAnimation(obj, 2);
  Actor_StartScript(obj, gData_02004f10);
}

extern u32 gData_02004eb8[];

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_Get(8);
  FieldEvent_Wait(60);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(39321, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  EventCamera_FollowActor(8, 1);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(8), 0);
  Actor_SetSpeed(8, 0.4);
  actor->timer = 0;
  Actor_SetScript(8, gData_02004eb8);
  Task_AddFrame(Func_020028cc, TASK_PASS_VBLANK);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  PaletteFade_ApplyTarget(0x10003, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  FieldEffect_ScheduleRatioTransition(0x16666, 300);
  FieldEvent_Wait(270);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  *(u16 *)BG_PLTT = RGB(31, 31, 31);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(77);
}

void WorldMap_DriftNorthOfContinent(void) {
  struct Actor *boat = Actor_Get(10);
  struct RescueMap *map = (struct RescueMap *)Ram_HeapSlots->mapWork;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldWorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  FieldWorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = 0x13333;
  boat->scaleY = 0x13333;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(10), 0);
  Actor_SetSpeed(10, 0.4);
  boat->timer = 0;
  boat->acceleration = px(1) / 8;
  boat->speedLimit = px(1) / 8;
  FieldActor_SetMoveTargetFixed(boat, px(10239), boat->y, px(7884));
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeReSaved);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  FieldActor_SetMoveTargetFixed(boat, px(10255), boat->y, px(7868));
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SE_162);
  Event_ShowMessage(gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(8, 0);
  Event_PlaySound(MUS_061);
  WorldMap_ReloadPendingAreaGraphics();
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(10303, CAMERA_KEEP, 7820, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_Get(10)->scaleY += -0x300;
    System_Wait(1);
  }
  WorldMap_Freeze();
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(1, 2, 1);
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(39321, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  System_Wait(1);
  FieldCamera_MoveTo(3655, CAMERA_KEEP, 7632, 0);
  WorldMap_UpdateCurrentTileBlock();
  Actor_SetPosition(8, 3690, 7616);
  Actor_SetPosition(9, 3646, 7679);
  Actor_SetPosition(4, 3669, 7664);
  Actor_SetPosition(5, 3657, 7621);
  Actor_SetPosition(6, 3703, 7651);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  System_Wait(1);
  WorldMap_Freeze();
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldActor_Jump(6, 6, 15);
  FieldActor_Jump(6, 6, 23);
  Actor_FaceActor(6, 4, 0);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(8, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(8, 1);
  Actor_WalkTo(8, 3608, 7695);
  Event_PlaySound(SE_163);
  Func_020042e0(0);
  FieldEvent_SetExitRequest(65);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_AlhafraManor.
void Func_02002e00(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(4);
  {
    /* FAKEMATCH(pin): the mode pinned to r3 starts right after the 1 negated
     * for y, before x loads. */
    register s32 mode asm("r3") = 1;

    FieldCamera_MoveTo(11890, CAMERA_KEEP, 10090, mode);
  }
  goto wait;
  do {
    wait(1);
  wait:;
  } while (gInput.held == 0);
  Event_SetDestination((s32)&SceneId_AlhafraManor, 80);
}

void WorldMap_TakeTheTiller(void) {
  s32 scale;
  s32 seen = Event_GetFlag(0x8de);

  if (seen == 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    scale = Actor_GetById(gMainRoutineWork.currentOwner)->scaleX;
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
    Event_Wait(40);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    FieldEvent_ShowMessageThenWait(4, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldMessage_ShowAtActor(7, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldEvent_ShowMessageThenWait(9, 0, 10);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
      FieldEvent_ShowMessageThenWait(9, 0, 10);
    }
    FieldActor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldActor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldActor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
    gMainRoutineWork.renderMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    WorldMap_ReloadPendingAreaGraphics();
    Event_SetFlag(0x8de);
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(6, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = scale;
    Event_End();
  }
}

void Func_0200310c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToAndWait(8, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(66);
}

/// The ship's first flight once it has wings (flag 0xa78).
void WorldMap_TakeOff(void) {
  s32 x, z, i;

  if (GameFlag_Get(FLAG_SHIP_FLIES) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(8, 7158, 8532);
  WorldMap_SetUpShip();
  x = Actor_Get(8)->x / 0x10000;
  z = Actor_Get(8)->z / 0x10000;
  i = 0;
  {
    do {
      if (Actor_Get(i) != NULL) {
        Data_02006be8[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSpriteFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x, z);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(23)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_Get(23), 0);
  Actor_SetPosition(23, x, z);
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeDidIt);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_ShowMessage(2, 0);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(50);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(MUS_038);
  Event_ShowMessage(6, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(3, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(2, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(1, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(60);
  Event_ShowMessage(6, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Scene_PlayMusic();
  WorldMap_ReloadPendingAreaGraphics();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSpriteFlags(Actor_Get(Data_02006be8[i]), 1);
      Actor_Get(Data_02006be8[i])->scaleX = Q16_ONE;
      Actor_SetPosition(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetPosition(23, 0, 0);
  Event_End();
}

s32 Func_02003418(struct Actor *object) {
  if (Random16() < 0x4000)
    Actor_SetSpriteColorMode(object, Random16() * 8 >> 16);
  return 0;
}

extern s32 gData_02004f70[];

void Func_0200343c(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x;
    s32 y = parent->y;
    s32 z = parent->z;
    struct Actor *object = (FieldActor_Create(673, x, y, z));

    if (object != NULL) {
      Actor_StartScript(object, gData_02004f70);
      Import_ActorSelectAnimation(object, 1);
      Actor_SetSpriteFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = Q16_ONE * 2;
      object->scaleY = Q16_ONE * 2;
    }
  }
}

void Func_02003490(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_0200343c;
  Actor_Get(gMainRoutineWork.currentOwner)->acceleration = px(1) / 8;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit = px(2);
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(49152, 60);
  Event_StartScreenTransition();
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14537, 8577);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14465, 8662);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14452, 8766);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14387, 8828);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14331, 8888);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14284, 8904);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8870);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8831);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8772);
  FieldEffect_ScheduleRatioTransition(0x14000, 120);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
  FieldEvent_SetExitRequest(78);
}

s32 Func_020035e0(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

#undef Actor_SetSpriteColorMode
#include "shared/world_map_objects.c"

#include "shared/actor_flicker.h"

/* VIEW(s3b38): routine gData_02006be4 as this part declared it; the joined file
 * has another; tried as the definition's extern LATE_DATA
 * DEFINE_G_DATA_02003584(gWorldMap_PlacedActorScript, Func_020035e0, 17, 0)
 * LATE_DATA u32 gData_02006be4[], it does not compile in tla-ja:
 * games/tla/src/scene/world_map.c:2101: `gData_02006be4' undeclared (first use
 * in this function). */
extern s32 gData_02006be4__s3b38 __asm__("gData_02006be4");
#define gData_02006be4 gData_02006be4__s3b38

#include "shared/actor_cycle_child_values.h"

#include "shared/actor_set_flight_mode.h"

#include "shared/leader_search.h"

void Func_02003c3c(void) {
  Actor_SetFlightMode(Actor_GetById(Party_GetLeaderActor()), 0);
}

static inline void QueueBlend(u32 address, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline sets the arguments up in
   * their own order: the native finishes the value's orr before the register
   * address's add. */
  IoWriteQueue_Store16(address, value);
}

/// Every other two frames the world map's blend alternates between two
/// strengths.
void Func_02003c50(void) {

  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2)
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 12);
  else
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 16);
}

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
#include "shared/camera_jitter.h"

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02003d44(s32 mode) {
  u8 *buffer;
  u32 blend;

  System_Wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(2, 3, 1);
    Event_PlaySound(SE_RUMBLE);
  }
  WorldMap_Freeze();
  buffer = Heap_AllocEwram(0x4000);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0),
                      (u32)(gWorldMapPalettes + mode * 32), 0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  Task_AddFrame(Func_02003c50, 0x480);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GLITTER);
  gData_02006e08 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    MapGrid_SetShake(2, 2, 1);
    Event_PlaySound(SE_RUMBLE);
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_02003c50);
    DebugText_Clear();
    /* FAKEMATCH(temporary): the blend read into blend before each of the two
     * IoWriteQueue_Store16 calls. */
    blend = gData_02006e08;
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, blend);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  } else {
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_02003c50);
    DebugText_Clear();
    blend = gData_02006e08;
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, blend);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

void Func_020042e0(void) {
  u8 *buffer = Heap_AllocEwram(0x4000);
  u8 *frames;
  s32 x, y, i, t, scrollX;
  u16 scrollY;
  s32 mode;

  Decompress_TaggedLz(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Decompress_TaggedLz(gWorldMapPackedPicturesB, frames);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(14, 0), (u32)gWorldMapPalettesB,
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000400);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)frames, 0x84000800);
  IoWriteQueue_Store16((u32)&REG_BLDCNT, 0x3f41);
  IoWriteQueue_Store16((u32)&REG_BLDALPHA, 0x1010);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG0CNT, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    /* FAKEMATCH(temporary): x * 16 held in t before the divide. */
    t = x * 16;
    scrollX = t / 128;
    scrollY = -y / 128 + 352;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = scrollY;
    System_Wait(1);
    y += 352;
    x += 17;
  }
  Event_PlaySound(SE_SCATTER);
  System_Wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  PaletteFade_ApplyTarget(0x7fff, mode);
  PaletteFade_Interpolate(1);
  IoWriteQueue_Store16((u32)&REG_BLDALPHA, 0x1000);
  System_Wait(1);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  System_Wait(120);
  Ram_HeapSlots->objectSystem->suspended = 1;
  System_Wait(1);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                         gMainRoutineWork.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

void Func_02004500(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_02004634(object);
  }
}

#include "shared/actor_animate_and_emit.h"

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

void Func_02004634(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = (FieldActor_Create(271, parent->x, parent->y, parent->z));
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument(sprite, 0);
        sprite->nextFrame = 0;
        VramCache_FreeEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        ((struct SpriteCacheBits *)&sprite->unk11)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct WorldMapOam *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->sprite;

    p->update = (void *)Actor_GrowDown;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Actor_GrowUp;
    p->priorityFlags = 2;
  }
}

extern s16 gData_02006e0c;
extern s16 gData_02006e0e;
extern u8 gData_02006f20[][32];

#include "shared/gauge_draw.h"

// The world map cursor: counts down while A or B is held, redraws the gauge and
// queues its sprite over the leader.
void Func_020048f4(void) {
  u32 *oam = Data_02006e14;
  s32 tile = gResourceTableEntries[gData_02006e0e].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput.held & (KEY_A | KEY_B)) != 0 &&
      Event_GetFlag(FLAG_MENU_OPEN) == 0 && leader->y > px(4)) {
    if (--gData_02006e10 <= 0) {
      Party_DrainPp(1);
      gData_02006e10 = 10;
    }
  }
  Gauge_Draw(Party_GetPpRatio());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Oam_AddSpriteAtDepthClamped(Data_02006e14, 255);
}

#include "shared/gauge_init.h"

#include "shared/gauge_end.h"

// clang-format off
u32 gData_02004e38[] = {
    39, 2, 48, 0x23010000, 0x16cd0000, 1,
    3, 17,
};

u32 gData_02004e58[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gData_02004e88[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gData_02004eb8[] = {
    5, 0x180000, 0, 0, 1, 13,
    10, 0, 22, 35, 1, 5,
    0x180000, 0, 0, 1, 5, 0x180000,
    0, 0, 1, 17,
};

s32 gData_02004f10[] = {
    23, 5, (s32)0xffffe667, 23, 7, 0x1999,
    23, 9, 0xe00, 23, 10, -384,
    0, 1, 13, 48, 0, 2,
    0, 0, 0, 0, 1, 38,
};

s32 gData_02004f70[] = {
    46,
    (s32)Func_02003418,
    0,
    1,
    13,
    120,
    0,
    38,
};

s32 gWorldMap_SparkScript[] = {
    46,
    (s32)WorldMap_RiseFromAnchor,
    38,
};

u32 gWorldMap_ParticleScript[] = {
    0, 39, 22, 5, 0, 22,
    7, 0, 0, 1, 38,
};

u32 gData_02004fc8[] = {
    0, 9, 0, 6,
};
// clang-format on

#undef PUSH_PICTURE
#undef WORLD_MAP_TRY_ENCOUNTER
#undef ARM_WEST
#undef TUTORIAL_SET_MESSAGE
#undef TUTORIAL_MESSAGE
#undef Actor_SetSpriteColorMode
#undef gData_02006be4

void Func_02000038(void);
void WorldMap_BoardShip(void);
void WorldMap_LeaveShip(void);

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 10240, 0, 7974, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 10137, 0, 8602, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 10423, 0, 8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 10578, 0, 8952, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 10635, 0, 9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 10270, 0, 9522, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 10271, 0, 9425, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 10380, 0, 9370, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 10440, 0, 9399, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 12070, 0, 10470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 11880, 0, 10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 10976, 0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 11736, 0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 11910, 0, 9729, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 9244, 0, 9528, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {22, CONDITION_ALWAYS, 9328, 0, 9528, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {23, CONDITION_ALWAYS, 9148, 0, 9048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {24, CONDITION_ALWAYS, 9148, 0, 8988, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {25, CONDITION_ALWAYS, 9199, 0, 8410, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {26, CONDITION_ALWAYS, 9060, 0, 8290, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {27, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {28, CONDITION_ALWAYS, 9546, 0, 8316, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {29, CONDITION_ALWAYS, 12770, 0, 11570, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 11724, 0, 8064, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {31, CONDITION_ALWAYS, 11680, 0, 6678, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {32, CONDITION_ALWAYS, 13486, 0, 7482, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {33, CONDITION_ALWAYS, 12923, 0, 9681, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {34, CONDITION_ALWAYS, 12921, 0, 9752, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {35, CONDITION_ALWAYS, 12680, 0, 10297, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {36, CONDITION_ALWAYS, 14478, 0, 8410, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {37, CONDITION_ALWAYS, 14250, 0, 8728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {38, CONDITION_ALWAYS, 13376, 0, 6068, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {39, CONDITION_ALWAYS, 13452, 0, 6204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {40, CONDITION_ALWAYS, 10903, 0, 6285, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {41, CONDITION_ALWAYS, 11201, 0, 6105, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {42, CONDITION_ALWAYS, 12461, 0, 7566, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {43, CONDITION_ALWAYS, 12090, 0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {44, CONDITION_ALWAYS, 13010, 0, 1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {45, CONDITION_ALWAYS, 13088, 0, 1090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {46, CONDITION_ALWAYS, 9490, 0, 10960, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {47, CONDITION_ALWAYS, 5836, 0, 10120, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {48, CONDITION_ALWAYS, 8582, 0, 7420, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {49, CONDITION_ALWAYS, 6314, 0, 6223, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {50, CONDITION_ALWAYS, 8847, 0, 4629, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {51, CONDITION_ALWAYS, 8970, 0, 7500, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {52, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {53, CONDITION_ALWAYS, 7280, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {54, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {55, CONDITION_ALWAYS, 7277, 0, 7330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {56, CONDITION_ALWAYS, 7383, 0, 5753, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {57, CONDITION_ALWAYS, 7504, 0, 5650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {58, CONDITION_ALWAYS, 7663, 0, 5535, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {59, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {60, CONDITION_ALWAYS, 8933, 0, 3704, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {61, CONDITION_ALWAYS, 8916, 0, 1640, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {62, CONDITION_ALWAYS, 9056, 0, 1250, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {63, CONDITION_ALWAYS, 9056, 0, 1175, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {64, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {65, CONDITION_ALWAYS, 10239, 0, 7884, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {67, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {68, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {69, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {71, CONDITION_ALWAYS, 10720, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {72, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {73, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {74, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {77, CONDITION_ALWAYS, 12096, 0, 8325, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {78, CONDITION_ALWAYS, 14558, 0, 8457, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 11754, 0, 8030, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 11700, 0, 8034, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {82, CONDITION_ALWAYS, 11596, 0, 10204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {83, CONDITION_ALWAYS, 10240, 0, 9142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {84, CONDITION_ALWAYS, 9020, 0, 882, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {85, CONDITION_ALWAYS, 9288, 0, 9434, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {86, CONDITION_ALWAYS, 9288, 0, 9640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {87, CONDITION_ALWAYS, 12640, 0, 4390, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {88, CONDITION_ALWAYS, 7868, 0, 9120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {89, CONDITION_ALWAYS, 9479, 0, 5060, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(WorldMap),
    SCENE_EXIT(64, WorldMap, 77),
    SCENE_EXIT(65, Idejima, 2),
    SCENE_EXIT(66, LemurianShipDeck1, 10),
    SCENE_EXIT(77, Idejima, 12),
    SCENE_EXIT(78, AquaRock1, 5),
    SCENE_EXIT(1, Idejima, 4),
    SCENE_EXIT_UNLESS(2, Daila1, 8, 0x873),
    SCENE_EXIT_UNLESS(3, Daila1, 9, 0x873),
    SCENE_EXIT(2, Daila2, 8),
    SCENE_EXIT(3, Daila2, 9),
    SCENE_EXIT(4, KandoreanTemple8, 1),
    SCENE_EXIT(5, ShrineOfTheSeaGod1, 3),
    SCENE_EXIT(6, DehkanPlateau1, 1),
    SCENE_EXIT(7, DehkanPlateau3, 6),
    SCENE_EXIT(8, LemurianShipDeck2, 2),
    SCENE_EXIT(9, Madra, 11),
    SCENE_EXIT(10, MadraDrawbridge, 1),
    SCENE_EXIT(11, MadraDrawbridge, 2),
    SCENE_EXIT(12, OseniaCliffs, 1),
    SCENE_EXIT(13, OseniaCliffs, 2),
    SCENE_EXIT(14, Mikasalla1, 6),
    SCENE_EXIT(15, Garoh3, 1),
    SCENE_EXIT(16, AirsRock1, 1),
    SCENE_EXIT(17, YampiDesert1, 1),
    SCENE_EXIT(18, YampiDesert8, 2),
    SCENE_EXIT(19, YampiDesert9, 2),
    SCENE_EXIT(20, Alhafra1, 9),
    SCENE_EXIT(21, GondowanCliffs, 1),
    SCENE_EXIT(22, GondowanCliffs, 2),
    SCENE_EXIT(23, Naribwe, 6),
    SCENE_EXIT(25, KibomboMountains2, 1),
    SCENE_EXIT(26, KibomboMountains1, 1),
    SCENE_EXIT_UNLESS(27, Kibombo4, 1, 0x8ff),
    SCENE_EXIT_UNLESS(27, Kibombo1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT(28, Islets1, 1),
    SCENE_EXIT(29, Islets2, 1),
    SCENE_EXIT(30, Islets3, 1),
    SCENE_EXIT(31, Islets4, 10),
    SCENE_EXIT(32, Islets5, 1),
    SCENE_EXIT(33, Yallam1, 10),
    SCENE_EXIT(34, Yallam1, 11),
    SCENE_EXIT(35, TaopoSwamp1, 1),
    SCENE_EXIT(36, ApojiiIslands1, 1),
    SCENE_EXIT(37, AquaRock1, 1),
    SCENE_EXIT(38, Izumo, 1),
    SCENE_EXIT(39, GaiaRock1, 5),
    SCENE_EXIT(40, Champa1, 9),
    SCENE_EXIT(41, AnkohlRuins7, 1),
    SCENE_EXIT(42, SeaOfTime1, 1),
    SCENE_EXIT(43, SeaOfTime2, 5),
    SCENE_EXIT(44, SeaOfTime2, 3),
    SCENE_EXIT(45, LemuriaEntrance, 1),
    SCENE_EXIT(46, TundariaTower1, 1),
    SCENE_EXIT(47, Settlements9, 1),
    SCENE_EXIT(48, Settlements1, 1),
    SCENE_EXIT(49, Settlements4, 1),
    SCENE_EXIT(50, Settlements7, 1),
    SCENE_EXIT(51, MagmaRock1, 1),
    SCENE_EXIT(52, AttekaInlet1, 1),
    SCENE_EXIT(53, AttekaInlet2, 2),
    SCENE_EXIT(54, Contigo1, 1),
    SCENE_EXIT(55, JupiterLighthouseEntrance, 1),
    SCENE_EXIT(56, ShamanVillageCave, 1),
    SCENE_EXIT(57, ShamanVillageCave, 10),
    SCENE_EXIT(58, ShamanVillage2, 1),
    SCENE_EXIT(59, Loho1, 7),
    SCENE_EXIT(60, NorthernReaches, 1),
    SCENE_EXIT(61, NorthernReaches, 4),
    SCENE_EXIT(62, Prox1, 1),
    SCENE_EXIT(63, Prox3, 3),
    SCENE_EXIT(82, Caverns2, 1),
    SCENE_EXIT(83, Caverns1, 1),
    SCENE_EXIT(84, MarsLighthouseEntrance1, 1),
    SCENE_EXIT(85, GondowanCliffsPass, 1),
    SCENE_EXIT(86, GondowanCliffsPass, 2),
    SCENE_EXIT(87, TreasureIsle1, 1),
    SCENE_EXIT(88, Caverns3, 5),
    SCENE_EXIT(89, Caverns4, 1),
    SCENE_EXITS_END,
};

LATE_DATA s32 gData_02006320[] = {
    46,
    (s32)TileSprite_Reset,
    17,
};

LATE_DATA s32 gData_0200632c[] = {
    46,
    (s32)StoryActor_Initialize,
    17,
};

LATE_DATA struct ActorPlacement gData_02006338[] = {
    ACTOR_PLACEMENT(8, CONDITION_ALWAYS, (s32)gData_02006320, 10640, 0, 9075,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(243, 0x37, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(243, 0x38, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(244, 0x4c, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(244, 0x4f, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(246, 0x78, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(243, 0x3d, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(245, 0x65, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(221, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(240, IF_FLAG_SET(0xa76), MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(241, IF_FLAG_SET(0xa76), MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(241, IF_FLAG_SET(0xa76), MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02006500[] = {
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1968, 0, 7488,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1936, 0, 7472,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1904, 0, 7488,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(8, CONDITION_ALWAYS, (s32)gData_02006320, 1946, 0, 7557,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gData_02006578[] = {
    46,
    (s32)Func_02000388,
    17,
};

LATE_DATA s32 gData_02006584[] = {
    46, (s32)Func_020003a8, 46, (s32)Func_020003b4, 17,
};

LATE_DATA struct ActorPlacement gData_02006598[] = {
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000, 0, 7500,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000, 0, 7500,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000, 0, 8000,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000, 0, 8000,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(433, CONDITION_ALWAYS, (s32)gData_02006578, 10207, 0, 7900,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10524, 0, 7450,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10532, 0, 7480,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10540, 0, 7510,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10548, 0, 7540,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10556, 0, 7570,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10564, 0, 7600,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10572, 0, 7630,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gData_02006584, 10580, 0, 7660,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(8, CONDITION_ALWAYS, (s32)gData_02006320, 3515, 0, 7621,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02006700[] = {
    ACTOR_PLACEMENT(9, CONDITION_ALWAYS, (s32)gData_02006320, 10640, 0, 9075,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550, 0, 8682,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550, 0, 9282,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850, 0, 8682,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850, 0, 9282,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gData_02006790[] = {
    ACTOR_PLACEMENT(433, CONDITION_ALWAYS, (s32)gData_02006578, 12096, 0, 8325,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_RAISED, 251, CONDITION_ALWAYS, EVENT(WorldMap_BoardShip)},
    {TRIGGER_RAISED, 252, CONDITION_ALWAYS, EVENT(WorldMap_LeaveShip)},
    {TRIGGER_RAISED, 253, CONDITION_ALWAYS, EVENT(WorldMap_SetUpFlyingShip)},
    {TRIGGER_RAISED, 254, CONDITION_ALWAYS, EVENT(WorldMap_SetUpShip)},
    {TRIGGER_STEP, 92, 0x38, EVENT(Func_02000038)},
    {TRIGGER_STEP, 93, 0x4c, EVENT(Func_0200009c)},
    {TRIGGER_STEP, 94, 0x4f, EVENT(Func_02000100)},
    {TRIGGER_STEP, 95, 0x78, EVENT(Func_02000164)},
    {TRIGGER_STEP, 96, 0x3d, EVENT(Func_020001c8)},
    {TRIGGER_STEP, 97, 0x65, EVENT(Func_0200022c)},
    {TRIGGER_EXAMINE, 1123, CONDITION_ALWAYS, EVENT(WorldMap_GazeIntoPond)},
    {TRIGGER_STEP, 100, CONDITION_ALWAYS, EVENT(Func_02000db0)},
    {TRIGGER_EXIT, 101, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 102, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 103, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 104, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 105, 0x8de, 8},
    {TRIGGER_EXIT, 106, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 107, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 108, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 109, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 110, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 111, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 112, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT, 113, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 114, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 115, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 116, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 117, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT, 118, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT, 119, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 120, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 121, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 122, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 123, CONDITION_ALWAYS, 31},
    {TRIGGER_EXIT, 124, CONDITION_ALWAYS, 32},
    {TRIGGER_STEP, 125, CONDITION_ALWAYS, EVENT(Func_02000da4)},
    {TRIGGER_EXIT, 126, CONDITION_ALWAYS, 35},
    {TRIGGER_EXIT, 127, CONDITION_ALWAYS, 36},
    {TRIGGER_EXIT, 128, CONDITION_ALWAYS, 37},
    {TRIGGER_EXIT, 129, CONDITION_ALWAYS, 38},
    {TRIGGER_EXIT, 130, CONDITION_ALWAYS, 39},
    {TRIGGER_EXIT, 131, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 132, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 133, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 134, CONDITION_ALWAYS, 43},
    {TRIGGER_EXIT, 136, CONDITION_ALWAYS, 45},
    {TRIGGER_EXIT, 137, CONDITION_ALWAYS, 46},
    {TRIGGER_EXIT, 138, CONDITION_ALWAYS, 47},
    {TRIGGER_EXIT, 139, CONDITION_ALWAYS, 48},
    {TRIGGER_EXIT, 140, CONDITION_ALWAYS, 49},
    {TRIGGER_EXIT, 141, CONDITION_ALWAYS, 50},
    {TRIGGER_EXIT, 142, CONDITION_ALWAYS, 51},
    {TRIGGER_EXIT, 143, CONDITION_ALWAYS, 54},
    {TRIGGER_EXIT, 144, CONDITION_ALWAYS, 55},
    {TRIGGER_EXIT, 145, CONDITION_ALWAYS, 56},
    {TRIGGER_EXIT, 146, CONDITION_ALWAYS, 57},
    {TRIGGER_EXIT, 147, CONDITION_ALWAYS, 58},
    {TRIGGER_EXIT, 148, CONDITION_ALWAYS, 59},
    {TRIGGER_STEP, 150, CONDITION_ALWAYS, EVENT(Func_02000d98)},
    {TRIGGER_EXIT, 151, CONDITION_ALWAYS, 82},
    {TRIGGER_EXIT, 152, CONDITION_ALWAYS, 83},
    {TRIGGER_EXIT, 153, CONDITION_ALWAYS, 84},
    {TRIGGER_EXIT, 154, CONDITION_ALWAYS, 84},
    {TRIGGER_EXIT, 155, CONDITION_ALWAYS, 87},
    {TRIGGER_EXIT, 156, CONDITION_ALWAYS, 88},
    {TRIGGER_EXIT, 157, CONDITION_ALWAYS, 89},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {TRIGGER_EXIT, 44, CONDITION_ALWAYS, 44},
    {TRIGGER_EXIT, 45, CONDITION_ALWAYS, 45},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(Func_02000918)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(WorldMap_RefuseToRunAway)},
    {TRIGGER_STEP, 40, 0xa78, EVENT(WorldMap_TakeOff)},
    {TRIGGER_EXIT, 60, CONDITION_ALWAYS, 60},
    {TRIGGER_EXIT, 61, CONDITION_ALWAYS, 61},
    {TRIGGER_EXIT, 85, CONDITION_ALWAYS, 85},
    {TRIGGER_EXIT, 86, CONDITION_ALWAYS, 86},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Func_02000d1c)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gData_02006bc8[] = {
    0,
    16,
    38,
};

LATE_DATA s32 gWorldMap_PlacedActorScript[] = {
    46,
    (s32)Func_020035e0,
    17,
    0,
};

LATE_DATA u32 gData_02006be4[] = {
    0,
};
