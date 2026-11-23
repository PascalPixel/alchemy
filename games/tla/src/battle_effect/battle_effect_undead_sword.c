/// @file
/// Undead Sword, Plume Edge and Diamond Berg, and the bodies of Dreamtide and
/// Watery Grave and of Waft and Aroma.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The water sheet's modes (BattleEffect_RunWaterSheet).
enum WaterSheetMode { WATER_SHEET_DREAMTIDE, WATER_SHEET_WATERY_GRAVE };

/// The wind funnel's modes (BattleEffect_RunWindFunnel).
enum WindFunnelMode { WIND_FUNNEL_WAFT, WIND_FUNNEL_AROMA };

/// The sparks, and the frames each lasts.
#define UNDEAD_SWORD_SPARK_COUNT 16
#define UNDEAD_SWORD_SPARK_LIFE 28
/// LimeColumnSheet: one column of light, 14 by 80.
#define LIME_COLUMN_WIDTH 14
#define LIME_COLUMN_HEIGHT 80
/// Where BlastSheet is decoded, after the column.
#define UNDEAD_SWORD_SPARK_SHEET PICTURE_BYTES(LIME_COLUMN)

#define UNDEAD_SWORD_FRAMES 54     ///< frames of the columns
#define UNDEAD_SWORD_HIT_FRAME 10  ///< the targets are hit
#define UNDEAD_SWORD_RISE_FRAME 8  ///< the columns break the ground
#define UNDEAD_SWORD_PEAK_FRAME 31 ///< the columns stop rising
/// The canvas line the columns stand on and the sparks start from.
#define UNDEAD_SWORD_GROUND 112

