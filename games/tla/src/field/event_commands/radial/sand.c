/// @file
/// Two routines not yet understood.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#include "field/field_psynergy_work.h"
#include "system/main_routine_work.h"
#include "system/io_reg.h"

extern char ResourceId_DustPuffA;

struct GrabEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xd48 - 395];
  s16 standing;
  s16 phase;
};

void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedActor, s32 groupMode,
                           s32 action);
s32 MapGrid_IsSandCell(s32 x, s32 z, s32 terrain);
void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode);
void FieldPsynergy_WatchSandGround(void);
void FieldPsynergy_EndSand(void);
void Picture_SinkColumnsByWave(u8 *source, u8 *destination, s32 phase);
void Sand_DrawLeaderSprite(void *sprite);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
void Actor_SetFirstSpriteAnimationFar(struct Actor *actor, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
struct Actor *Actor_GetById(u32 objectId);

void FieldPsynergy_Sand(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  struct BattleFxScene *effect = Ram_HeapSlots->effectWork;
  struct GrabEventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  s32 savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
  s32 frame;
  s32 delay;
  s32 i;

  owner = effect->mainObject;
  savedY = owner->y;
  motionFlags = &owner->motionFlags;
  savedFlags = *motionFlags;
  if (*(u16 *)((u8 *)owner->sprite + 20) != 0x2020)
    goto end;
  Func_080dc954();
  Actor_SetOthersPaused(owner, 1, 0);
  if (MapGrid_IsSandCell(owner->x, owner->z, owner->terrainId) == 0)
    event->standing = 1;
  else
    event->standing = 0;
  FieldPsynergy_FadeToCastColor();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  particleEntry = VramCache_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (; i < 64; i++) {
    Sand_SpawnGrain(sprite, particle, i, particleTiles, event->standing);
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectUpdateMode(effect->mainObject, 0);
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
  *motionFlags = 0;
  wait(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y += 0x3999;
    if (frame & 31)
      Sound_PlayFar(SE_566);
    owner->facing += TRIG_TURN / 8;
    wait(1);
  }
  owner->facing = TRIG_TURN / 4;
  wait(1);
  FieldPsynergy_DecodePicture(*(void **)((u8 *)owner->sprite + 40), work,
                              0x400);
  pictureEntry = VramCache_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Sand_InitLeaderSprite(work + 0x1900, pictureTiles);
  frame = 15;
  for (; frame <= 92; frame++) {
    Picture_SinkColumnsByWave(work, work + 0x400, frame * 3 / 2);
    ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
        work + 0x400, 32, 32, (u8 *)OBJ_VRAM0 + pictureTiles * 32);
    Sand_DrawLeaderSprite(work + 0x1900);
    i = 0;
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (; i <= 32; i++) {
      GrowingParticle_Update(sprite, particle, i, particleTiles);
      sprite += 40;
      particle += 28;
    }
    if (frame == 16)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
    if (frame == 45)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 60) {
      Actor_SetFirstSpriteAnimationFar(owner, 251);
      ((u8 *)owner->sprite)[5] &= 63;
      ((u8 *)owner->sprite)[7] = (((u8 *)owner->sprite)[7] & 63) | 64;
      Actor_SelectAnimationFar(owner, 1);
      Actor_SetSpriteFlagsFar(Actor_GetById(effect->firstObjectId), 0);
      FieldCamera_FollowActor(effect->firstObjectId, 1);
      owner->y = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
    wait(1);
  }
  if (event->mode == 3)
    gMainRoutineWork.renderMode = 9;
  else
    gMainRoutineWork.renderMode = 5;
  owner->timer = 0;
  owner->unk66 = 0;
  event->phase = 3;
  gMainRoutineWork.pendingThird = 1;
  if (event->standing == 0)
    Task_Add(FieldPsynergy_WatchSandGround, 0x480);
  Actor_SetOthersPaused(owner, 0, 16);
  VramCache_FreeEntry(pictureEntry);
  VramCache_FreeEntry(particleEntry);
  FieldPsynergy_FadeFromCastColor();
end:
  HeapSlot_Release(92);
  if (event->standing != 0)
    FieldPsynergy_EndSand();
}

struct GlowParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct GlowWork {
  u8 tiles[0x400];
  struct GlowParticle sparks[32];
  struct EffectSprite sparkSprites[32];
  struct EffectSprite burstSprites[32];
  struct GlowParticle bursts[32];
  s16 phase;
  s16 timer;
  s16 shake;
  u16 burstTile;
  s16 nextBurst;
  s16 newBursts;
  s8 paletteMode;
};

extern const u16 Data_080f0fa0[];

void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void MapGrid_SetScreenShakeFar(s32 x, s32 y, s32 decay);

#define GLOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of the Burst: cycles its particles' colours and draws them.
void FieldPsynergy_UpdateBurst(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct GlowParticle *particle;
  struct EffectSprite *sprite;
  volatile u16 *palette;
  s32 pos[3];
  s32 i;
  s32 *frames;
  s32 red;
  s32 blue;

  work->shake = 0;
  if (work->paletteMode == 1) {
    palette = (volatile u16 *)(OBJ_PLTT + 14 * 16);
    i = 0;
    frames = &canvas->drawnFrames;
    red = 31;
    blue = 31 << 10;
    for (; i <= 31; i++) {
      s32 green =
          (((Trig_Sin((((u32)*frames >> 1) + i << 16) + 0x90000 >> 5) * 12) >>
            16) +
           16)
          << 5;

      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      *palette++ = green | blue | red;
      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
    }
  } else if (work->paletteMode == 2) {
    Dma_Set(Data_080f0fa0, (void *)(OBJ_PLTT + 15 * 16),
            DMA_ENABLE << 16 | 0x10, REG_DMA3);
    work->paletteMode = 0;
  }
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);

  if (scene->enabled == 0) {
    particle = work->sparks;
    for (i = 0; i < 32; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(Random16() * 12 + 0x40000, Random16(),
                            &particle->x);
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 71)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 6:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 12:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 32:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 42:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 52:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 62:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x300;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX >= Q16_ONE / 2) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 40)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0)
        work->paletteMode = 2;
      if (work->timer == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if ((work->timer & 15) == 0)
        Sound_PlayFar(SE_CLATTER);
      if ((work->timer & 7) == 0) {
        MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      if (work->timer == 128)
        GLOW_NEXT_PHASE(work);
      break;
    case 5:
      if (work->timer == 35)
        GLOW_NEXT_PHASE(work);
      break;
    case 6:
      if (work->timer == 0)
        work->newBursts = 10;
      if (work->timer == 4)
        work->newBursts = 10;
      if (work->timer == 0) {
        MapGrid_SetScreenShakeFar(px(12), px(12), 0xe666);
        Sound_PlayFar(SE_CRASH);
        if (work->timer == 0)
          work->shake = 3;
      }
      if (work->timer == 20)
        work->shake = 1;
      if (work->timer == 72) {
        work->phase = 999;
        work->timer = -1;
      }
      break;
    }
  } else {
    particle = work->sparks;
    for (i = 0; i < 12; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 47)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 4:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 8:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 14:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 28:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 35:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 41:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(SE_SPIN);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x500;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX > 0x4ccb) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0) {
        work->paletteMode = 2;
        if (work->timer == 0) {
          MapGrid_SetScreenShakeFar(px(3), px(3), 0xe666);
          work->newBursts = 1;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if (work->timer == 5)
        BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
      if (work->timer == 20) {
        work->phase = 999;
        work->timer = -1;
      }
      break;
    }
  }
  work->timer++;
}

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);

/// Runs field Psynergy animation 21 for the dispatcher's targeted path.
void FieldPsynergy_BurstOnTarget(void) { FieldPsynergy_Burst(); }
#undef GLOW_NEXT_PHASE
