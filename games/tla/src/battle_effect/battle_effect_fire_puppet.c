/// @file
/// Fire Puppet and Whiplash.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define FIRE_PUPPET_FRAMES 103
/// The ghost's trail is PUPPET_SEGMENTS pairs of corners, laid in the map cell
/// buffer: the corners, those Render_TransformVertices makes of them at
/// PUPPET_STRIP_VERTICES, and the trail's mesh at PUPPET_STRIP_MESH, whose
/// quads each take 4 texels of StreakTexture across and its 32 down.
#define PUPPET_SEGMENTS 33
#define PUPPET_CORNERS (PUPPET_SEGMENTS * 2)
#define PUPPET_QUADS (PUPPET_SEGMENTS - 1)
#define PUPPET_STRIP_VERTICES (Ram_MapCellBuffer + 4096)
#define PUPPET_STRIP_MESH (Ram_MapCellBuffer + 8192)
/// The last particle keeps a copy of the target's place and scale.
#define PUPPET_TARGET_COPY (state->particles[ARRAY_COUNT(state->particles) - 1])

/// The ghost rises at PUPPET_RISE_FRAME, its trail curling a further
/// PUPPET_CURL_SPEED a segment each frame up to PUPPET_CURL_MAX; it strikes at
/// PUPPET_STRIKE_FRAME, and from PUPPET_RETURN_FRAME it uncurls and shrinks
/// back into the caster over PUPPET_RETURN_FRAMES.
#define PUPPET_RISE_FRAME 16
#define PUPPET_CURL_SPEED 44
#define PUPPET_CURL_MAX 1424
#define PUPPET_STRIKE_FRAME 54
#define PUPPET_RETURN_FRAME 74
#define PUPPET_RETURN_FRAMES 32
/// The caster kneels, grey, until this frame.
#define PUPPET_CASTER_RISE_FRAME 100
/// The skull grows from half size by this much a step of the curl, to about
/// full size at PUPPET_CURL_MAX.
#define PUPPET_SKULL_GROWTH 23

/// The trail is PUPPET_WIDTH wide, pinched by up to PUPPET_PINCH along a sine
/// that runs PUPPET_PINCH_STEP a segment, starting five segments before the
/// caster: 33 pixels at the caster, 6 at its narrowest, near the middle, and 47
/// at the skull.
#define PUPPET_WIDTH 50
#define PUPPET_PINCH 44
#define PUPPET_PINCH_STEP 862
#define PUPPET_PINCH_START (5 * PUPPET_PINCH_STEP)
/// The trail sways up to PUPPET_SWAY pixels at the skull, less towards the
/// caster, a wave that runs along it a segment a frame.
#define PUPPET_SWAY 16
/// The skull's turn at the trail's end, against the trail's own turn there.
#define PUPPET_SKULL_TURN 22384

/// Where GhostSkullTexture is decoded in the effect's pictures.
#define FIRE_PUPPET_GHOST_SKULL 16384