/// Two mirrored columns of light rise at the first target's feet and sink
/// again, scattering sparks that drift upward.
///
/// Abilities: Undead Sword.
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct ScreenPoint screenPosition;
  struct Particle *spark;
  struct Particle *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *src;
  s32 speed;
  s32 angle;
  s32 picture;
  s32 size;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;

  // Open the canvas and load the pictures.
  EFFECT_START_NARROW_CANVAS(0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET,
                             work->sheet + UNDEAD_SWORD_SPARK_SHEET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Slide the canvas so that its middle is under the target.
  EffectPosition_ProjectBattler(args->targets[0], &screenPosition);
  REG_BG2X = (CANVAS_SIZE / 2 - screenPosition.x) << 8;

  // The sparks start together at the columns' foot and fly apart, upward
  // faster than sideways.
  for (i = 0, spark = work->particles; i != UNDEAD_SWORD_SPARK_COUNT; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & TRIG_ANGLE_MASK;
    spark->x = toFIXED(CANVAS_SIZE / 2);
    spark->y = toFIXED(UNDEAD_SWORD_GROUND);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;

  for (frame = 0; frame != UNDEAD_SWORD_FRAMES; frame++) {
    // The hit: the damage shown, a shake, knock-back and a flash.
    if (frame == UNDEAD_SWORD_HIT_FRAME) {
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }

    // The columns: up 12 pixels a frame until frame 31, then down 6.
    if (frame >= UNDEAD_SWORD_RISE_FRAME) {
      if (frame <= UNDEAD_SWORD_PEAK_FRAME)
        height = (frame - UNDEAD_SWORD_RISE_FRAME) * 12;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > LIME_COLUMN_HEIGHT) {
          height = LIME_COLUMN_HEIGHT;
          mode = BLITTER_MAX;
        } else {
          mode = BLITTER_ADDITIVE;
        }
        // The column, then its mirror image on its right.
        for (i = 0, x = CANVAS_SIZE / 2 - LIME_COLUMN_WIDTH; i != 2;
             i++, x += LIME_COLUMN_WIDTH) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, mode);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
          (draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode)(
              canvas, work->sheet, x, UNDEAD_SWORD_GROUND - height,
              LIME_COLUMN_WIDTH, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // The sparks: a new blast picture every third frame, seven in all; the
    // pairs leave one frame apart from frame 8.
    BattleEffect_FetchRectangleBlitters(args->side, draw);
    for (i = 0, particle = work->particles; i != UNDEAD_SWORD_SPARK_COUNT;
         i++) {
      if (frame >= i / 2 + UNDEAD_SWORD_RISE_FRAME &&
          particle->variant <= UNDEAD_SWORD_SPARK_LIFE) {
        s32 drawX;
        s32 drawY;

        picture = particle->variant / 3;
        drawX = FIXED_INTEGER_PART(particle->x);
        drawY = FIXED_INTEGER_PART(particle->y);
        if (picture > 6)
          picture = 6;
        src = work->sheet + gUndeadSword_SparkPictures[picture] +
              UNDEAD_SWORD_SPARK_SHEET;
        size = gUndeadSword_SparkSizes[picture];
        draw[0](canvas, src, drawX - size / 2, drawY - size / 2, size, size);
        particle->variant++;
        Particle_AdvanceWithGravity2D(particle, 62, -toFIXED(1) / 8);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();

    if (frame < UNDEAD_SWORD_RISE_FRAME)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Where FlashBurstSheet is decoded in the effect's pictures: its six
/// pictures, in Mercury's colours, are a puff of foam bursting and fading.
#define PLUME_EDGE_FOAM_PICTURES 8192

#define PLUME_EDGE_FRAMES 80

/// Where BillowTexture is decoded in the effect's pictures.
#define PLUME_EDGE_BILLOW 18192

/// The puffs of foam drawn, one more each frame from frame 0, each for
/// PLUME_EDGE_FOAM_FRAMES: its six pictures, three frames apiece.
#define PLUME_EDGE_FOAM_PUFFS 32
#define PLUME_EDGE_FOAM_FRAMES 18
/// The geyser lifts the target this many frames, two pixels a frame.
#define PLUME_EDGE_LIFT_FRAMES 24
/// The frame the geyser bursts into its drops; it is drawn until then.
#define PLUME_EDGE_BURST_FRAME 32
/// The drops it bursts into, in PARTICLES from PLUME_EDGE_FIRST_DROP.
#define PLUME_EDGE_FIRST_DROP 256
#define PLUME_EDGE_DROPS 256
/// The geyser's foot, below the target's place; it rises 8 pixels a frame.
#define PLUME_EDGE_GEYSER_FOOT 8
#define PLUME_EDGE_GEYSER_SPEED 8

/// A geyser of water erupts under the target in billows of foam and lifts it;
/// the user dashes in, and the geyser bursts into a spray of drops as the
/// canvas flashes white.
///
/// Abilities: Plume Edge.
void BattleEffect_RunPlumeEdge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct MotionObject *target;
  s32 geyserY;
  DrawRectangle draw[2];
  struct ScreenPoint position;
  struct Particle *particle;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  geyserY = 0;
  // DEAD: nothing here draws BillowTexture; the ROM keeps the call.
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
                             work->sheet + PLUME_EDGE_BILLOW, 0, 0);
  Iwram_CopyWordsEntry(
      (void *)BG_PLTT,
      Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET),
      SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             work->sheet + PLUME_EDGE_FOAM_PICTURES, 1, 0);
  EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);

  for (frame = 0; frame != PLUME_EDGE_FRAMES; frame++) {
    EffectPosition_ProjectBattler(args->targets[0], &position);

    // Frame 0: the puffs of foam are set to fan out upward from the target,
    // one more each frame, and the target takes its damage pose, its gravity
    // turned off.
    if (frame == 0) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = (Random16() & 0x7fff) - DEGtoANG(90);

        particle = &work->particles[i];
        particle->x = INTtoFIXED(position.x / 2);
        particle->y = INTtoFIXED(position.y);
        particle->velocityX = (Trig_Sin(angle) * speed) >> 8;
        particle->velocityY = -(Trig_Cos(angle) * speed * 2) >> 8;
        particle->variant = -i;
      }
      target->verticalMotionStrength = 0;
      target->y = 0;
      geyserY = position.y;
      BattleEffect_TintBattler(args->targets[0], TINT_KEEP, POSE_DAMAGE, 0, -1);
      work->shakeFrames = 4;
      Sound_PlayFar(SE_CLATTER);
    }

    // The burst: the geyser bursts into its drops, the target is knocked back
    // and flashes, and the whole canvas flashes white.
    if (frame == PLUME_EDGE_BURST_FRAME) {
      for (i = 0; i != PLUME_EDGE_DROPS; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = Random16();

        particle = &PARTICLES[PLUME_EDGE_FIRST_DROP + i];
        particle->x = INTtoFIXED(position.x / 2);
        particle->y = INTtoFIXED(position.y - 16);
        particle->velocityX = (Trig_Sin(angle) * speed) >> 7;
        particle->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        particle->variant = (Random16() & 7) + 48;
      }
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }

    // The geyser lifts the target while the user dashes in and leaps.
    if (frame >= 0 && frame < PLUME_EDGE_LIFT_FRAMES)
      target->y += toFIXED(2);
    if (frame == 18)
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 80);
    if (frame == 21)
      BattleMotion_ApproachTargetFar(args->user, args->targets[0], 12,
                                     toFIXED(8));

    // The drops fall and shrink: a drop is drawn at its life / 6 pixels wide,
    // two pixels at the least.
    particle = PARTICLES;
    for (i = 0; i != PLUME_EDGE_FIRST_DROP + PLUME_EDGE_DROPS;
         i++, particle++) {
      if (particle->variant >= 0) {
        s32 size = particle->variant / 6;

        if (size < 2)
          size = 2;
        particle->variant--;
        Particle_AdvanceWithGravity2D(particle, 60, toFIXED(1) / 8);
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet,
                         FIXED_INTEGER_PART(particle->x),
                         FIXED_INTEGER_PART(particle->y), size);
      }
    }

    // Each puff of foam billows out and fades through its six pictures.
    for (i = 0; i != PLUME_EDGE_FOAM_PUFFS; i++) {
      particle = &work->particles[i];
      particle->variant++;
      if (particle->variant >= 0 &&
          particle->variant < PLUME_EDGE_FOAM_FRAMES) {
        s32 picture = particle->variant / 3;

        Particle_AdvanceWithGravity2D(particle, 60, -toFIXED(1) / 8);
        draw[0](canvas,
                work->sheet + PLUME_EDGE_FOAM_PICTURES +
                    picture * PICTURE_BYTES(FLASH_BURST),
                FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
                FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
                FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      }
    }

    // The geyser: its picture scrolls up from its foot, the part that has
    // risen drawn again below it.
    if (frame >= 0 && frame < PLUME_EDGE_BURST_FRAME) {
      s32 scroll = (frame * PLUME_EDGE_GEYSER_SPEED) % FIRE_PILLAR_HEIGHT;

      draw[0](canvas, work->sheet, position.x / 2 - FIRE_PILLAR_WIDTH / 2,
              geyserY + PLUME_EDGE_GEYSER_FOOT - scroll - FIRE_PILLAR_HEIGHT,
              FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
      draw[0](canvas, work->sheet, position.x / 2 - FIRE_PILLAR_WIDTH / 2,
              geyserY - scroll + PLUME_EDGE_GEYSER_FOOT, FIRE_PILLAR_WIDTH,
              scroll);
    }

    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Copies a width by height picture into a picture twice its size, mirrored
/// into all four quadrants.
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height) {
  s32 x;
  s32 y;

  for (y = 0; y != height; y++) {
    for (x = 0; x != width; x++) {
      u8 c = src[y * width + x];

      dst[y * width * 2 + x] = c;
      dst[y * 2 * width + (width * 2 - 1 - x)] = c;
      dst[(height * 2 - 1 - y) * 2 * width + x] = c;
      dst[(height * 2 - 1 - y) * 2 * width + (width * 2 - 1 - x)] = c;
    }
  }
}

/// Abilities: Dreamtide.
void BattleEffect_RunDreamtide(struct BattleEffectParams *args) {
  BattleEffect_RunWaterSheet(args, WATER_SHEET_DREAMTIDE);
}

/// Abilities: Watery Grave.
void BattleEffect_RunWateryGrave(struct BattleEffectParams *args) {
  BattleEffect_RunWaterSheet(args, WATER_SHEET_WATERY_GRAVE);
}

/// A corner of the water sheet: x, y and depth, a byte each.
struct WaterVertex {
  s32 x : 8;
  s32 y : 8;
  s32 z : 8;
};

/// The water's 16 by 8 vertices, at the head of the map cell buffer.
#define WATER_VERTICES ((struct WaterVertex *)Ram_MapCellBuffer)

/// One triangle of the water's mesh: three vertex indices, then a texture
/// coordinate for each.
struct WaterTriangle {
  u32 vertexA : 8;
  u32 vertexB : 8;
  u32 vertexC : 8;
  u32 : 8;
  u32 u0 : 8;
  u32 v0 : 8;
  u32 u1 : 8;
  u32 v1 : 8;
  u32 u2 : 8;
  u32 v2 : 8;
};

/// The water's grid of quads, across and down.
#define WATER_COLUMNS 16
#define WATER_ROWS 8
/// A quad of the grid spans this many texels across and down; the texture
/// wraps every 64.
#define WATER_QUAD_TEXELS_ACROSS 8
#define WATER_QUAD_TEXELS_DOWN 16
#define WATER_QUADS ((WATER_ROWS - 1) * (WATER_COLUMNS - 1))
/// Where the water's mesh is built in the map cell buffer.
#define WATER_MESH_OFFSET 16384
/// The water's mesh as triangles, two to a quad.
#define WATER_MESH                                                             \
  ((struct WaterTriangle *)(Ram_MapCellBuffer + WATER_MESH_OFFSET))

/// Where SwirlTexture is decoded in the effect's pictures.
#define WATER_SHEET_SWIRL 8192

/// Where SmokeSheet is decoded in the effect's pictures, and its six puffs.
#define WATER_SHEET_SMOKE 12288
#define WATER_SHEET_SMOKE_PICTURE_COUNT 6

/// Where the 32x32 picture at the sheet's start is mirrored into a 64x64 one.
#define WATER_SHEET_MIRRORED 1024

#define DREAMTIDE_FRAMES 48
#define WATERY_GRAVE_FRAMES 81
/// The camera turns WATER_SHEET_CAMERA_TURN a frame until its yaw reaches
/// WATER_SHEET_CAMERA_LIMIT either way.
#define WATER_SHEET_CAMERA_TURN 128
#define WATER_SHEET_CAMERA_LIMIT 12000
/// The angle a wave's phase moves on from one column of the grid to the next,
/// and the one each row's phase is the sine of a multiple of.
#define WATER_WAVE_COLUMN_PHASE 5000
#define WATER_WAVE_ROW_PHASE 3553
/// Watery Grave's puffs of smoke, in work->particles; one is drawn only
/// beyond WATERY_GRAVE_PUFF_DEPTH.
#define WATERY_GRAVE_PUFFS 20
#define WATERY_GRAVE_PUFF_DEPTH 10
/// Watery Grave tilts its water towards the camera.
#define WATERY_GRAVE_TILT 4000
/// The water slides in from WATER_SHEET_START_X to WATER_SHEET_END_X, 8
/// pixels a frame.
#define WATER_SHEET_START_X 128
#define WATER_SHEET_END_X (-64)

/// The body of Dreamtide and Watery Grave: a sheet of
/// water (a 16 by 8 grid of vertices, its mesh built here) rolls in under
/// the targets in waves while the camera turns; Dreamtide strikes the first
/// target with two splashes at frame 17, Watery Grave strikes every target
/// at frames 17 and 46 while puffs of smoke bob on the water.
void BattleEffect_RunWaterSheet(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  s32 frame;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  void *canvas;
  struct BattleCamera *camera;
  s16 savedYaw;
  s32 frames;
  struct ScreenPoint screen;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *puff;
  s32 height;
  s32 phase;
  s32 row;
  s32 column;
  u8 *mesh;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  mesh = Ram_MapCellBuffer + WATER_MESH_OFFSET;
  if (mode == WATER_SHEET_DREAMTIDE) {
    REG_BLDALPHA = BLEND_ALPHA(16, 12);
  } else {
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = (s16)camera->yaw;
  Resource_LoadAndDecompress(RESOURCE_CRACKED_TEXTURE_SHEET, work->sheet, 1, 1);
  Picture_MirrorIntoQuadrants(work->sheet, work->sheet + WATER_SHEET_MIRRORED,
                              32, 32);
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                             work->sheet + WATER_SHEET_SWIRL, 0, 0);
  if (mode == WATER_SHEET_WATERY_GRAVE) {
    u8 *pixel;

    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                               work->sheet + WATER_SHEET_SMOKE, 1, 0);
    for (i = 0, pixel = work->sheet + WATER_SHEET_SMOKE;
         i != WATER_SHEET_SMOKE_PICTURE_COUNT * PICTURE_BYTES(SMOKE_PUFF);
         i++, pixel++) {
      if (*pixel != 0) {
        *pixel += 8;
        if (*pixel > CANVAS_LEVEL_BRIGHTEST) {
          *pixel = CANVAS_LEVEL_BRIGHTEST;
        }
      }
    }
    LOAD_BG_PALETTE(RESOURCE_CRYSTAL_SHEET);
  }

  // The water's grid: 16 by 8 vertices, and a quad (two triangles) for
  // each cell but the last row and column. A row's first vertex index is also
  // the texture's v at its top, a row being as many texels down as it has
  // vertices.
  for (row = 0; row != WATER_ROWS; row++) {
    s32 v = row * WATER_COLUMNS;
    /* FAKEMATCH(dead): the row's first vertex index with the row added and
     * taken away again. */
    s32 q = v + row - row;

    for (column = 0; column != WATER_COLUMNS; column++) {
      s32 a = row * WATER_COLUMNS + column;
      /* FAKEMATCH(dead): the vertex's index with q added and v, its equal,
       * taken away. */
      struct WaterVertex *vertex =
          (struct WaterVertex *)((s8 *)gMapGridCellBuffer + (a + q - v) * 4);
      s32 u = column * WATER_QUAD_TEXELS_ACROSS;

      SET_POSITION(vertex, column * 11 - 82, 60, row * 24 - 84);
      if (row < WATER_ROWS - 1 && column < WATER_COLUMNS - 1) {
        struct WaterTriangle *quad = (struct WaterTriangle *)mesh +
                                     (row * (WATER_COLUMNS - 1) + column) * 2;
        s32 b = q + column + 1;
        s32 d = q + column + WATER_COLUMNS + 1;
        s32 c = q + column + WATER_COLUMNS;

        /* FAKEMATCH(dead): a store the frame count below overwrites. */
        frames = (column * 8 - 60) & 1;
        quad[0].u0 = u + WATER_QUAD_TEXELS_ACROSS - 1;
        quad[0].v0 = q;
        quad[0].u1 = u + WATER_QUAD_TEXELS_ACROSS - 1;
        quad[0].v1 = v + WATER_QUAD_TEXELS_DOWN - 1;
        quad[0].u2 = u;
        quad[0].v2 = v + WATER_QUAD_TEXELS_DOWN - 1;
        quad[1].u0 = u;
        quad[1].v0 = q;
        quad[1].u1 = u + WATER_QUAD_TEXELS_ACROSS - 1;
        quad[1].v1 = q;
        quad[1].u2 = u;
        quad[1].v2 = v + WATER_QUAD_TEXELS_DOWN - 1;
        quad[0].vertexA = b;
        quad[0].vertexB = d;
        quad[0].vertexC = c;
        quad[1].vertexA = a;
        quad[1].vertexB = b;
        quad[1].vertexC = c;
      }
    }
  }
  // A triangle of three zero indices ends the mesh.
  WATER_MESH[WATER_QUADS * 2].vertexA = 0;
  WATER_MESH[WATER_QUADS * 2].vertexB = 0;
  WATER_MESH[WATER_QUADS * 2].vertexC = 0;
  frames = DREAMTIDE_FRAMES;
  if (mode != WATER_SHEET_DREAMTIDE) {
    frames = WATERY_GRAVE_FRAMES;
  }

  for (frame = 0; frame != frames; frame++) {
    void *vertices;
    s32 shade;
    s32 angle;

    if (frame == 0) {
      camera->yaw = savedYaw;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if ((s16)camera->yaw < WATER_SHEET_CAMERA_LIMIT) {
        camera->yaw = (s16)camera->yaw + WATER_SHEET_CAMERA_TURN;
      }
    } else if ((s16)camera->yaw > -WATER_SHEET_CAMERA_LIMIT) {
      camera->yaw = (s16)camera->yaw - WATER_SHEET_CAMERA_TURN;
    }

    if (mode == WATER_SHEET_DREAMTIDE) {
      if (frame == 17) {
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, toFIXED(8),
                                      100);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 16);
        work->shakeFrames = 8;
      }
    } else {
      if (frame == 17) {
        Sound_PlayFar(SE_CRASH);
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
      }
      if (frame == 46) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0, toFIXED(8),
                                        100);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
      }
    }
    if (mode == WATER_SHEET_DREAMTIDE) {
      if ((u32)(frame - 33) <= 14) {
        REG_BLDALPHA = BLEND_ALPHA(48 - frame, BLEND_WEIGHT_MAX);
      }
    } else if ((u32)(frame - 65) <= 14) {
      REG_BLDALPHA = BLEND_ALPHA(80 - frame, BLEND_WEIGHT_MAX);
    }

    vertices = Heap_AllocEwram(1024);
    model = Render_AllocateModelList(1);
    shade = 0;

    // The waves rise, then ebb.
    if (mode == WATER_SHEET_DREAMTIDE) {
      if (frame <= 31) {
        height = frame * 4;
        if (height > 64) {
          height = 64;
        }
      } else {
        height = 320 - frame * 8;
        if (height < 0) {
          height = 0;
        }
      }
    } else if (frame <= 63) {
      height = frame * 2;
      if (height > 64) {
        height = 64;
      }
    } else {
      height = 576 - frame * 8;
      if (height < 0) {
        height = 0;
      }
    }
    phase = frame * 2;
    row = 1;
    angle = frame * (TRIG_TURN / 32);
    for (; row != WATER_ROWS - 1; row++) {
      for (column = 0; column != WATER_COLUMNS; column++) {
        s32 amplitude = (16 - column) * height / 32;
        s32 y = (FIXEDtoINT(Trig_Sin(angle + column * WATER_WAVE_COLUMN_PHASE +
                                     Trig_Sin(row * WATER_WAVE_ROW_PHASE)) *
                            amplitude)) -
                amplitude;

        WATER_VERTICES[row * WATER_COLUMNS + column].y = y + 64;
      }
    }
    // A puff bobs at half the waves' height, with the row it rises in: the
    // puffs lie a quarter of a row apart from row 1.
    i = 0;
    height /= 2;
    for (puff = work->particles; i != WATERY_GRAVE_PUFFS; i++, puff++) {
      if (puff->variant == 0) {
        puff->y = INTtoFIXED(
            (FIXEDtoINT(Trig_Sin(angle + Trig_Sin((i + 4) *
                                                  (WATER_WAVE_ROW_PHASE / 4))) *
                        height)) -
            height + 64);
      }
    }

    texture.logWidth = 6;
    texture.logHeight = 6;
    texture.pixels = work->sheet + WATER_SHEET_MIRRORED;
    Matrix_Initialize();
    if (mode != WATER_SHEET_DREAMTIDE) {
      Matrix_RotateX(WATERY_GRAVE_TILT);
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      Matrix_RotateY((s16)camera->yaw);
    } else {
      Matrix_RotateY((s16)camera->yaw + DEGtoANG(180));
    }
    {
      s32 shift = WATER_SHEET_START_X - frame * 8;

      if (shift < WATER_SHEET_END_X) {
        shift = WATER_SHEET_END_X;
      }
      Matrix_Translate(INTtoFIXED(shift), 0, 0);
    }
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &texture;
    model->mesh = mesh;
    model->vertices = vertices;
    model->shade = shade;
    model->scrollU = (frame + phase) & TEXTURE_SCROLL_MASK;
    model->scrollV = TEXTURE_SCROLL_MASK & ~frame;
    Render_TransformVertices(gMapGridCellBuffer, vertices,
                             WATER_ROWS * WATER_COLUMNS);
    Render_DrawModelList(model);

    if (frame == 0) {
      FIXED puffZ = -toFIXED(60);

      for (i = 0, puff = work->particles; i != WATERY_GRAVE_PUFFS;
           i++, puff++) {
        puff->variant = Random16() % WATER_SHEET_SMOKE_PICTURE_COUNT;
        puff->z = puffZ;
        puff->x = -toFIXED(60);
        puff->velocityX = 0;
        puff->velocityY = 0;
        puff->velocityZ = 0;
        puffZ += toFIXED(6);
      }
    }
    if (mode == WATER_SHEET_WATERY_GRAVE) {
      for (i = 0; i != WATERY_GRAVE_PUFFS; i++) {
        struct Particle *smoke = &work->particles[i];
        s32 picture = smoke->variant;
        struct ScreenPoint *position;

        if (picture < 0) {
          continue;
        }
        /* FAKEMATCH(temporary): position holds &screen. */
        position = &screen;
        if (EffectPosition_ProjectPoint(smoke, position) == 0 &&
            FIXED_INTEGER_PART(position->depth) > WATERY_GRAVE_PUFF_DEPTH) {
          s32 x = FIXED_INTEGER_PART(position->x);
          s32 y;

          position->x = x + 64;
          y = FIXED_INTEGER_PART(position->y);
          position->y = y + 64;
          blit[0](canvas,
                  work->sheet + WATER_SHEET_SMOKE +
                      picture * PICTURE_BYTES(SMOKE_PUFF),
                  x + 52, y + 40, SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
          Particle_AdvanceWithGravity3D(smoke, 62, -toFIXED(1) / 8);
        }
        if (++smoke->variant == WATER_SHEET_SMOKE_PICTURE_COUNT) {
          smoke->variant = 0;
        }
      }
    } else {
      s32 t;

      EffectPosition_ProjectBattler(args->targets[0], &target);
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (i = 0; i != 2; i++) {
        t = frame - 16 - i * 4;
        if (t < 0) {
          continue;
        }
        texture.pixels = work->sheet + WATER_SHEET_SWIRL;
        if (t > 7) {
          shade = (8 - t) * 4;
        }
        model->shade = shade;
        if (shade <= MODEL_SHADE_INVISIBLE) {
          continue;
        }
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(target.x / 2 - 64),
                         INTtoFIXED(target.y - 64), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateZ(i * (DEGtoANG(90)) + DEGtoANG(45));
        Matrix_RotateX(TRIG_TURN / 16);
        Matrix_RotateY(angle);
        Matrix_ScaleUniform(t * (FIXED_ONE * 3 / 16));
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Waft.
void BattleEffect_RunWaft(struct BattleEffectParams *args) {
  BattleEffect_RunWindFunnel(args, WIND_FUNNEL_WAFT);
}

/// Abilities: Aroma.
void BattleEffect_RunAroma(struct BattleEffectParams *args) {
  BattleEffect_RunWindFunnel(args, WIND_FUNNEL_AROMA);
}

/// The funnel is a band of WIND_FUNNEL_SEGMENTS segments, each two corners
/// WIND_FUNNEL_BAND apart in height, laid in the map cell buffer: the corners,
/// then those Render_TransformVertices makes of them, then the band's mesh.
#define WIND_FUNNEL_SEGMENTS 33
#define WIND_FUNNEL_CORNERS (WIND_FUNNEL_SEGMENTS * 2)
#define WIND_FUNNEL_BAND 24
/// The funnel's head goes round a circle of this radius.
#define WIND_FUNNEL_RADIUS 32
#define WIND_FUNNEL_VERTICES                                                   \
  (Ram_MapCellBuffer + WIND_FUNNEL_CORNERS * sizeof(struct ModelVertex))
#define WIND_FUNNEL_MESH                                                       \
  (WIND_FUNNEL_VERTICES + WIND_FUNNEL_CORNERS * sizeof(struct ProjectedVertex))
/// The band's quads, each 4 texels of StreakTexture across and its 32 down.
#define WIND_FUNNEL_QUADS (WIND_FUNNEL_SEGMENTS - 1)

/// Where JupiterDjinnSmallSheet is decoded in the effect's pictures.
#define WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES 8192

/// How high the funnel's head stands, and how far it dips and rises again, an
/// angle of WIND_FUNNEL_DIP_SPEED a frame.
#define WAFT_FUNNEL_HEIGHT 80
#define AROMA_FUNNEL_HEIGHT 30
#define AROMA_FUNNEL_DIP 20
#define WIND_FUNNEL_DIP_SPEED 820
/// The funnel is drawn three times, each turned WIND_FUNNEL_SPREAD further.
#define WIND_FUNNEL_SPREAD 21000
/// Waft's funnels swell and shrink about WAFT_FUNNEL_SCALE, an angle of
/// WAFT_FUNNEL_PULSE_SPEED a frame; Aroma's about full size.
#define WAFT_FUNNEL_SCALE (FIXED_ONE * 2673 / 2048)
#define WAFT_FUNNEL_PULSE_SPEED 964
#define AROMA_FUNNEL_PULSE_SPEED 682
/// Aroma's three funnels stand one above another, this far apart.
#define AROMA_FUNNEL_SPACING 16

/// The body of Waft and Aroma: after the Jupiter Djinni's
/// entrance a small Djinni bobs and rises, while three funnels of wind, each a
/// band whose head moves and whose tail follows where the head has been, twist
/// over the targets and take on Waft's cyan or Aroma's tan; Waft strikes the
/// first target at frame 28, Aroma touches each target in turn, four frames
/// apart.
void BattleEffect_RunWindFunnel(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  struct BattleCamera *camera;
  struct ScreenPoint other;
  struct ScreenPoint screen;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 yaw;
  s32 frame;
  s32 frames;
  s32 i;
  s32 k;
  u8 *vertices;
  u8 *funnel;
  void (*transfer)(void);

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  // The funnel's corners, two to a segment, from its head.
  funnel = gMapGridCellBuffer;
  BattleEffect_StartCanvasLayer(0);
  if (mode == WIND_FUNNEL_WAFT) {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_OVER_TARGET),
        (s32 *)&screen, (s32 *)&other);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  } else {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_TARGET_X),
        (s32 *)&screen, (s32 *)&other);
  }
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  transfer = BattleEffect_ProcessPendingGraphicsTransfer;
  Task_Add(transfer, TASK_PASS_VBLANK);
  yaw = 0; // Default: set again at frame 0 before it is read.
  frames = 36;
  if (mode == WIND_FUNNEL_AROMA && args->targetCount > 4) {
    frames = args->targetCount * 6 + 12;
  }
  Resource_LoadAndDecompress(RESOURCE_STREAK_TEXTURE, work->sheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(
      RESOURCE_JUPITER_DJINN_SMALL_SHEET,
      work->sheet + WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES, 1, 1);
  Render_BuildQuadStrip(WIND_FUNNEL_MESH, 4, 32, WIND_FUNNEL_QUADS);

  for (frame = 0; frame != frames; frame++) {
    s32 shade;
    s32 height;
    s32 y;
    s32 angle;
    struct ModelVertex *corner;
    s32 x; // The Djinni's x; at frame 0, the funnel's height.

    if (frame == 0) {
      struct ModelVertex *head;
      s32 row;
      s32 radius;

      yaw = 0;
      /* FAKEMATCH(temporary): radius, WIND_FUNNEL_RADIUS held in a local set
       * in both arms; as the constant at its use. */
      if (mode == WIND_FUNNEL_WAFT) {
        x = WAFT_FUNNEL_HEIGHT;
        radius = WIND_FUNNEL_RADIUS;
      } else {
        x = AROMA_FUNNEL_HEIGHT;
        radius = WIND_FUNNEL_RADIUS;
      }
      for (i = 0; i != WIND_FUNNEL_SEGMENTS; i++) {
        row = i * 8;
        for (k = 0; k != 2; k++) {
          head = (struct ModelVertex *)(gMapGridCellBuffer + row) + k;
          head->y = -(x + k * WIND_FUNNEL_BAND);
          head->x = 0;
          head->z = radius;
        }
      }
      Sound_PlayFar(SE_WIND_FUNNEL);
    }
    if (frame > 19) {
      if (mode == WIND_FUNNEL_WAFT) {
        BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);
      } else if (mode == WIND_FUNNEL_AROMA) {
        BattleEffect_StepPaletteToResource(RESOURCE_TAN_PALETTE);
      }
    }

    // The Djinni, bobbing over the caster and rising from frame 8.
    if (frame <= 79) {
      s32 y;

      x = (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * 8)) + screen.x / 2 -
          SMALL_DJINNI_WIDTH / 2;
      y = (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4)) + screen.y - 24;

      if (frame > 7) {
        y -= (frame - 8) * 4;
      }
      blit[0](canvas, work->sheet + WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES, x,
              y, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }

    // The funnel's tail follows its head: each segment takes the corners of
    // the one before, every frame but the even ones from 32 to 47.
    if ((frame & 1) || frame > 47 || frame <= 31) {
      struct ModelVertex *corner;
      s32 i;
      for (i = WIND_FUNNEL_QUADS; i != 0; i--) {
        k = 0;
        corner = (struct ModelVertex *)gMapGridCellBuffer + (i - 1) * 2;
        for (; k != 2; k++) {
          corner[2].x = corner[0].x;
          corner[2].y = corner[0].y;
          corner[2].z = corner[0].z;
          corner++;
        }
      }
    }
    // The funnels turn a 32nd of a turn a frame, slowing from frame 24.
    if (frame > 23) {
      yaw += TRIG_TURN / 32 - (frame - 24) * 16;
    } else {
      yaw += TRIG_TURN / 32;
    }
    if (mode == WIND_FUNNEL_WAFT) {
      angle = frame * (TRIG_TURN / 16);
      height = WAFT_FUNNEL_HEIGHT -
               (FIXEDtoINT(Trig_Sin(frame * WIND_FUNNEL_DIP_SPEED) *
                           WAFT_FUNNEL_HEIGHT));
    } else {
      angle = yaw;
      height = AROMA_FUNNEL_HEIGHT -
               (FIXEDtoINT(Trig_Sin(frame * WIND_FUNNEL_DIP_SPEED) *
                           AROMA_FUNNEL_DIP));
    }
    k = 0;
    corner = (struct ModelVertex *)funnel;
    y = -height;
    for (; k != 2; k++) {
      // The (u32) casts make each division round down, as a shift.
      corner->x = (u32)Trig_Sin(angle) / (FIXED_ONE / WIND_FUNNEL_RADIUS);
      corner->y = y;
      corner->z = (u32)Trig_Cos(angle) / (FIXED_ONE / WIND_FUNNEL_RADIUS);
      y -= WIND_FUNNEL_BAND;
      corner++;
    }

    if (mode == WIND_FUNNEL_WAFT) {
      if (frame == 28) {
        work->shakeFrames = 6;
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
    } else {
      if (frame == 12) {
        EFFECT_SHOW_DAMAGE(-1);
      }
      for (k = 0; k != args->targetCount; k++) {
        if (frame == k * 4 + 12) {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, k, 8);
        }
      }
    }

    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    vertices = WIND_FUNNEL_VERTICES;
    model = Render_AllocateModelList(1);
    shade = 0;
    if (frame <= 15) {
      shade = frame * 4 + MODEL_SHADE_INVISIBLE;
      if (shade > 0) {
        shade = 0;
      }
    } else if (frame > 27) {
      shade = (28 - frame) * 8;
      if (shade < MODEL_SHADE_INVISIBLE) {
        shade = MODEL_SHADE_INVISIBLE;
      }
    }
    texture.logWidth = 7;
    texture.logHeight = 5;
    texture.pixels = work->sheet;
    model->kind = MODEL_SHADED_MAX;
    model->texture = &texture;
    model->mesh = WIND_FUNNEL_MESH;
    model->scrollU = 0;
    model->scrollV = 0;
    gProjection.centerY = 104;
    model->vertices = vertices;
    model->shade = shade;
    for (k = 0; k != gWindFunnel_Counts[mode]; k++) {
      Matrix_Initialize();
      if (mode == WIND_FUNNEL_WAFT) {
        s32 scale;

        Matrix_Translate(INTtoFIXED(screen.x / 2 - 64), toFIXED(40), 0);
        Matrix_RotateX(k * (TRIG_TURN / 32));
        Matrix_RotateY(k * WIND_FUNNEL_SPREAD + yaw);
        scale = WAFT_FUNNEL_SCALE - Trig_Sin(frame * WAFT_FUNNEL_PULSE_SPEED);
        // From frame 28 the funnels widen as they fade.
        if (frame > 27) {
          scale += (frame - 28) * (FIXED_ONE / 32);
        }
        Matrix_Scale(scale * 3 / 2, FIXED_ONE, scale * 3 / 2);
      } else {
        s32 scale;

        Matrix_Translate(INTtoFIXED(screen.x / 2 - 64),
                         INTtoFIXED(k * AROMA_FUNNEL_SPACING) + toFIXED(16), 0);
        Matrix_RotateX(k * (TRIG_TURN / 16) - TRIG_TURN / 32);
        Matrix_RotateY(k * WIND_FUNNEL_SPREAD + yaw);
        scale = FIXED_ONE - Trig_Sin(frame * AROMA_FUNNEL_PULSE_SPEED) / 4;
        if (frame > 27) {
          scale += (frame - 28) * (FIXED_ONE / 32);
        }
        Matrix_Scale(scale * 2, FIXED_ONE, scale * 2);
      }
      Render_TransformVertices(funnel, WIND_FUNNEL_VERTICES,
                               WIND_FUNNEL_CORNERS);
      Render_DrawModelList(model);
    }
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
    Heap_Release(model);
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(transfer);
  BattleEffect_EndCanvasLayer();
}

