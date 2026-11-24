/// @file
/// A summon tablet read: its pictures cycling, its pebbles and runes flying,
/// and placing the flag blocks.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field_event.h"
#include "field/field.h"
#include "field/actor_script.h"

/// The summon tablet's update while its runes fly: on the actor's timer, its
/// sprite flashes white and outlined, then outlined, white again, and plain,
/// redrawn each frame; the fourth step leaves the outline as it was.
void SummonTablet_CyclePictures(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;
  struct SpriteAnimation *layer = sprite->entries[0];

  switch (actor->timer & 3) {
  case 0:
    layer->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    break;
  case 1:
    sprite->dirty = 1;
    sprite->flags = 0;
    break;
  case 2:
    layer->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = 0;
    break;
  case 3:
    layer->colorMode = SPRITE_COLOR_NORMAL;
    sprite->dirty = 1;
    break;
  }
  actor->timer++;
}

/// A point on the screen chasing its target, 16.16 pixels, as
/// Camera_WorldToScreen gives one: z down the screen.
struct Chase {
  FIXED x;
  FIXED z;
  FIXED unusedAfterZ; ///< no code names it
  FIXED targetX;
  FIXED targetZ;
};

/// The sign bit of a 32-bit number: two numbers' signs differ where their
/// exclusive or has it.
#define SIGN_BIT 0x80000000

/// Moves toward the target by speed, stopping on it on overshoot.
void Motion_StepTowardTarget(struct Chase *chase, FIXED speed) {
  s32 dx = chase->targetX - chase->x;
  s32 dz = chase->targetZ - chase->z;
  u16 angle = Trig_Atan(dz, dx);

  chase->x += Iwram_MulFX(Trig_Cos(angle), speed);
  chase->z += Iwram_MulFX(Trig_Sin(angle), speed);
  {
    s32 nextDx = chase->targetX - chase->x;
    s32 nextDz = chase->targetZ - chase->z;

    if ((nextDx ^ dx) & SIGN_BIT)
      chase->x = chase->targetX;
    if ((dz ^ nextDz) & SIGN_BIT)
      chase->z = chase->targetZ;
  }
}

/// One rune or pebble and a frame count, negative before it starts.
struct TabletPiece {
  union {
    /// A rune: a point on the screen chasing its target.
    struct Chase rune;
    /// A pebble: a position in the world and its velocity
    /// (Field_AdvanceWithGravity3D's vector).
    struct {
      FIXED position[3];
      FIXED velocity[3];
    } pebble;
  };
  s32 timer;
};

/// The pebbles that fly off the tablet, a power of two: the next wraps by a
/// mask.
#define TABLET_PEBBLE_COUNT 32
/// The pebbles thrown off at once by the last, heavy blow (SE_HEAVY_IMPACT).
#define TABLET_PEBBLES_ON_IMPACT 25
/// A pebble's four pictures (RESOURCE_PEBBLES, 16 by 16 and shrinking), five
/// frames each; a picture of 256 colours is 8 tile numbers.
#define PEBBLE_PICTURES 4
#define PEBBLE_PICTURE_FRAMES 5
#define PEBBLE_PICTURE_TILES 8
/// The tablet's runes, in three rows of 16.
#define TABLET_RUNE_COUNT 48
#define TABLET_RUNE_ROW 16
/// RESOURCE_RUNES' 16 grey runes, 8 by 8, and after them the same runes lit
/// in blue and green; an 8 by 8 tile of 256 colours is two tile numbers.
#define TABLET_RUNE_PICTURES 16
#define RUNE_LIT (TABLET_RUNE_PICTURES * 2)

/// What the runes do (SummonTabletState.state).
enum RuneMotion {
  RUNES_LINE_UP = 0, ///< each flies from the tablet to its place in the rows
  RUNES_CIRCLE = 2,  ///< each chases the one before round the circle
  RUNES_GATHER = 3   ///< each flies to the leader and is drawn till it lands
};

