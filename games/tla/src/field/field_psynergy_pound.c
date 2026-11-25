/// @file
/// Pound: its shine, the object it drives into the ground, and a picture's
/// columns shifted by a wave.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

#include "sprite_ids.h"

/// The sparks of Pound's shine; the first 12 are drawn.
#define POUND_SPARK_COUNT 32
#define POUND_DRAWN_SPARKS 12
/// The shine's colours, two palettes of the canvas: blue full, red and green
/// swinging together round their middle as the cycle steps.
#define POUND_SHINE_COLORS (2 * PALETTE_COLORS)
/// The pieces the fist shatters into when it cannot come down.
#define POUND_SHATTER_PIECES 16
/// The glow under the fist, 64 pixels square in 16 colours.
#define POUND_GLOW_SIZE 64

/// The stages of the fist's fall (PoundState.state).
enum PoundStage {
  POUND_FALLS = 0,       ///< the fist comes down; it lands on the eighth frame
  POUND_SHINE_GROWS = 1, ///< the shine spreads, its colours cycling
  POUND_SHINE_HOLDS = 2,
  POUND_SETTLES = 3, ///< the shaking stops
  POUND_LIFTS = 4    ///< the fist rises off the ground
};

/// Pound's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the star tiles, the
/// shine's 32 sparks, the glow under the fist, the fist itself and the stage of
/// its fall.
struct PoundState {
  u8 tiles[4096];
  struct FieldPsynergyParticle particles[POUND_SPARK_COUNT];
  struct EffectSprite sprites[POUND_SPARK_COUNT];
  struct EffectSprite glow;
  struct Actor *actor; ///< the fist, then what sinks into the ground
  s16 state;   ///< the stage of the fall; FIELD_PSYNERGY_FINISHED once it ends
  s16 timer;   ///< frames into the stage
  s8 sparkles; ///< nonzero draws the sparks
  s8 landed;   ///< set on the frame the fist lands, passed to the event
};

