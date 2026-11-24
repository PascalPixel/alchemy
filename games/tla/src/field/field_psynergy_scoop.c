/// @file
/// Scoop: the giant hand digging, the actor dropped from it and the item dug
/// up.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_state.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "system/bg_scroll.h"

#include "sprite_ids.h"

/// A dig at nothing draws the first SCOOP_DRAWN_DUST of the particles of dust
/// and sparkles Scoop throws up, a dig on a target the first
/// SCOOP_DRAWN_TARGET_DUST.
#define SCOOP_DRAWN_DUST 18
#define SCOOP_DRAWN_TARGET_DUST 20
/// The sparkles the hand bursts into when it cannot dig.
#define SCOOP_SPARKLES 16
/// A dig at nothing turns something up when Random16() falls below this, one
/// time in sixteen; what turns up is an item below SCOOP_ITEM_CHANCE, three
/// times in four, and coins otherwise.
#define SCOOP_FIND_CHANCE (FIXED_ONE / 16)
#define SCOOP_ITEM_CHANCE (FIXED_ONE * 3 / 4)

/// The kinds of dig (ScoopState.mode).
enum ScoopMode {
  SCOOP_EARTH = 0,  ///< at nothing: earth is thrown up
  SCOOP_FINDS = 1,  ///< at nothing, one time in sixteen: something turns up
  SCOOP_TARGET = 2, ///< on a target: the place's Scoop event runs
  SCOOP_SPARKLE = 3 ///< at nothing where lit: sparkles rise
};

/// The stages of a dig (ScoopState.phase).
enum ScoopPhase {
  SCOOP_DIGS = 0,
  SCOOP_HAND_RISES = 1,
  SCOOP_SETTLES = 2 ///< what was dug up shows, then the dig ends
};

/// Why the hand cannot dig.
enum ScoopFailure {
  SCOOP_CAN_DIG = 0,
  SCOOP_ACTOR_IN_WAY = 1,
  SCOOP_OUT_OF_REACH = 2,
  SCOOP_HARD_GROUND = 3,
  SCOOP_WALL = 4
};

/// Moves the state on to its next phase, its timer restarted.
#define NEXT_PHASE(state) ((state)->phase++, (state)->timer = -1)

