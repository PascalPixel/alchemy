/// @file
/// The scene code of overlay 0x64b, in ROM order.

#include "shared/actors.h"
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "version.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "frame.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_coordinate.h"
#include "text/msg_ids.h"
#include "owner_state.h"
#include "event_work.h"
#include "field_sprite.h"
#include "call.h"
#include "task.h"
#include "fixed_math.h"
#include "input.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/scene/world_map.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "module/window/window.h"
#include "module/game/game.h"
#include "scene_ids.h"

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
 * void Actor_SetSpriteColorMode(struct Actor *, u32). */
void Actor_SetSpriteColorMode__world_map(
    struct Actor *object, s32 value) __asm__("Actor_SetSpriteColorMode");
#define Actor_SetSpriteColorMode Actor_SetSpriteColorMode__world_map
u32 __umodsi3(u32 a, u32 b);

DEFINE_TILE_SPRITE_RESET(TileSprite_Reset)

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): through the inline the multiply's address loads between
   * the halves of 0x40000, as in the native, where called directly both halves
   * come first; as Iwram_MulQ16 called directly, 2 of 56 instructions of
   * WorldMap_UpdateBobbingMarker differ in tla-en. */
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
    marker->x.fixed = 0x27880000;
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
  if (Event_GetFlag(0x16E) != 0) {
    return 0;
  }
  actor->update = (void (*)(void))WorldMap_UpdateBobbingMarker;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->unk66 = 0;
  actor->scaleX = 0x8000;
  actor->scaleY = 0x8000;
  return 0;
}

DEFINE_TILE_SPRITE_RESET(Func_02000388)