/// Task of a Pound that comes down: the fist falls and lands with a shake,
/// the shine spreads round it with its sparks, then the fist lifts again.
void FieldPsynergy_UpdatePound(void) {
  struct PoundState *state = Ram_HeapSlots->battleEffect;
  struct CanvasSpriteState *canvas = Ram_HeapSlots->canvasSprite;
  struct FieldPsynergyParticle *particle;
  struct EffectSprite *sprite;
  s32 i;
  s32 red, green;

  state->landed = 0;
  if (state->state == POUND_SHINE_GROWS) {
    // Each colour is a 32nd of a turn further along the cycle, which moves on
    // every other frame.
    for (i = 0; i < POUND_SHINE_COLORS; i++) {
      u32 angle =
          (INTtoFIXED(((u32)canvas->drawnFrames >> 1) + i) + INTtoFIXED(9)) >>
          5;
      s32 blue = RGB(0, 0, RGB_LEVEL_MAX);

      red = Trig_Sin(angle) * CANVAS_SHINE_SWING;
      green = Trig_Sin((INTtoFIXED(((u32)canvas->drawnFrames >> 1) + i) +
                        INTtoFIXED(9)) >>
                       5) *
              CANVAS_SHINE_SWING;
      red = FIXEDtoINT(red) + CANVAS_SHINE_MIDDLE;
      green = FIXEDtoINT(green) + CANVAS_SHINE_MIDDLE;
      Io_Write16(RGB(0, green, 0) | blue | red,
                 &OBJ_PLTT_COLOR(CANVAS_PALETTE, i));
    }
  }
  if (state->sparkles) {
    particle = state->particles;
    sprite = state->sprites;
    for (i = 0; i < POUND_DRAWN_SPARKS; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        EffectSprite_Draw(sprite);
        Field_AdvanceWithGravity2D(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (state->state) {
  case POUND_FALLS:
    state->actor->y.fixed -= toFIXED(7) / 2;
    if (state->timer == 8) {
      canvas->scaleX = FIXED_ONE / 8;
      canvas->scaleY = FIXED_ONE / 8;
      MapGrid_SetScreenShakeFar(toFIXED(13) / 10, toFIXED(13) / 10, FIXED_ONE);
      Sound_PlayFar(SE_POUND);
      state->sparkles = 1;
      state->landed = POUND_STAGE_LANDED;
    }
    if (state->timer == 8) {
      state->state++;
      state->timer = -1;
    }
    break;
  case POUND_SHINE_GROWS:
    canvas->scaleX += FIXED_ONE / 16;
    canvas->scaleY = canvas->scaleX;
    if (canvas->scaleX > toFIXED(4) / 5 - 1) {
      canvas->scaleX = toFIXED(4) / 5;
      canvas->scaleY = toFIXED(4) / 5;
      state->state++;
      state->timer = -1;
    }
    break;
  case POUND_SHINE_HOLDS:
    canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
    if (state->timer == 32) {
      state->state++;
      state->timer = -1;
    }
    break;
  case POUND_SETTLES:
    if (state->timer == 0)
      MapGrid_SetScreenShakeFar(1, 1, 1);
    if (state->timer == 10) {
      state->state++;
      state->timer = -1;
    }
    break;
  case POUND_LIFTS:
    state->actor->y.fixed += toFIXED(3) / 2;
    if (state->timer == 13)
      state->state = FIELD_PSYNERGY_FINISHED;
    break;
  }
  state->timer++;
}

/// Task of a Pound that cannot come down: the fist sinks 2.5 pixels and
/// shrinks by 1/64 a frame for 8 frames, before it shatters.
void Pound_SinkFist(void) {
  struct PoundState *state = Ram_HeapSlots->battleEffect;

  if (state->state == POUND_FALLS) {
    struct Actor *actor = state->actor;

    actor->y.fixed += -(toFIXED(5) / 2);
    actor->scaleX += -(FIXED_ONE / 64);
    actor->scaleY += -(FIXED_ONE / 64);
    if (state->timer == 8)
      state->state = FIELD_PSYNERGY_FINISHED;
  }
  state->timer++;
}

/// Pound cast on a target: runs FieldPsynergy_Pound.
void FieldPsynergy_PoundOnTarget(void) { FieldPsynergy_Pound(); }

void FieldPsynergy_StartSceneSprite(void);
void FieldPsynergy_ShrinkToCaster(struct Actor *actor, s32 x, s32 y, s32 z);

/// Pound ("Drive objects into the ground."): a giant fist forms over the spot
/// 32 pixels before the caster, or over the target, and comes down on it. Out
/// of reach, or with a solid actor in the way, it shatters with the error
/// sound; else the place's Pound event runs.
void FieldPsynergy_Pound(void) {
  struct PoundState *state =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct PoundState));
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *child = scene->target;
  struct Actor *caster = scene->caster;
  struct Actor *actor;
  struct Actor *fist;
  struct CanvasSpriteState *canvas;
  struct EffectSprite *sprite;
  struct EffectSprite *glow;
  void *pending;
  s32 pos[3];
  s32 starEntry;
  s32 starTile;
  s32 glowEntry;
  s32 inReach;
  FIXED lift;
  s32 glowTile;
  s32 spot;
  s32 size = POUND_GLOW_SIZE * POUND_GLOW_SIZE / 2;
  s32 riseFrames;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 j;
  struct FieldPsynergyParticle *particle;

  lift = toFIXED(32);
  if (child == NULL) {
    SET_POSITION(scene, caster->x.fixed, caster->y.fixed, caster->z.fixed);
    Vector_AddPolarOffset(lift, scene->angle, &scene->x);
  } else if (child->motionFlags & ACTOR_FLOATS) {
    /* FAKEMATCH(dead): lift cleared under a test of the ground's height. */
    if (child->groundHeight == 300)
      lift = 0;
    scene->y = child->groundHeight;
    lift = child->y.fixed - child->groundHeight + toFIXED(32);
  }
  if (scene->aimAhead != 0) {
    // Field_MeasurePsynergyHeight returns -1 where the spot is out of reach.
    spot =
        ~Field_MeasurePsynergyHeight(scene->fieldPsynergy, caster, &scene->x);
    inReach = NONZERO(spot);
    if (inReach == 1) {
      for (j = 0; j < FIELD_ACTOR_COUNT; j++) {
        actor = field->actors[j];
        if (actor != NULL && actor->script != NULL &&
            (actor->collisionFlags & ACTOR_SOLID) &&
            FieldPsynergy_CheckOverlap(&scene->x, 4, &actor->x.fixed,
                                       actor->radius - 2) >= 0)
          goto outOfReach;
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && child->sceneByte != 0) {
    outOfReach:
      inReach = 0;
    }
  }

  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(SE_ITEM_BREAK);
  fist = Actor_Spawn(SPRITE_POUND_FIST, 0, 0, 0);
  state->actor = fist;
  if (fist != NULL) {
    fist->motionFlags = 0;
    if (field->psynergyDrawFlags & PSYNERGY_DRAW_FIXED_PRIORITY) {
      if ((fist->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
          ACTOR_SPRITE_ONE) {
        ((struct EffectSprite *)fist->sprite)->oam.priority = 1;
        fist->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      }
    } else {
      fist->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    fist->facing = caster->facing;
    Actor_SelectAnimationFar(fist, 2);
    if (!(field->psynergyDrawFlags & PSYNERGY_DRAW_NO_SCENE_SPRITE))
      FieldPsynergy_StartSceneSprite();

    BattleEffect_StartCanvasSprite();
    canvas = Ram_HeapSlots->canvasSprite;
    canvas->priority = Actor_GetSpritePriority(scene->caster);
    canvas->depth = 1;
    canvas->mode = CANVAS_SPRITE_256_COLORS;
    canvas->scaleX = 0;
    canvas->scaleY = 0;
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Camera_WorldToScreen(pos);
    canvas->x = (s16)FIXEDtoINT(pos[0]);
    canvas->y = (s16)FIXEDtoINT(pos[2]);
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GLOW_DISC),
                        Ram_HeapSlots->battleCanvas);
    canvas->command = CANVAS_UPLOAD;

    IWRAM_FILL_WORDS(state, size, 0);
    glowEntry = VramCache_FindFreeEntry();
    glowTile = VramCache_Load(glowEntry, size, state);
    glow = &state->glow;
    EffectSprite_Init(glow, 32, 32, OBJ_SIZE_64X64, glowTile);
    glow->oam.priority = 0;
    glow->oam.fullColor = 0;
    glow->oam.palette = 0;
    glow->depth = 0;
    glow->scaleY = glow->scaleX = FIXED_ONE * 4 / 5;
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Camera_WorldToScreen(pos);
    glow->x = pos[0];
    glow->y = pos[2];

    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GOLD_STAR), state);
    starEntry = VramCache_FindFreeEntry();
    starTile = VramCache_Load(starEntry, 256, state);
    for (j = 0, particle = state->particles; j < POUND_SPARK_COUNT;
         j++, particle++) {
      sprite = &state->sprites[j];
      speed = Random16() * 18 / 10;
      speed += toFIXED(5) / 2;
      angle = j * (DEGtoANG(30)) + (Random16() >> 5);
      pos[0] = scene->x;
      pos[1] = scene->y;
      pos[2] = scene->z;
      Vector_AddPolarOffset(toFIXED(4), angle, pos);
      Camera_WorldToScreen(pos);
      particle->x = pos[0];
      particle->y = pos[2];
      particle->velocityX = Iwram_MulFX(Trig_Cos(angle), speed);
      particle->velocityY = Iwram_MulFX(Trig_Sin(angle), speed);
      particle->timer = -j;
      EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, starTile);
      sprite->oam.fullColor = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = 0;
      sprite->depth = OAM_DEPTH_WINDOW;
      sprite->scaleY = sprite->scaleX = FIXED_ONE * 4 / 5;
    }
    wait(1);
    FieldPsynergy_GrowFromCaster(fist, scene->x, scene->y + lift, scene->z);
    Actor_SelectAnimationFar(fist, 1);
    wait(15);
    riseFrames = 24;
    do {
      fist->y.fixed += toFIXED(1) / 2;
      riseFrames--;
      wait(1);
    } while (riseFrames >= 0);
    wait(15);
    state->state = POUND_FALLS;
    state->timer = 0;
    state->sparkles = 0;
    if (!inReach) {
      Task_Add(Pound_SinkFist, TASK_PASS_FRAME);
      while (state->state != FIELD_PSYNERGY_FINISHED)
        wait(1);
      Task_Remove(Pound_SinkFist);
      for (j = 0; j < POUND_SHATTER_PIECES; j++)
        BattleEffect_SpawnScatterParticle(fist->x.fixed, fist->y.fixed,
                                          fist->z.fixed);
      Sound_PlayFar(SE_MENU_ERROR);
      wait(1);
    } else {
      pending = Scene_FindPsynergyTrigger(
          PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
          Scene_SelectPsynergyTarget(scene->fieldPsynergy));
      Task_Add(FieldPsynergy_UpdatePound, TASK_PASS_FRAME);
      frame = 0;
      while (state->state != FIELD_PSYNERGY_FINISHED) {
        wait(1);
        scene->stage = state->landed;
        scene->frame = frame;
        if (field->mode != ACTOR_SYSTEM_WORLD_MAP &&
            !(field->psynergyDrawFlags & PSYNERGY_DRAW_NO_SCENE_SPRITE)) {
          glow = &state->glow;
          EffectSprite_Draw(glow);
        }
        Field_RunPsynergyEvent(pending, state->landed, frame++);
      }
      Task_Remove(FieldPsynergy_UpdatePound);
      wait(10);
      Sound_PlayFar(SE_MENU_ERROR);
      FieldPsynergy_ShrinkToCaster(fist, fist->x.fixed, fist->y.fixed,
                                   fist->z.fixed);
    }
    Actor_ReleaseFar(fist);
    VramCache_FreeEntry(starEntry);
    VramCache_FreeEntry(glowEntry);
    BattleEffect_EndCanvasSprite();
  }
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// The pictures Picture_ShiftColumnsByWave shifts, 32 pixels square.
#define WAVE_PICTURE_SIZE 32

