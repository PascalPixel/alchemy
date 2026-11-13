/// @file
/// The view swayed up and down, and the glow of field Psynergy kind 24.
// Part of psynergy.c.

#include "../psynergy_internal.h"

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

/* VIEW(f0e92b4): tag struct FxScene as struct FxScene__f0e92b4; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:5353: structure has no member
 * named `facing'. */
struct FxScene__f0e92b4 {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

/* VIEW(f0e92b4): tag struct PendingEvent as struct PendingEvent__f0e92b4;
 * event_work.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:5484: structure has no member
 * named `callback'. */
struct PendingEvent__f0e92b4 {
  u8 unk00[8];
  void (*callback)(s32, s32, s32);
};

extern char ResourceId_WaterSplash;

/* VIEW(f0e92b4): routine Scene_FindPsynergyTrigger as struct
 * PendingEvent__f0e92b4 *Scene_FindPsynergyTrigger(u32 kind, s32 entry, s32
 * arg); f0e5d5c has void *Scene_FindPsynergyTrigger(u32 kind, s32 entry, s32
 * arg). */
extern struct PendingEvent__f0e92b4 *
Func_080ce458__f0e92b4(u32 kind, s32 entry,
                       s32 arg) __asm__("Scene_FindPsynergyTrigger");
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Actor_GetSpriteIdFar(struct Actor *object);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
/* VIEW(f0e92b4): routine EffectSprite_DrawAtWorldPosition as void
 * EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct
 * SplashParticle *particle); f0e4a34 has void
 * EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct
 * GlowParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e92b4(
    struct EffectSprite *sprite,
    struct SplashParticle
        *particle) __asm__("EffectSprite_DrawAtWorldPosition");
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows);

void FieldPsynergy_GrindOnTarget(void) {
  struct SplashWork *work = HeapSlot_Alloc(92, 0x1920);
  struct FxScene__f0e92b4 *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
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
  selector = Func_080ce31c(scene->fieldKind);
  pending = Func_080ce458__f0e92b4(0x50000005, scene->fieldKind, selector);
  if (child != NULL) {
    if (animation != NULL) {
      if (animation->width * animation->height <= 0x800) {
      } else
        goto end;
    }
  } else {
    scene->x = caster->x;
    scene->z = caster->z + 0x80000;
    scene->y = caster->y;
    Vector_AddPolarOffset(0x200000, scene->facing, &scene->x);
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
  Sound_PlayFar(238);
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
        angle = ((Random16() * 10) >> 4) - 0x1000;
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
      particle->x = scene->x;
      particle->y = scene->y + 0x50000;
      particle->z = scene->z;
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
        EffectSprite_DrawAtWorldPosition__f0e92b4(sprite, particle);
        particle->timer++;
      }
    }
    if (child != NULL && animation != NULL && animating) {
      s32 size = animation->width * animation->height;
      dst =
          (u8 *)0x06010000 + VramCache_Load(animation->entry, size, NULL) * 32;
      if (shifts % 3 == 0)
        BattleEffect_ShiftRowsDown(picture, animation->width,
                                   animation->height);
      shifts++;
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
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
  Sound_PlayFar(390);
  ViewMove_End();
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(entry);
end:
  HeapSlot_Release(92);
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
  struct EffectScene *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  scene = Ram_HeapSlots->effectWork;
  FieldPsynergy_FadeToCastColor();
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
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
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}

/// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
/// read a third of a turn apart for red, green and blue.
extern const u8 gData_080f0fe0[];

/// The colour of field Psynergy kind 24's glow (HoverGlow_Update) for a step of
/// its 24-step cycle and a brightness level (7 is neutral): three channels of
/// the wave, each raised or lowered by the level and held to 0..31, packed as
/// RGB555.
// ⚓️'s form of ☀️'s GraphicsPalette_SetTintChannelsFromCounters.
s32 HoverGlow_GetColor(s32 counter, s32 level) {
  s16 phase;
  s16 bias;
  s16 red;
  s16 green;
  s16 blue;
  s16 redLevel;
  s32 step;
  const u8 *wave;

  step = (counter + 12) % 24;
  wave = gData_080f0fe0;
  phase = step * 4;
  redLevel = wave[(s16)(phase % 96)];
  bias = level - 7;
  red = redLevel + bias;
  green = wave[(phase + 32) % 96] + bias;
  blue = wave[(phase + 64) % 96] + bias;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return (blue << 10) | (green << 5) | red;
}

#define OBJ_PALETTE_14 ((u16 *)0x050003c0)

// The glow's strength, in frames: full at 112, and it fades out a frame at a
// time once the player leaves the place that feeds it.
#define GLOW_FULL 112
#define GLOW_SPENT_CUE 0x2099

// Kind 24's work (battle_fx_work[0]).
/* VIEW(f0e92b4): tag struct GlowWork as struct GlowWork__f0e92b4; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:5689: structure has no member
 * named `fading'. */
struct GlowWork__f0e92b4 {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  u8 unk102c[2];
  u16 tile;
  u8 unk1030[2];
  u16 phase;
  s16 fading;
};

struct GlowEvent {
  u8 unk000[0x166];
  u16 cue;
  u8 unk168[0xbdc];
  s16 glow;
  s16 glowHeld;
};

s32 MapGrid_IsHoverCell(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);

// Kind 24's frame callback: keeps the glow's strength full while the player
// stands where MapGrid_IsHoverCell allows, and runs it down otherwise (once it
// is fading, the glow shrinks with it and the event takes its cue when it is
// spent). It cycles OBJ palette 14 through HoverGlow_GetColor's colours,
// flickers between two pictures every other frame, and draws the glow under the
// player, flattened to three quarters of its height.
void HoverGlow_Update(void) {
  struct GlowWork__f0e92b4 *work = Ram_HeapSlots->battleFxWork[0];
  struct Actor *player = ObjectTable_Get(Party_GetLeaderActor());
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  u16 *palette;
  s32 position[3];
  s32 level;
  s32 i;
  s32 scaleX;
  s32 scaleY;
  struct EffectSprite *sprite;

  // The glow's strength.
  if (MapGrid_IsHoverCell(player->x, player->z, player->terrainId) == 0) {
    if (event->glowHeld != 0)
      event->glow--;
  } else {
    event->glow = GLOW_FULL;
  }

  if (work->fading == 1) {
    level = event->glow;
    if (level > GLOW_FULL)
      level = GLOW_FULL;
    if (level < 0)
      level = 0;
    work->scale = (level << 15) / GLOW_FULL + 0x8000;
    if (level == 0)
      event->cue = GLOW_SPENT_CUE;
  }

  // Cycle the glow's colours; the last takes the 15th's level.
  palette = OBJ_PALETTE_14;
  for (i = 0; i < 16; i++) {
    // The colour's store in a once-only block, which ends the scheduler's
    // region there; as the plain store, 2 of 194 instructions differ in
    // tla-en.
    do {
      *palette++ = HoverGlow_GetColor((((s16)work->phase >> 2) + i / 2) % 24,
                                      i == 15 ? 14 : i);
    } while (0);
  }
  work->phase++;

  // Draw it under the player.
  scaleX = work->scale;
  scaleY = scaleX * 6 / 8;
  sprite = &work->sprite;
  sprite->oam.index = work->tile + ((gFrameCount >> 1) & 1) * 32;
  sprite->oam.priority = BattleEffect_GetObjectPriority(player);
  sprite->depth = BattleEffect_GetObjectDepth(player) - 1;
  position[0] = player->x;
  position[1] = player->y;
  position[2] = player->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  EffectSprite_Draw(sprite);
}

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);
void Func_080e74d8(void);

/// Runs field Psynergy animation 24 for the dispatcher; the target is unused.
void FieldPsynergy_HoverOnTarget(void) { FieldPsynergy_Hover(); }
#undef VIEW_MOVE_WORK_SIZE
#undef SWAY_START
#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_HOLD_FRAMES
#undef OBJ_PALETTE_14
#undef GLOW_FULL
#undef GLOW_SPENT_CUE