s32 Func_020003a8(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

/// With flag 0x200 set the object is released, takes acceleration and speed
/// limit 0x1000 and is sent to an offset of its position.
s32 Func_020003b4(struct Actor *object) {
  s32 x, z;

  if (!Event_GetFlag(0x200))
    return 1;
  Actor_SetSpriteFlags(object, 0);
  x = object->x;
  object->acceleration = 0x1000;
  object->speedLimit = 0x1000;
  z = object->z;
  FieldActor_SetMoveTarget(object, x - 0x1e0000, object->y, z + 0x80000);
  return 0;
}

extern u8 gData_02006338[];
extern u8 gData_02006500[];
extern u8 gData_02006598[];
extern u8 gData_02006700[];
extern u8 gData_02006790[];

void Func_02003bf0(void);

u8 *Scene_GetPlacements(void) {
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
  Func_02003bf0();
  if (Event_GetFlag(0x147))
    work[3387] = 0;
  else
    work[3387] = 1;
  if (Event_GetFlag(0x147)) {
    Message_CloseFinishedWindows();
    WorldMap_ReloadPendingAreaGraphics();
  }
  Func_020049bc();
  {
    s32 zero;

    /* FAKEMATCH(temporary): the halfword counters take an s32 zero assigned
     * inside the first store; as a halfword constant it loads from the pool,
     * and as a zero set first 346 is kept out of r3; as 0 stored in each, 13
     * of 73 instructions differ in tla-en. */
    *(u16 *)(work + 346) = zero = 0;
    *(u16 *)(work + 350) = zero;
    *(u16 *)(work + 348) = zero;
    *(u16 *)(work + 356) = zero;
    *(u16 *)(work + 358) = zero;
  }
}

/// Sets render mode 7, clears object 8's child flags, then adds up the PP of
/// the active party; when it is 0, the current owner gets 1 PP back. The loop
/// is the listing's goto loop, which the loop pass leaves alone
/// (gMainRoutineWork reloaded each pass); its first test reads pp, which still
/// holds the index's 0.
void WorldMap_SetUpShip(void) {
  s32 count;
  s32 pp;
  s32 i;

  gMainRoutineWork.renderMode = 7;
  Func_02003c3c();
  Func_02004a1c();
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
  Event_PlaySound(78);
  Import_ActorSelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(152);
  Actor_SetSpriteFlags(leader, 0);
  Import_ActorSelectAnimation(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x40000;
  leader->terrainHeight = 0x40000;
  Actor_StartIdle(leader);
  FieldActor_SetMoveTarget(leader, ship->x, 0, ship->z);
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
    Event_PlaySound(78);
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
    leader->y = 0x40000;
    leader->z = ship->z;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(leader, 0);
    Import_ActorSelectAnimation(leader, 6);
    System_Wait(6);
    Event_PlaySound(152);
    Import_ActorSelectAnimation(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    leader->motionFlags = flags;
    Actor_StartIdle(leader);
    FieldActor_SetMoveTarget(leader, position[0], 0, position[2]);
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
  Event_PlaySound(123);
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

/// ★ Named from the game: "We shouldn't run from anymore, ." (MsgWeShouldnT)
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
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      Actor_SetAnimation(5, 4);
      WorldMap_Freeze();
      Event_ShowMessage(5, 0);
      Actor_ShowEmote(5, 265, 0);
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
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(work->scene);
  }
}

/// ★ Named from the game: " gazed into the pond..." (MsgGazedIntoThe)
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
    /* FAKEMATCH(pin): object in r6, which leaves r7 to the amount; as a plain
     * local, 24 of 129 instructions differ in tla-en. */
    register struct Actor *object asm("r6");

    Message_ShowCentered((s32)MsgThereAreSome, 1);
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    FieldVector_AddPolarOffset(0x100000, leader->facing, position);
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
    /* FAKEMATCH(dead): the store and the call in a once-only block; as plain
     * statements, 6 of 129 instructions differ in tla-en. */
    do {
      *(u16 *)((u8 *)work + 0xd34) = amount;
      Scene_TalkToActor(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    MapGrid_SetShake(0x30000, 0x50000, 0x10000);
    MapGrid_SetShake((s32)(-1), (s32)(-1), (s32)(0xe666));
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

  FieldActor_ShowItemIconById(19, 0x198);
  object = Actor_Get(19);
  animation = object->sprite;
  animation[17] |= 1;
  Actor_SetSpriteFlags(Actor_Get(19), 0);
  a = 0x4000;
  id = 19;
  Actor_SetPositionAndFacing(id, PIXELS(10168), PIXELS(9648), a);
  Event_SetFlag(0xa77);
}

void Func_02000d98(s32 id) { Func_02003654(id, 63, 62); }

void Func_02000da4(s32 id) { Func_02003654(id, 34, 33); }

void Func_02000db0(s32 id) { Func_02003654(id, 3, 2); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern u8 gData_0200632c[];

s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (GameFlag_Get(0x109) == 0 && GameFlag_Get(0x8de) != 0 &&
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
  GameFlag_Set(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  FieldCamera_SetSpeed(0x80000, 0x10000);
  GameFlag_Clear(0x12f);
  Func_02003960();
  if (GameFlag_Get(0x90f))
    Field_SetWorldMapWindowCellTile(0, 240, 336, 288);
  if (GameFlag_Get(0xa21)) {
    Field_SetWorldMapWindowCellTile(0, 224, 208, 256);
    Field_SetWorldMapWindowCellTile(16, 224, 224, 256);
  }
  if (GameFlag_Get(0xa77))
    Func_02000d4c();
  switch (gMainRoutineWork.entrance) {
  case 39:
    if (GameFlag_Get(0x927))
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
    Actor_SetPosition(7, 0x1c6d0000, 0x1c840000);
    wait(1);
    FieldEvent_SetMessageId((s32)MsgIVeHeard);
    Event_ShowMessage(7, 0);
    FieldEvent_Wait(20);
    Event_ShowMessage(7, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_SetDestination((s32)&SceneId_Ov6a5, 9);
    break;
  case 98:
    gMainRoutineWork.ownerOverride = 1;
    Event_SetFlag(0x8de);
    Event_SetFlag(0x8df);
    Event_SetFlag(0x163);
    Event_SetFlag(0x11d);
    Event_SetDestination((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_Set(0x163);
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
/// ★ Named from the game: "To give your Djinni to someone else, move it like
/// this." (MsgDjinnGiveTutorialA)
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
  Event_PlaySound(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_ClearFlag(0x16f);
  Event_ClearFlag(0x171);
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
  Event_ClearFlag(0x16f);
  Event_SetFlag(0x171);
  DjinnMenu_Run();
  FieldEvent_Wait(30);
  Field_RequestBattleFromEncounterTable(9, 5);
}

#define ARM_WEST(id)                                                           \
  do {                                                                         \
    /* FAKEMATCH(barrier): the native copies the angle before it loads the     \
     * actor id; as the plain Call3, 10 of 946 instructions differ in tla-en.  \
     */                                                                        \
    s32 a1 = west;                                                             \
                                                                               \
    asm volatile("" : "+l"(a1));                                               \
    Actor_FaceDirection(id, a1, 0);                                            \
  } while (0)

/// The bandit and the thieves from Vault, after a linked game, want Isaac.
/// ★ Named from the game: "Hey! Wait a minute!" (MsgHeyWaitA)
void WorldMap_FightBanditAndThieves(s32 entrance) {
  s32 east, west, east74, eastB, westB;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (entrance == 9) {
    west = 0xc000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x23280000, west);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgHeyWaitA);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(0x18ccc, 16);
    Actor_ShowEmote(4, 0x102, 30);
    Actor_WalkByAndWait(4, 0, 32);
    Actor_FaceDirection(4, west, 0);
    Event_WaitForZoomTransition();
    Actor_WaitForMoveById(4);
    WorldMap_Freeze();
    {
      /* FAKEMATCH(barrier): the native copies the angle before the other
       * arguments; as the plain call, 2 of 946 instructions differ in tla-en.
       */
      s32 a3 = west;

      asm volatile("" : "+l"(a3));
      Actor_LaunchFromLeader(23, 16, 0, a3);
    }
    Actor_RefreshSelectorById(23);
    Actor_FaceDirectionAndWait(23, west);
    east = 0x4000;
    Event_Wait(10);
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25800000, east);
    Actor_SetPositionAndFacing(21, 0x27b80000, 0x25800000, east);
    Actor_SetPositionAndFacing(22, 0x27b80000, 0x25800000, east);
    Actor_WalkBy(20, 0, 16);
    Actor_WalkBy(21, -16, 16);
    Actor_WalkByAndWait(22, 16, 16);
    Actor_FaceDirection(20, 0x3000, 0);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(22, 0x3000, 0);
    Actor_ShowEmote(23, 0x100, 30);
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
       * the actor; as the plain Call3, 2 of 946 instructions differ in tla-en.
       */
      s32 a1 = west, a2;

      asm volatile("" : "+l"(a1));
      a2 = 0;
      Actor_ArmCallback(23, a1, a2);
    }
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, 0x107, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, 0x101, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, 0x107, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(23, 0x102, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(22, 0x107, 30);
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
       * loads the actor last; as the plain Value3, 2 of 946 instructions differ
       * in tla-en. */
      s32 a1 = 129, a2;

      asm volatile("" : "+l"(a1));
      a2 = 30;
      /* FAKEMATCH(barrier): the asm on the 30, the same call's second; as the
       * plain Value3, with the other two, 2 of 946 instructions differ in
       * tla-en. */
      asm volatile("" : "+l"(a2));
      a1 <<= 1;
      Actor_ShowEmote(4, a1, a2);
    }
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(dead): the native loads the scene's id after this store; as
     * no block, 2 of 946 instructions differ in tla-en. */
    do {
    } while (0);
    {
      s32 worldMap = (s32)&SceneId_WorldMap;

      FieldEvent_SetAfterBattleDestination(worldMap, 73);
      Party_SetFields1f2And1f4(worldMap, 74);
    }
    WorldMap_ReloadPendingAreaGraphics();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Field_RequestBattleFromEncounterTable(102, 3);
  } else if (entrance == 74) {
    Event_ClearFlag(0xa76);
    east74 = 0x4000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25900000, east74);
    Actor_SetPositionAndFacing(21, 0x27a80000, 0x25900000, east74);
    Actor_SetPositionAndFacing(22, 0x27c80000, 0x25900000, east74);
    {
      s32 a3 = 0xe000;

      Actor_SetPositionAndFacing(23, 0x27a80000, 0x25c00000, a3);
    }
    Actor_SetPositionAndFacing(4, 0x27b80000, 0x25b00000, 0xa000);
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
    Actor_ShowEmote(20, 0x108, 30);
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
    Actor_ShowEmote(22, 0x108, 30);
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
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x25a00000, eastB);
    westB = 0xc000;
    Actor_SetPositionAndFacing(21, 0x27a80000, 0x25a00000, eastB);
    Actor_SetPositionAndFacing(22, 0x27c80000, 0x25a00000, eastB);
    Actor_SetPositionAndFacing(23, 0x27b00000, 0x25d00000, westB);
    Actor_SetPositionAndFacing(4, 0x27c00000, 0x25d00000, westB);
    EventCamera_FollowActor(4, 0);
    FieldEffect_ScheduleRatioTransition(0x18000, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgWeLostAgain);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    Actor_ShowEmote(20, 0x102, 30);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_ShowEmote(21, 0x102, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_ShowEmote(22, 0x102, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 0x101, 30);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_FaceDirectionAndWait(20, 0x8000);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Actor_FaceDirectionAndWait(22, 0x8000);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x101, 30);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_ShowEmote(23, 0x101, 30);
    Actor_FaceDirection(20, eastB, 0);
    Actor_FaceDirection(21, eastB, 0);
    Actor_FaceDirection(22, eastB, 0);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(20, 3);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(10);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_ShowEmote(20, 0x100, 0);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(22, 0x100, 30);
    Event_Wait(40);
    Actor_TurnToFaceEachOther(20, 21, 0);
    Actor_FaceDirection(22, 0x8000, 0);
    Actor_ShowEmote(22, 0x105, 30);
    Actor_ShowEmote(20, 0x102, 40);
    Actor_FaceDirectionAndWait(20, eastB);
    Event_Wait(20);
    Event_OpenMessage(20, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(20, 0x109, 30);
      FieldEvent_ShowMessageThenWait(20, 0, 5);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    } else {
      Actor_ShowEmote(20, 0x109, 30);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      FieldEvent_ShowMessageThenWait(20, 0, 5);
    }
    Actor_TurnToFaceEachOther(4, 23, 0);
    Event_Wait(40);
    Actor_FaceDirection(4, 0xc000, 0);
    Actor_FaceDirection(23, 0xc000, 0);
    Actor_ShowEmote(23, 0x109, 30);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_RunRepeatedMotion(21, 2);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(22, 0x5000, 0);
    Event_Wait(5);
    FieldEvent_ShowMessageThenWait(21, 0, 5);
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    Actor_RunRepeatedMotion(20, 2);
    Func_02000d4c();
    Event_Wait(5);
    Actor_ShowEmote(20, 0x109, 30);
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
    Actor_FaceDirection(21, 0x4000, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Actor_FaceDirectionAndWait(20, 0x4000);
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
    Actor_FaceDirectionAndWait(20, 0x4000);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(23, 0);
    Actor_FaceDirectionAndWait(4, 0x8000);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    FieldEvent_ShowMessageThenWait(23, 0, 5);
    Actor_SetAnimation(4, 3);
    Actor_SetAnimationAndWait(23, 3);
    Event_Wait(10);
    Actor_SetSpeed(23, 0x13333, 0x9999);
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

/* VIEW(world_map): variable gData_02004e38 as const u8 gData_02004e38[];
 * scene/world_map/s3b38 has u32 gData_02004e38[] = {. */
extern const u8 gData_02004e38__world_map[] __asm__("gData_02004e38");
#define gData_02004e38 gData_02004e38__world_map
/* VIEW(world_map): variable gData_02004e58 as const u8 gData_02004e58[];
 * scene/world_map/s3b38 has u32 gData_02004e58[] = {. */
extern const u8 gData_02004e58__world_map[] __asm__("gData_02004e58");
#define gData_02004e58 gData_02004e58__world_map
/* VIEW(world_map): variable gData_02004e88 as const u8 gData_02004e88[];
 * scene/world_map/s3b38 has u32 gData_02004e88[] = {. */
extern const u8 gData_02004e88__world_map[] __asm__("gData_02004e88");
#define gData_02004e88 gData_02004e88__world_map

/// ★ Named from the game: "We got the cannon onto your ship with no problems."
/// (MsgYouDonT)
void WorldMap_DeliverLohoCannon(void) {
  s32 angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(16)->motionFlags = 2;
  Actor_Get(17)->motionFlags = 2;
  Actor_Get(18)->motionFlags = 2;
  /* FAKEMATCH(temporary): the facing angle held in a local is built once and
   * shared by the three placements, as in the native; as a literal, each
   * forwarded call builds it again; as 0xe000 in each call, 25 of 168
   * instructions differ in tla-en. */
  angle = 0xe000;
  Actor_SetPositionAndFacing(16, PIXELS(8913), PIXELS(5853), angle);
  Actor_SetPositionAndFacing(17, PIXELS(8929), PIXELS(5869), angle);
  Actor_SetPositionAndFacing(18, PIXELS(8929), PIXELS(5885), angle);
  Event_StartScreenTransition();
  Actor_SetSpeed(16, 0x3333, 0x1999);
  Actor_SetSpeed(17, 0x3333, 0x1999);
  Actor_SetSpeed(18, 0x3333, 0x1999);
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
  Actor_FaceDirection(17, 0xd000, 0);
  Actor_FaceDirection(18, 0xd000, 0);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(60);
  FieldActor_WatchAndSetScript(4, 18);
  Actor_SetSpeed(16, 0x6666, 0x3333);
  Actor_SetSpeed(17, 0x6666, 0x3333);
  Actor_SetSpeed(18, 0x6666, 0x3333);
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
/// ★ Named from the game: "The Venus Djinni has become 's pal."
/// (MsgTheVenusDjinni)
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
  x = (leader->x - 0x27880000) / 2 + 0x27880000;
  z = (leader->z - 0x20a40000) / 2 + 0x20a40000;
  /* FAKEMATCH(temporary): flag holds the id 0x16e, then the test's result,
   * then flag 0x175's result for MsgHey2, so the test's known zero dies before
   * the stores of 0 to velocityY and terrainHeight; as
   * `if (Event_GetFlag(0x16e) == 0)` with TUTORIAL_MESSAGE(MsgHey2), 53 of 714
   * instructions differ in tla-en. */
  flag = 0x16e;
  flag = GameFlag_Get(flag);
  if (flag == 0) {
    if (gInput.held & 64)
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
    Event_SetFlag(0x16e);
    Event_Start();
    Event_ResolvePendingActions(0);
    Djinn_Give(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    WorldMap_Freeze();
    Actor_FaceActorById(owner, 9, 0);
    Event_Wait(10);
    FieldActor_ShowEmote(owner, 0x101, 60);
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
    djinni->gravity = 0x4000;
    djinni->speedLimit = 0x10000;
    djinni->acceleration = 0x10000;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    /* FAKEMATCH(barrier): an empty asm that reads and writes all of djinni,
     * so sched2 takes the two stores of 0 before the copy of the motionFlags
     * address into sl; as no asm, 2 of 714 instructions differ in tla-en. */
    asm("" : "+m"(*djinni));
    FieldActor_SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
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
    djinni->gravity = 0x10000;
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
    FieldActor_ShowEmote(owner, 0x100, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    FieldActor_ShowEmote(owner, 0x102, 30);
    Event_ShowMessageAndWait(9, 0);
    FieldActor_ShowEmote(owner, 0x100, 30);
    Event_ShowMessageAndWait(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(9, 2, 30);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 0;
    FieldActor_SetMoveTarget(djinni, x, 0x100000, z);
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
    Actor_ShowEmoteById(9, 0x102, 30);
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
    Sound_Play(81);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(9, 2, 20);
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Event_GetFlag(0x175))
      Sound_Play(17);
    else
      Sound_Play(35);
    goto tutorial;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToObject(9, owner);
  djinni->velocityY = 0xa0000;
  FieldActor_SetMoveTarget(djinni, x, 0, z);
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
  Sound_Play(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_SetFlag(0x16f);
  Event_ClearFlag(0x171);
  DjinnMenu_Run();
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
  FieldActor_SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
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
  Actor_ShowEmoteById(9, 0x100, 30);
  Event_ShowMessageAndWait(9, 0);
  Event_SetFlag(0x16f);
  Event_SetFlag(0x171);
  DjinnMenu_Run();
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  WorldMap_ReloadPendingAreaGraphics();
  // The constant local: with a plain 0 here, the zero that motionFlags = 2
  // leaves is shared by the first visit's stores of 0 and its reloads take
  // other registers (differ 26).
  FieldDjinn_JoinParty(9, zero, 7);
  Scene_PlayMusic();
  Event_End();
  Event_ClearFlag(0x16e);
  Event_ClearFlag(0x16f);
  Event_ClearFlag(0x171);
}

extern const u8 gData_02006bc8[];

/* VIEW(world_map): routine Actor_StartScript as void Actor_StartScript(struct
 * Actor *object, const u8 *script); scene/shared/world_map_s36a8.h has void
 * Actor_StartScript(struct Actor *object, const s32 *script). */
void Actor_StartScript__world_map(
    struct Actor *object, const u8 *script) __asm__("Actor_StartScript");
#define Actor_StartScript Actor_StartScript__world_map

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
    // The flags byte's address in a local: stored directly, 2 of 53 differ.
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Import_ActorSelectAnimation(obj, 1);
    Actor_StartScript(obj, gData_02006bc8);
  }
}

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, the command
   * takes pixels and shifts them into fixed point inside an inline; as a direct
   * call or a forwarding inline, id and dz take each other's register; as
   * Actor_SetHorizontalPositionWithTerrain called directly, 8 of 18
   * instructions differ in tla-en. */
  Actor_SetPosition(id, x << 16, z << 16);
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

/// ★ Named from the game: "They've done it, ! They've lit the beacon!"
/// (MsgTheyVeDone)
void WorldMap_WatchVenusLighthouseCollapse(void) {
  Func_02003d44(1);
  FieldEvent_SetMessageId((s32)MsgTheyVeDone);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(145);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(Func_02002594, TASK_PASS_VBLANK);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  MapGrid_SetShake(0x70000, 0x60000, 0x10000);
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(0x70000, 0x70000, 0x10000);
  FieldCamera_MoveTo(PIXELS(1988), -1, PIXELS(7553), 1);
  FieldEffect_ScheduleRatioTransition(0x9999, 60);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(145);
  Func_02002640(0, 128, 0, 0);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 7);
  System_Wait(4);
  Event_PlaySound(145);
  Func_02002640(32, 128, 32, 0);
  System_Wait(4);
  Func_02002640(64, 128, 32, 0);
  System_Wait(4);
  FieldCamera_MoveTo(PIXELS(2052), -1, PIXELS(7553), 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(5, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Func_02002640(96, 128, 32, 0);
  System_Wait(20);
  Func_02002640(0, 160, 32, 0);
  System_Wait(20);
  Func_02002640(32, 160, 32, 0);
  FieldCamera_MoveTo(PIXELS(2148), -1, PIXELS(7553), 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(78);
  Event_PlaySound(390);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(64);
  FieldEvent_Wait(120);
}

/* VIEW(world_map): variable gData_02004f10 as const u8 gData_02004f10[];
 * scene/world_map/s3b38 has s32 gData_02004f10[] = {. */
extern const u8 gData_02004f10__world_map[] __asm__("gData_02004f10");
#define gData_02004f10 gData_02004f10__world_map

void Func_020028cc(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_Get(8);
  obj = FieldActor_Create(285, leader->x - PIXELS(32), leader->y,
                          leader->z - PIXELS(16));
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

#include "io_reg.h"

/* VIEW(world_map): variable gData_02004eb8 as const u8 gData_02004eb8[];
 * scene/world_map/s3b38 has u32 gData_02004eb8[] = {. */
extern const u8 gData_02004eb8__world_map[] __asm__("gData_02004eb8");
#define gData_02004eb8 gData_02004eb8__world_map

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_Get(8);
  FieldEvent_Wait(60);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  EventCamera_FollowActor(8, 1);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(8), 0);
  Actor_SetSpeed(8, 0x6666, 0x3333);
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

/* VIEW(world_map): routine Func_020042e0 as void Func_020042e0(s32 mode);
 * scene/world_map/s3b38 has void Func_020042e0(void) {. */
void Func_020042e0__world_map(s32 mode) __asm__("Func_020042e0");
#define Func_020042e0 Func_020042e0__world_map

/// ★ Named from the game: "We're saved!" (MsgWeReSaved)
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
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(10), 0);
  Actor_SetSpeed(10, 0x6666, 0x3333);
  boat->timer = 0;
  boat->acceleration = 0x2000;
  boat->speedLimit = 0x2000;
  FieldActor_SetMoveTarget(boat, 0x27ff0000, boat->y, 0x1ecc0000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeReSaved);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  FieldActor_SetMoveTarget(boat, 0x280f0000, boat->y, 0x1ebc0000);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(78);
  Event_PlaySound(162);
  Event_ShowMessage(gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(8, 0);
  Event_PlaySound(61);
  WorldMap_ReloadPendingAreaGraphics();
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x283f0000, -1, 0x1e8c0000, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_Get(10)->scaleY += -0x300;
    System_Wait(1);
  }
  WorldMap_Freeze();
  Event_SetFlag(0x200);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  System_Wait(1);
  FieldCamera_MoveTo(0x0e470000, -1, 0x1dd00000, 0);
  WorldMap_UpdateCurrentTileBlock();
  Actor_SetPosition(8, 0x0e6a0000, 0x1dc00000);
  Actor_SetPosition(9, 0x0e3e0000, 0x1dff0000);
  Actor_SetPosition(4, 0x0e550000, 0x1df00000);
  Actor_SetPosition(5, 0x0e490000, 0x1dc50000);
  Actor_SetPosition(6, 0x0e770000, 0x1de30000);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  System_Wait(1);
  WorldMap_Freeze();
  Actor_SetAttachedEffect(5, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldActor_Jump(6, 6, 15);
  FieldActor_Jump(6, 6, 23);
  Actor_FaceActor(6, 4, 0);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(8, 0x101);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, 0xe18, 0x1e0f);
  Event_PlaySound(163);
  Func_020042e0(0);
  FieldEvent_SetExitRequest(65);
}

#include "scene_ids.h"

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_Ov667.
void Func_02002e00(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  {
    /* FAKEMATCH(pin): the mode pinned to r3 starts right after the 1 negated
     * for y, before x loads; as a plain constant, 2 of 39 instructions differ
     * in tla-en. */
    register s32 mode asm("r3") = 1;

    FieldCamera_MoveTo(11890 << 16, -1, 10090 << 16, mode);
  }
  // Entered at its test: as a while loop the test is copied ahead of it and
  // gInput hoisted (differ 17).
  goto wait;
  do {
    wait(1);
  wait:;
  } while (gInput.held == 0);
  Event_SetDestination((s32)&SceneId_Ov667, 80);
}

/// ★ Named from the game: "Take the tiller, !" (MsgTakeTheTiller)
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
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(4, 0, 10);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldMessage_ShowAtActor(7, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
      Actor_ShowEmote(8, 265, 30);
      FieldEvent_ShowMessageThenWait(9, 0, 10);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    } else {
      Actor_ShowEmote(8, 265, 30);
      ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
      FieldEvent_ShowMessageThenWait(9, 0, 10);
    }
    FieldActor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldActor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldActor_ShowEmote(8, 264, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    Actor_ShowEmote(8, 258, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 257, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    Actor_ShowEmote(8, 264, 30);
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
/// ★ Named from the game: "We did it! The ship took off without a hitch!"
/// (MsgWeDidIt)
void WorldMap_TakeOff(void) {
  s32 x, z, i;

  if (GameFlag_Get(0x8df) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(8, 0x1bf6, 0x2154);
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
        Actor_SetPosition(i, x << 16, z << 16);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(23)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_Get(23), 0);
  Actor_SetPosition(23, x << 16, z << 16);
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeDidIt);
  Event_PlaySound(390);
  Event_ShowMessage(2, 0);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_PlaySound(78);
  FieldEvent_Wait(50);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(38);
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
  Event_PlaySound(78);
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
      Actor_Get(Data_02006be8[i])->scaleX = 0x10000;
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

/* VIEW(world_map): variable gData_02004f70 as const u8 gData_02004f70[];
 * scene/world_map/s3b38 has s32 gData_02004f70[] = {. */
extern const u8 gData_02004f70__world_map[] __asm__("gData_02004f70");
#define gData_02004f70 gData_02004f70__world_map

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
      object->scaleX = 0x20000;
      object->scaleY = 0x20000;
    }
  }
}

void Func_02003490(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_0200343c;
  Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x2000;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit = 0x20000;
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(0xc000, 60);
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

DEFINE_TILE_SPRITE_RESET(Func_020035e0)

#undef Actor_StartScript
#undef Actor_SetSpriteColorMode
#undef Func_020042e0
#undef gData_02004e38
#undef gData_02004e58
#undef gData_02004e88
#undef gData_02004eb8
#undef gData_02004f10
#undef gData_02004f70
// ---- scene/world_map/s3600.c ----

/* VIEW(s3600): variable gData_02006c08 as struct Actor *gData_02006c08[];
 * module/scene/scene.h has u8 gData_02006c08[]. The three parts after see it so
 * too. */
extern struct Actor *gData_02006c08__s3600[] __asm__("gData_02006c08");
#define gData_02006c08 gData_02006c08__s3600

/// Scene code of overlay 0x64b, in ROM order.

#include "shared/world_map_s3600.h"

// ---- scene/world_map/s36a8.c ----

/// Scene code of overlay 0x64b, in ROM order.

#include "shared/world_map_s36a8.h"

// ---- scene/world_map/s3960.c ----

/// Scene code of overlay 0x64b, in ROM order.

#include "shared/world_map_s3960.h"

#undef gData_02006c08

// ---- scene/world_map/cell_index.c ----

/// Scene code of overlay 0x64b, in ROM order.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "frame.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/scene/world_map.h"

/* VIEW(cell_index): routine Actor_SetSpriteColorMode as void
 * Actor_SetSpriteColorMode(struct Actor *object, s32 value); actor_view.h has
 * void Actor_SetSpriteColorMode(struct Actor *, u32). */
void Actor_SetSpriteColorMode__cell_index(
    struct Actor *object, s32 value) __asm__("Actor_SetSpriteColorMode");
#define Actor_SetSpriteColorMode Actor_SetSpriteColorMode__cell_index
u32 __umodsi3(u32 a, u32 b);

s32 Func_02003988(s32 x, s32 z) { return x / 0x100000 + (z / 0x100000) * 128; }

s32 Func_020039a8(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
  return 1;
}

#undef Actor_SetSpriteColorMode
// ---- scene/world_map/s39f4.c ----

/// Scene code of overlay 0x64b, in ROM order.

#include "shared/world_map_s39f4.h"

// ---- scene/world_map/s3b38.c ----

/// Scene code of overlay 0x64b, in ROM order.

#include "shared/flight.h"
#include "types.h"
#include "system.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "field.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/scene/world_map.h"
#include "module/system/system.h"
#include "resource.h"

extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];
void WorldMap_Freeze(void);

/* VIEW(s3b38): routine Actor_SetSpriteColorMode as void
 * Actor_SetSpriteColorMode(struct Actor *object, s32 value); actor_view.h has
 * void Actor_SetSpriteColorMode(struct Actor *, u32). */
void Actor_SetSpriteColorMode__s3b38(struct Actor *object, s32 value) __asm__(
    "Actor_SetSpriteColorMode");
#define Actor_SetSpriteColorMode Actor_SetSpriteColorMode__s3b38

DEFINE_ACTOR_FLICKER(Func_02003b38, Func_020039f4)

extern s32 gData_02006be4;

DEFINE_ACTOR_CYCLE_CHILD_VALUES(Func_02003b70, gData_02004fc8, gData_02006be4,
                                Func_02003aac)

DEFINE_ACTOR_SET_FLIGHT_MODE(Func_02003bb8, Func_02003b38, Func_02003b70)

DEFINE_LEADER_SEARCH(Func_02003bf0, Func_02003bb8, WorldMap_GetBlockPlace)

u32 __umodsi3(u32 a, u32 b);

void Func_02003c3c(void) {
  Func_02003bb8(Actor_GetById(Party_GetLeaderActor()), 0);
}

static inline void QueueBlend(u32 address, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline sets the arguments up in
   * their own order: the native finishes the value's orr before the register
   * address's add; as direct calls, 4 of 31 instructions of Func_02003c50
   * differ in tla-en. */
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
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, forwarding
   * through the inline sets the arguments up in their own order: the native
   * shifts z before it starts the -1, where called directly the -1 starts
   * first; as direct calls, 6 of 66 instructions of
   * WorldMap_JitterTransitionCamera differ in tla-en. */
  FieldCamera_MoveTo(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
DEFINE_CAMERA_JITTER(WorldMap_JitterTransitionCamera)

#include "event_work.h"
#include "bg_scroll.h"
#include "sprite.h"
#include "field_sprite.h"
#include "vram_tab.h"
#include "resource.h"

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02003d44(s32 mode) {
  u8 *buffer;
  u32 blend;

  System_Wait(1);
  Event_ClearFlag(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(0x20000, 0x30000, 0x10000);
    Event_PlaySound(141);
  }
  WorldMap_Freeze();
  buffer = Heap_AllocEwram(0x4000);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  Task_AddFrame(Func_02003c50, 0x480);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(246);
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
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Event_PlaySound(141);
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
     * IoWriteQueue_Store16 calls; as IoWriteQueue_Store16((u32)&REG_BLDALPHA,
     * gData_02006e08) at both, 2 of 593 instructions differ in tla-en. */
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
  Event_SetFlag(0x101);
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
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)gWorldMapPalettesB, 0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000400);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)frames, 0x84000800);
  IoWriteQueue_Store16(0x04000050, 0x3f41);
  IoWriteQueue_Store16(0x04000052, 0x1010);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH(0x04000008, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    /* FAKEMATCH(temporary): x * 16 held in t before the divide; as scrollX =
     * x * 16 / 128, 22 of 227 instructions differ in tla-en. */
    t = x * 16;
    scrollX = t / 128;
    scrollY = -y / 128 + 352;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = scrollY;
    System_Wait(1);
    y += 352;
    x += 17;
  }
  Event_PlaySound(164);
  System_Wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  PaletteFade_ApplyTarget(0x7fff, mode);
  PaletteFade_Interpolate(1);
  IoWriteQueue_Store16(0x04000052, 0x1000);
  System_Wait(1);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_PlaySound(78);
  System_Wait(60);
  Event_PlaySound(390);
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

DEFINE_ACTOR_FLICKER(Func_02004500, Func_02004634)

DEFINE_ACTOR_ANIMATE_AND_EMIT(Func_02004538, Func_02004634)

DEFINE_ACTOR_ANIMATE(Func_02004570)

DEFINE_ACTOR_GROW_UP(Func_02004598, Actor_Release)

DEFINE_ACTOR_GROW_DOWN(Func_020045e4, Actor_Release)

/* VIEW(s3b38): routine Animation_ApplyChildArgument as s32
 * Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags);
 * actor_view.h has s32 Animation_ApplyChildArgument(struct ActorSpriteView *,
 * s32). */
s32 Animation_ApplyChildArgument__s3b38(
    struct AnimationGroup *state,
    s32 flags) __asm__("Animation_ApplyChildArgument");
#define Animation_ApplyChildArgument Animation_ApplyChildArgument__s3b38

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
        // A one-bit field: as a byte `|= 1` its zero mask moves the
        // motionFlags address out of r3 (differ 15).
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

    p->update = (void *)Func_020045e4;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Func_02004598;
    p->priorityFlags = 2;
  }
}

#include "call.h"
#include "task.h"
#include "input.h"

/* VIEW(s3b38): variable gData_02006e0c as s16 gData_02006e0c;
 * module/scene/scene.h has u8 gData_02006e0c[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/world_map/s3b38.c:447: `gData_02006e0c' has an incomplete
 * type. */
extern s16 gData_02006e0c__s3b38 __asm__("gData_02006e0c");
/* VIEW(s3b38): variable gData_02006e0e as s16 gData_02006e0e;
 * module/scene/scene.h has u8 gData_02006e0e[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/world_map/s3b38.c:532: array subscript is not an integer.
 */
extern s16 gData_02006e0e__s3b38 __asm__("gData_02006e0e");
/* VIEW(s3b38): variable gData_02006f20 as u8 gData_02006f20[][32];
 * module/scene/scene.h has u8 gData_02006f20[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/world_map/s3b38.c:492: subscripted value is neither array
 * nor pointer. */
extern u8 gData_02006f20__s3b38[][32] __asm__("gData_02006f20");

DEFINE_GAUGE_DRAW(Func_02004744, gData_02006e0c__s3b38, Data_02006e20,
                  gData_02006f20__s3b38, gData_02006e0e__s3b38)

// The world map cursor: counts down while A or B is held, redraws the gauge and
// queues its sprite over the leader.
void Func_020048f4(void) {
  u32 *oam = Data_02006e14;
  s32 tile = gResourceTableEntries[gData_02006e0e__s3b38].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput.held & 3) != 0 && Event_GetFlag(0x106) == 0 &&
      leader->y > 0x40000) {
    if (--gData_02006e10 <= 0) {
      Party_DrainPp(1);
      gData_02006e10 = 10;
    }
  }
  Func_02004744(Party_GetPpRatio());
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

DEFINE_GAUGE_INIT(Func_020049bc, gData_02006f20__s3b38, gData_02006e0e__s3b38,
                  gData_02006e0c__s3b38, gData_02006e10, Func_020048f4)

DEFINE_GAUGE_END(Func_02004a1c, gData_02006f20__s3b38, gData_02006e0e__s3b38,
                 gData_02006e0c__s3b38, Func_020048f4)

#include "scene_tables.h"

/* VIEW(s3b38): routine Func_02003418 as void Func_02003418(void);
 * scene/world_map has s32 Func_02003418(struct Actor *object) {. */
void Func_02003418__s3b38(void) __asm__("Func_02003418");
#define Func_02003418 Func_02003418__s3b38
/* VIEW(s3b38): routine Func_020039a8 as void Func_020039a8(void);
 * scene/world_map/cell_index has s32 Func_020039a8(struct Actor *object) {. */
void Func_020039a8__s3b38(void) __asm__("Func_020039a8");
#define Func_020039a8 Func_020039a8__s3b38

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

s32 gData_02004f90[] = {
    46,
    (s32)Func_020039a8,
    38,
};

u32 gData_02004f9c[] = {
    0, 39, 22, 5, 0, 22,
    7, 0, 0, 1, 38,
};

u32 gData_02004fc8[] = {
    0, 9, 0, 6,
};
// clang-format on

#undef Actor_SetSpriteColorMode
#undef Animation_ApplyChildArgument
#undef Func_02003418
#undef Func_020039a8