/// @file
/// The Kibombo Mountains, the pass the Kibombo warriors guard on the way to
/// their village, in three rooms: the chest room with the Disk Axe, the ledges
/// above it where Waft hides, and the small cavern behind a bush.
///
/// As the party nears the pass, warriors report to one another that the way is
/// clear (KibomboMountains_GuardsReport), and from then on any guard who sees
/// Felix shouts for the others and the party is marched back down the road
/// (KibomboMountains_GuardSpotsParty). In the chest room the party climbs the
/// vines past the second guard, hops across the two crates below him once they
/// are pushed into place, drops a pillar into the gap in the west as a
/// shortcut, and pushes the crate along the first guard's ledge to hide
/// behind it; the rope over the third guard is tied with Lash. On the ledges
/// above, a crate pushed off a ledge knocks out the guard below, a pillar
/// dropped into the dog's run shuts the dog in, and Tremor shakes a bone off a
/// crate to keep the dog busy; Whirlwind blows the bush from the cavern's way
/// in. Later, with Frost and Growth, an ice pillar and a vine lead up to Waft
/// (KibomboMountains_FightWaft), and the warriors let the party pass.

#include "flags.h"
#include "sprite_ids.h"
#include "djinn_ids.h"
#include "item_ids.h"
#include "field/field.h"
#include "types.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/field_event.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "field/call.h"
#include "system/ram_buffer.h"
#include "version.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/// Once the message of the Gabomba is carried the Kibombo let the party pass:
/// the guards and the dog are gone.
#define FLAG_KIBOMBO_LETS_PARTY_PASS FLAG_GABOMBA_MESSAGE_CARRIED
#define FLAG_KIBOMBO_GUARDS_REPORTED 0x880
#define FLAG_KIBOMBO_BUSH_BLOWN_AWAY 0x882
/// Set once the party has lashed the rope between the posts.
#define FLAG_KIBOMBO_ROPE_TIED 0x9e8
#define FLAG_KIBOMBO_FIRST_PILLAR_DROPPED 0x8e7
#define FLAG_KIBOMBO_SECOND_PILLAR_DROPPED 0x8e8
/// For this visit: the lower guard knocked out by the crate, the dog shut in
/// south or north of the dropped crate or eating the bone, and the party turned
/// back by the guards.
#define FLAG_GUARD_KNOCKED_OUT FLAG_SCENE_TEMP_00
#define FLAG_DOG_SHUT_SOUTH FLAG_SCENE_TEMP_01
#define FLAG_DOG_EATING_BONE FLAG_SCENE_TEMP_02
#define FLAG_DOG_SHUT_NORTH FLAG_SCENE_TEMP_03
#define FLAG_TURNED_BACK FLAG_AREA_TEMP_00
/// For this visit: the first and second puddles of the Disk Axe room frozen,
/// the Waft room's puddle frozen and its sprout grown, and the dog's pillar and
/// the guard's crate dropped.
#define FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_10
#define FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_11
#define FLAG_WAFT_ROOM_PUDDLE_FROZEN FLAG_SCENE_TEMP_21
#define FLAG_SPROUT_GROWN FLAG_SCENE_TEMP_22
#define FLAG_DOG_PILLAR_DROPPED FLAG_SCENE_TEMP_23
#define FLAG_GUARD_CRATE_DROPPED FLAG_SCENE_TEMP_24
#include "shared/tables.h"

/// The window the rooms open with: a shape in Japanese, an interlaced wipe
/// from patch 1 on.
#if !TLA_PATCH_1
#define ROOM_TRANSITION                                                        \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define ROOM_TRANSITION                                                        \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

/// The room where Waft hides above the guards, by the rows of
/// gKibomboMountains_WaftPlacements.
enum KibomboMountainsWaftActorId {
  /// The warrior on watch above the dog, who also shouts when it barks.
  ACTOR_UPPER_GUARD = ACTOR_FIRST_PLACED,
  /// The warrior on watch below, whom the crate can knock out.
  ACTOR_LOWER_GUARD,
  ACTOR_GUARD_DOG,
  /// The puddle Frost turns into a pillar.
  ACTOR_PUDDLE,
  /// The sprout Growth turns into a vine up to Waft's ledge.
  ACTOR_SPROUT,
  /// The bush Whirlwind blows away from the small cavern's way in.
  ACTOR_CAVERN_BUSH,
  /// The pillar pushed off the ledge to shut the dog in.
  ACTOR_DOG_PILLAR,
  /// The crate pushed off the ledge onto the lower guard.
  ACTOR_GUARD_CRATE,
  /// The crate the bone lies on, which Tremor shakes.
  ACTOR_BONE_CRATE,
  ACTOR_BONE,
  ACTOR_WAFT,
  /// Unused: an unseen marker under the trees south of the dog's pillar. It
  /// draws nothing, and no trigger or event names it; the room only takes away
  /// its shadow and collision as it opens.
  ACTOR_UNSEEN_MARKER
};

/// The room of the Disk Axe chest, by the rows of
/// gKibomboMountains_DiskAxeChestPlacements.
enum KibomboMountainsDiskAxeChestActorId {
  ACTOR_WEST_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_EAST_ROPE_POST,
  ACTOR_FIRST_GUARD,
  ACTOR_SECOND_GUARD,
  ACTOR_THIRD_GUARD,
  /// The pillar dropped to the chest's ledge.
  ACTOR_SECOND_DROPPED_PILLAR,
  /// The two crates the party hides behind from the first guard.
  ACTOR_WEST_CRATE,
  ACTOR_EAST_CRATE,
  /// The two crates further south.
  ACTOR_LOWER_WEST_CRATE,
  ACTOR_LOWER_EAST_CRATE,
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  /// The pillar dropped to the lower path.
  ACTOR_FIRST_DROPPED_PILLAR,
  ACTOR_WEST_TORCH,
  ACTOR_EAST_TORCH,
  /// The warriors who turn the party back; ACTOR_WARRIOR_E also comes to
  /// hear the second guard's report.
  ACTOR_WARRIOR_A,
  ACTOR_WARRIOR_B,
  ACTOR_WARRIOR_C,
  ACTOR_WARRIOR_D,
  ACTOR_WARRIOR_E,
  ACTOR_WARRIOR_F
};