/// The caster kneels, grey, and its ghost rises from it: a trail of blue
/// streaks that curls up and over the battle with a skull at its head. The
/// skull dives on the targets, the screen flashes white and every target is
/// thrown back; then the ghost shrinks back into the caster, who stands again.
///
/// Abilities: Fire Puppet.
void BattleEffect_RunFirePuppet(struct BattleEffectParams *args) {
  u8 *mesh;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  u8 *motes;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct ScreenPoint caster;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 endX;
  s32 endY;
  s32 i;

  mesh = PUPPET_STRIP_MESH;
  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  motes = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  // The skull's place, set at the trail's last segment.
  endX = 0;
  endY = 0;
  // DEAD: nothing reads the copy; the ROM keeps the stores.
  PUPPET_TARGET_COPY.x = target->x;
  PUPPET_TARGET_COPY.y = target->y;
  PUPPET_TARGET_COPY.z = target->z;
  PUPPET_TARGET_COPY.velocityX = target->scaleX;
  PUPPET_TARGET_COPY.velocityY = target->scaleY;
  Resource_LoadAndDecompress(RESOURCE_STREAK_TEXTURE, state->sheet, 0, 0);
  // DEAD: Fire Puppet draws none of these motes; the ROM keeps the load.
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GHOST_SKULL_TEXTURE,
                             state->sheet + FIRE_PUPPET_GHOST_SKULL, 0, 0);
  // The ghost takes the Mercury Djinn's blues.
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SHEET),
                       SHEET_PALETTE_BYTES);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(mesh, 4, 32, PUPPET_QUADS);

  for (frame = 0; frame != FIRE_PUPPET_FRAMES; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 0) {
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_GREY, POSE_PARTY_HELD,
                               0, -1);
    }
    if (frame == 8) {
      Sound_PlayFar(SE_CURSE);
    }
    if (frame == PUPPET_CASTER_RISE_FRAME) {
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, POSE_ENEMY_HELD,
                               0, -1);
    }
    if (frame == PUPPET_STRIKE_FRAME) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      Sound_PlayFar(SE_POWER_UP);
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      state->shakeFrames = 8;
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(2),
                                      toFIXED(4), toFIXED(4), 150);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
      }
    }
    if (frame >= PUPPET_RISE_FRAME) {
      s32 curl;
      s32 j;
      struct ModelVertex(*corners)[2] =
          (struct ModelVertex(*)[2])Ram_MapCellBuffer;
      u8 *projected = PUPPET_STRIP_VERTICES;
      s32 shade;

      curl = (frame - PUPPET_RISE_FRAME) * PUPPET_CURL_SPEED;
      if (curl > PUPPET_CURL_MAX) {
        curl = PUPPET_CURL_MAX;
      }
      if (frame >= PUPPET_RETURN_FRAME) {
        curl =
            PUPPET_CURL_MAX - (frame - PUPPET_RETURN_FRAME) * PUPPET_CURL_SPEED;
      }

      // The trail curls about a point beside its root: segment j lies j * curl
      // round it, its two corners the trail's width apart.
      for (j = 0; j != PUPPET_SEGMENTS; j++) {
        s32 width =
            PUPPET_WIDTH -
            (FIXEDtoINT(Trig_Sin(j * PUPPET_PINCH_STEP + PUPPET_PINCH_START) *
                        PUPPET_PINCH));
        s32 sway = (FIXEDtoINT(Trig_Sin((frame + j) * (TRIG_TURN / 32)) *
                               PUPPET_SWAY)) *
                   j / (PUPPET_SEGMENTS - 1);
        s32 outer;

        i = 0;
        outer = width + sway;
        for (; i != 2; i++) {
          corners[j][i].x =
              (FIXEDtoINT(Trig_Cos(j * curl) * (i * width + sway + 16))) -
              outer / 2 - 16;
          corners[j][i].y =
              -(FIXEDtoINT(Trig_Sin(j * curl) * (i * width + j + sway + 32)));
          corners[j][i].z = 0;
        }
        // The skull's place, the middle of the last segment.
        if (j == PUPPET_SEGMENTS - 1) {
          endX = (FIXEDtoINT(Trig_Cos(curl * (PUPPET_SEGMENTS - 1)) *
                             (width / 2 + sway + 16))) -
                 (width + sway) / 2 - 16;
          endY = -(FIXEDtoINT(Trig_Sin(curl * (PUPPET_SEGMENTS - 1)) *
                              (width / 2 + j + sway + 32)));
        }
      }

      // The trail, its streaks running along it 4 texels a frame.
      model = Render_AllocateModelList(1);
      shade = 0;
      texture.logWidth = 7;
      texture.logHeight = 5;
      texture.pixels = state->sheet;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = mesh;
      model->vertices = projected;
      model->shade = shade;
      model->texture = &texture;
      EffectPosition_ProjectScaledBattler(args->user, &caster);
      model->scrollV = 0;
      model->scrollU = TEXTURE_SCROLL_MASK & ~(frame << 2);
      Matrix_Initialize();
      Matrix_Translate(INTtoFIXED(caster.x / 2 - CANVAS_SIZE / 2),
                       INTtoFIXED(caster.y) - toFIXED(42), 0);
      Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
      if (args->side == EFFECT_SIDE_PARTY) {
        Matrix_RotateY(DEGtoANG(180));
      }
      Matrix_RotateZ(DEGtoANG(90));
      if (frame >= PUPPET_RETURN_FRAME) {
        Matrix_ScaleUniform(FIXED_ONE - (frame - PUPPET_RETURN_FRAME) *
                                            (FIXED_ONE / PUPPET_RETURN_FRAMES));
      }
      Render_TransformVertices(Ram_MapCellBuffer, PUPPET_STRIP_VERTICES,
                               PUPPET_CORNERS);
      Render_DrawModelList(model);

      // The skull at the trail's end, swelling from half its size as the
      // trail curls out.
      model->mesh = gBattleEffect_SquareMesh;
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->vertices = projected;
      model->scrollU = 0;
      model->scrollV = 0;
      texture.pixels = state->sheet + FIRE_PUPPET_GHOST_SKULL;
      model->shade = 0;
      Matrix_Translate(INTtoFIXED(endX), INTtoFIXED(endY), 0);
      Matrix_RotateZ(PUPPET_SKULL_TURN - curl * (PUPPET_SEGMENTS - 1));
      Matrix_ScaleUniform(curl * PUPPET_SKULL_GROWTH + FIXED_ONE / 2);
      Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    EFFECT_PRESENT_CANVAS(state);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#define WHIPLASH_FRAMES 52
/// The frame the lash strikes the target.
#define WHIPLASH_HIT_FRAME 18
/// SlashFlameSheet: the violet lash in five pictures, shown one after another
/// for WHIPLASH_LASH_PICTURE_FRAMES apiece.
#define WHIPLASH_LASH_A 0
#define WHIPLASH_LASH_A_WIDTH 48
#define WHIPLASH_LASH_A_HEIGHT 96
#define WHIPLASH_LASH_B (WHIPLASH_LASH_A + PICTURE_BYTES(WHIPLASH_LASH_A))
#define WHIPLASH_LASH_B_WIDTH 38
#define WHIPLASH_LASH_B_HEIGHT 92
#define WHIPLASH_LASH_C (WHIPLASH_LASH_B + PICTURE_BYTES(WHIPLASH_LASH_B))
#define WHIPLASH_LASH_C_WIDTH 30
#define WHIPLASH_LASH_C_HEIGHT 76
#define WHIPLASH_LASH_D (WHIPLASH_LASH_C + PICTURE_BYTES(WHIPLASH_LASH_C))
#define WHIPLASH_LASH_D_WIDTH 56
#define WHIPLASH_LASH_D_HEIGHT 96
#define WHIPLASH_LASH_E (WHIPLASH_LASH_D + PICTURE_BYTES(WHIPLASH_LASH_D))
#define WHIPLASH_LASH_E_WIDTH 56
#define WHIPLASH_LASH_E_HEIGHT 88
#define WHIPLASH_LASH_PICTURE_FRAMES 6
/// The motes the lash's crack sprays, kept as particles in the map cell
/// buffer.
#define WHIPLASH_MOTES 256

/// A lash of violet light swings out from the caster and cracks down on the
/// target, which is thrown up and flashes, and motes spray from where the lash
/// struck, half of them rising and half falling as they burn down.
///
/// Abilities: Whiplash.
void BattleEffect_RunWhiplash(struct BattleEffectParams *args) {
  struct HeapSlots *heap;
  struct BattleEffectState *state;
  u8 *canvas;
  struct ScreenPoint caster;
  s32 frame;
  struct Particle *mote;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  u8 *motes;
  s32 i;

  heap = Ram_HeapSlots;
  state = (struct BattleEffectState *)heap->battleEffect;
  canvas = heap->battleCanvas;
  motes = heap->battleSheet;

  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    draw[0] = (DrawRectangle)heap->ramCode;
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    draw[0] = (DrawRectangle)heap->ramCode;
  }
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_SLASH_FLAME_SHEET, state->sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_F),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != WHIPLASH_FRAMES; frame++) {
    // The lash, its pictures placed by the caster.
    EffectPosition_ProjectBattler(args->user, &caster);
    if (args->side == EFFECT_SIDE_ENEMIES) {
      switch (frame / WHIPLASH_LASH_PICTURE_FRAMES) {
      case 0:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_A,
                            caster.x / 2 - 48, caster.y - 120);
        break;
      case 1:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_B,
                            caster.x / 2 - 38, caster.y - 120);
        break;
      case 2:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_C,
                            caster.x / 2 - 38, caster.y - 100);
        break;
      case 3:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_D,
                            caster.x / 2 - 64, caster.y - 88);
        break;
      case 4:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_E,
                            caster.x / 2 - 64, caster.y - 88);
        break;
      }
    } else {
      switch (frame / WHIPLASH_LASH_PICTURE_FRAMES) {
      case 0:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_A,
                            caster.x / 2, caster.y - 120);
        break;
      case 1:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_B,
                            caster.x / 2, caster.y - 120);
        break;
      case 2:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_C,
                            caster.x / 2, caster.y - 100);
        break;
      case 3:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_D,
                            caster.x / 2, caster.y - 88);
        break;
      case 4:
        EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet, WHIPLASH_LASH_E,
                            caster.x / 2, caster.y - 88);
        break;
      }
    }

    // The hit, and the motes sprayed from 48 pixels out, towards the target.
    if (frame == WHIPLASH_HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      state->shakeFrames = 8;
      for (i = 0; i != WHIPLASH_MOTES; i++) {
        s32 angle;
        s32 speed;

        mote = &((struct Particle *)Ram_MapCellBuffer)[i];
        if (args->side == EFFECT_SIDE_ENEMIES)
          mote->x = INTtoFIXED(caster.x / 2 - 48);
        else
          mote->x = INTtoFIXED(caster.x / 2 + 48);
        mote->y = INTtoFIXED(caster.y - (Random16() & 7));
        angle = (Random16() & 0x7fff) + DEGtoANG(90);
        speed = Random16() & 255;
        if (args->side == EFFECT_SIDE_ENEMIES)
          mote->velocityX = ((Trig_Sin(angle) * speed) >> 4) - toFIXED(8);
        else
          mote->velocityX = ((Trig_Sin(angle) * speed) >> 4) + toFIXED(8);
        mote->velocityY = (Trig_Cos(angle) * (speed + 64)) >> 5;
        mote->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= WHIPLASH_HIT_FRAME) {
      for (i = 0; i != WHIPLASH_MOTES; i++) {
        mote = &((struct Particle *)Ram_MapCellBuffer)[i];
        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, motes, FIXED_INTEGER_PART(mote->x),
                           FIXED_INTEGER_PART(mote->y), size);
          if (i & 1)
            Particle_AdvanceWithGravity2D(mote, 60, -toFIXED(1) / 8);
          else
            Particle_AdvanceWithGravity2D(mote, 60, toFIXED(1) / 16);
          mote->variant--;
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef FIRE_PUPPET_FRAMES
#undef PUPPET_SEGMENTS
#undef PUPPET_CORNERS
#undef PUPPET_QUADS
#undef PUPPET_STRIP_VERTICES
#undef PUPPET_STRIP_MESH
#undef PUPPET_TARGET_COPY
#undef PUPPET_RISE_FRAME
#undef PUPPET_CURL_SPEED
#undef PUPPET_CURL_MAX
#undef PUPPET_STRIKE_FRAME
#undef PUPPET_RETURN_FRAME
#undef PUPPET_RETURN_FRAMES
#undef PUPPET_CASTER_RISE_FRAME
#undef PUPPET_SKULL_GROWTH
#undef PUPPET_WIDTH
#undef PUPPET_PINCH
#undef PUPPET_PINCH_STEP
#undef PUPPET_PINCH_START
#undef PUPPET_SWAY
#undef PUPPET_SKULL_TURN
#undef FIRE_PUPPET_GHOST_SKULL
#undef WHIPLASH_FRAMES
#undef WHIPLASH_HIT_FRAME
#undef WHIPLASH_LASH_A
#undef WHIPLASH_LASH_A_WIDTH
#undef WHIPLASH_LASH_A_HEIGHT
#undef WHIPLASH_LASH_B
#undef WHIPLASH_LASH_B_WIDTH
#undef WHIPLASH_LASH_B_HEIGHT
#undef WHIPLASH_LASH_C
#undef WHIPLASH_LASH_C_WIDTH
#undef WHIPLASH_LASH_C_HEIGHT
#undef WHIPLASH_LASH_D
#undef WHIPLASH_LASH_D_WIDTH
#undef WHIPLASH_LASH_D_HEIGHT
#undef WHIPLASH_LASH_E
#undef WHIPLASH_LASH_E_WIDTH
#undef WHIPLASH_LASH_E_HEIGHT
#undef WHIPLASH_LASH_PICTURE_FRAMES
#undef WHIPLASH_MOTES
