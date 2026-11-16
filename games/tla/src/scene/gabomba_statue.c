/// @file
/// The Gabomba Statue above Kibombo, in the seven rooms of this overlay: halls
/// of pinions and cogs the party rides and hops across, stopped by Lashing a
/// rope (GabombaStatue_LashRope) and turned the other way by Pounding the two
/// cone pillars beside a flashing cog (GabombaStatue_PoundConePillar). From
/// inside, the party hears the offering made below
/// (GabombaStatue_HearTheOffering), and works out that the channel must bring
/// two magical charges to the statue in the middle at once
/// (GabombaStatue_FocusEnergies to GabombaStatue_FixTheChannel).

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#include "flags.h"
#include "djinn_ids.h"

/// Story flags: both cone pillars Pounded and the cogs turned back, each cone
/// Pounded, and the mouse that shows the way seen.
#define FLAG_GABOMBA_COGS_TURNED_BACK 0x8cd
/// The party has watched how the statue's works run.
#define FLAG_GABOMBA_SAW_HOW_IT_WORKS 0x8f6
/// The broken channel is fixed ("just fixing the channel should do the
/// trick").
#define FLAG_GABOMBA_CHANNEL_FIXED 0x9ec
#define FLAG_GABOMBA_FIRST_CONE_POUNDED 0x8ce
#define FLAG_GABOMBA_SECOND_CONE_POUNDED 0x8cf
#define FLAG_GABOMBA_MOUSE_SEEN 0xa6c

/// The mouse of the turntable room that runs ahead and shows the way
/// (GabombaStatue_MouseShowsTheWay).
enum { ACTOR_MOUSE = 39 };

/// The arrows room at the top, where Akafubu waits with Kraden for the
/// offering (GabombaStatue_HearTheOffering, GabombaStatue_SeeHowItWorks).
enum { ACTOR_KRADEN = 23, ACTOR_AKAFUBU = 24 };

/// Each room's placed actors. A row of cogs that share one sprite is named by
/// the first of them: turning its animation turns them all. A Lashed rope runs
/// between two posts, its second post riding a cog's tooth.
enum {
  /// The hole the party climbed out of, a dark patch held above the floor.
  ACTOR_BONE_ARMLET_HOLE_SHADOW = 8
};
enum {
  ACTOR_FIRST_ROPE_POST_A = 8,
  ACTOR_FIRST_ROPE_POST_B,
  ACTOR_SECOND_ROPE_POST_A,
  ACTOR_SECOND_ROPE_POST_B,
  ACTOR_FIRST_CONE_PILLAR,
  ACTOR_SECOND_CONE_PILLAR,
  ACTOR_ELIXIR_GREEN_COGS,
  ACTOR_ELIXIR_BLUE_COGS = 21,
  /// The blue cog that flashes until both cone pillars are Pounded.
  ACTOR_FLASHING_COG = 26,
  ACTOR_ELIXIR_PINION_A,
  ACTOR_ELIXIR_PINION_B,
  ACTOR_ELIXIR_HOLE_SHADOW_A, ///< over the west and east holes
  ACTOR_ELIXIR_HOLE_SHADOW_B
};
enum {
  ACTOR_STEEL_GREEN_COGS = 8,
  ACTOR_STEEL_BLUE_COGS = 15,
  ACTOR_STEEL_RED_COGS = 20,
  ACTOR_STEEL_BLUE_PILLARS = 25,
  ACTOR_STEEL_PINK_DRUMS = 29,
  ACTOR_STEEL_ROPE_POST_A = 31,
  ACTOR_STEEL_ROPE_POST_B
};
enum {
  ACTOR_TURNTABLE_GREEN_COGS = 8,
  ACTOR_TURNTABLE_BLUE_COGS = 21,
  ACTOR_TURNTABLE_RED_COGS = 29,
  ACTOR_TURNTABLE_BLUE_PILLAR_A = 34,
  ACTOR_TURNTABLE_BLUE_PILLAR_B,
  ACTOR_TURNTABLE_PINK_DRUM_A = 37,
  ACTOR_TURNTABLE_PINK_DRUM_B
};
enum {
  ACTOR_COGS_ROPE_POST_A = 8,
  ACTOR_COGS_ROPE_POST_B,
  ACTOR_COGS_RED_COGS,
  ACTOR_COGS_PINION_A = 12,
  ACTOR_COGS_PINION_B
};
/// The arrows room: the idol in the middle, the statues holding the blue and
/// the red orb whose charges run along the channel, the grey cone that raises
/// the blocks, and the seven blocks the channel is laid on ("It looks like the
/// energy charge is supposed to run over these blocks.").
enum {
  ACTOR_IDOL = 8,
  ACTOR_BLUE_ORB_STATUE,
  ACTOR_RED_ORB_STATUE,
  ACTOR_CHARGE_A, ///< the blue orb's two charges, then the red orb's
  ACTOR_CHARGE_B,
  ACTOR_CHARGE_C,
  ACTOR_CHARGE_D,
  ACTOR_GREY_CONE,
  ACTOR_BLOCK_A,
  ACTOR_BLOCK_B,
  ACTOR_BLOCK_C,
  ACTOR_BLOCK_D,
  ACTOR_BLOCK_E,
  ACTOR_BLOCK_F,
  ACTOR_BLOCK_G
};
enum {
  ACTOR_PINION_A = 8,
  ACTOR_PINION_B,
  ACTOR_PINION_C,
  ACTOR_PINION_D,
  ACTOR_PINION_E
};

/// The triggers this overlay raises: both cone pillars Pounded, the grey cone
/// Pounded, and the offering's clock run out.
enum {
  RAISED_COGS_TURN_BACK = 200,
  RAISED_CONE_CHIMES = 200,
  RAISED_OFFERING_HEARD = 201
};
#include "graphics/palette_buffers.h"
#include "system/io_reg.h"
#include "field/field.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "scene/gabomba_statue.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "system/system.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "field/field_event.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "system/input.h"
#include "system/task.h"
#include "shared/tables.h"
#include "system/heap_release.h"
#include "field/field_effect.h"
#include "graphics/resource.h"
#include "system/affine.h"
#include "graphics/vram_tab.h"
#include "field/call.h"
#include "field/map_grid.h"
#include "field/map.h"
#include "field/actor_script.h"
#include "fixed_math.h"

extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// Steel, the Djinni waiting in this room.
enum { ACTOR_STEEL = 33 };

/// Talking to Steel starts its battle.
void GabombaStatue_FightSteel(void) {
  Field_RequestDjinnBattle(ACTOR_STEEL, DJINNI_STEEL);
}
/// A member of the party looking about: it turns between its two headings
/// every 10 to 49 frames.
s32 GabombaStatue_LookAround(struct Actor *actor) {
  if (actor->lookTimer != 0) {
    actor->lookTimer--;
  } else {
    actor->lookTimer = (Random_Below(40)) + 10;
    if (actor->lookTurned != 0) {
      actor->lookTurned = 0;
      actor->facing = actor->lookFacing;
    } else {
      actor->lookTurned = 1;
      actor->facing = actor->lookOtherFacing;
    }
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gGabombaStatue_PinionExitRegions[];
extern struct SceneRegion gGabombaStatue_BoneArmletChestExitRegions[];
extern struct SceneRegion gGabombaStatue_ElixirChestExitRegions[];
extern struct SceneRegion gGabombaStatue_SteelExitRegions[];
extern struct SceneRegion gGabombaStatue_TurntableExitRegions[];
extern struct SceneRegion gGabombaStatue_CogsExitRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_PINION)
    return gGabombaStatue_PinionExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    return gGabombaStatue_BoneArmletChestExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    return gGabombaStatue_ElixirChestExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_STEEL)
    return gGabombaStatue_SteelExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_TURNTABLE)
    return gGabombaStatue_TurntableExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_COGS)
    return gGabombaStatue_CogsExitRegions;
  return NULL;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The flashing cog's update: its colours swap between two orders every ten
/// frames.
s32 GabombaStatue_FlashCog(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame % 10 == 0)
    ((struct Sprite *)actor->sprite)->entries[0]->colorMode =
        frame % 20 / 10 + SPRITE_COLOR_BGR;
  return 1;
}

/// An actor as this scene's cogs, teeth, rope posts and charges use it: the
/// scene's own bytes from phase on.
struct GabombaActor {
  u8 unusedScript[6]; ///< the script and its cursor
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unusedBeforePriority[15];
  u8 priorityFlags;
  u8 unusedBeforeAnimation[44];
  void **animation;
  u8 kind;
  u8 motionFlags;
  u8 unusedBeforePhase[12];
  u8 phase;
  u8 slot;
  s16 centerX;
  s16 centerZ;
  void *link;
};

/// The steps of a cog tooth's turn: its phase counts them, two degrees each.
enum { COG_TOOTH_STEPS = 180 };

/// The arrows room's channel: a grid of blocks, eleven columns by eight rows.
enum {
  CHANNEL_COLUMNS = 11,
  CHANNEL_ROWS = 8,
  CHANNEL_CELLS = CHANNEL_COLUMNS * CHANNEL_ROWS
};
extern s32 gGabombaStatue_CogDirection;
s32 __divsi3(s32 numerator, s32 denominator);

/// Where the cog tooth turning about actor's centre is now, in 16.16 pixels.
void GabombaStatue_GetToothPosition(struct GabombaActor *actor, s32 *x,
                                    s32 *z) {
  s32 position[3];
  s32 angle = actor->phase * TRIG_TURN / COG_TOOTH_STEPS;
  s32 turn;

  SET_VECTOR(position, INTtoFIXED(actor->centerX + 16), toFIXED(16),
             INTtoFIXED(actor->centerZ));
  turn = gGabombaStatue_CogDirection * (TRIG_TURN * 3 / 16) + angle;
  FieldVector_AddPolarOffset(13, turn + DEGtoANG(180), position);
  *x = position[0];
  *z = position[2];
}

/// A cog tooth's update: it turns about the cog's centre by the cogs'
/// direction.
s32 GabombaStatue_TurnTooth(struct GabombaActor *actor) {
  u8 *phase = &actor->phase;
  s32 angle = *phase * TRIG_TURN / COG_TOOTH_STEPS;
  s32 next;

  actor->priorityFlags = 0;
  SET_POSITION(actor, INTtoFIXED(actor->centerX + 16), toFIXED(16),
               INTtoFIXED(actor->centerZ));
  FieldVector_AddPolarOffset(13, angle + DEGtoANG(180), &actor->x);
  next = *phase + gGabombaStatue_CogDirection;
  if (next < 0)
    next = COG_TOOTH_STEPS - 1;
  if (next > COG_TOOTH_STEPS - 1)
    next = 0;
  *phase = next;
  actor->link = GabombaStatue_GetToothPosition;
  return 1;
}

/// A sprite as the depth queue takes it (struct QueuedSprite), its affine mode
/// and matrix written as bytes.
struct SpriteView {
  u8 link[4]; ///< the depth queue's link to the next sprite
  u8 y;
  u8 affineMode : 2;
  u8 restOfAttribute0 : 6;
  u8 xLow;
  u8 xHigh : 1;
  u8 matrix : 5;
  u8 size : 2;
  u32 attribute2;
};

struct SceneState {
  struct PuzzleCell cells[CHANNEL_ROWS][CHANNEL_COLUMNS];
  s32 count;
  s16 firstId;
  s16 endId;
  s16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 statuesReady;
  u8 unusedAfterStatuesReady[2];
  s32 statueOffset;
  struct SpriteView oam[10];
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
#define OBJECT_BYTE(actor, offset) (((u8 *)(actor))[offset])

extern struct SceneState gSceneState;
extern s8 gGabombaStatue_CogAnimations[];

/// Each frame, keeps an actor circling the actor it watches.
s32 GabombaStatue_CircleOwner(struct Actor *actor) {
  struct Actor *owner = actor->watchedActor;

  if (owner != NULL) {
    u8 *phase = &actor->riseCounter;

    actor->x.fixed = owner->x.fixed;
    actor->y.fixed = owner->y.fixed + toFIXED(16);
    actor->z.fixed = owner->z.fixed;
    actor->facing = *phase << 8;
    FieldVector_AddPolarOffset(6, *phase << 8, &actor->x.fixed);
    *phase += 16;
  }
  return 1;
}

extern struct ActorPlacement gGabombaStatue_Placements[];
extern struct ActorPlacement gGabombaStatue_BoneArmletChestActorPlacements[];
extern struct ActorPlacement gGabombaStatue_ElixirChestActorPlacements[];
extern struct ActorPlacement gGabombaStatue_SteelActorPlacements[];
extern struct ActorPlacement gGabombaStatue_TurntableActorPlacements[];
extern struct ActorPlacement gGabombaStatue_CogsActorPlacements[];
extern struct ActorPlacement gGabombaStatue_ArrowsActorPlacements[];
extern struct ActorPlacement gGabombaStatue_PinionActorPlacements[];

const u8 *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;
  const u8 *table = gGabombaStatue_Placements;

  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    table = gGabombaStatue_BoneArmletChestActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    table = gGabombaStatue_ElixirChestActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_STEEL)
    table = gGabombaStatue_SteelActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_TURNTABLE)
    table = gGabombaStatue_TurntableActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_COGS)
    table = gGabombaStatue_CogsActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_ARROWS)
    table = gGabombaStatue_ArrowsActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_PINION)
    table = gGabombaStatue_PinionActorPlacements;
  return table;
}
#undef REG_BLDALPHA
#undef OBJECT_BYTE

extern s8 gGabombaStatue_RopeStopSteps[];

#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

/// Lash on the rope between the posts: tied, it stops the cogs.
void GabombaStatue_LashRope(void) {
  struct GabombaActor *actor =
      ((struct GabombaActor *)Actor_Get(ACTOR_FIRST_ROPE_POST_B));
  s32 direction;
  s32 i;

  if (Rope_GetState(0) == 1) {
    direction = gGabombaStatue_CogDirection;
    GabombaStatue_TurnCogs(0);
    Event_SetFlag(FLAG_SCENE_TEMP_03);
    gFieldWork->psynergy = 0;
    i = 0;
    do {
      actor->phase += gGabombaStatue_RopeStopSteps[i] * direction;
      System_Wait(1);
    } while (++i <= 24);
  }
}

