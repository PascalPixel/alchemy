/// @file
/// A debug sprite viewer.
// Part of field.c.

#include "../field_internal.h"

enum {
  /// The sprite copies and their ring positions; a facing kind places 4, 6, 8
  /// or 10 of them (Map_BuildProbeRing).
  UI_SPRITE_VIEWER_COPY_COUNT = 10,
  UI_SPRITE_VIEWER_SPRITE_LIMIT = 512,    ///< sprite ids 0 to 511 are offered
  UI_SPRITE_VIEWER_ANIMATION_LIMIT = 100, ///< animations 0 to 99
  /// A layer priority above 3, which Sprite_ComposeAnimationFrame skips.
  UI_SPRITE_VIEWER_HIDDEN_PRIORITY = 8
};

/// What left and right choose for L and R to change on the selected layer.
enum UiSpriteViewerEditMode {
  UI_SPRITE_VIEWER_EDIT_ANIMATION,
  UI_SPRITE_VIEWER_EDIT_SPRITE,
  UI_SPRITE_VIEWER_EDIT_PRIORITY,
  UI_SPRITE_VIEWER_EDIT_COLOR,
  UI_SPRITE_VIEWER_EDIT_COUNT
};

/// One animation layer the sprite viewer edits.
struct UiSpriteViewerLayer {
  s16 spriteId;       ///< metadata record selected for this layer
  s16 animationIndex; ///< script selected from that metadata
  u8 enabled;         ///< zero hides it by a priority above the drawing range
  s8 priority;        ///< drawing order, 0 to 3
  s8 colorMode;       ///< SpriteColorMode; the viewer cycles through 0 to 15
};

