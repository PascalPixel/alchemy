/// @file
/// A debug sprite viewer.
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"

#include "system/input.h"
#include "graphics/render_placement.h"

enum {
  /// The sprite copies and their ring positions; a facing kind places 4, 6, 8
  /// or 10 of them.
  UI_SPRITE_VIEWER_COPY_COUNT = 10,
  UI_SPRITE_VIEWER_SPRITE_LIMIT = 716,    ///< sprite ids 0 to 715 are offered
  UI_SPRITE_VIEWER_ANIMATION_LIMIT = 100, ///< animations 0 to 99
  /// A sprite animation priority above 3, which Sprite_ComposePicture skips.
  UI_SPRITE_VIEWER_HIDDEN_PRIORITY = 8,
  /// The sprite ids L and R step by while Start is held: any step past one
  /// goes to the next stop of gSpriteViewer_SkipStops.
  UI_SPRITE_VIEWER_FAST_STEP = 10,
  /// gSpriteViewer_SkipStops' entries, its end marker the last.
  UI_SPRITE_VIEWER_SKIP_STOPS = 10,
  /// The colour SPRITE_COLOR_FILL paints in the viewer (gDecodeFillByte):
  /// colour 3 of the basic sprite palette, orange.
  UI_SPRITE_VIEWER_FILL_COLOR = 3,
  /// gDebugMode while the viewer runs. Every reader tests it only for
  /// nonzero, so 2 turns on the same debug keys any other value would.
  UI_SPRITE_VIEWER_DEBUG_MODE = 2
};

/// An angle's bits: shifted up and back down, a value keeps only them.
#define ANGLE_BITS 16

/// What left and right choose for L and R to change on the selected sprite
/// animation.
enum UiSpriteViewerEditMode {
  UI_SPRITE_VIEWER_EDIT_ANIMATION,
  UI_SPRITE_VIEWER_EDIT_SPRITE,
  UI_SPRITE_VIEWER_EDIT_PRIORITY,
  UI_SPRITE_VIEWER_EDIT_COLOR,
  UI_SPRITE_VIEWER_EDIT_COUNT
};

/// One sprite animation the sprite viewer edits.
struct UiSpriteViewerLayer {
  s16 spriteId;       ///< metadata record selected for this layer
  s16 animationIndex; ///< animation script selected from that metadata
  u8 enabled;         ///< zero hides it by a priority above the drawing range
  s8 priority;        ///< drawing order, 0 to 3
  s8 colorMode;       ///< SpriteColorMode; the viewer cycles through 0 to 15
};