/// The cog of sprite id within 24 pixels of origin, or null.
struct GabombaActor *GabombaStatue_FindCogUnder(struct GabombaActor *origin,
                                                s32 id) {
  s32 i;

  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct GabombaActor *object = ((struct GabombaActor *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, origin->x, toFIXED(24) - 1) &&
          DIFF_WITHIN(object->z, origin->z, toFIXED(24) - 1))
        return object;
    }
  }
  return NULL;
}

/// As GabombaStatue_FindCogUnder, but around the point 16 pixels ahead of
/// origin in its facing.
struct GabombaActor *GabombaStatue_FindCogAhead(struct GabombaActor *origin,
                                                s32 id) {
  s32 position[3];
  s32 i;

  position[0] = origin->x;
  position[2] = origin->z;
  FieldVector_AddPolarOffset(16, origin->facing, position);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct GabombaActor *object = ((struct GabombaActor *)Actor_Get(i));

    if (object != NULL && object->kind == 1 && object->animation[10] != NULL &&
        *(s16 *)object->animation[10] == id) {
      if (DIFF_WITHIN(object->x, position[0], toFIXED(24) - 1) &&
          DIFF_WITHIN(object->z, position[2], toFIXED(24) - 1))
        return object;
    }
  }
  return NULL;
}

/// The leader hops off a cog by dx, dz.
void GabombaStatue_HopOffCog(struct Actor *actor, s32 dx, s32 dz,
                             s32 velocity) {
  u8 *flags = &actor->motionFlags;
  u8 saved;

  Import_LeaderStopMoving(actor);
  saved = *flags;
  Actor_SelectAnimation(actor, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SelectAnimation(actor, 7);
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  actor->velocityY = velocity;
  *flags &= ACTOR_JUMP_KEEPS_MOTION;
  Actor_SetSpriteFlags(actor, 0);
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed + dx, actor->y.fixed,
                             actor->z.fixed + dz);
  FieldActor_WaitForMove(actor);
  *flags = saved;
  Actor_SelectAnimation(actor, 6);
  Actor_SetSpriteFlags(actor, 1);
  System_Wait(3);
}

/// Standing on a turning cog (kind 358) turns the leader with it.
void GabombaStatue_TurnWithCog(void) {
  s32 step = gGabombaStatue_CogDirection;

  if (step != 0) {
    s32 turn = step * 1092;
    s32 delta = -turn;
    struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *near = GabombaStatue_FindCogUnder(leader, SPRITE_GREEN_COG);

    if (near != NULL) {
      u16 angle = near->facing;

      if (angle == FACING_WEST)
        delta = turn;
    }
    leader->facing += delta;
  }
}

/// Standing on a turntable (kind 357) carries the leader round with it, unless
/// a wall stops him.
void GabombaStatue_RideTurntable(void) {
  struct Actor *actor, *table;
  struct Vector3 test;
  s32 step = -364;
  s32 turn;
  FIXED x, z, dx, dz;
  s32 back;
  FIXED tx, tz;

  if (gGabombaStatue_CogDirection == 0)
    return;
  actor = Actor_GetById(Party_GetLeaderActor());
  table = GabombaStatue_FindCogUnder(actor, SPRITE_BLUE_COG);
  turn = gGabombaStatue_CogDirection * step;
  if (table != 0) {
    if (table->y.fixed + toFIXED(16) > actor->y.fixed)
      return;
    if (table->facing == FACING_WEST)
      turn = -turn;
    dx = actor->x.fixed - table->x.fixed;
    dz = actor->z.fixed - table->z.fixed;
    back = -turn;
    {
      s32 s = Math_Sin(back);
      s32 c = Trig_Cos(back);
      x = table->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
      z = table->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
    }
    test.x = x + actor->velocityX;
    test.y = table->y.fixed;
    test.z = z + actor->velocityZ;
    if (MapGrid_CheckCollision(actor, &test) == 0) {
      s32 s, c, angle;
      angle = DEGtoANG(30) - turn;
      s = Math_Sin(angle);
      c = Trig_Cos(angle);
      tx = table->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
      tz = table->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
      test.x = tx + actor->velocityX;
      test.z = tz + actor->velocityZ;
      if (MapGrid_CheckCollision(actor, &test) == 0) {
        FIXED s, c;
        s32 angle;
        angle = back - DEGtoANG(30);
        s = Math_Sin(angle);
        c = Trig_Cos(angle);
        tx = table->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
        tz = table->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
        test.x = tx + actor->velocityX;
        test.z = tz + actor->velocityZ;
        if (MapGrid_CheckCollision(actor, &test) == 0) {
          actor->x.fixed = x;
          actor->z.fixed = z;
        }
      }
    }
  }
  actor->facing += turn;
}

/// Moves the controlled character with the platform kind 359 along x by the
/// speed in gGabombaStatue_CogDirection, backward when the platform faces half
/// a turn; it stays within 2 pixels of the platform's z, and past 20 pixels on
/// x it steps off (GabombaStatue_HopOffCog).
void GabombaStatue_RideSlidingCog(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = GabombaStatue_FindCogUnder(actor, SPRITE_RED_COG);
    s32 dx = *speed << 15;

    if (found != NULL) {
      FIXED diff, x, foundX;

      if (found->facing == FACING_WEST)
        dx = -dx;
      if (found->z.fixed - actor->z.fixed < -toFIXED(2))
        actor->z.fixed = found->z.fixed + toFIXED(2);
      if (found->z.fixed - actor->z.fixed > toFIXED(2))
        actor->z.fixed = found->z.fixed - toFIXED(2);
      x = actor->x.fixed + dx;
      actor->x.fixed = x;
      foundX = found->x.fixed;
      diff = foundX - x;
      if (diff >= 0 ? diff >= toFIXED(20) : x - foundX >= toFIXED(20)) {
        s32 sign;
        FIXED offset;

        sign = 0;
        offset = actor->x.fixed - foundX;
        if (offset != 0) {
          sign = 1;
          if (offset <= 0)
            sign = -sign;
        }
        GabombaStatue_HopOffCog(actor, sign * toFIXED(12), 0, toFIXED(2));
      }
    }
  }
}

/// The ride on the cogs ends: object is set down at the room's south and the
/// party walks on.
void GabombaStatue_EndRide(struct Actor *actor) {
  u32 i;
  struct MainRoutineWork *party;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(6);
  actor->z.fixed = 0xf741cf;
  actor->facing = FACING_SOUTH;
  party = &gMainRoutineWork;
  Actor_SetAttachedEffect(party->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimation(party->leaderActorId, 27);
  FieldEvent_Wait(30);
  Actor_SetAnimation(party->leaderActorId, 28);
  FieldEvent_Wait(10);
  for (i = 0; i <= 19; i++) {
    actor->scaleX -= FIXED_ONE * 3 / 256;
    actor->scaleY -= FIXED_ONE * 3 / 256;
    actor->y.fixed -= toFIXED(3) / 2;
    System_Wait(1);
    if (i == 10)
      Event_PlaySound(SE_DUST_PUFF);
    if (i == 20) {
      gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
      Event_FinishScreenTransition();
    }
  }
}

/// Moves the controlled character with the moving platforms
/// (GabombaStatue_FindCogUnder finds the one under it, 360 forward and 361
/// back): it stays within 2 pixels of the platform's x and is carried along z
/// by the speed in gGabombaStatue_CogDirection; past 20 pixels from the
/// platform it steps off (GabombaStatue_HopOffCog). In GabombaStatueTurntable
/// at cell (28, 15) or (30, 15) it ends the ride (GabombaStatue_EndRide) and
/// runs exit 5 or 6 (Import_EventSetExitRequest).
void GabombaStatue_RideRollingCog(void) {
  struct Actor *actor, *platform;
  s32 speed;
  FIXED d;
  s32 sign, x;
  FIXED z, platformZ, offset;

  if (gGabombaStatue_CogDirection == 0)
    return;
  actor = Actor_Get(FieldParty_GetLeaderActor());
  speed = gGabombaStatue_CogDirection << 15;
  platform = GabombaStatue_FindCogUnder(actor, SPRITE_BLUE_PILLAR);
  if (platform != 0) {
    if (platform->x.fixed - actor->x.fixed < -toFIXED(2))
      actor->x.fixed = platform->x.fixed + toFIXED(2);
    if (platform->x.fixed - actor->x.fixed > toFIXED(2))
      actor->x.fixed = platform->x.fixed - toFIXED(2);
    z = actor->z.fixed + speed;
    actor->z.fixed = z;
    platformZ = platform->z.fixed;
    d = platformZ - z;
    if (d >= 0 ? d >= toFIXED(20) : z - platformZ >= toFIXED(20)) {
      sign = 0;
      offset = actor->z.fixed - platformZ;
      if (offset != 0) {
        sign = 1;
        if (offset <= 0)
          sign = -sign;
      }
      GabombaStatue_HopOffCog(actor, 0, (sign * 3) << 18, toFIXED(2));
    }
  }
  platform = GabombaStatue_FindCogUnder(actor, SPRITE_PINK_DRUM);
  if (platform == 0)
    return;
  if (platform->x.fixed - actor->x.fixed < -toFIXED(2))
    actor->x.fixed = platform->x.fixed + toFIXED(2);
  if (platform->x.fixed - actor->x.fixed > toFIXED(2))
    actor->x.fixed = platform->x.fixed - toFIXED(2);
  z = actor->z.fixed - speed;
  actor->z.fixed = z;
  platformZ = platform->z.fixed;
  d = platformZ - z;
  if (d >= 0 ? d >= toFIXED(20) : z - platformZ >= toFIXED(20)) {
    sign = 0;
    offset = actor->z.fixed - platformZ;
    if (offset != 0) {
      sign = 1;
      if (offset <= 0)
        sign = -sign;
    }
    GabombaStatue_HopOffCog(actor, 0, (sign * 3) << 18, toFIXED(2));
  }
  if (gMainRoutineWork.next != SCENE_GABOMBA_STATUE_TURNTABLE)
    return;
  x = FIELD_CELL(actor->x.fixed);
  if (FIELD_CELL(actor->z.fixed) != 15)
    return;
  if (x == 28) {
    GabombaStatue_EndRide(actor);
    FieldEvent_SetExitRequest(5);
  } else if (x == 30) {
    GabombaStatue_EndRide(actor);
    FieldEvent_SetExitRequest(6);
  }
}

/// Walking east or west off a sliding cog hops the leader to the next.
void GabombaStatue_HopAcrossEastOrWest(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = GabombaStatue_FindCogAhead(actor, SPRITE_RED_COG);
    s32 dx = *speed << 20;

    if (found != NULL) {
      u16 facing = found->facing;

      if (facing == FACING_WEST)
        dx = -dx;
      facing = actor->facing;
      if ((facing == 0 && dx > 0) || (facing == FACING_WEST && dx < 0))
        GabombaStatue_HopOffCog(actor, dx, 0, toFIXED(4));
    }
  }
}

void GabombaStatue_HopAcrossNorthOrSouth(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = GabombaStatue_FindCogAhead(actor, SPRITE_BLUE_PILLAR);
    s32 dz = *speed << 20;
    u16 facing;

    if (found == NULL) {
      found = GabombaStatue_FindCogAhead(actor, SPRITE_PINK_DRUM);
      dz = -dz;
      if (found == NULL)
        return;
    }
    facing = actor->facing;
    if ((facing == FACING_SOUTH && dz > 0) ||
        (facing == FACING_NORTH && dz < 0))
      GabombaStatue_HopOffCog(actor, 0, dz, toFIXED(4));
  }
}

/// A little mouse pops up and hops from cog to cog, showing the way across.
void GabombaStatue_MouseShowsTheWay(struct Actor *actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  Actor_SetPosition(ACTOR_MOUSE, 328, 296);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(15);
  FieldActor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_MOUSE);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  EventCamera_FollowActor(ACTOR_MOUSE, 1);
  FieldEvent_Wait(30);
  Actor_Get(ACTOR_MOUSE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetAnimation(ACTOR_MOUSE, 1);
  Actor_WalkToAndWait(ACTOR_MOUSE, 320, 240);
  Actor_WalkToAndWait(ACTOR_MOUSE, 304, 216);
  Actor_WalkToAndWait(ACTOR_MOUSE, 280, 216);
  Actor_WalkToAndWait(ACTOR_MOUSE, 280, 184);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MOUSE, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_VOICE_GIRL_C);
  {
    s32 a = 256, b = 30;

    Actor_ShowEmote(ACTOR_MOUSE, a, b);
  }
  Actor_WalkToAndWait(ACTOR_MOUSE, 344, 184);
  Actor_WalkToAndWait(ACTOR_MOUSE, 344, 200);
  FieldEvent_Wait(2);
  Actor_FaceDirection(ACTOR_MOUSE, FACING_WEST, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(4);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 312, 200);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 312, 168);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 328, 152);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 360, 152);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 360, 184);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 376, 184);
  Actor_WalkToAndWait(ACTOR_MOUSE, 392, 200);
  Actor_WalkToAndWait(ACTOR_MOUSE, 408, 184);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 440, 184);
  Actor_SetPosition(ACTOR_MOUSE, 0, 0);
  Event_SetFlag(FLAG_GABOMBA_MOUSE_SEEN);
  Event_End();
}

#include "text/msg_ids.h"

void GabombaStatue_SendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle);
void ObjectTable_Snapshot(void);

/// Takes charge id off the map, stopped and standing.
void GabombaStatue_ResetCharge(s32 id) {
  Actor_SetPosition(id, 0, 0);
  FieldActor_Stop(id);
  Actor_SetAnimation(id, 1);
}

/// Puts the four charges at their starting cells, ready to run along the
/// channel.
void GabombaStatue_ReleaseCharges(void) {
  GabombaStatue_ResetCharge(ACTOR_CHARGE_A);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_B);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_C);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_D);
  GabombaStatue_SendCharge(ACTOR_CHARGE_A, 11, ACTOR_BLUE_ORB_STATUE, 0);
  GabombaStatue_SendCharge(ACTOR_CHARGE_B, 11, ACTOR_BLUE_ORB_STATUE,
                           DEGtoANG(180));
  GabombaStatue_SendCharge(ACTOR_CHARGE_C, 10, ACTOR_RED_ORB_STATUE,
                           DEGtoANG(90));
  GabombaStatue_SendCharge(ACTOR_CHARGE_D, 10, ACTOR_RED_ORB_STATUE,
                           TRIG_TURN * 3 / 4);
}

