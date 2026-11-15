/// @file
/// Air's Rock's inside, the rooms of this overlay within the rock in Osenia:
/// halls of tightropes, pillars to Move and twisters to Whirlwind. At its heart
/// Sheba reads the tablet and learns Reveal (AirsRockInside_ReadRevealTablet,
/// walked to by AirsRockInside_WalkToRevealTablet), and a Psynergy Stone
/// restores the party's PP (AirsRockInside_CheckPsynergyStone).

#include "field/field_sequence.h"
#include "field/emote.h"
#include "field/field_psynergy_work.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "sounds.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/iwram_call.h"
#include "system/io_write.h"
#include "system/main_routine_work.h"
#include "system/frame.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/scene_call.h"
#include "scene/airs_rock.h"
#include "scene/dehkan_plateau.h"
#include "scene/gabomba_statue.h"
#include "graphics/palette_buffers.h"

/// Set once Move has left the hall's pillar on cell (103, 41) in
/// half cells.
#define FLAG_AIRS_ROCK_HALL_PILLAR_PLACED 0xa8e
/// Set once Move has left the Vial chest room's western pillar on (13, 49).
#define FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED 0x898
/// Set once Move has left its eastern pillar on (41, 49).
#define FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED 0x897
/// Set once Move has left the Clarity Circlet chest room's first pillar on (33,
/// 79).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED 0x89b
/// Its second, on (77, 71).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED 0x89c
/// Its third, on (59, 65).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED 0x89d
/// Set once Move has left the tightropes room's pillar on (81, 37).
#define FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED 0x89e
/// Set once Move has moved the Elixir chest room's pillar
/// (AirsRockInside_MoveElixirChestPillar).
#define FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED 0x89f
/// Set once Move has left the paired statues room's pillar on (43, 87).
#define FLAG_AIRS_ROCK_PAIRED_STATUES_PILLAR_PLACED 0x8d0
/// Set once Move has left the face warp room's first pillar on (35, 59).
#define FLAG_AIRS_ROCK_FACE_WARP_FIRST_PILLAR_PLACED 0x8d1
/// Its second, on (45, 105).
#define FLAG_AIRS_ROCK_FACE_WARP_SECOND_PILLAR_PLACED 0x8d2
/// Its third, on (23, 77).
#define FLAG_AIRS_ROCK_FACE_WARP_THIRD_PILLAR_PLACED 0xa8f
/// Set once Move has left the coins chest room's pillar on (45, 33).
#define FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED 0x8d3

/// Story flags: the statues of the hall and the Vial chest room woken by
/// Whirlwind, and the Reveal tablet read.
#define FLAG_AIRS_ROCK_HALL_STATUE_WOKEN 0x89a
#define FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN 0x899
#define FLAG_REVEAL_TABLET_READ 0x8d4

/// The hall's entrance from the Reveal tablet's room, and where the party
/// lands after dropping from the Vial chest room (AirsRockInside_LandInHall);
/// the Vial chest room's entrance on the lift from the hall
/// (AirsRockInside_RiseOnLift).
enum {
  ENTRANCE_HALL_FROM_REVEAL_TABLET = 16,
  ENTRANCE_HALL_LANDING = 11,
  ENTRANCE_VIAL_CHEST_ON_THE_LIFT = 3
};

void Actor_StartScript(struct Actor *object, const u8 *data);

/// The actors of each inner room, numbering its placement table. The wind
/// statues (sprite 377) breathe whirlwinds once Whirlwind on their room's
/// twister rock wakes them; the gust statues (sprite 325) blow a gust Psynergy
/// can steer; stone pillars (SPRITE_STONE_PILLAR) are Moved.
enum AirsRockInsideHallActorId {
  ACTOR_HALL_WEST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_HALL_EAST_STATUE,
  /// The statue the hall's own twister rock wakes.
  ACTOR_HALL_TWISTER_STATUE,
  ACTOR_HALL_TWISTER_ROCK,
  /// The statue standing in the pool, which Move pushes.
  ACTOR_HALL_POOL_STATUE
};

enum AirsRockInsideVialChestActorId {
  ACTOR_VIAL_CHEST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_VIAL_CHEST_EAST_PILLAR,
  ACTOR_VIAL_CHEST_TWISTER_ROCK,
  ACTOR_VIAL_CHEST_WEST_PILLAR
};

enum AirsRockInsideClarityCircletChestActorId {
  ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
  ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
  ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C
};

enum AirsRockInsideTightropesActorId {
  ACTOR_TIGHTROPES_NORTH_GUST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPES_PILLAR,
  ACTOR_TIGHTROPES_SOUTH_GUST_STATUE,
  /// The posts of its five tightropes, two to a rope.
  ACTOR_TIGHTROPES_POST_A,
  ACTOR_TIGHTROPES_POST_B,
  ACTOR_TIGHTROPES_POST_C,
  ACTOR_TIGHTROPES_POST_D,
  ACTOR_TIGHTROPES_POST_E,
  ACTOR_TIGHTROPES_POST_F,
  ACTOR_TIGHTROPES_POST_G,
  ACTOR_TIGHTROPES_POST_H,
  ACTOR_TIGHTROPES_POST_I,
  ACTOR_TIGHTROPES_POST_J
};

/// The Reveal tablet (sprite 323) that rises from the floor.
#define ACTOR_REVEAL_TABLET ACTOR_FIRST_PLACED

enum AirsRockInsideElixirChestActorId {
  /// The stone pillar AirsRockInside_MoveElixirChestPillar moves.
  ACTOR_ELIXIR_CHEST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_ELIXIR_CHEST_SOUTH_GUST_STATUE,
  ACTOR_ELIXIR_CHEST_NORTH_GUST_STATUE,
  ACTOR_ELIXIR_CHEST_POST_A,
  ACTOR_ELIXIR_CHEST_POST_B,
  ACTOR_ELIXIR_CHEST_POST_C,
  ACTOR_ELIXIR_CHEST_POST_D,
  ACTOR_ELIXIR_CHEST_POST_E,
  ACTOR_ELIXIR_CHEST_POST_F
};

enum AirsRockInsidePairedStatuesActorId {
  ACTOR_PAIRED_STATUES_PILLAR = ACTOR_FIRST_PLACED,
  /// The three pillars pushed among the cones.
  ACTOR_PAIRED_STATUES_PILLAR_A,
  ACTOR_PAIRED_STATUES_PILLAR_B,
  ACTOR_PAIRED_STATUES_PILLAR_C,
  /// The four grey cones (sprite 406).
  ACTOR_PAIRED_STATUES_CONE_A,
  ACTOR_PAIRED_STATUES_CONE_B,
  ACTOR_PAIRED_STATUES_CONE_C,
  ACTOR_PAIRED_STATUES_CONE_D,
  ACTOR_PAIRED_STATUES_NORTH_GUST_STATUE,
  ACTOR_PAIRED_STATUES_SOUTH_GUST_STATUE
};

enum AirsRockInsideFaceWarpActorId {
  ACTOR_FACE_WARP_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FACE_WARP_SECOND_PILLAR,
  ACTOR_FACE_WARP_GUST_STATUE,
  ACTOR_FACE_WARP_STATUE,
  ACTOR_FACE_WARP_THIRD_PILLAR,
  ACTOR_FACE_WARP_PILLAR_A,
  ACTOR_FACE_WARP_PILLAR_B
};

enum AirsRockInsideCoinsChestActorId {
  ACTOR_COINS_CHEST_GUST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_COINS_CHEST_PILLAR,
  ACTOR_COINS_CHEST_STATUE
};

/// The cell stampers keep their last cell and the block they copy.
extern s32 gAirsRockInside_StampedCell;
extern s32 gAirsRockInside_StampedWidth;
extern s32 gAirsRockInside_StampedHeight;
extern s32 gAirsRockInside_StampedX;
extern s32 gAirsRockInside_StampedZ;

#include "system/system.h"
#include "field/actor.h"
#include "types.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/map_grid.h"
#include "flags.h"
#include "field/field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "field/call.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "field/field_sequence_event.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "macros.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/task.h"
#include "field/flag_call.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "graphics/resource.h"
#include "system/bg_scroll.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "version.h"
#include "shared/tables.h"
#include "field/field_effect.h"

struct Actor;

struct AirsRockInsideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct AirsRockInsideSpawn {
  s32 mode;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

extern struct ActorPlacement gAirsRockInside_Placements[];
extern struct ActorPlacement gAirsRockInside_HallPlacements[];
extern struct ActorPlacement gAirsRockInside_VialChestPlacements[];
extern struct ActorPlacement gAirsRockInside_ClarityCircletChestPlacements[];
extern struct ActorPlacement gAirsRockInside_RevealTabletPlacements[];

void AirsRockInside_RestoreCellBehindLeader(void);

void AirsRockInside_BurstDust(s32 x, s32 z);
void AirsRockInside_ScatterSparks(s32 x, s32 z);

extern s16 gAirsRockInside_HallPillars[];
extern s16 gAirsRockInside_VialChestPillars[];
extern s16 gAirsRockInside_ClarityCircletChestPillars[];
extern s16 gAirsRockInside_TightropesPillars[];
extern s16 gAirsRockInside_PairedStatuesPillars[];
extern s16 gAirsRockInside_FaceWarpPillars[];
extern s16 gAirsRockInside_CoinsChestPillars[];

void Actor_LiftTargetBlock(const s16 *list);
void Actor_PlaceMovedBlock(void);

void AirsRockInside_StampCellUnderLeader(void);
void AirsRockInside_ReadRevealTablet(void);

extern struct Trigger gAirsRockInside_HallTriggers[];
extern struct Trigger gAirsRockInside_VialChestTriggers[];
extern struct Trigger gAirsRockInside_ClarityCircletChestTriggers[];
extern struct Trigger gAirsRockInside_TightropesTriggers[];
extern struct Trigger gAirsRockInside_RevealTabletTriggers[];
extern struct Trigger gAirsRockInside_ElixirChestTriggers[];
extern struct Trigger gAirsRockInside_PairedStatuesTriggers[];
extern struct Trigger gAirsRockInside_FaceWarpTriggers[];
extern struct Trigger gAirsRockInside_CoinsChestTriggers[];
extern struct Trigger gAirsRockInside_PsynergyStoneTriggers[];

void AirsRockInside_StartRestoringCells(void);
void AirsRockInside_SetLedgeCellsByLevel(void);
void AirsRockInside_SpawnMotes(void);

void Actor_PlaceBlocks(u16 *actors);

void AirsRockInside_RiseOnLift(void);

void Tightrope_Initialize(void);
s32 Tightrope_Place(s32 slot, s32 a, s32 b);

void AirsRockInside_PlaceCones(u16 *list);
void AirsRockInside_TakeOverWhirlwind(void);
void AirsRockInside_RideWhirlwind(void);
void AirsRockInside_BlowGust(void);

void AirsRockInside_StampOtherCellUnderLeader(void);
void AirsRockInside_StampTallCellUnderLeader(void);
void AirsRockInside_StampTallCellUnderLeaderNearer(void);
void AirsRockInside_WakeHallStatue(void);
void AirsRockInside_StepOntoLedge(void);
void AirsRockInside_StartPushingPillar(void);
void AirsRockInside_FinishPushingPillar(void);
void AirsRockInside_WakeVialChestStatue(void);
void AirsRockInside_JumpDownLedge(void);
void AirsRockInside_StepOffRide(void);
void AirsRockInside_StepOnVanishingStone(void);
void AirsRockInside_StampWideCellUnderLeader(void);
void AirsRockInside_HideRevealedDoors(void);
void AirsRockInside_RevealDoors(void);
void AirsRockInside_WalkToRevealTablet(void);
void AirsRockInside_ReadTabletFacingWest(void);
void AirsRockInside_ReadTabletFacingEast(void);
void AirsRockInside_MoveElixirChestPillar(void);
void AirsRockInside_PoundPairedStatuesCone(s32 mode, s32 step);
void AirsRockInside_TradePairedStatues(void);
void AirsRockInside_RideFaceWarp(void);
void AirsRockInside_CheckPsynergyStone(void);
void AirsRockInside_IgnoreStep(void);

extern u32 gFieldEffect_MediumScript[];
extern u32 gAirsRockInside_StatueScript[];

void Actor_PlaySoundInView(struct Actor *object, s32 cue);
s32 Actor_HasArrived(struct Actor *object);

void AirsRockInside_SetConeCells(struct Actor *object);

void Pillars_Place(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    object->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->layer);
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}

/// Finds the current target's table row and shrinks it.
void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  while (*list != 0xffff) {
    s32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= FIXED_ONE * 31 / 256;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, a, flag, object->layer);
        object->scaleY = FIXED_ONE;
      }
      break;
    }
    list += 3;
  }
}

