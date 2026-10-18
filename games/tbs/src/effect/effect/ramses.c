// Part of effect.c, the battle effect object, compiled only through it.
// The Ramses summon.

// Ramses' slots: objects 0 to 6 are the formation, 7 and 8 the orb and 9 to
// 14 the rocks. work->particles 0 to 5 move the rocks (variant counts their
// bounces), 6 to 29 are the orb's puffs (eight kept per drop, four thrown)
// and 30 to 41 the rocks' puffs, two per rock. PARTICLES holds the sparks.
#define RAMSES_FRAMES 320         // frames in the effect
#define RAMSES_STRIKE_FRAME 260   // the orb's burst strikes the targets
#define RAMSES_ROCKS 6            // rocks that fall and bounce
#define RAMSES_FIRST_PUFF 6       // the first puff in work->particles
#define RAMSES_FIRST_ROCK_PUFF 30 // the first rock puff in work->particles
#define RAMSES_SPARKS 512         // sparks of the orb's burst

// Abilities: Ramses (scene kind 1).
//
// Over the desert backdrop a formation stands while an orb circles above it,
// drops three times throwing puffs, and flies off; RAMSES_ROCKS rocks fall
// and bounce, and the orb's flight ends in a burst of RAMSES_SPARKS sparks
// from PARTICLES. Up to RAMSES_FRAMES frames; A or B ends it early.
//
//   0 to 90     the orb circles above the formation
//   91 to 196   it drops three times, 40 frames apart, throwing puffs, and
//               rises again; the screen shakes over frames 96 to 251
//   244 to 275  it flies off to the left, speeding up
//   260         the targets are struck and the sparks burst out
//
// After frame 96 the rocks fall and bounce. Within a frame the order of the
// steps matters: later steps draw over earlier ones, and several of them
// consume random numbers.
void BattleFx_RunRamses(struct BattleEffectArgument *effect) {
  // Every object of the scene takes this scale.
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  struct SpritePlacement position;
  DrawRectangle draw[2];
  struct SpriteScale scale;
  void **cache;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  struct Point2D base;
  struct Point2D orb;
  s32 i;
  s32 k;

  cache = &gBattleFxWork[1];
  canvas = cache[0];
  work = cache[-1];
  sheet = cache[1];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(1, 0);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(9, 379, 2);
  // Add the six falling rocks after creating the nine-object formation.
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct AnimationObject *object = ResourceObject_CreateFar(390);

    work->objects[9 + i] = object;
    if (object != 0) {
      object->flags = 0;
      AnimationObjects_SelectAnimationFar(object, i % 3);
      work->objects[9 + i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  // Keep the effect between lines 16 and 136.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
  BattleFx_WipeCanvas(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  // The formation's corner at (188, 92) and the orb at (160, 92); the rocks
  // wait 16 pixels apart above the field at random columns.
  base.x = 0xbc0000;
  base.y = 0x5c0000;
  orb.x = 0xa00000;
  orb.y = 0x5c0000;
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct EffectStep *rock = &work->particles[i];

    rock->x = (Random16() & 127) << 16;
    rock->y = i * -0x100000;
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  // A puff's variant counts up and from 24 it is no longer drawn; every puff
  // and spark starts finished.
  for (i = 0; i != 58; i++)
    work->particles[RAMSES_FIRST_PUFF + i].variant = 24;
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0; frame != RAMSES_FRAMES && !(gKeysRepeat & (KEY_A | KEY_B));
       frame++) {
    if (frame == 94)
      AudioCommand_PlayFar(SE_156);
    if (frame == 136)
      AudioCommand_PlayFar(SE_156);
    if (frame == 178)
      AudioCommand_PlayFar(SE_156);
    if (frame == RAMSES_STRIKE_FRAME)
      AudioCommand_PlayFar(SE_145);
    scale = unitScale;
    if (frame >= 96 && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= RAMSES_STRIKE_FRAME && frame <= 263)
      work->shakeFrames = 1;
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != 7; i++) {
      position.x = (gRamses_ObjectColumns[i] << 16) + base.x - 0x200000;
      position.y = (gRamses_ObjectRows[i] << 16) + base.y - 0x200000;
      Object_ApplyProjectedPlacementFar(work->objects[i], &position, &scale, 0);
    }
    // The orb circles until frame 90.
    if (frame <= 90) {
      orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
      orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
    }
    // Then it drops 32 pixels three times, 40 frames apart, throwing four
    // puffs as it lands and rising back over 16 frames.
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = 91; i != 3; start += 40, i++) {
        if (frame >= start && frame < start + 4)
          orb.y += 0x80000;
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct EffectStep *puff =
                &work->particles[RAMSES_FIRST_PUFF + i * 8 + k];

            puff->x = 0x400000;
            puff->y = 0x600000;
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          orb.y -= 0x20000;
      }
    }
    // From frame 244 it flies off to the left, faster each frame from 252.
    if (frame >= 244 && frame <= 251)
      orb.x -= 0x10000;
    if (frame >= 252 && frame <= 275)
      orb.x -= (frame - 250) << 16;
    // The orb's two objects, 32 pixels apart, high above the field.
    if (frame <= 259) {
      position.x = orb.x;
      position.height = -0x1000000;
      position.y = orb.y - 0x1000000;
      Object_ApplyProjectedPlacementFar(work->objects[7], &position, &scale, 0);
      position.x = orb.x + 0x200000;
      Object_ApplyProjectedPlacementFar(work->objects[8], &position, &scale, 0);
    }
    position.height = 0;
    // The rocks fall after frame 96. Past row 120 a rock bounces back at
    // half speed, throwing two puffs; on its next landing it starts again
    // from the top until frame 199, and after that it is left where it
    // lands.
    for (i = 0; i != RAMSES_ROCKS; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Object_ApplyProjectedPlacementFar(work->objects[9 + i], &position,
                                          &scale, 0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > 96)
          rock->velocityY += 0x4000;
        if (rock->y > 0x780000) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct EffectStep *puff =
                  &work->particles[RAMSES_FIRST_ROCK_PUFF + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - 0x200000;
              puff->velocityX = ((Random16() & 255) - 127) << 10;
              puff->velocityY = ((Random16() & 255) - 127) << 10;
              puff->variant = Random16() & 15;
            }
          } else if (frame <= 199) {
            rock->y = 0;
            rock->velocityY = 0;
            rock->variant = 0;
          }
        }
      }
    }
    // Puffs show the last four puff cells, six frames each, as they drift.
    for (i = 0; i != 56; i++) {
      struct EffectStep *puff = &work->particles[RAMSES_FIRST_PUFF + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant <= 23) {
          s32 cell = puff->variant / 6 + 3;

          draw[0](canvas, work->sheet + gRamses_PuffSheetOffsets[cell],
                  HI(puff->x) - gRamses_PuffSizes[cell] / 2,
                  HI(puff->y) - gRamses_PuffSizes[cell] / 2,
                  gRamses_PuffSizes[cell], gRamses_PuffSizes[cell]);
        }
        EffectStep_AdvanceWithGravity2D(puff, 60, -0x4000);
        puff->variant++;
      }
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      // The strike: every target recoils and flashes white, and the screen
      // shakes.
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
      }
      work->shakeFrames = 8;
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      // Throw the sparks from (32, 92) at random angles and speeds, twice as
      // hard up and down as across, each living 32 to 47 frames.
      for (i = 0; i != RAMSES_SPARKS; i++) {
        struct EffectStep *spark = &PARTICLES[i];
        s32 speed;
        s32 angle;

        speed = 0x3ff;
        speed &= Random16();
        angle = Random16() & 0xffff;
        spark->x = 0x200000;
        spark->y = 0x5c0000;
        spark->velocityX = Trig_Sin(angle) * (speed + 32) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
        spark->variant = (Random16() & 15) + 32;
      }
    }
    // Sparks shrink as their lives run down; alternate sparks are drawn
    // additively, the others brightest-wins.
    for (i = 0; i != RAMSES_SPARKS; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, HI(spark->x), HI(spark->y),
                         size);
        EffectStep_AdvanceWithGravity2D(spark, 62, 0x1000);
        spark->variant--;
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  BattleEventRuntime_BeginPhaseFar(134);
  BattleFx_SetupBlendedDisplay();
  for (i = 0; i != 15; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  EFFECT_FINISH_CANVAS();
}
