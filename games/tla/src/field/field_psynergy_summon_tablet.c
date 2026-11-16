/// @file
/// A summon tablet read: its pictures cycling, its motes and runes flying, and
/// placing the flag blocks.
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
void SummonTablet_CyclePictures(struct Actor *object) {
  struct Sprite *sprite = object->sprite;
  struct SpriteAnimation *layer = sprite->entries[0];

  switch (object->timer & 3) {
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
  object->timer++;
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

    if ((nextDx ^ dx) & 0x80000000)
      chase->x = chase->targetX;
    if ((dz ^ nextDz) & 0x80000000)
      chase->z = chase->targetZ;
  }
}

/// One mote or spark and a frame count, negative before it starts.
struct ChasingMote {
  union {
    /// A mote: a point on the screen chasing its target
    /// (Motion_StepTowardTarget's struct Chase).
    struct {
      FIXED x;
      FIXED z;
      FIXED unusedAfterZ; ///< no code names it
      FIXED targetX;
      FIXED targetZ;
      FIXED unusedAfterTargetZ; ///< no code names it
    };
    /// A spark: a position in the world and its velocity
    /// (Field_AdvanceWithGravity3D's vector).
    struct {
      FIXED position[3];
      FIXED velocity[3];
    } spark;
  };
  s32 timer;
};

/// The sparks that fly off the tablet, a power of two: the next wraps by a
/// mask.
#define TABLET_SPARK_COUNT 32
/// The tablet's runes, flown as motes in three rows of 16.
#define TABLET_MOTE_COUNT 48
#define TABLET_MOTE_ROW 16