#include "shared/actor_set_cell_open.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern struct ActorPlacement gAirsRockInside_TightropesPlacements[];
extern struct ActorPlacement gAirsRockInside_ElixirChestPlacements[];
extern struct ActorPlacement gAirsRockInside_PairedStatuesPlacements[];
extern struct ActorPlacement gAirsRockInside_FaceWarpPlacements[];
extern s32 gAirsRockInside_CoinsChestPlacements[];

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

/// On most frames, moves the object to a random spot: x 176 to 431 pixels, y
/// and z up to 31 and 127 pixels past the leader's. Then it keeps its action as
/// the previous one and sets mode 9 after action 212, mode 10 otherwise.
void AirsRockInside_ScatterMote(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  if (Random16() & 0xff) {
    object->x.fixed = INTtoFIXED(Random16() & 0xff) + toFIXED(176);
    object->y.fixed = leader->y.fixed + INTtoFIXED(Random16() & 31);
    object->z.fixed = leader->z.fixed + INTtoFIXED(Random16() & 127);
  }
  object->userValue = object->timer;
  if (object->timer == 212)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, 10);
}

/// A mote's update, stopped while a menu or Psynergy is up.
void AirsRockInside_DriftMote(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(object, ANIMATION_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if (object->riseEnabled != 0) {
    if ((gFrameCount & 7) == 0)
      object->riseEnabled--;
    return;
  }
  if (object->userValue == 0)
    AirsRockInside_ScatterMote(object);
  object->userValue--;
  object->x.fixed +=
      Trig_Cos(((object->z.fixed >> 14) + (object->y.fixed >> 15)) << 8) / 2;
  object->y.fixed += object->speedLimit;
}

/// Whirlwind on a twister rock or statue: the scene's own effect runs in place
/// of Whirlwind's.
void AirsRockInside_StartWhirlwind(void) { Event_ResolvePendingActions(0); }

void AirsRockInside_TakeOverWhirlwind(void) {
  ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->childOption = 1;
}

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_idle.c"

#include "../field/map_cell_get.c"

/// Sets an object's cells by its flag (its action parameter): when the flag is
/// set it clears the kind byte of its layer 2 cell and its layer 0 cell, sinks
/// 4 pixels when not on its ground and takes sprite priority 3; otherwise it
/// marks both cells (0xff00 and 0xd480), rises 4 pixels and takes priority 1;
/// then it writes both cells back and moves the object's height
/// (MapGrid_SetCellHeight).
void AirsRockInside_SetConeCells(struct Actor *object) {
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  s32 ground = MapCell_GetAttributes(0, x, z);
  s32 cell = MapCell_GetAttributes(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed) >>
      19;
  s32 set = GameFlag_Get(object->timer);
  u8 *flags = &object->priorityFlags;
  s32 px = x << 20;
  s32 pz = z << 20;

  if (set) {
    cell &= 0xffff00ff;
    ground = 0;
    if (object->groundHeight >> 19 != height)
      height -= 4;
    // Through the field: through flags the byte, not the 2, would take the
    // result register.
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    cell |= 0xff00;
    ground |= 0xd480;
    *flags &= ~2;
    height += 4;
    Scene_SetActorSpritePriority(object, 1);
  }
  MapCell_SetAttributes(0, x, z, ground);
  MapCell_SetAttributes(2, x, z, cell);
  MapGrid_SetCellHeight(px, pz, object->layer, height);
}

/// One step of two paired objects trading places: the effect work's halfword
/// at 26 names the first, and its partner is the other id of its even and odd
/// pair. On step 0 the partner rises 28 pixels, its sprite tiles are cleared
/// and the first's copied over them; on steps 1 to 5 it sinks and 0x30 words
/// more are copied; through step 7 the first flattens. In mode 1 both settle,
/// the first's flag is set and the partner's cleared, their animations step
/// and their cells are written again (AirsRockInside_SetConeCells).
void AirsRockInside_PoundCone(s32 mode, s32 step) {
  s16 *pair = (s16 *)((u8 *)Ram_HeapSlots->fieldEffectWork + 26);
  s32 even = *pair - ((*(u16 *)pair & 1) << 1);
  struct Actor *first = Actor_GetById(*pair);
  struct Actor *second = Actor_GetById(even + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramCacheEntry *entry;

  if (step == 0) {
    firstSprite = first->sprite;
    secondSprite = second->sprite;
    second->motionFlags = 0;
    second->y.fixed -= toFIXED(28);
    wait(1);
    entry = &gVramCacheEntries[secondSprite->vramBlock];
    second->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
    entry = &gVramCacheEntries[firstSprite->vramBlock];
    first->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
    Dma_Fill32(step, second->watchedActor, entry->size);
    Dma_Set(first->watchedActor, second->watchedActor, DMA_COPY_WORDS(192),
            REG_DMA3);
    first->watchedActor = (void *)((u8 *)first->watchedActor + 0x300);
    second->watchedActor = (void *)((u8 *)second->watchedActor + 0x300);
  } else if (step <= 5) {
    second->y.fixed += toFIXED(26) / 5;
    Dma_Set(first->watchedActor, second->watchedActor, DMA_COPY_WORDS(48),
            REG_DMA3);
    first->watchedActor = (void *)((u8 *)first->watchedActor + 0x100);
    second->watchedActor = (void *)((u8 *)second->watchedActor + 0x100);
  }
  if (step <= 7)
    first->scaleY += -(FIXED_ONE * 31 / 256);
  if (mode == 1) {
    first->scaleY = FIXED_ONE;
    second->y.fixed = second->groundHeight;
    GameFlag_Set(first->timer);
    GameFlag_Clear(second->timer);
    Actor_SelectAnimation(first,
                          ((struct FieldSprite *)first->sprite)->frame + 1);
    Actor_SelectAnimation(second,
                          ((struct FieldSprite *)second->sprite)->frame - 1);
    AirsRockInside_SetConeCells(first);
    AirsRockInside_SetConeCells(second);
  }
}

/// Places the objects of a list ended by 0xffff: each entry (three halfwords)
/// names an object, the first of its three script choices and the flag that
/// picks between them; the object stops moving, loses its solid bit and keeps
/// the flag as its action parameter.
void AirsRockInside_PlaceCones(u16 *list) {
  struct Actor *object;
  s32 flag, base;
  u16 *entry;

  if (*list == 0xffff)
    return;
  entry = list;
  do {
    object = Actor_GetById(*entry);
    base = ((s16 *)entry)[1];
    flag = ((s16 *)entry)[2];
    Actor_SetSpriteFlags(object, 0);
    {
      s32 choice = GameFlag_Get((u16)flag);

      // The two shifts stay statements: as (u16)base, base loses its rank.
      base <<= 16;
      base = (u32)base >> 16;
      base += base << 1;
      base += choice;
    }
    Actor_SelectAnimation(object, base);
    object->motionFlags = 0;
    object->collisionFlags &= ~ACTOR_SOLID;
    object->timer = flag;
    AirsRockInside_SetConeCells(object);
    entry += 3;
  } while (*entry != 0xffff);
}

/// The leader rides a statue's whirlwind along its path until it fades.
void AirsRockInside_RideWhirlwind(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *ride = leader->watchedActor;
  struct Actor *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u16 facing;
  s32 angle, noFlags;

  Event_Start();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_50);
  Sound_Play(SE_KIRIN);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x.fixed = ride->x.fixed;
    leader->y.fixed = ride->y.fixed + rise;
    leader->z.fixed = ride->z.fixed;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL &&
      leader->x.fixed >> 19 == 39) {
    Event_SetExitRequest(16);
    Event_End();
    return;
  }
  priority = &leader->priorityFlags;
  facing = ride->facing;
  switch (facing) {
  case 0xc000:
    rider = ride->watchedActor;
    dx = 0;
    dz = 0;
    *priority = 4;
    leader->x.fixed = (((leader->x.fixed >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y.fixed = (((ride->y.fixed >> 19) & ~1) + 3) << 19;
    leader->z.fixed = (((leader->z.fixed >> 19) & ~1) + 1) << 19;
  waiting:
    moved = 0;
    if (gInput.held & KEY_UP) {
      dx = 0;
      dz = -toFIXED(16);
      leader->facing = FACING_NORTH;
      moved = 1;
    }
    if (gInput.held & KEY_RIGHT) {
      dx = toFIXED(16);
      dz = 0;
      leader->facing = 0;
      moved = 1;
    }
    if (gInput.held & KEY_LEFT) {
      dx = -toFIXED(16);
      leader->facing = FACING_WEST;
      dz = 0;
      moved = 1;
    }
    if (moved &&
        MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed + dx),
                              (ride->z.fixed - ride->y.fixed + dz) >> 20) >>
                8 !=
            255)
      goto found;
    wait(1);
    i++;
    if (i != 60)
      goto waiting;
    dx = 0;
    dz = -toFIXED(16);
    leader->facing = FACING_NORTH;
    if ((u32)(MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed),
                                    (ride->z.fixed - ride->y.fixed + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = toFIXED(16);
    dz = 0;
    leader->facing = 0;
    if ((u32)(MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed + dx),
                                    (ride->z.fixed - ride->y.fixed) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = -toFIXED(16);
    leader->facing = FACING_WEST;
  found:
    height = Map_GetTerrainHeight(rider->riseCounter, ride->x.fixed + dx,
                                  ride->z.fixed + dz);
    leader->y.fixed = height;
    leader->groundHeight = height;
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(2);
    leader->acceleration = toFIXED(1) / 2;
    leader->velocityY = toFIXED(4);
    *motion = 2;
    *priority = 1;
    leader->layer = rider->riseCounter;
    Actor_SetMoveTarget(leader, leader->x.fixed + dx, leader->y.fixed,
                        leader->z.fixed + dz);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    *motion = 3;
    break;
  default:
    angle = ride->facing;
    leader->x.fixed = (((ride->x.fixed >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z.fixed = (((ride->z.fixed >> 19) & ~1) - (angle & 2) + 1) << 19;
    noFlags = 0;
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = toFIXED(6);
    *priority = 33;
    *motion = 3;
    FieldCamera_MoveToFX(-1, -1, -1, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = toFIXED(8);
    dust->acceleration = toFIXED(1);
    dust->motionFlags = noFlags;
    Actor_SetMoveTarget(
        dust, leader->x.fixed,
        Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
        leader->z.fixed);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(dust);
    break;
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  GameFlag_Clear(FLAG_SCENE_TEMP_50);
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

// AirsRockInside_MovePooledWhirlwind's twin for another shot: it also stops
// while the render mode is 5, and on a cell of kind 212 it ends with one of two
// end scripts by its angle; otherwise, the first time it reaches the leader
// (within 16 pixels on the same height when the leader climbs, else within 8 on
// the same level) and flag 0x104 is clear, it carries the leader along (counter
// 0x16a 200, render mode 0); it moves 1.5 pixels a frame along its angle,
// upward when its angle is 0xc000.

extern u32 gAirsRockInside_WhirlwindFadesScript[];
extern u32 gAirsRockInside_WhirlwindRisesScript[];

/// AirsRockInside_MovePooledWhirlwind's twin for another shot: it also stops
/// while the render mode is 5, and on a cell of kind 212 it ends with one of
/// two end scripts by its angle; otherwise, the first time it reaches the
/// leader (within 16 pixels on the same height when the leader climbs, else
/// within 8 on the same level) and flag 0x104 is clear, it carries the leader
/// along (counter 0x16a 200, render mode 0); it moves 1.5 pixels a frame along
/// its angle, upward when its angle is 0xc000.
void AirsRockInside_MoveWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = toFIXED(3) / 2;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || gMainRoutineWork.movementMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, ANIMATION_STEP_NORMAL);
  if (MapCell_GetAttributes(2, FIELD_CELL(shot->x.fixed),
                            (shot->z.fixed - shot->y.fixed) >> 20) >>
          8 ==
      212) {
    shot->riseEnabled = 0;
    shot->update = 0;
    Call2((void (*)())Actor_PlaySoundInView, (s32)shot, 539);
    if (shot->facing == FACING_NORTH)
      Actor_StartScript(shot, gAirsRockInside_WhirlwindFadesScript);
    else
      Actor_StartScript(shot, gAirsRockInside_WhirlwindRisesScript);
    return;
  }
  if (shot->riseCounter == 0) {
    near = 0;
    leaderPosition = &leader->x.fixed;
    shotPosition = &shot->x.fixed;
    if (gMainRoutineWork.movementMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        near = 1;
    }
    if (near && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      work->raisedTrigger = 200;
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
      shot->riseCounter = 1;
      shot->riseEnabled = 1;
    }
  }
  if (shot->facing == FACING_NORTH) {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulFX(Trig_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulFX(Trig_Sin(shot->facing), speed);
  }
}

/// A launcher's update with a new shot each time: while none of the event
/// work's four busy counters is set it counts frames in its action parameter,
/// and at 240 launches a shot 8 pixels ahead along its angle with a dust effect
/// below it.
void AirsRockInside_StatueBreathesWhirlwind(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  if (object->timer == 240) {
    if (!GameFlag_Get(FLAG_SCENE_TEMP_50))
      Actor_PlaySoundInView(object, SE_FLAME_FLARE);
    if (object->facing == FACING_NORTH) {
      position.x = object->x.fixed;
      position.z = object->z.fixed;
    } else {
      position.x =
          object->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(object->facing));
      position.z =
          object->z.fixed + Math_MulFX(toFIXED(8), Trig_Sin(object->facing));
    }
    shot = (struct Actor *)Import_ActorCreate(SPRITE_WHIRLWIND, position.x,
                                              object->y.fixed, position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    shot->riseCounter = 0;
    shot->riseEnabled = 0;
    shot->facing = object->facing;
    shot->update = AirsRockInside_MoveWhirlwind;
    shot->watchedActor = object;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y.fixed, position.z - toFIXED(6), 0,
                      0, 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

/// Wakes a statue: it starts breathing whirlwinds after the frames given.
void AirsRockInside_WakeStatue(struct Actor *object, s16 timer, u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    object->steeringFlags = 0;
    object->timer = timer;
    object->riseCounter = value;
    object->update = (s32)AirsRockInside_StatueBreathesWhirlwind;
  }
}

extern u32 gAirsRockInside_PooledWhirlwindFadesScript[];

void AirsRockInside_MovePooledWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = toFIXED(3) / 2;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || work->transitionActive == 0) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, ANIMATION_STEP_NORMAL);
  if (MapCell_GetAttributes(2, FIELD_CELL(shot->x.fixed),
                            (shot->z.fixed - shot->y.fixed) >> 20) >>
          8 ==
      212) {
    {
      s32 cue = 284;

      Actor_PlaySoundInView(shot, cue + 255);
    }
    shot->riseEnabled = 0;
    shot->update = 0;
    Actor_StartScript(shot, gAirsRockInside_PooledWhirlwindFadesScript);
    /* FAKEMATCH(dead): an empty once-only block holds the next statement after
     * this one. */
    do {
    } while (0);
    return;
  }
  if (shot->riseCounter == 0) {
    near = 0;
    leaderPosition = &leader->x.fixed;
    shotPosition = &shot->x.fixed;
    if (gMainRoutineWork.movementMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        near = 1;
    }
    if (near && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      work->raisedTrigger = 200;
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
      shot->riseCounter = 1;
      shot->riseEnabled = 1;
    }
  }
  if (shot->facing == FACING_NORTH) {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulFX(Trig_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulFX(Trig_Sin(shot->facing), speed);
  }
}

/// An object's update: while none of the event work's four busy counters
/// (0x15a, 0x15e, 0x166, 0x168) is set it counts frames in its action
/// parameter, and at 240 (playing cue 202 unless flag 0x250 is set) it launches
/// the first free one of objects 13 to 15 from 8 pixels ahead along its angle,
/// with AirsRockInside_MovePooledWhirlwind as its update, and a dust effect
/// below it.
void AirsRockInside_StatueBreathesPooledWhirlwind(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;
  s32 i;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  for (i = 0; i != 3;)
    Actor_GetById(13 + i++);
  if (object->timer == 240) {
    shot = 0;
    if (!GameFlag_Get(FLAG_SCENE_TEMP_50))
      Actor_PlaySoundInView(object, SE_FLAME_FLARE);
    if (object->facing == FACING_NORTH) {
      position.x = object->x.fixed;
      position.z = object->z.fixed;
    } else {
      position.x =
          object->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(object->facing));
      position.z =
          object->z.fixed + Math_MulFX(toFIXED(8), Trig_Sin(object->facing));
    }
    for (i = 0; i != 3; i++) {
      shot = Actor_GetById(13 + i);
      if (shot->update == 0)
        break;
    }
    shot->x.fixed = position.x;
    shot->y.fixed = object->y.fixed;
    shot->z.fixed = position.z;
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    shot->motionFlags = 0;
    shot->riseCounter = 0;
    shot->riseEnabled = 0;
    shot->facing = object->facing;
    shot->watchedActor = object;
    shot->update = AirsRockInside_MovePooledWhirlwind;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y.fixed, position.z - toFIXED(6), 0,
                      0, 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void AirsRockInside_WakePooledStatue(struct Actor *object, s16 timer,
                                     u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    object->steeringFlags = 0;
    object->timer = timer;
    object->riseCounter = value;
    object->update = (s32)AirsRockInside_StatueBreathesPooledWhirlwind;
    Actor_SetSpriteFlags(object, 0);
  }
}

void AirsRockInside_UpdateVentPuff(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *other;
  s32 *pos;
  u8 i;

  if (*(s8 *)((u8 *)Ram_HeapSlots->paletteFade + 0x2a01) != 0)
    return;
  if (work->transitionActive == 0)
    return;
  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, ANIMATION_STEP_NORMAL);
  if (object->timer == 0) {
    if (object->x.fixed == 0) {
      Actor_Release(object);
    } else {
      object->z.fixed = 0;
      object->x.fixed = 0;
    }
    return;
  }
  if (Position_FlatDistance(&leader->x.fixed, pos = &object->x.fixed) <= 8 &&
      leader->y.part[0].integer == object->y.part[0].integer) {
    work->raisedTrigger = 201;
    object->riseBaseY = 0;
    object->speedLimit = -object->speedLimit >> 1;
    object->z.fixed = leader->z.fixed - toFIXED(8);
  }
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    other = Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR);
    if (Position_FlatDistance(&other->x.fixed, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->riseBaseY = 0;
      object->z.fixed = other->z.fixed - toFIXED(8);
    }
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
    for (i = 0; i != 3; i++) {
      struct Actor *pillar = Actor_GetById(i + ACTOR_FACE_WARP_THIRD_PILLAR);
#define other pillar
      if (Position_FlatDistance(&other->x.fixed, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->riseBaseY = 0;
        object->z.fixed = other->z.fixed - toFIXED(8);
      }
    }
#undef other
  }
  object->timer--;
  object->z.fixed += object->speedLimit;
  object->x.fixed += object->acceleration;
  object->speedLimit -= object->riseBaseY;
  object->scaleX += FIXED_ONE / 20;
  object->scaleY += FIXED_ONE / 20;
}

/// A puff of air rising from a vent.
void AirsRockInside_PuffVent(s32 x, s32 y, s32 z) {
  struct Actor *object = Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);

  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetSpriteColor(object, 6);
  Actor_SetBlendMode(object, 1);
  object->timer = 27;
  object->motionFlags = 0;
  object->steeringFlags = 0;
  object->scaleX = FIXED_ONE / 2;
  object->speedLimit = toFIXED(4);
  object->acceleration = ((Random16() & 0x7fff) << 1) + 0xffff8001;
  object->riseBaseY = toFIXED(7) / 32;
  object->update = (s32)AirsRockInside_UpdateVentPuff;
}

/// Every eighth frame pair, while none of the event work's four busy counters
/// is set, starts a drip (AirsRockInside_PuffVent) at a place chosen by scene
/// and frame: in one room one of two spots by
/// FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, in the other one
/// of six by the leader's position and flags 0x200 to 0x202. Places the two
/// frame arms share are one call, reached by goto from the other arm.
void AirsRockInside_PuffVents(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 frame;
  s32 scene;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  frame = gFrameCount;
  if ((frame & 6) != 0)
    return;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    s32 x;

    if ((frame & 1) &&
        !GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
      x = toFIXED(376);
    else
      x = toFIXED(280);
    AirsRockInside_PuffVent(x, 0, toFIXED(616));
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
    if (frame & 1) {
      if (Actor_GetById(gMainRoutineWork.leaderActorId)->z.fixed >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.leaderActorId)->x.fixed >> 19 <= 44)
        puffWestVent:
          AirsRockInside_PuffVent(toFIXED(168), 0, toFIXED(603));
        else if (!GameFlag_Get(FLAG_SCENE_TEMP_01))
          AirsRockInside_PuffVent(toFIXED(504), 0, toFIXED(456));
        else
          goto puffEastVent;
      } else {
      puffNorthVent:
        AirsRockInside_PuffVent(toFIXED(616), -toFIXED(32), toFIXED(136));
      }
    } else {
      if (Actor_GetById(gMainRoutineWork.leaderActorId)->z.fixed >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.leaderActorId)->x.fixed >> 19 > 49)
        puffEastVent:
          AirsRockInside_PuffVent(toFIXED(552), 0, toFIXED(456));
        else if (GameFlag_Get(FLAG_SCENE_TEMP_00))
          goto puffWestVent;
        else
          AirsRockInside_PuffVent(toFIXED(264), 0, toFIXED(600));
      } else if (GameFlag_Get(FLAG_SCENE_TEMP_02)) {
        goto puffNorthVent;
      } else {
        AirsRockInside_PuffVent(toFIXED(648), -toFIXED(32), toFIXED(136));
      }
    }
  }
}

void AirsRockInside_FollowGust(struct Actor *object) {
  struct Actor *target = object->watchedActor;

  object->priorityFlags ^= 2 << (gFrameCount & 1);
  SET_ACTOR_POSITION(object, target->x.fixed, target->y.fixed + (128 << 12),
                     target->z.fixed);
}

void AirsRockInside_SpiralGust(struct Actor *object);

#define TARGET(effect) ((effect)->child)

/// The gust picks up an object it passes over, to drop it at cell `at`
/// (x in the high half, z in the low).
#define PICK_UP(object, at)                                                    \
  do {                                                                         \
    (object)->watchedActor = gust;                                             \
    (object)->update = AirsRockInside_FollowGust;                              \
    (object)->motionFlags = 0;                                                 \
    (object)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;                      \
    Actor_SetSpriteFlags(object, 0);                                           \
    ((struct FieldSprite *)(object)->sprite)->priority =                       \
        ((struct FieldSprite *)gust->sprite)->priority;                        \
    gust->watchedActor = (object);                                             \
    carried = (at);                                                            \
  } while (0)

/// One shake of the walls (cue 157).
#define SHAKE()                                                                \
  (Sound_Play(SE_GATE_MOVE), MapGrid_SetShake(5, 5, 1),                        \
   MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9))

/// Whirlwind on a twister rock: a gust rises from it and travels ahead of the
/// leader, carrying what it meets.
void AirsRockInside_BlowGust(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct Actor *gust, *spark, *object;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell dir, cell;
  s32 i, steps = 0, carried;
  s32 hit = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_RUSH);
  gust = (struct Actor *)Import_ActorCreate(SPRITE_WHIRLWIND, leader->x.fixed,
                                            leader->y.fixed, leader->z.fixed);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = FIXED_ONE * 17 / 10;
  gust->scaleY = FIXED_ONE / 5;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read as long, an alias set outside
   * struct Actor's. */
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->child)->sprite)
          ->priority;
  Actor_SetMoveTarget(gust, TARGET(effect)->x.fixed, TARGET(effect)->y.fixed,
                      TARGET(effect)->z.fixed - toFIXED(1));
  gust->groundHeight = Map_GetTerrainHeight(0, gust->x.fixed, gust->z.fixed);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += FIXED_ONE / 10;
    FieldEvent_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z.fixed = TARGET(effect)->z.fixed - toFIXED(1);
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          SPRITE_PARTICLE, TARGET(effect)->x.fixed, TARGET(effect)->y.fixed,
          TARGET(effect)->z.fixed);
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->groundHeight =
          Map_GetTerrainHeight(0, spark->x.fixed, spark->z.fixed);
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(4) / 5;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = AirsRockInside_SpiralGust;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetSpriteColor(TARGET(effect), 15);
    } else {
      Actor_SetSpriteColor(TARGET(effect), 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetSpriteColor(TARGET(effect), 0);
  Sound_Play(SE_WHOOSH);
  cell.x = FIELD_CELL(TARGET(effect)->x.fixed);
  cell.z = FIELD_CELL(TARGET(effect)->z.fixed);
  if (FIELD_CELL(leader->x.fixed) < cell.x) {
    dir.x = 1;
    dir.z = 0;
  } else if (FIELD_CELL(leader->x.fixed) > cell.x) {
    dir.x = -1;
    dir.z = 0;
  } else if (FIELD_CELL(leader->z.fixed) < cell.z) {
    dir.x = 0;
    dir.z = 1;
  } else {
    dir.x = 0;
    dir.z = -1;
  }
  do {
    hit = MapCell_GetAttributes(2, cell.x, cell.z);
    hit >>= 8;
    if (Map_GetTerrainHeight(2, cell.x << 20, cell.z << 20) !=
        gust->groundHeight)
      hit = 0;
    cell.x += dir.x;
    cell.z += dir.z;
    steps += 2;
  } while (hit == 0);
  MapCell_GetAttributes(2, cell.x, cell.z);
  {
    s32 x;
    s32 z;

    steps -= 2;
    x = FIXEDtoINT(TARGET(effect)->x.fixed) + ((steps * dir.x) << 3);
    z = FIXEDtoINT(TARGET(effect)->z.fixed) + ((steps * dir.z) << 3);
    ((struct FieldSprite *)gust->sprite)->priority = 1;
    cell.x = x;
    cell.z = z;
    gust->speedLimit = toFIXED(4);
    gust->acceleration = toFIXED(4);
    Actor_SetMoveTarget(gust, INTtoFIXED(cell.x), gust->y.fixed,
                        INTtoFIXED(cell.z));
    FieldCamera_SetSpeedAndAcceleration(5, 5);
    FieldCamera_MoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
  }
  carried = 0;
  while (Actor_HasArrived(gust) == 0) {
    if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
      object = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR);
      if (gust->x.fixed >> 19 == object->x.fixed >> 19 && carried == 0)
        PICK_UP(object, 0x270025);
    } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
      object = Actor_GetById(ACTOR_FACE_WARP_SECOND_PILLAR);
      if (FIELD_CELL(gust->x.fixed) == FIELD_CELL(object->x.fixed) &&
          FIELD_CELL(gust->z.fixed) == FIELD_CELL(object->z.fixed) &&
          carried == 0)
        PICK_UP(object, 0x290069);
    } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
      object = Actor_GetById(ACTOR_COINS_CHEST_STATUE);
      if (FIELD_CELL(gust->x.fixed) == FIELD_CELL(object->x.fixed) &&
          FIELD_CELL(gust->z.fixed) == FIELD_CELL(object->z.fixed) &&
          carried == 0)
        PICK_UP(object, 0xf0033);
    }
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y.fixed + INTtoFIXED(Random16() & 31);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
      FieldEffect_Spawn(
          gust->x.fixed, velocity.y, gust->z.fixed, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &leaves);
    }
    FieldEvent_Wait(1);
  }
  if (carried != 0) {
    object = gust->watchedActor;
    object->x.fixed = (carried >> 16) << 19;
    object->z.fixed = carried << 19;
    object->velocityY = toFIXED(8);
    object->update = 0;
    object->motionFlags = ACTOR_FALLS;
    object->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    object->priorityFlags &=
        ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
    Actor_SetSpriteFlags(object, 1);
    Scene_SetActorSpritePriority(object, 2);
    if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
      object->groundHeight = -toFIXED(32);
      MapGrid_CopyCellAttributes(19, 16, 1, 1, 19, 18);
      GameFlag_Set(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
    } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
      MapGrid_CopyCellAttributes(19, 38, 1, 1, 20, 38);
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      object->motionFlags = 0;
      object->groundHeight = 0;
      object->y.fixed = 0;
      MapGrid_SetCellHeight(0x1400000, 0x3400000, 0, 4);
    } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      Actor_SetSpriteFlags(object, 0);
      object->motionFlags = 0;
      object->groundHeight = 0;
      object->y.fixed = 0;
      MapGrid_SetCellHeight(0x780000, 0x1980000, 0, 4);
      if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
        AirsRockInside_WakeStatue(object, 60, 0);
    }
  }
  if (hit == 255) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = FIXEDtoINT(gust->x.fixed) + dir.x * 48;
    cell.z = FIXEDtoINT(gust->z.fixed) + dir.z * 48;
    gust->speedLimit = toFIXED(1) / 2;
    gust->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(gust, INTtoFIXED(cell.x), gust->y.fixed,
                               INTtoFIXED(cell.z));
    hit -= 30;
    i = 0;
    while (Actor_HasArrived(gust) == 0) {
      if (hit > 4)
        goto shrink;
      if (i == 0) {
        Sound_Play(SE_GUST);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        {
          dust.startScaleX = dust.startScaleY =
              (Random16() & 0xffff) + FIXED_ONE;
        }
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = toFIXED(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
        FieldEffect_Spawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
                          velocity.x, velocity.y, velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
          GameFlag_Set(FLAG_AREA_TEMP_00);
          cell.x = FIELD_CELL(gust->x.fixed) + 64;
          cell.z = FIELD_CELL(gust->z.fixed);
          MapGrid_CopyCellPictureIndicesRect(90, 25, cell.x + 1, cell.z, 1, 3);
          MapGrid_CopyCellPictureIndicesRect(21, 88, 24, 89, 1, 2);
          Map_CopyCellAttributeRect(23, 92, 2, 1, 23, 89);
          Map_CopyCellAttributeRect(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineWork.next ==
                   SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES) {
          cell.x = FIELD_CELL(gust->x.fixed);
          cell.z = FIELD_CELL(gust->z.fixed - gust->y.fixed) + 64;
          if (hit == 0) {
            GameFlag_Set(FLAG_AREA_TEMP_04);
            MapGrid_CopyCellPictureIndicesRect(39, 82, cell.x, cell.z, 1, 1);
            Map_CopyCellAttributeRect(39, 82, 1, 1, 38, 82);
            MapGrid_CopyCellAttributes(38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_Set(FLAG_AREA_TEMP_05);
            MapGrid_CopyCellsTo(39, 82, cell.x, cell.z, 1, 1);
            MapGrid_CopyCellAttributes(38, 98, 1, 1, 38, 99);
            MapGrid_CopyCellAttributes(38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineWork.next ==
                   SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
          GameFlag_Set(FLAG_AREA_TEMP_06);
          cell.x = FIELD_CELL(gust->x.fixed);
          MapGrid_CopyCellsTo(15, 80, cell.x, 77, 1, 3);
          MapGrid_CopyCellAttributes(15, 80, 1, 1, 15, 79);
          MapGrid_CopyCellAttributes(17, 15, 2, 1, 15, 15);
        }
        MapGrid_CenterOnFollowedPoint();
      }
      FieldEvent_Wait(1);
    }
    Sound_Play(SE_STOP_EXIT_SE);
  }
shrink:
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= FIXED_ONE * 3 / 20;
    gust->scaleY -= FIXED_ONE / 4;
    gust->y.fixed += toFIXED(7) / 2;
    FieldEvent_Wait(1);
  }
  Actor_Release(gust);
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    if (hit == 5) {
      SHAKE();
      MapGrid_CopyCellsTo(79, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      MapGrid_CopyCellsTo(69, 0x40, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(46, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(0x20, 89, 1, 1, 31, 89);
      MapGrid_CopyCellAttributes(50, 86, 1, 1, 33, 87);
      GameFlag_Set(FLAG_AREA_TEMP_01);
    } else if (hit == 6) {
      SHAKE();
      MapGrid_CopyCellsTo(74, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      MapGrid_CopyCellsTo(0x40, 0x40, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(52, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(33, 88, 1, 1, 33, 87);
      MapGrid_CopyCellAttributes(0x30, 88, 1, 1, 31, 89);
      GameFlag_Clear(FLAG_AREA_TEMP_01);
    }
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    if (hit == 5) {
      SHAKE();
      MapGrid_CopyCellsTo(79, 0x40, 0x20, 79, 5, 7);
      if (Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed >> 19 == 65) {
        velocity.x =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed - toFIXED(8);
        velocity.y = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
        velocity.z =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed + toFIXED(8);
        Actor_SetSpritePriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + INTtoFIXED(Random16() & 15),
                            velocity.y + INTtoFIXED(Random16() & 31),
                            velocity.z, 0, 0, -0x8000, EFFECT_USE_PRIORITY,
                            &dust);
        }
        wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(0x40, 0x40, 0x20, 79, 5, 7);
        Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(69, 0x40, 0x20, 79, 5, 7);
        MapGrid_CopyCellAttributes(49, 15, 5, 5, 0x20, 15);
        MapGrid_CopyCellAttributes(33, 82, 1, 1, 34, 82);
        MapGrid_CopyCellAttributes(52, 81, 1, 1, 36, 81);
        GameFlag_Clear(FLAG_AREA_TEMP_02);
      }
    } else if (hit == 6) {
      SHAKE();
      if (Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed >> 19 == 65) {
        velocity.x =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed - toFIXED(8);
        velocity.y = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
        velocity.z = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + INTtoFIXED(Random16() & 15),
                            velocity.y,
                            velocity.z - INTtoFIXED(Random16() & 31), 0, 0,
                            -(toFIXED(1) / 2), EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        MapGrid_CopyCellsTo(79, 0x40, 0x20, 79, 5, 7);
        Sound_Play(SE_GATE_MOVE);
        FieldEvent_Wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(0x40, 0x40, 0x20, 79, 5, 7);
        MapGrid_CopyCellAttributes(49, 21, 5, 5, 0x20, 15);
        MapGrid_CopyCellAttributes(35, 81, 1, 1, 36, 81);
        MapGrid_CopyCellAttributes(50, 82, 1, 1, 34, 82);
        GameFlag_Set(FLAG_AREA_TEMP_02);
      }
    }
  }
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

extern struct ActorPlacement gAirsRockInside_Entrances[];
extern s16 gAirsRockInside_HallEntrances[];

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallEntrances;
  return gAirsRockInside_Entrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
    return gAirsRockInside_VialChestPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
    return gAirsRockInside_ClarityCircletChestPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
    return gAirsRockInside_TightropesPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return gAirsRockInside_RevealTabletPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
    return gAirsRockInside_ElixirChestPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES)
    return gAirsRockInside_PairedStatuesPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP)
    return gAirsRockInside_FaceWarpPlacements;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
    return gAirsRockInside_CoinsChestPlacements;
  return gAirsRockInside_Placements;
}

void AirsRockInside_RestoreCellBehindLeader(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  struct FieldWork *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gAirsRockInside_StampedCell;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gAirsRockInside_StampedCell = -1;
  MapGrid_CopyCellsLayered(73, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
  if (gMainRoutineWork.next != SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return;
  work = Ram_HeapSlots->fieldWork;
  value = MapCell_GetAttributes(2, x, z) >> 8;
  if (value != 0) {
    work->raisedTrigger = value + 200;
  }
}

void AirsRockInside_StartRestoringCells(void) {
  s32 value = -1;

  gAirsRockInside_StampedCell = value;
  {
    s32 priority = 0x480;
    Task_Add(AirsRockInside_RestoreCellBehindLeader, priority);
  }
}

/// As the leader steps onto a new cell, saves the cells under it and stamps the
/// floor's marked cells in their place; AirsRockInside_RestoreCellBehindLeader
/// puts them back once he has left.
void AirsRockInside_StampCellUnderLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gAirsRockInside_StampedCell)
    return;
  gAirsRockInside_StampedCell = cell;
  gAirsRockInside_StampedWidth = 1;
  gAirsRockInside_StampedHeight = 2;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
      scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST ||
      scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    gAirsRockInside_StampedX = x;
    gAirsRockInside_StampedZ = z - y + 64;
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
             scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST ||
             scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES ||
             scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
  }
  MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                           gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight, 73, 72);
  MapGrid_CopyCellsLayered(70, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
}

void AirsRockInside_StampOtherCellUnderLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gAirsRockInside_StampedCell)
    return;
  gAirsRockInside_StampedCell = cell;
  gAirsRockInside_StampedWidth = 1;
  gAirsRockInside_StampedHeight = 2;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
      scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    gAirsRockInside_StampedX = x;
    gAirsRockInside_StampedZ = z - y + 64;
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
             scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
  }
  MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                           gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight, 73, 72);
  MapGrid_CopyCellsLayered(71, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
}

