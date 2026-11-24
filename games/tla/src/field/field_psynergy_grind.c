/// @file
/// Grind: the screen shakes up and down while spray rises round the target,
/// a sea rock is ground away, and the plain sway Grind makes at nothing.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

/// The splashes of spray Grind keeps.
#define GRIND_SPLASH_COUNT 64
/// The largest picture Grind grinds down, in bytes, a byte a pixel.
#define GRIND_PICTURE_SIZE 2048
/// The tiles of each frame of the splash picture (RESOURCE_WATER_SPLASH), 16 by
/// 16 pixels in 256 colours.
#define GRIND_SPLASH_FRAME_TILES SPRITE_TILES_8BPP(16, 16)
/// How far in front of or behind the target the spray is drawn.
#define GRIND_SPRAY_DEPTH 15

/// Grind's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the splash picture and
/// the splash particles.
struct GrindState {
  u8 unused[32]; ///< no code names it
  u8 picture[GRIND_PICTURE_SIZE];
  struct FieldPsynergyParticle particles[GRIND_SPLASH_COUNT];
  struct EffectSprite sprites[GRIND_SPLASH_COUNT];
};

/// What Grind does.
enum GrindMode {
  GRIND_ON_TARGET = 0, ///< the target's Grind event runs as the shaking peaks
  GRIND_SEA_ROCK = 1,  ///< a sea rock is ground down row by row
  GRIND_ON_NOTHING = 2 ///< spray rises before the caster
};