/// From inside the statue the party hears the offering made below: "O Great
/// Gabomba! Please accept this, our humble gift!"
void GabombaStatue_HearTheOffering(void) {
  s32 result;

  Event_SetFlag(FLAG_SCENE_TEMP_05);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  GabombaStatue_StopClock();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaAcceptOffering);
  // "O Great Gabomba! Please accept this, our humble gift!"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 256, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_SHIMMER);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  GabombaStatue_ReleaseCharges();
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(SE_PSYNERGY_START);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE), 7);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), 7);
  System_Wait(8);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), 3);
  FieldActor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  Event_PlaySound(MUS_KIBOMBO);
  result = GabombaStatue_RunCharges(ACTOR_CHARGE_A, 1);
  ObjectTable_Snapshot();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case 3:
    FieldEvent_SetExitRequest(14);
    break;
  case 1:
    FieldEvent_SetExitRequest(15);
    break;
  case 2:
    FieldEvent_SetExitRequest(16);
    break;
  case 0:
    FieldEvent_SetExitRequest(17);
    break;
  }
}

extern struct Trigger gGabombaStatue_Triggers[];
extern struct Trigger gGabombaStatue_BoneArmletChestTriggers[];
extern struct Trigger gGabombaStatue_ElixirChestTriggers[];
extern struct Trigger gGabombaStatue_SteelTriggers[];
extern struct Trigger gGabombaStatue_TurntableTriggers[];
extern struct Trigger gGabombaStatue_CogsTriggers[];
extern struct Trigger gGabombaStatue_ArrowsTriggers[];
extern struct Trigger gGabombaStatue_PinionTriggers[];

const u8 *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;
  const u8 *table = gGabombaStatue_Triggers;

  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    table = gGabombaStatue_BoneArmletChestTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    table = gGabombaStatue_ElixirChestTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_STEEL)
    table = gGabombaStatue_SteelTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_TURNTABLE)
    table = gGabombaStatue_TurntableTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_COGS)
    table = gGabombaStatue_CogsTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_ARROWS)
    table = gGabombaStatue_ArrowsTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_PINION)
    table = gGabombaStatue_PinionTriggers;
  return table;
}

/// The leader walks to the hole at x, z, drops in and the scene leaves by its
/// exit.
void GabombaStatue_DropIntoHole(s32 x, s32 z) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, x, z);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 13);
  FieldActor_SetMoveTarget(actor, x, -12, z);
  FieldActor_WaitForMove(actor);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 10);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(name, n1, n2, n3, n4, n5, n6, n7) \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);          \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & KEY_RIGHT) == 0)                           \
        return;                                                                \
      if (x == n3 && (gInput.held & KEY_LEFT) == 0)                            \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & KEY_UP) == 0) {            \
      return;                                                                  \
    }                                                                          \
    GabombaStatue_DropIntoHole(n6, n7);                                        \
  }

DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(GabombaStatue_StepIntoBoneArmletChestHole,
                                     11, 18, 20, 19, 12, 312, 180)

#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(name, n1, n2, n3, n4, n5,    \
                                                  n6, n7, n8)                  \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);          \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (x == n1) {                                                             \
      if (z == n2 && (gInput.held & KEY_DOWN) == 0)                            \
        return;                                                                \
      if (z == n3 && (gInput.held & KEY_UP) == 0)                              \
        return;                                                                \
    } else if (z == n4) {                                                      \
      if (x == n5 && (gInput.held & KEY_RIGHT) == 0)                           \
        return;                                                                \
      if (x == n6 && (gInput.held & KEY_LEFT) == 0)                            \
        return;                                                                \
    }                                                                          \
    GabombaStatue_DropIntoHole(n7, n8);                                        \
  }

DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(GabombaStatue_StepIntoElixirChestHole,
                                          29, 11, 13, 12, 28, 30, 472, 196)

void GabombaStatue_StepIntoFirstSteelHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (z == 7) {
    if (x == 32 && (gInput.held & KEY_RIGHT) == 0)
      return;
  } else if (x == 33 && z == 8 && (gInput.held & KEY_UP) == 0) {
    return;
  }
  GabombaStatue_DropIntoHole(536, 112);
}

void GabombaStatue_StepIntoSecondSteelHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (z == 13) {
    if (x == 5 && (gInput.held & KEY_RIGHT) == 0)
      return;
    if (x == 7 && (gInput.held & KEY_LEFT) == 0)
      return;
  } else if (x == 6) {
    if (z == 12 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 14 && (gInput.held & KEY_UP) == 0)
      return;
  }
  GabombaStatue_DropIntoHole(104, 208);
}

void GabombaStatue_StepIntoThirdSteelHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42) {
    if (z == 12 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 14 && (gInput.held & KEY_UP) == 0)
      return;
  } else if (x == 43 && z == 13 && (gInput.held & KEY_RIGHT) == 0) {
    return;
  }
  GabombaStatue_DropIntoHole(680, 208);
}

DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(GabombaStatue_StepIntoFirstTurntableHole,
                                     7, 23, 25, 24, 8, 392, 112)

DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(GabombaStatue_StepIntoSecondTurntableHole,
                                     9, 5, 7, 6, 10, 104, 146)

void GabombaStatue_StepIntoThirdTurntableHole(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42 && z == 10 && (gInput.held & KEY_UP) == 0)
    return;
  if (x == 43 && z == 9 && (gInput.held & KEY_LEFT) == 0)
    return;
  GabombaStatue_DropIntoHole(680, 146);
}

DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(GabombaStatue_StepIntoCogsHole, 27,
                                          17, 19, 18, 26, 28, 440, 288)

/// Jumps down the hole.
void GabombaStatue_JumpDownHole(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  struct Actor *leader;
  struct FieldWork *work;

  if (x == 28 && z == 9 && !(gInput.held & KEY_RIGHT))
    return;
  if (x == 29 && z == 10 && !(gInput.held & KEY_UP))
    return;
  work = Ram_HeapSlots->fieldWork;
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 472, 144);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 13);
  FieldActor_SetMoveTarget(leader, 472, 0, 156);
  FieldActor_WaitForMove(leader);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 10);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->stepTrigger);
}

#define DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(name, n1, n2)                     \
  void name(struct Actor *actor) {                                             \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    Event_PlaySound(SE_GABOMBA_STATUE_HOLE);                                   \
    Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 1.6, 1.6);   \
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, n1, n2);               \
    Event_End();                                                               \
  }

DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionCarriesLeader, 152,
                                     424)

/// A turning pinion carries the leader along and throws him down.
void GabombaStatue_PinionThrowsLeader(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GABOMBA_STATUE_HOLE);
  actor->motionFlags = 0;
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 1.6, 1.6);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 152, 488);
  actor->motionFlags = 3;
  FieldEvent_Wait(8);
  Event_PlaySound(SE_RATTLE);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 22);
  FieldEvent_Wait(20);
  Event_End();
}

DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionCarriesLeaderNorth,
                                     376, 200)

DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionCarriesLeaderSouth,
                                     376, 136)

DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionCarriesLeaderEast, 440,
                                     248)

DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionCarriesLeaderWest, 440,
                                     184)

void GabombaStatue_SetActorsHidden(s32 show) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    Actors_SetHidden(show, ACTOR_BONE_ARMLET_HOLE_SHADOW, 1);
  else if (gMainRoutineWork.next == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    Actors_SetHidden(show, ACTOR_ELIXIR_HOLE_SHADOW_A, 2);
}

extern s16 gGabombaStatue_ConePillars[];

void GabombaStatue_HideActors(void) {
  GabombaStatue_SetActorsHidden(1);
  Event_SetFlag(FLAG_SCENE_TEMP_00);
}

void GabombaStatue_ShowActors(void) {
  GabombaStatue_SetActorsHidden(0);
  Event_ClearFlag(FLAG_SCENE_TEMP_00);
}

void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_Get(first + i);

    if (show)
      actor->priorityFlags &= ~ACTOR_PRIORITY_NOT_DRAWN;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}

/// Pound on a cone pillar; with both Pounded, the cogs turn back.
void GabombaStatue_PoundConePillar(s32 a, s32 b) {
  Pound_SinkTarget(gGabombaStatue_ConePillars, a, b);
  if (Event_GetFlag(FLAG_GABOMBA_FIRST_CONE_POUNDED) &&
      Event_GetFlag(FLAG_GABOMBA_SECOND_CONE_POUNDED))
    gFieldWork->raisedTrigger = RAISED_COGS_TURN_BACK;
}

/// With both cone pillars Pounded, the flashing cog stops, and all the cogs
/// start turning the other way.
void GabombaStatue_CogsTurnBack(void) {
  struct Actor *actor = Actor_Get(ACTOR_FLASHING_COG);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(360, 0, 496, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_GABOMBA_STATUE_COGS);
  GabombaStatue_TurnCogs(0);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_582);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(8);
  FieldEvent_Wait(10);
  actor->y.fixed = 0;
  FieldEvent_Wait(10);
  GabombaStatue_TurnCogs(-1);
  Event_SetFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
  FieldEvent_Wait(20);
  Event_End();
}

void Actor_SetAnimationSpeed(struct Actor *actor, s32 value);
/// Pound on the grey cone of the channel room raises all the floating tiles.
void GabombaStatue_PoundGreyCone(s32 value) {
  GabombaStatue_RaiseTiles(value);
  if (value == 1)
    gFieldWork->raisedTrigger = RAISED_CONE_CHIMES;
}

/// The grey cone rings as the tiles rise.
void GabombaStatue_ConeChimes(void) {
  struct Actor *actor = Actor_Get(ACTOR_GREY_CONE);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_RISING_CHIMES);
  Actor_SelectAnimation(actor, 5);
  actor->scaleY = FIXED_ONE;
  Event_End();
}

/// When the clock has run out, the offering below is heard again.
void GabombaStatue_CheckOfferingClock(void) {
  if (GabombaStatue_GetClock() == 0)
    gFieldWork->raisedTrigger = RAISED_OFFERING_HEARD;
}

/// Dims actor id and fixes it in place behind the others.
void GabombaStatue_DimActor(s32 id) {
  Actor_DisableById(id);
  FieldActor_SetSpriteColor(Actor_Get(id), 15);
  FieldActor_SetSpritePriority(id, 1);
  Actor_Get(id)->priorityFlags |= 2;
}

/// Shows the hidden actors again when the party comes back to the same spot,
/// unless they were hidden before it left.
void GabombaStatue_RestoreHiddenActors(void) {
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
      GabombaStatue_ShowActors();
  } else {
    Event_SetFlag(FLAG_SCENE_TEMP_00);
  }
}
#define DEFINE_GABOMBA_STATUE_RAISE_ACTORS(name, n1, n2)                       \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < n2; i++) {                                                 \
      s32 id = i + n1;                                                         \
      struct Actor *actor = Actor_Get(id);                                     \
                                                                               \
      GabombaStatue_DimActor(id);                                              \
      actor->motionFlags = 0;                                                  \
      actor->y.fixed = toFIXED(36);                                            \
    }                                                                          \
    GabombaStatue_RestoreHiddenActors();                                       \
  }

DEFINE_GABOMBA_STATUE_RAISE_ACTORS(GabombaStatue_DimBoneArmletChestActors,
                                   ACTOR_BONE_ARMLET_HOLE_SHADOW, 1)

DEFINE_GABOMBA_STATUE_RAISE_ACTORS(GabombaStatue_DimElixirChestActors,
                                   ACTOR_ELIXIR_HOLE_SHADOW_A, 2)
/// While a menu or Psynergy is up the cogs and wanderers stand still.
void GabombaStatue_PauseCogsInMenus(void) {
  s32 value = 16;
  s32 open = 0;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 i;

  if (Event_GetFlag(FLAG_MENU_OPEN) || event->psynergy != 0) {
    open = 1;
    value = 0;
  }
  i = ACTOR_FIRST_PLACED;
  do {
    {
      struct Actor *actor = Actor_Get(i);

      if (actor != NULL && actor->spriteStorageKind == 1) {
        struct SpriteAnimation *part =
            ((struct Sprite *)actor->sprite)->entries[0];

        if (part != NULL) {
          switch (part->animationId) {
          case SPRITE_BLUE_COG:
          case SPRITE_GREEN_COG:
          case SPRITE_RED_COG:
          case SPRITE_BLUE_PILLAR:
          case SPRITE_PINK_DRUM:
          case 362:
            Actor_SetAnimationSpeed(actor, value);
            break;
          case SPRITE_TIGHTROPE_POST:
            actor->paused = open;
            break;
          }
        }
      }
    }
    i++;
  } while (i < FIELD_TREASURE_ACTOR);
}

#define MARK(at, cellPicture, shape)                                           \
  (cell = (at), layerA[cell] = (cellPicture), layerB[cell] = (cellPicture),    \
   word = &shapesA[cell], *word |= (shape), word = &shapesB[cell],             \
   *word |= (shape))