/// Task of the Scoop: the hand digs and rises, then the earth, sparkles or
/// dust it threw up show, each kind of dig at its own pace.
void Scoop_Update(void) {
  struct ScoopState *state;
  struct Actor *hand;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  hand = state->actor;
  if (state->mode == SCOOP_EARTH) {
    switch (state->phase) {
    case SCOOP_DIGS:
      if (state->timer == 1)
        state->other->scaleX = FIXED_ONE;
      if (state->timer == 4)
        NEXT_PHASE(state);
      break;
    case SCOOP_HAND_RISES:
      if (state->timer == 8)
        Actor_SelectAnimationFar(hand, 3);
      if (state->timer <= 15)
        hand->y.fixed += toFIXED(23) / 10;
      if (state->timer == 28)
        NEXT_PHASE(state);
      break;
    case SCOOP_SETTLES:
      if (state->timer == 25)
        state->other->scaleX = 0;
      if (state->timer == 50)
        state->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    phase = &state->phase;
    if (*phase == SCOOP_SETTLES) {
      picture = state->picture;
      vram = (u8 *)OBJ_VRAM0 + state->pictureTile * TILE_SIZE_4BPP;
      Picture_ShiftColumnsByWave(state->tiles, picture, state->timer + 14);
      ((const u8 *(*)(const u8 *, s32, s32,
                      void *))Ram_HeapSlots->pictureUploader)(
          picture, SCOOP_PICTURE_SIZE, SCOOP_PICTURE_SIZE, vram);
      sprite = &state->pictureSprite;
      EffectSprite_Draw(sprite);
    }
    if (*phase == SCOOP_SETTLES) {
      for (i = 0, particle = state->particles; i < SCOOP_DRAWN_DUST;
           i++, particle++) {
        sprite = &state->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                   state->particleTile);
        particle->timer++;
      }
    }
  } else if (state->mode == SCOOP_SPARKLE) {
    switch (state->phase) {
    case SCOOP_DIGS:
      if (state->timer == 1)
        state->other->scaleX = FIXED_ONE;
      if (state->timer == 4)
        NEXT_PHASE(state);
      break;
    case SCOOP_HAND_RISES:
      if (state->timer == 8)
        Actor_SelectAnimationFar(state->actor, 3);
      if (state->timer <= 15)
        hand->y.fixed += toFIXED(23) / 10;
      if (state->timer == 28)
        NEXT_PHASE(state);
      break;
    case SCOOP_SETTLES:
      if (state->timer == 70)
        state->other->scaleX = 0;
      if (state->timer == 90)
        state->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    if (state->phase == SCOOP_SETTLES) {
      for (i = 0, particle = state->particles; i < SCOOP_PARTICLE_COUNT;
           i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &state->sprites[i];
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (particle->timer < 42) {
            particle->y -= FIXED_ONE;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
      }
    }
  } else if (state->mode == SCOOP_FINDS) {
    switch (state->phase) {
    case SCOOP_DIGS:
      if (state->timer == 8)
        NEXT_PHASE(state);
      break;
    case SCOOP_HAND_RISES:
      if (state->timer == 0)
        Actor_SelectAnimationFar(state->actor, 4);
      hand->y.fixed += toFIXED(23) / 10;
      if (state->timer == 15)
        NEXT_PHASE(state);
      break;
    case SCOOP_SETTLES:
      if (state->timer == 20)
        state->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
  } else {
    switch (state->phase) {
    case SCOOP_DIGS:
      if (state->timer == 8)
        NEXT_PHASE(state);
      break;
    case SCOOP_HAND_RISES:
      if (state->timer == 0)
        Actor_SelectAnimationFar(state->actor, 4);
      hand->y.fixed += toFIXED(23) / 10;
      if (state->timer == 15)
        NEXT_PHASE(state);
      break;
    case SCOOP_SETTLES:
      if (state->timer == 30)
        state->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    particle = state->particles;
    sprite = state->sprites;
    for (i = 0; i < SCOOP_DRAWN_TARGET_DUST; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 state->particleTile);
      particle->timer++;
    }
  }
  state->timer++;
}

/// Scoop cast on a target: runs FieldPsynergy_Scoop.
void FieldPsynergy_ScoopOnTarget(void) { FieldPsynergy_Scoop(); }

s32 MapGrid_CanScoop(s32 x, s32 z, s32 layer);
void Scoop_DigUpItem(void);

/// Scoop ("Dig in soft ground."): a giant hand rises before the caster and
/// digs the cell 16 pixels ahead. With an actor in the way, out of reach or on
/// ground that cannot be dug it stops with the error sound; else earth (or, in
/// a lit place, sparkles) is thrown up and the place's Scoop event runs.
void FieldPsynergy_Scoop(void) {
  struct ScoopState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*state));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *caster = scene->mainActor;
  struct Actor *boulder = NULL;
  struct Actor *hand;
  struct Actor *target;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  void *pending;
  void *data;
  s32 particleEntry = VRAM_CACHE_ENTRY_COUNT;
  s32 particleTile;
  s32 pictureEntry = VRAM_CACHE_ENTRY_COUNT;
  s32 pictureTile;
  FIXED pos[3];
  s32 failed;
  s8 *lit;
  s32 stage;
  s32 frame;
  s32 i;

  if (field->mode != ACTOR_SYSTEM_WORLD_MAP) {
    SET_POSITION(scene, (caster->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
                 caster->y.fixed,
                 (caster->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
    Vector_AddPolarOffset(toFIXED(16), scene->angle, &scene->x);
  }
  FieldPsynergy_FadeToCastColor();
  hand = Actor_Spawn(SPRITE_SCOOP_HAND, 0, 0, 0);
  state->actor = hand;
  if (hand != NULL) {
    hand->motionFlags = 0;
    if (field->psynergyDrawFlags & PSYNERGY_DRAW_FIXED_PRIORITY) {
      if ((hand->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
          ACTOR_SPRITE_ONE) {
        ((struct EffectSprite *)hand->sprite)->oam.priority = 1;
        hand->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      }
    } else {
      hand->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    hand->facing = caster->facing;
    Actor_SelectAnimationFar(hand, 0);
    lit = &scene->lit;
    if (*lit != 0)
      Actor_SetSpriteColorModeFar(hand, SPRITE_COLOR_GREY);
    if (*lit == 0)
      boulder = Actor_Spawn(SPRITE_BROWN_BOULDER, scene->x, scene->y, scene->z);
    else
      boulder = Actor_Spawn(SPRITE_GREY_BOULDER, scene->x, scene->y, scene->z);
    state->other = boulder;
    if (boulder != NULL) {
      boulder->z.fixed += toFIXED(3);
      boulder->motionFlags = 0;
      boulder->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
      boulder->scaleX = 0;
      if (!(field->psynergyDrawFlags & PSYNERGY_DRAW_NO_SCENE_SPRITE))
        FieldPsynergy_StartSceneSprite();
      Sound_PlayFar(SE_ITEM_BREAK);
      FieldPsynergy_GrowFromCaster(hand, scene->x, scene->y + toFIXED(32),
                                   scene->z);
      wait(10);
      Actor_SelectAnimationFar(hand, 1);
      wait(15);
      for (i = 0; i < 10; i++) {
        hand->y.fixed += -(toFIXED(5) / 2);
        wait(1);
      }
      Actor_SelectAnimationFar(hand, 2);
      Sound_PlayFar(SE_WATER);

      failed = SCOOP_CAN_DIG;
      for (i = 0; i < FIELD_ACTOR_COUNT; i++) {
        target = field->actors[i];
        if (target != NULL && target->script != NULL &&
            (Actor_GetSpriteIdFar(target) == SPRITE_STAKE ||
             (target->collisionFlags & ACTOR_SOLID)) &&
            target != scene->mainActor &&
            ((struct Sprite *)target->sprite)->count != 0 &&
            FieldPsynergy_CheckOverlap(&scene->x, 4, &target->x.fixed,
                                       target->radius - 2) >= 0) {
          failed = SCOOP_ACTOR_IN_WAY;
          break;
        }
      }
      if (MapGrid_GetCellBehavior(scene->x, scene->z, caster->layer) ==
          CELL_BEHAVIOR_WALL)
        failed = SCOOP_WALL;
      if (Field_MeasurePsynergyHeight(scene->fieldPsynergy, caster,
                                      &scene->x) == -1)
        failed = SCOOP_OUT_OF_REACH;
      if (MapGrid_CanScoop(scene->x, scene->z, caster->layer) == 0)
        failed = SCOOP_HARD_GROUND;
      if (failed) {
        wait(2);
        for (i = 0; i < SCOOP_SPARKLES; i++)
          BattleEffect_SpawnScatterParticle(hand->x.fixed, hand->y.fixed,
                                            hand->z.fixed);
        Sound_PlayFar(SE_MENU_ERROR);
        wait(1);
      } else {

        if (scene->enabled != 0) {
          if (Random16() < SCOOP_FIND_CHANCE)
            state->mode = SCOOP_FINDS;
          else if (*lit == 0)
            state->mode = SCOOP_EARTH;
          else
            state->mode = SCOOP_SPARKLE;
        } else {
          state->mode = SCOOP_TARGET;
        }

        if (state->mode == SCOOP_EARTH) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A),
                              state);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 2048, state);
          state->particleTile = particleTile;
          i = 0;
          particle = state->particles;
          for (; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
            sprite = &state->sprites[i];
            EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = Actor_GetSpritePriority(boulder);
            sprite->depth = Actor_GetSpriteDepth(boulder) + 1;
            sprite->scaleY = sprite->scaleX = FIXED_ONE / 5;
            SET_POSITION(particle, scene->x, scene->y, scene->z);
            Vector_AddPolarOffset(toFIXED(2), Random16(), &particle->x);
            particle->velocityX = 0;
            particle->velocityY = 0;
            particle->velocityZ = 0;
            Vector_AddPolarOffset(toFIXED(1.2), Random16(),
                                  &particle->velocityX);
            particle->timer = -i * 2;
          }
        } else if (state->mode == SCOOP_TARGET) {
          if (*lit == 0)
            data = Resource_GetTableEntry(RESOURCE_DUST_PUFF_A);
          else
            data = Resource_GetTableEntry(RESOURCE_DUST_PUFF_B);
          Decompress_TaggedLz(data, state);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 2048, state);
          state->particleTile = particleTile;
          i = 0;
          particle = state->particles;
          for (; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
            sprite = &state->sprites[i];
            EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = Actor_GetSpritePriority(boulder);
            sprite->depth = Actor_GetSpriteDepth(boulder) + 1;
            SET_POSITION(particle, scene->x, scene->y, scene->z);
            Vector_AddPolarOffset(toFIXED(2), Random16(), &particle->x);
            particle->velocityX = 0;
            particle->velocityY = 0;
            particle->velocityZ = 0;
            Vector_AddPolarOffset(toFIXED(1), Random16(), &particle->velocityX);
            particle->timer = -i;
          }
        } else if (state->mode == SCOOP_SPARKLE) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHITE_SPARKLE),
                              state);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 128, state);
          state->particleTile = particleTile;
          i = 0;
          particle = state->particles;
          sprite = state->sprites;
          for (; i < SCOOP_PARTICLE_COUNT; i++) {
            EffectSprite_Init(sprite, 4, 4, 0, particleTile + (i & 1) * 2);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = Actor_GetSpritePriority(hand);
            sprite->depth = Actor_GetSpriteDepth(hand) - 1;
            SET_POSITION(particle, scene->x, scene->y + toFIXED(40), scene->z);
            Vector_AddPolarOffset(Random16() * 4 + toFIXED(3), Random16(),
                                  &particle->x);
            particle->timer = -i * 3 / 4;
            sprite++;
            particle++;
          }
        }

        if (state->mode == SCOOP_EARTH) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SCOOP_EARTH),
                              state);
          pictureEntry = VramCache_FindFreeEntry();
          pictureTile = VramCache_Load(pictureEntry, 1024, NULL);
          state->pictureTile = pictureTile;
          sprite = &state->pictureSprite;
          EffectSprite_Init(sprite, 16, 0, OBJ_SIZE_32X32, pictureTile);
          sprite->depth = Actor_GetSpriteDepth(scene->mainActor);
          sprite->oam.priority = Actor_GetSpritePriority(scene->mainActor);
          sprite->oam.fullColor = 1;
          sprite->oam.palette = 0;
          pos[0] = hand->x.fixed;
          pos[1] = hand->y.fixed + toFIXED(32);
          pos[2] = hand->z.fixed;
          Camera_WorldToScreen(pos);
          sprite->x = pos[0];
          sprite->y = pos[2];
        }
        if (state->mode == SCOOP_EARTH || state->mode == SCOOP_SPARKLE) {
          wait(6);
          state->other->scaleX = FIXED_ONE;
          wait(4);
        } else {
          wait(10);
        }

        state->phase = SCOOP_DIGS;
        state->timer = 0;
        {
          s32 order = TASK_PASS_FRAME;

          Task_Add(Scoop_Update, order);
        }
        if (state->mode == SCOOP_FINDS) {
          frame = 0;
          while (state->phase != FIELD_PSYNERGY_FINISHED) {
            if (frame == 24)
              Scoop_DigUpItem();
            wait(1);
            frame++;
          }
        } else {
          pending = Scene_FindPsynergyTrigger(
              PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
              Scene_SelectPsynergyTarget(scene->fieldPsynergy));
          frame = 0;
          while (state->phase != FIELD_PSYNERGY_FINISHED) {
            stage = 0;
            wait(1);
            if (frame == SCOOP_DUG_FRAME)
              stage = 1;
            if (frame == 50)
              stage = 2;
            scene->stage = stage;
            scene->frame = frame;
            Field_RunPsynergyEvent(pending, stage, frame);
            frame++;
          }
          Field_RunPsynergyEvent(pending, FIELD_PSYNERGY_FINISHED, frame);
        }
        Task_Remove(Scoop_Update);
        Sound_PlayFar(SE_MENU_ERROR);
        FieldPsynergy_ShrinkToCaster(hand, hand->x.fixed, hand->y.fixed,
                                     hand->z.fixed);
        if (particleEntry != VRAM_CACHE_ENTRY_COUNT)
          VramCache_FreeEntry(particleEntry);
        if (pictureEntry != VRAM_CACHE_ENTRY_COUNT)
          VramCache_FreeEntry(pictureEntry);
      }
    }
  }
  Actor_ReleaseFar(hand);
  Actor_ReleaseFar(boulder);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Drops an actor from Scoop's hand (if there is one) to fall to the ground,