/// The chest room's entrances the guards send the party back to: the first
/// time it is caught on a visit the warriors march it down the road, after
/// that it is only put back.
enum KibomboMountainsDiskAxeChestEntrance {
  ENTRANCE_CAUGHT = 8,
  ENTRANCE_CAUGHT_AGAIN
};

/// The cell where the dog's pillar lands off its ledge, beside the dog's run.
#define DOG_PILLAR_CELL_X 47
#define DOG_PILLAR_CELL_Z 28
/// The cell where the guard's crate lands on the lower guard's path.
#define GUARD_CRATE_CELL_X 53
#define GUARD_CRATE_CELL_Z 36

/// The rooms' own cell behaviours, named by what stands near them.
enum KibomboMountainsCellBehavior {
  /// Waft's ledges: south west of the upper guard, cells (35, 21) to
  /// (35, 23), and south east of him, (41, 23) to (41, 26); by the north end
  /// of the dog's run, (49, 22) to (49, 24), (45, 24) and (45, 25); south of
  /// the lower guard's path, (51, 36) to (51, 38) and (47, 38) to (47, 40),
  /// and south east of it, (54, 38) to (55, 41); and by the south end of the
  /// dog's run, (50, 30) to (50, 32).
  CELL_BEHAVIOR_WEST_OF_UPPER_GUARD = 20,
  CELL_BEHAVIOR_EAST_OF_UPPER_GUARD,
  CELL_BEHAVIOR_NORTH_END_OF_DOG_RUN,
  CELL_BEHAVIOR_SOUTH_OF_LOWER_GUARD,
  CELL_BEHAVIOR_SOUTH_EAST_OF_LOWER_GUARD,
  CELL_BEHAVIOR_SOUTH_END_OF_DOG_RUN,
  /// Waft's ledges: the edge Felix jumps west from, cell (43, 30).
  CELL_BEHAVIOR_JUMP_WEST = 33,
  /// The chest room: the first guard's ledge west of where he paces, cells
  /// (8, 18) to (10, 19), and east of it, (16, 18) to (19, 18); east of the
  /// second guard, (18, 28) to (18, 30), and south of him, (14, 30) to
  /// (17, 32); between the torches below him, (15, 33) to (16, 34); and the
  /// rope's line above the third guard, (26, 12) and (27, 12), and the cells
  /// west of him, (25, 15), (24, 16) and (25, 16).
  CELL_BEHAVIOR_WEST_OF_FIRST_GUARD = 30,
  CELL_BEHAVIOR_EAST_OF_FIRST_GUARD,
  CELL_BEHAVIOR_EAST_OF_SECOND_GUARD,
  CELL_BEHAVIOR_SOUTH_OF_SECOND_GUARD,
  CELL_BEHAVIOR_BETWEEN_TORCHES,
  CELL_BEHAVIOR_ROPE_ABOVE_THIRD_GUARD,
  CELL_BEHAVIOR_WEST_OF_THIRD_GUARD
};

/// A trigger's subject that answers only while the leader faces `facing`.
#define FACING_SUBJECT(facing, target)                                         \
  ((s16)((facing) | TRIGGER_SUBJECT_NEEDS_FACING | (target)))

extern s32 gKibomboMountains_TorchScript[];
extern s32 gKibomboMountains_CrateScript[];
extern s32 gKibomboMountains_WatchScript[];
extern s32 gKibomboMountains_PaceScript[];
extern s32 gKibomboMountains_DogRunScript[];
extern s32 gKibomboMountains_SecondGuardPaceScript[];

void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);

#include "shared/field_rope_place.c"

void Field_TryJumpForward(void);

/// Felix talks to Waft, the Jupiter Djinni on the high ledge reached by
/// the ice pillar and the vine: Waft fights.
void KibomboMountains_FightWaft(void) {
  Field_RequestDjinnBattle(ACTOR_WAFT, DJINNI_WAFT);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "shared/actor_bob.h"

extern struct ActorPlacement gKibomboMountains_Placements[];
extern struct ActorPlacement gKibomboMountains_WaftPlacements[];
extern struct ActorPlacement gKibomboMountains_DiskAxeChestPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
    return gKibomboMountains_WaftPlacements;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
    return gKibomboMountains_DiskAxeChestPlacements;
  return gKibomboMountains_Placements;
}

/// Piers casts Frost on the puddle on the third level of Waft's ledges: it
/// freezes into an ice pillar, and the cell beside it, (22, 32), becomes one
/// Felix can hop across on its top.
void KibomboMountains_FreezePuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
  Event_SetFlag(FLAG_WAFT_ROOM_PUDDLE_FROZEN);
}

/// Growth on the sprout near Waft's ledge: it grows into a vine, and cells
/// (28, 33) and (28, 34) become climbable, up to Waft.
void KibomboMountains_GrowVine(void) {
  MapGrid_CopyCellAttributes(30, 35, 1, 1, 28, 34);
  MapGrid_CopyCellAttributes(30, 34, 1, 1, 28, 33);
  Event_SetFlag(FLAG_SPROUT_GROWN);
}

#include "shared/actor_wait_to_land.h"

/// Nothing visible happens as Felix starts pushing the crate on the ledge above
/// the lower guard's path, cell (55, 34). The crate's sprite is set to be drawn
/// in front of the guard, so that it covers him if it lands on him.
void KibomboMountains_StartPushingGuardCrate(s32 unused, s32 id) {
  if (id == ACTOR_GUARD_CRATE)
    FieldActor_SetSpritePriority(ACTOR_GUARD_CRATE, 1);
}

extern s32 gKibomboMountains_CrateJumpScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);
void Actor_StartIdle(struct Actor *actor);