/// A debug sprite viewer, which never returns. It shows a sprite (with up to
/// three more animation layers) once for each of its facings, in a ring.
/// Start with the direction pad moves the ring; otherwise up and down choose
/// a layer and left and right what to edit: 0 the animation, 1 the sprite
/// number (ten at a time with Start), 2 the priority, 3 the draw parameter,
/// changed with L and R. A restarts the animation, or in the other modes
/// turns a layer on or off. Pressing Start toggles the shadows; B turns the
/// ring when it is rebuilt with Start held. Select starts over; it passes
/// a far-call resource set (the alternate one unless B is held) to
/// RuntimeDispatch_ReturnZero, a stub that ignores it.
///
/// It has a bare veneer in the field far-call table, and no caller names it.
/// Takes heap slot 9 for the ring of ten positions and runs
/// Battle_PlaceActorsByFormationKind each frame to draw the copies.
void Ui_RunIconMonitor(void) {
  struct UiSpriteViewerLayer layers[ANIMATION_LAYER_COUNT];
  struct AnimationMetadata *spriteMetadata;
  struct AnimationObject *sprites;
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
    struct AnimationObject *sprite;
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
  ring = Runtime_AllocateBlock(HEAP_SLOT_BATTLE,
                               UI_SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
  gDecodeFillByte = 3;

  // Only the ring's first word is cleared; Map_BuildProbeRing fills it.
  Dma_Fill32Cell(fillWord, 0, ring, 4);
  Dma_Fill32Cell(fillWord, 0, layers, sizeof(layers));

  // All four layers start on the first sprite with an image, animation 1,
  // priority 1; only layer 0 is shown.
  spriteId = Ui_FindNextSpriteWithImage(-1, 1);
  for (i = 0; i < ANIMATION_LAYER_COUNT; i++) {
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
  Fade_FromBlack(1);

  // Rebuilt from here whenever layer 0's sprite changes or Select is
  // pressed: a fresh heap, task table and VRAM cache each time.
restart:
  Runtime_InitializeHeap();
  Scheduler_ResetTaskTable();
  ring = Runtime_AllocateBlock(HEAP_SLOT_BATTLE,
                               UI_SPRITE_VIEWER_COPY_COUNT * sizeof(*ring));
  VramCache_Reset();
  ObjectSystem_Initialize(2);
  // With no map loaded, the map cell buffer holds layer 0's frames; a
  // two-part sprite (draw kind 20) puts its second part in the actor
  // sprite buffer, as resource slot 1.
  ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, layers[0].spriteId, 0);
  spriteMetadata = Resource_GetMetadataRecordFar(layers[0].spriteId);
  if (spriteMetadata->drawKind == 20) {
    ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, layers[0].spriteId + 1,
                      0);
  }

  // Ten copies of the sprite; for a sprite of draw kind 20 every other copy
  // shows its second part, number + 1, from resource slot 1.
  for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
    spriteMetadata = Resource_GetMetadataRecordFar(layers[0].spriteId);
    spriteWork.resourceOffset = 0;
    if (spriteMetadata->drawKind == 20 && (i & 1) != 0) {
      spriteWork.resourceOffset = 1;
    }
    spriteWork.sprite =
        ResourceObject_Create(layers[0].spriteId + spriteWork.resourceOffset +
                              (spriteWork.resourceOffset << 12));
    // Layers 1 to 3 are added to every copy whether shown or not; a hidden
    // layer is kept out of the drawing by its priority below.
    ResourceMetadata_Register(spriteWork.sprite, layers[1].spriteId);
    ResourceMetadata_Register(spriteWork.sprite, layers[2].spriteId);
    ResourceMetadata_Register(spriteWork.sprite, layers[3].spriteId);
    /* FAKEMATCH(temporary): the native adds the flags offset to the sprite's
     * own register (r5) in place; as spriteWork.sprite->flags = shadowsEnabled,
     * 21 of 672 instructions differ in tbs-en. */
    spriteWork.flags = &spriteWork.sprite->flags;
    *spriteWork.flags = shadowsEnabled;
  }

  // Priority 8 hides a layer: Sprite_ComposeAnimationFrame draws only
  // priorities 0 to 3.
  for (i = 0; i < ANIMATION_LAYER_COUNT; i++) {
    if (layers[i].enabled != 0) {
      Ui_SetGridColumnPriority(i, layers[i].priority);
    } else {
      Ui_SetGridColumnPriority(i, UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
    }
    Ui_SetGridColumnParameter(i, layers[i].colorMode);
    Ui_FillGridColumnFromMetadata(i, layers[i].animationIndex);
  }
  // The copies are drawn each frame by the task below, from the ring's
  // positions.

  Map_BuildProbeRing(x, y, ring);
  // Order 3200 (0xc80), where the field camera runs.
  Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
  WaitFrames(1);
  heldKeys = &gKeysHeld;
  repeatedKeys = &gKeysRepeat;
  for (;;) {
    // Start held: the direction pad moves the ring a pixel a frame.
    // Otherwise up and down pick the layer and left and right the mode,
    // both wrapping at four.

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
      Map_BuildProbeRing(x, y, ring);
    } else {
      (void)gKeysRepeat;
      if ((*repeatedKeys & KEY_UP) != 0) {
        selectedLayer = (selectedLayer - 1) & (ANIMATION_LAYER_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        selectedLayer = (selectedLayer + 1) & (ANIMATION_LAYER_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        editMode = (editMode - 1) & (UI_SPRITE_VIEWER_EDIT_COUNT - 1);
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        editMode++;
        editMode &= UI_SPRITE_VIEWER_EDIT_COUNT - 1;
      }
    }

    // A press of Start also toggles the shadows (sprite flag bit 0) of the
    // ten copies.
    if ((gKeysPressed & KEY_START) != 0) {
      sprites = gSpriteObjects;
      shadowsEnabled ^= 1;
      for (i = 0; i < UI_SPRITE_VIEWER_COPY_COUNT; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    // Mode 0, the animation: layer 1 follows layer 0's and is not edited on
    // its own; 0 to 99.
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

    // Mode 1, the sprite number: one sprite with an image at a time, or ten
    // with Start held.
    case UI_SPRITE_VIEWER_EDIT_SPRITE:
      changed = 0;
      spriteSteps = 1;
      if ((*heldKeys & KEY_START) != 0) {
        spriteSteps = 10;
      }
      if ((*repeatedKeys & KEY_L) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].spriteId =
              Ui_FindNextSpriteWithImage(layers[selectedLayer].spriteId, -1);
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].spriteId =
              Ui_FindNextSpriteWithImage(layers[selectedLayer].spriteId, 1);
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      // A new sprite on layer 0 changes the ring and the copies: start over.
      if (selectedLayer == 0) {
        goto restart;
      }
      // A hidden layer only remembers the number until it is shown again.
      if (layers[selectedLayer].enabled != 0) {
        Ui_SetGridColumnNumber(selectedLayer, layers[selectedLayer].spriteId);
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
        Ui_SetGridColumnParameter(selectedLayer,
                                  layers[selectedLayer].colorMode);
        Ui_FillGridColumnFromMetadata(selectedLayer,
                                      layers[selectedLayer].animationIndex);
      }
      goto next;

    // Mode 2, the layer's draw priority, 0 to 3 and wrapping.
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
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
      }
      goto next;

    // Mode 3, the draw parameter (SpriteColorMode), 0 to 15 and wrapping;
    // 15, hidden, is the value ObjectGroup_SetChildValueUnlessFifteen leaves
    // alone.
    case UI_SPRITE_VIEWER_EDIT_COLOR:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0) {
          layers[selectedLayer].colorMode = 15;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > 15) {
          layers[selectedLayer].colorMode = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].enabled != 0) {
        Ui_SetGridColumnParameter(selectedLayer,
                                  layers[selectedLayer].colorMode);
      }
      goto next;

    default:
      break;
    }

    // A, in mode 0: restart the selected layer's animation (layer 0 also
    // restarts layer 1, which follows it). In the other modes: show or hide
    // the layer; layer 0 is always shown.
    if ((*repeatedKeys & KEY_A) != 0) {
      if (editMode == UI_SPRITE_VIEWER_EDIT_ANIMATION) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].enabled != 0) {
            Ui_FillGridColumnFromMetadata(selectedLayer,
                                          layers[selectedLayer].animationIndex);
            if (selectedLayer == 0) {
              Ui_FillGridColumnFromMetadata(1, layers[1].animationIndex);
            }
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].enabled ^= 1;
        Ui_SetGridColumnPriority(selectedLayer,
                                 layers[selectedLayer].enabled != 0
                                     ? layers[selectedLayer].priority
                                     : UI_SPRITE_VIEWER_HIDDEN_PRIORITY);
        goto next;
      }
    }

    // RuntimeDispatch_ReturnZero ignores its arguments, so the resource set
    // chosen here changes nothing in this build.
    if ((*repeatedKeys & KEY_SELECT) != 0) {
      Scheduler_ResetTaskTable();
      if ((*heldKeys & KEY_B) != 0) {
        RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls,
                                   FarCall_ResourceTable);
      } else {
        RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate,
                                   FarCall_ResourceTable);
      }
      goto restart;
    }
    WaitFrames(1);
  }
}

