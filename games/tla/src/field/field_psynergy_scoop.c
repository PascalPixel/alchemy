/// @file
/// Scoop: digging, the object dropped at the effect's anchor and the item dug
/// up.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_work.h"
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

/// Moves the work on to its next phase, its timer restarted.
#define NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of the Scoop: plays the dig on the anchor phase by phase.
void Scoop_Update(void) {
  struct ScoopWork *work;
  struct Actor *anchor;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  anchor = work->actor;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = FIXED_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SelectAnimationFar(anchor, 3);
      if (work->timer <= 15)
        anchor->y.fixed += toFIXED(23) / 10;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    phase = &work->phase;
    if (*phase == 2) {
      picture = work->picture;
      vram = (u8 *)OBJ_VRAM0 + work->pictureTile * TILE_SIZE_4BPP;
      Picture_ShiftColumnsByWave(work->tiles, picture, work->timer + 14);
      ((const u8 *(*)(const u8 *, s32, s32,
                      void *))Ram_HeapSlots->pictureUploader)(
          picture, SCOOP_PICTURE_SIZE, SCOOP_PICTURE_SIZE, vram);
      sprite = &work->pictureSprite;
      EffectSprite_Draw(sprite);
    }
    if (*phase == 2) {
      for (i = 0, particle = work->particles; i < SCOOP_DRAWN_DUST;
           i++, particle++) {
        sprite = &work->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                   work->particleTile);
        particle->timer++;
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = FIXED_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SelectAnimationFar(work->actor, 3);
      if (work->timer <= 15)
        anchor->y.fixed += toFIXED(23) / 10;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    if (work->phase == 2) {
      for (i = 0, particle = work->particles; i < SCOOP_PARTICLE_COUNT;
           i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &work->sprites[i];
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
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SelectAnimationFar(work->actor, 4);
      anchor->y.fixed += toFIXED(23) / 10;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 20)
        work->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
  } else {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SelectAnimationFar(work->actor, 4);
      anchor->y.fixed += toFIXED(23) / 10;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = FIELD_PSYNERGY_FINISHED;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < SCOOP_DRAWN_TARGET_DUST; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
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
  struct ScoopWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *caster = scene->mainActor;
  struct Actor *other = NULL;
  struct Actor *actor;
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
  s8 *alternate;
  s32 stage;
  s32 frame;
  s32 i;

  if (event->mode != ACTOR_SYSTEM_WORLD_MAP) {
    SET_POSITION(scene, (caster->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
                 caster->y.fixed,
                 (caster->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
    Vector_AddPolarOffset(toFIXED(16), scene->angle, &scene->x);
  }
  FieldPsynergy_FadeToCastColor();
  actor = Actor_Spawn(SPRITE_SCOOP_HAND, 0, 0, 0);
  work->actor = actor;
  if (actor != NULL) {
    actor->motionFlags = 0;
    if (event->psynergyDrawFlags & 2) {
      if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
          ACTOR_SPRITE_ONE) {
        ((struct EffectSprite *)actor->sprite)->oam.priority = 1;
        actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      }
    } else {
      actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    actor->facing = caster->facing;
    Actor_SelectAnimationFar(actor, 0);
    alternate = &scene->lit;
    if (*alternate != 0)
      Actor_SetSpriteColorModeFar(actor, SPRITE_COLOR_GREY);
    if (*alternate == 0)
      other = Actor_Spawn(SPRITE_BROWN_BOULDER, scene->x, scene->y, scene->z);
    else
      other = Actor_Spawn(SPRITE_GREY_BOULDER, scene->x, scene->y, scene->z);
    work->other = other;
    if (other != NULL) {
      other->z.fixed += toFIXED(3);
      other->motionFlags = 0;
      other->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
      other->scaleX = 0;
      if (!(event->psynergyDrawFlags & 1))
        FieldPsynergy_StartSceneSprite();
      Sound_PlayFar(SE_ITEM_BREAK);
      FieldPsynergy_GrowFromCaster(actor, scene->x, scene->y + toFIXED(32),
                                   scene->z);
      wait(10);
      Actor_SelectAnimationFar(actor, 1);
      wait(15);
      for (i = 0; i < 10; i++) {
        actor->y.fixed += -(toFIXED(5) / 2);
        wait(1);
      }
      Actor_SelectAnimationFar(actor, 2);
      Sound_PlayFar(SE_WATER);

      failed = 0;
      for (i = 0; i < FIELD_ACTOR_COUNT; i++) {
        target = event->actors[i];
        if (target != NULL && target->script != NULL &&
            (Actor_GetSpriteIdFar(target) == SPRITE_STAKE ||
             (target->collisionFlags & ACTOR_SOLID)) &&
            target != scene->mainActor &&
            ((struct Sprite *)target->sprite)->count != 0 &&
            FieldPsynergy_CheckOverlap(&scene->x, 4, &target->x.fixed,
                                       target->radius - 2) >= 0) {
          failed = 1;
          break;
        }
      }
      if (MapGrid_GetCellBehavior(scene->x, scene->z, caster->layer) ==
          CELL_BEHAVIOR_WALL)
        failed = 4;
      if (Field_MeasurePsynergyHeight(scene->fieldPsynergy, caster,
                                      &scene->x) == -1)
        failed = 2;
      if (MapGrid_CanScoop(scene->x, scene->z, caster->layer) == 0)
        failed = 3;
      if (failed) {
        wait(2);
        for (i = 0; i < SCOOP_SPARKLES; i++)
          BattleEffect_SpawnScatterParticle(actor->x.fixed, actor->y.fixed,
                                            actor->z.fixed);
        Sound_PlayFar(SE_MENU_ERROR);
        wait(1);
      } else {

        if (scene->enabled != 0) {
          if (Random16() < SCOOP_FIND_CHANCE)
            work->mode = 1;
          else if (*alternate == 0)
            work->mode = 0;
          else
            work->mode = 3;
        } else {
          work->mode = 2;
        }

        if (work->mode == 0) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A),
                              work);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 2048, work);
          work->particleTile = particleTile;
          i = 0;
          particle = work->particles;
          for (; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
            sprite = &work->sprites[i];
            EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = BattleEffect_GetObjectPriority(other);
            sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
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
        } else if (work->mode == 2) {
          if (*alternate == 0)
            data = Resource_GetTableEntry(RESOURCE_DUST_PUFF_A);
          else
            data = Resource_GetTableEntry(RESOURCE_DUST_PUFF_B);
          Decompress_TaggedLz(data, work);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 2048, work);
          work->particleTile = particleTile;
          i = 0;
          particle = work->particles;
          for (; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
            sprite = &work->sprites[i];
            EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = BattleEffect_GetObjectPriority(other);
            sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
            SET_POSITION(particle, scene->x, scene->y, scene->z);
            Vector_AddPolarOffset(toFIXED(2), Random16(), &particle->x);
            particle->velocityX = 0;
            particle->velocityY = 0;
            particle->velocityZ = 0;
            Vector_AddPolarOffset(toFIXED(1), Random16(), &particle->velocityX);
            particle->timer = -i;
          }
        } else if (work->mode == 3) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHITE_SPARKLE),
                              work);
          particleEntry = VramCache_FindFreeEntry();
          particleTile = VramCache_Load(particleEntry, 128, work);
          work->particleTile = particleTile;
          i = 0;
          particle = work->particles;
          sprite = work->sprites;
          for (; i < SCOOP_PARTICLE_COUNT; i++) {
            EffectSprite_Init(sprite, 4, 4, 0, particleTile + (i & 1) * 2);
            sprite->oam.fullColor = 1;
            sprite->oam.palette = 0;
            sprite->oam.priority = BattleEffect_GetObjectPriority(actor);
            sprite->depth = BattleEffect_GetObjectDepth(actor) - 1;
            SET_POSITION(particle, scene->x, scene->y + toFIXED(40), scene->z);
            Vector_AddPolarOffset(Random16() * 4 + toFIXED(3), Random16(),
                                  &particle->x);
            particle->timer = -i * 3 / 4;
            sprite++;
            particle++;
          }
        }

        if (work->mode == 0) {
          Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SCOOP_EARTH),
                              work);
          pictureEntry = VramCache_FindFreeEntry();
          pictureTile = VramCache_Load(pictureEntry, 1024, NULL);
          work->pictureTile = pictureTile;
          sprite = &work->pictureSprite;
          EffectSprite_Init(sprite, 16, 0, OBJ_SIZE_32X32, pictureTile);
          sprite->depth = BattleEffect_GetObjectDepth(scene->mainActor);
          sprite->oam.priority =
              BattleEffect_GetObjectPriority(scene->mainActor);
          sprite->oam.fullColor = 1;
          sprite->oam.palette = 0;
          pos[0] = actor->x.fixed;
          pos[1] = actor->y.fixed + toFIXED(32);
          pos[2] = actor->z.fixed;
          Camera_WorldToScreen(pos);
          sprite->x = pos[0];
          sprite->y = pos[2];
        }
        if (work->mode == 0 || work->mode == 3) {
          wait(6);
          work->other->scaleX = FIXED_ONE;
          wait(4);
        } else {
          wait(10);
        }

        work->phase = 0;
        work->timer = 0;
        {
          s32 order = TASK_PASS_FRAME;

          Task_Add(Scoop_Update, order);
        }
        if (work->mode == 1) {
          frame = 0;
          while (work->phase != FIELD_PSYNERGY_FINISHED) {
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
          while (work->phase != FIELD_PSYNERGY_FINISHED) {
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
        FieldPsynergy_ShrinkToCaster(actor, actor->x.fixed, actor->y.fixed,
                                     actor->z.fixed);
        if (particleEntry != VRAM_CACHE_ENTRY_COUNT)
          VramCache_FreeEntry(particleEntry);
        if (pictureEntry != VRAM_CACHE_ENTRY_COUNT)
          VramCache_FreeEntry(pictureEntry);
      }
    }
  }
  Actor_ReleaseFar(actor);
  Actor_ReleaseFar(other);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Puts object id at the effect's anchor (if any) and lets it fall under