/// The debug sprite viewer, in The Broken Seal's form with this game's
/// changes; it never returns.
void SpriteViewer_Run(void) {
  struct UiSpriteViewerLayer layers[ANIM_LAYER_COUNT];
  volatile u32 zero;
  s32 x;
  s32 y;
  u32 editMode;
  s32 shadowsEnabled;
  u32 backdrop;
  void *ring;
  u32 selectedLayer;
  u32 i;
  s32 spriteId;
  s32 changed;

  x = 144;
  y = 96;
  editMode = UI_SPRITE_VIEWER_EDIT_SPRITE;
  selectedLayer = 0;
  shadowsEnabled = 1;
  backdrop = 0;
  ring = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, 160);
  gDecodeFillByte = UI_SPRITE_VIEWER_FILL_COLOR;
  Dma_Fill32Cell(zero, 0, ring, 4);
  Dma_Fill32Cell(zero, 0, layers, sizeof(layers));
  spriteId = SpriteViewer_StepSpriteId(-1, 1);
  for (i = 0; i < ANIM_LAYER_COUNT; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = UI_SPRITE_VIEWER_DEBUG_MODE;
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  *(volatile u16 *)BG_PLTT = RGB(0, 0, 31);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
  ScreenFade_FromBlack(1);

restart:
  Heap_Initialize();
  Task_ResetAll();
  ring = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE, 160);
  VramCache_Reset();
  ActorSystem_Initialize(ACTOR_SYSTEM_MAP);
  SpritePictureCache_Load(0, gMapGridCellBuffer, layers[0].spriteId, 0);
  if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind ==
      ANIM_FACING_TWO_PART)
    SpritePictureCache_Load(1, gActorSpriteSlots, layers[0].spriteId + 1, 0);
  for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
    struct Sprite *sprite;
    s32 part = 0;

    if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind ==
            ANIM_FACING_TWO_PART &&
        (i & 1))
      part = 1;
    sprite = Sprite_Create(SPRITE_PICTURE_TAG(part, layers[0].spriteId + part));
    Sprite_AddAnimation(sprite, layers[1].spriteId);
    Sprite_AddAnimation(sprite, layers[2].spriteId);
    Sprite_AddAnimation(sprite, layers[3].spriteId);
    sprite->flags = shadowsEnabled;
  }
  for (i = 0; i < ANIM_LAYER_COUNT; i++) {
    if (layers[i].enabled != 0)
      SpriteViewer_SetLayerPriority(i, layers[i].priority);
    else
      SpriteViewer_SetLayerPriority(i, UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
    SpriteViewer_SetLayerColorMode(i, layers[i].colorMode);
    {
      s32 a;

      /* FAKEMATCH(dead): the index read in a once-only block; as the plain call
       * with layers[i].animationIndex. */
      do {
        a = layers[i].animationIndex;
      } while (0);
      SpriteViewer_StartStaggeredAnimations(i, a);
    }
  }
  SpriteViewer_LayOutRing(x, y, ring);
  Task_Add(SpriteViewer_PlaceRing, TASK_PASS_FRAME);

next:
  wait(1);
  for (;;) {
    if (gInput.held & KEY_START) {
      if (gInput.held & KEY_LEFT)
        x--;
      if (gInput.held & KEY_RIGHT)
        x++;
      if (gInput.held & KEY_UP)
        y--;
      if (gInput.held & KEY_DOWN)
        y++;
      SpriteViewer_LayOutRing(x, y, ring);
    } else {
      // DEAD: a read of the repeated keys nothing uses; the ROM keeps the load.
      (void)gInput.repeat;
      if (gInput.repeat & KEY_UP)
        selectedLayer = (selectedLayer - 1) & (ANIM_LAYER_COUNT - 1);
      if (gInput.repeat & KEY_DOWN)
        selectedLayer = (selectedLayer + 1) & (ANIM_LAYER_COUNT - 1);
      if (gInput.repeat & KEY_LEFT)
        editMode = (editMode - 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
      if (gInput.repeat & KEY_RIGHT)
        editMode = (editMode + 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
    }

    if (gInput.pressed & KEY_START) {
      struct Sprite *sprites = Ram_HeapSlots->sprites;

      shadowsEnabled ^= 1;
      for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    case UI_SPRITE_VIEWER_EDIT_ANIMATION:
      // The second sprite animation follows the first's animation.
      if (selectedLayer == 1)
        break;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].animationIndex--;
        if (layers[selectedLayer].animationIndex < 0)
          layers[selectedLayer].animationIndex = 0;
        if (selectedLayer == 0)
          layers[1].animationIndex = layers[0].animationIndex;
        goto next;
      }
      if (!(gInput.repeat & KEY_R))
        break;
      layers[selectedLayer].animationIndex++;
      if (layers[selectedLayer].animationIndex >
          UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1)
        layers[selectedLayer].animationIndex =
            UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1;
      if (selectedLayer == 0)
        layers[1].animationIndex = layers[0].animationIndex;
      goto next;

    case UI_SPRITE_VIEWER_EDIT_SPRITE: {
      s32 steps;

      changed = 0;
      steps = 1;
      if (gInput.held & KEY_START)
        steps = UI_SPRITE_VIEWER_FAST_STEP;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].spriteId =
            SpriteViewer_StepSpriteId(layers[selectedLayer].spriteId, -steps);
        changed = 1;
      }
      if (gInput.repeat & KEY_R) {
        layers[selectedLayer].spriteId =
            SpriteViewer_StepSpriteId(layers[selectedLayer].spriteId, steps);
        changed = 1;
      }
      if (changed == 0)
        break;
      if (selectedLayer == 0)
        goto restart;
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetLayerSprite(selectedLayer,
                                    layers[selectedLayer].spriteId);
        SpriteViewer_SetLayerPriority(selectedLayer,
                                      layers[selectedLayer].priority);
        SpriteViewer_SetLayerColorMode(selectedLayer,
                                       layers[selectedLayer].colorMode);
        SpriteViewer_StartStaggeredAnimations(
            selectedLayer, layers[selectedLayer].animationIndex);
      }
      goto next;
    }

    case UI_SPRITE_VIEWER_EDIT_PRIORITY:
      changed = 0;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0)
          layers[selectedLayer].priority = 3;
        changed = 1;
      }
      if (gInput.repeat & KEY_R) {
        layers[selectedLayer].priority++;
        if (layers[selectedLayer].priority > 3)
          layers[selectedLayer].priority = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetLayerPriority(selectedLayer,
                                      layers[selectedLayer].priority);
      goto next;

    case UI_SPRITE_VIEWER_EDIT_COLOR:
      changed = 0;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0)
          layers[selectedLayer].colorMode = SPRITE_COLOR_HIDDEN;
        changed = 1;
      }
      if (gInput.repeat & KEY_R) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > SPRITE_COLOR_HIDDEN)
          layers[selectedLayer].colorMode = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetLayerColorMode(selectedLayer,
                                       layers[selectedLayer].colorMode);
      goto next;
    }

    if (gInput.repeat & KEY_A) {
      if (editMode == UI_SPRITE_VIEWER_EDIT_ANIMATION) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].enabled != 0) {
            SpriteViewer_StartStaggeredAnimations(
                selectedLayer, layers[selectedLayer].animationIndex);
            if (selectedLayer == 0)
              SpriteViewer_StartStaggeredAnimations(1,
                                                    layers[1].animationIndex);
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].enabled ^= 1;
        SpriteViewer_SetLayerPriority(selectedLayer,
                                      layers[selectedLayer].enabled != 0
                                          ? layers[selectedLayer].priority
                                          : UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
        goto next;
      }
    }
    if (gInput.repeat & KEY_SELECT) {
      Task_ResetAll();
      Stub_ReturnZero(RESOURCE_SPRITE_METADATA, gSprite_AnimationMetadata);
      goto restart;
    }
    if (gInput.pressed & KEY_B) {
      volatile u16 *palette = (volatile u16 *)BG_PLTT;
      const u16 *colors = gSpriteViewer_BackdropColors;
      backdrop++;
      backdrop &= 7;
      // B steps the backdrop through its eight colours.
      *palette = *(const u16 *)((u32)colors + (backdrop << 1));
    }
    wait(1);
  }
}