/// When the controlled object steps onto a new cell, saves the 1 by 3 cells
/// below it at (73, 72) and stamps the cells kept at (72, 72) in their place.
#define DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(name, n1)                    \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(Party_GetLeaderActor());              \
    s32 x = FIELD_CELL(object->x.fixed);                                       \
    s32 y = FIELD_CELL(object->y.fixed);                                       \
    s32 z = FIELD_CELL(object->z.fixed);                                       \
    s32 key = (x << 16) + z;                                                   \
                                                                               \
    if (key != gAirsRockInside_StampedCell) {                                  \
      gAirsRockInside_StampedCell = key;                                       \
      gAirsRockInside_StampedWidth = 1;                                        \
      gAirsRockInside_StampedHeight = 3;                                       \
      gAirsRockInside_StampedX = x;                                            \
      gAirsRockInside_StampedZ = z - y + n1;                                   \
      MapGrid_CopyCellsLayered(x, gAirsRockInside_StampedZ, 1, 3, 73, 72);     \
      MapGrid_CopyCellsLayered(                                                \
          72, 72, gAirsRockInside_StampedWidth, gAirsRockInside_StampedHeight, \
          gAirsRockInside_StampedX, gAirsRockInside_StampedZ);                 \
    }                                                                          \
  }

DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
    AirsRockInside_StampTallCellUnderLeader, 64)