/// Felix pushes the pillar or the crate off its ledge on Waft's ledges. The
/// pillar lands in the middle of the dog's run, cell (47, 28), shutting the
/// dog in north or south of it (a dog caught under it cries out and is
/// pushed aside), and Felix can hop across its top. The crate lands on the
/// lower guard's path, cell (53, 36): if the guard is under it, he walks into
/// it and is trapped, the crate rocking as he struggles; if not, the guard
/// jumps, looks at the crate puzzled, shakes his head and walks on, and the
/// crate can be pushed again.
void KibomboMountains_DropOffLedge(s32 unused, s32 id) {
  struct Actor saved;

  if (id == ACTOR_DOG_PILLAR) {
    struct Actor *pillar = Actor_GetById(ACTOR_DOG_PILLAR);
    struct Actor *dog;
    s32 x;
    s32 z;
    s32 cell;

    if (!Event_GetFlag(FLAG_DOG_PILLAR_DROPPED) &&
        (x = FIELD_CELL(pillar->x.fixed)) == DOG_PILLAR_CELL_X &&
        (z = FIELD_CELL(pillar->z.fixed)) == DOG_PILLAR_CELL_Z) {
      Actor_WaitToLand(ACTOR_DOG_PILLAR);
      MapGrid_CopyCellAttributes(49, 28, 1, 1, x, z);
      Event_SetFlag(FLAG_DOG_PILLAR_DROPPED);
      Actor_SetSpritePriority(ACTOR_DOG_PILLAR, 2);
      dog = Actor_GetById(ACTOR_GUARD_DOG);
      cell = FIELD_CELL(dog->z.fixed);
      if (cell == DOG_PILLAR_CELL_Z - 1) {
        Sound_Play(SE_VOICE_ANIMAL_B);
        dog->z.fixed = toFIXED(440);
      } else if (cell == DOG_PILLAR_CELL_Z || cell == DOG_PILLAR_CELL_Z + 1) {
        Sound_Play(SE_VOICE_ANIMAL_C);
        dog->z.fixed = toFIXED(472);
      }
      if (pillar->z.fixed < dog->z.fixed)
        Event_SetFlag(FLAG_DOG_SHUT_SOUTH);
      else
        Event_SetFlag(FLAG_DOG_SHUT_NORTH);
    } else {
      Actor_SetSpritePriority(id, 1);
    }
  }
  if (id == ACTOR_GUARD_CRATE) {
    struct Actor *crate = Actor_GetById(ACTOR_GUARD_CRATE);
    struct Actor *guard;

    if (!Event_GetFlag(FLAG_GUARD_CRATE_DROPPED) &&
        FIELD_CELL(crate->x.fixed) == GUARD_CRATE_CELL_X &&
        FIELD_CELL(crate->z.fixed) == GUARD_CRATE_CELL_Z) {
      FIXED dx;

      Actor_WaitToLand(ACTOR_GUARD_CRATE);
      Actor_SetSpritePriority(ACTOR_GUARD_CRATE, 2);
      guard = Actor_GetById(ACTOR_LOWER_GUARD);
      dx = crate->x.fixed - guard->x.fixed;
      if (dx >= 0 ? dx < toFIXED(8)
                  : guard->x.fixed - crate->x.fixed < toFIXED(8)) {
        guard->motionFlags = 0;
        guard->speedLimit = toFIXED(1);
        FieldActor_SetMoveTargetFX(guard, crate->x.fixed, crate->y.fixed,
                                   crate->z.fixed);
        FieldActor_WaitForMove(guard);
        Actor_StartScript(crate, gKibomboMountains_CrateJumpScript);
        Actor_StartIdle(guard);
        guard->x.fixed = 0;
        guard->z.fixed = 0;
        Event_SetFlag(FLAG_GUARD_KNOCKED_OUT);
      } else {
        Iwram_CopyWords(&saved, guard, sizeof(saved));
        Actor_ResetTargetsAndVelocity(ACTOR_LOWER_GUARD);
        FieldActor_Jump(ACTOR_LOWER_GUARD, 4, 0);
        FieldEvent_Wait(30);
        Actor_ShowEmote(ACTOR_LOWER_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        Actor_FaceActor(ACTOR_LOWER_GUARD, ACTOR_GUARD_CRATE, 0);
        FieldEvent_Wait(60);
        Actor_SetAnimationAndWait(ACTOR_LOWER_GUARD, ANIM_SHAKE_HEAD);
        FieldEvent_Wait(20);
        Iwram_CopyWords(guard, &saved, sizeof(saved));
        MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
        Event_SetFlag(FLAG_GUARD_CRATE_DROPPED);
      }
    }
  }
}

/// Felix casts Tremor on the crate with the bone on it, by the north end of
/// the dog's run, facing it from the south or the west: the crate shakes, the
/// bone drops to the ground, and the dog, unless shut in south of the pillar,
/// runs to it and stays there eating, no longer watching the run.
void KibomboMountains_TremorDropsBone(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetDestination(ACTOR_BONE, 764, 384);
  Actor_MoveToAndWait(ACTOR_BONE_CRATE, 746, 380);
  Actor_Get(ACTOR_BONE)->velocityY = toFIXED(4);
  FieldActor_WaitForMoveById(ACTOR_BONE);
  Actor_Get(ACTOR_BONE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  if (!Event_GetFlag(FLAG_KIBOMBO_LETS_PARTY_PASS) &&
      !Event_GetFlag(FLAG_DOG_SHUT_SOUTH)) {
    FieldActor_Stop(ACTOR_GUARD_DOG);
    Actor_MoveToAndWait(ACTOR_GUARD_DOG, 764, 388);
    Actor_SetDestination(ACTOR_BONE, 764, 376);
    Actor_WalkToAndWait(ACTOR_GUARD_DOG, 764, 380);
    Actor_SetAnimation(ACTOR_GUARD_DOG, 4);
    Actor_SetAttachedEffect(ACTOR_GUARD_DOG,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Event_SetFlag(FLAG_DOG_EATING_BONE);
  }
  Event_End();
}

/// Felix casts Whirlwind on the bush on Waft's ledges: the bush
/// blows away, and the opening behind it leads into the small cavern.
void KibomboMountains_BlowAwayBush(void) {
  GameFlag_Set(FLAG_KIBOMBO_BUSH_BLOWN_AWAY);
}

/// Felix walks west off the edge at cell (43, 30) on Waft's ledges and jumps
/// the gap ahead.
void KibomboMountains_JumpForward(void) { Field_TryJumpForward(); }

/// Piers casts Frost on the puddle high in the north west of the chest room,
/// cell (4, 16): it freezes into an ice pillar Felix can hop across.
void KibomboMountains_FreezeFirstPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
  Event_SetFlag(FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN);
}

/// Piers casts Frost on the puddle on the west path of the chest room, cell
/// (10, 30): it freezes into an ice pillar Felix can hop across.
void KibomboMountains_FreezeSecondPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_SECOND_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
  Event_SetFlag(FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN);
}

/// Nothing visible happens as Felix starts pushing one of the chest room's two
/// pairs of crates: the pair on the first guard's ledge, (8, 18) and (19, 15),
/// or the pair below the second guard, (20, 34) and (27, 33). The ground the
/// pair stood on is laid back from the copy kept beside the map, so that the
/// cells they leave are ordinary ground again.
void KibomboMountains_StartPushingChestRoomCrate(s32 unused, s32 id) {
  if (id == ACTOR_LOWER_WEST_CRATE || id == ACTOR_LOWER_EAST_CRATE)
    MapGrid_CopyCellAttributes(120, 33, 8, 2, 20, 33);
  if (id == ACTOR_WEST_CRATE || id == ACTOR_EAST_CRATE)
    MapGrid_CopyCellAttributes(112, 15, 16, 4, 8, 15);
}

/// Copies the cell at (sourceX, sourceY) of the map grid to the cell actor `id`
/// stands on.
void KibomboMountains_SetCrateCell(s32 id, s32 sourceX, s32 sourceY) {
  struct Actor *actor = Actor_Get(id);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  MapGrid_CopyCellAttributes(sourceX, sourceY, 1, 1, x, z);
}

/// Felix finishes pushing a crate or pillar in the chest room. The pillar in
/// the west, pushed off its ledge into the gap at cell (4, 28), lands for good
/// as a shortcut Felix can hop across; the pillar by the chest, dropped to
/// cell (28, 30), lands where he can climb it. Where the two crates below the
/// second guard stop, Felix can hop across their tops.
void KibomboMountains_FinishPushingChestRoomCrate(s32 unused, s32 id) {
  if (id == ACTOR_FIRST_DROPPED_PILLAR) {
    struct Actor *actor = Actor_Get(ACTOR_FIRST_DROPPED_PILLAR);

    if (!Event_GetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED)) {
      s32 x = FIELD_CELL(actor->x.fixed);

      if (x == 4) {
        Actor_WaitToLand(ACTOR_FIRST_DROPPED_PILLAR);
        MapGrid_CopyCellAttributes(3, 28, 1, 1, x, 28);
        Event_SetFlag(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED);
      }
    }
  }
  if (id == ACTOR_SECOND_DROPPED_PILLAR) {
    struct Actor *actor = Actor_Get(ACTOR_SECOND_DROPPED_PILLAR);

    if (!Event_GetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED)) {
      s32 x = FIELD_CELL(actor->x.fixed);

      if (x == 28) {
        s32 z = FIELD_CELL(actor->z.fixed);

        if (z == 30) {
          Actor_WaitToLand(ACTOR_SECOND_DROPPED_PILLAR);
          MapGrid_CopyCellAttributes(32, 30, 1, 2, x, z);
          Event_SetFlag(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED);
        }
      }
    }
  }
  if (id == ACTOR_WEST_CRATE || id == ACTOR_EAST_CRATE) {
    KibomboMountains_SetCrateCell(ACTOR_WEST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_EAST_CRATE, 8, 17);
  }
  if (id == ACTOR_LOWER_WEST_CRATE || id == ACTOR_LOWER_EAST_CRATE) {
    KibomboMountains_SetCrateCell(ACTOR_LOWER_WEST_CRATE, 22, 33);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_EAST_CRATE, 22, 33);
  }
}

