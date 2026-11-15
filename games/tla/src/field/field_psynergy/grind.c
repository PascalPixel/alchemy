/// @file
/// Grind: the view swayed up and down.
// Part of field_psynergy.c.

#include "../field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

struct SplashParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct SplashWork {
  u8 unk0000[0x20];
  u8 picture[0x800];
  struct SplashParticle particles[64];
  struct EffectSprite sprites[64];
};

struct SplashAnimation {
  u8 unk00[16];
  u8 entry;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16[18];
  void *tiles;
};

/* VIEW(f0e92b4): tag struct PendingEvent as struct PendingEvent__f0e92b4;
 * event_work.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field_psynergy/grind.c:212: structure has no member named
 * `callback'. */
struct PendingEvent__f0e92b4 {
  u8 unk00[8];
  void (*callback)(s32, s32, s32);
};

extern char ResourceId_WaterSplash;

void FieldPsynergy_GrindOnTarget(void) {
  struct SplashWork *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x1920);
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *caster = scene->mainActor;
  struct Actor *child = scene->child;
  struct SplashAnimation *animation = child->sprite;
  struct EffectSprite *sprite;
  struct SplashParticle *particle;
  void *source;
  u8 *dst;
  s32 angle;
  s32 i;
  s32 entry;
  s32 tile;
  s32 phase;
  s32 timer;
  s32 shifts;
  s32 animating;
  s32 spraying;
  s32 next;
  s32 isWaterObject;
  s32 mode;
  s32 radius;
  s32 selector;
  struct PendingEvent__f0e92b4 *pending;
  u8 *picture;
  s32 counter;
  s32 amplitude;

  shifts = 0;
  radius = 0xe0000;
  animating = 0;
  spraying = 0;
  next = 0;
  mode = 0;
  selector = Scene_SelectPsynergyTarget(scene->fieldPsynergy);
  pending =
      Scene_FindPsynergyTrigger(0x50000005, scene->fieldPsynergy, selector);
  if (child != NULL) {
    if (animation != NULL) {
      if (animation->width * animation->height <= 0x800) {
      } else
        goto end;
    }
  } else {
    scene->x = caster->x.fixed;
    scene->z = caster->z.fixed + 0x80000;
    scene->y = caster->y.fixed;
    Vector_AddPolarOffset(px(32), scene->angle, &scene->x);
  }
  isWaterObject = 0;
  if (Actor_GetSpriteIdFar(child) == 424) {
    isWaterObject = 1;
    mode = 1;
  }
  if (child == NULL)
    mode = 2;
  if (mode == 1)
    radius = 0xe0000;
  if (mode == 2)
    radius = 0x150000;

  FieldPsynergy_FadeToCastColor();
  source = Resource_GetTableEntry((s32)&ResourceId_WaterSplash);
  picture = work->picture;
  Decompress_TaggedLz(source, picture);
  entry = VramCache_FindFreeEntry();
  tile = VramCache_Load(entry, 0x400, picture);
  particle = work->particles;
  for (i = 0; i < 64; i++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->timer = -1;
    particle++;
  }
  if (child != NULL && animation != NULL && isWaterObject == 1) {
    Animation_ApplyChildValueFar(animation, 0);
    FieldPsynergy_DecodePicture(animation->tiles, picture,
                                animation->width * animation->height);
  }
  ViewMove_Start(2);
  wait(1);
  Sound_PlayFar(SE_PUSH_STEP);
  counter = 0;
  amplitude = 0x20000;
  phase = 0;
  timer = 0;
  do {
    if (spraying) {
      if (mode == 1) {
        if (radius > 0x50000)
          radius += -0xa3d;
      } else if (radius > 0x50000) {
        radius += -0x147a;
      }
    }
    for (i = 0; i < spraying; i++) {
      angle = Random16();
      if (mode == 2)
        angle = ((Random16() * 10) >> 4) - TRIG_TURN / 16;
      sprite = &work->sprites[next];
      particle = &work->particles[next];
      if (child != NULL) {
        sprite->oam.priority = BattleEffect_GetObjectPriority(child);
        if (angle <= 0x7fff)
          sprite->depth = BattleEffect_GetObjectDepth(child) + 15;
        else
          sprite->depth = BattleEffect_GetObjectDepth(child) - 15;
      }
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y + 0x50000, scene->z);
      if (radius > 0x50000)
        Vector_AddPolarOffset(radius + Random16() * 5, angle, &particle->x);
      else
        Vector_AddPolarOffset(radius - (Random16() << 2), angle, &particle->x);
      next++;
      next %= 64;
    }
    particle = work->particles;
    for (i = 0; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      if ((u32)particle->timer <= 23) {
        if (particle->timer == 0)
          sprite->oam.index = tile;
        if (particle->timer == 18)
          sprite->oam.index = tile + 8;
        if (particle->timer == 20)
          sprite->oam.index = tile + 16;
        if (particle->timer == 22)
          sprite->oam.index = tile + 24;
        EffectSprite_DrawAtWorldPosition(sprite, particle);
        particle->timer++;
      }
    }
    if (child != NULL && animation != NULL && animating) {
      s32 size = animation->width * animation->height;
      dst = (u8 *)OBJ_VRAM0 + VramCache_Load(animation->entry, size, NULL) * 32;
      if (shifts % 3 == 0)
        BattleEffect_ShiftRowsDown(picture, animation->width,
                                   animation->height);
      shifts++;
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
          picture, animation->width, animation->height, dst);
    }
    ViewMove_Swing(2, counter++, amplitude);
    switch (phase) {
    case 0:
      if (timer == 40 && mode != 0)
        spraying = 1;
      amplitude += 0x1999;
      if (amplitude > 0xbffff) {
        phase++;
        timer = -1;
      }
      break;
    case 1:
      if (isWaterObject == 0) {
        if (timer == 50) {
          phase = 2;
          timer = -1;
        }
        if (pending != NULL)
          pending->callback(0, timer, selector);
      } else {
        if (timer == animation->height * 3) {
          phase = 2;
          timer = -1;
        }
        animating = 1;
      }
      break;
    case 2:
      if (isWaterObject == 1)
        animating = 0;
      if (timer == 5 && mode != 0)
        spraying = 0;
      amplitude += -0x2666;
      if (amplitude <= 0x20000) {
        phase++;
        timer = -1;
      }
      break;
    case 3:
      amplitude = 0;
      if (timer == 0)
        phase = 999;
      break;
    }
    wait(1);
    timer++;
  } while (phase != 999);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  ViewMove_End();
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(entry);
end:
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