void GabombaStatue_DrawCogCells(void) {
  u32 *dest = (u32 *)gMapGridShapes + 116;
  u32 *src = gGabombaStatue_CogCellPictures;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *layerA = map->layers[0].terrainIds;
  u8 *layerB = map->layers[1].terrainIds;
  u32 *shapesA = (u32 *)map->layers[0].cells;
  u32 *shapesB = (u32 *)map->layers[1].cells;
  struct Actor *actor;
  s16 *frames;
  u32 *word;
  u32 i;
  s32 x, z, cell, found;

  i = 0;
  do {
    *dest++ = *src++;
  } while (++i <= 11);
  for (i = ACTOR_FIRST_PLACED; (s32)i < FIELD_TREASURE_ACTOR; i++) {
    actor = Actor_GetById(i);
    if (actor == 0)
      continue;
    if (actor->spriteStorageKind != 1)
      continue;
    frames = (s16 *)((struct Sprite *)actor->sprite)->entries[0];
    if (frames == 0)
      continue;
    found = 0;
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
    switch (*frames) {
    case SPRITE_GREEN_COG:
      cell = x + (z << 7);
      layerA[cell] = 116;
      layerB[cell] = 117;
      word = &shapesA[cell];
      *word |= 0x80500000;
      word = &shapesB[cell];
      *word |= 0x80500000;
      found = 1;
      break;
    case SPRITE_BLUE_COG:
      MARK(x + (z << 7) - 129, 118, 0x510000);
      MARK(x + (z << 7) - 128, 117, 0x80510000);
      MARK(x + (z << 7) - 127, 119, 0x510000);
      MARK(x + (z << 7) - 1, 117, 0x80510000);
      MARK(x + (z << 7), 117, 0x510000);
      MARK(x + (z << 7) + 1, 117, 0x80510000);
      MARK(x + (z << 7) + 127, 120, 0x510000);
      MARK(x + (z << 7) + 128, 117, 0x80510000);
      MARK(x + (z << 7) + 129, 121, 0x510000);
      found = 1;
      break;
    case SPRITE_RED_COG:
      MARK(x + (z << 7) - 2, 127, 0x540000);
      MARK(x + (z << 7) - 1, 122, 0x529000);
      MARK(x + (z << 7), 117, 0x80526000);
      MARK(x + (z << 7) + 1, 123, 0x529000);
      MARK(x + (z << 7) + 2, 127, 0x540000);
      found = 1;
      break;
    case SPRITE_PINK_DRUM:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 126, 0x80550000);
      found = 1;
      break;
    case SPRITE_BLUE_PILLAR:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 127, 0x540000);
      found = 1;
      break;
    case 362:
      found = 1;
      break;
    }
    if (found) {
      actor->collisionFlags = 0;
      actor->motionFlags = 0;
      actor->priorityFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
    }
  }
  Task_AddFrame(GabombaStatue_PauseCogsInMenus, TASK_PASS_FRAME);
}

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)

/// Makes the scene more translucent the farther inside the object is.
void GabombaStatue_FadeByDepth(void) {
  s32 blend;
  struct Actor *actor = gFieldWork->camera;
  s32 level = actor->z.fixed;

  level = (level - toFIXED(232)) >> 19;
  CLAMP(level, 0, 16);
  blend = ((16 - level) << 8) + level;
  REG_BLDALPHA->value = blend;
}

/// Turns cog id by direction, its animation timers set to value.
void GabombaStatue_SetCogTurn(s32 id, s32 direction, s32 value) {
  struct Actor *actor = Actor_Get(id);
  void *animation = actor->sprite;

  Actor_SelectAnimation(actor,
                        gGabombaStatue_CogAnimations[(direction + 1) & 3]);
  actor->retainSpriteCache = 2;
  Sprite_SetAnimationTimers(animation, value);
}

/// Sets the cogs' direction and turns every cog of the room that way.
void GabombaStatue_TurnCogs(s32 direction) {
  s16 scene;

  gGabombaStatue_CogDirection = direction;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
    GabombaStatue_SetCogTurn(ACTOR_ELIXIR_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_ELIXIR_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_FLASHING_COG, direction, 2);
    if ((u16)(gMainRoutineWork.entrance - 3) > 1)
      GabombaStatue_SetCogTurn(ACTOR_ELIXIR_PINION_A, direction, 3);
    if (direction == 0)
      MapGrid_CopyCellAttributes(23, 21, 1, 3, 23, 9);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(24, 21, 1, 3, 23, 9);
  } else if (scene == SCENE_GABOMBA_STATUE_STEEL) {
    GabombaStatue_SetCogTurn(ACTOR_STEEL_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_RED_COGS, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_BLUE_PILLARS, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_PINK_DRUMS, direction, 4);
  } else if (scene == SCENE_GABOMBA_STATUE_TURNTABLE) {
    GabombaStatue_SetCogTurn(ACTOR_TURNTABLE_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_TURNTABLE_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_TURNTABLE_RED_COGS, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_TURNTABLE_BLUE_PILLAR_B, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_TURNTABLE_PINK_DRUM_B, direction, 4);
  } else if (scene == SCENE_GABOMBA_STATUE_COGS) {
    GabombaStatue_SetCogTurn(ACTOR_COGS_RED_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_COGS_PINION_A, direction, 1);
    if (direction == 0)
      MapGrid_CopyCellAttributes(34, 12, 1, 3, 27, 12);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(35, 12, 1, 3, 27, 12);
  } else if (scene == SCENE_GABOMBA_STATUE_ARROWS) {
    Actor_SetAnimation(ACTOR_CHARGE_C, 5);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_CHARGE_C), 0);
  } else if (scene == SCENE_GABOMBA_STATUE_PINION) {
    GabombaStatue_SetCogTurn(ACTOR_PINION_A, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_PINION_B, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_PINION_C, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_PINION_D, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_PINION_E, direction, 4);
    if (direction == 0)
      MapGrid_CopyCellAttributes(0, 0, 1, 3, 9, 27);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(1, 0, 1, 3, 9, 27);
  }
}

extern s32 gGabombaStatue_ChargeScript[];

void Actor_SetSpriteColorMode(struct GabombaActor *object, s32 value);

/// Starts charge id from the orb statue statueId at the angle given, drawn in
/// sprite colour mode colorMode.
void GabombaStatue_SendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle) {
  struct GabombaActor *object = ((struct GabombaActor *)Actor_Get(id));
  struct GabombaActor *target = ((struct GabombaActor *)Actor_Get(statueId));

  Actor_SelectAnimation(object, 0);
  Actor_StartScript(object, gGabombaStatue_ChargeScript);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->link = target;
  object->phase = angle / 256;
  object->slot = id;
  Actor_SetSpriteColorMode(object, colorMode);
}
#undef DIFF_WITHIN
#undef MARK
#undef REG_BLDALPHA

#include "../field/field_effect_advance_motion.c"

#undef DIFF_WITHIN
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR
#undef DEFINE_GABOMBA_STATUE_WALK_LEADER_TO
#undef DEFINE_GABOMBA_STATUE_RAISE_ACTORS
#undef MARK
#undef REG_BLDALPHA
#undef OBJECT_BYTE
/// The leader falls in from the cog above and lands with a thump beside Steel.
void GabombaStatue_LandFromFall(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y.fixed = toFIXED(130);
  actor->gravity = toFIXED(1) / 2;
  actor->bounce = 0;
  actor->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  i = 0;
  do {
    s32 angle;
    FIXED velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random_Below(toFIXED(3) / 4)) - toFIXED(3) / 8;
    velocity[2] += (Random_Below(toFIXED(3) / 8)) - toFIXED(3) / 16;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  System_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_End();
}

#include "text/msg_ids.h"

extern u32 gGabombaStatue_LeaderFocusScript[];
void Actor_MoveToObject(s32 id, s32 target);
void GabombaStatue_ReleaseCharges(void);

/// Actor_ShowEmoteById through copies of its arguments.
static __inline__ void GabombaStatue_ShowEmote(s32 id, s32 a, s32 b) {
  s32 a_ = a;
  s32 b_ = b;

  Actor_ShowEmoteById(id, a_, b_);
}

/// Kraden works out that the channel must bring the two charges to the statue
/// at once.
void GabombaStatue_FocusEnergies(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  EventCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaFocusEnergies);
  // "Focus your energies!"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_KradenFocusScript);
  System_Wait(10);
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gGabombaStatue_LeaderFocusScript);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_JennaFocusScript);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_PiersFocusScript);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(434, CAMERA_KEEP, 286, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_RunScriptAndWait(ACTOR_KRADEN, gGabombaStatue_KradenPointsScript);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "Everyone! Look over here!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(434, CAMERA_KEEP, 168, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
  Actor_Get(ACTOR_PIERS)->facing = FACING_SOUTH;
  Actor_Get(gMainRoutineWork.leaderActorId)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_SHEBA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_JENNA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PIERS, 400, 272);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 420, 272);
  Actor_WalkTo(ACTOR_SHEBA, 444, 272);
  Actor_WalkTo(ACTOR_JENNA, 468, 272);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(80);
  System_Wait(80);
  FieldEvent_SetExitRequest(11);
}

/// The charges flow along the channel and stick, while the party watches.
void GabombaStatue_WatchTheChargesStick(void) {
  s32 speed;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  speed = FACING_SOUTH;
  Actor_SetPositionAndFacing(gMainRoutineWork.leaderActorId, toFIXED(348),
                             toFIXED(312), speed);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312), speed);
  {
    Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
                               FACING_SOUTH_SOUTHEAST);
  }
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312), speed);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312), speed);
  System_Wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Event_PlaySound(SE_SHIMMER);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  GabombaStatue_ReleaseCharges();
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  GabombaStatue_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  GabombaStatue_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  GabombaStatue_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 60);
  Event_PlaySound(SE_PSYNERGY_START);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE), 7);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), 7);
  System_Wait(8);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), 3);
  FieldActor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  Actor_Get(gMainRoutineWork.leaderActorId)->lookTimer =
      (Random_Below(40)) + 10;
  Actor_Get(ACTOR_PIERS)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_JENNA)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_SHEBA)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_KRADEN)->lookTimer = (Random_Below(40)) + 10;
  Actor_SetScript(gMainRoutineWork.leaderActorId,
                  gGabombaStatue_LookNorthAndEastScript);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_LookNorthAndEastScript);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_LookNorthAndWestScript);
  Actor_SetScript(ACTOR_SHEBA, gGabombaStatue_LookNorthAndWestScript);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_LookNorthAndWestScript);
  Event_PlaySound(MUS_KIBOMBO);
  GabombaStatue_RunCharges(ACTOR_CHARGE_A, 0);
  FieldEvent_Wait(80);
  Event_SetFlag(FLAG_GABOMBA_MUSIC_SILENCED);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(12);
}

extern s16 gGabombaStatue_ConePillars[];
extern u32 gGabombaStatue_PathLengths[];

/// The party watches how the statue's works run.
void GabombaStatue_SeeHowItWorks(void) {
  s32 north;
  s32 cue = 0x6007;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  {
    s32 leader = gMainRoutineWork.leaderActorId;

    north = FACING_NORTH;
    Actor_SetPositionAndFacing(leader, toFIXED(348), toFIXED(312), north);
  }
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312), north);
  {
    Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
                               FACING_NORTH_NORTHWEST);
  }
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312), north);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312), north);
  System_Wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  GabombaStatue_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldEvent_SetMessageId(MsgGabombaUnderstandsChannel);
  // "Ah! So that's how it works!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 292, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "He's been failing because the conduit that channels his Psynergy is..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  // "Should we reconnect the conduit so he can complete the ceremony?"
  ((void *)Event_OpenMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0));
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            20);
    Actor_SetSpeed(ACTOR_PIERS, 2);
    Actor_WalkToAndWait(ACTOR_PIERS, 340, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 354, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    // "I won't help them, Felix! They're the ones who stole my orb from the..."
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    (gFieldWork->message)++;
  } else {
    (gFieldWork->message)++;
    Actor_SetSpeed(ACTOR_PIERS, 2);
    Actor_WalkToAndWait(ACTOR_PIERS, 340, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 354, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 364, 308);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    // "I agree, Felix! Why should we help this pack of thieves?"
    Event_ShowMessage(cue, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Piers, I understand that you're angry at them, but it really is in..."
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Think about it, Piers... Akafubu stole the orb solely to perform..."
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  GabombaStatue_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "So, what do you think he intends to do with the orb?"
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  // "He means to give it to the Great Gabomba..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Oh! So...if his ceremony succeeds, the orb will be taken into the..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "And we're INSIDE the statue! If the ceremony succeeds, we can go..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 10);
  // "I...don't want to help that Akafubu, but..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 440, 296);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "I don't like the idea of helping him any more than you do, but if we..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 80);
  GabombaStatue_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  // "You're right, Sheba. Let's see if we can't help him finish his..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  // "It looks like the energy charge is supposed to run over these blocks."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  GabombaStatue_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  // "If we can just move these around a little, that might be enough to..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForArrival();
  // "Ah! There we go!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "These panels have arrows on them... What do you think it all means?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Look at the directions they point... I'll bet these will rotate the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "But how do we set the blocks back in place?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  // "Just Pound them in!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 292, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "Won't that do the trick?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  // "We must watch the path his Psynergy takes and move the blocks..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_LookAtChannelScript);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_LookAtChannelScript);
  Actor_SetScript(ACTOR_SHEBA, gGabombaStatue_LookAtChannelScript);
  Actor_RunScriptAndWait(ACTOR_JENNA, gGabombaStatue_LookAtChannelScript);
  Event_SetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(13);
}

/// The party watches a charge run along the channel and splash.
void GabombaStatue_SawEnergyFlow(void) {
  s32 index = GabombaStatue_GetPuzzleSeed();
  FIXED x;
  FIXED z;
  struct Actor *spark;
  struct Actor *actor;
  s32 south;
  s32 facing;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  GabombaStatue_SetUpChannelPuzzle(19, 11, gGabombaStatue_PathLengths[index],
                                   ACTOR_BLOCK_A);
  Actor_SetAnimation(ACTOR_GREY_CONE, 5);
  Actor_SetPosition(ACTOR_CHARGE_A, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_B, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_C, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_D, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_A, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_B, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_C, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_D, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_E, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_F, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_G, 0, 0);
  System_Wait(1);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 182, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  south = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(472), toFIXED(216), south);
  facing = FACING_SOUTH_SOUTHEAST;
  Actor_SetPositionAndFacing(ACTOR_AKAFUBU, toFIXED(472), toFIXED(200), facing);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 312, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_JennaWatchesScript);
  Actor_RunScriptAndWait(ACTOR_AKAFUBU, gGabombaStatue_ShebaWatchesScript);
  FieldEvent_SetMessageId(MsgGabombaSawEnergyFlow);
  // "Yeah, we know... We saw your magic energy flowing past us."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 440, 296);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  // "See how this channel runs along the floor?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldCamera_MoveTo(414, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  // "Here, look... It runs all the way over there!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  GabombaStatue_FindFreeTileCell(&x, &z);
  x = (x << 20) + toFIXED(312);
  z = (z << 20) + toFIXED(184);
  spark = (FieldActor_Create(434, x, 0, z));
  spark->motionFlags |= 4;
  Actor_SetSpriteFlags(spark, 0);
  Actor_SelectAnimation(spark, 1);
  System_Wait(1);
  actor = Actor_Get(ACTOR_JENNA);
  actor->facing = Trig_Atan(spark->z.fixed - actor->z.fixed,
                            spark->x.fixed - actor->x.fixed);
  FieldCamera_MoveToFX(spark->x.fixed, -1, spark->z.fixed, 1);
  FieldCamera_WaitForArrival();
  // "But the thing is, the circuit was broken right there."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Release(spark);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "So we completed the circuit so that your magic energy could go where..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "But...if you truly did this, then why? Why did you help me?"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  Actor_SetPositionAndFacing(gMainRoutineWork.leaderActorId, toFIXED(472),
                             toFIXED(204), south);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(440), toFIXED(204), south);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(456), toFIXED(204), facing);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(488), toFIXED(204), south);
  System_Wait(1);
  // "We were sort of hoping that we could take the orb if your ceremony..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldActor_Jump(ACTOR_AKAFUBU, 4, 10);
  // "Ah, so you have taken the orb? Then begone! You have no further..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "But...we didn't get the orb."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "It rolled through this groove and right through this hole."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "We saw a light through there, so we suspect there must be another..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                    0);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  System_Wait(60);
  FieldEvent_SetExitRequest(18);
}