/// The summon tablet's state (heap slot battleEffect): the decoded pictures,
/// the pebbles and runes that fly between the tablet and the leader, and the
/// circle the runes gather on.
struct SummonTabletState {
  /// The runes' pictures, and then the pebbles', decoded to be loaded into
  /// VRAM.
  u8 pictures[2048];
  struct TabletPiece pebbles[TABLET_PEBBLE_COUNT];
  struct EffectSprite pebbleSprites[TABLET_PEBBLE_COUNT];
  struct TabletPiece runes[TABLET_RUNE_COUNT];
  struct EffectSprite runeSprites[TABLET_RUNE_COUNT];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state; ///< enum RuneMotion
  s8 moving;
  u16 pebbleTile;
  s32 targetX;
  s32 targetZ;
};

/// Task of SummonTablet_PlayRunes: moves and draws the runes and pebbles.
void SummonTablet_UpdateRunes(void) {
  struct SummonTabletState *state = Ram_HeapSlots->battleEffect;
  s32 position[3];
  struct TabletPiece *piece;
  struct EffectSprite *sprite;
  s32 i;
  u16 *tile;

  if (state->moving != 0) {
    position[0] = state->originX;
    position[2] = state->originZ;
    Vector_AddPolarOffset(state->radius, state->angle, position);
    piece = state->runes;
    piece->rune.targetX = position[0];
    piece->rune.targetZ = position[2];
    state->angle += TRIG_TURN * 3 / 128;
    for (i = 0; i < TABLET_RUNE_COUNT; i++, piece++) {
      if (piece->timer >= 0) {
        if (i != 0) {
          piece->rune.targetX = position[0];
          piece->rune.targetZ = position[2];
        }
        position[0] = piece->rune.x;
        position[2] = piece->rune.z;
        Vector_AddPolarOffset(state->spacing, 0, position);
      }
      piece->timer++;
    }
  }

  piece = state->runes;
  sprite = state->runeSprites;
  for (i = 0; i < TABLET_RUNE_COUNT; i++, sprite++, piece++) {
    if (state->state == RUNES_LINE_UP) {
      if (piece->timer >= 0) {
        Motion_StepTowardTarget(&piece->rune, toFIXED(4));
        sprite->x = piece->rune.x;
        sprite->y = piece->rune.z;
        EffectSprite_Draw(sprite);
      }
      piece->timer++;
    } else if (state->state == RUNES_CIRCLE) {
      Motion_StepTowardTarget(&piece->rune, toFIXED(6));
      sprite->x = piece->rune.x;
      sprite->y = piece->rune.z;
      EffectSprite_Draw(sprite);
    } else if (state->state == RUNES_GATHER) {
      if (piece->rune.x != state->targetX || piece->rune.z != state->targetZ) {
        Motion_StepTowardTarget(&piece->rune, toFIXED(6));
        sprite->x = piece->rune.x;
        sprite->y = piece->rune.z;
        EffectSprite_Draw(sprite);
      }
    }
  }

  piece = state->pebbles;
  tile = &state->pebbleTile;
  sprite = state->pebbleSprites;
  for (i = 0; i < TABLET_PEBBLE_COUNT; i++, sprite++, piece++) {
    if ((u32)piece->timer < PEBBLE_PICTURES * PEBBLE_PICTURE_FRAMES) {
      sprite->oam.index = *tile + (piece->timer / PEBBLE_PICTURE_FRAMES & 7) *
                                      PEBBLE_PICTURE_TILES;
      EffectSprite_DrawAtWorldPosition(sprite, piece->pebble.position);
      Field_AdvanceWithGravity3D(piece->pebble.position, DAMPING_SCALE - 1,
                                 -(toFIXED(1) / 2));
      piece->timer++;
    }
  }
}

