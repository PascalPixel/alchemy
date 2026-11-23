/// @file
/// The mirrored wall of Berserk, Spasm, Recovery and Sack, and the summon
/// Daedalus with its second strike.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define MIRRORED_WALL_FRAMES 48      ///< frames of the wall
#define MIRRORED_WALL_DRAW_FRAMES 28 ///< frames the wall's cels are drawn

/// The wall's three 64x64 cels, each a half wall and its mirror image, built
/// at the start of the sheet; EarthWallSheet decodes behind them, at
/// PICTURE_OFFSET.
#define CEL_SIZE 4096
#define PICTURE_OFFSET 16384

/// EarthWallSheet's three half walls of flame, each taller than the last
/// (earth_wall_11.png to _13.png): where each begins in the decoded sheet, and
/// its size. The Broken Seal's mirrored wall draws the same three pictures.
#define EARTH_WALL_LOW 2702
#define EARTH_WALL_LOW_WIDTH 17
#define EARTH_WALL_LOW_HEIGHT 49
#define EARTH_WALL_MIDDLE 3535
#define EARTH_WALL_MIDDLE_WIDTH 19
#define EARTH_WALL_MIDDLE_HEIGHT 55
#define EARTH_WALL_HIGH 4580
#define EARTH_WALL_HIGH_WIDTH 20
#define EARTH_WALL_HIGH_HEIGHT 64

/// Copies half wall `half` into the bottom middle of a 64x64 cel, once as it
/// is and once mirrored, so the cel is a whole wall.
#define MIRROR_HALF(cel, half)                                                 \
  for (y = 0; y != half##_HEIGHT; y++) {                                       \
    for (x = 0; x != half##_WIDTH; x++) {                                      \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - half##_HEIGHT) * 64 + 32 -      \
                  half##_WIDTH + x] =                                          \
          work->sheet[PICTURE_OFFSET + half + y * half##_WIDTH + x];           \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - half##_HEIGHT) * 64 + 31 +      \
                  half##_WIDTH - x] =                                          \
          work->sheet[PICTURE_OFFSET + half + y * half##_WIDTH + x];           \
    }                                                                          \
  }

/// Raises a mirrored wall at the first target's feet while the target glows.
void BattleEffect_RunMirroredWall(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  struct ModelTexture texture;
  struct ScreenPoint target;
  struct RenderModel *quad;
  void *vertices;
  s32 frame;
  s32 cel;
  s32 y;
  s32 x;

  // Build the three cels from their halves.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_EARTH_WALL_SHEET,
                             work->sheet + PICTURE_OFFSET, 1, 1);
  Iwram_ClearWords(work->sheet, PICTURE_OFFSET);
  MIRROR_HALF(0, EARTH_WALL_LOW)
  MIRROR_HALF(1, EARTH_WALL_MIDDLE)
  MIRROR_HALF(2, EARTH_WALL_HIGH)
  if (mode == MIRRORED_WALL_SACK)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  else if (mode == MIRRORED_WALL_RECOVERY)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_FIREBALL_SHEET),
                    SHEET_PALETTE_BYTES);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                    SHEET_PALETTE_BYTES);

  // Centre the canvas on the target.
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (CANVAS_SIZE / 2 - target.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  if (mode == MIRRORED_WALL_RECOVERY) {
    work->shakeFrames = 0;
    Sound_PlayFar(SE_POWER_UP);
  } else if (mode == MIRRORED_WALL_BERSERK || mode == MIRRORED_WALL_SACK) {
    work->shakeFrames = 8;
    Sound_PlayFar(SE_POWER_UP);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != MIRRORED_WALL_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == MIRRORED_WALL_RECOVERY)
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 32);
      else
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 8 && mode == MIRRORED_WALL_SPASM)
      Sound_PlayFar(SE_RECOVERY);

    // Cels 0, 1 and 2 four frames each, then 1 and 2 by turns.
    if (frame < 32) {
      cel = frame / 4;
      if (cel > 2)
        cel = (cel & 1) + 1;
      if (frame < MIRRORED_WALL_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(32);
        quad = Render_AllocateModelList(1);
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + cel * CEL_SIZE;
        quad->kind = MODEL_SHADED_MAX;
        quad->mesh = gBattleEffect_SquareMesh;
        quad->texture = &texture;
        quad->shade = gMirroredWall_Shades[mode];
        quad->vertices = vertices;
        Matrix_Initialize();
        Matrix_Translate(0, INTtoFIXED(target.y - 58), 0);
        Matrix_ScaleUniform(gMirroredWall_Scales[mode] << 1);
        Render_TransformVertices(gBattleEffect_StandingSquareCorners, vertices,
                                 4);
        Render_DrawModelList(quad);
        Heap_Release(quad);
        Heap_Release(vertices);
      }
    }

    if (mode == MIRRORED_WALL_SPASM)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