extern const u16 gSpriteViewer_SkipStops[];

/// Steps a sprite id (0 to 715) by delta: a step of one wraps around the
/// range, a larger step forward goes to the next stop above value and one back
/// to the last stop below it (gSpriteViewer_SkipStops, ended by (u16)-1, from
/// which a step forward wraps to 0). A delta of 0 returns value unchanged.
s32 SpriteViewer_StepSpriteId(s32 value, s32 delta) {
  s32 step;

  if (delta != 0) {
    step = delta;
    if (delta < 0)
      step = -delta;
    if (step == 1) {
      value += delta;
      if (value < 0)
        value = UI_SPRITE_VIEWER_SPRITE_LIMIT - 1;
      if (value >= UI_SPRITE_VIEWER_SPRITE_LIMIT)
        value = 0;
    } else if (delta > 0) {
      u32 i;

      for (i = 0; i < UI_SPRITE_VIEWER_SKIP_STOPS; i++) {
        if (value < gSpriteViewer_SkipStops[i]) {
          value = gSpriteViewer_SkipStops[i];
          if (value == (u16)-1)
            value = 0;
          break;
        }
      }
    } else if (value == 0) {
      value = gSpriteViewer_SkipStops[UI_SPRITE_VIEWER_SKIP_STOPS - 2];
    } else {
      s32 i;

      for (i = UI_SPRITE_VIEWER_SKIP_STOPS - 2; i >= 0; i--) {
        if (value > gSpriteViewer_SkipStops[i]) {
          value = gSpriteViewer_SkipStops[i];
          break;
        }
      }
    }
    return value;
  }
#ifdef UBFIX
  return value;
#else
  // UB: no return statement for a delta of 0; UBFIX returns value unchanged.
#endif
}