extern struct Trigger gKibomboMountains_Triggers[];
extern struct Trigger gKibomboMountains_WaftTriggers[];
extern struct Trigger gKibomboMountains_DiskAxeChestTriggers[];
extern struct Trigger gKibomboMountains_CavernTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT)
    return gKibomboMountains_WaftTriggers;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST)
    return gKibomboMountains_DiskAxeChestTriggers;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_CAVERN)
    return gKibomboMountains_CavernTriggers;
  return gKibomboMountains_Triggers;
}

void KibomboMountains_GuardsTurnPartyBack(void);
void Rope_Initialize(void);

/// Sets up the room the party enters: in Waft's room the puddle, vine,
/// pillar and crate as this visit left them, the bone on its crate or in the
/// dog's mouth, and the bush unless blown away; in the chest room the rope,
/// the puddles, the dropped pillars and the crates' cells, and the warriors
/// marching the party back if it was caught. Returns 0.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s16 scene;
  s32 flag;
  s32 height;

  gFieldState->transition = ROOM_TRANSITION;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  scene = gMainRoutineState.next;
  if (scene == SCENE_KIBOMBO_MOUNTAINS_WAFT) {
    Actor_SetSpritePriority(ACTOR_WAFT, 1);
    Actor_SetSpritePriority(ACTOR_UNSEEN_MARKER, 1);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_UNSEEN_MARKER), 0);
    Actor_GetById(ACTOR_UNSEEN_MARKER)->collisionFlags = 0;
    Actor_GetById(ACTOR_UNSEEN_MARKER)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_SPROUT)->collisionFlags = 0;
    if (GameFlag_Get(FLAG_SPROUT_GROWN)) {
      Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
      Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Get(FLAG_WAFT_ROOM_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
    if (GameFlag_Get(FLAG_DOG_PILLAR_DROPPED))
      MapGrid_CopyCellAttributes(49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(ACTOR_DOG_PILLAR, 1);
    if (GameFlag_Get(FLAG_GUARD_CRATE_DROPPED))
      MapGrid_CopyCellAttributes(53, 35, 1, 1, 53, 36);
    flag = GameFlag_Get(FLAG_DOG_EATING_BONE);
    if (flag == 0) {
      struct Actor *lift;

      Actor_GetById(ACTOR_BONE_CRATE)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
      lift = Actor_GetById(ACTOR_BONE);
      lift->priorityFlags |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
      lift->y.fixed = toFIXED(224);
      lift->groundHeight = toFIXED(224);
      lift->collisionFlags = flag;
    } else {
      Actor_SetAttachedEffect(ACTOR_GUARD_DOG, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ATTACH);
    }
    Actor_ShowItemIconById(ACTOR_BONE, ITEM_BONE);
    if (GameFlag_Get(FLAG_KIBOMBO_BUSH_BLOWN_AWAY))
      Actor_SetPosition(ACTOR_CAVERN_BUSH, 0, 0);
    else
      Actor_GetById(ACTOR_CAVERN_BUSH)->scaleY = FIXED_ONE * 5 / 4;
  } else if (scene == SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST) {
    Actor_SetSpritePriority(ACTOR_WEST_ROPE_POST, 1);
    Actor_SetSpritePriority(ACTOR_EAST_ROPE_POST, 1);
    Rope_Initialize();
    FieldRope_Place(0, FLAG_KIBOMBO_ROPE_TIED, ACTOR_WEST_ROPE_POST,
                    ACTOR_EAST_ROPE_POST);
    if (GameFlag_Get(FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
    if (GameFlag_Get(FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
    if (GameFlag_Get(FLAG_KIBOMBO_FIRST_PILLAR_DROPPED)) {
      actor = Actor_GetById(ACTOR_FIRST_DROPPED_PILLAR);
      Actor_SetPosition(ACTOR_FIRST_DROPPED_PILLAR, 72, 456);
      actor->motionFlags = 0;
      height = Map_GetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed,
                                    actor->z.fixed);
      actor->groundHeight = height;
      actor->y.fixed = height;
      MapGrid_CopyCellAttributes(3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Get(FLAG_KIBOMBO_SECOND_PILLAR_DROPPED)) {
      actor = Actor_GetById(ACTOR_SECOND_DROPPED_PILLAR);
      Actor_SetPosition(ACTOR_SECOND_DROPPED_PILLAR, 456, 488);
      actor->motionFlags = 0;
      height = Map_GetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed,
                                    actor->z.fixed);
      actor->groundHeight = height;
      actor->y.fixed = height;
      MapGrid_CopyCellAttributes(32, 30, 1, 2, 28, 30);
    }
    MapGrid_CopyCellAttributes(8, 15, 16, 4, 112, 15);
    MapGrid_CopyCellAttributes(20, 33, 8, 2, 120, 33);
    KibomboMountains_SetCrateCell(ACTOR_WEST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_EAST_CRATE, 8, 17);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_WEST_CRATE, 22, 33);
    KibomboMountains_SetCrateCell(ACTOR_LOWER_EAST_CRATE, 22, 33);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineState.entrance == ENTRANCE_CAUGHT)
      KibomboMountains_GuardsTurnPartyBack();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The first time Felix climbs the stairs between the two torches below the
/// stone gateway the second guard keeps, cells (15, 33) to (16, 34): a warrior
/// comes asking "Is the way clear?", the second guard reports that the strange
/// one has not been seen, and the warrior hurries off to find whoever attacked
/// their guards, leaving him on watch.
void KibomboMountains_GuardsReport(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId(MsgKibomboMountainsIsWayClear);
  Actor_SetPosition(ACTOR_WARRIOR_E, 312, 304);
  // "Is the way clear?"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  Actor_ResetTargetsAndVelocity(ACTOR_SECOND_GUARD);
  Actor_FaceActor(ACTOR_SECOND_GUARD, ACTOR_WARRIOR_E, 0);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 256, 400);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  // "This area is clear! The strange one has not been seen!"
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Actor_RunRepeatedMotion(ACTOR_SECOND_GUARD, 1);
  // "And there?"
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_E, ANIM_SHAKE_HEAD);
  // "We have not seen the stranger near the village."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  // "Do you believe he was truly headed toward Kibombo?"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "The guards here were only recently knocked unconscious."
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Correct. We were under orders to maintain a strict watch over the..."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  // "No one must be permitted into Kibombo until Akafubu has completed..."
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Then we had better hurry to find the one who attacked our guards."
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_WalkToAndWait(ACTOR_WARRIOR_E, 264, 312);
  Actor_WalkTo(ACTOR_WARRIOR_E, 360, 312);
  Actor_SetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  // "Keep a good watch. Do not permit anyone to pass."
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  FieldActor_WatchAndSetScript(ACTOR_SECOND_GUARD,
                               gMainRoutineState.leaderActorId);
  System_Wait(30);
  // "If anyone interrupts Akafubu's ceremony, he will hold you responsible..."
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Actor_RunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
  FieldEvent_Wait(60);
  Actor_SetPosition(ACTOR_WARRIOR_E, 0, 0);
  Actor_SetScript(ACTOR_SECOND_GUARD, gKibomboMountains_SecondGuardPaceScript);
  Event_End();
  Event_SetFlag(FLAG_KIBOMBO_GUARDS_REPORTED);
}

/// Sets an actor walking slowly, at half a pixel a frame.
void KibomboMountains_WalkSlowly(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SelectAnimation(actor, ANIM_STAND);
  actor->acceleration = toFIXED(1) / 4;
  actor->speedLimit = toFIXED(1) / 2;
}

/// Caught for the first time on this visit, the party is back at the top of
/// the stairs between two torches, below the second guard's stone gateway,
/// surrounded by six warriors: "Here! The intruder is here! Kibombo does not
/// permit visitors!" Two of them walk Felix down the stairs, and all of them
/// leave.
void KibomboMountains_GuardsTurnPartyBack(void) {
  s32 leader = gMainRoutineState.leaderActorId;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_WARRIOR_A, 248, 472);
  Actor_SetPosition(ACTOR_WARRIOR_B, 264, 472);
  Actor_SetPosition(ACTOR_WARRIOR_C, 235, 483);
  Actor_SetPosition(ACTOR_WARRIOR_D, 232, 496);
  Actor_SetPosition(ACTOR_WARRIOR_E, 277, 483);
  Actor_SetPosition(ACTOR_WARRIOR_F, 280, 496);
  KibomboMountains_WalkSlowly(leader);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_A);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_B);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_C);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_D);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_E);
  KibomboMountains_WalkSlowly(ACTOR_WARRIOR_F);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgKibomboMountainsHereIntruderIsHere);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Here! The intruder is here!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_FaceActor(ACTOR_WARRIOR_E, ACTOR_WARRIOR_C, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_C, ANIM_NOD);
  Actor_FaceActor(ACTOR_WARRIOR_E, leader, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WARRIOR_E, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Kibombo does not permit visitors!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_E, ANIM_NOD);
  // "You just follow this road right back to where you came from!"
  Event_ShowMessage(ACTOR_WARRIOR_E, 0);
  Actor_FaceActor(ACTOR_WARRIOR_C, ACTOR_WARRIOR_A, 0);
  Actor_ShowEmote(ACTOR_WARRIOR_C, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Guard! Get him out of here!"
  Event_ShowMessage(ACTOR_WARRIOR_C, 0);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 536);
  FieldEvent_Wait(1);
  Actor_WalkTo(ACTOR_WARRIOR_B, 264, 536);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_C, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_D, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_E, leader);
  FieldActor_WatchAndSetScript(ACTOR_WARRIOR_F, leader);
  FieldEvent_Wait(2);
  Actor_Get(leader)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(leader, 256, 552);
  FieldEvent_Wait(1);
  Actor_Get(leader)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_WARRIOR_A, ANIM_STAND);
  Actor_SetAnimation(ACTOR_WARRIOR_B, ANIM_STAND);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_WARRIOR_A, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(3);
  Actor_SetAnimationAndWait(ACTOR_WARRIOR_B, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 488);
  FieldEvent_Wait(2);
  Actor_WalkToAndWait(ACTOR_WARRIOR_B, 264, 488);
  Actor_WalkTo(ACTOR_WARRIOR_A, 248, 440);
  Actor_WalkTo(ACTOR_WARRIOR_B, 264, 440);
  Actor_WalkTo(ACTOR_WARRIOR_C, 232, 456);
  Actor_WalkTo(ACTOR_WARRIOR_D, 232, 472);
  Actor_WalkTo(ACTOR_WARRIOR_E, 280, 456);
  Actor_WalkToAndWait(ACTOR_WARRIOR_F, 280, 472);
  Actor_SetPosition(ACTOR_WARRIOR_A, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_B, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_C, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_D, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_E, 0, 0);
  Actor_SetPosition(ACTOR_WARRIOR_F, 0, 0);
  Event_End();
  Event_SetFlag(FLAG_TURNED_BACK);
}