#define DIAMOND_BERG_FRAMES 148

/// Where CrystalBitmapSheet, the iceberg (48 by 104), is decoded in the map
/// cell buffer.
#define DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES 20000
#define DIAMOND_BERG_WIDTH 48
#define DIAMOND_BERG_HEIGHT 104
/// The berg drops DIAMOND_BERG_DROP_SPEED lines a frame, then each of
/// DIAMOND_BERG_BLOWS blows drives it DIAMOND_BERG_BLOW_DEPTH further, and the
/// ground cuts it off at DIAMOND_BERG_GROUND.
#define DIAMOND_BERG_DROP_SPEED 16
#define DIAMOND_BERG_BLOWS 8
#define DIAMOND_BERG_BLOW_DEPTH 8
#define DIAMOND_BERG_GROUND 108

/// Where BillowTexture is decoded in the map cell buffer, after the frost's
/// FlameBandTexture.
#define DIAMOND_BERG_BILLOW 4096

/// Where FireRingTexture is decoded in the effect's pictures.
#define DIAMOND_BERG_FIRE_RING 12416

/// The shards: DIAMOND_BERG_CRASH_SHARDS when the berg lands, then
/// DIAMOND_BERG_BLOW_SHARDS at each blow, in the slots after them; a shard
/// bounces on DIAMOND_BERG_SHARD_GROUND and is drawn until it has spun
/// DIAMOND_BERG_SHARD_SPIN.
#define DIAMOND_BERG_CRASH_SHARDS 32
#define DIAMOND_BERG_BLOW_SHARDS 8
#define DIAMOND_BERG_SHARD_GROUND 104
#define DIAMOND_BERG_SHARD_SPIN 1024
/// The ripples of frost lean towards the camera, each starting at about a
/// quarter of its size and growing by its row's growth times
/// DIAMOND_BERG_RIPPLE_GROWTH a frame.
#define DIAMOND_BERG_RIPPLE_TILT 3000
#define DIAMOND_BERG_RIPPLE_START_SCALE (FIXED_ONE * 131 / 512)
#define DIAMOND_BERG_RIPPLE_GROWTH 1400