/// Returns the next sprite id after spriteId, stepping by direction (1 or
/// -1) and wrapping within 0-511, whose metadata has an image (a nonzero
/// width). Called by Ui_RunIconMonitor.
s32 Ui_FindNextSpriteWithImage(s32 spriteId, s32 direction) {
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
/// pairs for kind 20), 56 pixels out (40 for kind 20). B held turns the ring.
/// Called by Ui_RunIconMonitor when it starts and while Start moves the ring.
void Map_BuildProbeRing(s32 centerX, s32 centerY,
                        struct SpritePlacement *positions) {
  struct SpritePlacement *pairedPositions;
  s32 angle;
  s32 facingIndex;
  u16 initialHeading;
  u8 facingKind;

  angle = 0;
  // Select the viewer sprite's formation; headings wrap as 16-bit turns.
  facingKind = gSpriteObjects->entries[0]->kind;
  // With B held the ring starts at an angle that turns with the frame
  // counter, a full turn every 256 frames.
  if ((gKeysHeld & KEY_B) != 0)
    angle = (s16)(gFrameTick << 8);
  // 0x2aaa, 0x2000, 0x1999 and 0x4000 are a sixth, an eighth, a tenth and
  // a quarter of a turn; 0x380000 is 56 pixels. Unused positions of the ten
  // are skipped.
  switch ((u32)facingKind) {
  case 3:
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
  case 5:
  case 8:
  case 44:
  case 88:
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
  case 4:
  case 6:
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
  // Draw kind 20: two places 40 pixels out, a quarter turn round and then
  // opposite it, each holding both of the sprite's parts.
  case 20:
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
    struct AnimationObject *spriteGroup = gSpriteObjects;                      \
    s32 spriteIndex;                                                           \
                                                                               \
    layerIndex &= ANIMATION_LAYER_COUNT - 1;                                   \
    for (spriteIndex = 0; spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT;           \
         spriteIndex++) {                                                      \
      spriteGroup->entries[layerIndex]->field = value;                         \
      spriteGroup++;                                                           \
    }                                                                          \
  }

/// Sets the colour mode (SpriteColorMode, the layer's draw parameter) or the
/// priority of one layer of the viewer's ten sprites.
UI_DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnParameter, param)
UI_DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnPriority, priority)