/// gravity, waits 50 frames, then asks for its talk, which opens it without its
/// "checked the ..." message (FIELD_TALK_SILENT).
void BattleEffect_DropObjectAtAnchor(s32 id) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *anchor;
  struct Actor *actor;

  anchor = ((struct ScoopWork *)Ram_HeapSlots->battleEffect)->actor;
  actor = Actor_GetById(id);
  if (anchor != NULL) {
    SET_ACTOR_POSITION(actor, anchor->x.fixed, anchor->y.fixed,
                       anchor->z.fixed);
  }
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = 0;
  } else {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
  actor->gravity = toFIXED(9) / 100;
  wait(50);
  event->talk = id | FIELD_TALK_SILENT;
}

struct Unit *Unit_Get(u32 unitId);
extern const u16 gScoop_DugItems[16];

/// SCOOPED_ITEM_ACTOR at the anchor, fetched or spawned (SPRITE_UNSEEN_MARKER),
/// shows what Scoop dug up: three times in four one of gScoop_DugItems's
/// sixteen items (a coin when there is no room for it), else coins up to twice
/// the leader's level; it then drops (BattleEffect_DropObjectAtAnchor).
void Scoop_DigUpItem(void) {
  struct ScoopWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *anchor;
  struct FieldWork *event;
  struct Unit *unit;
  struct Actor *actor;
  s32 item;
  s32 id = SCOOPED_ITEM_ACTOR;

  anchor = work->actor;
  event = Ram_HeapSlots->fieldWork;
  unit = Unit_Get(gMainRoutineWork.leaderActorId);

  actor = Actor_GetById(id);
  if (actor == NULL) {
    actor = Actor_CreateFar(SPRITE_UNSEEN_MARKER, anchor->x.fixed,
                            anchor->y.fixed, anchor->z.fixed);
    if (actor == NULL)
      return;
    event->actors[id] = actor;
  }
  if (Random16() < SCOOP_ITEM_CHANCE) {
    item = gScoop_DugItems[Random_Below(ARRAY_COUNT(gScoop_DugItems))];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = SCOOPED_COINS | 1;
  } else {
    item = (Random_Below(unit->level * 2) + 1) | SCOOPED_COINS;
  }
  SET_ACTOR_POSITION(actor, anchor->x.fixed, anchor->y.fixed, anchor->z.fixed);
  Actor_ShowItemIconById(id, item);
  event->scoopedItem = item;
  BattleEffect_DropObjectAtAnchor(id);
}
#undef NEXT_PHASE
#undef SCOOP_DRAWN_DUST
#undef SCOOP_DRAWN_TARGET_DUST
#undef SCOOP_PICTURE_SIZE
#undef SCOOP_SPARKLES
#undef SCOOP_FIND_CHANCE
#undef SCOOP_ITEM_CHANCE