// As AirsRockInside_StampTallCellUnderLeader, one row nearer.
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
    AirsRockInside_StampTallCellUnderLeaderNearer, 63)

/// A burst of dust at x, z.
void AirsRockInside_BurstDust(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 spawn;
  s32 i;

  options.priority = 1;
  options.startScaleX = FIXED_ONE * 2;
  options.startScaleY = FIXED_ONE * 2;
  options.targetScaleX = FIXED_ONE / 2;
  options.targetScaleY = FIXED_ONE / 2;
  i = 0;
burst:
  spawn.x = x + INTtoFIXED(Random16() & 63) - toFIXED(32);
  spawn.y = (Random16() & 15) << 12;
  spawn.z = z + INTtoFIXED(Random16() & 7) - toFIXED(16);
  FieldEffect_Spawn(spawn.x, 0, spawn.z, 0, spawn.y, 0,
                    EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                        EFFECT_USE_PRIORITY | 1,
                    &options);
  i++;
  if (i != 8)
    goto burst;
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

void AirsRockInside_ScatterSparks(s32 x, s32 z) {
  struct AirsRockInsideSpawn spawn;
  struct AirsRockInsideVector position;
  s32 i;

  spawn.kind = 0x2a0;
  spawn.mode = 1;
  spawn.scaleX = FIXED_ONE * 4 / 5;
  spawn.scaleY = FIXED_ONE * 3;
  i = 0;
next:
  position.x = x + INTtoFIXED(Random16() & 31) - toFIXED(16);
  position.y = toFIXED(4);
  position.z = z;
  FieldEffect_Spawn(position.x, 0, position.z, 0, position.y, 0, toFIXED(26),
                    &spawn);
  FieldEvent_Wait(5);
  i++;
  if (i != 12)
    goto next;
}

void AirsRockInside_SpiralGust(struct Actor *object) {
  s32 radius = ((object->y.fixed - object->groundHeight) >> 1) + toFIXED(16);

  object->x.fixed =
      object->bounce +
      Iwram_MulFX(Trig_Cos((object->speedLimit & 255) << 11), radius);
  object->z.fixed =
      object->riseBaseY +
      Iwram_MulFX(Trig_Sin((object->speedLimit & 255) << 11), radius) / 3;
  object->y.fixed += object->gravity;
  object->speedLimit++;
}

/// Circles the object around its centre (fields 68 and 76) on an ellipse twice
/// as wide as deep, the radius growing with its height; it rises by field 72
/// each frame and switches its part attribute between 0 and 9 every two frames.
void AirsRockInside_SpiralSpark(struct Actor *object) {
  s32 y = object->y.fixed;
  s32 phase;
  s32 radius;

  phase = object->speedLimit;
  radius = toFIXED(12) + (y >> 2);
  object->x.fixed =
      object->bounce + Iwram_MulFX(Trig_Cos((phase & 255) << 11), radius) * 2;
  object->z.fixed =
      object->riseBaseY +
      Iwram_MulFX(Trig_Sin((object->speedLimit & 255) << 11), radius);
  object->y.fixed += object->gravity;
  object->speedLimit++;
  Actor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}

void AirsRockInside_FlickerTarget(struct Actor *object) {
  s32 bit = ((gFrameCount & 3) >> object->riseEnabled) & 1;

  Actor_SetSpriteColor(object, (u8)(bit * object->riseCounter));
}

/// Whirlwind on the hall's twister rock (object 11): it glows and the light it
/// sends wakes the statue (object 10), which starts breathing whirlwinds; sets
/// flag 0x89a.
void AirsRockInside_WakeHallStatue(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->riseCounter = 9;
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->riseEnabled = 1;
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->update = AirsRockInside_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(
          SPRITE_PARTICLE, Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->x.fixed,
          Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->y.fixed,
          Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = AirsRockInside_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeedFX(toFIXED(1), toFIXED(1) / 8);
  FieldCamera_MoveToFX(toFIXED(312), -1, toFIXED(680), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_ROCK), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_SetSource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellPictureIndicesRect(64, 78, 18, 105, 3, 3);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  PaletteFade_SetSource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  AirsRockInside_BurstDust(0x1380000, 0x2d00000);
  MapGrid_CopyCellsTo(64, 78, 18, 105, 3, 5);
  Camera_SetSpeedParameters(toFIXED(3) / 2, toFIXED(3) / 16);
  FieldCamera_MoveToActorById(10, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetSpriteColorById(ACTOR_HALL_TWISTER_STATUE, 0x100);
  AirsRockInside_ScatterSparks(
      Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->x.fixed,
      Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->z.fixed);
  Actor_ConfigureSpriteColor(ACTOR_HALL_TWISTER_STATUE, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 7);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_WEST_STATUE), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_EAST_STATUE), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 0);
  wait(40);
  AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN);
  Event_End();
}

/// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
/// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void AirsRockInside_StepOntoLedge(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 width;
  s32 height;

  if (MapCell_GetAttributes(1, FIELD_CELL(leader->x.fixed),
                            FIELD_CELL(leader->z.fixed) - 1) >>
          8 !=
      30)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  width = 1;
  height = 2;
  MapGrid_CopyCellPictureIndicesRect(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_GetById(gMainRoutineWork.leaderActorId)->speedLimit =
      Actor_GetById(gMainRoutineWork.leaderActorId)->acceleration =
          toFIXED(1) / 2;
  Sound_Play(SE_MAP_EXIT);
  Actor_WalkToById(gMainRoutineWork.leaderActorId, 312, 792);
  FieldEvent_Wait(5);
  Event_SetExitRequest(15);
  Event_End();
}

/// Runs each room's list on entering it; the paired statues room also clears
/// two cells' shapes and the coins chest room stops object 10's timer.
void AirsRockInside_StartPushingPillar(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL) {
    Actor_LiftTargetBlock(gAirsRockInside_HallPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    Actor_LiftTargetBlock(gAirsRockInside_VialChestPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    Actor_LiftTargetBlock(gAirsRockInside_ClarityCircletChestPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    Actor_LiftTargetBlock(gAirsRockInside_TightropesPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES) {
    FieldMapGrid_SetCellHeight(0x2600000, 0x1500000, 2, 0);
    MapGrid_SetCellHeight(0x2700000, 0x1500000, 2, 0);
    Actor_LiftTargetBlock(gAirsRockInside_PairedStatuesPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
    Actor_LiftTargetBlock(gAirsRockInside_FaceWarpPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    Actor_GetById(ACTOR_COINS_CHEST_STATUE)->timer = 0;
    Actor_LiftTargetBlock(gAirsRockInside_CoinsChestPillars);
  }
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

/// After the pushed object (the event work's object at 0xcbc) comes to rest:
/// if it stopped above the ground it drops onto it with cue 188; then, by
/// room, the place where it stopped sets the flag for that pillar's spot
/// (AirsRockInsideHall to 4 and 9), in AirsRockInsidePairedStatues marks two
/// cells and shows objects 13 and 14 by where objects 9, 10 and 11 stand, and
/// in AirsRockInsideFaceWarp and 9 sets or clears the flags of the spots along
/// its rows and redraws the pillars' cells (MapCell_SetAttributes).
void AirsRockInside_FinishPushingPillar(void) {
  struct Actor *pushed = *(struct Actor **)(EVENT_WORK + 0xcbc);
  s32 x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (Map_GetTerrainHeight(2, pushed->x.fixed, pushed->z.fixed) !=
      pushed->y.fixed) {
    pushed->layer = 2;
    pushed->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    OverlayObject_WaitUntilIdle(pushed);
    Sound_Play(SE_THUMP);
    OverlayObject_WaitUntilIdle(pushed);
    pushed->motionFlags = 0;
    pushed->layer = 0;
  }
  x = pushed->x.fixed >> 19;
  z = pushed->z.fixed >> 19;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
    if (x == 103 && z == 41)
      GameFlag_Set(FLAG_AIRS_ROCK_HALL_PILLAR_PLACED);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    if (x == 13 && z == 49)
      GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED);
    else if (x == 41 && z == 49)
      GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED);
  } else if (gMainRoutineWork.next ==
             SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    if (x == 33 && z == 79)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED);
    else if (x == 77 && z == 71)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED);
    else if (x == 59 && z == 65)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    if (x == 81 && z == 37)
      GameFlag_Set(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES) {
    FieldMapGrid_SetCellHeight(toFIXED(608), toFIXED(336), 2, -4);
    FieldMapGrid_SetCellHeight(toFIXED(624), toFIXED(336), 2, -4);
    if (x == 43 && z == 87)
      GameFlag_Set(FLAG_AIRS_ROCK_PAIRED_STATUES_PILLAR_PLACED);
    if ((Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_A)->x.fixed >> 19 == 73 &&
         Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_A)->z.fixed >> 19 == 43) ||
        (Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_B)->x.fixed >> 19 == 73 &&
         Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_B)->z.fixed >> 19 == 43))
      Actor_GetById(ACTOR_PAIRED_STATUES_CONE_B)->riseEnabled = 1;
    else
      Actor_GetById(ACTOR_PAIRED_STATUES_CONE_B)->riseEnabled = 0;
    if (pushed->x.fixed ==
            Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_C)->x.fixed &&
        pushed->z.fixed ==
            Actor_GetById(ACTOR_PAIRED_STATUES_PILLAR_C)->z.fixed) {
      if (x == 83 && z == 43)
        Actor_GetById(ACTOR_PAIRED_STATUES_CONE_C)->riseEnabled = 1;
      else
        Actor_GetById(ACTOR_PAIRED_STATUES_CONE_C)->riseEnabled = 0;
    }
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
    if (x == 35 && z == 59)
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_WARP_FIRST_PILLAR_PLACED);
    if (x == 45 && z == 105)
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_WARP_SECOND_PILLAR_PLACED);
    if (x == 23 && z == 77) {
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_WARP_THIRD_PILLAR_PLACED);
    }
    if (z == 59 && (x == 67 || x == 71 || x == 73))
      GameFlag_Set(FLAG_SCENE_TEMP_01);
    if (z == 19 && (x == 75 || x == 79))
      GameFlag_Set(FLAG_SCENE_TEMP_02);
    if (x == 69 && z == 59)
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
    if (x == 77 && z == 19)
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_FIRST_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_FIRST_PILLAR)->z.fixed), 0);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_SECOND_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_SECOND_PILLAR)->z.fixed), 0);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    if (x == 45 && z == 33)
      GameFlag_Set(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
  }
  Event_End();
}

#undef EVENT_WORK

