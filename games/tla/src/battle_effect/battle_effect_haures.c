/// @file
/// The Haures summon, and the open cylinder a summon's band is drawn on.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Two corners of the mound: its upper edge, then its lower.
struct MoundVertex {
  struct ModelVertex upper;
  struct ModelVertex lower;
};

/// The map cell buffer past its first 16 KB: the rune's picture in the coming,
/// then the charge's sparks, which are EXTRA_PARTICLES.
#define SCRATCH (Ram_MapCellBuffer + 16384)
/// The mound strip's corners, rebuilt each frame from the morphing shapes.
extern struct MoundVertex gHaures_WingCorners[];
extern const u16 gHaures_BodyPiecePoints[][2];

#define HAURES_COMING_FRAMES 440 ///< frames of Haures's coming, unless skipped
#define HAURES_FRAMES 92         ///< frames of the charge that follows
/// Haures's sprites: the beast's eleven pieces (SPRITE_HAURES), four violet
/// flames (SPRITE_HAURES_FLAME) and two drops (SPRITE_HAURES_DOT), then the
/// 64x64 stamp. The coming shows only the first two pieces, its raised claw,
/// until the frame he rears.
#define HAURES_BODY_PIECES 11
#define HAURES_CLAW_PIECES 2
#define HAURES_FIRST_FLAME 12
#define HAURES_FLAME_COUNT 4
#define HAURES_FIRST_DROP_SPRITE 16
#define HAURES_DROP_SPRITE_COUNT 2
#define HAURES_STAMP 18
/// The drops: four fall one after another, drawn by the two drop sprites in
/// turn, and land at HAURES_DROP_LAND_Y, each then bursting into a flame that
/// plays its four pictures over HAURES_FLAME_FRAMES.
#define HAURES_DROPS 4
#define HAURES_DROP_LAND_Y 120
#define HAURES_FLAME_FRAMES 24
/// The stamp's colours: OBJ palette HAURES_STAMP_PALETTE, a violet ramp the
/// ember puffs are drawn in, each puff level a quarter of its own.
#define HAURES_STAMP_PALETTE 14
#define HAURES_STAMP_FIRST_COLOR (HAURES_STAMP_PALETTE * PALETTE_COLORS)
/// The ember puffs that rise in the stamp: EmberStreakSheet's six pictures,
/// which take HAURES_PUFF_BYTES at the start of the effect's pictures, four
/// frames each.
#define HAURES_PUFFS 8
#define HAURES_PUFF_BYTES 6017
#define HAURES_PUFF_PICTURE_COUNT 6
#define HAURES_PUFF_PICTURE_FRAMES 4
#define HAURES_PUFF_FRAMES                                                     \
  (HAURES_PUFF_PICTURE_COUNT * HAURES_PUFF_PICTURE_FRAMES)
/// The mound: a strip of 32 points, upper and lower edge each, so 31 quads,
/// first an ellipse HAURES_POOL_WIDTH across and twice HAURES_POOL_DEPTH tall
/// whose points step HAURES_POOL_STEP round it.
#define HAURES_MOUND_POINTS 32
#define HAURES_MOUND_QUADS (HAURES_MOUND_POINTS - 1)
#define HAURES_POOL_WIDTH 128
#define HAURES_POOL_DEPTH 18
#define HAURES_POOL_STEP 992
/// The shapes the mound's upper edge morphs through in turn,
/// HAURES_MOUND_POINTS heights each.
#define HAURES_MOUND_SHAPES 13
/// The red swirl the mound stands on: tilted back about 22 degrees, turning
/// and growing to HAURES_SWIRL_SCALE_MAX, where the mound grows to
/// HAURES_MOUND_SCALE_MAX.
#define HAURES_SWIRL_TILT 4000
#define HAURES_SWIRL_SCALE_MAX (FIXED_ONE * 1875 / 2048)
#define HAURES_MOUND_SCALE_MAX (FIXED_ONE * 2423 / 4096)
/// Where SwirlTexture, 64 x 64 and dimmed to half, is decoded in the effect's
/// pictures.
#define HAURES_SWIRL_TEXTURE 16384
#define HAURES_SWIRL_WIDTH 64
#define HAURES_SWIRL_HEIGHT 64
/// ChainTexture is a quarter of the rune, mirrored into the other three.
#define HAURES_CHAIN_WIDTH 64
#define HAURES_CHAIN_HEIGHT 64
/// The canvas levels' colours: levels below HAURES_RED_LEVELS are red, the
/// rest violet, from black to white.
#define HAURES_RED_LEVELS 32
/// The cave turns red as Haures rears: its red raised by HAURES_CAVE_FADE_RED,
/// its green and blue lowered by HAURES_CAVE_FADE_DARK.
#define HAURES_CAVE_FADE_RED 8
#define HAURES_CAVE_FADE_DARK 12
/// The rune's scale on a frame: it holds its size at HAURES_RUNE_GROW_FRAME
/// until then, and from then grows a sixteenth a frame (its height; it is half
/// as wide).
#define HAURES_RUNE_SCALE(frame)                                               \
  (((frame) - HAURES_RUNE_GROW_FRAME) * (FIXED_ONE / 16) +                     \
   HAURES_RUNE_HELD_SCALE)
#define HAURES_RUNE_GROW_FRAME 269
#define HAURES_RUNE_HELD_SCALE (FIXED_ONE * 7567 / 4096)
/// BlueArcSheetB's six arcs (BLUE_ARC_WIDTH by BLUE_ARC_HEIGHT), each laid in
/// a texture HAURES_ARC_TEXTURE_WIDTH wide.
#define HAURES_ARCS 6
#define HAURES_ARC_TEXTURE_WIDTH 64
#define HAURES_ARC_TEXTURE_BYTES (HAURES_ARC_TEXTURE_WIDTH * BLUE_ARC_HEIGHT)
/// The shards the view breaks into as Haures rears: a row of
/// gHaures_PartPoints each, its middle then its four corners in screen
/// pixels. Each is built in the effect's pictures as its corners, then from
/// HAURES_SHARD_MESHES its mesh: two faces and the empty one that ends it.
#define HAURES_SHARDS 31
#define HAURES_SHARD_ROW 10
#define HAURES_SHARD_CORNERS 4
#define HAURES_SHARD_CORNER_BYTES                                              \
  (HAURES_SHARD_CORNERS * sizeof(struct ModelVertex))
