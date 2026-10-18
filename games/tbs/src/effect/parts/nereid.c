// Part of effect.c, the battle effect object, compiled only through it.
// The Nereid summon.

// Nereid's ember columns draw three wrapping columns of fire over a band
// rising along the canvas floor. Its embers burst from one point, fall back
// under gravity, and from frame 80 rise away; one that falls below the band
// goes out and its slot is lit again. The falling sword (Titan Blade) shares
// these embers.

#define NEREID_FRAMES 124

#define NEREID_OBJECTS 12         // objects in the sweeping formation
#define NEREID_COLUMNS 3          // wrapping columns of fire
#define NEREID_EMBERS 1024        // embers in PARTICLES
#define NEREID_COLUMN_IMAGE 0x6e4 // the column of fire in the sheet

// Abilities: Nereid (scene kind 2).
//
// 124 frames; pressing A
// or B during frames 33 to 97 skips to frame 98. A formation of twelve
// objects sweeps in from the right; embers burst up on frame 28 and over
// frames 32 to 79, fall back under gravity and from frame 80 rise away,
// while three columns of fire stream upwards over frames 32 to 79 and the
// targets bounce. At the end the formation explodes with
// BattleFx_RunImpactBurst; the battle shows the damage on frame 120.
void BattleFx_RunNereid(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **cursor;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  u8 *palette;

  // cursor points at WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS), the canvas.
  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = cursor[0];
  work = cursor[-1]; // [39] the effect work
  graphics = cursor[HEAP_SLOT_BATTLE_SHEET -
                    HEAP_SLOT_BATTLE_CANVAS]; // the particle images
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleFx_FetchRectangleBlitters(0, draw);

  // The band along the bottom from WaterSpray, the columns from FirePillarA
  // (at sheet + 0x6e4).
  palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + NEREID_COLUMN_IMAGE);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // trails keep 3/4 a frame
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation comes in from (256, 88), heading left.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    EFFECT_FREE_PARTICLES(k, work->particles, 64);
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    life = &PARTICLES->variant;
    for (k = 0; k != NEREID_EMBERS; k++) {
      *life = -1;
      life += 7; // sizeof(struct EffectStep) / 4
    }
    sparks = PARTICLES;
    BattleFx_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleFx_SpawnObjects(NEREID_OBJECTS, 380, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      if (frame <= 15)
        rise += 2;

      // The formation slows to 58/64 of its speed across and 56/64 down
      // each frame, and is pushed back right when left of x 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      BattleFx_PlaceFormationObjects(1, focus.x, focus.y);

      // Embers burst up from an oval around (32, 96), twice as tall as wide,
      // with a random spread: every idle one of the first 256 on frame 28,
      // then up to 16 a frame over frames 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_EMBERS; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        AudioCommand_PlayFar(SE_164);
      if (frame == 32)
        AudioCommand_PlayFar(SE_145);
      if (frame == 80)
        AudioCommand_PlayFar(SE_144);

      // The columns: a 104-pixel image scrolling up 16 pixels a frame,
      // drawn twice to wrap round.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + NEREID_COLUMN_IMAGE,
                  gNereid_Columns[k][NEREID_COLUMN_X] - 17,
                  gNereid_Columns[k][NEREID_COLUMN_Y] - span - 104, 34, 104);
          draw[0](canvas, work->sheet + NEREID_COLUMN_IMAGE,
                  gNereid_Columns[k][NEREID_COLUMN_X] - 17,
                  gNereid_Columns[k][NEREID_COLUMN_Y] - span, 34, span);
        }
      }

      // The band along the bottom: five 32x32 images that rise 32 pixels
      // over the first 16 frames and drift right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise,
                  32, 32);
      }

      // Embers: 2 to 4 pixels by index, 2 more while falling, swelling to 10
      // over frames 69 to 77 as they clear; those falling are drawn
      // additively. Until frame 80 they fall under gNereid_Gravity, then
      // speed upwards; they slow to 62/64 a frame, and go out when they fall
      // below y 104.
      for (k = 0; k != NEREID_EMBERS; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          EFFECT_DRAW_MOTE(draw[spark->velocityY > 0], canvas, graphics,
                           Q_16_16_TO_INT(spark->x), Q_16_16_TO_INT(spark->y),
                           size);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += gNereid_Gravity[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && Q_16_16_TO_INT(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // The targets bounce and flash white on frames 36, 48, 60 and 72,
      // getting their colours back 6 frames later.
      if (frame <= 79) {
        EFFECT_FOR_EACH_TARGET(k, work->effect) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *obj =
                  GetBattleObjectSlotFar(work->effect->actors[k])->object;

              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, -1, 0);
              obj->velocityY = 0x48000;
              obj->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              ObjectGroup_UpdateMembers(work->effect->actors[k], 0, POSE_DAMAGE,
                                        -1, 0);
          }
        }
      }
      EFFECT_PRESENT_FRAME(work);
    }

    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleFx_RunImpactBurst(1, focus.x, focus.y);
    for (k = 0; k != NEREID_OBJECTS; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleFx_EndCanvasLayer();
}

#undef NEREID_FRAMES
#undef NEREID_OBJECTS
#undef NEREID_COLUMNS
#undef NEREID_EMBERS
#undef NEREID_COLUMN_IMAGE