/// Whirlwind on the Vial chest room's twister rock (object 10): BG0 is loaded
/// with the light it sends, which flashes across the room, opens the cells at
/// (68, 84) and wakes the statue (object 8); sets flag 0x899.
void AirsRockInside_WakeVialChestStatue(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->riseCounter = 9;
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->riseEnabled = 1;
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update =
      AirsRockInside_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(
          SPRITE_PARTICLE,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->x.fixed,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->y.fixed,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = AirsRockInside_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeedFX(toFIXED(1), toFIXED(1) / 8);
  FieldCamera_MoveToFX(toFIXED(216), -1, toFIXED(224), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_SetSource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellPictureIndicesRect(68, 84, 12, 77, 3, 3);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  PaletteFade_SetSource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  AirsRockInside_BurstDust(0xd80000, 0x12d0000);
  MapGrid_CopyCellsTo(68, 84, 12, 77, 3, 6);
  Camera_SetSpeedParameters(toFIXED(3) / 2, toFIXED(3) / 16);
  FieldCamera_MoveToActorById(8, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetSpriteColorById(ACTOR_VIAL_CHEST_STATUE, 0x100);
  AirsRockInside_ScatterSparks(Actor_GetById(ACTOR_VIAL_CHEST_STATUE)->x.fixed,
                               Actor_GetById(ACTOR_VIAL_CHEST_STATUE)->z.fixed +
                                   toFIXED(32));
  Actor_ConfigureSpriteColor(ACTOR_VIAL_CHEST_STATUE, 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 7);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 0);
  wait(40);
  AirsRockInside_WakeStatue(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN);
  Event_End();
}

/// The leader jumps down the ledge to the room below.
void AirsRockInside_JumpDownLedge(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
  Event_SetExitRequest(3);
  Event_End();
}

/// While the leader is no further than z 653, moves the leader to (376, 668)
/// with bit 0 of its byte 90 cleared for the move.
void AirsRockInside_StepOffRide(void) {
  u8 *flags;
  u8 bit = 1;

  if (Actor_GetById(gMainRoutineWork.leaderActorId)->z.part[0].integer > 653)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeedParameters(gMainRoutineWork.leaderActorId, toFIXED(4),
                           toFIXED(2));
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 376, 668);
  flags = &Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags;
  *flags = bit | *flags;
  Event_End();
}

/// A stepping stone the leader has crossed fades with a drip and is gone.
void AirsRockInside_VanishStone(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(SPRITE_STACKED_CRATES, INTtoFIXED(x), 0,
                              INTtoFIXED(y));
  REG_BLDALPHA = BLEND_ALPHA(16, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSpriteFlags(marker, 0);
  sprite = marker->sprite;
  sprite->blendMode = 2;
  Event_Start();
  Event_ResolvePendingActions(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Sound_Play(SE_DRIP);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  i++;
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = BLEND_ALPHA(16 - i, BLEND_WEIGHT_MAX);
  wait(1);
  i++;
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, cellX + 64, cellY, 1, 2);
  MapCell_SetAttributes(0, cellX, cellY, 0xff00);
  Actor_Release(marker);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

/// Reads what the leader stands on (MapCell_GetAttributes, kind 2): on cells 1
/// and 7 it only redraws the marker when flag 0x200 is set; elsewhere it clears
/// (AirsRockInside_StampCellUnderLeader), redraws the marker in the row the
/// event work's counter gives, sets flag 0x200 and the counter's flag (312 on).
void AirsRockInside_StepOnVanishingStone(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  cell = MapCell_GetAttributes(2, FIELD_CELL(leader->x.fixed),
                               FIELD_CELL(leader->z.fixed));
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    AirsRockInside_StampCellUnderLeader();
    AirsRockInside_VanishStone(264, row * 8 + 10);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
      GameFlag_Set(FLAG_SCENE_TEMP_00);
    GameFlag_Set(*(s16 *)work + 312);
  } else if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
    AirsRockInside_VanishStone(264, row * 8 + 10);
  }
}

/// As AirsRockInside_StampTallCellUnderLeader with a 1 by 2 block: saves the
/// cells 64 columns across from the controlled object at (73, 72) and stamps
/// the cells kept at (70, 74) in their place.
void AirsRockInside_StampWideCellUnderLeader(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(object->x.fixed);
  s32 y = FIELD_CELL(object->y.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  s32 key = (x << 16) + z;

  if (key != gAirsRockInside_StampedCell) {
    gAirsRockInside_StampedCell = key;
    gAirsRockInside_StampedWidth = 1;
    gAirsRockInside_StampedHeight = 2;
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
    MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                             1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        70, 74, gAirsRockInside_StampedWidth, gAirsRockInside_StampedHeight,
        gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
  }
}

extern u32 gAirsRockInside_OrbitSparkScript[];

#define DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(name, n1, n2)                        \
  void name(void) {                                                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 8, 19);                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 24, 19);                           \
    MapGrid_CopyCellsTo(n2, 17, 72, 19, 1, 2);                                 \
    MapGrid_CopyCellPictureIndicesRect(n2, 17, 88, 19, 1, 2);                  \
  }

DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(AirsRockInside_HideRevealedDoors, 26, 90)

DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(AirsRockInside_RevealDoors, 25, 89)

void AirsRockInside_SpawnOrbitSpark(struct Actor *source) {
  struct Actor *object = FieldActor_Create(SPRITE_PARTICLE, source->x.fixed,
                                           source->y.fixed, source->z.fixed);

  object->scaleY = FIXED_ONE * 2 / 5;
  object->scaleX = FIXED_ONE * 2 / 5;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  Actor_StartScript(object, gAirsRockInside_OrbitSparkScript);
}

s32 AirsRockInside_OrbitAnchor(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFX(orbiter->radius, orbiter->angle,
                               (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == toFIXED(2))
    return 1;
  AirsRockInside_SpawnOrbitSpark(orbiter);
  orbiter->radius -= toFIXED(4);
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  Vector_AddPolarOffset(orbiter->radius, orbiter->angle, &orbiter->x);
  AirsRockInside_SpawnOrbitSpark(orbiter);
  orbiter->radius -= toFIXED(4);
  return 1;
}

/// Moves around in a circle.
s32 Orbit_Update(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulFX(Trig_Cos(object->angle), object->radius);
  z = IwramMulFX(Trig_Sin(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Trig_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_bubbles.h"

extern u32 gAirsRockInside_TabletRiseScript[];
extern u32 gAirsRockInside_RisingSparkScript[];
extern s32 gAirsRockInside_OrbitingSparkScript[];
extern u32 gAirsRockInside_ShebaRiseScript[];
extern s32 gAirsRockInside_SeekingSparkScript[];
extern s32 gAirsRockInside_TabletSettleScript[];
extern s32 gAirsRockInside_ShebaSettleScript[];

/// The tablet rises in a rumble, sparks circle Sheba and she learns Reveal.
void AirsRockInside_ShebaLearnsReveal(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Camera_SetSpeedParameters(toFIXED(1) / 2, toFIXED(1) / 16);
  FieldCamera_MoveWithinMapBounds(INTtoFIXED(a->x.part[0].integer),
                                  a->y.part[0].integer << 16,
                                  a->z.part[0].integer << 16, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = toFIXED(1) / 5;
  a->speedLimit = toFIXED(1) / 5;
  Actor_SetMovementTypeById(idA, gAirsRockInside_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  FieldEvent_Wait(45);
  Actor_ConfigureSpriteColor(idA, 0x100);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != 16; i++) {
    struct Actor *spark =
        Import_ActorCreate(SPRITE_PARTICLE, a->x.fixed, a->y.fixed, a->z.fixed);
    Sound_Play(SE_GLITTER);
    spark->x.fixed += INTtoFIXED((Random16() & 31) - 16);
    spark->y.fixed += INTtoFIXED((Random16() & 15) - 8);
    spark->speedLimit = toFIXED(8);
    spark->acceleration = toFIXED(1) / 5;
    spark->motionFlags = 0;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gAirsRockInside_RisingSparkScript);
    REG_BLDALPHA = BLEND_ALPHA(4, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
  }
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  wait(30);
  for (i = 0; i != 16; i++) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed, b->y.fixed, b->z.fixed);
    Sound_Play(SE_SPARKS);
    angle = &spark->timer;
    *angle = (Random16() & 0x7fff) + 0x8000;
    spark->userValue = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->riseBaseY = 0x7a0000;
    spark->watchedActor = b;
    spark->riseCounter = (16 - i) * 8 + 15;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->riseBaseY, *angle,
                                 (s32)&spark->x.fixed);
    Actor_StartScript(spark, gAirsRockInside_OrbitingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = toFIXED(1) / 5;
      b->speedLimit = toFIXED(1) / 5;
      Actor_SetMovementTypeById(idB, gAirsRockInside_ShebaRiseScript);
    }
    wait(8);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != 32; i += 2) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed,
                          b->y.fixed + i * 65536 - toFIXED(8), b->z.fixed);
    spark->timer = Random16();
    spark->userValue = 0x800;
    spark->riseCounter = i;
    spark->riseEnabled = 1;
    spark->bounce = spark->x.fixed;
    spark->riseBaseY = 0;
    spark->gravity = spark->z.fixed;
    spark->watchedActor = b;
    spark->motionFlags = 0;
    spark->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteColor(spark, 2);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gAirsRockInside_SeekingSparkScript);
  }
  Sound_Play(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(idA, 0);
  Actor_ConfigureSpriteColor(idB, 0);
  Sound_Play(SE_FLASH);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(idA, gAirsRockInside_TabletSettleScript);
  Actor_SetMovementTypeById(idB, gAirsRockInside_ShebaSettleScript);
  b->update = Actor_EmitBubbles;
  wait(120);
  b->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

#include "text/msg_ids.h"

#define EVENT_WORK gFieldWork

s32 Actor_MoveToObject();

/// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
/// the scene with actor 6 follows, as Magma Rock's tablet does for Blaze.
void AirsRockInside_ReadRevealTablet(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Event_GetFlag(FLAG_REVEAL_TABLET_READ) != 0) {
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgCheckedTheTablet);
    EVENT_WORK->message++;
    Actor_SetSpeedFX(gMainRoutineWork.leaderActorId, toFIXED(7) / 10,
                     toFIXED(7) / 20);
    Actor_SetSpeedFX(ACTOR_SHEBA, toFIXED(7) / 10, toFIXED(7) / 20);
    Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 256, 136);
    Actor_WalkToById(ACTOR_SHEBA, 272, 136);
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    Actor_WaitForMoveById(ACTOR_SHEBA);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    // "Do you think I ought to touch it?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(ACTOR_SHEBA, 264, 137);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    MapGrid_SetShake(5, 7, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_WaitForShakeToSettle();
    AirsRockInside_ShebaLearnsReveal(8, 6);
    Party_LearnAbility(SHEBA, ABILITY_REVEAL);
    GameFlag_Set(FLAG_REVEAL_TABLET_READ);
    Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_SHEBA, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetSpeedFX(ACTOR_SHEBA, toFIXED(6) / 5, toFIXED(3) / 5);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
    object = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (object != 0)
      Actor_SetDestinationById(ACTOR_SHEBA, object->x.part[0].integer,
                               object->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_SHEBA);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_SHEBA, 0, 0);
    Actor_SetMovementType(Actor_GetById(ACTOR_REVEAL_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    Event_End();
  }
}

void AirsRockInside_WalkToRevealTablet(void) {
  if (GameFlag_Get(FLAG_REVEAL_TABLET_READ)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Event_End();
  } else {
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 248, 107);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 248, 136);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 264, 136);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    AirsRockInside_ReadRevealTablet();
  }
}

#define DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(name, n1)                          \
  void name(void) {                                                            \
    if (GameFlag_Get(FLAG_REVEAL_TABLET_READ)) {                               \
      Event_Start();                                                           \
      Event_ResolvePendingActions(0);                                          \
      FieldMessage_Show(MsgCheckedTheTablet, 1);                               \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, n1, 136);        \
      Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 264, 136);       \
      Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH,    \
                              0);                                              \
      FieldEvent_Wait(10);                                                     \
      AirsRockInside_ReadRevealTablet();                                       \
    }                                                                          \
  }

DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(AirsRockInside_ReadTabletFacingWest, 280)

DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(AirsRockInside_ReadTabletFacingEast, 248)

void AirsRockInside_IgnoreStep(void) {}

/// Move on the Elixir chest room's pillar: its place sets the area flag.
void AirsRockInside_MoveElixirChestPillar(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed >> 19 == 65)
    GameFlag_Set(FLAG_AREA_TEMP_03);
  else
    GameFlag_Clear(FLAG_AREA_TEMP_03);
  Event_End();
}

void AirsRockInside_PoundPairedStatuesCone(s32 mode, s32 step) {
  AirsRockInside_PoundCone(mode, step);
}

extern u16 gAirsRockInside_PairedStatuesCones[];
void AirsRockInside_SetConeCells(struct Actor *object);