/// Lays out the sprite viewer's ring around pixel (x, z): one position per
/// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
/// pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
/// sprite). B held turns the ring. Called by SpriteViewer_Run when it starts
/// and while Start moves the ring.
void SpriteViewer_LayOutRing(s32 centerX, s32 centerY,
                             struct SpritePlacement *positions) {
  struct SpritePlacement *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = ((struct Sprite *)Ram_HeapSlots->sprites)->entries[0]->kind;
  if ((gInput.held & KEY_B) != 0)
    angle = (s16)(gVBlankCount << 8);
  switch ((u32)kind) {
  case ANIM_FACING_THREE_PICTURES:
    for (i = 0; i < 6; i++) {
      angle <<= ANGLE_BITS;
      positions->x = toFIXED(centerX);
      angle = (u32)angle >> ANGLE_BITS;
      positions->height = 0;
      positions->y = toFIXED(centerY);
      Vector_AddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + DEGtoANG(60));
    }
    for (; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
      positions++;
    break;
  case ANIM_FACING_FIVE_PICTURES:
  case ANIM_FACING_EIGHT_PICTURES:
  case ANIM_FACING_EIGHT_NO_TABLE:
  case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
    for (i = 0; i < 8; i++) {
      angle = (u32)angle << ANGLE_BITS;
      positions->x = toFIXED(centerX);
      angle = (u32)angle >> ANGLE_BITS;
      positions->height = 0;
      positions->y = toFIXED(centerY);
      Vector_AddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + DEGtoANG(45));
    }
    for (; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
      positions++;
    break;
  case ANIM_FACING_FOUR_PICTURES:
  case ANIM_FACING_SIX_PICTURES:
    for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
      angle <<= ANGLE_BITS;
      positions->x = toFIXED(centerX);
      angle = (u32)angle >> ANGLE_BITS;
      positions->height = 0;
      positions->y = toFIXED(centerY);
      Vector_AddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + DEGtoANG(36));
    }
    break;
  case ANIM_FACING_TWO_PART:
    turn = angle;
    angle = (s16)(turn + DEGtoANG(90));
    i = 0;
    pair = positions;
    while (i < 2) {
      pair[i * 2].x = toFIXED(centerX);
      pair[i * 2].height = 0;
      pair[i * 2].y = toFIXED(centerY);
      angle <<= ANGLE_BITS;
      angle = (u32)angle >> ANGLE_BITS;
      Vector_AddPolarOffset(toFIXED(40), angle, &positions[0].x);
      pair[i * 2 + 1].x = toFIXED(centerX);
      {
        s32 *point = SpritePlacement_Values(&positions[1]);
        pair[i * 2 + 1].height = 0;
        pair[i * 2 + 1].y = toFIXED(centerY);
        Vector_AddPolarOffset(toFIXED(40), (u16)angle, point);
      }
      angle = ((u32)angle + DEGtoANG(180)) << ANGLE_BITS;
      i++;
      positions += 2;
      angle >>= ANGLE_BITS;
    }
    break;
  default:
    angle =
        (s32)(((u32)angle << ANGLE_BITS) + ((u32)DEGtoANG(45) << ANGLE_BITS)) >>
        ANGLE_BITS;
    for (i = 0; i < 4; i++) {
      angle = (u32)angle << ANGLE_BITS;
      angle = (u32)angle >> ANGLE_BITS;
      positions->x = toFIXED(centerX);
      positions->height = 0;
      positions->y = toFIXED(centerY);
      Vector_AddPolarOffset(toFIXED(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + DEGtoANG(90));
    }
    for (i = 4; i < UI_SPRITE_VIEWER_COPY_COUNT; i++)
      positions++;
    break;
  }
}

/// Gives sprite animation slot of each of the viewer's ten sprites the colour
/// mode value.
void SpriteViewer_SetLayerColorMode(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->sprites;
  s32 offset = (slot & 3) * sizeof(struct SpriteAnimation *) +
               offsetof(struct Sprite, entries);
  s32 count = 9;

  do {
    struct SpriteAnimation *entry = *(struct SpriteAnimation **)(base + offset);

    count--;
    entry->colorMode = value;
    base += sizeof(struct Sprite);
  } while (count >= 0);
}