/// Kraden remarks that fixing the channel should be enough.
void GabombaStatue_FixTheChannel(void) {
  GabombaStatue_StopClock();
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KRADEN), 15);
  System_Wait(10);
  FieldEvent_SetMessageId(MsgGabombaChannelTiming);
  // "It looks like just fixing the channel should do the trick... Maybe..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  System_Wait(1);
  Event_SetFlag(FLAG_GABOMBA_CHANNEL_FIXED);
  GabombaStatue_RestartClock();
}

/// Prepares the current room.
s32 Scene_Initialize(void) {
  s16 scene;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (gMainRoutineWork.next != SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_03))
      GabombaStatue_TurnCogs(0);
    else if (Event_GetFlag(FLAG_GABOMBA_COGS_TURNED_BACK))
      GabombaStatue_TurnCogs(-1);
    else
      GabombaStatue_TurnCogs(1);
    GabombaStatue_DrawCogCells();
  }
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
    GabombaStatue_InitializeBoneArmletChest();
  } else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
    MapGrid_CopyCellAttributes(15, 68, 8, 3, 15, 4);
    GabombaStatue_InitializeElixirChest();
  } else if (scene == SCENE_GABOMBA_STATUE_STEEL) {
    GabombaStatue_InitializeSteel();
  } else if (scene == SCENE_GABOMBA_STATUE_COGS) {
    GabombaStatue_InitializeCogs();
  } else if (scene == SCENE_GABOMBA_STATUE_ARROWS) {
    GabombaStatue_InitializeArrows();
  } else if (scene == SCENE_GABOMBA_STATUE_PINION) {
    GabombaStatue_InitializePinion();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_GABOMBA_STATUE_PINION &&
      gMainRoutineWork.entrance == 1 &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_ClearFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
    Event_ClearFlag(FLAG_GABOMBA_FIRST_CONE_POUNDED);
    Event_ClearFlag(FLAG_GABOMBA_SECOND_CONE_POUNDED);
  }
  return 0;
}

/// Sets up the Bone Armlet chest's room.
void GabombaStatue_InitializeBoneArmletChest(void) {
  GabombaStatue_DimBoneArmletChestActors();
}

/// Places rope index between two posts, tied when its flag is set, else coiled
/// (Rope_Place): the event inline over the scene's import, as event_inline.h
/// writes them.
static inline void FieldRope_Place(s32 index, s32 flag, s32 firstPost,
                                   s32 secondPost) {
  GabombaStatue_PlaceRope(index, flag, firstPost, secondPost);
}

void GabombaStatue_InitializeElixirChest(void) {
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
    Actor_SetScript(ACTOR_ELIXIR_PINION_A, gGabombaStatue_PinionReleaseScript);
    Actor_SetScript(ACTOR_ELIXIR_PINION_B, gGabombaStatue_PinionReleaseScript);
    MapGrid_CopyCellAttributes(33, 24, 3, 3, 27, 29);
  }
  GabombaStatue_PlaceSwitchActors(gGabombaStatue_ConePillars);
  if (!Event_GetFlag(FLAG_GABOMBA_FIRST_CONE_POUNDED) ||
      !Event_GetFlag(FLAG_GABOMBA_SECOND_CONE_POUNDED))
    Actor_Get(ACTOR_FLASHING_COG)->y.fixed = toFIXED(16);
  Rope_Initialize();
  FieldRope_Place(0, FLAG_SCENE_TEMP_01, ACTOR_FIRST_ROPE_POST_A,
                  ACTOR_FIRST_ROPE_POST_B);
  FieldRope_Place(1, FLAG_SCENE_TEMP_02, ACTOR_SECOND_ROPE_POST_A,
                  ACTOR_SECOND_ROPE_POST_B);
  GabombaStatue_DimElixirChestActors();
}

void GabombaStatue_InitializeSteel(void) {
  Rope_Initialize();
  GabombaStatue_PlaceRope(0, FLAG_SCENE_TEMP_01, ACTOR_STEEL_ROPE_POST_A,
                          ACTOR_STEEL_ROPE_POST_B);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if ((u16)(entrance - 7) <= 1)
      GabombaStatue_LandFromFall();
    if (gMainRoutineWork.entrance == 7)
      Event_SetFlag(FLAG_SCENE_TEMP_04);
  }
}

void GabombaStatue_InitializeCogs(void) {
  Rope_Initialize();
  ((s32 (*)(s32, s32, s32))GabombaStatue_CoilRope)(0, ACTOR_COGS_ROPE_POST_A,
                                                   ACTOR_COGS_ROPE_POST_B);
}

/// The channel's random seed, from the leader's stats.
s32 GabombaStatue_GetPuzzleSeed(void) {
  u8 *state = Unit_Get(gMainRoutineWork.leaderActorId);
  s32 sum = 0;
  u32 i;

  for (i = 0; i < 14; i++)
    sum += state[i];
  return sum & 15;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void GabombaStatue_CheckOfferingClock();

/// Sets up the arrows room: its orb statues, its charges and the channel.
void GabombaStatue_InitializeArrows(void) {
  struct Actor *leader;
  s32 seed;

  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), 3);
  Task_AddFrame((s32)GabombaStatue_FadeByDepth, TASK_PASS_FRAME);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_A), 11);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_B), 11);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_C), 10);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_D), 10);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_B), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_C), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_D), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_E), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_F), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_G), 0);
  if (Event_GetFlag(FLAG_GABOMBA_RESPONDED) &&
      !Event_GetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatue_SawEnergyFlow();
    return;
  }
  if (Event_GetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(ACTOR_CHARGE_A, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_B, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_C, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_D, 0, 0);
    GabombaStatue_ResetClock();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveToFX(leader->x.fixed, -1, leader->z.fixed, 0);
    MapGrid_CenterOnFollowedPoint();
    System_Wait(1);
    Event_End();
  }
  if (Event_GetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS) &&
      !Event_GetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatue_StartClock(0);
    Task_AddFrame((s32)GabombaStatue_CheckOfferingClock, TASK_PASS_FRAME);
  }
  seed = GabombaStatue_GetPuzzleSeed();
  GabombaStatue_SetUpChannelPuzzle(19, 11, gGabombaStatue_PathLengths[seed],
                                   ACTOR_BLOCK_A);
  Actor_SetAnimation(ACTOR_GREY_CONE, 5);
  if (gMainRoutineWork.entrance == 2) {
    GabombaStatue_WatchTheChargesStick();
    return;
  }
  if (!Event_GetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS)) {
    if (gMainRoutineWork.entrance == 1)
      GabombaStatue_FocusEnergies();
    else if (gMainRoutineWork.entrance == 3)
      GabombaStatue_SeeHowItWorks();
  }
  if (Event_GetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_ClearFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY);
    if (Event_GetFlag(0x9eb) && !Event_GetFlag(FLAG_GABOMBA_CHANNEL_FIXED))
      GabombaStatue_FixTheChannel();
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
    Event_End();
  }
}

/// Sets up the pinion room; coming in from outside, Retreat leads back to it.
void GabombaStatue_InitializePinion(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineWork.entrance == 1) {
    Event_SetFlag(FLAG_GABOMBA_RETREAT_TO_PINION);
    Event_ClearFlag(FLAG_GABOMBA_RETREAT_TO_IDOL);
  }
}

static __inline__ void *Mem_Alloc(s32 size) { return Heap_Alloc(size); }

void GabombaStatue_LoadTileGraphics(void) {
  struct SceneState *state = &gSceneState;
  void *buffer = Mem_Alloc(3072);
  s32 handle;

  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  handle = VramCache_FindFreeEntry();
  VramCache_Load(handle, 3072, buffer);
  state->tileEntry = handle;
  Heap_Release(buffer);
}

/// Whether channel cell (x, z) holds a piece; 0 outside the grid.
s32 GabombaStatue_IsCellTaken(u32 x, u32 z) {
  u8 *cells = ((u8 *)&gSceneState);

  if (x >= CHANNEL_COLUMNS || z >= CHANNEL_ROWS)
    return 0;
  {
    u8 *cell = cells + (z * CHANNEL_COLUMNS + x) * 4;
    s32 code;

    if (cell[3] != 0 || cell[1] != 0)
      return 1;
    code = cell[0];
    code ^= 10;
    return 1 - NONZERO(code);
  }
}
/// Whether a cell beside channel cell (x, z) holds a piece.
s32 GabombaStatue_HasTakenNeighbour(s32 x, s32 z) {
  s32 found;

  if (GabombaStatue_IsCellTaken(x, z - 1) ||
      GabombaStatue_IsCellTaken(x, z + 1) ||
      GabombaStatue_IsCellTaken(x - 1, z))
    return 1;
  found = GabombaStatue_IsCellTaken(x + 1, z);
  return NONZERO(found);
}

/// Empties every cell of the channel's grid.
void GabombaStatue_ClearGrid(void) {
  struct SceneState *state = &gSceneState;
  u32 row;
  u32 col;

  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      struct PuzzleCell *cell = state->cells[0] + (row * CHANNEL_COLUMNS + col);
      s32 occupied = cell->occupied;

      if (Event_GetFlag(FLAG_GABOMBA_RESPONDED) && cell->occupied == 2) {
        cell->occupied = 1;
        occupied = 1;
      }
      if (occupied == 2)
        MapGrid_CopyCells(0, 11, 1, 1, state->originX + col,
                          state->originZ + row);
      else if (occupied == 1)
        MapGrid_CopyCells(1, state->cells[row][col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
      else
        MapGrid_CopyCells(occupied, state->cells[row][col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
    }
}

s32 GabombaStatue_GetPiece(s32 x, s32 z, s32 direction);

/// Lays this game's channel puzzle at random: paths of the given length to the
/// head in the middle (GabombaStatue_LayPath), their pieces scattered as tiles.
s32 GabombaStatue_LayChannel(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 row, col, n, straight, corner, cross, tries, left, mode;
  s32 piece, shape;
  struct PuzzleCell *cell;

  state->count = value - 1;
retry:
  state->count += 1;
  gRandomSeed = state->count;
  Iwram_ClearWordsEntry(state, sizeof(state->cells));
  state->cells[3][5].kind = 10;
  state->cells[7][1].kind = 10;
  state->cells[7][9].kind = 10;
  state->cells[7][0].blocked = 1;
  state->cells[7][2].blocked = 1;
  state->cells[7][8].blocked = 1;
  state->cells[7][10].blocked = 1;
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++)
      MapGrid_CopyCells(0, state->cells[row][col].kind, 1, 1,
                        state->originX + col, state->originZ + row);
  n = 0;
  if (Random16() < 32768) {
    n |= GabombaStatue_LayPath(2, 7, 5, 7, 4, 0, 3, 2);
    n |= GabombaStatue_LayPath(8, 7, 6, 6, 4, 2, 2, 3);
    GabombaStatue_WritePiece(5, 6, 9, 12);
    n |= GabombaStatue_LayPath(4, 6, 4, 3, 12, 2, 0, 2);
    n |= GabombaStatue_LayPath(5, 5, 5, 2, 14, 3, 1, 3);
  } else {
    n |= GabombaStatue_LayPath(2, 7, 4, 6, 4, 0, 0, 2);
    n |= GabombaStatue_LayPath(8, 7, 5, 7, 4, 2, 3, 3);
    GabombaStatue_WritePiece(5, 6, 8, 12);
    n |= GabombaStatue_LayPath(5, 5, 4, 3, 12, 3, 0, 2);
    n |= GabombaStatue_LayPath(6, 6, 5, 2, 14, 0, 1, 3);
  }
  if (n != 0)
    goto retry;
  n = GabombaStatue_LayPath(0, 7, 5, 4, 19, 2, 3, 0);
  n |= GabombaStatue_LayPath(10, 7, 6, 3, 17, 0, 2, 1);
  if (n != 0)
    goto retry;
  straight = 0;
  corner = 0;
  cross = 0;
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      piece = state->cells[row][col].kind;
      shape = state->cells[row][col].state;
      if (shape == 5)
        straight++;
      if (shape == 10)
        corner++;
      if (shape == 12 && (u32)(piece - 8) <= 1)
        cross++;
    }
  if (straight == 0 || corner == 0 || cross == 0)
    goto retry;
  mode = Random_Below(2);
  left = 7;
  for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
    col = Random_Below(CHANNEL_COLUMNS);
    row = (Random_Below(3)) + 5;
    if (GabombaStatue_HasTakenNeighbour(col, row) == 0) {
      piece = state->cells[row][col].kind;
      if ((u32)(piece - 1) <= 1) {
        shape = state->cells[row][col].state;
        if ((mode == 0 && (shape & 6) != 0) ||
            (mode == 1 && (shape & 9) != 0)) {
          state->cells[row][col].kind = 10;
          state->cells[row][col].occupied = 1;
          left = 6;
          break;
        }
      }
    }
  }
  for (n = 0; n < 4; n++)
    for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
      col = Random_Below(CHANNEL_COLUMNS);
      row = Random_Below(8);
      if (GabombaStatue_HasTakenNeighbour(col, row) == 0) {
        piece = state->cells[row][col].kind;
        if (piece == 3 || piece == 8 || piece == 9) {
          cell = &state->cells[row][col];
          shape = cell->state;
          if (shape != 6 && shape != 9) {
            switch (piece) {
            case 3:
              cell->kind = (Random_Below(2)) + 1;
              break;
            case 8:
              cell->kind = ((Random_Below(2)) << 1) + 4;
              break;
            case 9:
              cell->kind = ((Random_Below(2)) << 1) + 5;
              break;
            }
            state->cells[row][col].occupied = 1;
            left--;
            break;
          }
        }
      }
    }
  for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
    col = Random_Below(CHANNEL_COLUMNS);
    row = Random_Below(8);
    if (GabombaStatue_HasTakenNeighbour(col, row) == 0) {
      piece = state->cells[row][col].kind;
      if (piece == 4 || piece == 6 || piece == 5 || piece == 7) {
        state->cells[row][col].occupied = 1;
        left--;
        break;
      }
    }
  }
  for (n = 0; n < left; n++)
    for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
      col = Random_Below(CHANNEL_COLUMNS);
      row = Random_Below(8);
      if (GabombaStatue_HasTakenNeighbour(col, row) == 0) {
        piece = state->cells[row][col].kind;
        if ((u32)(piece - 1) <= 1) {
          state->cells[row][col].occupied = 1;
          break;
        }
      }
    }
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      piece = state->cells[row][col].kind;
      shape = state->cells[row][col].occupied;
      if (shape != 0) {
        switch (Random_Below(8)) {
        case 0:
        case 1:
        case 2:
          piece = GabombaStatue_GetPiece(col, row, 0);
          break;
        case 3:
        case 4:
        case 5:
          piece = GabombaStatue_GetPiece(col, row, 1);
          break;
        case 6:
          GabombaStatue_GetPiece(col, row, 0);
          piece = GabombaStatue_GetPiece(col, row, 0);
          break;
        }
      }
      MapGrid_CopyCells(shape, piece, 1, 1, state->originX + col,
                        state->originZ + row);
    }
  return state->count;
}