void AirsRockInside_TradePairedStatues(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  pair = gAirsRockInside_PairedStatuesCones;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (FIELD_CELL(first->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(first->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = first->riseEnabled | second->riseEnabled;
  if (busy != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  firstSprite = first->sprite;
  secondSprite = second->sprite;
  second->motionFlags = busy;
  second->y.fixed -= toFIXED(28);
  wait(1);
  entry = &gVramCacheEntries[secondSprite->vramBlock];
  second->watchedActor = (void *)(entry->offset + 0x6010000);
  entry = &gVramCacheEntries[firstSprite->vramBlock];
  first->watchedActor = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedActor, entry->size);
  Dma_Set(first->watchedActor, second->watchedActor, DMA_COPY_WORDS(192),
          REG_DMA3);
  first->watchedActor = (void *)((u8 *)first->watchedActor + 0x300);
  second->watchedActor = (void *)((u8 *)second->watchedActor + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y.fixed += toFIXED(26) / 5;
    Dma_Set(first->watchedActor, second->watchedActor, DMA_COPY_WORDS(48),
            REG_DMA3);
    first->watchedActor = (void *)((u8 *)first->watchedActor + 0x100);
    second->watchedActor = (void *)((u8 *)second->watchedActor + 0x100);
  }
  leader->y.fixed += -(toFIXED(7) / 2);
  leader->groundHeight = leader->y.fixed;
  first->scaleY += -(FIXED_ONE * 31 / 256);
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  first->scaleY = FIXED_ONE;
  second->y.fixed = second->groundHeight;
  GameFlag_Set(first->timer);
  GameFlag_Clear(second->timer);
  Actor_SelectAnimation(first,
                        ((struct FieldSprite *)first->sprite)->frame + 1);
  Actor_SelectAnimation(second,
                        ((struct FieldSprite *)second->sprite)->frame - 1);
  AirsRockInside_SetConeCells(first);
  AirsRockInside_SetConeCells(second);
  FieldEvent_Wait(10);
  Event_End();
}

/// When the event work's warp number (0x154) is set, plays cue 186 and moves
/// the leader to that warp's place (warps 50 to 55), inside an event.
void AirsRockInside_RideFaceWarp(void) {
  s16 *warp;
  s32 x, z;

  x = 0;
  z = 0;
  warp = &gFieldWork->stepTrigger;
  if (*warp == 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeedParameters(gMainRoutineWork.leaderActorId, toFIXED(4),
                           toFIXED(2));
  switch (*warp) {
  case 50:
    // XXX: the leader is looked up twice and neither result is used; likely
    // left from a position read, because the other warps give theirs as
    // constants.
    Actor_GetById(gMainRoutineWork.leaderActorId);
    Actor_GetById(gMainRoutineWork.leaderActorId);
    x = 148;
    z = 632;
    break;
  case 51:
    x = 264;
    z = 652;
    break;
  case 52:
    x = 504;
    z = 508;
    break;
  case 53:
    x = 552;
    z = 508;
    break;
  case 54:
    x = 616;
    z = 188;
    break;
  case 55:
    x = 648;
    z = 188;
    break;
  }
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, x, z);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Event_End();
}

void AirsRockInside_CheckPsynergyStone(void) {
  const u8 *message;

  Event_Start();
  Event_ResolvePendingActions(0);
  message = MsgCheckedThePsynergy;
  FieldMessage_Show(message, 1);
  Sound_Play(SE_RECOVERY);
  Party_AddPp(999, 0);
  FieldEvent_Wait(10);
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  FieldMessage_Show(message + 1, 1);
  Event_End();
}

/// The placement table for the current room.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
    return gAirsRockInside_VialChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
    return gAirsRockInside_ClarityCircletChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
    return gAirsRockInside_TightropesTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return gAirsRockInside_RevealTabletTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
    return gAirsRockInside_ElixirChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES)
    return gAirsRockInside_PairedStatuesTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_WARP)
    return gAirsRockInside_FaceWarpTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
    return gAirsRockInside_CoinsChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE)
    return gAirsRockInside_PsynergyStoneTriggers;
  return gAirsRockInside_VialChestTriggers;
}