/// waits 50 frames, then asks for its talk, which opens it without its
/// "checked the ..." message (FIELD_TALK_SILENT).
void Scoop_DropFromHand(s32 id) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *hand;
  struct Actor *actor;

  hand = ((struct ScoopState *)Ram_HeapSlots->battleEffect)->actor;
  actor = Actor_GetById(id);
  if (hand != NULL) {
    SET_ACTOR_POSITION(actor, hand->x.fixed, hand->y.fixed, hand->z.fixed);
  }
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = 0;
  } else {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
  actor->gravity = toFIXED(9) / 100;
  wait(50);
  field->talk = id | FIELD_TALK_SILENT;
}

struct Unit *Unit_Get(u32 unitId);
extern const u16 gScoop_DugItems[16];

/// SCOOPED_ITEM_ACTOR at the hand, fetched or spawned (SPRITE_UNSEEN_MARKER),
/// shows what Scoop dug up: three times in four one of gScoop_DugItems's
/// sixteen items (a coin when there is no room for it), else coins up to twice
/// the leader's level; it then drops (Scoop_DropFromHand).
void Scoop_DigUpItem(void) {
  struct ScoopState *state = Ram_HeapSlots->battleEffect;
  struct Actor *hand;
  struct FieldState *field;
  struct Unit *unit;
  struct Actor *actor;
  s32 item;
  s32 id = SCOOPED_ITEM_ACTOR;

  hand = state->actor;
  field = Ram_HeapSlots->fieldState;
  unit = Unit_Get(gMainRoutineState.leaderActorId);

  actor = Actor_GetById(id);
  if (actor == NULL) {
    actor = Actor_CreateFar(SPRITE_UNSEEN_MARKER, hand->x.fixed, hand->y.fixed,
                            hand->z.fixed);
    if (actor == NULL)
      return;
    field->actors[id] = actor;
  }
  if (Random16() < SCOOP_ITEM_CHANCE) {
    item = gScoop_DugItems[Random_Below(ARRAY_COUNT(gScoop_DugItems))];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = SCOOPED_COINS | 1;
  } else {
    item = (Random_Below(unit->level * 2) + 1) | SCOOPED_COINS;
  }
  SET_ACTOR_POSITION(actor, hand->x.fixed, hand->y.fixed, hand->z.fixed);
  Actor_ShowItemIconById(id, item);
  field->scoopedItem = item;
  Scoop_DropFromHand(id);
}
#undef NEXT_PHASE
#undef SCOOP_DRAWN_DUST
#undef SCOOP_DRAWN_TARGET_DUST
#undef SCOOP_PICTURE_SIZE
#undef SCOOP_SPARKLES
#undef SCOOP_FIND_CHANCE
#undef SCOOP_ITEM_CHANCE