/// Gives sprite animation slot of each of the viewer's ten sprites the drawing
/// order value.
void SpriteViewer_SetLayerPriority(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->sprites;
  s32 offset = (slot & 3) * sizeof(struct SpriteAnimation *) +
               offsetof(struct Sprite, entries);
  s32 count = 9;

  do {
    struct SpriteAnimation *entry = *(struct SpriteAnimation **)(base + offset);

    count--;
    entry->priority = value;
    base += sizeof(struct Sprite);
  } while (count >= 0);
}

/// Starts animation value on sprite animation slot of each of the viewer's ten
/// sprites, each one a frame (16 timer units) behind the last, so the copies
/// are staggered.
void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value) {
  s32 zero;
  s32 count;
  s32 offset;
  struct AnimationMetadata *metadata;
  struct SpriteAnimation *entry;
  u8 *state;

  state = Ram_HeapSlots->sprites;
  count = 0;
  offset = (3 & slot) * sizeof(struct SpriteAnimation *) +
           offsetof(struct Sprite, entries);
  zero = 0;
  do {
    entry = *(struct SpriteAnimation **)(state + offset);
    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animationId);
      if (value < metadata->animationCount) {
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[value];
        entry->timer = count * SPRITE_ANIMATION_NORMAL_STEP;
        entry->step = SPRITE_ANIMATION_NORMAL_STEP;
        entry->scriptOffset = zero;
        entry->pictureBase = zero;
        entry->picture = SPRITE_PICTURE_NONE;
      }
      ((struct Sprite *)state)->offsetY = metadata->adjustY;
      ((struct Sprite *)state)->rotation = zero;
    }
    count += 1;
    state += sizeof(struct Sprite);
  } while (count <= 9);
}

/// Sets sprite no on sprite animation slot of each of the viewer's ten sprites,
/// and resets each sprite animation from its new metadata.
void SpriteViewer_SetLayerSprite(s32 slot, s32 no) {
  u8 *base = Ram_HeapSlots->sprites;
  s32 offset;
  s32 count;

  Resource_GetMetadataRecordFar(no);
  offset = (slot & 3) * sizeof(struct SpriteAnimation *) +
           offsetof(struct Sprite, entries);
  count = 9;
  do {
    struct SpriteAnimation *entry = *(struct SpriteAnimation **)(base + offset);

    count--;
    entry->animationId = no;
    SpriteAnimation_Reset(entry);
    base += sizeof(struct Sprite);
  } while (count >= 0);
}

extern const struct SpriteScale gSpriteViewer_Scale;

/// Each frame in the sprite viewer: places each copy of the sprite at its ring
/// position, facing the way that position stands for.
void SpriteViewer_PlaceRing(void) {
  struct Sprite *sprites = Ram_HeapSlots->sprites;
  u32 kind = sprites->entries[0]->kind;
  struct SpriteScale scale;
  u8 *table;
  u16 angle;
  u16 step;
  u16 odd = 0;
  u32 count;
  u32 i;

  scale = gSpriteViewer_Scale;
  table = (u8 *)Ram_HeapSlots->battleState;

  switch (kind) {
  case ANIM_FACING_THREE_PICTURES:
    angle = 0;
    step = DEGtoANG(60);
    count = 6;
    break;
  case ANIM_FACING_FIVE_PICTURES:
  case ANIM_FACING_EIGHT_PICTURES:
  case ANIM_FACING_EIGHT_NO_TABLE:
  case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
    angle = 0;
    step = DEGtoANG(45);
    count = 8;
    break;
  case ANIM_FACING_FOUR_PICTURES:
  case ANIM_FACING_SIX_PICTURES:
    angle = 0;
    step = DEGtoANG(36);
    count = UI_SPRITE_VIEWER_COPY_COUNT;
    break;
  case ANIM_FACING_TWO_PART:
    angle = 0;
    step = 0;
    odd = DEGtoANG(180);
    count = 4;
    break;
  default:
    angle = DEGtoANG(45);
    step = DEGtoANG(90);
    count = 4;
    break;
  }

  for (i = 0; i < count; i++) {
    Render_ApplyProjectedPlacement(
        sprites, (s32 *)(table + i * sizeof(struct SpritePlacement)),
        (s32 *)&scale, angle);
    sprites++;
    angle += step;
    if (i & 1)
      angle += odd;
  }
}