/// Sends the party back to the stairs below the second guard's gateway:
/// the first time on a visit to be met by the warriors, after that only put
/// back.
void KibomboMountains_SendPartyBack(void) {
  s32 moved = Event_GetFlag(FLAG_TURNED_BACK);
  s32 scene = SCENE_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST;

  if (moved)
    Event_SetDestination(scene, ENTRANCE_CAUGHT_AGAIN);
  else
    Event_SetDestination(scene, ENTRANCE_CAUGHT);
}

/// A guard sees Felix: he starts, turns to face him, "Guards! An intruder!
/// Someone is trying to pass! To arms, Kibombo warriors! To arms!", and the
/// party is sent back to the stairs below the second guard's gateway.
void KibomboMountains_GuardSpotsParty(s32 id) {
  s32 leader = gMainRoutineState.leaderActorId;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_ResetTargetsAndVelocity(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, leader, 20);
  FieldEvent_SetMessageId(MsgKibomboMountainsGuardsIntruder);
  // "Guards! An intruder!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(30);
  // "Someone is trying to pass! To arms, Kibombo warriors! To arms!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  KibomboMountains_SendPartyBack();
  Event_End();
}

/// The guard dog sees Felix: it starts, "Grrrrr! Rrrruff! Rrufff!", the upper
/// guard shouts "An intruder! Repel the intruder immediately!", and the party
/// is sent back to the stairs below the second guard's gateway.
void KibomboMountains_DogSpotsParty(s32 id) {
  s32 leader = gMainRoutineState.leaderActorId;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldActor_Stop(id);
  Actor_ShowEmote(id, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(id, leader, 20);
  FieldEvent_SetMessageId(MsgKibomboMountainsGrrrrrRrrruffRrufff);
  // "Grrrrr! Rrrruff! Rrufff!"
  Event_ShowMessage(id, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(id, 4);
  FieldEvent_Wait(30);
  // "An intruder! Repel the intruder immediately!"
  Event_ShowMessage(ACTOR_UPPER_GUARD, 0);
  Actor_SetAttachedEffect(leader,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  KibomboMountains_SendPartyBack();
  Event_End();
}

/// Felix steps onto the first guard's ledge west of where he paces, cells
/// (8, 18) to (10, 19): the guard sees him and the party is sent back, unless
/// the west crate stands just east of Felix in the same row, hiding him.
void KibomboMountains_StepWestOfFirstGuard(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *crate = Actor_Get(ACTOR_WEST_CRATE);

  if ((crate->z.fixed - leader->z.fixed >= 0
           ? crate->z.fixed - leader->z.fixed
           : leader->z.fixed - crate->z.fixed) < toFIXED(16) &&
      crate->x.fixed > leader->x.fixed)
    return;
  KibomboMountains_GuardSpotsParty(ACTOR_FIRST_GUARD);
}

/// Felix steps onto the first guard's ledge east of where he paces, cells
/// (16, 18) to (19, 18): the guard sees him and the party is sent back, unless
/// the east crate stands just west of Felix in the same row, hiding him.
void KibomboMountains_StepEastOfFirstGuard(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *crate = Actor_Get(ACTOR_EAST_CRATE);

  if ((crate->z.fixed - leader->z.fixed >= 0
           ? crate->z.fixed - leader->z.fixed
           : leader->z.fixed - crate->z.fixed) < toFIXED(16) &&
      crate->x.fixed < leader->x.fixed)
    return;
  KibomboMountains_GuardSpotsParty(ACTOR_FIRST_GUARD);
}

/// Felix steps east of the second guard, cells (18, 28) to (18, 30): the
/// guard sees him and the party is sent back.
void KibomboMountains_StepEastOfSecondGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_SECOND_GUARD);
}

/// Felix steps south of the second guard, cells (14, 30) to (17, 32), in
/// front of where he looks: the guard sees him and the party is sent back.
void KibomboMountains_StepSouthOfSecondGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_SECOND_GUARD);
}