// Field Psynergy kind 27 (FieldPsynergy_Run's case 27): the view sways.

#define VIEW_MOVE_WORK_SIZE 0x1920

/// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 ///< 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10

/// Sways the view up and down the screen: the swing grows from 2 to 6 pixels,
/// holds for 10 frames, shrinks back to 2 and stops; then the map's view
/// bounds come back.
void FieldPsynergy_Grind(void) {
  struct FieldEffectWork *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, VIEW_MOVE_WORK_SIZE);
  scene = Ram_HeapSlots->fieldEffectWork;
  FieldPsynergy_FadeToCastColor();
  BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
  ViewMove_Start(VIEW_MOVE_Z);
  wait(10);

  phase = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    ViewMove_Swing(VIEW_MOVE_Z, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD;
        frames = -1;
      }
      break;
    case SWAY_HOLD:
      if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK;
        frames = -1;
      }
      break;
    case SWAY_SHRINK:
      amplitude -= SWAY_RAMP;
      if (amplitude <= SWAY_START) {
        state = SWAY_SETTLE;
        frames = -1;
      }
      break;
    case SWAY_SETTLE:
      amplitude = 0;
      if (frames == 0)
        state = SWAY_DONE;
      break;
    }
    wait(1);
    frames++;
  } while (state != SWAY_DONE);

  ViewMove_End();
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}
