/// @file
/// A debug sprite viewer.
// Part of field.c.

#include "../field_internal.h"

enum {
  /// The sprite copies and their ring positions; a facing kind places 4, 6, 8
  /// or 10 of them (SpriteViewer_BuildProbeRing).
  UI_SPRITE_VIEWER_COPY_COUNT = 10,
  UI_SPRITE_VIEWER_SPRITE_LIMIT = 512,    ///< sprite ids 0 to 511 are offered
  UI_SPRITE_VIEWER_ANIMATION_LIMIT = 100, ///< animations 0 to 99
  /// A sprite animation priority above 3, which Sprite_ComposePicture skips.
  UI_SPRITE_VIEWER_HIDDEN_PRIORITY = 8
};

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

/// A debug sprite viewer, which never returns. It shows a sprite (with up to
/// three more sprite animations) once for each of its facings, in a ring.
/// Start with the direction pad moves the ring; otherwise up and down choose
/// a sprite animation and left and right what to edit (enum
/// UiSpriteViewerEditMode): the animation, the sprite number (ten at a time
/// with Start), the priority or the draw parameter, changed with L and R. A
/// restarts the animation, or in the other modes turns a sprite animation on or
/// off. Pressing Start toggles the shadows; B turns the ring when it is rebuilt
/// with Start held. Select starts over; it passes a far-call resource set (the
/// alternate one unless B is held) to Stub_ReturnZero, a stub that ignores it.
///
/// It has a bare veneer in the field far-call table, and no caller names it.
/// Takes HEAP_SLOT_BATTLE for the ring of ten positions and runs
/// SpriteViewer_PlaceRing each frame to draw the copies.
void SpriteViewer_Run(void) {
  struct UiSpriteViewerLayer layers[ANIM_LAYER_COUNT];
  struct AnimationMetadata *spriteMetadata;
  struct Sprite *sprites;
  volatile s32 fillWord;
  s32 spriteId;
  s32 x;
  s32 y;
  u32 editMode;
  s32 shadowsEnabled;
  struct SpritePlacement *ring;
  s32 selectedLayer;
  u32 spriteSteps;
  // The selected resource offset becomes its sprite, then its flags address.
  union {
    s32 resourceOffset;
    struct Sprite *sprite;
    u8 *flags;
  } spriteWork;
  s32 changed;
  /* FAKEMATCH(temporary): only gKeysHeld and gKeysRepeat are held, in fp and
   * r9, read through these pointers; as gKeysHeld and gKeysRepeat directly,
   * 135 of 672 instructions differ in tbs-en. */
  volatile u32 *heldKeys;
  volatile u32 *repeatedKeys;
  u32 i;

  // The ring's centre, in pixels.
  x = 144;
  y = 96;
  editMode = UI_SPRITE_VIEWER_EDIT_SPRITE;
  shadowsEnabled = 1;
  selectedLayer = 0;
  ring = AllocHeapSlotEwram(HEAP_SLOT_BATTLE,
                            UI_SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
  gDecodeFillByte = 3;

  // Only the ring's first word is cleared; SpriteViewer_BuildProbeRing fills
  // it.
  Dma_Fill32Cell(fillWord, 0, ring, 4);
  Dma_Fill32Cell(fillWord, 0, layers, sizeof(layers));

  // All four sprite animations start on the first sprite with an image,
  // animation 1, priority 1; only sprite animation 0 is shown.
  spriteId = SpriteViewer_FindNextSpriteWithImage(-1, 1);
  for (i = 0; i < ANIM_LAYER_COUNT; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = 2;
  // Only BG0 and the sprites are shown.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // A green backdrop (BGR555 green 15).
  PLTT_BACKDROP = 0x01E0;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
  ScreenFade_FromBlack(1);

  // Rebuilt from here whenever sprite animation 0's sprite changes or Select is
  // pressed: a fresh heap, task table and VRAM cache each time.
restart:
  InitHeap();
  Task_ResetAll();
  ring = AllocHeapSlotEwram(HEAP_SLOT_BATTLE,
                            UI_SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
  VramCache_Reset();
  ActorSystem_Initialize(ACTOR_SYSTEM_MAP);
  // With no map loaded, the map cell buffer holds sprite animation 0's
  // pictures; a two-part sprite (ANIM_FACING_TWO_PART) puts its second part in
  // the actor sprite buffer, as sprite picture cache 1.
  SpritePictureCache_Load(0, (u32 *)Ram_MapGridCellBuffer, layers[0].spriteId,
                          0);
  spriteMetadata = Resource_GetMetadataRecordFar(layers[0].spriteId);
  if (spriteMetadata->drawKind == ANIM_FACING_TWO_PART) {
    SpritePictureCache_Load(1, (u32 *)Ram_EnemySpriteSlots,
                            layers[0].spriteId + 1, 0);
  }

  // Ten copies of the sprite; for a two-part sprite every other copy
  // shows its second part, number + 1, from sprite picture cache 1.
  for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
    spriteMetadata = Resource_GetMetadataRecordFar(layers[0].spriteId);
    spriteWork.resourceOffset = 0;
    if (spriteMetadata->drawKind == ANIM_FACING_TWO_PART && (i & 1) != 0) {
      spriteWork.resourceOffset = 1;
    }
    spriteWork.sprite =
        Sprite_Create(layers[0].spriteId + spriteWork.resourceOffset +
                      (spriteWork.resourceOffset << 12));
    // Sprite animations 1 to 3 are added to every copy whether shown or not; a
    // hidden sprite animation is kept out of the drawing by its priority below.
    Sprite_AddAnimation(spriteWork.sprite, layers[1].spriteId);
    Sprite_AddAnimation(spriteWork.sprite, layers[2].spriteId);
    Sprite_AddAnimation(spriteWork.sprite, layers[3].spriteId);
    /* FAKEMATCH(temporary): the native adds the flags offset to the sprite's
     * own register (r5) in place; as spriteWork.sprite->flags = shadowsEnabled,
     * 21 of 672 instructions differ in tbs-en. */
    spriteWork.flags = &spriteWork.sprite->flags;
    *spriteWork.flags = shadowsEnabled;
  }

  // Priority 8 hides a sprite animation: Sprite_ComposePicture draws only
  // priorities 0 to 3.
  for (i = 0; i < ANIM_LAYER_COUNT; i++) {
    if (layers[i].enabled != 0) {
      SpriteViewer_SetGridColumnPriority(i, layers[i].priority);
    } else {
      SpriteViewer_SetGridColumnPriority(i, UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
    }
    SpriteViewer_SetGridColumnColorMode(i, layers[i].colorMode);
    SpriteViewer_StartStaggeredAnimations(i, layers[i].animationIndex);
  }
  // The copies are drawn each frame by the task below, from the ring's
  // positions.

  SpriteViewer_BuildProbeRing(x, y, ring);
  // Order 3200 (0xc80), where the field camera runs.
  Task_Add(SpriteViewer_PlaceRing, TASK_PASS_FRAME);

next:
  WaitFrames(1);
  heldKeys = &gKeysHeld;
  repeatedKeys = &gKeysRepeat;
  for (;;) {
    // Start held: the direction pad moves the ring a pixel a frame.
    // Otherwise up and down pick the sprite animation and left and right the
    // mode, both wrapping at four.

    if ((*heldKeys & KEY_START) != 0) {
      if ((*heldKeys & KEY_LEFT) != 0) {
        x--;
      }
      if ((*heldKeys & KEY_RIGHT) != 0) {
        x++;
      }
      if ((*heldKeys & KEY_UP) != 0) {
        y--;
      }
      if ((*heldKeys & KEY_DOWN) != 0) {
        y++;
      }
      SpriteViewer_BuildProbeRing(x, y, ring);
    } else {
      (void)gKeysRepeat;
      if ((*repeatedKeys & KEY_UP) != 0) {
        selectedLayer = (selectedLayer - 1) & (ANIM_LAYER_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        selectedLayer = (selectedLayer + 1) & (ANIM_LAYER_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        editMode = (editMode - 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        editMode++;
        editMode &= UI_SPRITE_VIEWER_EDIT_COUNT - 1;
      }
    }

    // A press of Start also toggles the shadows (SPRITE_FLAG_SHADOW) of the
    // ten copies.
    if ((gKeysPressed & KEY_START) != 0) {
      sprites = gSprites;
      shadowsEnabled ^= 1;
      for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    // The animation: sprite animation 1 follows sprite animation 0's and is not
    // edited on its own; 0 to 99.
    case UI_SPRITE_VIEWER_EDIT_ANIMATION:
      if (selectedLayer == 1) {
        break;
      }
      if ((gKeysRepeat & KEY_L) != 0) {
        layers[selectedLayer].animationIndex--;
        if (layers[selectedLayer].animationIndex < 0) {
          layers[selectedLayer].animationIndex = 0;
        }
        if (selectedLayer == 0) {
          layers[1].animationIndex = layers[0].animationIndex;
        }
        goto next;
      }
      if ((gKeysRepeat & KEY_R) == 0) {
        break;
      }
      layers[selectedLayer].animationIndex++;
      if (layers[selectedLayer].animationIndex >=
          UI_SPRITE_VIEWER_ANIMATION_LIMIT) {
        layers[selectedLayer].animationIndex =
            UI_SPRITE_VIEWER_ANIMATION_LIMIT - 1;
      }
      if (selectedLayer == 0) {
        layers[1].animationIndex = layers[0].animationIndex;
      }
      goto next;

    // The sprite number: one sprite with an image at a time, or ten
    // with Start held.
    case UI_SPRITE_VIEWER_EDIT_SPRITE:
      changed = 0;
      spriteSteps = 1;
      if ((*heldKeys & KEY_START) != 0) {
        spriteSteps = 10;
      }
      if ((*repeatedKeys & KEY_L) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].spriteId = SpriteViewer_FindNextSpriteWithImage(
              layers[selectedLayer].spriteId, -1);
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].spriteId = SpriteViewer_FindNextSpriteWithImage(
              layers[selectedLayer].spriteId, 1);
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      // A new sprite on sprite animation 0 changes the ring and the copies:
      // start over.
      if (selectedLayer == 0) {
        goto restart;
      }
      // A hidden sprite animation only remembers the number until it is shown
      // again.
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetGridColumnNumber(selectedLayer,
                                         layers[selectedLayer].spriteId);
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
        SpriteViewer_StartStaggeredAnimations(
            selectedLayer, layers[selectedLayer].animationIndex);
      }
      goto next;

    // The sprite animation's draw priority, 0 to 3 and wrapping.
    case UI_SPRITE_VIEWER_EDIT_PRIORITY:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0) {
          layers[selectedLayer].priority = 3;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].priority++;
        if (layers[selectedLayer].priority > 3) {
          layers[selectedLayer].priority = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
      }
      goto next;

    // The draw parameter (SpriteColorMode), 0 to 15 and wrapping;
    // SPRITE_COLOR_HIDDEN is the value Sprite_SetAnimationColorModes
    // leaves alone.
    case UI_SPRITE_VIEWER_EDIT_COLOR:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0) {
          layers[selectedLayer].colorMode = SPRITE_COLOR_HIDDEN;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > SPRITE_COLOR_HIDDEN) {
          layers[selectedLayer].colorMode = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
      }
      goto next;

    default:
      break;
    }

    // A, editing the animation: restart the selected sprite animation (sprite
    // animation 0 also restarts sprite animation 1, which follows it). In the
    // other modes: show or hide the sprite animation; sprite animation 0 is
    // always shown.
    if ((*repeatedKeys & KEY_A) != 0) {
      if (editMode == UI_SPRITE_VIEWER_EDIT_ANIMATION) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].enabled != 0) {
            SpriteViewer_StartStaggeredAnimations(
                selectedLayer, layers[selectedLayer].animationIndex);
            if (selectedLayer == 0) {
              SpriteViewer_StartStaggeredAnimations(1,
                                                    layers[1].animationIndex);
            }
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].enabled ^= 1;
        SpriteViewer_SetGridColumnPriority(
            selectedLayer, layers[selectedLayer].enabled != 0
                               ? layers[selectedLayer].priority
                               : UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
        goto next;
      }
    }

    // Stub_ReturnZero ignores its arguments, so the resource set
    // chosen here changes nothing in this build.
    if ((*repeatedKeys & KEY_SELECT) != 0) {
      Task_ResetAll();
      if ((*heldKeys & KEY_B) != 0) {
        Stub_ReturnZero((s32)&ResourceId_ResourceFarCalls,
                        FarCall_ResourceTable);
      } else {
        Stub_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate,
                        FarCall_ResourceTable);
      }
      goto restart;
    }
    WaitFrames(1);
  }
}