/// Felix steps onto the line of the rope above the third guard, cells
/// (26, 12) and (27, 12): walking the rope he is not seen, but anywhere else
/// there the guard sees him and the party is sent back.
void KibomboMountains_StepOnRopeAboveThirdGuard(void) {
  if (gMainRoutineState.movementMode != LEADER_MOVEMENT_ROPE)
    KibomboMountains_GuardSpotsParty(ACTOR_THIRD_GUARD);
}

/// Felix steps west of the third guard, cells (25, 15), (24, 16) and
/// (25, 16): the guard sees him and the party is sent back.
void KibomboMountains_StepWestOfThirdGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_THIRD_GUARD);
}

/// Felix steps south west of the upper guard on Waft's ledges, cells (35, 21)
/// to (35, 23): the guard sees him and the party is sent back.
void KibomboMountains_StepWestOfUpperGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_UPPER_GUARD);
}

/// Felix steps south east of the upper guard, cells (41, 23) to (41, 26): the
/// guard sees him and the party is sent back.
void KibomboMountains_StepEastOfUpperGuard(void) {
  KibomboMountains_GuardSpotsParty(ACTOR_UPPER_GUARD);
}

/// Felix steps by the north end of the dog's run, cells (49, 22) to (49, 24),
/// (45, 24) and (45, 25): the dog sees him and barks, and the party is sent
/// back, unless the dog is eating the bone or shut in south of the pillar.
void KibomboMountains_StepByNorthEndOfDogRun(void) {
  if (!Event_GetFlag(FLAG_DOG_EATING_BONE) &&
      !Event_GetFlag(FLAG_DOG_SHUT_SOUTH))
    KibomboMountains_DogSpotsParty(ACTOR_GUARD_DOG);
}