/// Copies a 32 by 32 byte picture into a cleared one stretched down, its rows
/// four pixels apart, each column slid down by phase (1.6 pixels a step, from
/// 124 pixels up) less a sine wave across the columns; what falls outside is
/// dropped. The Scoop draws its rising picture with it.
void Picture_ShiftColumnsByWave(u8 *source, u8 *destination, s32 phase) {
  s32 width = WAVE_PICTURE_SIZE;
  s32 height = WAVE_PICTURE_SIZE;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, WAVE_PICTURE_SIZE * WAVE_PICTURE_SIZE);
  for (column = 0; column < width; column++) {
    FIXED wave = Trig_Sin(column * (TRIG_TURN / 8)) * 8 + FIXED_ONE * 8;

    for (row = 0; row < height; row++) {
      FIXED position =
          phase * (toFIXED(8) / 5) - FIXED_ONE * 124 + row * (FIXED_ONE * 4);
      // In two steps: the wave subtracted, then the shift.
      s32 shift = position - wave;
      shift = FIXEDtoINT(shift);

      if (shift >= 0 && shift < height)
        destination[shift * WAVE_PICTURE_SIZE + column] =
            source[row * width + column];
    }
  }
}
#undef POUND_SHINE_COLORS
#undef POUND_SHATTER_PIECES
#undef POUND_GLOW_SIZE
#undef WAVE_PICTURE_SIZE