/// An iceberg drops onto the target and is driven down in eight blows (every
/// eighth frame from 31), each throwing eight shards and flaring a ring; the
/// last shatters it in a flash. Ripples of frost spread beneath.
///
/// Abilities: Diamond Berg.
void BattleEffect_RunDiamondBerg(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  struct ScreenPoint pos;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 centerX;
  s32 top;
  s32 frame;
  s32 i;
  s32 k;
  u8 *cells;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EffectPosition_ProjectBattler(args->targets[0], &pos);
  centerX = pos.x / 2;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_CRYSTAL_SHEET, work->sheet, 1, 0);
  cells = gMapGridCellBuffer;
  Resource_LoadAndDecompress(RESOURCE_CRYSTAL_BITMAP_SHEET,
                             cells + DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, cells, 0, 0);
  // DEAD: nothing draws BillowTexture, as the ripples read only the band's 16
  // rows before it; the ROM keeps the call.
  Resource_LoadAndDecompress(RESOURCE_BILLOW_TEXTURE,
                             cells + DIAMOND_BERG_BILLOW, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
                             work->sheet + DIAMOND_BERG_FIRE_RING, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // DEAD: frame 0 sets top again before anything reads it; the ROM keeps the
  // store.
  top = -128;

  for (frame = 0; frame != DIAMOND_BERG_FRAMES; frame++) {
    if (frame == 0) {
      top = -DIAMOND_BERG_HEIGHT;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame <= 63) {
      EffectPosition_ProjectBattler(args->targets[0], &target);
    }
    if (frame == 20) {
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 60);
    }
    if (frame == 2) {
      Sound_PlayFar(SE_POWER_UP);
    }

    // The berg: it drops, then each blow drives it further down.
    if (frame <= 127) {
      s32 height = DIAMOND_BERG_HEIGHT;

      if (frame <= 8) {
        top += DIAMOND_BERG_DROP_SPEED;
      } else if ((u32)(frame - 24) <= 64) {
        s32 t = frame - 24;
        s32 beat = t - t / 8 * 8; // t % 8

        if (beat == 4) {
          Sound_PlayFar(SE_POWER_UP);
        }
        if (beat == 7) {
          work->shakeFrames = 4;
          if (frame <= 79) {
            Sound_PlayFar(SE_CLATTER);
            BattleEffect_SetApproachMotion(args->user, args->targets[0], 16,
                                           70);
          } else {
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                                 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 6);
          top += DIAMOND_BERG_BLOW_DEPTH;
          for (k = 0; k != DIAMOND_BERG_BLOW_SHARDS; k++) {
            struct Particle *shard;

            shard = &work->particles[((t - 7) / 8 * DIAMOND_BERG_BLOW_SHARDS +
                                      k + DIAMOND_BERG_CRASH_SHARDS) &
                                     (ARRAY_COUNT(work->particles) - 1)];
            shard->y = toFIXED(96);
            shard->x = INTtoFIXED(centerX);
            shard->velocityX = ((Random16() & 127) - 64) << 11;
            shard->velocityY = -((Random16() & 127) + 64) << 11;
            shard->z = 32;
            shard->variant = 0;
          }
        }
      }
      // The ground cuts off what of the berg is below it.
      if (top + DIAMOND_BERG_HEIGHT > DIAMOND_BERG_GROUND) {
        height = height - top + (DIAMOND_BERG_GROUND - DIAMOND_BERG_HEIGHT);
      }
      if (height > 0) {
        blit[0](
            canvas, Ram_MapCellBuffer + DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES,
            centerX - DIAMOND_BERG_WIDTH / 2, top, DIAMOND_BERG_WIDTH, height);
      }
    }
    if (frame == 7) {
      struct Particle *shard;

      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
      for (k = 0, shard = work->particles; k != DIAMOND_BERG_CRASH_SHARDS;
           k++, shard++) {
        shard->y = toFIXED(96);
        shard->x = INTtoFIXED(centerX);
        shard->velocityX = ((Random16() & 127) - 64) << 12;
        shard->velocityY = -((Random16() & 127) + 64) << 12;
        shard->z = 32;
        shard->variant = 0;
      }
    }

    // The shards tumble and bounce; z is their spin.
    if (frame > 6) {
      struct Particle *shard;

      for (i = 0, shard = work->particles; i != ARRAY_COUNT(work->particles);
           i++, shard++) {
        s32 cell;
        u8 *pixels;
        s32 x;
        s32 y;
        u32 width;
        u32 height;

        if ((u32)shard->variant >= DIAMOND_BERG_SHARD_SPIN) {
          continue;
        }
        cell = i % 5 * 3 + shard->variant / SHARD_TURN % 3;
        pixels =
            work->sheet + PRISM_SHARD_PICTURES + gPrism_ShardPictures[cell];
        width = gPrism_ShardWidths[cell];
        x = FIXED_INTEGER_PART(shard->x) - (width >> 1);
        y = FIXED_INTEGER_PART(shard->y);
        height = gPrism_ShardHeights[cell];
        y -= height >> 1;
        blit[0](canvas, pixels, x, y, width, height);
        Particle_AdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE,
                                      toFIXED(1) / 4);
        if (FIXED_INTEGER_PART(shard->y) > DIAMOND_BERG_SHARD_GROUND) {
          shard->velocityY = -shard->velocityY / 2;
        }
        shard->variant += shard->z;
        if (shard->z > 1 && (frame & 1)) {
          shard->z--;
        }
      }
    }

    if (frame > 5) {
      void *vertices;

      vertices = Heap_AllocEwram(1024);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = cells;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->flags = 0;
      model->mesh = gBattleEffect_CylinderMesh;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // The frost ripples.
      for (k = 0; k != 4; k++) {
        s32 start = gDiamondBerg_RingStarts[k] + 6;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * gDiamondBerg_RingGrowth[k] *
                    DIAMOND_BERG_RIPPLE_GROWTH +
                DIAMOND_BERG_RIPPLE_START_SCALE;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= MODEL_SHADE_INVISIBLE) {
          continue;
        }
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(centerX - 64),
                         INTtoFIXED(gDiamondBerg_RingHeights[k]), 0);
        Matrix_Scale(scale, scale, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Matrix_RotateX(DIAMOND_BERG_RIPPLE_TILT);
        Render_TransformVertices(gBattleEffect_CylinderCorners, vertices, 32);
        Render_DrawModelList(model);
      }

      // A ring of light flares at each blow, at the target, faster at each
      // blow, and fades after four frames.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + DIAMOND_BERG_FIRE_RING;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != DIAMOND_BERG_BLOWS; k++) {
        s32 t = frame - 31 - k * 8;
        s32 scale;
        s32 shade;

        if (t < 0) {
          continue;
        }
        scale = (k * (FIXED_ONE / 32) + FIXED_ONE / 8) * t + FIXED_ONE / 2;
        shade = 0;
        if (t > 3) {
          shade = (4 - t) * 16;
        }
        model->shade = shade;
        if (shade <= MODEL_SHADE_INVISIBLE) {
          continue;
        }
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(target.x / 2 - 64),
                         INTtoFIXED(target.y - 80), 0);
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        Matrix_RotateY(args->side * DEGtoANG(180) + DEGtoANG(180));
        Matrix_RotateZ(gDiamondBerg_ShardRolls[k]);
        Matrix_RotateX(gDiamondBerg_ShardPitches[k]);
        Matrix_ScaleUniform(scale);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef UNDEAD_SWORD_SPARK_COUNT