#define HAURES_SHARD_MESHES 2048
#define HAURES_SHARD_MESH_BYTES 12
/// Face `face` of shard j's mesh: its three corners, then a byte no face sets.
#define SHARD_FACE(mesh, j, face)                                              \
  ((mesh) + (j) * HAURES_SHARD_MESH_BYTES + (face) * 4)
/// The point the shards fly apart from: the right edge of the view, whose
/// width the canvas halves, at its middle height. Each shard starts a
/// sixteenth of a pixel a frame across (FIXED_ONE >>
/// (16 - HAURES_SHATTER_SPREAD_X_SHIFT)) and a sixty-fourth up or down for each
/// pixel it lies from it.
#define HAURES_SHATTER_X 120
#define HAURES_SHATTER_Y 60
#define HAURES_SHATTER_SPREAD_X_SHIFT 12
/// A shard is drawn while its middle is no more than HAURES_SHARD_REACH
/// outside the view.
#define HAURES_SHARD_REACH 16
/// As Haures dives away up and to the left, each frame adds to his speed a
/// quarter of a pixel a frame more than the one before (FIXED_ONE >>
/// (16 - HAURES_DIVE_PULL_X_SHIFT)); the eighth more that would lift him goes
/// unused.
#define HAURES_DIVE_PULL_X_SHIFT 14
#define HAURES_DIVE_PULL_Y_SHIFT 13
/// The charge's sparks: HAURES_SPARKS_PER_FRAME start each frame of the first
/// two passes, each living HAURES_SPARK_LIFE frames.
#define HAURES_SPARK_COUNT 512
#define HAURES_SPARKS_PER_FRAME 12
#define HAURES_SPARK_LIFE 16
/// Haures charges through the foes three times, a pass every
/// HAURES_CHARGE_PASS_FRAMES: from the right, from the left drawn mirrored,
/// then from the right again.
#define HAURES_CHARGE_PASS_FRAMES 20
/// Each of the first two passes' streaks of fire lasts HAURES_STREAK_FRAMES
/// from its gHaures_BandStarts frame, swelling and narrowing again over half a
/// turn of a sine.
#define HAURES_STREAK_FRAMES 16
/// The second pass's streaks of fire slash at about 56 degrees, where the
/// first's slash at -45.
#define HAURES_SECOND_BAND_ROLL 10192
/// The band that opens across the canvas from frame 56, HAURES_BAND_INSET in
/// from each side, its edges HAURES_BAND_REACH above and below its middle.
#define HAURES_BAND_INSET 4
#define HAURES_BAND_MIDDLE 60
#define HAURES_BAND_REACH 80