/// Returns the next sprite id after spriteId, stepping by direction (1 or
/// -1) and wrapping within 0-511, whose metadata has an image (a nonzero
/// width). Called by SpriteViewer_Run.
s32 SpriteViewer_FindNextSpriteWithImage(s32 spriteId, s32 direction) {
  for (;;) {
    spriteId += direction;
    // Past either end, jump to just beyond the other, so the next step
    // lands on 511 or 0.
    if (spriteId < 0) {
      spriteId = UI_SPRITE_VIEWER_SPRITE_LIMIT;
      continue;
    }
    {
      /* FAKEMATCH(temporary): the bound in a local compares as cmp against
       * 0x200 with blt, as native; as spriteId >= 0x200, 3 of 25 instructions
       * differ in tbs-en. */
      s32 spriteLimit = UI_SPRITE_VIEWER_SPRITE_LIMIT;
      if (spriteId >= spriteLimit) {
        spriteId = -1;
        continue;
      }
    }
    if (Resource_GetMetadataRecordFar(spriteId)->width) {
      return spriteId;
    }
  }
}

/// Lays out the sprite viewer's ring around pixel (x, z): one position per
/// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
/// pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
/// sprite). B held turns the ring. Called by SpriteViewer_Run when it starts
/// and while Start moves the ring.
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY,
                                 struct SpritePlacement *positions) {
  struct SpritePlacement *pairedPositions;
  s32 angle;
  s32 facingIndex;
  u16 initialHeading;
  u8 facingKind;

  angle = 0;
  // Select the viewer sprite's formation; headings wrap as 16-bit turns.
  facingKind = gSprites->entries[0]->kind;
  // With B held the ring starts at an angle that turns with the frame
  // counter, a full turn every 256 frames.
  if ((gKeysHeld & KEY_B) != 0)
    angle = (s16)(gVBlankCount << 8);
  // 0x2aaa, 0x2000, 0x1999 and 0x4000 are a sixth, an eighth, a tenth and
  // a quarter of a turn; 0x380000 is 56 pixels. Unused positions of the ten
  // are skipped.
  switch ((u32)facingKind) {
  case ANIM_FACING_THREE_PICTURES:
    for (facingIndex = 0; facingIndex < 6; facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; facingIndex < UI_SPRITE_VIEWER_COPY_COUNT; facingIndex++)
      positions++;
    break;
  case ANIM_FACING_FIVE_PICTURES:
  case ANIM_FACING_EIGHT_PICTURES:
  case ANIM_FACING_EIGHT_NO_TABLE:
  case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
    for (facingIndex = 0; facingIndex < 8; facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x2000);
    }
    for (; facingIndex < UI_SPRITE_VIEWER_COPY_COUNT; facingIndex++)
      positions++;
    break;
  case ANIM_FACING_FOUR_PICTURES:
  case ANIM_FACING_SIX_PICTURES:
    for (facingIndex = 0; facingIndex < UI_SPRITE_VIEWER_COPY_COUNT;
         facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  // Two places 40 pixels out, a quarter turn round and then
  // opposite it, each holding both of the sprite's parts.
  case ANIM_FACING_TWO_PART:
    initialHeading = angle;
    angle = (s16)(initialHeading + 0x4000);
    facingIndex = 0;
    pairedPositions = positions;
    while (facingIndex < 2) {
      pairedPositions[facingIndex * 2].x = centerX << 16;
      pairedPositions[facingIndex * 2].height = 0;
      pairedPositions[facingIndex * 2].y = centerY << 16;
      angle = (u32)angle << 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle,
                            SpritePlacement_Values(&positions[0]));
      pairedPositions[facingIndex * 2 + 1].x = centerX << 16;
      {
        s32 *point = SpritePlacement_Values(&positions[1]);
        pairedPositions[facingIndex * 2 + 1].height = 0;
        pairedPositions[facingIndex * 2 + 1].y = centerY << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = ((u32)angle + 0x8000) << 16;
      facingIndex++;
      positions += 2;
      angle >>= 16;
    }
    break;
  // Any other kind: four places, starting an eighth of a turn round.
  default:
    angle = (s32)(((u32)angle << 16) + 0x20000000) >> 16;
    for (facingIndex = 0; facingIndex < 4; facingIndex++) {
      angle = (u32)angle << 16;
      angle = (u32)angle >> 16;
      positions->x = centerX << 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x4000);
    }
    for (facingIndex = 4; facingIndex < UI_SPRITE_VIEWER_COPY_COUNT;
         facingIndex++)
      positions++;
    break;
  }
}