u8 gGabombaStatue_PathDirections[32] BSS_DATA;
u8 gGabombaStatue_PathXs[32] BSS_DATA;
u8 gGabombaStatue_PathZs[32] BSS_DATA;
u8 gGabombaStatue_PathKinds[32] BSS_DATA;
u8 gGabombaStatue_PathTried[32] BSS_DATA;
u8 gGabombaStatue_PathFlags[32] BSS_DATA;
extern s8 gGabombaStatue_PathSteps[];
extern u8 gGabombaStatue_PieceJoins[];
/// Lays a random path of `length` cells across the Gabomba Statue grid
/// (gSceneState, 11 by 8 cells of four bytes) from (x0, z0) to (endX, endZ),
/// backtracking through per-step stacks (gGabombaStatue_PathDirections to
/// gGabombaStatue_PathFlags: direction, x, z, kind, tried directions and flags)
/// when a cell has no free neighbour; each step joins the cells
/// (GabombaStatue_PickPiece picks the piece, GabombaStatue_WritePiece writes
/// it). Returns 0, or -1 when the parity of the distance rules a path out or
/// the search gives up.
s32 GabombaStatue_LayPath(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length,
                          s32 direction, s32 last, s32 bit) {
  u8 *cells, *kinds;
  s32 piece;
  s32 spare, step, dir, used, dz, index, one;
  u32 x, z, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z0;
  spare = length - abs(endX - x0) - abs(dz);
  Iwram_ClearWordsEntry(gGabombaStatue_PathDirections, 32);
  Iwram_ClearWordsEntry(gGabombaStatue_PathXs, 32);
  Iwram_ClearWordsEntry(gGabombaStatue_PathZs, 32);
  Iwram_ClearWordsEntry(gGabombaStatue_PathKinds, 32);
  Iwram_ClearWordsEntry(gGabombaStatue_PathTried, 32);
  Iwram_ClearWordsEntry(gGabombaStatue_PathFlags, 32);
  if (spare & 1)
    return -1;
  x = x0;
  cells = (u8 *)((struct PuzzleCell *)&gSceneState);
  one = 1;
  step = 0;
  attempts = 0;
  z = z0;
  while (1) {
    if (++attempts > 1280)
      return -1;
    {
      s32 offset = (z * CHANNEL_COLUMNS + x) * 4;
      gGabombaStatue_PathKinds[step] = cells[offset];
      gGabombaStatue_PathFlags[step] =
          ((struct PuzzleCell *)(cells + offset))->state;
    }
    gGabombaStatue_PathXs[step] = x;
    gGabombaStatue_PathZs[step] = z;
    gGabombaStatue_PathDirections[step] = direction;
    {
      u8 *joins = gGabombaStatue_PieceJoins + 1;
      kinds = gGabombaStatue_PathKinds;
      gGabombaStatue_PathTried[step] = gGabombaStatue_PathTried[step] |
                                       joins[kinds[step]] |
                                       1 << ((direction + 2) & 3);
    }
    tries = 0;
    while (1) {
      u32 value;
      if (++tries <= 63) {
        value = Random16();
        dir = direction;
        if (value > 32767)
          dir = Random_Below(4);
        tried = gGabombaStatue_PathTried[step];
        if ((tried >> dir) & one)
          continue;
        {
          s8 *steps = gGabombaStatue_PathSteps + 1;
          index = dir * 2;
          nextX = x + steps[index];
          index += 1;
          nextZ = z + steps[index];
        }
        if (nextX <= 10 && nextZ <= 7 &&
            !(step == length - 1 && (nextX != endX || nextZ != endZ))) {
          u8 *cell = cells + (nextZ * CHANNEL_COLUMNS + nextX) * 4;
          if (cell[1] == 0 && (used = (cell[2] >> bit) & one) == 0 &&
              cell[0] != 10) {
            tried |= one << dir;
            gGabombaStatue_PathTried[step] = tried;
            kinds = gGabombaStatue_PathKinds;
            piece = GabombaStatue_PickPiece(direction, dir, kinds[step]);
            {
              s32 offset = (z * CHANNEL_COLUMNS + x) * 4;
              GabombaStatue_WritePiece(x, z, piece,
                                       (cells + offset)[2] | one << bit);
            }
            step++;
            x = nextX;
            z = nextZ;
            direction = dir;
            if (step == length)
              goto done;
            gGabombaStatue_PathTried[step] = 0;
            break;
          }
        }
        gGabombaStatue_PathTried[step] |= 1 << dir;
        if (gGabombaStatue_PathTried[step] != 0x0f)
          continue;
      }
      gGabombaStatue_PathTried[step] = 0;
      if (step == 0)
        return -1;
      step--;
      x = gGabombaStatue_PathXs[step];
      z = gGabombaStatue_PathZs[step];
      direction = gGabombaStatue_PathDirections[step];
      GabombaStatue_WritePiece(x, z, gGabombaStatue_PathKinds[step],
                               gGabombaStatue_PathFlags[step]);
      break;
    }
  }
done:
  {
    s32 offset = (nextZ * CHANNEL_COLUMNS + nextX) * 4;
    piece = GabombaStatue_PickPiece(dir, last, cells[offset]);
    GabombaStatue_WritePiece(nextX, nextZ, piece,
                             (cells + offset)[2] | 1 << bit);
  }
  return 0;
}

extern u8 gGabombaStatue_PieceTable[];
extern u8 gGabombaStatue_TurnedPieces[];
/// The channel piece at row and column of the piece table, turned for a charge
/// coming from direction.
s32 GabombaStatue_PickPiece(s32 row, s32 column, s32 direction) {
  s32 value = gGabombaStatue_PieceTable[row * 4 + column];

  if (direction == 2 && value == 1)
    value = 3;
  if (direction == 1 && value == 2)
    value = 3;
  if (direction == 4 && value == 6)
    value = 8;
  if (direction == 6 && value == 4)
    value = 8;
  if (direction == 7 && value == 5)
    value = 9;
  if (direction == 5 && value == 7)
    value = 9;
  return value;
}

/// Puts piece kind, in state, at channel cell (x, z).
void GabombaStatue_WritePiece(s32 x, s32 z, s32 kind, s32 state) {

  gSceneState.cells[z][x].kind = kind;
  gSceneState.cells[z][x].state = state;
}

/// Shows or hides the statues: gives each occupied cell's actor, numbered from
/// id, its world position and draws its tile, or clears the actor and restores
/// the cell's kind tile.
void GabombaStatue_ShowTiles(s32 id, s32 show) {
  struct SceneState *state = &gSceneState;
  s32 row;
  s32 column;

  state->statuesReady = show;
  state->firstId = id;
  row = 0;
  do {
    column = 0;
    do {
      {
        struct PuzzleCell *cell =
            state->cells[0] + (row * CHANNEL_COLUMNS + column);

        if (cell->occupied != 0) {
          struct Actor *actor = Actor_Get(id);
          s32 mapX;
          s32 mapZ;
          s32 kind;

          if (show != 0) {
            actor->x.fixed = FIELD_CELL_CENTER(state->originX + column);
            actor->z.fixed = FIELD_CELL_CENTER(state->originZ + row);
            MapGrid_CopyCells(0, 11, 1, 1, state->originX + column,
                              state->originZ + row);
            cell->occupied = 2;
          } else {
            actor->x.fixed = 0;
            actor->z.fixed = 0;
            ((u8 *)cell)[3] = 1;
            MapGrid_CopyCells(1, cell->kind, 1, 1, state->originX + column,
                              state->originZ + row);
          }
          id++;
        }
      }
      column++;
    } while (column <= 10);
    row++;
  } while (row < CHANNEL_ROWS);
  state->endId = id;
}
#undef FIELD_AT

/// Turns the piece at channel cell (x, z) by direction, and returns it.
s32 GabombaStatue_GetPiece(s32 x, s32 z, s32 direction) {

  gSceneState.cells[z][x].kind =
      gGabombaStatue_TurnedPieces[gSceneState.cells[z][x].kind * 2 + direction];
  return gSceneState.cells[z][x].kind;
}

/// A step of the tiles rising out of the channel as the grey cone sinks.
void GabombaStatue_RaiseTiles(s32 value, s32 row) {
  struct Actor *actor = Actor_Get(ACTOR_GREY_CONE);

  if (row < CHANNEL_ROWS)
    actor->scaleY += -FIXED_ONE * 31 / 256;
  if (value == 1) {
    GabombaStatue_ShowTiles(gSceneState.firstId, 1);
    Actor_SelectAnimation(actor, 6);
    actor->scaleY = FIXED_ONE;
  }
}

/// Pounding a raised tile brings it down into its place in the channel.
void GabombaStatue_PoundTile(s32 mode) {
  struct SceneState *state;
  struct Actor *actor;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  state = &gSceneState;
  actor = Actor_Get(((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)
                        ->secondActorId);
  x = FIELD_CELL(actor->x.fixed) - state->originX;
  z = FIELD_CELL(actor->z.fixed) - state->originZ;
  {
    s32 kind = state->cells[z][x].kind;

    cell = &state->cells[z][x];
    cell->occupied = mode;
    MapGrid_CopyCells(1, kind, 1, 1, state->originX + x, state->originZ + z);
  }
  actor->x.fixed = 0;
  actor->z.fixed = 0;
  found = 0;
  row = 0;
  for (row = 0; row < CHANNEL_ROWS; row++) {
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      if (state->cells[row][col].occupied == 2) {
        found = 1;
        break;
      }
    }
    if (found)
      return;
  }
  state->statuesReady = found;
}

/// Sets the first raised tile's animation.
void GabombaStatue_SetTileMode(s32 mode) {
  Actor_SetAnimationById(gSceneState.firstId, mode);
}

/// Turns every raised tile a quarter turn.
void GabombaStatue_TurnTiles(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *actor = Actor_Get(id);

    GabombaStatue_GetPiece(FIELD_CELL(actor->x.fixed) - state->originX,
                           FIELD_CELL(actor->z.fixed) - state->originZ, value);
  }
}

/// Redraws side A once; with the statues ready, moves their offset down by
/// 0x333 a frame for twenty frames, then clears it.
void GabombaStatue_LowerArrowA(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawA == 0) {
    MapGrid_CopyCellsLayered(1, 13, 1, 1, state->originX + 3,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(SE_GABOMBA_STATUE_ARROW);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      GabombaStatue_SetTileMode(3);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset -= TRIG_TURN / 80;
        System_Wait(1);
        i--;
      } while (i >= 0);
      GabombaStatue_TurnTiles(0);
      Event_End();
      state->statueOffset = 0;
    }
    state->redrawA = 1;
  }
}

/// Redraws the first arrow over the channel, when it is due.
void GabombaStatue_RedrawArrowA(void) {
  if (gSceneState.redrawA != 0) {
    MapGrid_CopyCellsLayered(1, 12, 1, 1, gSceneState.originX + 3,
                             gSceneState.originZ - 2);
    gSceneState.redrawA = 0;
  }
}

/// Side B's twin of GabombaStatue_LowerArrowA: the offset moves up instead.
void GabombaStatue_RaiseArrowB(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawB == 0) {
    MapGrid_CopyCellsLayered(0, 13, 1, 1, state->originX + 7,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(SE_GABOMBA_STATUE_ARROW);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      GabombaStatue_SetTileMode(2);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset += TRIG_TURN / 80;
        System_Wait(1);
        i--;
      } while (i >= 0);
      GabombaStatue_TurnTiles(1);
      Event_End();
      state->statueOffset = 0;
    }
    state->redrawB = 1;
  }
}

/// Redraws the second arrow over the channel, when it is due.
void GabombaStatue_RedrawArrowB(void) {
  if (gSceneState.redrawB != 0) {
    MapGrid_CopyCellsLayered(0, 12, 1, 1, gSceneState.originX + 7,
                             gSceneState.originZ - 2);
    gSceneState.redrawB = 0;
  }
}