/// The stages of a summon tablet's reading.
enum TabletStage {
  TABLET_STRUCK = 0,      ///< pebbles fly off it, then it vanishes
  TABLET_RUNES_LIGHT = 1, ///< the runes, lined up, light up in a wave
  TABLET_LEADER_TURNS = 2,
  TABLET_ARMS_RAISED = 3,
  TABLET_RUNES_CIRCLE = 4,    ///< the runes stream round in a circle
  TABLET_RUNES_TO_LEADER = 5, ///< the circle moves over the leader
  TABLET_RUNES_CLOSE_IN = 6,  ///< and shrinks to nothing on it
  TABLET_LEADER_SQUASHED = 7
};

extern const s32 gFieldPsynergy_CastColors[];
extern const s32 gSummonTablet_SquashScript[];
extern const u8 gMovementType_SquashScript[];

/// Plays a summon tablet's reading (a CONTAINER_STONE_TABLET treasure,
/// "examined the stone tablet"): pebbles fly off the tablet as it is struck,
/// and its runes light up and fly round the leader, with their sounds, before
/// Summon_ShowLearnedMessage.
void SummonTablet_PlayRunes(s32 targetId) {
  struct SummonTabletState *summonTabletState =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct SummonTabletState));
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_Lookup(ownerId);
  struct Actor *target = Actor_Lookup(targetId);
  s32 state;
  s32 runeEntry;
  s32 pebbleEntry;
  FIXED ownerX;
  FIXED ownerZ;
  FIXED targetX;
  FIXED targetZ;
  s32 newPebbles = 0;
  s32 nextPebble = 0;
  FIXED pos[3];
  struct EffectSprite *sprite;
  struct TabletPiece *piece;
  s32 runeTile;
  s32 pebbleTile;
  s32 timer;
  s32 runeFrame;
  s32 delay;
  s32 angle;
  s32 i;
  FIXED startX = toFIXED(130);
  FIXED startZ = toFIXED(30);

  Field_StartColorTransition(8, gFieldPsynergy_CastColors[gFrameCount & 7]);
  pos[0] = owner->x.fixed;
  pos[1] = owner->y.fixed + toFIXED(24);
  pos[2] = owner->z.fixed;
  Camera_WorldToScreen(pos);
  ownerX = pos[0];
  ownerZ = pos[2];
  summonTabletState->targetX = ownerX;
  summonTabletState->targetZ = ownerZ;
  pos[0] = target->x.fixed;
  pos[1] = target->y.fixed;
  pos[2] = target->z.fixed;
  Camera_WorldToScreen(pos);
  targetX = pos[0];
  targetZ = pos[2];
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_RUNES),
                      summonTabletState->pictures);
  runeEntry = VramCache_FindFreeEntry();
  runeTile = VramCache_Load(runeEntry, sizeof(summonTabletState->pictures),
                            summonTabletState->pictures);
  piece = summonTabletState->runes;
  i = 0;
  sprite = summonTabletState->runeSprites;
  for (; i < TABLET_RUNE_COUNT; i++) {
    EffectSprite_Init(sprite, 4, 4, 0,
                      runeTile + (Random16() * TABLET_RUNE_PICTURES >> 16) * 2);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 1;
    sprite->depth = OAM_DEPTH_WINDOW;
    piece->rune.x = targetX;
    piece->rune.z = targetZ;
    // The runes line up on the screen in rows 12 pixels apart, the rows 24
    // pixels apart, from (30, 30).
    piece->rune.targetX = (i % TABLET_RUNE_ROW) * toFIXED(12) + toFIXED(30);
    piece->rune.targetZ = (i / TABLET_RUNE_ROW) * toFIXED(24) + toFIXED(30);
    piece->timer = -i;
    sprite++;
    piece++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PEBBLES),
                      summonTabletState->pictures);
  pebbleEntry = VramCache_FindFreeEntry();
  pebbleTile = VramCache_Load(pebbleEntry, sizeof(summonTabletState->pictures),
                              summonTabletState->pictures);
  summonTabletState->pebbleTile = pebbleTile;
  piece = summonTabletState->pebbles;
  sprite = summonTabletState->pebbleSprites;
  for (i = 0; i < TABLET_PEBBLE_COUNT; i++) {
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, pebbleTile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    piece->timer = -1;
    sprite++;
    piece++;
  }
  wait(1);
  Sound_PlayFar(SE_DJINNI_SET);
  Actor_SelectAnimationFar(target, 2);
  Event_Wait(60);
  target->timer = 0;
  target->update = SummonTablet_CyclePictures;
  Sound_PlayFar(SE_SWIRL);
  Event_Wait(80);
  Actor_StartScriptFar(target, gSummonTablet_SquashScript);
  summonTabletState->moving = 0;
  Task_Add(SummonTablet_UpdateRunes, TASK_PASS_FRAME);
  state = TABLET_STRUCK;
  timer = 0;
  do {
    switch (state) {
    case TABLET_STRUCK:
      summonTabletState->state = RUNES_LINE_UP;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Sound_PlayFar(SE_IMPACT);
        newPebbles = 1;
      }
      if (timer == 40) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        newPebbles = TABLET_PEBBLES_ON_IMPACT;
        Actor_StartIdleFar(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_RUNES_LIGHT:
      // Each rune lights up three times, a frame later for each place across
      // or down the rows.
      for (i = 0; i < TABLET_RUNE_COUNT; i++) {
        struct EffectSprite *rune;
        runeFrame = i % TABLET_RUNE_ROW + i / TABLET_RUNE_ROW;
        runeFrame = timer - runeFrame;
        rune = &summonTabletState->runeSprites[i];
        if (runeFrame == 0)
          rune->oam.index += RUNE_LIT;
        if (runeFrame == 3)
          rune->oam.index += -RUNE_LIT;
        if (runeFrame == 8)
          rune->oam.index += RUNE_LIT;
        if (runeFrame == 10)
          rune->oam.index += -RUNE_LIT;
        if (runeFrame == 16)
          rune->oam.index += RUNE_LIT;
        if (runeFrame == 18)
          rune->oam.index += -RUNE_LIT;
      }
      if (timer == 0)
        Sound_PlayFar(SE_GLITTER);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_LEADER_TURNS:
      if (timer == 0) {
        Actor_FaceDirectionById(ownerId, FACING_SOUTH, 0);
        break;
      }
      if (owner->script[owner->scriptCursor] == SCRIPT_COMMAND_END) {
        state++;
        timer = -1;
      }
      if (timer == 60) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_ARMS_RAISED:
      if (timer == 0)
        Actor_SelectAnimationFar(owner, ANIM_LEADER_ARMS_RAISED);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_RUNES_CIRCLE:
      if (timer == 0) {
        piece = summonTabletState->runes;
        delay = 0;
        for (i = 0; i < TABLET_RUNE_COUNT; i++) {
          piece->timer = delay;
          delay -= 3;
          piece++;
        }
        summonTabletState->radius = toFIXED(60);
        summonTabletState->angle = TRIG_TURN * 3 / 4;
        summonTabletState->originX = startX;
        summonTabletState->originZ = startZ;
        summonTabletState->moving = 1;
        summonTabletState->spacing = toFIXED(4);
        summonTabletState->state = RUNES_CIRCLE;
      }
      if (timer == 0)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 48)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 96)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 100) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_RUNES_TO_LEADER:
      summonTabletState->originX = timer * (ownerX - startX) / 30 + startX;
      summonTabletState->originZ = timer * (ownerZ - startZ) / 30 + startZ;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case TABLET_RUNES_CLOSE_IN:
      if (timer == 0)
        summonTabletState->spacing = 0;
      summonTabletState->radius += -toFIXED(2);
      if (summonTabletState->radius < 0) {
        summonTabletState->radius = 0;
        state++;
        timer = -1;
      }
      break;
    case TABLET_LEADER_SQUASHED:
      if (timer == 0)
        Actor_StartScriptFar(owner, gMovementType_SquashScript);
      if ((timer & 3) == 0)
        Sound_PlayFar(SE_MAP_TRANSITION);
      summonTabletState->state = RUNES_GATHER;
      if (timer == 60)
        state = FIELD_PSYNERGY_FINISHED;
      break;
    }
    for (i = 0; i < newPebbles; i++) {
      angle = Random16();
      piece = &summonTabletState->pebbles[nextPebble];
      piece->timer = 0;
      piece->pebble.position[0] = target->x.fixed;
      piece->pebble.position[1] = target->y.fixed + toFIXED(12);
      piece->pebble.position[2] = target->z.fixed;
      Vector_AddPolarOffset(Random16() * 4 + toFIXED(2), angle,
                            piece->pebble.position);
      piece->pebble.velocity[0] = 0;
      piece->pebble.velocity[1] = toFIXED(9) / 2;
      piece->pebble.velocity[2] = 0;
      Vector_AddPolarOffset(
          Random16() * 2 + toFIXED(2), angle,
          summonTabletState->pebbles[nextPebble].pebble.velocity);
      nextPebble = (nextPebble + 1) & (TABLET_PEBBLE_COUNT - 1);
    }
    newPebbles = 0;
    wait(1);
    timer++;
  } while (state != FIELD_PSYNERGY_FINISHED);
  Actor_StartIdleFar(owner);
  owner->scaleX = FIXED_ONE;
  owner->scaleY = FIXED_ONE;
  Event_Wait(10);
  Task_Remove(SummonTablet_UpdateRunes);
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(runeEntry);
  VramCache_FreeEntry(pebbleEntry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// A flag block: a puddle's actor and the flag that says it is frozen, in a
/// list ended by ACTOR_LIST_END (FlagBlock_PlaceAll).
struct ActorFlag {
  u16 actor;
  s16 flag;
};

void FlagBlock_ApplyFlags(struct ActorFlag *list);

/// Sets up a scene's flag blocks, the puddles Frost turns into pillars of ice
/// (actor and flag pairs, ended by ACTOR_LIST_END): each stops being solid and
/// blocks its cell; one whose flag is set shows the ice pillar and has the
/// flag cleared, the others the puddle. Then FlagBlock_ApplyFlags raises the
/// frozen ones and sets their flags again, and the leader is lifted onto the
/// ground if it now lies below it. A scene with such puddles calls it as it
/// starts.
void FlagBlock_PlaceAll(struct ActorFlag *actors) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != ACTOR_LIST_END; actors++) {
    struct Actor *actor = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Actor_GetCellIndex(actor);
    struct MapCell *cell = &map->layers[actor->layer].cells[slot];

    actor->motionFlags = 0;
    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlagsFar(actor, 0);
    cell->collisionCode = CELL_BEHAVIOR_WALL;
    if (GameFlag_Get(flag)) {
      Actor_SelectAnimationFar(actor, PUDDLE_ANIMATION_FROZEN);
      GameFlag_Clear(flag);
    } else {
      Actor_SelectAnimationFar(actor, PUDDLE_ANIMATION_PUDDLE);
    }
  }
  FlagBlock_ApplyFlags(list);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  height = MapGrid_GetTerrainHeightFar(leader->layer, leader->x.fixed,
                                       leader->z.fixed);
  if (leader->y.fixed < height)
    leader->y.fixed = leader->groundHeight = height;
}

/// Marks the cell at (x, z) MAP_CELL_JUMP, a gap the leader jumps across, where
/// no ground lies above height base.
void MapGrid_MarkJumpIfOpen(FIXED x, FIXED z, s32 layer, FIXED base) {
  if (MapGrid_GetTerrainHeightAboveFar(x, z, layer, base) == 0)
    MapGrid_SetCellAttributesAt(
        x, z, layer, MapGrid_GetCellAttributesAt(x, z, layer) | MAP_CELL_JUMP);
}