#undef UNDEAD_SWORD_SPARK_LIFE
#undef LIME_COLUMN_WIDTH
#undef LIME_COLUMN_HEIGHT
#undef UNDEAD_SWORD_SPARK_SHEET
#undef UNDEAD_SWORD_FRAMES
#undef UNDEAD_SWORD_HIT_FRAME
#undef UNDEAD_SWORD_RISE_FRAME
#undef UNDEAD_SWORD_PEAK_FRAME
#undef UNDEAD_SWORD_GROUND
#undef PLUME_EDGE_FOAM_PICTURES
#undef PLUME_EDGE_FRAMES
#undef PLUME_EDGE_BILLOW
#undef PLUME_EDGE_FOAM_PUFFS
#undef PLUME_EDGE_FOAM_FRAMES
#undef PLUME_EDGE_LIFT_FRAMES
#undef PLUME_EDGE_BURST_FRAME
#undef PLUME_EDGE_FIRST_DROP
#undef PLUME_EDGE_DROPS
#undef PLUME_EDGE_GEYSER_FOOT
#undef PLUME_EDGE_GEYSER_SPEED
#undef WATER_VERTICES
#undef WATER_COLUMNS
#undef WATER_ROWS
#undef WATER_QUAD_TEXELS_ACROSS
#undef WATER_QUAD_TEXELS_DOWN
#undef WATER_QUADS
#undef WATER_MESH_OFFSET
#undef WATER_MESH
#undef WATER_SHEET_SWIRL
#undef WATER_SHEET_SMOKE
#undef WATER_SHEET_SMOKE_PICTURE_COUNT
#undef WATER_SHEET_MIRRORED
#undef DREAMTIDE_FRAMES
#undef WATERY_GRAVE_FRAMES
#undef WATER_SHEET_CAMERA_TURN
#undef WATER_SHEET_CAMERA_LIMIT
#undef WATER_WAVE_COLUMN_PHASE
#undef WATER_WAVE_ROW_PHASE
#undef WATERY_GRAVE_PUFFS
#undef WATERY_GRAVE_PUFF_DEPTH
#undef WATERY_GRAVE_TILT
#undef WATER_SHEET_START_X
#undef WATER_SHEET_END_X
#undef WIND_FUNNEL_SEGMENTS
#undef WIND_FUNNEL_CORNERS
#undef WIND_FUNNEL_BAND
#undef WIND_FUNNEL_RADIUS
#undef WIND_FUNNEL_VERTICES
#undef WIND_FUNNEL_MESH
#undef WIND_FUNNEL_QUADS
#undef WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES
#undef WAFT_FUNNEL_HEIGHT
#undef AROMA_FUNNEL_HEIGHT
#undef AROMA_FUNNEL_DIP
#undef WIND_FUNNEL_DIP_SPEED
#undef WIND_FUNNEL_SPREAD
#undef WAFT_FUNNEL_SCALE
#undef WAFT_FUNNEL_PULSE_SPEED
#undef AROMA_FUNNEL_PULSE_SPEED
#undef AROMA_FUNNEL_SPACING
#undef DIAMOND_BERG_FRAMES
#undef DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES
#undef DIAMOND_BERG_WIDTH
#undef DIAMOND_BERG_HEIGHT
#undef DIAMOND_BERG_DROP_SPEED
#undef DIAMOND_BERG_BLOWS
#undef DIAMOND_BERG_BLOW_DEPTH
#undef DIAMOND_BERG_GROUND
#undef DIAMOND_BERG_BILLOW
#undef DIAMOND_BERG_FIRE_RING
#undef DIAMOND_BERG_CRASH_SHARDS
#undef DIAMOND_BERG_BLOW_SHARDS
#undef DIAMOND_BERG_SHARD_GROUND
#undef DIAMOND_BERG_SHARD_SPIN
#undef DIAMOND_BERG_RIPPLE_TILT
#undef DIAMOND_BERG_RIPPLE_START_SCALE
#undef DIAMOND_BERG_RIPPLE_GROWTH