/// Grind ("Pulverize large objects."): the screen shakes harder and harder
/// while spray rises round the target, or round the spot 32 pixels before the
/// caster. A sea rock (SPRITE_SEA_ROCK) is ground down row by row; otherwise
/// the place's Grind event runs while the shaking is at its height.
void FieldPsynergy_GrindOnTarget(void) {
  struct GrindState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct GrindState));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct Actor *caster = scene->caster;
  struct Actor *child = scene->target;
  struct Sprite *animation = child->sprite;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
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
  s32 isSeaRock;
  s32 mode;
  FIXED radius;
  s32 selector;
  struct Trigger *pending;
  u8 *picture;
  s32 counter;
  FIXED amplitude;

  shifts = 0;
  radius = toFIXED(14);
  animating = 0;
  spraying = 0;
  next = 0;
  mode = GRIND_ON_TARGET;
  selector = Scene_SelectPsynergyTarget(scene->fieldPsynergy);
  pending = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
                                      scene->fieldPsynergy, selector);
  if (child != NULL) {
    if (animation != NULL) {
      if (animation->width * animation->height <= GRIND_PICTURE_SIZE) {
      } else
        goto end;
    }
  } else {
    scene->x = caster->x.fixed;
    scene->z = caster->z.fixed + toFIXED(8);
    scene->y = caster->y.fixed;
    Vector_AddPolarOffset(toFIXED(32), scene->angle, &scene->x);
  }
  isSeaRock = 0;
  if (Actor_GetSpriteIdFar(child) == SPRITE_SEA_ROCK) {
    isSeaRock = 1;
    mode = GRIND_SEA_ROCK;
  }
  if (child == NULL)
    mode = GRIND_ON_NOTHING;
  if (mode == GRIND_SEA_ROCK)
    radius = toFIXED(14);
  if (mode == GRIND_ON_NOTHING)
    radius = toFIXED(21);

  FieldPsynergy_FadeToCastColor();
  source = Resource_GetTableEntry(RESOURCE_WATER_SPLASH);
  picture = state->picture;
  Decompress_TaggedLz(source, picture);
  entry = VramCache_FindFreeEntry();
  tile = VramCache_Load(entry, 1024, picture);
  particle = state->particles;
  for (i = 0; i < GRIND_SPLASH_COUNT; i++) {
    sprite = &state->sprites[i];
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    particle->timer = -1;
    particle++;
  }
  if (child != NULL && animation != NULL && isSeaRock == 1) {
    Sprite_SetAnimationSpeedFar(animation, 0);
    FieldPsynergy_DecodePicture((struct PictureSource *)animation->entries[0],
                                picture, animation->width * animation->height);
  }
  ViewMove_Start(VIEW_MOVE_Z);
  wait(1);
  Sound_PlayFar(SE_PUSH_STEP);
  counter = 0;
  amplitude = toFIXED(2);
  phase = SWAY_GROW;
  timer = 0;
  do {
    if (spraying) {
      if (mode == GRIND_SEA_ROCK) {
        if (radius > toFIXED(5))
          radius += -(toFIXED(1) / 25);
      } else if (radius > toFIXED(5)) {
        radius += -(toFIXED(2) / 25);
      }
    }
    for (i = 0; i < spraying; i++) {
      angle = Random16();
      if (mode == GRIND_ON_NOTHING)
        angle = ((Random16() * 10) >> 4) - TRIG_TURN / 16;
      sprite = &state->sprites[next];
      particle = &state->particles[next];
      if (child != NULL) {
        sprite->oam.priority = Actor_GetSpritePriority(child);
        if (angle < TRIG_TURN / 2)
          sprite->depth = Actor_GetSpriteDepth(child) + GRIND_SPRAY_DEPTH;
        else
          sprite->depth = Actor_GetSpriteDepth(child) - GRIND_SPRAY_DEPTH;
      }
      particle->timer = 0;
      SET_POSITION(particle, scene->x, scene->y + toFIXED(5), scene->z);
      if (radius > toFIXED(5))
        Vector_AddPolarOffset(radius + Random16() * 5, angle, &particle->x);
      else
        Vector_AddPolarOffset(radius - (Random16() << 2), angle, &particle->x);
      next++;
      next %= GRIND_SPLASH_COUNT;
    }
    particle = state->particles;
    for (i = 0; i < GRIND_SPLASH_COUNT; i++, particle++) {
      sprite = &state->sprites[i];
      if ((u32)particle->timer <= 23) {
        if (particle->timer == 0)
          sprite->oam.index = tile;
        if (particle->timer == 18)
          sprite->oam.index = tile + GRIND_SPLASH_FRAME_TILES;
        if (particle->timer == 20)
          sprite->oam.index = tile + 2 * GRIND_SPLASH_FRAME_TILES;
        if (particle->timer == 22)
          sprite->oam.index = tile + 3 * GRIND_SPLASH_FRAME_TILES;
        EffectSprite_DrawAtWorldPosition(sprite, particle);
        particle->timer++;
      }
    }
    if (child != NULL && animation != NULL && animating) {
      s32 size = animation->width * animation->height;
      dst = (u8 *)OBJ_VRAM0 +
            VramCache_Load(animation->slot, size, NULL) * TILE_SIZE_4BPP;
      if (shifts % 3 == 0)
        BattleEffect_ShiftRowsDown(picture, animation->width,
                                   animation->height);
      shifts++;
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->pictureUploader)(
          picture, animation->width, animation->height, dst);
    }
    ViewMove_Swing(VIEW_MOVE_Z, counter++, amplitude);
    switch (phase) {
    case SWAY_GROW:
      if (timer == 40 && mode != GRIND_ON_TARGET)
        spraying = 1;
      amplitude += toFIXED(1) / 10;
      if (amplitude > toFIXED(12) - 1) {
        phase++;
        timer = -1;
      }
      break;
    case SWAY_HOLD:
      if (isSeaRock == 0) {
        if (timer == 50) {
          phase = SWAY_SHRINK;
          timer = -1;
        }
        if (pending != NULL)
          ((void (*)(s32, s32, s32))pending->value)(0, timer, selector);
      } else {
        if (timer == animation->height * 3) {
          phase = SWAY_SHRINK;
          timer = -1;
        }
        animating = 1;
      }
      break;
    case SWAY_SHRINK:
      if (isSeaRock == 1)
        animating = 0;
      if (timer == 5 && mode != GRIND_ON_TARGET)
        spraying = 0;
      amplitude += -(toFIXED(3) / 20);
      if (amplitude <= toFIXED(2)) {
        phase++;
        timer = -1;
      }
      break;
    case SWAY_SETTLE:
      amplitude = 0;
      if (timer == 0)
        phase = SWAY_DONE;
      break;
    }
    wait(1);
    timer++;
  } while (phase != SWAY_DONE);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  ViewMove_End();
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(entry);
end:
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

// FIELD_PSYNERGY_GRIND as FieldPsynergy_Run casts it: the view sways.

/// Sways the view up and down the screen: the swing grows from SWAY_START to
/// SWAY_PEAK, holds for SWAY_HOLD_FRAMES, shrinks back and stops; then the
/// map's view bounds come back.
void FieldPsynergy_Grind(void) {
  struct FieldEffectState *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  // Grind's state's size, though the sway holds only the view move.
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct GrindState));
  scene = Ram_HeapSlots->fieldEffectState;
  FieldPsynergy_FadeToCastColor();
  BattleEffect_SetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
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
