/// @file
/// A debug sprite viewer.
/// Part of field.c, the field object, compiled only through it.

#include "../field_internal.h"

/// One animation layer the sprite viewer edits.
struct UiIconColumn {
  s16 number;    ///< sprite number
  s16 animation; ///< animation shown
  u8 on;         ///< layer shown
  s8 priority;
  s8 param;
};

/// A debug sprite viewer, which never returns. It shows a sprite (with up to
/// three more animation layers) once for each of its facings, in a ring.
/// Start with the direction pad moves the ring; otherwise up and down choose
/// a layer and left and right what to edit: 0 the animation, 1 the sprite
/// number (ten at a time with Start), 2 the priority, 3 the draw parameter,
/// changed with L and R. A restarts the animation, or in the other modes
/// turns a layer on or off. Pressing Start toggles the shadows; B turns the
/// ring when it is rebuilt with Start held. Select reloads the far-call
/// resources (the alternate set unless B is held) and starts over.
///
/// It has a bare veneer in the field far-call table, and no caller names it.
/// Takes heap slot 9 for the ring of ten positions and runs
/// Battle_PlaceActorsByFormationKind each frame to draw the copies.
void Ui_RunIconMonitor(void) {
  struct UiIconColumn layers[4];
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
  editMode = 1;
  shadowsEnabled = 1;
  selectedLayer = 0;
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  gDecodeFillByte = 3;

  Dma_Fill32Cell(fillWord, 0, ring, 4);
  Dma_Fill32Cell(fillWord, 0, layers, sizeof(layers));

  spriteId = Ui_FindNextSpriteWithImage(-1, 1);
  for (i = 0; i <= 3; i++) {
    layers[i].animation = 1;
    layers[i].priority = 1;
    layers[i].number = spriteId;
  }
  layers[0].on = 1;
  gDebugMode = 2;
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // A green backdrop (BGR555 green 15).
  PLTT_BACKDROP = 0x01E0;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
  Fade_FromBlack(1);

  // Rebuilt from here whenever layer 0's sprite changes or Select reloads
  // the resources.
restart:
  Runtime_InitializeHeap();
  Scheduler_ResetTaskTable();
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  VramCache_Reset();
  ObjectSystem_Initialize(2);
  ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, layers[0].number, 0);
  spriteMetadata = Resource_GetMetadataRecordFar(layers[0].number);
  if (spriteMetadata->drawKind == 20) {
    ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, layers[0].number + 1, 0);
  }

  // Ten copies of the sprite; for a sprite of draw kind 20 every other copy
  // shows its second part, number + 1, from resource slot 1.
  for (i = 0; i <= 9; i++) {
    spriteMetadata = Resource_GetMetadataRecordFar(layers[0].number);
    spriteWork.resourceOffset = 0;
    if (spriteMetadata->drawKind == 20 && (i & 1) != 0) {
      spriteWork.resourceOffset = 1;
    }
    spriteWork.sprite =
        ResourceObject_Create(layers[0].number + spriteWork.resourceOffset +
                              (spriteWork.resourceOffset << 12));
    ResourceMetadata_Register(spriteWork.sprite, layers[1].number);
    ResourceMetadata_Register(spriteWork.sprite, layers[2].number);
    ResourceMetadata_Register(spriteWork.sprite, layers[3].number);
    /* FAKEMATCH(temporary): the native adds the flags offset to the sprite's
     * own register (r5) in place; as spriteWork.sprite->flags = shadowsEnabled,
     * 21 of 672 instructions differ in tbs-en. */
    spriteWork.flags = &spriteWork.sprite->flags;
    *spriteWork.flags = shadowsEnabled;
  }

  for (i = 0; i <= 3; i++) {
    if (layers[i].on != 0) {
      Ui_SetGridColumnPriority(i, layers[i].priority);
    } else {
      Ui_SetGridColumnPriority(i, 8);
    }
    Ui_SetGridColumnParameter(i, layers[i].param);
    Ui_FillGridColumnFromMetadata(i, layers[i].animation);
  }

  Map_BuildProbeRing(x, y, ring);
  // Order 3200 (0xc80), where the field camera runs.
  Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
  WaitFrames(1);
  heldKeys = &gKeysHeld;
  repeatedKeys = &gKeysRepeat;
  for (;;) {

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
        selectedLayer = (selectedLayer - 1) & 3;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        selectedLayer = (selectedLayer + 1) & 3;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        editMode = (editMode - 1) & 3;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        editMode++;
        editMode &= 3;
      }
    }

    if ((gKeysPressed & KEY_START) != 0) {
      sprites = gSpriteObjects;
      shadowsEnabled ^= 1;
      for (i = 0; i <= 9; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    // Mode 0, the animation: layer 1 follows layer 0's and is not edited on
    // its own; 0 to 99.
    case 0:
      if (selectedLayer == 1) {
        break;
      }
      if ((gKeysRepeat & KEY_L) != 0) {
        layers[selectedLayer].animation--;
        if (layers[selectedLayer].animation < 0) {
          layers[selectedLayer].animation = 0;
        }
        if (selectedLayer == 0) {
          layers[1].animation = layers[0].animation;
        }
        goto next;
      }
      if ((gKeysRepeat & KEY_R) == 0) {
        break;
      }
      layers[selectedLayer].animation++;
      if (layers[selectedLayer].animation > 99) {
        layers[selectedLayer].animation = 99;
      }
      if (selectedLayer == 0) {
        layers[1].animation = layers[0].animation;
      }
      goto next;

    case 1:
      changed = 0;
      spriteSteps = 1;
      if ((*heldKeys & KEY_START) != 0) {
        spriteSteps = 10;
      }
      if ((*repeatedKeys & KEY_L) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].number =
              Ui_FindNextSpriteWithImage(layers[selectedLayer].number, -1);
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].number =
              Ui_FindNextSpriteWithImage(layers[selectedLayer].number, 1);
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
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnNumber(selectedLayer, layers[selectedLayer].number);
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
        Ui_SetGridColumnParameter(selectedLayer, layers[selectedLayer].param);
        Ui_FillGridColumnFromMetadata(selectedLayer,
                                      layers[selectedLayer].animation);
      }
      goto next;

    case 2:
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
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
      }
      goto next;

    case 3:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].param--;
        if (layers[selectedLayer].param < 0) {
          layers[selectedLayer].param = 15;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].param++;
        if (layers[selectedLayer].param > 15) {
          layers[selectedLayer].param = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnParameter(selectedLayer, layers[selectedLayer].param);
      }
      goto next;

    default:
      break;
    }

    if ((*repeatedKeys & KEY_A) != 0) {
      if (editMode == 0) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].on != 0) {
            Ui_FillGridColumnFromMetadata(selectedLayer,
                                          layers[selectedLayer].animation);
            if (selectedLayer == 0) {
              Ui_FillGridColumnFromMetadata(1, layers[1].animation);
            }
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].on ^= 1;
        Ui_SetGridColumnPriority(
            selectedLayer,
            layers[selectedLayer].on != 0 ? layers[selectedLayer].priority : 8);
        goto next;
      }
    }

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
    if (spriteId < 0) {
      spriteId = 0x200;
      continue;
    }
    {
      /* FAKEMATCH(temporary): the bound in a local compares as cmp against
       * 0x200 with blt, as native; as spriteId >= 0x200, 3 of 25 instructions
       * differ in tbs-en. */
      s32 spriteLimit = 0x200;
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
    for (; facingIndex < 10; facingIndex++)
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
    for (; facingIndex < 10; facingIndex++)
      positions++;
    break;
  case 4:
  case 6:
    for (facingIndex = 0; facingIndex < 10; facingIndex++) {
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
    for (facingIndex = 4; facingIndex < 10; facingIndex++)
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
    layerIndex &= 3;                                                           \
    for (spriteIndex = 0; spriteIndex < 10; spriteIndex++) {                   \
      spriteGroup->entries[layerIndex]->field = value;                         \
      spriteGroup++;                                                           \
    }                                                                          \
  }

/// Sets the draw parameter or the priority of one layer of the viewer's ten
/// sprites.
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
    struct AnimationEntry *entry = spriteGroup->entries[layerIndex & 3];
    struct AnimationMetadata *metadata;
    s32 initialFrame = 0;

    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (animationIndex < metadata->animationCount) {
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[animationIndex];
        entry->timer = spriteIndex * 0x10;
        entry->step = 0x10;
        entry->pos = initialFrame;
        entry->frameBase = initialFrame;
        entry->frame = 0xff;
      }
      spriteGroup->offsetY = metadata->adjustY;
      spriteGroup->rotation = initialFrame;
    }
    spriteIndex++;
    spriteGroup++;
  } while (spriteIndex <= 9);
}

/// Sets spriteId on layerIndex of each of the viewer's ten sprites.
/// Resets each layer from its new metadata. Called by Ui_RunIconMonitor when
/// a layer other than 0 gets a new sprite.
void Ui_SetGridColumnNumber(s32 layerIndex, s32 spriteId) {
  struct AnimationObject *spriteGroup = gSpriteObjects;
  s32 spriteIndex;

  // The ROM looks the metadata up here and drops the result.
  Resource_GetMetadataRecordFar(spriteId);
  layerIndex &= 3;
  for (spriteIndex = 0; spriteIndex < 10; spriteIndex++) {
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
  case 20:
    angle = 0;
    step = 0;
    pairedFacingStep = 0x8000;
    count = 4;
    break;
  default:
    angle = 0x2000;
    step = 0x4000;
    count = 4;
    break;
  }

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