/// Felix steps south of the lower guard's path, cells (51, 36) to (51, 38) and
/// (47, 38) to (47, 40): the guard sees him and the party is sent back, unless
/// the crate has trapped him.
void KibomboMountains_StepSouthOfLowerGuard(void) {
  if (!Event_GetFlag(FLAG_GUARD_KNOCKED_OUT))
    KibomboMountains_GuardSpotsParty(ACTOR_LOWER_GUARD);
}

/// Felix steps south east of the lower guard's path, cells (54, 38) to
/// (55, 41): the guard sees him and the party is sent back, unless the crate
/// has trapped him.
void KibomboMountains_StepSouthEastOfLowerGuard(void) {
  if (!Event_GetFlag(FLAG_GUARD_KNOCKED_OUT))
    KibomboMountains_GuardSpotsParty(ACTOR_LOWER_GUARD);
}

/// Felix steps by the south end of the dog's run, cells (50, 30) to (50, 32):
/// the dog sees him and barks, and the party is sent back, unless the dog is
/// eating the bone or shut in north of the pillar.
void KibomboMountains_StepBySouthEndOfDogRun(void) {
  if (!Event_GetFlag(FLAG_DOG_SHUT_NORTH) &&
      !Event_GetFlag(FLAG_DOG_EATING_BONE))
    KibomboMountains_DogSpotsParty(ACTOR_GUARD_DOG);
}

extern s32 gKibomboMountains_FirstGuardPaceScript[];

// clang-format off
/// A torch burning.
s32 gKibomboMountains_TorchScript[] = {
    SC_ANIMATION(2),
    SC_END,
};

/// Unused: a flame raised 24 pixels and held there, as Naribwe's
/// gNaribwe_FlameRises raises the flames over its braziers.
UNUSED s32 gKibomboMountains_FlameRisesScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(24)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(KibomboMountainsWaft),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_WAFT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, KibomboMountainsDiskAxeChest, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_KIBOMBO_MOUNTAINS_WAFT_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, KibomboMountainsCavern, KIBOMBO_MOUNTAINS_CAVERN_FROM_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountainsDiskAxeChest),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboMountainsCavern),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, KibomboMountainsWaft, KIBOMBO_MOUNTAINS_WAFT_FROM_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// A crate or pillar held still until it is pushed.
s32 gKibomboMountains_CrateScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// The mountain path's actors.
struct ActorPlacement gKibomboMountains_Placements[] = {
    {SCENE_TABLE_END},
};

/// A guard keeping watch where he stands, looking about to the south.
s32 gKibomboMountains_WatchScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The lower guard pacing his path, looking about to the south at each end.
s32 gKibomboMountains_PaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(856, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(792, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The crate over the knocked-out guard, jumping and rocking as he struggles
/// inside.
s32 gKibomboMountains_CrateJumpScript[] = {
    SC_CALL(Actor_Bob),
    SC_END,
};

/// The guard dog running up and down its path, hopping as it goes.
s32 gKibomboMountains_DogRunScript[] = {
    SC_SPEED(1.25),
    SC_ACCELERATION(0.25),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(3),
    SC_SET(SW_VELOCITY_Y, toFIXED(3)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_MOVE(760, 0, 392),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(3)),
    SC_MOVE(764, 0, 408),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 424),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 440),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 456),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(764, 0, 472),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(2)),
    SC_MOVE(760, 0, 488),
    SC_SYNC,
    SC_MOVE(764, 0, 504),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Waft's ledges' actors, by enum KibomboMountainsWaftActorId.