/// Draws the raised tiles as sprites over the room.
void GabombaStatue_DrawFloatingTiles(void) {
  s32 affine = 0;
  s32 *scroll = &Ram_HeapSlots->mapGrid->viewX;
  struct SceneState *state = &gSceneState;
  struct SpriteView *oam = state->oam;
  s32 scrollX = scroll[0] & 0xffff0000;
  s32 scrollZ = scroll[1] & 0xffff0000;
  u16 tile = gVramCacheEntries[state->tileEntry].offset >> 5;
  struct AffineTransform effect;
  s32 id;

  if (state->statueOffset != 0) {
    effect.angle = state->statueOffset;
    effect.scaleX = Q_8_8(1);
    effect.scaleY = Q_8_8(1);
    affine = OamMatrix_AllocFromTransform(&effect);
  }
  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *piece = Actor_Get(id);
    FIXED px = piece->x.fixed;
    if (px != 0) {
      s32 x = px - scrollX;
      FIXED y = piece->y.fixed + toFIXED(16);
      FIXED pz = piece->z.fixed;
      FIXED z = pz - scrollZ;
      s32 sy = z - y;
      s32 depth = FIXEDtoINT(y + z) + 58;
      s32 cellX = FIELD_CELL(px) - state->originX;
      s32 cellZ = FIELD_CELL(pz) - state->originZ;
      s32 kind = state->cells[cellZ][cellX].kind;
      u32 *p;
      x = (FIXEDtoINT(x) - 8) & 0x1ff;
      sy = (FIXEDtoINT(sy) - 8) & 255;
      p = (u32 *)oam;
      *p++ = 0;
      *p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      *p = (tile + (kind << 3)) | OBJ_PRIORITY(2);
      if (state->statueOffset != 0) {
        oam->affineMode = 1;
        oam->matrix = affine;
      }
      Oam_AddSpriteAtDepthClamped(oam, depth);
      oam++;
    }
  }
}

/// Sets up the channel puzzle on entering the room, or redraws it when
/// returning.
s32 GabombaStatue_SetUpChannelPuzzle(s32 x, s32 z, s32 value, s32 id) {
  struct SceneState *state = &gSceneState;
  s32 count;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    GabombaStatue_LoadTileGraphics();
    GabombaStatue_ClearGrid();
    count = state->count;
    Task_AddFrame(GabombaStatue_DrawFloatingTiles, TASK_PASS_FRAME);
  } else {
    Iwram_ClearWordsEntry(state, sizeof(struct SceneState));
    state->originX = x;
    state->originZ = z;
    GabombaStatue_LoadTileGraphics();
    count = GabombaStatue_LayChannel(value);
    GabombaStatue_ShowTiles(id, 0);
    Task_AddFrame(GabombaStatue_DrawFloatingTiles, TASK_PASS_FRAME);
  }
  Actor_Get(id)->retainSpriteCache = 2;
  return count;
}

extern s32 gGabombaStatue_TileFallScript[];
extern u32 gGabombaStatue_TileRiseScript[];

// GabombaStatue_RunCharges passes the charge's actor id as well, which this
// routine never reads.
/// A charge's step along the channel's pieces, splashing at its end.
void GabombaStatue_MoveCharge(struct Actor *charge, s32 step, s32 id) {
  s16 *colp = &charge->chargeColumn;
  s32 col = *colp;
  s16 *rowp = &charge->chargeRow;
  s32 row = *rowp;
  u8 *headingp = &charge->chargeHeading;
  s32 heading = *headingp;
  s32 angle = heading << 14;
  s32 cell = row * CHANNEL_COLUMNS + col;
  struct SceneState *board = &gSceneState;
  s32 piece = board->cells[row][col].kind;
  s32 goal = board->cells[row][col].occupied;
  s32 key = (heading << 8) + piece;
  s32 d = step - 8;
  s32 arc = step << 10;
  FIXED s = Math_MulFX(toFIXED(8), Trig_Sin(arc));
  FIXED c = Math_MulFX(toFIXED(8), Trig_Cos(arc));
  s32 z;
  s32 x;

  x = FIELD_CELL_CENTER(board->originX + col);
  z = FIELD_CELL_CENTER(board->originZ + row);
  if (goal == 2) {
    heading = row = col = 0;
    if (charge->chargeState == 1) {
      Sound_Play(SE_DUST_PUFF);
      Actor_StartScript(charge, gGabombaStatue_TileFallScript);
      charge->chargeState = goal;
    }
  } else {
    switch (key) {
    case 514:
    case 515:
      col--;
      x -= INTtoFIXED(d);
      break;
    case 2:
    case 3:
      col++;
      x += INTtoFIXED(d);
      break;
    case 257:
    case 259:
      row++;
      z += INTtoFIXED(d);
      break;
    case 769:
    case 771:
      row--;
      z -= INTtoFIXED(d);
      break;
    case 516:
    case 520:
      row++;
      heading = 1;
      x = x - s + toFIXED(8);
      z = z - c + toFIXED(8);
      angle = DEGtoANG(180) - arc;
      break;
    case 773:
    case 777:
      col--;
      heading = 2;
      x = x + c - toFIXED(8);
      z = z - s + toFIXED(8);
      angle = TRIG_TURN * 3 / 4 - arc;
      break;
    case 6:
    case 8:
      row--;
      heading = 3;
      x = x + s - toFIXED(8);
      z = z + c - toFIXED(8);
      angle = -arc;
      break;
    case 263:
    case 265:
      col++;
      heading = 0;
      x = x - c + toFIXED(8);
      z = z + s - toFIXED(8);
      angle = DEGtoANG(90) - arc;
      break;
    case 519:
    case 521:
      row--;
      heading = 3;
      x = x - s + toFIXED(8);
      z = z + c - toFIXED(8);
      angle = arc + DEGtoANG(180);
      break;
    case 262:
    case 264:
      col--;
      heading = 2;
      x = x + c - toFIXED(8);
      z = z + s - toFIXED(8);
      angle = arc + DEGtoANG(90);
      break;
    case 5:
    case 9:
      row++;
      heading = 1;
      x = x + s - toFIXED(8);
      z = z - c + toFIXED(8);
      angle = arc;
      break;
    case 772:
    case 776:
      col++;
      heading = 0;
      x = x - c + toFIXED(8);
      z = z - s + toFIXED(8);
      angle = arc + TRIG_TURN * 3 / 4;
      break;
    case 11:
      heading = row = col = 0;
      break;
    case 267:
      heading = row = col = 0;
      break;
    case 523:
      heading = row = col = 0;
      break;
    case 10:
    case 266:
    case 522:
    case 778:
      if (charge->chargeState == 1) {
        charge->x.fixed = x;
        charge->y.fixed = 0;
        charge->z.fixed = z;
        charge->facing = angle;
        if (FIELD_CELL(x) == 24 && FIELD_CELL(z) == 14) {
          charge->chargeState = 7;
        } else {
          Sound_Play(SE_DUST_PUFF);
          Actor_StartScript(charge, gGabombaStatue_TileFallScript);
          charge->chargeState = 2;
        }
      }
      break;
    case 779:
      heading = row = col = 0;
      break;
    default:
      heading = row = col = 0;
      if (charge->chargeState == 1) {
        Sound_Play(SE_WATER);
        Actor_StartScript(charge, gGabombaStatue_TileRiseScript);
        charge->chargeState = 2;
      }
      break;
    }
  }
  if (step == 15) {
    *colp = col;
    *rowp = row;
    *headingp = heading;
  }
  if (charge->chargeState == 1) {
    charge->y.fixed = 0;
    charge->x.fixed = x;
    charge->z.fixed = z;
    charge->facing = angle;
    charge->riseBaseY++;
  }
}

/// Runs a pair of charges, one red and one blue, along the channel to the head.
s32 GabombaStatue_RunCharges(s32 first, s32 check) {
  struct Actor *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = Actor_Get(first);
  actor->chargeColumn = 0;
  actor->chargeRow = 7;
  actor->chargeHeading = 2;
  actor->chargeState = 1;
  actor->riseBaseY = 0;
#ifdef UBFIX
  Actor_StartIdle(actor);
#else
  // UB: called without the actor it starts, which it finds in r0 only because
  // Actor_Get left it there.
  Actor_StartIdle();
#endif
  Actor_SelectAnimation(actor, 2);
  id = second;
  actor = Actor_Get(id++);
  actor->chargeColumn = 2;
  actor->chargeRow = 7;
  actor->chargeHeading = 0;
  actor->chargeState = 1;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  actor = Actor_Get(id++);
  actor->chargeColumn = 8;
  actor->chargeRow = 7;
  actor->chargeHeading = 2;
  actor->chargeState = 1;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  actor = Actor_Get(id);
  actor->chargeColumn = 10;
  actor->chargeRow = 7;
  actor->chargeHeading = 0;
  actor->chargeState = 1;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  for (tries = 0; tries <= 127; tries++) {
    done = 1;
    for (frame = 0; frame <= 15; frame++) {
      for (id = first; id < first + 4; id++) {
        actor = Actor_Get(id);
        if (actor->chargeState != 0 && actor->chargeState != 7)
          done = 0;
        GabombaStatue_MoveCharge(actor, frame, id);
      }
      System_Wait(1);
    }
    if (done)
      break;
  }
  System_Wait(20);
  result = 3;
  if (check) {
    u32 a = 0;
    u32 b = 0;
    {
      struct Actor *r = Actor_Get(first);
      if (r->x.fixed != 0)
        a = r->riseBaseY;
    }
    {
      struct Actor *r = Actor_Get(second);
      if (r->x.fixed != 0) {
        u32 value = r->riseBaseY;
        if (a == 0 || a > value)
          a = value;
      }
    }
    {
      struct Actor *r = Actor_Get(first + 2);
      if (r->x.fixed != 0)
        b = r->riseBaseY;
    }
    {
      struct Actor *r = Actor_Get(first + 3);
      if (r->x.fixed != 0) {
        u32 value = r->riseBaseY;
        if (b == 0 || b > value)
          b = value;
      }
    }
    if (a != 0) {
      result = 1;
      if (b != 0) {
        result = 0;
        if (a != b) {
          if (a < b) {
            Event_SetFlag(0x9eb);
            result = 1;
          } else {
            Event_SetFlag(0x9eb);
            result = 2;
          }
        }
      }
    } else {
      if (b != 0)
        return 2;
      return 3;
    }
  }
  return result;
}

/// Finds an empty cell of the channel a tile can take.
void GabombaStatue_FindFreeTileCell(u32 *x_out, u32 *z_out) {
  u32 z;
  u32 x;

  for (z = 0; z <= 7; z++) {
    for (x = 0; x <= 10; x++) {
      if (gSceneState.cells[z][x].kind == 10 && !(x == 5 && z == 3) &&
          !(z == 7 && (x == 1 || x == 9))) {
        *x_out = x;
        *z_out = z;
        return;
      }
    }
  }
}

struct ClockWork {
  u8 *tiles;  ///< the digit sheet
  u8 *digits; ///< the five digit pictures, as loaded
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct SpriteView oam[8];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

/// The clock's work, kept in the inventory snapshot's spare room.
#define CLOCK_WORK ((struct ClockWork *)(gInventorySnapshot + 0x288))

/// Converts the remaining frames into minutes, seconds and tenths
/// (`MM:SS:T`).
void GabombaStatue_FormatClock(s32 time, u8 *text) {
  if (time < 0)
    time = 0;
  *text = time / (FRAMES_PER_SECOND * 60 * 10) + '0';
  time %= (FRAMES_PER_SECOND * 60 * 10);
  *++text = time / (FRAMES_PER_SECOND * 60) + '0';
  time %= (FRAMES_PER_SECOND * 60);
  *++text = ':';
  *++text = time / (FRAMES_PER_SECOND * 10) + '0';
  time %= (FRAMES_PER_SECOND * 10);
  *++text = time / FRAMES_PER_SECOND + '0';
  time %= FRAMES_PER_SECOND;
  *++text = ':';
  *++text = time / (FRAMES_PER_SECOND / 10) + '0';
  text[1] = 0;
}

/// The clock's tick each second, and its ring as time runs out.
s32 GabombaStatue_TickClock(s32 time) {
  struct ClockWork *clock = CLOCK_WORK;
  s32 second = time % FRAMES_PER_SECOND;
  s32 volume = 512;
  u8 *ringing = &clock->ringing;

  if (second != 0) {
    *ringing = 1;
  } else if (*ringing) {
    if (clock->timer != 0)
      Event_PlaySound(SE_COLOSSO_COUNTDOWN);
    *ringing = second;
  }
  if (second <= 19) {
    FIXED cosine = Trig_Cos((second << 15) / 10);
    s32 (*mul)(s32 a, s32 b);
    s32 scale = 128;

    mul = Iwram_MulFX;
    volume = mul(scale << 2, cosine);
  }
  if (volume <= 16)
    volume = 16;
  return volume;
}

/// Draws the clock's five-digit count: copies the pictures of the digits of
/// timer + 50 out of the digit sheet and loads them, builds a plain and a
/// scaled affine matrix, and fills five OAM entries, giving the scaled matrix
/// to the middle digit and to each digit where timer + 60 and timer differ.
void GabombaStatue_DrawClock(void) {
  struct ClockWork *clock = CLOCK_WORK;
  u8 *digit = clock->digit;
  u8 *old = clock->tiles;
  u8 *now = clock->digits;
  u32 tile = gVramCacheEntries[clock->tileEntry].offset >> 5;
  struct SpriteView *oam = clock->oam;
  s32 *timer = &clock->timer;
  struct AffineTransform effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  GabombaStatue_FormatClock(*timer + 50, digit);
  GabombaStatue_FormatClock(*timer + 60, clock->old);
  GabombaStatue_FormatClock(*timer, clock->now);
  // old and now walk the sheet and the pictures here, the digit lists below.
  for (i = 0; i < 5; i++) {
    Iwram_CopyWordsEntry(now, old + (*digit << 6) - 3072, 64);
    now += 64;
    digit++;
  }
  Vram_Load(clock->tileEntry, 320, clock->digits);
  effect.angle = 0;
  effect.scaleX = Q_8_8(2);
  effect.scaleY = Q_8_8(2);
  plain = OamMatrix_AllocFromTransform(&effect);
  scale = GabombaStatue_TickClock(*timer);
  effect.angle = 0;
  effect.scaleX = scale;
  effect.scaleY = scale;
  scaled = OamMatrix_AllocFromTransform(&effect);
  old = clock->old;
  now = clock->now;
  for (i = 0; i <= 4; i++, tile += 2, oam++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->affineMode = 3;
    if (*old++ != *now++ || i == 2)
      oam->matrix = scaled;
    else
      oam->matrix = plain;
    Oam_AddSpriteAtDepthClamped(oam, 236);
  }
}

s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

/// Counts the clock down while no menu is open, and draws it.
void GabombaStatue_CountDownShowingClock(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ClockWork *clock = CLOCK_WORK;

  if (event->psynergy == 0 && event->fieldMenu == 0 && event->pauseMenu == 0 &&
      clock->timer > 0)
    clock->timer--;
  GabombaStatue_DrawClock();
}

/// Counts the clock down while no menu is open.
void GabombaStatue_CountDown(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ClockWork *clock = CLOCK_WORK;

  if (event->psynergy == 0 && event->fieldMenu == 0 && event->pauseMenu == 0 &&
      clock->timer > 0)
    clock->timer--;
}

/// Starts the clock at one minute, shown or hidden.
void GabombaStatue_StartClock(s32 mode) {
  struct ClockWork *clock = CLOCK_WORK;
  u8 *tiles;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Iwram_ClearWordsEntry(clock, sizeof(struct ClockWork));
    clock->timer = FRAMES_PER_SECOND * 60;
    clock->ringing = 1;
  }
  tiles = Heap_AllocEwram(1024);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_CLOCK_DIGITS), tiles);
  clock->tiles = tiles;
  clock->digits = tiles + 704;
  clock->tileEntry = VramCache_FindFreeEntry();
  Vram_Load(clock->tileEntry, 320, NULL);
  clock->mode = mode;
  if (mode)
    Task_AddFrame(GabombaStatue_CountDownShowingClock, TASK_PASS_FRAME);
  else
    Task_AddFrame(GabombaStatue_CountDown, TASK_PASS_FRAME);
}