/// Defines name(slot, value), which sets field of entry slot (0-3) in each of
/// the first ten sprites to value.
#define UI_DEFINE_SET_GRID_COLUMN(name, field)                                 \
  void name(s32 layerIndex, s32 value) {                                       \
    struct Sprite *spriteGroup = gSprites;                                     \
    s32 spriteIndex;                                                           \
                                                                               \
    layerIndex &= ANIM_LAYER_COUNT - 1;                                        \
    for (spriteIndex = 0; spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT;           \
         spriteIndex++) {                                                      \
      spriteGroup->entries[layerIndex]->field = value;                         \
      spriteGroup++;                                                           \
    }                                                                          \
  }

/// Sets the colour mode (SpriteColorMode, the sprite animation's draw
/// parameter) or the priority of one sprite animation of the viewer's ten
/// sprites.
UI_DEFINE_SET_GRID_COLUMN(SpriteViewer_SetGridColumnColorMode, colorMode)
UI_DEFINE_SET_GRID_COLUMN(SpriteViewer_SetGridColumnPriority, priority)

/// Starts animationIndex on layerIndex of each of the viewer's ten sprites,
/// each one a frame (16 timer units) behind the last, so the copies are
/// staggered. Also sets each copy's offsetY and clears its rotation.
/// Called by SpriteViewer_Run.
void SpriteViewer_StartStaggeredAnimations(s32 layerIndex, s32 animationIndex) {
  s32 spriteIndex;
  struct Sprite *spriteGroup;

  spriteGroup = gSprites;
  spriteIndex = 0;
  do {
    struct SpriteAnimation *entry =
        spriteGroup->entries[layerIndex & (ANIM_LAYER_COUNT - 1)];
    struct AnimationMetadata *metadata;
    s32 initialFrame = 0;

    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animationId);
      if (animationIndex < metadata->animationCount) {
        // Copy n starts n frames (16 timer units each) late, at normal
        // speed, with no frame drawn yet.
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[animationIndex];
        entry->timer = spriteIndex * SPRITE_ANIMATION_NORMAL_STEP;
        entry->step = SPRITE_ANIMATION_NORMAL_STEP;
        entry->scriptOffset = initialFrame;
        entry->pictureBase = initialFrame;
        entry->picture = 0xff;
      }
      spriteGroup->offsetY = metadata->adjustY;
      spriteGroup->rotation = initialFrame;
    }
    spriteIndex++;
    spriteGroup++;
  } while (spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT);
}