/// The summon tablet's work (heap slot battleEffect): the decoded mote
/// pictures, the sparks and motes that fly between the tablet and the leader,
/// and the circle they gather on.
struct MoteWork {
  u8 unused[0x800]; ///< no code names it
  struct ChasingMote sparks[TABLET_SPARK_COUNT];
  struct EffectSprite sparkSprites[TABLET_SPARK_COUNT];
  struct ChasingMote motes[TABLET_MOTE_COUNT];
  struct EffectSprite moteSprites[TABLET_MOTE_COUNT];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

/// Task of SummonTablet_PlayRunes: moves and draws the motes.
void SummonTablet_UpdateMotes(void) {
  struct MoteWork *work = Ram_HeapSlots->battleEffect;
  s32 position[3];
  struct ChasingMote *mote;
  struct EffectSprite *sprite;
  s32 i;
  u16 *tile;

  if (work->moving != 0) {
    position[0] = work->originX;
    position[2] = work->originZ;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    mote = work->motes;
    mote->targetX = position[0];
    mote->targetZ = position[2];
    work->angle += TRIG_TURN * 3 / 128;
    for (i = 0; i < TABLET_MOTE_COUNT; i++, mote++) {
      if (mote->timer >= 0) {
        if (i != 0) {
          mote->targetX = position[0];
          mote->targetZ = position[2];
        }
        position[0] = mote->x;
        position[2] = mote->z;
        Vector_AddPolarOffset(work->spacing, 0, position);
      }
      mote->timer++;
    }
  }

  mote = work->motes;
  sprite = work->moteSprites;
  for (i = TABLET_MOTE_COUNT - 1; i >= 0; i--, sprite++, mote++) {
    if (work->state == 0) {
      if (mote->timer >= 0) {
        Motion_StepTowardTarget(mote, toFIXED(4));
        sprite->x = mote->x;
        sprite->y = mote->z;
        EffectSprite_Draw(sprite);
      }
      mote->timer++;
    } else if (work->state == 2) {
      Motion_StepTowardTarget(mote, toFIXED(6));
      sprite->x = mote->x;
      sprite->y = mote->z;
      EffectSprite_Draw(sprite);
    } else if (work->state == 3) {
      if (mote->x != work->targetX || mote->z != work->targetZ) {
        Motion_StepTowardTarget(mote, toFIXED(6));
        sprite->x = mote->x;
        sprite->y = mote->z;
        EffectSprite_Draw(sprite);
      }
    }
  }

  mote = work->sparks;
  tile = &work->sparkTile;
  sprite = work->sparkSprites;
  for (i = TABLET_SPARK_COUNT - 1; i >= 0; i--, sprite++, mote++) {
    if ((u32)mote->timer < 20) {
      sprite->oam.index = *tile + (mote->timer / 5 & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, mote->spark.position);
      Field_AdvanceWithGravity3D(mote->spark.position, 63, -(toFIXED(1) / 2));
      mote->timer++;
    }
  }
}

extern const s32 gFieldEffect_TargetColors[];
extern const s32 gSummonTablet_SquashScript[];
extern const u8 gMovementType_SquashScript[];

/// Plays a summon tablet's reading (a treasure of type 132, "examined the
/// stone tablet"): the tablet's runes and pebbles fly as motes between it
/// and the leader, with their sounds, before Summon_ShowLearnedMessage.
void SummonTablet_PlayRunes(s32 targetId) {
  struct MoteWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct MoteWork));
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_Lookup(ownerId);
  struct Actor *target = Actor_Lookup(targetId);
  s32 state;
  s32 moteEntry;
  s32 sparkEntry;
  FIXED ownerX;
  FIXED ownerZ;
  FIXED targetX;
  FIXED targetZ;
  s32 sparks = 0;
  s32 nextSpark = 0;
  FIXED pos[3];
  struct EffectSprite *sprite;
  struct ChasingMote *mote;
  s32 moteTile;
  s32 sparkTile;
  s32 timer;
  s32 wave;
  s32 delay;
  s32 angle;
  s32 i;
  FIXED startX = 130 * FIXED_ONE;
  FIXED startZ = 30 * FIXED_ONE;

  Field_StartColorTransition(8, gFieldEffect_TargetColors[gFrameCount & 7]);
  pos[0] = owner->x.fixed;
  pos[1] = owner->y.fixed + 24 * FIXED_ONE;
  pos[2] = owner->z.fixed;
  Camera_WorldToScreen(pos);
  ownerX = pos[0];
  ownerZ = pos[2];
  work->targetX = ownerX;
  work->targetZ = ownerZ;
  pos[0] = target->x.fixed;
  pos[1] = target->y.fixed;
  pos[2] = target->z.fixed;
  Camera_WorldToScreen(pos);
  targetX = pos[0];
  targetZ = pos[2];
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_RUNES), work);
  moteEntry = VramCache_FindFreeEntry();
  moteTile = VramCache_Load(moteEntry, 0x800, work);
  mote = work->motes;
  i = 0;
  sprite = work->moteSprites;
  for (; i < TABLET_MOTE_COUNT; i++) {
    EffectSprite_Init(sprite, 4, 4, 0,
                      moteTile + ((Random16() << 4) >> 16) * 2);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 1;
    sprite->depth = OAM_DEPTH_WINDOW;
    mote->x = targetX;
    mote->z = targetZ;
    mote->targetX = ((i % TABLET_MOTE_ROW) * 3 << 18) + toFIXED(30);
    mote->targetZ = ((i / TABLET_MOTE_ROW) * 3 << 19) + toFIXED(30);
    mote->timer = -i;
    sprite++;
    mote++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PEBBLES), work);
  sparkEntry = VramCache_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 0x800, work);
  work->sparkTile = sparkTile;
  mote = work->sparks;
  sprite = work->sparkSprites;
  for (i = TABLET_SPARK_COUNT - 1; i >= 0; i--) {
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, sparkTile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    mote->timer = -1;
    sprite++;
    mote++;
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
  work->moving = 0;
  Task_Add(SummonTablet_UpdateMotes, TASK_PASS_VBLANK);
  state = 0;
  timer = 0;
  do {
    switch (state) {
    case 0:
      work->state = 0;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Sound_PlayFar(SE_IMPACT);
        sparks = 1;
      }
      if (timer == 40) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        sparks = 25;
        Actor_StartIdleFar(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case 1:
      for (i = 0; i < TABLET_MOTE_COUNT; i++) {
        struct EffectSprite *rune;
        wave = i % TABLET_MOTE_ROW + i / TABLET_MOTE_ROW;
        wave = timer - wave;
        rune = &work->moteSprites[i];
        if (wave == 0)
          rune->oam.index += 32;
        if (wave == 3)
          rune->oam.index += -32;
        if (wave == 8)
          rune->oam.index += 32;
        if (wave == 10)
          rune->oam.index += -32;
        if (wave == 16)
          rune->oam.index += 32;
        if (wave == 18)
          rune->oam.index += -32;
      }
      if (timer == 0)
        Sound_PlayFar(SE_GLITTER);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case 2:
      if (timer == 0) {
        Actor_FaceDirectionById(ownerId, FACING_SOUTH, 0);
        break;
      }
      if (owner->script[owner->scriptCursor] == SCRIPT_CMD_END) {
        state++;
        timer = -1;
      }
      if (timer == 60) {
        state++;
        timer = -1;
      }
      break;
    case 3:
      if (timer == 0)
        Actor_SelectAnimationFar(owner, ANIM_LEADER_ARMS_RAISED);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case 4:
      if (timer == 0) {
        mote = work->motes;
        delay = 0;
        for (i = TABLET_MOTE_COUNT - 1; i >= 0; i--) {
          mote->timer = delay;
          delay -= 3;
          mote++;
        }
        work->radius = 60 * FIXED_ONE;
        work->angle = TRIG_TURN * 3 / 4;
        work->originX = startX;
        work->originZ = startZ;
        work->moving = 1;
        work->spacing = 4 * FIXED_ONE;
        work->state = 2;
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
    case 5:
      work->originX = timer * (ownerX - startX) / 30 + startX;
      work->originZ = timer * (ownerZ - startZ) / 30 + startZ;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case 6:
      if (timer == 0)
        work->spacing = 0;
      work->radius += -2 * FIXED_ONE;
      if (work->radius < 0) {
        work->radius = 0;
        state++;
        timer = -1;
      }
      break;
    case 7:
      if (timer == 0)
        Actor_StartScriptFar(owner, gMovementType_SquashScript);
      if ((timer & 3) == 0)
        Sound_PlayFar(SE_MAP_TRANSITION);
      work->state = 3;
      if (timer == 60)
        state = FIELD_PSYNERGY_FINISHED;
      break;
    }
    for (i = 0; i < sparks; i++) {
      angle = Random16();
      mote = &work->sparks[nextSpark];
      mote->timer = 0;
      mote->spark.position[0] = target->x.fixed;
      mote->spark.position[1] = target->y.fixed + toFIXED(12);
      mote->spark.position[2] = target->z.fixed;
      Vector_AddPolarOffset(Random16() * 4 + toFIXED(2), angle,
                            mote->spark.position);
      mote->spark.velocity[0] = 0;
      mote->spark.velocity[1] = toFIXED(9) / 2;
      mote->spark.velocity[2] = 0;
      Vector_AddPolarOffset(Random16() * 2 + toFIXED(2), angle,
                            work->sparks[nextSpark].spark.velocity);
      nextSpark = (nextSpark + 1) & (TABLET_SPARK_COUNT - 1);
    }
    sparks = 0;
    wait(1);
    timer++;
  } while (state != FIELD_PSYNERGY_FINISHED);
  Actor_StartIdleFar(owner);
  owner->scaleX = FIXED_ONE;
  owner->scaleY = FIXED_ONE;
  Event_Wait(10);
  Task_Remove(SummonTablet_UpdateMotes);
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(moteEntry);
  VramCache_FreeEntry(sparkEntry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// A flag block: its actor and the flag that says it has been moved, in a list
/// ended by actor 0xffff (FlagBlock_PlaceAll).
struct ActorFlag {
  u16 actor;
  s16 flag;
};

void FlagBlock_ApplyFlags(struct ActorFlag *list);

/// Sets up a scene's flag blocks, the puddles Frost turns into pillars of ice
/// (actor and flag pairs, ended by actor 0xffff): each stops being solid and
/// blocks its cell; one whose flag is set takes animation 0 and has the flag
/// cleared, the others animation 1. Then FlagBlock_ApplyFlags applies them and
/// the leader is lifted onto the ground if it now lies below it. Daila,
/// Kibombo, the Izumo ruins, Air's Rock and the Lemurian Ship call it as they
/// start.
void FlagBlock_PlaceAll(struct ActorFlag *actors) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Actor_GetCellIndex(object);
    struct MapCell *cell = &map->layers[object->layer].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSpriteFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    if (GameFlag_Get(flag)) {
      Actor_SelectAnimationFar(object, 0);
      GameFlag_Clear(flag);
    } else {
      Actor_SelectAnimationFar(object, 1);
    }
  }
  FlagBlock_ApplyFlags(list);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  height = MapGrid_GetTerrainHeightFar(leader->layer, leader->x.fixed,
                                       leader->z.fixed);
  if (leader->y.fixed < height)
    leader->y.fixed = leader->groundHeight = height;
}

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_RunDrainMotes(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

/// Sets attribute bit 0x80 (raised) on the cell at (x, z) where no ground lies
/// above height base.
void MapGrid_MarkRaisedIfOpen(FIXED x, FIXED z, s32 layer, FIXED base) {
  if (MapGrid_GetTerrainHeightAboveFar(x, z, layer, base) == 0)
    MapGrid_SetAttributeBAt(x, z, layer,
                            MapGrid_GetAttributeBAt(x, z, layer) | 0x80);
}