/// Stops the clock's countdown.
void GabombaStatue_StopClock(void) {
  struct ClockWork *clock = CLOCK_WORK;

  if (clock->mode)
    FieldTask_Remove(GabombaStatue_CountDownShowingClock);
  else
    FieldTask_Remove(GabombaStatue_CountDown);
}

/// Starts the clock counting down from a minute.
void GabombaStatue_RestartClock(void) {
  struct ClockWork *clock = CLOCK_WORK;

  if (clock->mode)
    Task_AddFrame(GabombaStatue_CountDownShowingClock, TASK_PASS_FRAME);
  else
    Task_AddFrame(GabombaStatue_CountDown, TASK_PASS_FRAME);
  clock->timer = 3600;
}

/// The frames left on the clock.
s32 GabombaStatue_GetClock(void) { return CLOCK_WORK->timer; }

/// Sets the clock back to a minute.
void GabombaStatue_ResetClock(void) {
  CLOCK_WORK->timer = FRAMES_PER_SECOND * 60;
}

/// Layer switch table: three halfwords (id, mode, flag), ended by -1.
void GabombaStatue_PlaceSwitchActors(const u16 *entry) {
  s32 value;

  while ((value = *entry) != (u16)-1) {
    u16 id = *entry++;
    struct Actor *actor = Actor_Get(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)mode, (u16)flag, actor->layer);
    entry += 2;
  }
}

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

u32 gGabombaStatue_CogCellPictures[] = {
    0x20007, 512, 0x20003, 0x20004, 516, 515,
    0x20101, 0x10201, 0x20102, 0x10202, 0xf800, 0,
};
// clang-format on

s8 gGabombaStatue_CogAnimations[] = {
    3,
    1,
    2,
    0,
};

u32 gGabombaStatue_PinionReleaseScript[] = {
    SC_RELEASE,
};

#undef FIELD_AT

void GabombaStatue_FightSteel(void);
void GabombaStatue_LashRope(void);
void GabombaStatue_RideTurntable(void);
void GabombaStatue_StepIntoSecondSteelHole(void);
void GabombaStatue_StepIntoThirdTurntableHole(void);
void GabombaStatue_ConeChimes(void);

// clang-format off
LATE_DATA u8 gGabombaStatue_PieceShapes[] = {
    0, 2, 2, 1, 1, 3, 3, 7, 5, 4,
    6, 5, 7, 6, 4, 9, 9, 8, 8, 10,
};

LATE_DATA s8 gGabombaStatue_PathSteps[] = {
    10, 1, 0, 0, 1, -1, 0, 0,
};

LATE_DATA u8 gGabombaStatue_PieceJoins[] = {
    255, 0, 10, 5, 15, 3, 6, 12,
    9, 15, 15, 15,
};

LATE_DATA u8 gGabombaStatue_PieceTable[] = {
    2, 5, 10, 6, 7, 1, 6, 10,
    10, 4, 2, 7, 4, 10, 5, 1,
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA s32 gGabombaStatue_CogDirection = 0;

LATE_DATA u32 gGabombaStatue_LeaderFocusScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(472, 0, 184), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

LATE_DATA u32 gGabombaStatue_JennaFocusScript[] = {                                                               
      SC_ANIMATION(2),  SC_MOVE(488, 0, 176), SC_SYNC,    SC_ANIMATION(1),  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(1), SC_END,     
  };

LATE_DATA u32 gGabombaStatue_PiersFocusScript[] = {                                                               
      SC_ANIMATION(2),  SC_MOVE(456, 0, 176), SC_SYNC,    SC_ANIMATION(1),  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(1), SC_END,     
  };

LATE_DATA u32 gGabombaStatue_KradenFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(464, 0, 208),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_KradenPointsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(432, 0, 286),
    SC_SYNC,
    SC_MOVE(432, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_LookAtChannelScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(348, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_LookNorthAndEastScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHEAST),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_CALL(GabombaStatue_LookAround),
};

LATE_DATA s32 gGabombaStatue_LookNorthAndWestScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHWEST),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_CALL(GabombaStatue_LookAround),
};

LATE_DATA u32 gGabombaStatue_JennaWatchesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(440, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_ShebaWatchesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(460, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s16 gGabombaStatue_ConePillars[] = {
    ACTOR_FIRST_CONE_PILLAR, 2, FLAG_GABOMBA_FIRST_CONE_POUNDED, ACTOR_SECOND_CONE_PILLAR, 2, FLAG_GABOMBA_SECOND_CONE_POUNDED, -1, 0,
};

LATE_DATA u32 gGabombaStatue_PathLengths[] = {
    36, 122, 201, 312, 431, 504,
    619, 712, 823, 908, 1054, 1121,
    1207, 1311, 1401, 1555,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_PinionExitRegions[] = {
    {784, 16, 80, 800, 32, 96, CONDITION_ALWAYS, 1},
    {80, 48, 368, 96, 64, 384, CONDITION_ALWAYS, 2},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_BoneArmletChestExitRegions[] = {
    {464, 16, 240, 480, 32, 256, CONDITION_ALWAYS, 2},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_ElixirChestExitRegions[] = {
    {528, 16, 64, 544, 32, 80, CONDITION_ALWAYS, 2},
    {96, 16, 160, 112, 32, 176, CONDITION_ALWAYS, 3},
    {672, 16, 160, 688, 32, 176, CONDITION_ALWAYS, 4},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_SteelExitRegions[] = {
    {384, 16, 112, 400, 32, 128, CONDITION_ALWAYS, 4},
    {96, 16, 144, 112, 32, 160, CONDITION_ALWAYS, 5},
    {672, 16, 144, 688, 32, 160, CONDITION_ALWAYS, 6},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_TurntableExitRegions[] = {
    {432, 16, 448, 448, 32, 464, CONDITION_ALWAYS, 4},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_CogsExitRegions[] = {
    {304, 16, 144, 320, 32, 160, CONDITION_ALWAYS, 2},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(GabombaStatuePinion),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_GABOMBA_STATUE_PINION, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueBoneArmletChest),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXITS(GabombaStatueElixirChest),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_ELIXIR_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_WEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_EAST),
    SCENE_EXITS(GabombaStatueSteel),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_WEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_EAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_TURNTABLE_NORTHWEST, GabombaStatueTurntable, GABOMBA_STATUE_TURNTABLE_FROM_GABOMBA_STATUE_STEEL_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_TURNTABLE_WEST, GabombaStatueTurntable, GABOMBA_STATUE_TURNTABLE_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_TURNTABLE_EAST, GabombaStatueTurntable, GABOMBA_STATUE_TURNTABLE_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXITS(GabombaStatueTurntable),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_STEEL_NORTHWEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_TURNTABLE_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_TURNTABLE_WEST),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_TURNTABLE_EAST),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_COGS, GabombaStatueCogs, GABOMBA_STATUE_COGS_FROM_GABOMBA_STATUE_TURNTABLE),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_TURNTABLE_SOUTHEAST_A),
    SCENE_EXIT(GABOMBA_STATUE_TURNTABLE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_TURNTABLE_SOUTHEAST_B),
    SCENE_EXITS(GabombaStatueCogs),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_TURNTABLE, GabombaStatueTurntable, GABOMBA_STATUE_TURNTABLE_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXITS(GabombaStatueArrows),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_EAST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_EAST),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_FACE_EAST_A_A, KibomboStatueFace, KIBOMBO_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_FACE_EAST_B, KibomboStatueFace, KIBOMBO_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_SOUTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_NORTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_D),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_E),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_FACE_EAST_A_B, KibomboStatueFace, KIBOMBO_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST),
    SCENE_EXITS_END,
};

LATE_DATA s32 gGabombaStatue_TurntableScript[] = {
    SC_CALL(GabombaStatue_FlashCog),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_TurnToothScript[] = {
    SC_CALL(GabombaStatue_TurnTooth),
    SC_END,
};

LATE_DATA struct ActorPlacement gGabombaStatue_BoneArmletChestActorPlacements[] = {
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_ElixirChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gGabombaStatue_TurnToothScript, 328, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 72, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 72, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 88, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 488, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, (s32)gGabombaStatue_TurntableScript, 360, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_SteelActorPlacements[] = {
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 120, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_STEEL), MOVEMENT_TYPE_STAND, 504, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_TurntableActorPlacements[] = {
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 232, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(358, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 376, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(357, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 328, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(360, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(132, FLAG_GABOMBA_MOUSE_SEEN, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_CogsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(359, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gGabombaStatue_ChargeScript[] = {
    SC_CALL(GabombaStatue_CircleOwner),
    SC_END,
};

LATE_DATA struct ActorPlacement gGabombaStatue_ArrowsActorPlacements[] = {
    ACTOR_PLACEMENT(376, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(374, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(374, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(349, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(349, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(349, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(349, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(411, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 430, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_PinionActorPlacements[] = {
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 288, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 336, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(362, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 456, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA s8 gGabombaStatue_RopeStopSteps[] = {
    1, 1, 1, 0, 0, 0, -1, -1,
    -1, 0, 0, 0, 1, 1, 0, 0,
    -1, -1, 0, 0, 1, 0, -1, 0,
    0, 0, 0, 0,
};

LATE_DATA struct Trigger gGabombaStatue_BoneArmletChestTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoBoneArmletChestHole)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_ElixirChestTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoElixirChestHole)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionCarriesLeaderNorth)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionCarriesLeaderSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_ROPE_POST_A, FLAG_SCENE_TEMP_01, EVENT(GabombaStatue_LashRope)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SECOND_ROPE_POST_A, FLAG_SCENE_TEMP_02, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRST_CONE_PILLAR, FLAG_GABOMBA_FIRST_CONE_POUNDED, EVENT(GabombaStatue_PoundConePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_SECOND_CONE_PILLAR, FLAG_GABOMBA_SECOND_CONE_POUNDED, EVENT(GabombaStatue_PoundConePillar)},
    {TRIGGER_RAISED, RAISED_COGS_TURN_BACK, FLAG_GABOMBA_COGS_TURNED_BACK, EVENT(GabombaStatue_CogsTurnBack)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_SteelTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoFirstSteelHole)},
    {STEP_TRIGGER_WALKING, 2, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoSecondSteelHole)},
    {STEP_TRIGGER_WALKING, 3, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoThirdSteelHole)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(GabombaStatue_RideTurntable)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRollingCog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_STEEL_ROPE_POST_A, FLAG_SCENE_TEMP_01, 0},
    {TRIGGER_TALK, ACTOR_STEEL, IF_FLAG_SET(FLAG_SCENE_TEMP_04), EVENT(GabombaStatue_FightSteel)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_TurntableTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoFirstTurntableHole)},
    {STEP_TRIGGER_WALKING, 2, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoSecondTurntableHole)},
    {STEP_TRIGGER_WALKING, 3, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoThirdTurntableHole)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(GabombaStatue_RideTurntable)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRollingCog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {TRIGGER_STEP, 77, FLAG_GABOMBA_MOUSE_SEEN, EVENT(GabombaStatue_MouseShowsTheWay)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_CogsTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_StepIntoCogsHole)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(GabombaStatue_RideTurntable)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRollingCog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 84, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionCarriesLeaderEast)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionCarriesLeaderWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_COGS_ROPE_POST_A, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_ArrowsTriggers[] = {
    {STEP_TRIGGER_WALKING, 1, CONDITION_ALWAYS, EVENT(GabombaStatue_JumpDownHole)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(GabombaStatue_RedrawArrowA)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(GabombaStatue_LowerArrowA)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(GabombaStatue_RedrawArrowB)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(GabombaStatue_RaiseArrowB)},
    {TRIGGER_RAISED, RAISED_OFFERING_HEARD, FLAG_SCENE_TEMP_05, EVENT(GabombaStatue_HearTheOffering)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_GREY_CONE, FLAG_GABOMBA_RESPONDED, EVENT(GabombaStatue_PoundGreyCone)},
    {TRIGGER_RAISED, RAISED_CONE_CHIMES, CONDITION_ALWAYS, EVENT(GabombaStatue_ConeChimes)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_A, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_B, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_C, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_D, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_E, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_F, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_G, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundTile)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_PinionTriggers[] = {
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionCarriesLeader)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionThrowsLeader)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA u8 gGabombaStatue_TurnedPieces[] = {
    0, 0, 2, 2, 1, 1, 3, 3,
    7, 5, 4, 6, 5, 7, 6, 4,
    9, 9, 8, 8, 10, 10, 11, 11,
};

LATE_DATA u32 gGabombaStatue_TileRiseScript[] = {
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_TileFallScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_SCALE_X, FIXED_ONE * 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 2),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(48, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_WAIT(1),
    SC_END,
};
// clang-format on