/// Sets spriteId on layerIndex of each of the viewer's ten sprites.
/// Resets each sprite animation from its new metadata. Called by
/// SpriteViewer_Run when a sprite animation other than 0 gets a new sprite.
void SpriteViewer_SetGridColumnNumber(s32 layerIndex, s32 spriteId) {
  struct Sprite *spriteGroup = gSprites;
  s32 spriteIndex;

  // The ROM looks the metadata up here and drops the result.
  Resource_GetMetadataRecordFar(spriteId);
  layerIndex &= ANIM_LAYER_COUNT - 1;
  for (spriteIndex = 0; spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT;
       spriteIndex++) {
    struct SpriteAnimation *entry = spriteGroup->entries[layerIndex];

    entry->animationId = spriteId;
    SpriteAnimation_Reset(entry);
    spriteGroup++;
  }
}

/// Each frame in the sprite viewer: places each copy of the sprite at its
/// ring position, facing the way that position stands for. SpriteViewer_Run
/// adds it as a task; its ring is the placements in HEAP_SLOT_BATTLE and its
/// sprites the pool in HEAP_SLOT_SPRITES.
void SpriteViewer_PlaceRing(void) {
  struct Sprite *sprite = HEAP_SLOT_BLOCK(HEAP_SLOT_SPRITES);
  u32 facingKind = sprite->entries[0]->kind;
  struct SpriteScale scale;
  struct SpritePlacement *facingPositions;
  u16 angle;
  u16 step;
  u16 pairedFacingStep = 0;
  u32 count;
  u32 spriteIndex;

  scale = gSpriteViewer_Scale;
  facingPositions = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);

  switch (facingKind) {
  case ANIM_FACING_THREE_PICTURES:
    angle = 0;
    step = 0x2aaa;
    count = 6;
    break;
  case ANIM_FACING_FIVE_PICTURES:
  case ANIM_FACING_EIGHT_PICTURES:
  case ANIM_FACING_EIGHT_NO_TABLE:
  case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
    angle = 0;
    step = 0x2000;
    count = 8;
    break;
  case ANIM_FACING_FOUR_PICTURES:
  case ANIM_FACING_SIX_PICTURES:
    angle = 0;
    step = 0x1999;
    count = 10;
    break;
  // The two parts at each place face the same way; the second place faces
  // half a turn from the first.
  case ANIM_FACING_TWO_PART:
    angle = 0;
    step = 0;
    pairedFacingStep = 0x8000;
    count = 4;
    break;
  // The eighth-turn start matches SpriteViewer_BuildProbeRing's default ring.
  default:
    angle = 0x2000;
    step = 0x4000;
    count = 4;
    break;
  }

  // Copy n faces n steps round, the way its ring position lies.
  for (spriteIndex = 0; spriteIndex < count; spriteIndex++) {
    Render_ApplyProjectedPlacement(sprite, &facingPositions[spriteIndex],
                                   &scale, angle);
    sprite++;
    angle += step;
    if (spriteIndex & 1)
      angle += pairedFacingStep;
  }
}

#undef UI_DEFINE_SET_GRID_COLUMN