/// The leader rises 80 pixels on a lift into the room.
void AirsRockInside_RiseOnLift(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Event_Start();
  Event_ResolvePendingActions(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(leader, 0);
  leader->speedLimit = toFIXED(3) / 2;
  leader->acceleration = toFIXED(3) / 2;
  lift = Import_ActorCreate(SPRITE_WHIRLWIND, leader->x.fixed,
                            leader->y.fixed - toFIXED(8), leader->z.fixed);
  Actor_SelectAnimation(lift, 2);
  Actor_SetSpriteFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = toFIXED(7) / 5;
  lift->acceleration = toFIXED(7) / 5;
  Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed + toFIXED(80),
                      leader->z.fixed);
  Actor_SetMoveTarget(lift, lift->x.fixed, lift->y.fixed + toFIXED(72),
                      lift->z.fixed);
  Sound_Play(SE_KIRIN);
  Event_StartScreenTransition();
  while (!Actor_HasArrived(leader)) {
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  FieldEvent_Wait(10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  height = Map_GetTerrainHeight(0, lift->x.fixed, lift->z.fixed - toFIXED(16));
  leader->y.fixed = height;
  leader->groundHeight = height;
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  wait(6);
  Sound_Play(SE_JUMP);
  Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
  leader->speedLimit = toFIXED(2);
  leader->acceleration = toFIXED(1) / 2;
  leader->velocityY = toFIXED(4);
  leader->motionFlags = ACTOR_FALLS;
  leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
                      leader->z.fixed - toFIXED(16));
  OverlayObject_WaitUntilIdle(leader);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_SelectAnimation(lift, 5);
  wait(12);
  Actor_Release(lift);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->groundHeight = leader->y.fixed;
  Actor_SetSpriteFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Actor_Release(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Event_End();
}

/// Eight motes of light drift about the hall.
void AirsRockInside_SpawnMotes(void) {
  struct Actor *object;
  s32 i = 0;
  struct FieldSprite *sprite;

  do {
    object = Import_ActorCreate(SPRITE_PARTICLE, 0, 0, 0);
    sprite = object->sprite;
    sprite->priority = 1;
    object->riseEnabled = i << 4;
    {
      s32 zero = 0;
      object->motionFlags = zero;
      object->priorityFlags = zero;
    }
    object->acceleration = object->speedLimit = (i << 12) + (toFIXED(1) / 2);
    object->update = AirsRockInside_DriftMote;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    if (i & 1) {
      object->timer = 212;
      object->userValue = 0;
    } else {
      object->timer = 116;
      object->userValue = 0;
    }
    Actor_SetBlendMode(object, 1);
    i++;
  } while (i != 8);
}

extern s32 gAirsRockInside_LeaderLevel;

/// Sets the hall's ledge cells for the height the leader is at.
void AirsRockInside_SetLedgeCellsByLevel(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (actor->y.fixed + toFIXED(32) >= toFIXED(312))
    level = 2;
  else if (actor->y.fixed + toFIXED(32) >= toFIXED(216))
    level = 1;
  else
    level = 0;
  if (gAirsRockInside_LeaderLevel == level)
    return;
  switch (level) {
  case 2: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 64, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 68, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 72, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  }
  gAirsRockInside_LeaderLevel = level;
}
#undef EVENT_WORK

#include "../field/field_effect_advance_motion.c"

#undef EVENT_WORK
#undef DEFINE_AIRS_ROCK_INSIDE_SET_DOORS
#undef DEFINE_AIRS_ROCK_INSIDE_READ_TABLET
#undef DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef other
#undef TARGET
#undef PICK_UP
#undef SHAKE

void AirsRockInside_LandInHall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveToFX(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -(toFIXED(3) / 8);
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -(toFIXED(3) / 16);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 22);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

void AirsRockInside_PuffVents(void);

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 i, z, one;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  GameFlag_Set(FLAG_AREA_TEMP_00);
  GameFlag_Set(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
    AirsRockInside_StartRestoringCells();
    // The Japanese release arrives here whatever flag 265 says.
    if (gMainRoutineWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET &&
        (!TLA_PATCH_1 || !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))) {
      Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 1;
      wait(8);
      MapGrid_CenterOnFollowedPoint();
      wait(1);
    }
    if (gMainRoutineWork.entrance <= 11 ||
        gMainRoutineWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET) {
      Task_Add(AirsRockInside_SetLedgeCellsByLevel, TASK_PASS_VBLANK);
      REG_BLDALPHA = BLEND_ALPHA(8, 16);
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      AirsRockInside_SpawnMotes();
    }
    if (gMainRoutineWork.entrance > 13) {
      if (GameFlag_Get(FLAG_AIRS_ROCK_HALL_PILLAR_PLACED))
        Actor_SetPositionFX(ACTOR_HALL_POOL_STATUE, toFIXED(824), toFIXED(328));
      Actor_PlaceBlocks(gAirsRockInside_HallPillars);
    }
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->motionFlags = 0;
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->y.fixed = 0;
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_HALL_TWISTER_STATUE, 3);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 6);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_ROCK), 6);
      MapGrid_CopyCellsTo(64, 78, 18, 105, 3, 5);
    }
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_WEST_STATUE), 6);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_EAST_STATUE), 6);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_POOL_STATUE), 6);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_WEST_STATUE), 180, 0);
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_EAST_STATUE), 60, 0);
      AirsRockInside_WakePooledStatue(Actor_GetById(ACTOR_HALL_POOL_STATUE), 60,
                                      0);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_WEST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_EAST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_POOL_STATUE), 0);
    if (gMainRoutineWork.entrance == ENTRANCE_HALL_LANDING &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      AirsRockInside_LandInHall();
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    AirsRockInside_StartRestoringCells();
    if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_VIAL_CHEST_EAST_PILLAR, toFIXED(328),
                          toFIXED(392));
    if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_VIAL_CHEST_WEST_PILLAR, toFIXED(104),
                          toFIXED(392));
    Actor_PlaceBlocks(gAirsRockInside_VialChestPillars);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 6);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 60, 0);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK), 6);
      MapGrid_CopyCellsTo(68, 84, 12, 77, 3, 6);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 0);
    if (gMainRoutineWork.entrance == ENTRANCE_VIAL_CHEST_ON_THE_LIFT &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      AirsRockInside_RiseOnLift();
  } else if (gMainRoutineWork.next ==
             SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    AirsRockInside_StartRestoringCells();
    Scene_QueueMusic(SE_AMBIENT_STREAM);
    REG_BLDALPHA = BLEND_ALPHA(8, 12);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    Task_Add(AirsRockInside_PuffVents, TASK_PASS_VBLANK);
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
                          toFIXED(264), toFIXED(632));
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
                          toFIXED(616), toFIXED(568));
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
                          toFIXED(472), toFIXED(520));
    Actor_PlaceBlocks(gAirsRockInside_ClarityCircletChestPillars);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
                           6);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
                           6);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
                           6);
    } else {
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A), 60, 0);
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B), 180, 0);
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C), 200, 0);
    }
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A)->steeringFlags = 0;
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B)->steeringFlags = 0;
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C)->steeringFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
                         0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
                         0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
                         0);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPES_POST_A, ACTOR_TIGHTROPES_POST_B);
    Tightrope_Place(1, ACTOR_TIGHTROPES_POST_C, ACTOR_TIGHTROPES_POST_D);
    Tightrope_Place(2, ACTOR_TIGHTROPES_POST_E, ACTOR_TIGHTROPES_POST_F);
    Tightrope_Place(3, ACTOR_TIGHTROPES_POST_G, ACTOR_TIGHTROPES_POST_H);
    Tightrope_Place(4, ACTOR_TIGHTROPES_POST_I, ACTOR_TIGHTROPES_POST_J);
    Actor_GetById(ACTOR_TIGHTROPES_POST_H)->riseCounter = 1;
    if (GameFlag_Get(FLAG_AREA_TEMP_01)) {
      MapGrid_CopyCellsTo(69, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(46, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(32, 89, 1, 1, 31, 89);
      MapGrid_CopyCellAttributes(50, 86, 1, 1, 33, 87);
    } else {
      MapGrid_CopyCellsTo(64, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(52, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(33, 88, 1, 1, 33, 87);
      MapGrid_CopyCellAttributes(48, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_00)) {
      MapGrid_CopyCellsTo(90, 25, 86, 25, 4, 3);
      MapGrid_CopyCellsTo(21, 88, 24, 89, 1, 2);
      MapGrid_CopyCellAttributes(23, 92, 2, 1, 23, 89);
      MapGrid_CopyCellAttributes(18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_TIGHTROPES_PILLAR, toFIXED(648), toFIXED(296));
    Actor_PlaceBlocks(gAirsRockInside_TightropesPillars);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    AirsRockInside_StartRestoringCells();
    i = 0;
    do {
      if (GameFlag_Get(514 + i)) {
        z = 24 - (i << 1);
        MapGrid_CopyCellsTo(65, 1, 80, z, 1, 2);
        FieldMapCell_SetAttributes(0, 16, z, 0xff00);
      }
      i++;
    } while (i != 6);
    Actor_GetById(ACTOR_REVEAL_TABLET)->motionFlags = 0;
    object = Actor_GetById(ACTOR_REVEAL_TABLET);
    Actor_GetById(ACTOR_REVEAL_TABLET)->groundHeight = toFIXED(16);
    object->y.fixed = toFIXED(16);
    Actor_SetMovementType(Actor_GetById(ACTOR_REVEAL_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    AirsRockInside_StartRestoringCells();
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_ELIXIR_CHEST_POST_A, ACTOR_ELIXIR_CHEST_POST_B);
    Tightrope_Place(1, ACTOR_ELIXIR_CHEST_POST_C, ACTOR_ELIXIR_CHEST_POST_D);
    Tightrope_Place(2, ACTOR_ELIXIR_CHEST_POST_E, ACTOR_ELIXIR_CHEST_POST_F);
    if (!GameFlag_Get(FLAG_AREA_TEMP_02)) {
      MapGrid_CopyCellsTo(69, 64, 32, 79, 5, 7);
      MapGrid_CopyCellAttributes(49, 15, 5, 5, 32, 15);
      MapGrid_CopyCellAttributes(33, 82, 1, 1, 34, 82);
      MapGrid_CopyCellAttributes(52, 81, 1, 1, 36, 81);
    } else {
      MapGrid_CopyCellsTo(64, 64, 32, 79, 5, 7);
      MapGrid_CopyCellAttributes(49, 21, 5, 5, 32, 15);
      MapGrid_CopyCellAttributes(35, 81, 1, 1, 36, 81);
      MapGrid_CopyCellAttributes(50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_03))
      Actor_SetPositionFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(520),
                          toFIXED(296));
    if (GameFlag_Get(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED)) {
      Actor_SetPositionFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(312),
                          toFIXED(296));
      Actor_SetSpritePriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
      Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->motionFlags = ACTOR_FALLS;
      Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->groundHeight = -toFIXED(32);
      MapGrid_CopyCellAttributes(19, 16, 1, 1, 19, 18);
    }
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STATUES) {
    AirsRockInside_StartRestoringCells();
    one = 1;
    Actor_GetById(ACTOR_PAIRED_STATUES_CONE_A)->riseCounter = one;
    Actor_GetById(ACTOR_PAIRED_STATUES_CONE_B)->riseCounter = one;
    Actor_GetById(ACTOR_PAIRED_STATUES_CONE_C)->riseCounter = one;
    Actor_GetById(ACTOR_PAIRED_STATUES_CONE_D)->riseCounter = one;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Set(FLAG_SCENE_TEMP_02);
    }
    AirsRockInside_PlaceCones(gAirsRockInside_PairedStatuesCones);
    if (GameFlag_Get(FLAG_AIRS_ROCK_PAIRED_STATUES_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_PAIRED_STATUES_PILLAR, toFIXED(344),
                          toFIXED(696));
    Actor_PlaceBlocks(gAirsRockInside_PairedStatuesPillars);
    if (GameFlag_Get(FLAG_AREA_TEMP_04)) {
      MapGrid_CopyCellsTo(39, 82, 38, 82, one, 3);
      MapGrid_CopyCellAttributes(39, 82, 1, 1, 38, 82);
      MapGrid_CopyCellAttributes(38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_05)) {
      MapGrid_CopyCellsTo(39, 82, 38, 99, one, 3);
      MapGrid_CopyCellAttributes(38, 98, 1, 1, 38, 99);
      MapGrid_CopyCellAttributes(38, 35, 1, 1, 38, 37);
    }
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_WARP) {
    Scene_QueueMusic(SE_AMBIENT_STREAM);
    REG_BLDALPHA = BLEND_ALPHA(8, 12);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    Task_Add(AirsRockInside_PuffVents, TASK_PASS_VBLANK);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_WARP_FIRST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_FACE_WARP_FIRST_PILLAR, toFIXED(280),
                          toFIXED(472));
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_WARP_SECOND_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_FACE_WARP_SECOND_PILLAR, toFIXED(360),
                          toFIXED(840));
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_WARP_THIRD_PILLAR_PLACED)) {
      Actor_SetPositionFX(ACTOR_FACE_WARP_THIRD_PILLAR, toFIXED(184),
                          toFIXED(616));
      GameFlag_Set(FLAG_SCENE_TEMP_00);
    }
    Actor_SetSpritePriority(ACTOR_FACE_WARP_SECOND_PILLAR, 2);
    Actor_PlaceBlocks(gAirsRockInside_FaceWarpPillars);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_FIRST_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_FIRST_PILLAR)->z.fixed), 0);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_SECOND_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_WARP_SECOND_PILLAR)->z.fixed), 0);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_FACE_WARP_STATUE), 6);
    else
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_FACE_WARP_STATUE), 8, 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FACE_WARP_STATUE), 0);
  } else if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    AirsRockInside_StartRestoringCells();
    if (GameFlag_Get(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_COINS_CHEST_PILLAR, toFIXED(360), toFIXED(264));
    Actor_PlaceBlocks(gAirsRockInside_CoinsChestPillars);
    MapCell_SetAttributes(
        2, FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
    Actor_GetById(ACTOR_COINS_CHEST_STATUE)->steeringFlags = 0;
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_COINS_CHEST_STATUE), 6);
    else
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_COINS_CHEST_STATUE), 60, 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_COINS_CHEST_STATUE), 0);
    if (GameFlag_Get(FLAG_AREA_TEMP_06)) {
      MapGrid_CopyCellsTo(15, 80, 15, 77, 2, 3);
      MapGrid_CopyCellAttributes(15, 80, 1, 1, 15, 79);
      MapGrid_CopyCellAttributes(17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    AirsRockInside_SetLedgeCellsByLevel();
  return 0;
}

void AirsRockInside_StartWhirlwind(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gAirsRockInside_HallPillars[] = {
    12, -1,
};

s16 gAirsRockInside_VialChestPillars[] = {
    9, 11, -1,
};

s16 gAirsRockInside_ClarityCircletChestPillars[] = {
    8, 11, 12, 10, -1,
};

s16 gAirsRockInside_TightropesPillars[] = {
    9, -1,
};

s16 gAirsRockInside_PairedStatuesPillars[] = {
    8, 9, 10, 11, -1,
};

s16 gAirsRockInside_FaceWarpPillars[] = {
    12, 8, 9, 13, 14, -1,
};

s16 gAirsRockInside_CoinsChestPillars[] = {
    9, 10, -1,
};

u32 gAirsRockInside_OrbitSparkScript[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_RisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 gAirsRockInside_OrbitingSparkScript[] = {                                                               
      46,                                                                      
      (s32)AirsRockInside_OrbitAnchor,                                                              
      38,                                                                      
      17,                                                                      
  };

s32 gAirsRockInside_SeekingSparkScript[] = {                                                               
      46, (s32)Orbit_Update, 46, (s32)Actor_AimAtWatched, 46, (s32)Actor_StepToWatched, 38, 17,                  
  };

u32 gAirsRockInside_TabletRiseScript[] = {
    SC_MOVE_BY(0, 24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

u32 gAirsRockInside_ShebaRiseScript[] = {
    SC_MOVE_BY(0, 16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

s32 gAirsRockInside_TabletSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -24, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 0),
    SC_END,
};

s32 gAirsRockInside_ShebaSettleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, -16, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gAirsRockInside_WhirlwindRisesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_WhirlwindFadesScript[] = {
    SC_WAIT(60),
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_PooledWhirlwindFadesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gAirsRockInside_StatueScript[] = {
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_END,
};

u16 gAirsRockInside_PairedStatuesCones[] = {
    12, 1, 512, 13, 1, 513, 14, 1,
    514, 15, 1, 515, -1, 0,
};

struct ActorPlacement gAirsRockInside_Entrances[] = {
    {0, CONDITION_ALWAYS, 320, 0x40000140, toFIXED(-1), -0x1, 0xffff, 0, 0},
    {SCENE_TABLE_END},
};

s16 gAirsRockInside_HallEntrances[] = {
    1, -1, 232, 0, 120, 0x4000, 0, 64,
    16, 560, 1008, 0, 2, -1, 152, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    3, -1, 152, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 4, -1, 152, 320,
    648, 0x4000, 0, 64, 16, 560, 1008, 0,
    5, -1, 152, 128, 648, 0x4000, 0, 64,
    16, 560, 1008, 0, 6, -1, 232, 0,
    632, 0x4000, 0, 64, 16, 560, 1008, 0,
    7, -1, 312, 0, 616, 0x4000, 0, 64,
    16, 560, 1008, 0, 8, -1, 472, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    9, -1, 472, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 10, -1, 408, 0,
    952, 0xc000, 0, 64, 16, 560, 1008, 0,
    11, -1, 312, 0, 840, 0x4000, 0, 64,
    16, 560, 1008, 0, 12, -1, 808, 0,
    136, 0xc000, 0, 608, 16, 864, 224, 0,
    13, -1, 680, 0, 168, 0x4000, 0, 608,
    16, 864, 224, 0, 14, -1, 872, 0,
    344, 0x4000, 0, 592, 256, 976, 1008, 0,
    15, -1, 904, 0, 904, 0x4000, 0, 592,
    256, 976, 1008, 0, 16, -1, 312, 0,
    808, 0x4000, 0, 64, 16, 560, 1008, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRockInsideHall),
    SCENE_EXIT_UNLESS(1, AirsRockInsideVialChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideClarityCircletChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideTightropes, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInsideElixirChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockInsideHall, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockInsidePsynergyStone, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockInsidePsynergyStone, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockInsidePairedStatues, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockInsideFaceWarp, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRockEntrance, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRockInsideHall, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRockInsideHall, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRockInsideHall, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AirsRockInsideCoinsChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AirsRockInsideRevealTablet, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AirsRockInsideVialChest, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideVialChest),
    SCENE_EXIT_UNLESS(1, AirsRockFujinShieldChest, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideHall, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideClarityCircletChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideTightropes, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideTightropes),
    SCENE_EXIT_UNLESS(1, AirsRockInsideClarityCircletChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideElixirChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideCoinsChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInsideHall, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideRevealTablet),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 16, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideElixirChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideTightropes, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsidePairedStatues),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideFaceWarp, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideFaceWarp),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsidePairedStatues, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideCoinsChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideTightropes, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 15, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsidePsynergyStone),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_HallPlacements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, 7, 312, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {377, CONDITION_ALWAYS, 7, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_VialChestPlacements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {382, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_ClarityCircletChestPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_TightropesPlacements[] = {
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_RevealTabletPlacements[] = {
    {323, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_ElixirChestPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_PairedStatuesPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x9, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0xa, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_FaceWarpPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gAirsRockInside_StatueScript, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

s32 gAirsRockInside_CoinsChestPlacements[] = {                                                               
      (s32)0xffff0145, (s32)gAirsRockInside_StatueScript, 1, 0, 0, 0x2024000, (s32)0xffff0110, 1, 2, 0, 0, 0x2024000,    
      (s32)0xffff0179, 1,         3, 0, 0, 0x2020000, 0xffff,          0,  0,  0, 0, 0,     
  };

s32 gAirsRockInside_StampedCell = -1;

struct Trigger gAirsRockInside_HallTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOntoLedge)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(AirsRockInside_StampTallCellUnderLeader)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(AirsRockInside_StampTallCellUnderLeaderNearer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_WakeHallStatue)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_POOL_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HALL_POOL_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(AirsRockInside_JumpDownLedge)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_WakeVialChestStatue)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_ClarityCircletChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffRide)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_TightropesTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_RevealTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(AirsRockInside_StampWideCellUnderLeader)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(AirsRockInside_IgnoreStep)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_WalkToRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadTabletFacingEast)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AirsRockInside_RevealDoors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AirsRockInside_HideRevealedDoors)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOnVanishingStone)},
    {TRIGGER_RAISED, 203, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOnVanishingStone)},
    {TRIGGER_RAISED, 204, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOnVanishingStone)},
    {TRIGGER_RAISED, 205, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOnVanishingStone)},
    {TRIGGER_RAISED, 206, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOnVanishingStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_ElixirChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELIXIR_CHEST_PILLAR, FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED, EVENT(AirsRockInside_MoveElixirChestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_PairedStatuesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, 212, CONDITION_ALWAYS, EVENT(AirsRockInside_TradePairedStatues)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STATUES_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STATUES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STATUES_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STATUES_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STATUES_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_PILLAR_C, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STATUES_PILLAR_C, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STATUES_CONE_A, FLAG_SCENE_TEMP_00, EVENT(AirsRockInside_PoundPairedStatuesCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STATUES_CONE_B, FLAG_SCENE_TEMP_01, EVENT(AirsRockInside_PoundPairedStatuesCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STATUES_CONE_C, FLAG_SCENE_TEMP_02, EVENT(AirsRockInside_PoundPairedStatuesCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STATUES_CONE_D, FLAG_SCENE_TEMP_03, EVENT(AirsRockInside_PoundPairedStatuesCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STATUES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STATUES_NORTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STATUES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STATUES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STATUES_SOUTH_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_FaceWarpTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(AirsRockInside_RideFaceWarp)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_WARP_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_WARP_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_WARP_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_WARP_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_WARP_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_WARP_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_FACE_WARP_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_WARP_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_WARP_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_CoinsChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_COINS_CHEST_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_COINS_CHEST_GUST_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(AirsRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

s32 gAirsRockInside_LeaderLevel = -1;
#undef Actor_SetHorizontalPositionWithTerrain
