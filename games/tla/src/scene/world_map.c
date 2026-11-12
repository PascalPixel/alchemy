/// @file
/// The scene code of overlay 0x64b that its scene tables belong to, in ROM order; the rest of the overlay is in world_map/.

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

/// The world map's event work, as far as the encounter events read it.
struct EncounterWork {
  u8 unk000[0x194];
  s32 bias;
  s32 threshold;
};

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
      if (Random16Far() < 0x8000) {                                            \
        Field_RequestDjinnBattle(a, b);                                        \
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
        if (Random16Far() < 0x8000) {                                          \
          Field_RequestDjinnBattle(a, b);                                      \
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

extern u8 gData_02006c08[];
extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02004c20(void);
void Func_02004c28(void);
s32 Func_02004c38(s32 a, s32 b, s32 c);
void Func_02003654(s32 a, s32 b, s32 c);
void Func_020036a8(void);
void Func_020048f4(void);
void Func_02003bb8(struct Actor *object, s32 value);
void Func_02004aa8(const u8 *a, u8 *b);
void Resource_ResetEntry(s32 entry);
u32 __umodsi3(u32 a, u32 b);

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

s32 Func_020002a4(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

struct BobbingMarker {
  u8 unk00[6];
  u16 facing;
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  u8 unk14[80];
  s16 angle;
  s16 stopped;
};

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
    Animation_ApplyChildValues(marker, 10);
  else
    Animation_ApplyChildValues(marker, 7);
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
  actor->action = 0;
  actor->unk66 = 0;
  actor->scaleX = 0x8000;
  actor->scaleY = 0x8000;
  return 0;
}

s32 Func_02000388(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

s32 Func_020003a8(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

/// With flag 0x200 set the object is released, takes acceleration and speed
/// limit 0x1000 and is sent to an offset of its position.
s32 Func_020003b4(struct Actor *object) {
  s32 x, z;

  if (!Event_GetFlag(0x200))
    return 1;
  Actor_SetSingleChildFlags(object, 0);
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

void Message_CloseFinishedWindows(void);
void Func_02003bf0(void);
void Func_020049bc(void);
void Func_02004b28(void);
void Func_02004ba8(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);

u8 *Func_020003f8(void) {
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
void Func_02000458(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  gMainRoutineWork.renderMode = 8;
  if (Event_GetFlag(0x147)) {
    Func_02004da8();
    Func_02004ba8(153, 4);
    Message_ShowCentered((s32)MsgBlank2, 4);
  }
  if (Actor_Get(FieldParty_GetLeaderActor()) != NULL)
    Func_02004b28();
  Func_02003bf0();
  if (Event_GetFlag(0x147))
    work[3387] = 0;
  else
    work[3387] = 1;
  if (Event_GetFlag(0x147)) {
    Message_CloseFinishedWindows();
    Func_02004db0();
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

s32 Unit_AddPp();
void Func_02003c3c(void);
void Func_02004a1c(void);

/// Sets render mode 7, clears object 8's child flags, then adds up the PP of
/// the active party; when it is 0, the current owner gets 1 PP back. The loop
/// is the listing's goto loop, which the loop pass leaves alone
/// (gMainRoutineWork reloaded each pass); its first test reads pp, which still
/// holds the index's 0.
void Func_0200050c(void) {
  s32 count;
  s32 pp;
  s32 i;

  gMainRoutineWork.renderMode = 7;
  Func_02003c3c();
  Func_02004a1c();
  Actor_SetSingleChildFlags(Actor_Get(8), 0);
  count = Party_CountActiveOwners();
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

s32 Func_02004d68(s32 owner);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b48(struct Actor *object);
void Func_02004dc8(void);
void Func_02004dc0(void);

void Func_02000574(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  struct Actor *ship;
  s32 kind, dx, dz, distance;

  if (GameFlag_Test(0x8de) == 0)
    return;
  kind = Func_02004d68(gMainRoutineWork.currentOwner);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ship = Actor_Get(8);
  dx = (leader->x - ship->x) >> 16;
  dz = (leader->z - ship->z) >> 16;
  distance = dx * dx + dz * dz;
  if (kind != 8 && distance > 255)
    return;
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(0x176);
  gMainRoutineWork.savedX = ship->x;
  gMainRoutineWork.savedZ = ship->z;
  gMainRoutineWork.unk280 = ship->angle;
  Event_PlaySound(78);
  Func_02004b18(leader, 6);
  System_Wait(6);
  Event_PlaySound(152);
  Actor_SetSingleChildFlags(leader, 0);
  Func_02004b18(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x40000;
  leader->terrainHeight = 0x40000;
  Func_02004b48(leader);
  FieldActor_SetMoveTarget(leader, ship->x, 0, ship->z);
  Actor_WaitForMove(leader);
  Func_02004b18(leader, 6);
  System_Wait(6);
  leader->terrainHeight = 0;
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  Func_02004dc8();
  Func_02004dc0();
  Func_02004c28();
  *(s16 *)work->unk15c = 0;
#if TLA_PATCH_1
  work->exit = 0;
#endif
  FieldEffect_ScheduleRatioTransition(0x10000, 30);
}

extern u8 gMapBlocks[];

s32 GetWorldMapCollision(s32 *position);
s32 Func_02004b88(struct Actor *object, s32 *position);

void Func_020006d0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ship = Actor_GetById(8);
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 cell = GetWorldMapCollision(&ship->x);
  s32 position[3];
  s32 shore = 0;
  s32 blocked;
  u8 *block;
  u8 *blocks = gMapBlocks;
  u8 flags;

  position[0] = ship->x;
  position[1] = ship->y;
  position[2] = ship->z;
  Vector_AddPolarOffsetFar(0x180000, ship->angle, position);
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
      blocked |= Func_02004b88(ship, position);
      position[0] += -0xc0000;
      blocked |= Func_02004b88(ship, position);
      position[0] += 0x60000;
      position[1] += 0x60000;
      blocked |= Func_02004b88(ship, position);
      position[1] += -0xc0000;
      blocked |= Func_02004b88(ship, position);
      position[1] += 0x60000;
      blocked |= Func_02004b88(ship, position);
    }
  }
  if (blocked == 0) {
    Event_PlaySound(78);
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    FieldEffect_ScheduleRatioTransition(0x10000, 10);
    gMainRoutineWork.renderMode = 1;
    gMainRoutineWork.ownerOverride = 0;
    Event_ClearFlag(0x176);
    gMainRoutineWork.savedX = ship->x;
    gMainRoutineWork.savedZ = ship->z;
    gMainRoutineWork.unk280 = ship->angle;
    flags = leader->motionFlags;
    leader->x = ship->x;
    leader->y = 0x40000;
    leader->z = ship->z;
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(leader, 0);
    Func_02004b18(leader, 6);
    System_Wait(6);
    Event_PlaySound(152);
    Func_02004b18(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    leader->motionFlags = flags;
    Func_02004b48(leader);
    FieldActor_SetMoveTarget(leader, position[0], 0, position[2]);
    Actor_WaitForMove(leader);
    Func_02004b18(leader, 6);
    System_Wait(6);
    Func_02004c28();
    Func_02004dc8();
    Func_02004dc0();
    *(s16 *)work->unk15c = 0;
  }
}

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void Func_02004c98(s32 actor, s32 target);

/// Back to the map: actor 8's position is kept in gMainRoutineWork.
void Func_02000918(void) {
  struct EventDestination *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(8);

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.renderMode == 8)
    Func_0200050c();
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

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

void Func_02000994(void) {
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
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0x9b3)) {
      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      Actor_WalkToAndWait(5, (x << 4) + 24, (z << 4) - 12);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      FieldEvent_SetMessageId((s32)MsgWeShouldnT);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    } else {
      s32 besideZ;

      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_WalkToAndWait(5, x + 24, besideZ);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      FieldEvent_SetMessageId((s32)MsgYouReNot);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_FaceDirection(5, west, 0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      Actor_SetAnimation(5, 4);
      Func_02004da8();
      Event_ShowMessage(5, 0);
      Actor_ShowEmote(5, 265, 0);
      Event_ShowMessage(5, 0);
      Actor_SetAnimation(5, 3);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      Event_SetFlag(0x9b3);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    }
    System_Wait(8);
    Func_02004c28();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(work->scene);
  }
}

void Func_02004d70(s32 value);

void Func_02000be0(void) {
  u32 roll = (Random16Far() << 4) >> 16;

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgGazedIntoThe, 1);
  if (roll == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;
    s32 slot = 79;
    struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
    u8 *unit = Unit_Get(gMainRoutineWork.currentOwner);
    s32 amount = (((unit[15] * Random16Far()) << 1) >> 16) + 0x8001;
    s32 position[3];
    s32 x;
    /* FAKEMATCH(pin): object in r6, which leaves r7 to the amount; as a plain
     * local, 24 of 129 instructions differ in tla-en. */
    register struct Actor *object asm("r6");

    Message_ShowCentered((s32)MsgThereAreSome, 1);
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    FieldVector_AddPolarOffset(0x100000, leader->angle, position);
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
    Actor_ShowItemIconById(79, amount);
    /* FAKEMATCH(dead): the store and the call in a once-only block; as plain
     * statements, 6 of 129 instructions differ in tla-en. */
    do {
      *(u16 *)((u8 *)work + 0xd34) = amount;
      Func_02004d70(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    MapGrid_SetShake(0x30000, 0x50000, 0x10000);
    MapGrid_SetShake((s32)(-1), (s32)(-1), (s32)(0xe666));
    FieldEvent_Wait(30);
    Message_ShowCentered((s32)MsgTheBoatS, 1);
  } else {
    Message_ShowCentered((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Func_02004c28();
}

void Func_02000d1c(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  if (Value3((s32 (*)())Func_02004c38, 19, 0x198, -1) == 0)
    Event_ClearFlag(0xa77);
  Func_02004c28();
}

void Func_02000d4c(void) {
  struct Actor *object;
  u8 *animation;
  s32 a, id;

  Actor_ShowItemIconById(19, 0x198);
  object = Actor_Get(19);
  animation = object->animation;
  animation[17] |= 1;
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  a = 0x4000;
  id = 19;
  Actor_SetPositionAndFacing(id, PIXELS(10168), PIXELS(9648), a);
  Event_SetFlag(0xa77);
}

void Func_02000d98(s32 id) { Func_02003654(id, 63, 62); }

void Func_02000da4(s32 id) { Func_02003654(id, 34, 33); }

void Func_02000db0(s32 id) { Func_02003654(id, 3, 2); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern char SceneId_WorldMap;
extern char SceneId_Ov6a5;
extern u8 gData_0200632c[];

void Func_020012bc(s32 entrance);
void Func_02001c5c(void);
void Func_02001e34(void);
void Func_02002698(void);
void Func_020029a4(void);
void Func_02002ab0(void);
void Func_02002e00(void);
void Func_02002e78(void);
void Func_0200310c(void);
void Func_02003490(void);
void Func_02003960(void);
void Func_02003d44(s32 mode);
void Func_02004c00(void);
void Func_02004d38(s32 scene, s32 entrance);

s32 Func_02000dc4(void) {
  struct EventWork *work;

  if (GameFlag_Test(0x109) == 0 && GameFlag_Test(0x8de) != 0 &&
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.savedX != 0) {
    struct Actor *ship = Actor_GetById(8);

    ship->x = gMainRoutineWork.savedX;
    ship->z = gMainRoutineWork.savedZ;
    ship->angle = gMainRoutineWork.unk280;
    ship->action = 0;
    ship->unk66 = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    Func_080d46a4(0x10000, 1);
  GameFlag_SetBit(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  FieldCamera_SetSpeed(0x80000, 0x10000);
  GameFlag_ClearBit(0x12f);
  Func_02003960();
  /* FAKEMATCH(wrapper): the three Map_SetWindowCellTile calls forwarded through
   * Call4, so each builds its own constants (the 256 is not shared); as direct
   * calls, 12 of 270 instructions differ in tla-en. */
  if (GameFlag_Test(0x90f))
    Field_SetWorldMapWindowCellTile(0, 240, 336, 288);
  if (GameFlag_Test(0xa21)) {
    Field_SetWorldMapWindowCellTile(0, 224, 208, 256);
    Field_SetWorldMapWindowCellTile(16, 224, 224, 256);
  }
  if (GameFlag_Test(0xa77))
    Func_02000d4c();
  switch (gMainRoutineWork.entrance) {
  case 39:
    if (GameFlag_Test(0x927))
      GameFlag_SetBit(0x9ab);
    break;
  case 9:
  case 73:
  case 74:
    if (GameFlag_Test(0xa76))
      Func_020012bc(gMainRoutineWork.entrance);
    break;
  case 77:
    Func_020029a4();
    break;
  case 78:
    Func_02003490();
    break;
  case 59:
    /* FAKEMATCH(wrapper): the flag test here and case 98's four sets forwarded
     * through flag_call.h's Flag_IsSet and Flag_Set; as direct GameFlag_Test
     * and GameFlag_SetBit calls, 75 of 270 instructions differ in tla-en. */
    if (!Event_GetFlag(0x90a) && GameFlag_Test(0xa5f) != 0) {
      GameFlag_SetBit(0x90a);
      Func_02001c5c();
    }
    break;
  case 64:
    Func_02002698();
    break;
  case 65:
    Func_02002ab0();
    break;
  case 70:
    Func_02004c00();
    break;
  case 2:
    if (GameFlag_Test(55) == 0) {
      Actor_SetScript(9, gData_0200632c);
      Func_02001e34();
    }
    break;
  case 67:
    Func_02002e00();
    break;
  case 68:
    Func_02002e78();
    break;
  case 69:
    Func_0200310c();
    break;
  case 72:
    Func_02003d44(2);
    Actor_SetSpriteColor(Actor_GetById(7), 15);
    Actor_SetPosition(7, 0x1c6d0000, 0x1c840000);
    wait(1);
    FieldEvent_SetMessageId((s32)MsgIVeHeard);
    Event_ShowMessage(7, 0);
    FieldEvent_Wait(20);
    Event_ShowMessage(7, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004d38((s32)&SceneId_Ov6a5, 9);
    break;
  case 98:
    gMainRoutineWork.ownerOverride = 1;
    Event_SetFlag(0x8de);
    Event_SetFlag(0x8df);
    Event_SetFlag(0x163);
    Event_SetFlag(0x11d);
    Func_02004d38((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_SetBit(0x163);
    GameFlag_SetBit(0x11d);
    break;
  }
  return 0;
}

void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void Func_02004d48(s32 a, s32 b);
void Func_02004e00(void);
void Func_02004e28(void);

s32 Scene_PrepareMap(void) { return 0; }

/// The djinn tutorial. With flag 0x175 set each message is offset by
/// MsgDjinnGiveTutorialB less A.
// As The Broken Seal's WorldMap_ShowDjinnTutorial.
void Func_020011fc(void) {
  Func_02004db0();
  FieldEffect_ScheduleRatioTransition(0x10000, 6);
  Func_02004e00();
  Func_02004da8();
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
  Func_02004e28();
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
  Func_02004e28();
  FieldEvent_Wait(30);
  Func_02004d48(9, 5);
}

/// gMainRoutineWork.savedScene and savedEntrance as this script reads them.
struct SavedDestination {
  u8 unk000[0x1e4];
  s16 scene;
  s16 entrance;
};

#define ARM_WEST(id)                                                           \
  do {                                                                         \
    /* FAKEMATCH(barrier): the native copies the angle before it loads the     \
     * actor id; as the plain Call3, 10 of 946 instructions differ in tla-en.  \
     */                                                                        \
    s32 a1 = west;                                                             \
                                                                               \
    asm volatile("" : "+l"(a1));                                               \
    Actor_ArmCallback(id, a1, 0);                                              \
  } while (0)

void Func_020012bc(s32 entrance) {
  s32 east, west, east74, eastB, westB;

  Func_02004c20();
  Event_ResolvePendingActions(0);
  if (entrance == 9) {
    west = 0xc000;
    Actor_SetPositionAndFacing(20, 0x27b80000, 0x23280000, west);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
    Event_SetMessageId((s32)MsgHeyWaitA);
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    Func_02004db0();
    Func_080d46a4(0x18ccc, 16);
    Actor_ShowEmote(4, 0x102, 30);
    Actor_WalkByAndWait(4, 0, 32);
    Actor_FaceDirection(4, west, 0);
    Func_02004e00();
    Actor_WaitForMoveById(4);
    Func_02004da8();
    {
      /* FAKEMATCH(barrier): the native copies the angle before the other
       * arguments; as the plain call, 2 of 946 instructions differ in tla-en.
       */
      s32 a3 = west;

      asm volatile("" : "+l"(a3));
      Func_080d3070(23, 16, 0, a3);
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
    Func_02004da8();
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
      /* FAKEMATCH(barrier): the asm on the 0, the same call's second; as the
       * plain Call3, with the asm above, 2 of 946 instructions differ in
       * tla-en. */
      asm volatile("" : "+l"(a2));
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
      /* FAKEMATCH(barrier): the asm on the shifted emote, the same call's
       * third; as the plain Value3, with the other two, 2 of 946 instructions
       * differ in tla-en. */
      asm volatile("" : "+l"(a1));
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

      Event_SetAfterBattleDestination(worldMap, 73);
      Party_SetFields1f2And1f4(worldMap, 74);
    }
    Func_02004db0();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Func_02004d48(102, 3);
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
    FieldCamera_FollowActor(4, 0);
    Func_080d46a4(0x18ccc, 1);
    Actor_SetAnimation(4, 19);
    Actor_SetSingleChildFlags(Actor_Get(4), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
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
    Func_02004d38(((struct SavedDestination *)&gMainRoutineWork)->scene,
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
    FieldCamera_FollowActor(4, 0);
    Func_080d46a4(0x18000, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
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
    Func_02004db0();
    Func_080d46a4(0x10000, 15);
    Func_02004c28();
  }
}

extern const u8 gData_02004e38[];
extern const u8 gData_02004e58[];
extern const u8 gData_02004e88[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02004c90_(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Func_02001c5c(void) {
  s32 angle;

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(16)->motionFlags = 2;
  Actor_Get(17)->motionFlags = 2;
  Actor_Get(18)->motionFlags = 2;
  /* FAKEMATCH(temporary): the facing angle held in a local is built once and
   * shared by the three placements, as in the native; as a literal, each
   * forwarded call builds it again; as 0xe000 in each call, 25 of 168
   * instructions differ in tla-en. */
  angle = 0xe000;
  Func_02004c90_(16, PIXELS(8913), PIXELS(5853), angle);
  Func_02004c90_(17, PIXELS(8929), PIXELS(5869), angle);
  Func_02004c90_(18, PIXELS(8929), PIXELS(5885), angle);
  Event_StartScreenTransition();
  Actor_SetSpeedParameters_(16, 0x3333, 0x1999);
  Actor_SetSpeedParameters_(17, 0x3333, 0x1999);
  Actor_SetSpeedParameters_(18, 0x3333, 0x1999);
  Actor_WalkBy(16, 10, 0);
  Actor_WalkBy(17, 10, 0);
  Actor_WalkBy(18, 10, 0);
  Event_WaitTransitionFrames();
  FieldEffect_ScheduleRatioTransition(0x13333, 6);
  Func_02004e00();
  Func_02004da8();
  FieldActor_WaitForMoveById(16);
  FieldEvent_Wait(40);
  Actor_SetAnimation(16, 3);
  FieldEvent_SetMessageId((s32)MsgYouDonT);
  Actor_ArmCallback_(17, 0xd000, 0);
  Actor_ArmCallback_(18, 0xd000, 0);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(60);
  FieldActor_WatchAndSetScript(4, 18);
  Actor_SetSpeedParameters_(16, 0x6666, 0x3333);
  Actor_SetSpeedParameters_(17, 0x6666, 0x3333);
  Actor_SetSpeedParameters_(18, 0x6666, 0x3333);
  Actor_SetScript(16, gData_02004e38);
  FieldEvent_Wait(40);
  Actor_SetScript(17, gData_02004e58);
  Actor_SetScript(18, gData_02004e88);
  FieldActor_RefreshSelectorById(16);
  FieldActor_RefreshSelectorById(17);
  FieldActor_RefreshSelectorById(18);
  Func_02004db0();
  FieldEvent_Wait(30);
  FieldEffect_ScheduleRatioTransition(0x10000, 6);
  Func_02004c28();
}

void Party_RemoveActiveOwner(s32 owner);
void Party_AddActiveOwner(s32 owner);
static inline void SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  Actor_SetMoveTarget(object, x, y, z);
}
void Func_02004bf8(s32 value);

static inline void ShowEmote(s32 id, s32 animation, s32 frames) {
  Func_080d47b4(id, animation, frames);
}

/// A tutorial message: the second set (from MsgDjinnGiveTutorialB) when
/// secondSet is 1.
#define TUTORIAL_SET_MESSAGE(secondSet, base)                                  \
  ((secondSet) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +   \
   (s32)(base))

/// A tutorial message: the second set while flag 0x175 is set.
#define TUTORIAL_MESSAGE(base) TUTORIAL_SET_MESSAGE(Event_GetFlag(0x175), base)

void Func_02001e34(void) {
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
  flag = GameFlag_Test(flag);
  if (flag == 0) {
    if (gInput.held & 64)
      Event_SetFlag(0x175);
    Func_02004c20();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Party_RemoveActiveOwner(0);
    Party_RemoveActiveOwner(1);
    Party_RemoveActiveOwner(2);
    Party_RemoveActiveOwner(3);
    Party_RemoveActiveOwner(4);
    Party_RemoveActiveOwner(5);
    Party_RemoveActiveOwner(6);
    Party_RemoveActiveOwner(7);
    Party_AddActiveOwner(4);
    Party_AddActiveOwner(5);
    Party_AddActiveOwner(6);
    owner = gMainRoutineWork.currentOwner;
    Func_02004bf8(1);
    Event_SetFlag(0x16e);
    Func_02004c20();
    Event_ResolvePendingActions(0);
    Djinn_AddToOwner(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    Func_02004da8();
    Actor_SetAngleToward(owner, 9, 0);
    Event_Wait(10);
    ShowEmote(owner, 0x101, 60);
    {
      s32 one = 1;
      *(u16 *)&djinni->unk66 = one;
    }
    Actor_SetAngleToward(9, owner, 0);
    wait(16);
    flag = Event_GetFlag(0x175);
    Event_SetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey2));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    Func_02004db0();
    Func_080d46a4(0x13333, 6);
    Func_02004e00();
    Func_02004da8();
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
    SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
    while (i >= 0) {
      djinni->scaleX += 0x800;
      djinni->scaleY += 0x800;
      wait(1);
      i--;
    }
    Actor_SetAngleToward(9, owner, 0);
    Actor_SetAngleToward(owner, 9, 0);
    wait(16);
    djinni->update = 0;
    Animation_ApplyChildValues(djinni, 0);
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
    ShowEmote(owner, 0x100, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    ShowEmote(owner, 0x102, 30);
    Event_ShowMessageAndWait(9, 0);
    ShowEmote(owner, 0x100, 30);
    Event_ShowMessageAndWait(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(9, 2, 30);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 0;
    SetMoveTarget(djinni, x, 0x100000, z);
    while (i >= 0) {
      *(u16 *)&djinni->angle += 0x1000;
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
      *(u16 *)&djinni->angle += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 22);
    Event_ShowMessageAndWait(9, 0);
    Func_080d47b4(9, 0x102, 30);
    Actor_SetAngleToward(9, owner, 0);
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
    Func_02004ba8(0x133, 4);
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
  Func_02004c20();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c98(9, owner);
  djinni->velocityY = 0xa0000;
  SetMoveTarget(djinni, x, 0, z);
  Event_Wait(30);
  Func_02004da8();
  Actor_SetAngleToward(9, owner, 0);
  Actor_SetAngleToward(owner, 9, 0);
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
  Func_02004e28();
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
  SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAngleToward(9, owner, 0);
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
  Func_020011fc();
  Func_02004db0();
  return;
refused:
  Actor_SetModeById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellThatS));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Func_080d47b4(9, 0x100, 30);
  Event_ShowMessageAndWait(9, 0);
  Event_SetFlag(0x16f);
  Event_SetFlag(0x171);
  Func_02004e28();
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Func_02004db0();
  // The constant local: with a plain 0 here, the zero that motionFlags = 2
  // leaves is shared by the first visit's stores of 0 and its reloads take
  // other registers (differ 26).
  Djinn_JoinParty(9, zero, 7);
  Func_02004dc0();
  Func_02004c28();
  Event_ClearFlag(0x16e);
  Event_ClearFlag(0x16f);
  Event_ClearFlag(0x171);
}

extern const u8 gData_02006bc8[];

void Func_02004b20(struct Actor *object, const u8 *script);

/// Spawns object 285 at a random spot with a random scale and starts its
/// script.
void Func_02002594(void) {
  struct Actor *obj;
  struct FieldSprite *sprite;
  u8 *motionFlags;
  s32 x;
  s32 z;
  s32 scale;

  x = ((Random16Far() * 40) >> 16) << 16;
  z = ((Random16Far() * 30) >> 16) << 16;
  obj = (FieldActor_Create(285, x + 0x7b80000, 0,
                                            z + 0x1c5c0000));
  if (obj != NULL) {
    sprite = obj->animation;
    scale = ((Random16Far() << 15) >> 16) + 0x13333;
    sprite->nextFrame = 0;
    sprite->priority = 2;
    // The flags byte's address in a local: stored directly, 2 of 53 differ.
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Func_02004b18(obj, 1);
    Func_02004b20(obj, gData_02006bc8);
  }
}

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, the command
   * takes pixels and shifts them into fixed point inside an inline; as a direct
   * call or a forwarding inline, id and dz take each other's register; as
   * Actor_SetHorizontalPositionWithTerrain called directly, 8 of 18
   * instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
}

void Func_02002618(s32 id, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL)
    PosSet(id, *(s16 *)((u8 *)object + 10) + dx,
           *(s16 *)((u8 *)object + 18) + dz);
}

void Func_02002640(s32 a, s32 b, s32 dx, s32 dz) {
  WorldMap_SetWindowCellTileAndDraw(a, b, 48, 224);
  Func_02002618(5, dx, dz);
  Func_02002618(8, dx, dz);
  Func_02002618(9, dx, dz);
  Func_02002618(10, dx, dz);
  WorldMap_SetWindowCellTileAndDraw(a + 16, b, 64, 224);
}

void Func_02004d30(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Func_02004b78_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

static __inline__ void Func_02004d78_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_02002698(void) {
  Func_02003d44(1);
  FieldEvent_SetMessageId((s32)MsgTheyVeDone);
  Event_ShowMessage(8, 0);
  Func_02004b78_(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Func_02004db0();
  Event_PlaySound(145);
  PaletteFade_ApplySource(0x7fff, 0);
  Func_02004d78_(0x10000, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(Func_02002594, TASK_PASS_VBLANK);
  Func_02004b78_(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Func_02004b78_(0x70000, 0x60000, 0x10000);
  FieldEvent_Wait(30);
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  Func_02004db0();
  Func_02004b78_(0x70000, 0x70000, 0x10000);
  Camera_MoveWithinMapBounds_(PIXELS(1988), -1, PIXELS(7553), 1);
  FieldEffect_ScheduleRatioTransition(0x9999, 60);
  Func_02004d30();
  FieldEvent_Wait(30);
  Func_02004da8();
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  Func_02004db0();
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
  Camera_MoveWithinMapBounds_(PIXELS(2052), -1, PIXELS(7553), 1);
  Func_02004d30();
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(5, 0);
  Func_02004db0();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Func_02002640(96, 128, 32, 0);
  System_Wait(20);
  Func_02002640(0, 160, 32, 0);
  System_Wait(20);
  Func_02002640(32, 160, 32, 0);
  FieldCamera_MoveTo(PIXELS(2148), -1, PIXELS(7553), 1);
  Func_02004d30();
  Func_02004da8();
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(78);
  Event_PlaySound(390);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(64);
  FieldEvent_Wait(120);
}

extern const u8 gData_02004f10[];

static __inline__ struct Actor *Func_02004b30_(s32 kind, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the create call forwarded through an inline, so its
   * arguments build in r0 after the position loads; as a direct call, 32 of
   * 91 instructions differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(kind, x, y, z));
}

void Func_020028cc(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_Get(8);
  obj = Func_02004b30_(285, leader->x - PIXELS(32), leader->y,
                       leader->z - PIXELS(16));
  if (obj == NULL)
    return;
  obj->scaleX = Q16_ONE / 2;
  obj->scaleY = Q16_ONE / 2;
  spr = obj->animation;
  if ((Random16Far() * 2) >> 16) {
    s32 back = ((Random16Far() * 48) >> 16) << 16;

    obj->x -= back >> 1;
    obj->z -= back;
  } else {
    value = ((Random16Far() << 5) >> 16) << 16;
    obj->x += value;
    value = (s32)value >> 1;
    obj->z += value;
  }
  spr->nextFrame = 0;
  spr->priority = ((struct FieldSprite *)leader->animation)->priority;
  obj->priorityFlags |= 2;
  obj->motionFlags = leader->motionFlags;
  Actor_SetSpriteColor(obj, 9);
  Func_02004b18(obj, 2);
  Func_02004b20(obj, gData_02004f10);
}

#include "io_reg.h"

extern const u8 gData_02004eb8[];

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_Get(8);
  FieldEvent_Wait(60);
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  FieldCamera_FollowActor(8, 1);
  System_Wait(1);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_Get(8), 0);
  Actor_SetSpeedParameters_(8, 0x6666, 0x3333);
  actor->action = 0;
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

void Func_020042e0(s32 mode);
void Func_02004b40(void);

struct RescueMap {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 unk11e;
};

void Func_02002ab0(void) {
  struct Actor *boat = Actor_Get(10);
  struct RescueMap *map = (struct RescueMap *)Ram_HeapSlots->mapWork;
  s32 i;

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  WorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  WorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = 0x13333;
  boat->scaleY = 0x13333;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetSpeed(10, 0x6666, 0x3333);
  boat->action = 0;
  boat->acceleration = 0x2000;
  boat->speedLimit = 0x2000;
  FieldActor_SetMoveTarget(boat, 0x27ff0000, boat->y, 0x1ecc0000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004da8();
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
  Func_02004db0();
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x283f0000, -1, 0x1e8c0000, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_Get(10)->scaleY += -0x300;
    System_Wait(1);
  }
  Func_02004da8();
  Event_SetFlag(0x200);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Func_02004db0();
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  System_Wait(1);
  FieldCamera_MoveTo(0x0e470000, -1, 0x1dd00000, 0);
  Func_02004b40();
  Actor_SetPosition(8, 0x0e6a0000, 0x1dc00000);
  Actor_SetPosition(9, 0x0e3e0000, 0x1dff0000);
  Actor_SetPosition(4, 0x0e550000, 0x1df00000);
  Actor_SetPosition(5, 0x0e490000, 0x1dc50000);
  Actor_SetPosition(6, 0x0e770000, 0x1de30000);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     1);
  System_Wait(1);
  Func_02004da8();
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

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames) {
  Func_080d47b4(actor, value, frames);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_Ov667.
void Func_02002e00(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  {
    /* FAKEMATCH(pin): the mode pinned to r3 starts right after the 1 negated
     * for y, before x loads; as a plain constant, 2 of 39 instructions differ
     * in tla-en. */
    register s32 mode asm("r3") = 1;

    Camera_MoveWithinMapBounds(11890 << 16, -1, 10090 << 16, mode);
  }
  // Entered at its test: as a while loop the test is copied ahead of it and
  // gInput hoisted (differ 17).
  goto wait;
  do {
    wait(1);
  wait:;
  } while (gInput.held == 0);
  Func_02004d38((s32)&SceneId_Ov667, 80);
}

void Func_02002e78(void) {
  s32 scale;
  s32 seen = Event_GetFlag(0x8de);

  if (seen == 0) {
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    scale = Actor_GetById(gMainRoutineWork.currentOwner)->scaleX;
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004da8();
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
    Func_02004d08_(8, 256, 30);
    FieldEvent_ShowMessageThenWait(7, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Func_02004d08_(8, 257, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    Actor_ShowEmote(8, 256, 30);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    FieldEvent_ShowMessageThenWait(9, 0, 10);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    Func_02004d08_(8, 264, 30);
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
    Func_02004db0();
    Event_SetFlag(0x8de);
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(6, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = scale;
    Func_02004c28();
  }
}

void Func_0200310c(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToAndWait(8, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(66);
}

extern s32 Data_02006be8[8];

void Func_02003144(void) {
  s32 x, z, i;

  if (GameFlag_Test(0x8df) == 0)
    return;
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(8, 0x1bf6, 0x2154);
  Func_0200050c();
  x = Actor_Get(8)->x / 0x10000;
  z = Actor_Get(8)->z / 0x10000;
  i = 0;
  {
    do {
      if (Actor_Get(i) != NULL) {
        Data_02006be8[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSingleChildFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x << 16, z << 16);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(23)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_Get(23), 0);
  Actor_SetPosition(23, x << 16, z << 16);
  Func_02004da8();
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
  Func_02004dc0();
  Func_02004db0();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSingleChildFlags(Actor_Get(Data_02006be8[i]), 1);
      Actor_Get(Data_02006be8[i])->scaleX = 0x10000;
      Actor_SetPosition(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetPosition(23, 0, 0);
  Func_02004c28();
}

s32 Func_02003418(struct Actor *object) {
  if (Random16Far() < 0x4000)
    Animation_ApplyChildValues(object, Random16Far() * 8 >> 16);
  return 0;
}

extern const u8 gData_02004f70[];

void Func_0200343c(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x;
    s32 y = parent->y;
    s32 z = parent->z;
    struct Actor *object = (FieldActor_Create(673, x, y, z));

    if (object != NULL) {
      Func_02004b20(object, gData_02004f70);
      Func_02004b18(object, 1);
      Actor_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = 0x20000;
      object->scaleY = 0x20000;
    }
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  Actor_SetPositionAndCommit(actor, x, z);
}

void Func_02003490(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
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
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8870);
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8831);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8772);
  FieldEffect_ScheduleRatioTransition(0x14000, 120);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  FieldEvent_SetExitRequest(78);
}

s32 Func_020035e0(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->animation;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}