/// Haures: in a blue cavern four drops fall and burst into violet flames, a red
/// swirl opens on the ground and a dark pool on it heaves up into a mound; the
/// beast's claw rises from it in violet flame, a rune flashes as the claw
/// slashes, and the dark breaks into shards to show him circling in a reddened
/// cave; then he charges through the foes three times in streaks of fire that
/// end in a flash of light.
///
/// Abilities: Haures.
void BattleEffect_RunHaures(struct BattleEffectParams *args) {
  struct BattleWork *battle;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  struct BattleBackgroundWork *background;
  struct Point2D position;
  struct Point2D velocity;
  s32 morph;
  s32 morphStep;
  s32 dived;
  s32 i;
  s32 dropPlace[4];
  FIXED bodyPlace[4];
  struct Scale riseScale;
  struct Scale chargeScale;
  struct Scale orbitScale;
  struct ModelTexture texture;
  struct ModelTexture burstTexture;
  struct ModelTexture wideTexture;
  struct ModelTexture bandTexture;
  struct FieldSprite *sprite;
  const u8 *particleX;
  u8 *buffer;
  u8 *shades;
  u16 *dst;

  buffer = gMapGridCellBuffer;
  shades = gHaures_WingShades;
  battle = Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  background = Ram_HeapSlots->battleBackground;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BG2X = Q_8_8(4);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);
  background->busy = 1;
  gBgScroll[1].x = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add(BattleEffect_ProcessPendingStampTransfer, TASK_PASS_VBLANK);
  morph = 0;
  morphStep = 0;
  dived = 0;
  BattleEffect_SpawnObjects(HAURES_CLAW_PIECES, SPRITE_HAURES, 2);
  for (i = 0; i != HAURES_FLAME_COUNT; i++) {
    sprite = Sprite_CreateFar(SPRITE_HAURES_FLAME);
    work->sprites[HAURES_FIRST_FLAME + i] = sprite;
    if (sprite != NULL) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, 0);
      {
        struct FieldSprite *added = work->sprites[HAURES_FIRST_FLAME + i];

        added->priority = 1;
        added->blendMode = OBJ_MODE_SEMI_TRANSPARENT;
      }
    }
  }
  for (i = 0; i != HAURES_DROP_SPRITE_COUNT; i++) {
    sprite = Sprite_CreateFar(SPRITE_HAURES_DOT);
    work->sprites[HAURES_FIRST_DROP_SPRITE + i] = sprite;
    if (sprite != NULL) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, 0);
      {
        struct FieldSprite *added = work->sprites[HAURES_FIRST_DROP_SPRITE + i];

        added->priority = 1;
      }
    }
  }
  sprite =
      EffectSprite_CreateWithTiles(64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);
  ((struct FieldSprite **)work->sprites)[HAURES_STAMP] = sprite;
  sprite->blendMode = OBJ_MODE_SEMI_TRANSPARENT;
  STAMP_TILES(work) = gVramCacheEntries[sprite->slot].offset + OBJ_VRAM0;
  {
    u8 *c = work->sheet;

    Resource_LoadAndDecompress(RESOURCE_CHAIN_TEXTURE, c, 0, 0);
    Picture_MirrorIntoQuadrants(c, SCRATCH, HAURES_CHAIN_WIDTH,
                                HAURES_CHAIN_HEIGHT);
    Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, c, 1, 0);
    for (i = 0; i != HAURES_PUFF_BYTES; i++, c++) {
      u8 v = *c;

      if (v != 0) {
        *c = (v >> 2) + HAURES_STAMP_FIRST_COLOR;
      }
    }
  }
  {
    u32 *w;
    u32 mask;

    Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                               w = (u32 *)(work->sheet + HAURES_SWIRL_TEXTURE),
                               0, 0);
    // Halves the swirl's levels four pixels at a time.
    mask = PIXELS_8BPP(0x7f);
    for (i = 0; i != PICTURE_BYTES(HAURES_SWIRL) / 4; i++) {
      w[i] = (w[i] >> 1) & mask;
    }
  }
  for (i = 0; i != HAURES_MOUND_QUADS; i++) {
    gHaures_WingStrip[i * 8 + 0] = i * 2;
    gHaures_WingStrip[i * 8 + 1] = i * 2 + 1;
    gHaures_WingStrip[i * 8 + 2] = i * 2 + 2;
    gHaures_WingStrip[i * 8 + 4] = i * 2 + 1;
    gHaures_WingStrip[i * 8 + 5] = i * 2 + 3;
    gHaures_WingStrip[i * 8 + 6] = i * 2 + 2;
  }
  // The strip ends with a face of no corners, cleared through the end's own
  // name.
  gHaures_WingStripEnd[0] = 0;
  gHaures_WingStripEnd[1] = 0;
  gHaures_WingStripEnd[2] = 0;
  {
    dst = &BG_PLTT_COLOR(0, 1);

    for (i = 1; i != SHEET_PALETTE_COLORS; i++) {
      s32 b;
      s32 r;
      s32 g;

      if (i < HAURES_RED_LEVELS) {
        b = 0;
        g = 0;
        r = i;
        if (i < 0) {
          r = 0;
        }
        *dst = r;
      } else {
        s32 t = i - HAURES_RED_LEVELS;

        b = t * 4;
        g = t;
        r = b;
        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
      }
      *dst++ = RGB(r, g, b);
    }
  }
  {
    dst = &OBJ_PLTT_COLOR(HAURES_STAMP_PALETTE, 0);

    for (i = 0; i != PALETTE_COLORS; i++, dst++) {
      s32 b = i * 3 + 2;
      s32 r = i * 2 + 2;

      if (r > RGB_LEVEL_MAX) {
        r = RGB_LEVEL_MAX;
      }
      if (b > RGB_LEVEL_MAX) {
        b = RGB_LEVEL_MAX;
      }
      *dst = RGB(r, i + 2, b);
    }
  }
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_MOSS_CAVE_BACKDROP,
                           0);
  // The coming, which A or B skips.
  frame = 0;
  if (!(gInput.repeat & (KEY_A | KEY_B))) {
    do {
      s32 cut = frame - 267;

      if (frame == 0) {
        work->transferValue = frame;
        position.x = toFIXED(108);
        position.y = toFIXED(120);
        velocity.x = 0;
        velocity.y = 0;
        Iwram_CopyWordsEntry((void *)&BG_PLTT_COLOR(6, 0), battle->palette,
                             sizeof(battle->palette));
        {
          u8 *q = shades;

          for (i = 0; i != HAURES_MOUND_QUADS; i++, q += 2) {
            q[0] = 1;
            q[1] = 1;
          }
        }
        particleX = gHaures_ParticleX;
        {
          struct Particle *p = work->particles;

          for (i = 0; i != HAURES_DROPS; i++, p++, particleX++) {
            p->x = INTtoFIXED(*particleX);
            p->y = toFIXED(16);
            p->velocityX = 0;
            p->velocityY = 0;
            p->variant = 0;
          }
        }
        {
          for (i = 0; i != HAURES_PUFFS; i++) {
            struct Particle *p = &work->particles[i + HAURES_DROPS];
            p->x = (Random16() & 15) + 24;
            p->y = Random16() & 7;
            p->variant = i * 3;
            p->velocityX = 0;
            p->velocityY = 0;
          }
        }
        // DEAD: the last particle is placed and nothing reads it; the ROM keeps
        // its stores.
        {
          struct Particle *p =
              &work->particles[ARRAY_COUNT(work->particles) - 1];

          p->x = toFIXED(60);
          p->y = toFIXED(112);
          p->variant = 0;
        }
        for (i = 0; i != HAURES_MOUND_POINTS; i++) {
          gHaures_WingCorners[i].upper.x = FIXEDtoINT(
              -(Trig_Cos(i * HAURES_POOL_STEP) * (HAURES_POOL_WIDTH / 2)));
          gHaures_WingCorners[i].upper.y =
              FIXEDtoINT(-(Trig_Sin(i * HAURES_POOL_STEP) * HAURES_POOL_DEPTH));
          // DEAD: lower.x is set again two lines on; the ROM keeps this
          // store.
          gHaures_WingCorners[i].lower.x = i * 4 - 64;
          gHaures_WingCorners[i].upper.z = 0;
          gHaures_WingCorners[i].lower.x = FIXEDtoINT(
              -(Trig_Cos(i * HAURES_POOL_STEP) * (HAURES_POOL_WIDTH / 2)));
          gHaures_WingCorners[i].lower.y =
              (u32)(Trig_Sin(i * HAURES_POOL_STEP) * (HAURES_POOL_DEPTH / 2)) /
              (FIXED_ONE / 2);
          gHaures_WingCorners[i].lower.z = 0;
        }
      }
      if ((u32)frame <= 31) {
        s32 a = frame * 2 / 3 - 35;

        Palette_BrightenSavedBattleBg(a, a, frame - 35);
      }
      if ((u32)(frame - 242) <= 21 && (frame & 1)) {
        Palette_BrightenBattleBg(1, 0, 0);
      }
      if (frame == 277) {
        Palette_BrightenSavedBattleBg(HAURES_CAVE_FADE_RED,
                                      -HAURES_CAVE_FADE_DARK,
                                      -HAURES_CAVE_FADE_DARK);
      }
      if (frame > 359) {
        Palette_BrightenSavedBattleBg(
            HAURES_CAVE_FADE_RED - (frame - 360) / 4, -HAURES_CAVE_FADE_DARK,
            (frame - 360) / 8 - HAURES_CAVE_FADE_DARK);
      }
      // A whoosh as each drop lands.
      if (frame == 48) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 74) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 100) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 126) {
        Sound_PlayFar(SE_WHOOSH);
      }
      if (frame == 140) {
        Sound_PlayFar(SE_RUMBLE);
      }
      if (frame <= 266) {
        // DEAD: frame <= 639 always holds here; the ROM keeps the compare.
        if (frame <= 639) {
          // The drops fall one after another, each bursting into a flame where
          // it lands.
          dropPlace[1] = 0;
          dropPlace[3] = 0;
          {
            struct Particle *p = work->particles;

            for (i = 0; i != HAURES_DROPS; i++, p++) {
              if (frame > i * 26) {
                if (p->variant == 0) {
                  dropPlace[0] = p->x;
                  dropPlace[2] = p->y;
                  Render_ApplyProjectedPlacementFar(
                      work->sprites[HAURES_FIRST_DROP_SPRITE + (i & 1)],
                      dropPlace, gHaures_UnitScale, 0);
                  Particle_AdvanceWithGravity2D(p, 63, toFIXED(1) / 8);
                  if (FIXED_INTEGER_PART(p->y) >= HAURES_DROP_LAND_Y) {
                    p->variant = 1;
                  }
                } else if (p->variant < HAURES_FLAME_FRAMES) {
                  dropPlace[0] = p->x;
                  dropPlace[2] = p->y;
                  Sprite_StartAnimationFar(
                      work->sprites[HAURES_FIRST_FLAME + (i & 1)],
                      (p->variant / 6) & 3);
                  Render_ApplyProjectedPlacementFar(
                      work->sprites[HAURES_FIRST_FLAME + (i & 1)], dropPlace,
                      gHaures_UnitScale, 0);
                  p->variant++;
                }
              }
            }
          }
        }
        // The red swirl turns on the ground under the dark pool, which from
        // frame 74 heaves up through the mound's shapes and from 236 glows.
        if (frame > 46) {
          struct RenderModel *model;
          void *verts;
          s32 t;
          s32 sh;
          s32 s;
          u8 *strip = gHaures_WingStrip;

          verts = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          texture.logWidth = 6;
          t = frame - 47;
          texture.logHeight = 6;
          texture.pixels = work->sheet + HAURES_SWIRL_TEXTURE;
          sh = frame - 111;
          if (sh > 0) {
            sh = 0;
          }
          model->kind = MODEL_SHADED_MAX;
          model->shade = sh;
          model->texture = &texture;
          model->mesh = gBattleEffect_SquareMesh;
          model->vertices = verts;
          Matrix_Initialize();
          Matrix_Translate(0, toFIXED(36), 0);
          Matrix_RotateX(HAURES_SWIRL_TILT);
          Matrix_RotateY(-t << 8);
          s = t * (FIXED_ONE / 64);
          if (s > HAURES_SWIRL_SCALE_MAX) {
            s = HAURES_SWIRL_SCALE_MAX;
          }
          Matrix_ScaleUniform(s);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, verts, 4);
          Render_DrawModelList(model);
          // DEAD: a second square, textured from the sheet past where
          // ChainTexture was decoded, is set up and never drawn, the mound's
          // settings replacing it before the draw; the ROM keeps its stores.
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = work->sheet + PICTURE_BYTES(HAURES_CHAIN);
          model->kind = MODEL_SHADED;
          model->texture = &texture;
          model->mesh = gBattleEffect_SmallSquareMeshReversed;
          model->vertices = verts;
          model->shade = sh;
          Matrix_Initialize();
          Matrix_Translate(0, toFIXED(36), 0);
          s = t * 3 * (FIXED_ONE / 256) + FIXED_ONE / 64;
          if (s > HAURES_MOUND_SCALE_MAX) {
            s = HAURES_MOUND_SCALE_MAX;
          }
          Matrix_ScaleUniform(s);
          if (frame > 73) {
            s32 a;
            s32 b;
            s32 n;
            s32 next;
            struct MoundVertex *mound = gHaures_WingCorners;

            if (frame == 74) {
              morph = 0;
              morphStep = 0;
            }
            a = morph % HAURES_MOUND_SHAPES;
            next = morph + 1;
            b = next % HAURES_MOUND_SHAPES;
            n = morphStep;
            morphStep = n + 1;
            if (morphStep == gHaures_WingMorphSteps[a]) {
              morphStep = 0;
              morph = next;
            }
            for (i = 0; i != HAURES_MOUND_POINTS; i++) {
              mound[i].upper.y =
                  -(gHaures_WingShapes[i + a * HAURES_MOUND_POINTS] +
                    n *
                        (gHaures_WingShapes[i + b * HAURES_MOUND_POINTS] -
                         gHaures_WingShapes[i + a * HAURES_MOUND_POINTS]) /
                        gHaures_WingMorphSteps[a]);
            }
            for (i = 0; i != HAURES_MOUND_POINTS; i++) {
              gHaures_WingCorners[i].upper.x = i * 4 - 64;
            }
          }
          model->flags = 0;
          model->kind = MODEL_SOLID;
          model->mesh = strip;
          model->vertices = buffer;
          if ((u32)(frame - 236) <= 31) {
            s32 v = frame - 235;

            for (i = 0; i != HAURES_MOUND_QUADS; i++) {
              gHaures_WingShades[i * 2] = v;
              gHaures_WingShades[i * 2 + 1] = v;
            }
          }
          model->shade = (long)shades;
          Render_TransformVertices(gHaures_WingCorners, buffer,
                                   HAURES_MOUND_POINTS * 2);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
        }
        riseScale = gHaures_ChargeScale;
        bodyPlace[3] = 0;
        // The claw rises out of the mound amid violet puffs of embers.
        if (frame > 149) {
          position.x += velocity.x;
          position.y += velocity.y;
          if (frame <= 159) {
            velocity.y += -toFIXED(1) / 4;
          } else if ((u32)(frame - 210) <= 13) {
            velocity.x += -toFIXED(1) / 32;
            velocity.y += -toFIXED(1) / 16;
          } else if ((u32)(frame - 236) <= 3) {
            velocity.x += toFIXED(1) / 8;
            velocity.y += -toFIXED(1) / 4;
          }
          velocity.x = velocity.x * 60 / DAMPING_SCALE;
          velocity.y = velocity.y * 60 / DAMPING_SCALE;
          riseScale.x = toFIXED(1);
          riseScale.y = toFIXED(1);
          for (i = 0; i != HAURES_CLAW_PIECES; i++) {
            bodyPlace[0] = position.x;
            bodyPlace[1] = toFIXED(272) - (position.y + i * toFIXED(32));
            bodyPlace[2] = toFIXED(256);
            Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                              (s32 *)&riseScale, 0);
          }
          if ((u32)(frame - 150) <= 116) {
            BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 6, 6,
                                      EFFECT_CLIP_BOUNDS, BLITTER_MAX);
            draw[1] = Ram_HeapSlots->ramCodePaired;
            {
              for (i = 0; i != HAURES_PUFFS; i++) {
                struct Particle *p = &work->particles[i + HAURES_DROPS];
                s32 n = (p->variant / HAURES_PUFF_PICTURE_FRAMES) %
                        HAURES_PUFF_PICTURE_COUNT;
                u8 *src = work->sheet + gBattleEffect_PuffArcPictures[n];
                u32 w = gBattleEffect_PuffArcPictureWidths[n];

                draw[1](STAMP_CANVAS(work), src, p->x - (w >> 1),
                        p->y + gBattleEffect_PuffArcPictureBiasY[n], w,
                        gBattleEffect_PuffArcPictureHeights[n]);
                if (++p->variant == HAURES_PUFF_FRAMES) {
                  p->variant = 0;
                  p->x = (Random16() & 15) + 24;
                  p->y = Random16() & 7;
                }
              }
            }
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          }
          riseScale.x = frame * 1128 - 103664;
          if (riseScale.x > toFIXED(4)) {
            riseScale.x = toFIXED(4);
          }
          riseScale.y = frame * 564 - 19064;
          if (riseScale.y > toFIXED(2)) {
            riseScale.y = toFIXED(2);
          }
          bodyPlace[0] = position.x;
          bodyPlace[1] = toFIXED(120) - position.y;
          bodyPlace[2] = toFIXED(128);
          Render_ApplyProjectedPlacementFar(work->sprites[HAURES_STAMP],
                                            bodyPlace, (s32 *)&riseScale, 0);
        }
      }
      if (frame == 267) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 271) {
        Sound_PlayFar(SE_CLATTER);
      }
      // The rune flashes and grows behind the dark, and for six frames the
      // claw slashes across it, one of BlueArcSheetB's arcs a frame.
      if ((u32)cut <= 9) {
        struct RenderModel *model;
        void *verts;
        s32 s;

        if (frame == 267) {
          u16 *dst = &BG_PLTT_COLOR(0, 1);

          // DEAD: a ramp of reds that MarsDjinnSheet's colours replace before
          // the frame shows it; the ROM keeps its loop.
          for (i = 1; i != SHEET_PALETTE_COLORS; i++) {
            s32 v = i;

            if (v > RGB_LEVEL_MAX) {
              v = RGB_LEVEL_MAX;
            }
            if (v < 0) {
              v = 0;
            }
            *dst++ = v;
          }
          Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B, work->sheet, 1,
                                     0);
          Iwram_CopyWordsEntry(
              (void *)BG_PLTT,
              Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
              SHEET_PALETTE_BYTES);
          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(1));
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        burstTexture.logWidth = 7;
        burstTexture.logHeight = 7;
        burstTexture.pixels = SCRATCH;
        model->shade = 0;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->texture = &burstTexture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = verts;
        Matrix_Initialize();
        Matrix_Translate(0, -toFIXED(4), 0);
        s = HAURES_RUNE_HELD_SCALE;
        if (frame >= HAURES_RUNE_GROW_FRAME) {
          s = HAURES_RUNE_SCALE(frame);
        }
        Matrix_Scale(s >> 1, s, s >> 1);
        Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
        Render_DrawModelList(model);
        if ((u32)cut <= 5) {
          s32 j;

          Iwram_ClearWords(buffer, PICTURE_BYTES(BLUE_ARC));
          for (j = 0; j != BLUE_ARC_HEIGHT; j++) {
            s32 m;

            for (m = 0; m != BLUE_ARC_WIDTH; m++) {
              buffer[j * HAURES_ARC_TEXTURE_WIDTH + m] =
                  work->sheet[cut * PICTURE_BYTES(BLUE_ARC) +
                              j * BLUE_ARC_WIDTH + m];
            }
          }
          if ((u32)(frame - 267) <= 5) {
            burstTexture.logWidth = 6;
            burstTexture.logHeight = 7;
            burstTexture.pixels = buffer;
            model->mesh = gHaures_BurstMesh;
            Matrix_Initialize();
            Matrix_Translate(0, -toFIXED(4), 0);
            Matrix_Scale(toFIXED(1.69483), toFIXED(2.61036), FIXED_ONE * 2);
            Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      // Haures rears: the dark breaks into shards that fly apart, and he
      // circles in the reddened cave until, from frame 397, he dives away.
      if (frame > 276) {
        struct RenderModel *model;
        void *verts;
        u8 *cells;
        u8 *mesh;

        if (frame == 277) {
          const u8 *q;

          EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
          dived = 1;
          Sprite_DestroyFar(work->sprites[0]);
          Sprite_DestroyFar(work->sprites[1]);
          for (i = 0; i != HAURES_FLAME_COUNT; i++) {
            Sprite_DestroyFar(work->sprites[HAURES_FIRST_FLAME + i]);
          }
          for (i = 0; i != HAURES_DROP_SPRITE_COUNT; i++) {
            Sprite_DestroyFar(work->sprites[HAURES_FIRST_DROP_SPRITE + i]);
          }
          Sprite_DestroyFar(work->sprites[HAURES_STAMP]);
          BattleEffect_SpawnObjects(HAURES_BODY_PIECES, SPRITE_HAURES, 2);
          q = gHaures_PartPoints;
          {
            struct Particle *p = work->particles;

            for (i = 0; i != HAURES_SHARDS; i++, q += HAURES_SHARD_ROW, p++) {
              s32 x = q[0] >> 1;
              s32 y = q[1];

              p->x = INTtoFIXED(x);
              p->y = INTtoFIXED(y);
              {
                s32 velocityY = (HAURES_SHATTER_Y - y) * (toFIXED(1) / 64);

                p->velocityX = (x - HAURES_SHATTER_X)
                               << HAURES_SHATTER_SPREAD_X_SHIFT;
                p->velocityY = velocityY;
              }
              p->z = 0;
              p->velocityX += ((Random16() & 31) - 16) << 13;
              p->velocityY += ((Random16() & 31) - 16) << 13;
              p->velocityZ = (Random16() & 127) * 5 + 640;
              if (Random16() & 1) {
                p->velocityZ = -p->velocityZ;
              }
            }
          }
          velocity.x = 0;
        }
        gBgScroll[1].x -= 12;
        orbitScale = gHaures_OrbitScale;
        bodyPlace[3] = 0;
        position.x = (Trig_Sin(frame << 9) << 3) + toFIXED(108);
        position.y = toFIXED(56) - (Trig_Cos(frame << 9) << 3);
        if (frame > 396) {
          velocity.x -= (frame - 397) << HAURES_DIVE_PULL_X_SHIFT;
          position.x += velocity.x;
          velocity.y -= (frame - 397) << HAURES_DIVE_PULL_Y_SHIFT;
          // BUG: the fall adds the horizontal speed; velocity.y is never used.
#ifdef BUGFIX
          position.y += velocity.y;
#else
          position.y += velocity.x;
#endif
        }
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = INTtoFIXED(gHaures_BodyPiecePoints[i][0]) + position.x;
          bodyPlace[1] =
              toFIXED(272) -
              (INTtoFIXED(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = toFIXED(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&orbitScale, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        cells = work->sheet;
        mesh = work->sheet + HAURES_SHARD_MESHES;
        if (frame == 277) {
          s32 j;

          for (j = 0; j != HAURES_SHARDS; j++) {
            for (i = 0; i != HAURES_SHARD_CORNERS; i++) {
              struct ModelVertex *corner =
                  (struct ModelVertex *)cells + j * HAURES_SHARD_CORNERS + i;

              corner->z = 0;
              corner->x = gHaures_PartPoints[j * HAURES_SHARD_ROW + i * 2 + 2] -
                          gHaures_PartPoints[j * HAURES_SHARD_ROW];
              corner->y = gHaures_PartPoints[j * HAURES_SHARD_ROW + i * 2 + 3] -
                          gHaures_PartPoints[j * HAURES_SHARD_ROW + 1];
            }
            SHARD_FACE(mesh, j, 0)[1] = 1;
            SHARD_FACE(mesh, j, 0)[2] = 2;
            SHARD_FACE(mesh, j, 1)[1] = 2;
            SHARD_FACE(mesh, j, 1)[2] = 3;
            SHARD_FACE(mesh, j, 0)[0] = 0;
            SHARD_FACE(mesh, j, 1)[0] = 0;
            SHARD_FACE(mesh, j, 2)[0] = 0;
            SHARD_FACE(mesh, j, 2)[1] = 0;
            SHARD_FACE(mesh, j, 2)[2] = 0;
          }
        }
        // DEAD: the shards are solid and draw no texture; the ROM keeps the
        // stores that set one up.
        wideTexture.logWidth = 8;
        wideTexture.logHeight = 7;
        wideTexture.pixels = buffer;
        model->kind = MODEL_SOLID;
        model->texture = &wideTexture;
        model->vertices = verts;
        model->shade = (long)shades;
        {
          /* FAKEMATCH(dead): a never-true test of the byte scroll. */
          s32 scroll = model->scrollU;

          if (scroll > 255) {
            i = 0;
          }
        }
        for (i = 0; i != HAURES_SHARDS; i++) {
          gHaures_WingShades[i] = 1;
        }
        {
          for (i = 0; i != HAURES_SHARDS; i++) {
            struct Particle *p = &work->particles[i];
            model->mesh = mesh + i * HAURES_SHARD_MESH_BYTES;
            if ((u32)(FIXED_INTEGER_PART(p->x) + HAURES_SHARD_REACH) <=
                    DISPLAY_WIDTH / 2 + 2 * HAURES_SHARD_REACH &&
                FIXEDtoINT(p->y) >= -HAURES_SHARD_REACH &&
                FIXEDtoINT(p->y) <= BATTLE_VIEW_HEIGHT + HAURES_SHARD_REACH) {
              Matrix_Initialize();
              Matrix_Translate(p->x - toFIXED(64), p->y - toFIXED(64), 0);
              Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
              switch (i & 3) {
              case 0:
                Matrix_RotateX(p->z);
                Matrix_RotateZ(p->z);
                break;
              case 1:
                Matrix_RotateX(p->z);
                Matrix_RotateZ(-p->z);
                break;
              case 2:
                Matrix_RotateX(p->z);
                Matrix_RotateY(p->z);
                break;
              case 3:
                Matrix_RotateZ(p->z);
                Matrix_RotateY(-p->z);
                break;
              }
              Render_TransformVertices(cells + i * HAURES_SHARD_CORNER_BYTES,
                                       verts, HAURES_SHARD_CORNERS);
              Render_DrawModelList(model);
            }
            Particle_AdvanceWithGravity3D(p, 62, 0);
            p->x += toFIXED(4);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      if (frame <= 266) {
        work->stampPending = 1;
      }
      wait(1);
      frame++;
    } while (frame != HAURES_COMING_FRAMES &&
             !(gInput.repeat & (KEY_A | KEY_B)));
  }
  if (dived == 0) {
    Sprite_DestroyFar(work->sprites[0]);
    Sprite_DestroyFar(work->sprites[1]);
    for (i = 0; i != HAURES_FLAME_COUNT; i++) {
      Sprite_DestroyFar(work->sprites[HAURES_FIRST_FLAME + i]);
    }
    for (i = 0; i != HAURES_DROP_SPRITE_COUNT; i++) {
      Sprite_DestroyFar(work->sprites[HAURES_FIRST_DROP_SPRITE + i]);
    }
    Sprite_DestroyFar(work->sprites[HAURES_STAMP]);
  } else {
    for (i = 0; i != HAURES_BODY_PIECES; i++) {
      Sprite_DestroyFar(work->sprites[i]);
    }
  }
  background->busy = 0;
  Task_Remove(BattleEffect_ProcessPendingStampTransfer);
  BattleEffect_SetupBlendedDisplay();
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  BattleEffect_SpawnObjects(HAURES_BODY_PIECES, SPRITE_HAURES, 2);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                       SHEET_PALETTE_BYTES);
  // DEAD: the claw's six slashes are laid out as textures the charge never
  // draws; the ROM keeps the loading and the copy.
  Resource_LoadAndDecompress(RESOURCE_BLUE_ARC_SHEET_B, work->sheet, 1, 0);
  {
    Iwram_ClearWords(buffer, HAURES_ARCS * HAURES_ARC_TEXTURE_BYTES);
    for (i = 0; i != HAURES_ARCS; i++) {
      s32 j;

      for (j = 0; j != BLUE_ARC_HEIGHT; j++) {
        s32 m;

        for (m = 0; m != BLUE_ARC_WIDTH; m++) {
          buffer[(i * HAURES_ARC_TEXTURE_BYTES) + j * HAURES_ARC_TEXTURE_WIDTH +
                 m] =
              work->sheet[i * PICTURE_BYTES(BLUE_ARC) + j * BLUE_ARC_WIDTH + m];
        }
      }
    }
  }
  Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  // The charge: three passes through the foes, sparks streaming behind.
  for (frame = 0; frame != HAURES_FRAMES; frame++) {
    struct RenderModel *model;
    void *verts;

    if (frame == 0) {
      position.x = toFIXED(270);
      position.y = -toFIXED(22);
      velocity.x = -toFIXED(12);
      velocity.y = toFIXED(6);
      {
        s32 *variant = &EXTRA_PARTICLES->variant;

        for (i = 0; i != HAURES_SPARK_COUNT;
             i++, variant += sizeof(struct Particle) / sizeof(s32)) {
          *variant = -1;
        }
      }
    }
    if (frame == 20) {
      position.x = -toFIXED(10);
      position.y = -toFIXED(22);
      velocity.x = toFIXED(12);
      velocity.y = toFIXED(6);
    }
    if (frame == 40) {
      position.x = toFIXED(260);
      position.y = toFIXED(64);
      velocity.x = -toFIXED(8);
      velocity.y = 0;
    }
    {
      for (i = 0; i != HAURES_SPARK_COUNT; i++) {
        struct Particle *p = &EXTRA_PARTICLES[i];
        if (p->variant >= 0) {
          s32 n = (p->variant >> 3) + 1;

          draw[0](canvas, sprites + gBattleEffect_MotePictures[n - 1],
                  FIXED_INTEGER_PART(p->x) - n / 2,
                  FIXED_INTEGER_PART(p->y) - n, n, n * 2);
          Particle_AdvanceWithGravity2D(p, 63, -toFIXED(1) / 2);
          p->variant--;
        }
      }
    }
    if (frame <= 39) {
      struct Particle *p;

      // This frame's sparks start at spark frame * HAURES_SPARKS_PER_FRAME,
      // three rows of four on.
      for (i = 0,
          p = ((struct Particle(*)[4])EXTRA_PARTICLES)[frame * 2 + frame];
           i != HAURES_SPARKS_PER_FRAME; i++, p++) {
        s32 angle;
        s32 speed;

        p->x = INTtoFIXED((Random16() & 63) - 16) + position.x / 2;
        p->y = INTtoFIXED(Random16() & 31) + position.y;
        angle = Random16();
        speed = (Random16() & 127) + 128;
        p->velocityX = Trig_Sin(angle) * speed >> 7;
        p->velocityY = Trig_Cos(angle) * speed >> 6;
        p->variant = HAURES_SPARK_LIFE;
      }
    }
    if (frame <= 73) {
      s32 t;

      chargeScale = gHaures_OpeningScale;
      bodyPlace[3] = 0;
      t = frame - 20;
      if ((u32)t < HAURES_CHARGE_PASS_FRAMES) {
        // Mirrored for the pass from the left.
        chargeScale.x = -(FIXED_ONE + 1);
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = position.x - INTtoFIXED(gHaures_BodyPiecePoints[i][0]);
          bodyPlace[1] =
              toFIXED(272) -
              (INTtoFIXED(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = toFIXED(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&chargeScale, 0);
        }
      } else {
        for (i = 0; i != HAURES_BODY_PIECES; i++) {
          bodyPlace[0] = INTtoFIXED(gHaures_BodyPiecePoints[i][0]) + position.x;
          bodyPlace[1] =
              toFIXED(272) -
              (INTtoFIXED(gHaures_BodyPiecePoints[i][1]) + position.y);
          bodyPlace[2] = toFIXED(256);
          Render_ApplyProjectedPlacementFar(work->sprites[i], bodyPlace,
                                            (s32 *)&chargeScale, 0);
        }
      }
      position.x += velocity.x;
      position.y += velocity.y;
      if ((u32)frame < HAURES_CHARGE_PASS_FRAMES) {
        velocity.x += -toFIXED(1) / 2;
        velocity.y += toFIXED(1) / 2;
      } else if ((u32)t < HAURES_CHARGE_PASS_FRAMES) {
        velocity.x += toFIXED(1) / 2;
        velocity.y += toFIXED(1) / 4;
      } else if ((u32)(frame - 40) < HAURES_CHARGE_PASS_FRAMES) {
        velocity.x += -toFIXED(1) / 2;
      }
    }
    if (frame == 10) {
      Sound_PlayFar(SE_POWER_UP);
    }
    // Each pass strikes the foes, the last hardest.
    if (frame == 14) {
      Sound_PlayFar(SE_CLATTER);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                      toFIXED(8), toFIXED(4), 150);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 30) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 34) {
      Sound_PlayFar(SE_CLATTER);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                      toFIXED(8), toFIXED(4), 50);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 58) {
      Sound_PlayFar(SE_CRASH);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                      toFIXED(8), toFIXED(4), 200);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        work->shakeFrames = 8;
      }
    }
    // Three streaks of fire across each of the first two passes, then from
    // frame 56 a band of light that opens over the whole view.
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    model->shade = -8;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->texture = &bandTexture;
    bandTexture.logWidth = 6;
    bandTexture.logHeight = 5;
    model->mesh = gHaures_BandMesh;
    model->vertices = verts;
    bandTexture.pixels = work->sheet;
    model->scrollU = (frame * 8) & TEXTURE_SCROLL_MASK;
    for (i = 0; i != 2; i++) {
      s32 start = gHaures_BandStarts[i];

      if (frame >= start && frame < start + HAURES_STREAK_FRAMES) {
        FIXED amp =
            Trig_Sin((frame - start) * (DEGtoANG(180) / HAURES_STREAK_FRAMES)) /
            8;
        s32 x;
        s32 m;

        for (m = 0; m != 3; m++) {
          s32 y = -4;
          x = m * 20 - 16;
          Matrix_Initialize();
          if (i == 0) {
            Matrix_Translate(INTtoFIXED(x), INTtoFIXED(y), 0);
            Matrix_RotateZ(-DEGtoANG(45));
          } else {
            Matrix_Translate(INTtoFIXED(x - 16), INTtoFIXED(y), 0);
            Matrix_RotateZ(HAURES_SECOND_BAND_ROLL);
          }
          Matrix_Scale(FIXED_ONE * 3, amp, FIXED_ONE * 2);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    bandTexture.logWidth = 6;
    bandTexture.logHeight = 5;
    model->mesh = gHaures_BandMesh;
    model->kind = MODEL_SHADED_ADDITIVE;
    if ((u32)(frame - 56) <= 31) {
      s32 sh = 0;
      s32 v;
      s32 left;
      s32 right;
      struct ProjectedVertex *corner = verts;

      if (frame > 79) {
        sh = (80 - frame) * 8;
      }
      model->shade = sh;
      model->scrollU = (frame * 8) & TEXTURE_SCROLL_MASK;
      bandTexture.pixels = work->sheet;
      if (frame <= 71) {
        v = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 64) - DEGtoANG(315)) *
                       HAURES_BAND_REACH);
      } else if (frame > 79) {
        v = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 64) + -TRIG_TURN) *
                       HAURES_BAND_REACH);
      } else {
        v = HAURES_BAND_REACH;
      }
      left = HAURES_BAND_INSET;
      corner[0].y = HAURES_BAND_MIDDLE - v;
      corner[1].y = HAURES_BAND_MIDDLE - v;
      right = CANVAS_SIZE - 1 - HAURES_BAND_INSET;
      corner[0].x = left;
      corner[1].x = right;
      corner[2].x = left;
      corner[2].y = v + HAURES_BAND_MIDDLE;
      corner[3].x = right;
      corner[3].y = v + HAURES_BAND_MIDDLE;
      Render_DrawModelList(model);
    }
    Heap_Release(model);
    Heap_Release(verts);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  for (i = 0; i != HAURES_BODY_PIECES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The two rings sit at y = -36 and y = -18.
#define FIRST_RING_Y -36
#define RING_SPACING 18

/// Each side of the band shows an 8 x 16 column of the texture.
#define COLUMN_WIDTH 8
#define COLUMN_HEIGHT 16

/// The rings' radius.
#define CYLINDER_RADIUS 96

/// Builds the mesh of an open cylinder, the band Coatlicue shows: two rings of
/// `segments` vertices of radius CYLINDER_RADIUS around the y axis, and two
/// textured triangles for each side of the band, joining the first ring
/// (texture row 0) to the second (row 15). A triangle whose three vertices are
/// 0 ends the list. Coatlicue builds it with 32 segments.
void Mesh_BuildCylinderBand(struct MeshVertex *vertices,
                            struct MeshTriangle *triangles, s32 segments) {
  s32 ring;
  s32 i;

  for (ring = 0; ring != 2; ring++) {
    for (i = 0; i != segments; i++) {
      // The vertex: a point on the ring, CYLINDER_RADIUS out.
      vertices[ring * segments + i].x =
          (u32)(Trig_Sin(i * (TRIG_TURN / segments)) * (CYLINDER_RADIUS / 32)) /
          (FIXED_ONE / 32);
      vertices[ring * segments + i].y = ring * RING_SPACING + FIRST_RING_Y;
      vertices[ring * segments + i].z =
          (u32)(Trig_Cos(i * (TRIG_TURN / segments)) * (CYLINDER_RADIUS / 32)) /
          (FIXED_ONE / 32);

      // The side from this vertex to the next, as two triangles.
      if (ring < 1) {
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((ring * segments + i) *
                                        (2 * sizeof(struct MeshTriangle)) +
                                    (u32)triangles);
        s32 left = i * COLUMN_WIDTH;
        s32 right = left + COLUMN_WIDTH - 1;

        triangle[0].uv[0][0] = right;
        triangle[0].uv[0][1] = 0;
        triangle[0].uv[1][0] = right;
        triangle[0].uv[1][1] = COLUMN_HEIGHT - 1;
        triangle[0].uv[2][0] = left;
        triangle[0].uv[2][1] = COLUMN_HEIGHT - 1;
        {
          struct MeshTriangle *second = triangle + 1;

          second->uv[0][0] = left;
          second->uv[0][1] = 0;
          second->uv[1][0] = right;
          second->uv[1][1] = 0;
          second->uv[2][0] = left;
          second->uv[2][1] = COLUMN_HEIGHT - 1;

          // (next, next on ring 1, this on ring 1), then (this, next, this on
          // ring 1).
          triangle->vertex[0] = (i + 1) % segments + ring * segments;
          triangle->vertex[1] = (i + 1) % segments + segments + ring * segments;
          triangle->vertex[2] = i % segments + segments + ring * segments;
          second->vertex[0] = i % segments + ring * segments;
          second->vertex[1] = (i + 1) % segments + ring * segments;
          second->vertex[2] = i % segments + segments + ring * segments;
        }
      }
    }
  }

  // The end of the list.
  triangles[segments * 2].vertex[0] = 0;
  triangles[segments * 2].vertex[1] = 0;
  triangles[segments * 2].vertex[2] = 0;
}
#undef SCRATCH
#undef SHARD_FACE
#undef FIRST_RING_Y
#undef RING_SPACING
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
#undef CYLINDER_RADIUS