/// Packs YellowOrbSheet's six pictures to half width, keeping every second
/// byte, the odd column's. stride: the source's bytes a row; the packed rows
/// are half as long.
void BattleEffect_PackTileRows(u8 *src, u8 *dst, s32 stride) {
  s32 row = 0;
  s32 rows = YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT;
  s32 offset = 0;

  for (; row != rows; row++) {
    s32 x;

    for (x = 0; x != YELLOW_ORB_WIDTH; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
  }
}

/// A sprite (struct Sprite) as Daedalus reads it: its attribute 2 fields,
/// scale, VRAM cache slot, rotation, size and drawing offset.
struct DaedalusSprite {
  /// The part's link and its attributes 0 and 1, which the effect leaves alone.
  u8 linkAndPosition[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2];
  FIXED scale;
  u8 slot;
  u8 cacheFlags;
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
};

/// The effect work's sprite slot `i`, read as Daedalus's sprite.
#define DAEDALUS_SPRITE(i) ((struct DaedalusSprite *)work->sprites[i])

/// A sprite's first ten bytes, its attribute 2 fields after eight others.
struct SpriteAttributes {
  u8 linkAndPosition[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

typedef void (*CopyFunc)(void *, const void *, s32);
typedef void (*BlitFunc)(void *, const void *, s32, s32, s32, s32);

/// Iwram_CopyWords called at its offset from IwramIrqMain, as the trade's
/// snapshot copies call it.
#define IWRAM_COPY_WORDS_LINKED                                                \
  ((CopyFunc)(IwramIrqMain + Iwram_CopyWordsOffset))

/// Where the effect's pictures are decoded before they are copied to OBJ VRAM.
#define DECODE_BUFFER Ram_MapCellBuffer
/// The particles the sky scene, the fall and the second strike's sparks keep in
/// EXTRA_PARTICLES.
#define DAEDALUS_PARTICLE_COUNT (DAEDALUS_MISSILE_COUNT * DAEDALUS_TRAIL_LENGTH)

/// The opening's mote copies: DAEDALUS_MOTE_COPIES copies of
/// ParticleSpritesA's mote DAEDALUS_MOTE, 4 pixels wide and 8 tall, in the
/// battle sheet's heap slot, the first DAEDALUS_MOTE_DIM_FIRST levels dimmer
/// than the mote and each next DAEDALUS_MOTE_DIM_STEP more. Nothing in
/// Daedalus draws them.
#define DAEDALUS_MOTE 3
#define DAEDALUS_MOTE_BYTES ((DAEDALUS_MOTE + 1) * (DAEDALUS_MOTE + 1) * 2)
#define DAEDALUS_MOTE_COPIES 10
#define DAEDALUS_MOTE_DIM_FIRST 40
#define DAEDALUS_MOTE_DIM_STEP 4

/// ForestCanopyTiles: two rows of eight 32x32 pictures of treetops, in OBJ
/// palette CANOPY_PALETTE. Each row is drawn by DAEDALUS_CANOPY_ROW sprites,
/// the last showing the picture before the first, so the row wraps as it
/// scrolls.
#define CANOPY_PALETTE 15
#define CANOPY_ROWS 2
#define CANOPY_PICTURES 8
#define CANOPY_PICTURE_SIZE 32
#define CANOPY_ROW_BYTES (CANOPY_PICTURES * OBJ_32X32_4BPP_BYTES)
/// A whole number of rows of pictures, added to a picture's place before the
/// remainder by CANOPY_PICTURES takes it away again.
#define CANOPY_WRAP (CANOPY_PICTURES * 8)
/// The canopy's top in the sky scene. The opening shows it DAEDALUS_CLIMB
/// pixels higher; as the camera climbs, the sky scene lowers the canopy by
/// that much and raises the machine twice as far.
#define CANOPY_Y 132
#define DAEDALUS_CLIMB 29

/// RocketFlameSheet: six 12x24 flames, each dimmer than the last.
#define ROCKET_FLAME_WIDTH 12
#define ROCKET_FLAME_HEIGHT 24
#define ROCKET_FLAME_COUNT 6
/// BirdFlockSheet's 16x16 birds, and the twelve steps of a wing beat
/// (gDaedalus_WingBeatPictures).
#define BIRD_WIDTH 16
#define BIRD_HEIGHT 16
#define BIRD_WING_BEAT 12

/// The sky scene's pictures: the rocket's flames, then the bursts, then the
/// birds.
#define DAEDALUS_FLASH_BURST_PICTURES                                          \
  (ROCKET_FLAME_COUNT * PICTURE_BYTES(ROCKET_FLAME))
#define DAEDALUS_BIRD_FLOCK                                                    \
  (DAEDALUS_FLASH_BURST_PICTURES +                                             \
   FLASH_BURST_PICTURE_COUNT * PICTURE_BYTES(FLASH_BURST))

/// The fall's pictures: the bursts, then the rocket's flames.
#define DAEDALUS_ROCKET_FLAMES                                                 \
  (FLASH_BURST_PICTURE_COUNT * PICTURE_BYTES(FLASH_BURST))

/// The sky scene's sprites: the canopy's two rows; the twenty-one 32x32 pieces
/// of the machine (BronzeAutomatonTiles), among them its missiles' cover, its
/// four hull plates and its spinning blade; the 32x64 rocket it launches; and
/// the missile with fifteen copies. The work's first particles keep the canopy
/// pictures' tile numbers in their variants, row by row as the sprites.
#define DAEDALUS_CANOPY_ROW (CANOPY_PICTURES + 1)
#define DAEDALUS_MACHINE 18
#define DAEDALUS_MACHINE_PIECES 21
/// BronzeAutomatonTiles' colours, OBJ palette 0 to 223. The first of them all
/// green is the machine's core, which the sky scene recolours as it glows.
#define DAEDALUS_MACHINE_COLORS 224
/// The core's colour: dark at first; from DAEDALUS_GLOW_START it brightens a
/// level a frame towards orange (red at most, CORE_GLOW_GREEN and
/// CORE_GLOW_BLUE) until DAEDALUS_GLOW_END, then cools towards a dark red
/// (CORE_EMBER_RED).
#define CORE_DARK RGB(11, 8, 6)
#define CORE_GLOW_GREEN 20
#define CORE_GLOW_BLUE 6
#define CORE_EMBER_RED 20
#define DAEDALUS_GLOW_START 542
#define DAEDALUS_GLOW_END 558

/// Until DAEDALUS_SWAY_STOP the machine's legs swing DAEDALUS_SWAY pixels to
/// and fro, opposite ways, once every 256 frames, and it bobs up twice a swing,
/// never below its place. It lurches as its core glows and again as it fires,
/// each lurch half a turn of a sine over 16 frames.
#define DAEDALUS_SWAY 4
#define DAEDALUS_SWAY_STEP (TRIG_TURN / 256)
#define DAEDALUS_SWAY_STOP 538
#define DAEDALUS_LURCH_STEP (TRIG_TURN / 32)
#define DAEDALUS_RECOIL_START 560
#define DAEDALUS_RECOIL_END 576
/// The frame the machine fires: a ring of fire and a ring of flames spread
/// from it, and two frames later the rocket leaves.
#define DAEDALUS_FIRE_FRAME 568

/// The rocket: where it rests in the machine and leaves from, how far its
/// nozzle is from its middle, the pull of its flight, from which frame of it
/// the camera would let it draw ahead, and its scale, a little over one so
/// that it turns in a sprite box twice its size and keeps its fins.
#define DAEDALUS_ROCKET_X 152
#define DAEDALUS_ROCKET_Y 92
#define ROCKET_NOZZLE 27
#define ROCKET_GRAVITY toFIXED(0.03431)
#define ROCKET_LEAD_START 230
#define ROCKET_SCALE toFIXED(1.01561)

/// The rocket's flight, in frames from its launch. It sinks a little faster
/// over its first ROCKET_SINK_FRAMES and speeds forward from
/// ROCKET_THRUST_START. It lifts from ROCKET_LIFT_START to ROCKET_LIFT_END,
/// tipping its nose up from ROCKET_NOSE_UP_START to ROCKET_NOSE_UP_END. From
/// ROCKET_TURN_START to ROCKET_TURN_END it turns its nose up as it climbs, the
/// nose dipping a moment from ROCKET_DIP_START to ROCKET_DIP_END; it climbs
/// more gently until ROCKET_SOAR_START and steeply after. Bursts rise from the
/// foot of the view every fourth frame while its nose tips up, and again from
/// ROCKET_BURSTS_START to ROCKET_BURSTS_END.
#define ROCKET_THRUST_START 30
#define ROCKET_LIFT_START 40
#define ROCKET_NOSE_UP_START 50
#define ROCKET_NOSE_UP_END 60
#define ROCKET_LIFT_END 68
#define ROCKET_SINK_FRAMES 80
#define ROCKET_TURN_START 108
#define ROCKET_BURSTS_START 113
#define ROCKET_DIP_START 120
#define ROCKET_DIP_END 123
#define ROCKET_TURN_END 140
#define ROCKET_SOAR_START 160
#define ROCKET_BURSTS_END 200

/// The ring of flames the machine sends out as it fires: FLAME_RING_COUNT
/// flames round a ring centred on gDaedalus_RingX and FLAME_RING_Y, its radius
/// spreading for FLAME_RING_FRAMES frames while each flame burns down through
/// RocketFlameSheet's pictures, two frames each.
#define FLAME_RING_COUNT 32
#define FLAME_RING_STEP (TRIG_TURN / FLAME_RING_COUNT)
#define FLAME_RING_Y 60
#define FLAME_RING_FRAMES 26

/// From DAEDALUS_SPLASH_START, flames burst along four lines of six points
/// over the hull (gDaedalus_SplashPoints), a point every two frames, each
/// burning down through RocketFlameSheet's pictures two frames each; the hull
/// falls away after them.
#define DAEDALUS_SPLASH_START 490

/// The sky scene's frames, unless A or B ends it sooner.
#define DAEDALUS_SKY_FRAMES 800

/// The fall's frames, and its runs of EXTRA_PARTICLES: each missile's
/// FALL_BURST_SLOTS, of which its two bursts use the first, and from
/// FALL_TRAILS each missile's trail of FALL_TRAIL_LENGTH flames.
#define DAEDALUS_FALL_FRAMES 109
#define FALL_BURST_SLOTS 8
#define FALL_TRAILS 256
#define FALL_TRAIL_LENGTH 16
/// The angle a missile's dive turns by for each unit of its velocityZ, a
/// spread the fall leaves at 0.
#define FALL_DIVE_SPREAD 40

/// The missiles fire in pairs, the first DAEDALUS_VOLLEY_START frames after
/// their cover flies off and the next every DAEDALUS_VOLLEY_STEP frames.
#define DAEDALUS_VOLLEY_START 10
#define DAEDALUS_VOLLEY_STEP 6

/// The machine's pieces, 32 pixels square, by their place in
/// BronzeAutomatonTiles: the two of its top; its core; the five of its body
/// behind the hull; the five of the leg on the right, whose thigh holds the
/// front missiles behind a cover; the cover; the two of the leg on the left;
/// the four hull plates; and the blade.
#define DAEDALUS_TOP DAEDALUS_MACHINE
#define DAEDALUS_CORE (DAEDALUS_MACHINE + 2)
#define DAEDALUS_BODY (DAEDALUS_MACHINE + 3)
#define DAEDALUS_RIGHT_LEG (DAEDALUS_MACHINE + 8)
#define DAEDALUS_COVER (DAEDALUS_MACHINE + 13)
#define DAEDALUS_LEFT_LEG (DAEDALUS_MACHINE + 14)
#define DAEDALUS_HULL (DAEDALUS_MACHINE + 16)
#define DAEDALUS_BLADE (DAEDALUS_MACHINE + 20)
#define DAEDALUS_PIECE_SIZE 32
#define DAEDALUS_ROCKET (DAEDALUS_MACHINE + DAEDALUS_MACHINE_PIECES)
#define DAEDALUS_MISSILE 41
#define DAEDALUS_MISSILE_COPIES 15
#define DAEDALUS_MISSILE_COUNT (DAEDALUS_MISSILE_COPIES + 1)
#define DAEDALUS_MISSILE_PALETTE 14

/// Runs of EXTRA_PARTICLES in the sky scene: each missile's trail of
/// DAEDALUS_TRAIL_LENGTH, the flames of the rocket's launch, its exhaust, the
/// bursts that rise from the foot of the view as it climbs, and the birds that
/// drift across. The later runs share the last missiles' trails.
#define DAEDALUS_TRAIL_LENGTH 32
#define DAEDALUS_LAUNCH_FLAMES 312
#define DAEDALUS_LAUNCH_FLAME_COUNT 16
#define DAEDALUS_EXHAUST 400
#define DAEDALUS_EXHAUST_FLAMES 32
#define DAEDALUS_BURSTS 432
#define DAEDALUS_BURST_COUNT 32
#define DAEDALUS_BIRDS 502
#define DAEDALUS_BIRD_COUNT 10

/// In the sky scene the machine comes apart: the missiles' cover, which flies
/// off and times the missiles, the four hull plates, the rocket and the blade
/// each move by a particle of their own, as do the spreading ring of flames
/// and the sixteen missiles.
#define DAEDALUS_COVER_MOTION 20
#define DAEDALUS_HULL_MOTION 21
#define DAEDALUS_ROCKET_MOTION 22
#define DAEDALUS_BLADE_MOTION 23
#define DAEDALUS_RING_MOTION 30
#define DAEDALUS_MISSILE_MOTIONS 32

/// Daedalus: the battle flashes away to a cloudy blue sky over a forest. In the
/// sky scene the bronze machine climbs into view among birds, throws off the
/// cover of its missiles and its blade as the sixteen missiles fly, sheds its
/// hull plates, and its core glows until it sends out a ring of fire and
/// launches its rocket; A or B skips the scene. Then the battle comes back and
/// the missiles dive onto the foes in bursts of fire. Its second strike
/// (BattleEffect_RunDaedalusStrike) comes at the end of the next turn.
///
/// Abilities: Daedalus.
void BattleEffect_RunDaedalus(struct BattleEffectParams *args) {
  s16 targets[UNIT_MAX];
  struct SpritePlacement canopyPlace;
  struct SpritePlacement place;
  struct SpritePlacement missilePlace;
  struct Scale canopyScale;
  void *blit[2];
  struct Scale pieceScale;
  struct ModelTexture fireRingTexture;
  struct Scale missileScale;
  u8 *canvas;
  s32 frame;
  u8 *dimMotes;
  struct BattleBackgroundWork *view;
  struct BattleWork *battle;
  FIXED camera;
  FIXED prevCamera;
  FIXED canopyX;
  s32 canopyScroll;
  // The camera's climb still to come, from -DAEDALUS_CLIMB pixels up to 0.
  FIXED climb;
  s32 canopyPhase;
  s32 savedScrollX;
  s32 bgX;
  struct BattleEffectWork *work;
  s32 k;
  s32 i;
  s32 j;
  // The mote copies' spacing, in halfwords.
  s32 step = DAEDALUS_MOTE_BYTES / 2;
  // Reused: the sky scene's ring radius, and the 0 the fall stores.
  s32 radius;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  dimMotes = Ram_HeapSlots->battleSheet;
  view = Ram_HeapSlots->battleBackground;
  battle = Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  RagingHeat_FillGradientCanvas();
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  camera = 0;
  canopyScroll = 0;
  climb = -toFIXED(DAEDALUS_CLIMB);
  canopyPhase = 0;
  savedScrollX = gBgScroll[1].x;
  bgX = -80;

  for (frame = 0; frame != SUMMON_OPENING_FRAMES; frame++) {
    if (frame == SUMMON_OPENING_LOAD_FRAME) {
      u16 *col;
      s32 n;
      s32 off;
      u8 *out;
      s32 k;
      s32 r, g, b;
      s32 v;
      const u8 *src;
      struct DaedalusSprite *obj;

      VramCache_FreeEntry(battle->arrowTiles);
      gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
      targets[0] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForListFar(targets, 0);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_VIOLET_SKY_BACKDROP, 0);
      for (col = (u16 *)&BG_PLTT_COLOR(6, 0), i = 0;
           i != BATTLE_BACKGROUND_COLORS; i++, col++) {
        s32 green;
        s32 red;

        // Each colour 4 levels darker, its red and green traded.
        red = RGB_RED(*col);
        green = RGB_GREEN(*col);
        b = RGB_BLUE(*col) - 4;
        g = green - 4;
        r = red - 4;
        if (g < 0)
          g = 0;
        if (r < 0)
          r = 0;
        if (b < 0)
          b = 0;
        if (g > RGB_LEVEL_MAX)
          g = RGB_LEVEL_MAX;
        if (r > RGB_LEVEL_MAX)
          r = RGB_LEVEL_MAX;
        if (b > RGB_LEVEL_MAX)
          b = RGB_LEVEL_MAX;
        *col = RGB(g, r, b);
      }
      work->scrollTimer = 0;
      work->scrollInterval = 5;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
      view->busy = 1;
      Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, work->sheet, 0,
                                 0);
      // The dimmed mote copies, from the work's sheet.
      off = offsetof(struct BattleEffectWork, sheet);
      n = DAEDALUS_MOTE;
      for (j = 0; j != DAEDALUS_MOTE_COPIES; j++) {
        k = j * step;
        i = 0;
        // v first holds the copy's byte offset.
        v = k * 2;
        out = (u8 *)(v + (s32)dimMotes);
        do {
          v = ((u8 *)work)[gBattleEffect_MotePictures[n] + i + off];
          if (v != 0) {
            v = v - j * DAEDALUS_MOTE_DIM_STEP - DAEDALUS_MOTE_DIM_FIRST;
            if (v <= 0)
              v = 1;
          }
          *out++ = v;
          // Mote picture n is n + 1 pixels wide and twice as tall.
        } while (++i != (n + 1) * (n + 1) * 2);
      }
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
      Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(CANOPY_PALETTE, 0), src,
                      PALETTE_BANK_BYTES);
      src += PALETTE_BANK_BYTES;
      Decompress_TaggedLz(src, work->sheet);
      for (j = 0; j != CANOPY_ROWS; j++) {
        for (i = 0; i != CANOPY_PICTURES; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32,
                                             OBJ_PALETTE(CANOPY_PALETTE));
          work->sprites[j * DAEDALUS_CANOPY_ROW + i] = obj;
          obj->priority = 1;
          Iwram_CopyWords(
              (void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
              &work->sheet[j * CANOPY_ROW_BYTES + i * OBJ_32X32_4BPP_BYTES],
              OBJ_32X32_4BPP_BYTES);
        }
        obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32,
                                   OBJ_PALETTE(CANOPY_PALETTE));
        work->sprites[j * DAEDALUS_CANOPY_ROW + CANOPY_PICTURES] = obj;
        Iwram_CopyWords(obj, work->sprites[j * DAEDALUS_CANOPY_ROW],
                        offsetof(struct Sprite, lastNo));
        for (i = 0; i != CANOPY_PICTURES; i++)
          work->particles[j * DAEDALUS_CANOPY_ROW + i].variant =
              DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile;
      }
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_BRONZE_AUTOMATON_TILES);
      Iwram_CopyWords((void *)OBJ_PLTT, src, DAEDALUS_MACHINE_COLORS * 2);
      src += DAEDALUS_MACHINE_COLORS * 2;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      for (i = 0; i != DAEDALUS_MACHINE_PIECES; i++) {
        obj = EffectSprite_CreateWithTiles(32, 32,
                                           OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
        obj->priority = 3;
        work->sprites[DAEDALUS_MACHINE + i] = obj;
        Iwram_CopyWords(
            (void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
            DECODE_BUFFER + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
      }
      obj = EffectSprite_CreateWithTiles(32, 64,
                                         OBJ_SIZE_32X64 | OBJ_256_COLORS, 0);
      /* FAKEMATCH(dead): the slot's store alone in a once-only block; as the
       * plain store. */
      do {
        work->sprites[DAEDALUS_MACHINE + i] = obj;
      } while (0);
      obj->priority = 3;
      /* FAKEMATCH(dead): updates of i that cancel out. */
      i ^= frame;
      i ^= frame;
      Iwram_CopyWords((void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
                      DECODE_BUFFER + i * OBJ_32X32_8BPP_BYTES, 32 * 64);
      src = (const u8 *)Resource_GetTableEntry(RESOURCE_MISSILE_TILES);
      Iwram_CopyWords((void *)&OBJ_PLTT_COLOR(DAEDALUS_MISSILE_PALETTE, 0), src,
                      PALETTE_BANK_BYTES);
      src += PALETTE_BANK_BYTES;
      Decompress_TaggedLz(src, DECODE_BUFFER);
      obj = EffectSprite_CreateWithTiles(16, 8, OBJ_SIZE_16X8,
                                         OBJ_PALETTE(DAEDALUS_MISSILE_PALETTE));
      obj->priority = 1;
      work->sprites[DAEDALUS_MISSILE] = obj;
      Iwram_CopyWords((void *)(gVramCacheEntries[obj->slot].offset + OBJ_VRAM0),
                      DECODE_BUFFER, 256);
      /* FAKEMATCH(barrier): an empty asm before the missile's copies; as the
       * copies alone. */
      __asm__("");
      for (i = 0; i != DAEDALUS_MISSILE_COPIES; i++) {
        work->sprites[DAEDALUS_MISSILE + 1 + i] =
            EffectSprite_Acquire(16, 8, OBJ_SIZE_16X8, OBJ_PALETTE(15));
        Iwram_CopyWords(work->sprites[DAEDALUS_MISSILE + 1 + i],
                        work->sprites[DAEDALUS_MISSILE],
                        offsetof(struct Sprite, lastNo));
        DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->tile =
            DAEDALUS_SPRITE(DAEDALUS_MISSILE)->tile;
        // Missiles 8 to 15, the back row, behind the machine.
        if (i > 6)
          DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->priority = 2;
      }
    }
    // The canopy scrolls by half a pixel a frame; each time it has moved a
    // whole picture, every sprite takes the picture before its own.
    if (frame > SUMMON_OPENING_LOAD_FRAME) {
      struct SpritePlacement *canopyPlaceRef;
      s32 row;

      canopyScale = gDaedalus_CanopyScale;
      canopyPlace.groundHeight = 0;
      canopyPlace.height = toFIXED(255);
      canopyScroll += toFIXED(1) / 2;
      canopyX = canopyScroll + camera;
      if (canopyX > toFIXED(CANOPY_PICTURE_SIZE) - 1) {
        canopyPhase = (canopyPhase + CANOPY_PICTURES - 1) % CANOPY_PICTURES;
        canopyX -= toFIXED(CANOPY_PICTURE_SIZE);
        canopyScroll -= toFIXED(CANOPY_PICTURE_SIZE);
      }
      for (j = 0; j != CANOPY_ROWS; j++) {
        for (i = 0; i != DAEDALUS_CANOPY_ROW; i++) {
          struct SpriteAttributes *obj;
          s32 v;

          canopyPlaceRef = &canopyPlace;
          {
            s32 h;

            k = INTtoFIXED(i * CANOPY_PICTURE_SIZE) + canopyX;
            h = j * CANOPY_PICTURE_SIZE + CANOPY_Y - DAEDALUS_CLIMB;
            canopyPlaceRef->x = k;
            canopyPlaceRef->y = INTtoFIXED(h);
          }
          if (i == CANOPY_PICTURES) {
            obj = work->sprites[j * DAEDALUS_CANOPY_ROW + CANOPY_PICTURES];
            v = work->particles[j * DAEDALUS_CANOPY_ROW +
                                (canopyPhase + CANOPY_WRAP + CANOPY_PICTURES -
                                 1) %
                                    CANOPY_PICTURES]
                    .variant;
          } else {
            row = j * DAEDALUS_CANOPY_ROW;
            k = row;
            k += i;
            obj = work->sprites[k];
            v = work->particles[row + (canopyPhase + CANOPY_WRAP + i) %
                                          CANOPY_PICTURES]
                    .variant;
          }
          obj->tile = v & OBJ_TILE_MASK;
          // The ninth sprite wraps round to the left of the first.
          if (canopyPlace.x >
              toFIXED(CANOPY_PICTURES * CANOPY_PICTURE_SIZE) - 1)
            canopyPlace.x -= toFIXED(DAEDALUS_CANOPY_ROW * CANOPY_PICTURE_SIZE);
          Render_ApplyProjectedPlacementFar(
              work->sprites[j * DAEDALUS_CANOPY_ROW + i], &canopyPlace,
              &canopyScale, 0);
        }
      }
    }
    bgX += 4;
    REG_BG2X = bgX << 8;
    EFFECT_PRESENT_CANVAS(work);
  }

  {
    void **blits;
    u16 *pal;
    s32 swayX;
    s32 swayY;
    FIXED bobX;
    FIXED bobY;
    s32 *ringX;
    void *pixels;

    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Iwram_ClearWordsEntry(canvas, CANVAS_VIEW_BYTES);
    Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2X = 0;
    blits = blit;
    BattleEffect_FetchRectangleBlitters(0, blits);
    work->scrollTimer = 0;
    work->scrollInterval = 5;
    work->scrollStepX = -1;
    work->scrollStepY = 0;
    view->busy = 1;
    Resource_LoadAndDecompress(RESOURCE_ROCKET_FLAME_SHEET, work->sheet, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + DAEDALUS_FLASH_BURST_PICTURES, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_BIRD_FLOCK_SHEET,
                               work->sheet + DAEDALUS_BIRD_FLOCK, 0, 0);
    pixels = DECODE_BUFFER;
    Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    (const void *)Resource_GetTableEntry(RESOURCE_SMOKE_SHEET),
                    SHEET_PALETTE_BYTES);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    // The core's colour: the machine's first colour all green.
    for (i = 0; i != DAEDALUS_MACHINE_COLORS; i++) {
      pal = (u16 *)OBJ_PLTT + i;
      if (RGB_GREEN(*pal) == RGB_LEVEL_MAX)
        break;
    }
    for (i = 0; i != DAEDALUS_PARTICLE_COUNT; i++)
      EXTRA_PARTICLES[i].variant = -1;

    frame = 0;
    if (!(gInput.held & KEY_B) && !(gInput.repeat & KEY_A)) {
    skyFrame:
      if (frame == 0) {
        REG_BG2PA = Q_8_8(1);
        *pal = CORE_DARK;
        climb = -toFIXED(DAEDALUS_CLIMB);
        camera = 0;
        DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation = TRIG_TURN * 3 / 4;
        DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation = 0;
        DAEDALUS_SPRITE(DAEDALUS_COVER)->rotation = 0;
        work->particles[DAEDALUS_COVER_MOTION].x = 0;
        work->particles[DAEDALUS_COVER_MOTION].y = 0;
        work->particles[DAEDALUS_COVER_MOTION].velocityX = -toFIXED(6);
        work->particles[DAEDALUS_COVER_MOTION].velocityY = 0;
        work->particles[DAEDALUS_COVER_MOTION].variant = 0;
        work->particles[DAEDALUS_BLADE_MOTION].x = 0;
        work->particles[DAEDALUS_BLADE_MOTION].y = 0;
        work->particles[DAEDALUS_BLADE_MOTION].velocityX = -toFIXED(12);
        work->particles[DAEDALUS_BLADE_MOTION].velocityY = 0;
        work->particles[DAEDALUS_BLADE_MOTION].variant = 0;
        work->particles[DAEDALUS_HULL_MOTION].x = 0;
        work->particles[DAEDALUS_HULL_MOTION].y = 0;
        work->particles[DAEDALUS_HULL_MOTION].velocityX = -toFIXED(1);
        work->particles[DAEDALUS_HULL_MOTION].velocityY = 0;
        work->particles[DAEDALUS_HULL_MOTION].variant = 0;
        work->particles[DAEDALUS_ROCKET_MOTION].x = toFIXED(160);
        work->particles[DAEDALUS_ROCKET_MOTION].y = 0;
        work->particles[DAEDALUS_ROCKET_MOTION].velocityX = 0;
        work->particles[DAEDALUS_ROCKET_MOTION].velocityY = 0;
        work->particles[DAEDALUS_ROCKET_MOTION].variant = 0;
        // The front eight wait in the right thigh and the back eight behind
        // the body, in columns of three.
        for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++) {
          struct Particle *p;

          p = &work->particles[DAEDALUS_MISSILE_MOTIONS + i];
          p->x = INTtoFIXED((i / 3) * 4) + toFIXED(176);
          p->y = INTtoFIXED((i % 3) * 3 * 2) + toFIXED(74);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
        for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++) {
          struct Particle *p;

          p = &work->particles[DAEDALUS_MISSILE_MOTIONS +
                               DAEDALUS_MISSILE_COUNT / 2 + i];
          p->x = INTtoFIXED((i / 3) * 4) + toFIXED(112);
          p->y = INTtoFIXED((i % 3) * 3 * 2) + toFIXED(82);
          p->velocityX = 0;
          p->velocityY = 0;
          p->variant = 0;
        }
      }
      prevCamera = camera;
      if (frame >= DAEDALUS_GLOW_START && frame < DAEDALUS_GLOW_END) {
        s32 r, g, b;

        r = RGB_RED(*pal);
        g = RGB_GREEN(*pal);
        b = RGB_BLUE(*pal);
        if (r < RGB_LEVEL_MAX)
          r++;
        if (g < CORE_GLOW_GREEN)
          g++;
        if (b < CORE_GLOW_BLUE)
          b++;
        *pal = RGB(r, g, b);
      }
      if (frame >= DAEDALUS_GLOW_END) {
        s32 r, g, b;

        r = RGB_RED(*pal);
        g = RGB_GREEN(*pal);
        b = RGB_BLUE(*pal);
        if (r > CORE_EMBER_RED)
          r--;
        if (g > 0)
          g--;
        if (b > 0)
          b--;
        *pal = RGB(r, g, b);
      }
      // The cover flies off, and the missiles follow it two at a time.
      if (frame == 280) {
        Iwram_CopyWords(
            (void *)BG_PLTT,
            (const void *)Resource_GetTableEntry(RESOURCE_SMOKE_SHEET),
            SHEET_PALETTE_BYTES);
        work->particles[DAEDALUS_COVER_MOTION].variant = 1;
      }
      // The blade flies off, spinning.
      if (frame == 288)
        work->particles[DAEDALUS_BLADE_MOTION].variant = 1;
      if (frame == 279) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BG2PA = Q_8_8(0.5);
      }
      // The hull falls away.
      if (frame == 506)
        work->particles[DAEDALUS_HULL_MOTION].variant = 1;
      if (frame == 489) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        work->scrollStepX = 0;
      }
      if (frame == 528) {
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                     BGCNT_SCREEN_BASE(7);
      }
      if (frame == 0) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        for (i = 0; i != DAEDALUS_BIRD_COUNT; i++) {
          struct Particle *p;

          p = &EXTRA_PARTICLES[DAEDALUS_BIRDS + i];
          p->x = INTtoFIXED(gDaedalus_BirdStarts[i][0]);
          p->y = INTtoFIXED(gDaedalus_BirdStarts[i][1]);
          p->velocityX = gDaedalus_BirdSpeeds[i][0];
          p->velocityY = gDaedalus_BirdSpeeds[i][1];
          p->variant = Random16() & 127;
        }
      }
      for (i = 0; i != DAEDALUS_BIRD_COUNT; i++) {
        struct Particle *p;

        p = &EXTRA_PARTICLES[DAEDALUS_BIRDS + i];
        if (frame >= gDaedalus_BirdFrames[i]) {
          ((BlitFunc)blit[0])(
              canvas,
              &work->sheet[gDaedalus_WingBeatPictures[(p->variant / 4) %
                                                      BIRD_WING_BEAT] *
                               PICTURE_BYTES(BIRD) +
                           DAEDALUS_BIRD_FLOCK],
              FIXEDtoINT(p->x), FIXEDtoINT(p->y), BIRD_WIDTH, BIRD_HEIGHT);
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->variant++;
        }
      }
      if (frame <= 473)
        canopyScroll += toFIXED(1) / 2;
      canopyX = canopyScroll + camera;
      if (canopyX >= toFIXED(CANOPY_PICTURE_SIZE)) {
        canopyPhase = (canopyPhase + CANOPY_PICTURES - 1) % CANOPY_PICTURES;
        canopyX -= toFIXED(CANOPY_PICTURE_SIZE);
        canopyScroll -= toFIXED(CANOPY_PICTURE_SIZE);
      }
      if (climb < 0)
        climb += toFIXED(1) / 8;
      pieceScale = gDaedalus_MachineScale;
      bobX = 0;
      bobY = 0;
      if (frame >= DAEDALUS_SWAY_STOP) {
        swayX =
            Trig_Cos(DAEDALUS_SWAY_STOP * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
        swayY =
            Trig_Sin(DAEDALUS_SWAY_STOP * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
        if (frame >= DAEDALUS_GLOW_START && frame < DAEDALUS_GLOW_END) {
          s32 v;

          v = frame * DAEDALUS_LURCH_STEP -
              DAEDALUS_GLOW_START * DAEDALUS_LURCH_STEP;
          bobX = Trig_Sin(v) * 3;
          bobY = Trig_Sin(v) * 2;
        }
        if (frame >= DAEDALUS_RECOIL_START && frame < DAEDALUS_RECOIL_END) {
          s32 v;

          v = frame * DAEDALUS_LURCH_STEP -
              DAEDALUS_RECOIL_START * DAEDALUS_LURCH_STEP;
          bobX = Trig_Sin(v) * 2;
          bobY = Trig_Sin(v) * 3;
        }
      } else {
        swayX = Trig_Cos(frame * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
        swayY = Trig_Sin(frame * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
      }
      if (swayY > 0)
        swayY = -swayY;
      swayY -= climb * 2;
      place.groundHeight = 0;
      place.height = toFIXED(255);
      for (i = 0; i != 5; i++) {
        if (frame == gDaedalus_ClatterFrames[i]) {
          work->shakeFrames = 23;
          Sound_PlayFar(SE_CLATTER);
        }
      }
      if (frame == 510)
        Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      if (frame == DAEDALUS_FIRE_FRAME) {
        work->shakeFrames = 10;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 578)
        Sound_PlayFar(SE_WAVE_HIGH);
      for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++) {
        struct Particle *p;

        p = &work->particles[DAEDALUS_MISSILE_MOTIONS + i];
        if (p->variant != 0) {
          struct Particle *q;

          p->variant++;
          if (p->x > -toFIXED(16)) {
            p->x += p->velocityX;
            p->y += p->velocityY;
            if (p->velocityY > -toFIXED(1) && p->velocityY < toFIXED(1))
              p->velocityY += p->velocityZ;
          }
          if (p->velocityX > -toFIXED(8))
            p->velocityX -= toFIXED(1) / 2;
          q = &EXTRA_PARTICLES[i * DAEDALUS_TRAIL_LENGTH +
                               (p->variant & (DAEDALUS_TRAIL_LENGTH - 1))];
          q->variant = 0;
          q->x = p->x / 2;
          q->y = p->y - toFIXED(20);
          q->velocityX = 0;
          q->velocityY = 0;
        }
        // Missiles i and i + 8 fire together.
        if (work->particles[DAEDALUS_COVER_MOTION].variant ==
            (i & 7) * DAEDALUS_VOLLEY_STEP + DAEDALUS_VOLLEY_START) {
          Sound_PlayFar(SE_ITEM_BREAK);
          p->variant = 1;
          p->x += swayX;
          p->y += swayY - INTtoFIXED(gDaedalus_ShakeOffsets[work->shakeFrames]);
          p->velocityY = (Random16() & TRIG_ANGLE_MASK) - toFIXED(0.5);
          p->velocityZ = (Random16() & 0x3fff) - toFIXED(0.125);
        }
      }
      if (frame <= 489) {
        for (i = DAEDALUS_MISSILE_COUNT / 2; i != DAEDALUS_MISSILE_COUNT; i++) {
          struct Particle *p;

          p = &work->particles[DAEDALUS_MISSILE_MOTIONS + i];
          place.x = p->x + camera;
          place.y = p->y;
          // BUG: the back eight are placed with the sprites of missiles 7 to
          // 14, so missile 8 shares the front row's last sprite and the last
          // copy, behind the machine, is never placed.
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(
#ifdef BUGFIX
                work->sprites[DAEDALUS_MISSILE + i],
#else
                work->sprites[DAEDALUS_MISSILE - 1 + i],
#endif
                &place, &pieceScale, 0);
        }
      }
      if (frame == 578) {
        work->scrollInterval = 1;
        work->scrollTimer = 0;
      }
      if (frame >= 578 && frame < 594)
        work->scrollStepX = ~((frame - 578) / 8);
      {
        for (i = 0; i != 2; i++) {
          place.x = camera - swayX + bobX + toFIXED(120);
          place.y = INTtoFIXED(i * DAEDALUS_PIECE_SIZE -
                               gDaedalus_ShakeOffsets[work->shakeFrames]) +
                    swayY + bobY + toFIXED(100);
          Render_ApplyProjectedPlacementFar(
              work->sprites[DAEDALUS_LEFT_LEG + i], &place, &pieceScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[DAEDALUS_BLADE_MOTION];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 48 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
          DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation += TRIG_TURN / 128;
        }
        place.x = p->x - swayX + camera + toFIXED(120);
        place.y = INTtoFIXED(99 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + p->y;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BLADE], &place,
                                          &pieceScale, 0);
      }
      {
        struct Particle *p;
        s32 v;

        v = camera + bobX;
        place.x = v + toFIXED(142);
        place.y = INTtoFIXED(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_TOP], &place,
                                          &pieceScale, 0);
        place.x = v + toFIXED(174);
        place.y = INTtoFIXED(72 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_TOP + 1],
                                          &place, &pieceScale, 0);
        v += toFIXED(128);
        place.x = v;
        place.y = INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_CORE], &place,
                                          &pieceScale, 0);
        p = &work->particles[DAEDALUS_ROCKET_MOTION];
        if (frame > 602)
          BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
        if (frame == DAEDALUS_FIRE_FRAME + 2) {
          p->x = toFIXED(DAEDALUS_ROCKET_X);
          p->y = INTtoFIXED(DAEDALUS_ROCKET_Y -
                            gDaedalus_ShakeOffsets[work->shakeFrames]) +
                 swayY;
          p->variant = 1;
          p->velocityX = -toFIXED(4);
          p->velocityY = -toFIXED(5) / 16;
          for (i = 0; i != DAEDALUS_LAUNCH_FLAME_COUNT; i++) {
            struct Particle *q;

            q = &EXTRA_PARTICLES[DAEDALUS_LAUNCH_FLAMES + i];
            q->x = INTtoFIXED((Random16() & 15) + 56);
            q->y = INTtoFIXED((Random16() & 31) + 64);
            q->velocityX = -((Random16() & 31) + 8) << 12;
            q->velocityY = -((Random16() & 31) + 8) << 12;
            q->variant = 0;
          }
          for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++)
            EXTRA_PARTICLES[DAEDALUS_EXHAUST + i].variant = -1;
          for (i = 0; i != DAEDALUS_BURST_COUNT; i++)
            EXTRA_PARTICLES[DAEDALUS_BURSTS + i].variant = -1;
        }
        // The rocket's flight, its particle's variant counting its frames:
        // exhaust from its tenth frame, bursts every fourth frame of two
        // stretches, and the turns, climbs and pulls of its course.
        if (p->variant != 0) {
          if (p->variant > 9) {
            struct Particle *q;

            q = &EXTRA_PARTICLES[DAEDALUS_EXHAUST +
                                 (p->variant & (DAEDALUS_EXHAUST_FLAMES - 1))];
            q->variant = 0;
            q->velocityX =
                -(Trig_Sin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2);
            q->velocityY =
                Trig_Cos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2;
            q->x = (p->x + camera) / 2 -
                   ((Trig_Sin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) *
                     ROCKET_NOZZLE) >>
                    1);
            q->y = p->y +
                   Trig_Cos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) *
                       ROCKET_NOZZLE -
                   toFIXED(ROCKET_FLAME_HEIGHT / 2);
          }
          if (((p->variant >= ROCKET_NOSE_UP_START &&
                p->variant < ROCKET_NOSE_UP_END) ||
               (p->variant >= ROCKET_BURSTS_START &&
                p->variant < ROCKET_BURSTS_END)) &&
              (p->variant & 3) == 0) {
            struct Particle *q;

            q = &EXTRA_PARTICLES[DAEDALUS_BURSTS +
                                 (p->variant & (DAEDALUS_BURST_COUNT - 1))];
            q->variant = 0;
            q->velocityX = ((Random16() & 31) + 16) << 12;
            q->velocityY = (-Random16() | ~15) << 14;
            q->x = toFIXED(88);
            q->y = toFIXED(128);
          }
          prevCamera = camera;
          {
            struct Particle *f;

            // The camera follows the rocket, keeping it where it left the
            // machine.
            f = &work->particles[DAEDALUS_ROCKET_MOTION];
            // DEAD: the lead of two pixels a frame from ROCKET_LEAD_START,
            // which is 0 on the one frame that reaches it, the sky scene's
            // last; the ROM keeps its test and its sum.
            if (f->variant >= ROCKET_LEAD_START)
              camera +=
                  (toFIXED(DAEDALUS_ROCKET_X) -
                   (f->x + INTtoFIXED((f->variant - ROCKET_LEAD_START) << 1)) -
                   camera) /
                  4;
            else if (f->variant > 0)
              camera += (toFIXED(DAEDALUS_ROCKET_X) - f->x - camera) / 4;
          }
          if (p->variant > 1) {
            p->x += p->velocityX;
            p->y += p->velocityY;
          }
          if (p->variant > 7) {
            p->velocityX = p->velocityX * 63 / DAMPING_SCALE;
            p->velocityY = p->velocityY * 63 / DAMPING_SCALE + ROCKET_GRAVITY;
          }
          if (p->variant < ROCKET_SINK_FRAMES)
            p->velocityY += toFIXED(1) / 128;
          if (p->variant >= ROCKET_THRUST_START)
            p->velocityX -= toFIXED(1) / 4;
          if (p->variant >= ROCKET_LIFT_START && p->variant < ROCKET_LIFT_END)
            p->velocityY -= toFIXED(1) / 16;
          if (p->variant >= ROCKET_NOSE_UP_START &&
              p->variant < ROCKET_NOSE_UP_END) {
            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation =
                (p->variant << 7) - TRIG_TURN * 89 / 256;
            p->velocityY -= toFIXED(1) / 16;
          }
          if (p->variant >= ROCKET_TURN_START && p->variant < ROCKET_TURN_END) {
            if (p->variant >= ROCKET_DIP_START && p->variant < ROCKET_DIP_END)
              DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation -= TRIG_TURN * 3 / 512;
            else
              DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation += TRIG_TURN / 256;
            p->velocityY -= toFIXED(1) / 16;
          }
          if (p->variant >= ROCKET_TURN_END && p->variant < ROCKET_SOAR_START)
            p->velocityY -= toFIXED(1) / 32;
          if (p->variant >= ROCKET_SOAR_START)
            p->velocityY -= toFIXED(1) / 8;
          for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++) {
            struct Particle *q;

            q = &EXTRA_PARTICLES[DAEDALUS_EXHAUST + i];
            if ((u32)q->variant < ROCKET_FLAME_COUNT) {
              ((BlitFunc)blits[1])(
                  canvas,
                  &work->sheet[q->variant * PICTURE_BYTES(ROCKET_FLAME)],
                  (FIXEDtoINT(q->x)) - 2,
                  (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                  ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
              Particle_AdvanceWithGravity2D(q, PARTICLE_DAMPING_NONE, 0);
              q->variant++;
            }
          }
          for (i = 0; i != DAEDALUS_BURST_COUNT; i++) {
            struct Particle *q;

            q = &EXTRA_PARTICLES[DAEDALUS_BURSTS + i];
            if ((u32)q->variant < FLASH_BURST_PICTURE_COUNT * 4) {
              ((BlitFunc)blits[1])(
                  canvas,
                  &work->sheet[q->variant / 4 * PICTURE_BYTES(FLASH_BURST) +
                               DAEDALUS_FLASH_BURST_PICTURES],
                  (FIXEDtoINT(q->x)) - FLASH_BURST_WIDTH / 2,
                  (FIXEDtoINT(q->y)) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
              q->velocityX += toFIXED(1) / 16;
              Particle_AdvanceWithGravity2D(q, PARTICLE_DAMPING_NONE, 0);
              q->variant++;
            }
          }
          p->variant++;
        }
        pieceScale.x = ROCKET_SCALE;
        pieceScale.y = ROCKET_SCALE;
        if (p->variant == 0) {
          place.x = camera + bobX + toFIXED(DAEDALUS_ROCKET_X);
          place.y = INTtoFIXED(DAEDALUS_ROCKET_Y -
                               gDaedalus_ShakeOffsets[work->shakeFrames]) +
                    swayY + bobY;
        } else {
          place.x = p->x + camera;
          place.y = p->y;
        }
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_ROCKET],
                                          &place, &pieceScale, 0);
      }
      DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetX = 0;
      DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetY = 32;
      // The ring of fire: FireRingTexture on a square laid nearly flat,
      // growing by a quarter a frame, bright for seven frames and fading out
      // over the next three.
      if (frame >= DAEDALUS_FIRE_FRAME) {
        struct RenderModel *fireRing;
        void *buffer;
        s32 v;
        s32 z;

        buffer = Heap_AllocEwram(512);
        fireRing = Render_AllocateModelList(1);
        fireRingTexture.logWidth = 7;
        fireRingTexture.logHeight = 7;
        fireRingTexture.pixels = pixels;
        fireRing->texture = &fireRingTexture;
        fireRing->flags = 0;
        fireRing->mesh = gBattleEffect_LargeSquareMesh;
        fireRing->kind = MODEL_SHADED_ADDITIVE;
        fireRing->vertices = buffer;
        v = frame - DAEDALUS_FIRE_FRAME;
        z = 96 - (v << 4);
        if (z > 0)
          z = 0;
        if (z > MODEL_SHADE_INVISIBLE) {
          fireRing->shade = z;
          Matrix_Initialize();
          Matrix_Translate(camera + toFIXED(2), toFIXED(8), 0);
          Matrix_RotateZ(TRIG_TURN * 67 / 256);
          Matrix_RotateX(-TRIG_TURN / 32);
          Matrix_ScaleUniform(v * (FIXED_ONE / 4) + FIXED_ONE * 131 / 512);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, buffer,
                                   4);
          Render_DrawModelList(fireRing);
        }
        Heap_Release(fireRing);
        Heap_Release(buffer);
      }
      if (frame == DAEDALUS_FIRE_FRAME) {
        work->particles[DAEDALUS_RING_MOTION].x = 0;
        work->particles[DAEDALUS_RING_MOTION].velocityX = toFIXED(8);
      }
      for (j = 0; j != 1; j++) {
        ringX = &work->particles[DAEDALUS_RING_MOTION].x;
        if (frame >= gDaedalus_RingStarts[j] &&
            frame < gDaedalus_RingStarts[j] + FLAME_RING_FRAMES) {
          s32 n;

          radius = FIXEDtoINT(*ringX);
          n = (frame - gDaedalus_RingStarts[j]) / 2;
          *ringX += work->particles[DAEDALUS_RING_MOTION].velocityX;
          work->particles[DAEDALUS_RING_MOTION].velocityX =
              work->particles[DAEDALUS_RING_MOTION].velocityX * 48 /
              DAMPING_SCALE;
          if (n < ROCKET_FLAME_COUNT) {
            for (i = 0; i != FLAME_RING_COUNT; i++) {
              s32 v;

              v = (FIXEDtoINT(Trig_Sin(i * FLAME_RING_STEP) * radius / 2)) +
                  gDaedalus_RingX[j];
              ((BlitFunc)blit[0])(
                  canvas, &work->sheet[n * PICTURE_BYTES(ROCKET_FLAME)],
                  v - ROCKET_FLAME_WIDTH / 2,
                  (FIXEDtoINT(Trig_Cos(i * FLAME_RING_STEP) * radius * 2)) +
                      FLAME_RING_Y,
                  ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
            }
          }
        }
      }
      pieceScale.x = FIXED_ONE;
      pieceScale.y = FIXED_ONE;
      DAEDALUS_SPRITE(DAEDALUS_BODY)->priority = 2;
      DAEDALUS_SPRITE(DAEDALUS_BODY + 1)->priority = 2;
      DAEDALUS_SPRITE(DAEDALUS_BODY + 2)->priority = 2;
      DAEDALUS_SPRITE(DAEDALUS_BODY + 3)->priority = 2;
      {
        s32 v;

        v = prevCamera + bobX;
        place.x = v + toFIXED(160);
        place.y = INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BODY], &place,
                                          &pieceScale, 0);
        place.x = v + toFIXED(128);
        place.y = INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BODY + 1],
                                          &place, &pieceScale, 0);
        place.x = v + toFIXED(160);
        place.y = INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BODY + 2],
                                          &place, &pieceScale, 0);
        place.x = v + toFIXED(192);
        place.y = INTtoFIXED(136 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BODY + 3],
                                          &place, &pieceScale, 0);
        DAEDALUS_SPRITE(DAEDALUS_BODY + 4)->priority = 2;
        place.x = v + toFIXED(128);
        place.y = INTtoFIXED(104 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_BODY + 4],
                                          &place, &pieceScale, 0);
      }
      for (i = 0; i != 5; i++) {
        place.x = INTtoFIXED(gDaedalus_RightLegPoints[i][0]) + swayX +
                  prevCamera + bobX + toFIXED(176);
        place.y = INTtoFIXED(gDaedalus_RightLegPoints[i][1] -
                             gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + bobY + toFIXED(84);
        DAEDALUS_SPRITE(DAEDALUS_RIGHT_LEG + i)->priority = 2;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_RIGHT_LEG + i],
                                          &place, &pieceScale, 0);
      }
      if (frame <= 489) {
        for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++) {
          struct Particle *p;

          p = &work->particles[DAEDALUS_MISSILE_MOTIONS + i];
          place.x = p->x + camera;
          place.y = p->y;
          if (p->variant != 0)
            Render_ApplyProjectedPlacementFar(
                work->sprites[DAEDALUS_MISSILE + i], &place, &pieceScale, 0);
        }
      }
      if (frame < 540) {
        struct Particle *p;

        p = &work->particles[DAEDALUS_HULL_MOTION];
        if (p->variant == 1) {
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 60 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
        }
        for (i = 0; i != 4; i++) {
          place.x = INTtoFIXED((i & 1) * DAEDALUS_PIECE_SIZE) + p->x + camera +
                    toFIXED(128);
          place.y = INTtoFIXED((i / 2) * DAEDALUS_PIECE_SIZE -
                               gDaedalus_ShakeOffsets[work->shakeFrames]) +
                    swayY + p->y + toFIXED(104);
          DAEDALUS_SPRITE(DAEDALUS_HULL + i)->priority = 1;
          Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_HULL + i],
                                            &place, &pieceScale, 0);
        }
      }
      if (frame <= 316) {
        struct Particle *p;

        p = &work->particles[DAEDALUS_COVER_MOTION];
        if (p->variant != 0) {
          p->variant++;
          p->x += p->velocityX;
          p->y += p->velocityY;
          p->velocityY += toFIXED(1) / 8;
          p->velocityX = p->velocityX * 48 / DAMPING_SCALE;
          p->velocityY = p->velocityY * 63 / DAMPING_SCALE;
          DAEDALUS_SPRITE(DAEDALUS_COVER)->rotation += TRIG_TURN / 256;
        }
        place.x = swayX + p->x + camera + toFIXED(184);
        place.y = INTtoFIXED(92 - gDaedalus_ShakeOffsets[work->shakeFrames]) +
                  swayY + p->y;
        DAEDALUS_SPRITE(DAEDALUS_COVER)->priority = 1;
        Render_ApplyProjectedPlacementFar(work->sprites[DAEDALUS_COVER], &place,
                                          &pieceScale, 0);
      }
      {
        s32 row;

        for (j = 0; j != CANOPY_ROWS; j++) {
          for (i = 0; i != DAEDALUS_CANOPY_ROW; i++) {
            struct DaedalusSprite *obj;
            s32 v;
            place.x = INTtoFIXED(i * CANOPY_PICTURE_SIZE) + canopyX;
            place.y = INTtoFIXED(j * CANOPY_PICTURE_SIZE -
                                 gDaedalus_ShakeOffsets[work->shakeFrames]) +
                      climb + toFIXED(CANOPY_Y);
            if (i == CANOPY_PICTURES) {
              DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile =
                  work->particles[j * DAEDALUS_CANOPY_ROW +
                                  (canopyPhase + CANOPY_WRAP + CANOPY_PICTURES -
                                   1) %
                                      CANOPY_PICTURES]
                      .variant &
                  OBJ_TILE_MASK;
            } else {
              row = j * DAEDALUS_CANOPY_ROW;
              k = row;
              k += i;
              DAEDALUS_SPRITE(k)->tile =
                  work->particles[row + (canopyPhase + CANOPY_WRAP + i) %
                                            CANOPY_PICTURES]
                      .variant &
                  OBJ_TILE_MASK;
            }
            if (place.x > toFIXED(CANOPY_PICTURES * CANOPY_PICTURE_SIZE) - 1)
              place.x -= toFIXED(DAEDALUS_CANOPY_ROW * CANOPY_PICTURE_SIZE);
            Render_ApplyProjectedPlacementFar(
                work->sprites[j * DAEDALUS_CANOPY_ROW + i], &place, &pieceScale,
                0);
          }
        }
      }
      for (j = 0; j != 4; j++) {
        for (i = 0; i != 6; i++) {
          if (frame >= i * 2 + DAEDALUS_SPLASH_START &&
              frame < i * 2 + DAEDALUS_SPLASH_START + ROCKET_FLAME_COUNT * 2) {
            s32 x, y, n;

            x = gDaedalus_SplashPoints[j][i][0];
            n = (frame - (i * 2 + DAEDALUS_SPLASH_START)) / 2;
            y = gDaedalus_SplashPoints[j][i][1] + (FIXEDtoINT(swayY)) -
                gDaedalus_ShakeOffsets[work->shakeFrames];
            ((BlitFunc)blit[0])(
                canvas, &work->sheet[n * PICTURE_BYTES(ROCKET_FLAME)],
                x - ROCKET_FLAME_WIDTH / 2, y - ROCKET_FLAME_HEIGHT / 2,
                ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
          }
        }
      }
      // Every particle of the runs young enough to be a flame is drawn and
      // moved here: the missiles' trails, and with them the launch flames,
      // which have no loop of their own, and the young exhaust and bursts,
      // which their own loops draw and move as well.
      for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++) {
        for (j = 0; j != DAEDALUS_TRAIL_LENGTH; j++) {
          struct Particle *q;

          q = &EXTRA_PARTICLES[i * DAEDALUS_TRAIL_LENGTH + j];
          if ((u32)q->variant < ROCKET_FLAME_COUNT) {
            ((BlitFunc)blit[0])(
                canvas, &work->sheet[q->variant * PICTURE_BYTES(ROCKET_FLAME)],
                (FIXEDtoINT(q->x)) - 2,
                (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
            q->x += q->velocityX;
            q->y += q->velocityY;
            q->velocityY -= toFIXED(1) / 4;
            q->variant++;
          }
        }
      }
      {
        s32 *shake;
        s32 v;
        struct BgScroll *bg;

        shake = &work->shakeFrames;
        bg = gBgScroll;
        v = gDaedalus_ShakeOffsets[*shake];
        bg[1].y = v + BATTLE_BACKGROUND_SCROLL_Y;
        gProjection.centerY = BATTLE_PROJECTION_CENTER_Y - v;
        if (*shake > 0)
          *shake -= 1;
      }
      EFFECT_END_FRAME(work);
      frame++;
      if (frame != DAEDALUS_SKY_FRAMES && !(gInput.held & KEY_B) &&
          !(gInput.repeat & KEY_A))
        goto skyFrame;
    }

    Task_Remove(BattleEffect_AdvanceScrollOnInterval);
    gBgScroll[1].x = savedScrollX;
    view->busy = 0;
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != DAEDALUS_MISSILE + 1 + DAEDALUS_MISSILE_COPIES; i++)
      Sprite_DestroyFar(work->sprites[i]);
    battle->arrowTiles = VramCache_AllocEntry(128);
    Palette_LoadBasicSpriteColors();
    Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
            DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
    BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    REG_BG2X = 0;
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BG2PA = Q_8_8(0.5);
    REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForListFar(args->targets, 0);
    // The fall: sixteen missiles dive onto the foes.
    BattleEffect_SpawnObjects(1, SPRITE_DAEDALUS_MISSILE, 1);
    for (i = 0; i != DAEDALUS_MISSILE_COPIES; i++) {
      work->sprites[1 + i] =
          EffectSprite_Acquire(32, 16, OBJ_SIZE_32X16 | OBJ_256_COLORS, 0);
      IWRAM_COPY_WORDS_LINKED(work->sprites[1 + i], work->sprites[0],
                              offsetof(struct Sprite, lastNo));
      DAEDALUS_SPRITE(1 + i)->offsetX = 32;
      DAEDALUS_SPRITE(1 + i)->offsetY = 8;
    }
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_ROCKET_FLAME_SHEET,
                               work->sheet + DAEDALUS_ROCKET_FLAMES, 0, 0);
    {
      const void *colors =
          (const void *)Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
      CopyFunc copy = IWRAM_COPY_WORDS_LINKED;

      copy((void *)BG_PLTT, colors, SHEET_PALETTE_BYTES);
    }

    for (frame = 0; frame != DAEDALUS_FALL_FRAMES; frame++) {
      // Each missile dives at its own angle, eight pixels a frame, from where
      // it reaches its point in 24 frames.
      if (frame == 0) {
        s32 z;
        struct Particle *p;

        i = 0;
        radius = 0;
        for (; i != DAEDALUS_MISSILE_COUNT; i++) {
          p = &work->particles[i];
          z = i * -(TRIG_TURN / 256);
          p->variant = radius;
          Random16();
          p->velocityZ = radius;
          /* FAKEMATCH(dead): k = 0 under a test that is never true, a store
           * nothing reads. */
          if (work == NULL || p == NULL)
            k = 0;
          p->z = z - (p->velocityZ * FALL_DIVE_SPREAD) - TRIG_TURN / 32;
          p->velocityX = -Trig_Cos(p->z) * 8;
          p->velocityY = -Trig_Sin(p->z) * 8;
          p->x = INTtoFIXED(Random16() % 80 + 80) - p->velocityX * 24;
          p->y = INTtoFIXED((Random16() & 7) + 96) - p->velocityY * 24;
          DAEDALUS_SPRITE(i)->rotation = p->z;
          DAEDALUS_SPRITE(i)->offsetX = radius;
          DAEDALUS_SPRITE(i)->offsetY = 8;
        }
        for (i = 0; i != DAEDALUS_PARTICLE_COUNT; i++)
          EXTRA_PARTICLES[i].variant = -1;
      }
      missileScale = gDaedalus_MissileScale;
      missilePlace.groundHeight = 0;
      missilePlace.height = toFIXED(255);
      for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++) {
        struct Particle *p;

        p = &work->particles[i];
        if (frame >= gDaedalus_DiveStarts[i] && p->variant == 0) {
          missilePlace.x = p->x;
          missilePlace.y = p->y;
          if (missilePlace.y > 0 && missilePlace.y < toFIXED(128))
            Render_ApplyProjectedPlacementFar(work->sprites[i], &missilePlace,
                                              &missileScale, 0);
          // A missile that reaches the ground bursts and strikes the foes;
          // one still diving leaves a trail of flames.
          if (p->y > toFIXED(112) - 1) {
            p->variant = 1;
            for (j = 0; j != 2; j++) {
              s32 speed;
              s32 angle;
              struct Particle *q;

              q = &EXTRA_PARTICLES[i * FALL_BURST_SLOTS + j];
              speed = (Random16() & 31) + 32;
              angle = (Random16() & 0x7fff) + DEGtoANG(90);
              q->x = p->x / 2;
              q->y = p->y;
              q->velocityX = Trig_Sin(angle) * speed >> 4;
              q->velocityY = Trig_Cos(angle) * speed >> 4;
              q->variant = 0;
            }
            Sound_PlayFar(SE_CLATTER);
            for (j = 0; j != args->targetCount; j++) {
              BattleEffect_StartActorMotion(args->targets[j], 1, 0, 0,
                                            toFIXED(2), 100);
              BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, j, 7);
              work->shakeFrames = 4;
            }
          } else {
            struct Particle *q;

            {
              s32 m = (frame - gDaedalus_DiveStarts[i]) % FALL_TRAIL_LENGTH;
              q = &EXTRA_PARTICLES[i * FALL_TRAIL_LENGTH + m + FALL_TRAILS];
            }
            q->x = p->x / 2;
            q->y = p->y - toFIXED(16);
            q->velocityX = 0;
            q->velocityY = 0;
            q->variant = 0;
          }
          DAEDALUS_SPRITE(i)->rotation = p->z;
          Particle_AdvanceWithGravity3D(p, PARTICLE_DAMPING_NONE, 0);
          p->velocityX = -Trig_Cos(DAEDALUS_SPRITE(i)->rotation) * 8;
          p->velocityY = -Trig_Sin(DAEDALUS_SPRITE(i)->rotation) * 8;
          DAEDALUS_SPRITE(i)->tile = DAEDALUS_SPRITE(0)->tile;
        }
      }
      // A trail's flame shows from its third frame.
      for (i = 0; i != DAEDALUS_MISSILE_COUNT * FALL_TRAIL_LENGTH; i++) {
        struct Particle *q;

        q = &EXTRA_PARTICLES[FALL_TRAILS + i];
        if ((u32)q->variant < ROCKET_FLAME_COUNT + 2) {
          s32 v;

          v = q->variant - 2;
          if (v >= 0)
            ((BlitFunc)blits[1])(canvas,
                                 &work->sheet[v * PICTURE_BYTES(ROCKET_FLAME) +
                                              DAEDALUS_ROCKET_FLAMES],
                                 (FIXEDtoINT(q->x)) - ROCKET_FLAME_WIDTH / 2,
                                 (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                                 ROCKET_FLAME_WIDTH, ROCKET_FLAME_HEIGHT);
          q->variant++;
        }
      }
      // The bursts, three frames a picture, every other one added.
      for (i = 0; i != DAEDALUS_MISSILE_COUNT * FALL_BURST_SLOTS; i++) {
        struct Particle *q;

        q = &EXTRA_PARTICLES[i];
        if ((u32)q->variant < FLASH_BURST_PICTURE_COUNT * 3) {
          s32 v;

          v = q->variant / 3;
          ((BlitFunc)blits[i & 1])(
              canvas, &work->sheet[v * PICTURE_BYTES(FLASH_BURST)],
              (FIXEDtoINT(q->x)) - FLASH_BURST_WIDTH / 2,
              (FIXEDtoINT(q->y)) - FLASH_BURST_HEIGHT * 3 / 4,
              FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(q, 56, -toFIXED(1) / 8);
          q->variant++;
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
    for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++)
      Sprite_DestroyFar(work->sprites[i]);
    EFFECT_FINISH_CANVAS();
  }
}

/// The rocket's scale in the second strike: twice its size.
extern const struct Scale gDaedalusStrike_RocketScale;

#define DAEDALUS_STRIKE_FRAMES 108
/// The rocket dives in over the strike's first DAEDALUS_STRIKE_DIVE_FRAMES.
#define DAEDALUS_STRIKE_DIVE_FRAMES 92
/// From DAEDALUS_STRIKE_GLOW_START a disc of light swells to
/// DAEDALUS_STRIKE_DISC_SIZE beneath it and its rays of light spin out: six
/// of the sixteen set up, each shown for DAEDALUS_STRIKE_RAY_LIFE steps, two a
/// frame.
#define DAEDALUS_STRIKE_GLOW_START 56
#define DAEDALUS_STRIKE_DISC_SIZE 128
#define DAEDALUS_STRIKE_RAYS_SET_UP 16
#define DAEDALUS_STRIKE_RAYS 6
#define DAEDALUS_STRIKE_RAY_LIFE 48
/// At DAEDALUS_STRIKE_SPARK_FRAME the canvas flashes, and a burst of
/// SPARKS_PER_BURST sparks scatters each of the next DAEDALUS_STRIKE_BURSTS
/// frames.
#define DAEDALUS_STRIKE_SPARK_FRAME 80
#define DAEDALUS_STRIKE_BURSTS 8
#define SPARKS_PER_BURST 8
#define DAEDALUS_STRIKE_SPARKS (DAEDALUS_STRIKE_BURSTS * SPARKS_PER_BURST)

/// The rocket's display fields that the strike sets: a struct Sprite from its
/// rotation on.
struct StrikeRocket {
  u8 imageScaleAndSlot[18]; ///< struct Sprite's fields before the rotation
  u16 rotation;
  u8 size[2];
  u8 offsetX;
  u8 offsetY;
};

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define DAEDALUS_STRIKE_GLOW_BAND 16384

/// Daedalus's second strike, at the end of the turn after the summon: its
/// rocket (SPRITE_DAEDALUS_ROCKET) dives from above the screen and settles
/// while, from DAEDALUS_STRIKE_GLOW_START, a disc of light swells beneath it
/// and rays of light spin out. At DAEDALUS_STRIKE_SPARK_FRAME the canvas
/// flashes and a burst of sparks scatters each frame of DAEDALUS_STRIKE_BURSTS,
/// and at the frames of gDaedalusStrike_HitFrames every target is knocked back
/// and flashes.
///
/// Abilities: Daedalus (its second strike).
void BattleEffect_RunDaedalusStrike(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct ModelTexture texture;
  struct SpritePlacement place;
  struct Scale rocketScale;
  s32 frame;
  s32 i;
  s32 j;
  FIXED centerX;
  FIXED centerY;
  s32 burst;
  struct Particle *spark;
  s32 mask;
  FIXED discX;
  FIXED rayX = toFIXED(66);
  FIXED discY = toFIXED(80);

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  BattleEffect_ConfigureEffectDisplay();
  REG_WIN0V = WIN_RANGE(16, 128);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_SpawnObjects(1, SPRITE_DAEDALUS_ROCKET, 3);
  BattleEffect_SelectLivingTargets(work->args);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;

  frame = 0;
  // The first burst's sparks, counted from the strike's start.
  burst = -DAEDALUS_STRIKE_SPARK_FRAME * SPARKS_PER_BURST;
  do {
    if (frame == 0) {
      /* FAKEMATCH(temporary): the disc's x held in a local set on the first
         frame; as toFIXED(64) at its use. */
      discX = toFIXED(64);
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
    }
    if (frame == DAEDALUS_STRIKE_GLOW_START) {
      Sound_PlayFar(SE_SPIN);
    }
    for (i = 0; i != 7; i++) {
      if (frame == gDaedalusStrike_HitFrames[i]) {
        for (j = 0; j != args->targetCount; j++) {
          BattleEffect_StartActorMotion(args->targets[j], 1, toFIXED(2),
                                        toFIXED(8), toFIXED(2), 120);
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 7);
        }
        work->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
      }
    }

    // The flash, and a burst of sparks a frame.
    if (frame >= DAEDALUS_STRIKE_SPARK_FRAME) {
      // One pointer to the sparks for the burst and the draw loop below.
      struct Particle *sparks;

      if (frame == DAEDALUS_STRIKE_SPARK_FRAME) {
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // step at a time, past the step's scalar members.
        s32 *life = &EXTRA_PARTICLES[0].variant;

        for (i = 0; i != DAEDALUS_STRIKE_SPARKS; i++) {
          *life = -1;
          life += sizeof(struct Particle) / sizeof(s32);
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      sparks = EXTRA_PARTICLES;
      if ((u32)(frame - DAEDALUS_STRIKE_SPARK_FRAME) < DAEDALUS_STRIKE_BURSTS) {
        centerX = INTtoFIXED((Random16() & 63) + 28);
        centerY = INTtoFIXED((Random16() & 31) + 80);

        j = 0;
        mask = 15;
        spark = sparks + burst;
        for (; j != SPARKS_PER_BURST; j++, spark++) {
          s32 speed;
          s32 angle;
          speed = 31;
          speed &= Random16();
          angle = Random16();

          spark->x = centerX + INTtoFIXED((Random16() & mask) - 8);
          spark->y = centerY + INTtoFIXED((Random16() & mask) - 8);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      {
        struct Particle *mote;

        for (i = 0, mote = sparks; i != DAEDALUS_STRIKE_SPARKS; i++, mote++) {
          if ((u32)mote->variant < FLASH_BURST_PICTURE_COUNT * 3) {
            draw[0](canvas,
                    work->sheet +
                        mote->variant / 3 * PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(mote->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(mote->y) - FLASH_BURST_HEIGHT * 3 / 4,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            Particle_AdvanceWithGravity2D(mote, 62, -toFIXED(1) / 32);
            mote->variant++;
          }
        }
      }
    }

    // The rocket dives two pixels a frame for 64 frames, then sinks half a
    // pixel a frame.
    if (frame < DAEDALUS_STRIKE_DIVE_FRAMES) {
      struct StrikeRocket *rocket;

      rocketScale = gDaedalusStrike_RocketScale;
      rocket = work->sprites[0];
      rocket->offsetY = 32;
      rocket->offsetX = 0;
      place.groundHeight = 0;
      rocket->rotation = TRIG_TURN * 17 / 32;
      place.height = toFIXED(255);
      if (frame <= 63) {
        place.x = INTtoFIXED(152 - frame / 4);
        place.y = INTtoFIXED(frame * 2) - toFIXED(64);
      } else if ((u32)(frame - 64) <= 63) {
        s32 sinkFrame = frame - 64;

        place.x = INTtoFIXED(136 - sinkFrame / 16);
        place.y = INTtoFIXED(sinkFrame / 2) + toFIXED(64);
      } else {
        place.x = toFIXED(136);
        place.y = toFIXED(64);
      }
      Render_ApplyProjectedPlacementFar(work->sprites[0], &place,
                                        &rocketScale.x, 0);
    }

    // The disc and the rays, until the rocket settles.
    if ((u32)(frame - DAEDALUS_STRIKE_GLOW_START) <
        DAEDALUS_STRIKE_DIVE_FRAMES - DAEDALUS_STRIKE_GLOW_START) {
      s32 size;
      s32 grow = frame * 2 - 104;
      struct RenderModel *model;
      void *vertices;
      struct Particle *ray;
      u8 *cells;

      /* FAKEMATCH(temporary): the disc's x copied into the loop counter i. */
      i = discX;
      size = DAEDALUS_STRIKE_DISC_SIZE;
      if (grow <= DAEDALUS_STRIKE_DISC_SIZE / 2) {
        size = grow * 2;
        cells = gMapGridCellBuffer;
        BattleEffect_DrawShadedCircles(cells, size, FIXED_ONE);
      }
      cells = gMapGridCellBuffer;
      Canvas_BlitAtFoot(cells, FIXEDtoINT(i), FIXEDtoINT(discY), size);
      // Each ray at a random angle, turning a random way, a little after
      // the one before.
      if (frame == DAEDALUS_STRIKE_GLOW_START) {
        s32 delay;

        i = 0;
        delay = 0;
        for (ray = work->particles; i != DAEDALUS_STRIKE_RAYS_SET_UP;
             i++, ray++) {
          ray->x = Random16() & TRIG_ANGLE_MASK;
          ray->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ray->y = -ray->y;
          }
          ray->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                                   work->sheet + DAEDALUS_STRIKE_GLOW_BAND, 0,
                                   0);
      }
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + DAEDALUS_STRIKE_GLOW_BAND;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_SmallSquareMesh;

      // A ray swells and narrows again over its life.
      for (i = 0; i != DAEDALUS_STRIKE_RAYS; i++) {
        ray = &work->particles[i];
        ray->variant += 2;
        ray->x += ray->y * 2;
        if ((u32)ray->variant < DAEDALUS_STRIKE_RAY_LIFE) {
          s32 k;

          Matrix_Initialize();
          k = i & 3;
          Matrix_Translate(INTtoFIXED(k * 2) + rayX - toFIXED(64),
                           discY - INTtoFIXED(k + 2) - toFIXED(64), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
          Matrix_RotateZ(ray->x);
          Matrix_Scale(Trig_Sin(ray->variant *
                                (TRIG_TURN / 2 / DAEDALUS_STRIKE_RAY_LIFE)) +
                           FIXED_ONE,
                       FIXED_ONE * 2, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
    burst += SPARKS_PER_BURST;
    frame++;
  } while (frame != DAEDALUS_STRIKE_FRAMES);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}
#undef MIRRORED_WALL_FRAMES
#undef MIRRORED_WALL_DRAW_FRAMES
#undef CEL_SIZE
#undef PICTURE_OFFSET
#undef EARTH_WALL_LOW
#undef EARTH_WALL_LOW_WIDTH
#undef EARTH_WALL_LOW_HEIGHT
#undef EARTH_WALL_MIDDLE
#undef EARTH_WALL_MIDDLE_WIDTH
#undef EARTH_WALL_MIDDLE_HEIGHT
#undef EARTH_WALL_HIGH
#undef EARTH_WALL_HIGH_WIDTH
#undef EARTH_WALL_HIGH_HEIGHT
#undef MIRROR_HALF
#undef IWRAM_COPY_WORDS_LINKED
#undef DECODE_BUFFER
#undef DAEDALUS_STRIKE_FRAMES
#undef DAEDALUS_STRIKE_DIVE_FRAMES
#undef DAEDALUS_STRIKE_GLOW_START
#undef DAEDALUS_STRIKE_DISC_SIZE
#undef DAEDALUS_STRIKE_RAYS_SET_UP
#undef DAEDALUS_STRIKE_RAYS
#undef DAEDALUS_STRIKE_RAY_LIFE
#undef DAEDALUS_STRIKE_SPARK_FRAME
#undef DAEDALUS_STRIKE_BURSTS
#undef SPARKS_PER_BURST
#undef DAEDALUS_STRIKE_SPARKS
#undef BIRD_HEIGHT
#undef BIRD_WIDTH
#undef BIRD_WING_BEAT
#undef CANOPY_PALETTE
#undef CANOPY_PICTURES
#undef CANOPY_PICTURE_SIZE
#undef CANOPY_ROWS
#undef CANOPY_ROW_BYTES
#undef CANOPY_WRAP
#undef CANOPY_Y
#undef CORE_DARK
#undef CORE_EMBER_RED
#undef CORE_GLOW_BLUE
#undef CORE_GLOW_GREEN
#undef FALL_BURST_SLOTS
#undef FALL_TRAILS
#undef FALL_DIVE_SPREAD
#undef FALL_TRAIL_LENGTH
#undef FLAME_RING_COUNT
#undef FLAME_RING_FRAMES
#undef FLAME_RING_STEP
#undef FLAME_RING_Y
#undef ROCKET_FLAME_COUNT
#undef ROCKET_FLAME_HEIGHT
#undef ROCKET_FLAME_WIDTH
#undef ROCKET_GRAVITY
#undef ROCKET_LEAD_START
#undef ROCKET_NOZZLE
#undef ROCKET_SCALE
#undef ROCKET_THRUST_START
#undef ROCKET_LIFT_START
#undef ROCKET_NOSE_UP_START
#undef ROCKET_NOSE_UP_END
#undef ROCKET_LIFT_END
#undef ROCKET_SINK_FRAMES
#undef ROCKET_TURN_START
#undef ROCKET_BURSTS_START
#undef ROCKET_DIP_START
#undef ROCKET_DIP_END
#undef ROCKET_TURN_END
#undef ROCKET_SOAR_START
#undef ROCKET_BURSTS_END