/// Starts animationIndex on layerIndex of each of the viewer's ten sprites,
/// each one a frame (16 timer units) behind the last, so the copies are
/// staggered. Also sets each copy's offsetY and clears its rotation.
/// Called by Ui_RunIconMonitor.
void Ui_FillGridColumnFromMetadata(s32 layerIndex, s32 animationIndex) {
  s32 spriteIndex;
  struct AnimationObject *spriteGroup;

  spriteGroup = gSpriteObjects;
  spriteIndex = 0;
  do {
    struct AnimationEntry *entry =
        spriteGroup->entries[layerIndex & (ANIMATION_LAYER_COUNT - 1)];
    struct AnimationMetadata *metadata;
    s32 initialFrame = 0;

    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (animationIndex < metadata->animationCount) {
        // Copy n starts n frames (16 timer units each) late, at normal
        // speed, with no frame drawn yet.
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[animationIndex];
        entry->timer = spriteIndex * ANIMATION_NORMAL_STEP;
        entry->step = ANIMATION_NORMAL_STEP;
        entry->pos = initialFrame;
        entry->frameBase = initialFrame;
        entry->frame = 0xff;
      }
      spriteGroup->offsetY = metadata->adjustY;
      spriteGroup->rotation = initialFrame;
    }
    spriteIndex++;
    spriteGroup++;
  } while (spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT);
}

/// Sets spriteId on layerIndex of each of the viewer's ten sprites.
/// Resets each layer from its new metadata. Called by Ui_RunIconMonitor when
/// a layer other than 0 gets a new sprite.
void Ui_SetGridColumnNumber(s32 layerIndex, s32 spriteId) {
  struct AnimationObject *spriteGroup = gSpriteObjects;
  s32 spriteIndex;

  // The ROM looks the metadata up here and drops the result.
  Resource_GetMetadataRecordFar(spriteId);
  layerIndex &= ANIMATION_LAYER_COUNT - 1;
  for (spriteIndex = 0; spriteIndex < UI_SPRITE_VIEWER_COPY_COUNT;
       spriteIndex++) {
    struct AnimationEntry *entry = spriteGroup->entries[layerIndex];

    entry->animId = spriteId;
    Animation_InitWorkFromMetadata(entry);
    spriteGroup++;
  }
}

/// Each frame in the sprite viewer: places each copy of the sprite at its
/// ring position, facing the way that position stands for.
/// Despite its name it belongs to the sprite viewer, which schedules it; its
/// ring is the placements in heap slot 9 (HEAP_SLOT_BATTLE) and its sprites
/// the pool in heap slot 4.
void Battle_PlaceActorsByFormationKind(void) {
  struct AnimationObject *sprite = WORK_SLOT(4);
  u32 facingKind = sprite->entries[0]->kind;
  struct SpriteScale scale;
  struct SpritePlacement *facingPositions;
  u16 angle;
  u16 step;
  u16 pairedFacingStep = 0;
  u32 count;
  u32 spriteIndex;

  scale = gBattle_FormationPlacementScale;
  facingPositions = WORK_SLOT(HEAP_SLOT_BATTLE);

  switch (facingKind) {
  case 3:
    angle = 0;
    step = 0x2aaa;
    count = 6;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    angle = 0;
    step = 0x2000;
    count = 8;
    break;
  case 4:
  case 6:
    angle = 0;
    step = 0x1999;
    count = 10;
    break;
  // The two parts at each place face the same way; the second place faces
  // half a turn from the first.
  case 20:
    angle = 0;
    step = 0;
    pairedFacingStep = 0x8000;
    count = 4;
    break;
  // The eighth-turn start matches Map_BuildProbeRing's default ring.
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