struct ActorPlacement gKibomboMountains_WaftPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_WatchScript, 616, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_PaceScript, 824, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_DogRunScript, 760, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 544, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 712, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 888, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 732, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WAFT), MOVEMENT_TYPE_STAND, 440, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 584, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The first guard pacing his ledge, looking about to the south at each end.
s32 gKibomboMountains_FirstGuardPaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(184, 0, 296),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(248, 0, 296),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The second guard pacing his ledge, looking about to the south at each end.
s32 gKibomboMountains_SecondGuardPaceScript[] = {
    SC_SPEED(0.5),
    SC_ACCELERATION(0.1),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(248, 0, 448),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 448),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(3),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The chest room's actors, by enum KibomboMountainsDiskAxeChestActorId.
struct ActorPlacement gKibomboMountains_DiskAxeChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_FirstGuardPaceScript, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_SecondGuardPaceScript, 248, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_KIBOMBO_LETS_PARTY_PASS, gKibomboMountains_WatchScript, 424, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VINE_PILLAR, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 456, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 136, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 312, 0, 248, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 328, 0, 552, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 440, 0, 536, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, gKibomboMountains_CrateScript, 88, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, gKibomboMountains_TorchScript, 232, 0, 536, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, gKibomboMountains_TorchScript, 280, 0, 536, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_WatchScript, 0, 0, 0, FACING_SOUTH - 256, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_PaceScript, 0, 0, 0, FACING_SOUTH + 256, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_WatchScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_PaceScript, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_WatchScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, gKibomboMountains_PaceScript, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The mountain path's triggers.
struct Trigger gKibomboMountains_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The triggers of Waft's ledges.
struct Trigger gKibomboMountains_WaftTriggers[] = {
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_WORLD_MAP},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_A},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_EAST_B},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_WEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_NORTH},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A, IF_FLAG_SET(FLAG_KIBOMBO_BUSH_BLOWN_AWAY), KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_A},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_WAFT_TO_KIBOMBO_MOUNTAINS_CAVERN_SOUTH_B},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_OF_UPPER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepWestOfUpperGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_OF_UPPER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepEastOfUpperGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_NORTH_END_OF_DOG_RUN, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepByNorthEndOfDogRun)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_OF_LOWER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepSouthOfLowerGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_EAST_OF_LOWER_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepSouthEastOfLowerGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_END_OF_DOG_RUN, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepBySouthEndOfDogRun)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_JUMP_WEST, CONDITION_ALWAYS, EVENT(KibomboMountains_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingGuardCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_DropOffLedge)},
    {TRIGGER_TALK, ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgKibomboMountainsSniffSniffLickLickLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, IF_FLAG_SET(FLAG_DOG_EATING_BONE), MSGNO(MsgKibomboMountainsBoneThatsMyFavruffLick)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_DOG, CONDITION_ALWAYS, MSGNO(MsgKibomboMountainsThisRock)},
    {TRIGGER_TALK, ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgKibomboMountainsHeyWhosThereGetMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_CRATE, IF_FLAG_SET(FLAG_GUARD_KNOCKED_OUT), MSGNO(MsgKibomboMountainsItsDarkItsCrampedSomebody)},
    {TRIGGER_TALK, ACTOR_WAFT, CONDITION_ALWAYS, EVENT(KibomboMountains_FightWaft)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_WAFT_ROOM_PUDDLE_FROZEN, EVENT(KibomboMountains_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SPROUT_GROWN, EVENT(KibomboMountains_GrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVERN_BUSH, CONDITION_ALWAYS, EVENT(KibomboMountains_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOG_PILLAR, CONDITION_ALWAYS, EVENT(KibomboMountains_DropOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_GUARD_CRATE, FLAG_GUARD_KNOCKED_OUT, EVENT(KibomboMountains_StartPushingGuardCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARD_CRATE, FLAG_GUARD_KNOCKED_OUT, EVENT(KibomboMountains_DropOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), FACING_SUBJECT(FACING_NORTH, ACTOR_BONE_CRATE), FLAG_DOG_EATING_BONE, EVENT(KibomboMountains_TremorDropsBone)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), FACING_SUBJECT(FACING_EAST, ACTOR_BONE_CRATE), FLAG_DOG_EATING_BONE, EVENT(KibomboMountains_TremorDropsBone)},
    {TRIGGER_TABLE_END},
};

/// The chest room's triggers.
struct Trigger gKibomboMountains_DiskAxeChestTriggers[] = {
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_WORLD_MAP},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_A},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_EAST_B},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_TO_KIBOMBO_MOUNTAINS_WAFT_WEST},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_OF_FIRST_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepWestOfFirstGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_OF_FIRST_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepEastOfFirstGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_OF_SECOND_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepEastOfSecondGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_OF_SECOND_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepSouthOfSecondGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BETWEEN_TORCHES, FLAG_KIBOMBO_GUARDS_REPORTED, EVENT(KibomboMountains_GuardsReport)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROPE_ABOVE_THIRD_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepOnRopeAboveThirdGuard)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_OF_THIRD_GUARD, FLAG_KIBOMBO_LETS_PARTY_PASS, EVENT(KibomboMountains_StepWestOfThirdGuard)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_WEST_ROPE_POST, FLAG_KIBOMBO_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_DISK_AXE_ROOM_FIRST_PUDDLE_FROZEN, EVENT(KibomboMountains_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_DISK_AXE_ROOM_SECOND_PUDDLE_FROZEN, EVENT(KibomboMountains_FreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_DROPPED_PILLAR, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_DROPPED_PILLAR, FLAG_KIBOMBO_FIRST_PILLAR_DROPPED, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOWER_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOWER_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_StartPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_WEST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_EAST_CRATE, CONDITION_ALWAYS, EVENT(KibomboMountains_FinishPushingChestRoomCrate)},
    {TRIGGER_TABLE_END},
};

/// The small cavern's triggers.
struct Trigger gKibomboMountains_CavernTriggers[] = {
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_NORTH},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_A},
    {TRIGGER_EXIT, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B, CONDITION_ALWAYS, KIBOMBO_MOUNTAINS_CAVERN_TO_KIBOMBO_MOUNTAINS_WAFT_SOUTH_B},
    {TRIGGER_TABLE_END},
};
// clang-format on
