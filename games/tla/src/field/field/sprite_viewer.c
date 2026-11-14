/// @file
/// A debug sprite viewer.
// Part of field.c.

#include "../field_internal.h"
#include "fixed_math.h"

#include "system/input.h"

// The native reads the held keys afresh at each test.
#define HELD (*(volatile u32 *)&gInput.held)

/// The debug sprite viewer, in The Broken Seal's form with this game's
/// changes; it never returns.
void SpriteViewer_Run(void) {
  struct SpriteViewerLayer layers[4];
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
  editMode = 1;
  selectedLayer = 0;
  shadowsEnabled = 1;
  backdrop = 0;
  ring = HeapSlot_AllocEwram(36, 160);
  gDecodeFillByte = 3;
  Dma_Fill32Cell(zero, 0, ring, 4);
  Dma_Fill32Cell(zero, 0, layers, sizeof(layers));
  spriteId = SpriteViewer_FindNextSpriteWithPictures(-1, 1);
  for (i = 0; i < 4; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = 2;
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  *(volatile u16 *)BG_PLTT = 0x7c00;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON;
  ScreenFade_FromBlack(1);

restart:
  Heap_Initialize();
  Task_ResetAll();
  ring = HeapSlot_AllocEwram(36, 160);
  VramCache_Reset();
  ActorSystem_Initialize(2);
  SpritePictureCache_Load(0, gMapCellBuffer, layers[0].spriteId, 0);
  if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20)
    SpritePictureCache_Load(1, Data_02018000, layers[0].spriteId + 1, 0);
  for (i = 0; i < 10; i++) {
    struct ViewerSprite *sprite;
    s32 part = 0;

    if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20 &&
        (i & 1))
      part = 1;
    sprite = Sprite_Create(layers[0].spriteId + part + (part << 12));
    ResourceMetadata_Register(sprite, layers[1].spriteId);
    ResourceMetadata_Register(sprite, layers[2].spriteId);
    ResourceMetadata_Register(sprite, layers[3].spriteId);
    sprite->flags = shadowsEnabled;
  }
  for (i = 0; i < 4; i++) {
    if (layers[i].enabled != 0)
      SpriteViewer_SetGridColumnPriority(i, layers[i].priority);
    else
      SpriteViewer_SetGridColumnPriority(i, 8);
    SpriteViewer_SetGridColumnColorMode(i, layers[i].colorMode);
    {
      s32 a;

      /* FAKEMATCH(dead): a once-only block. */
      do {
        a = layers[i].animationIndex;
      } while (0);
      SpriteViewer_StartStaggeredAnimations(i, a);
    }
  }
  SpriteViewer_BuildProbeRing(x, y, ring);
  Task_Add(SpriteViewer_PlaceRing, 0x480);

next:
  wait(1);
  for (;;) {
    if (HELD & 8) {
      if (HELD & 32)
        x--;
      if (HELD & 16)
        x++;
      if (HELD & 64)
        y--;
      if (HELD & 128)
        y++;
      SpriteViewer_BuildProbeRing(x, y, ring);
    } else {
      // DEAD: the repeated keys are read once and the value goes unused.
      (void)gInput.repeat;
      if (gInput.repeat & KEY_UP)
        selectedLayer = (selectedLayer - 1) & 3;
      if (gInput.repeat & KEY_DOWN)
        selectedLayer = (selectedLayer + 1) & 3;
      if (gInput.repeat & KEY_LEFT)
        editMode = (editMode - 1) & 3;
      if (gInput.repeat & KEY_RIGHT)
        editMode = (editMode + 1) & 3;
    }

    if (gInput.pressed & KEY_START) {
      struct ViewerSprite *sprites = Ram_HeapSlots->unk0c[1];

      shadowsEnabled ^= 1;
      for (i = 0; i < 10; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    case 0:
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
      if (layers[selectedLayer].animationIndex > 99)
        layers[selectedLayer].animationIndex = 99;
      if (selectedLayer == 0)
        layers[1].animationIndex = layers[0].animationIndex;
      goto next;

    case 1: {
      s32 steps;

      changed = 0;
      steps = 1;
      if (HELD & 8)
        steps = 10;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].spriteId =
            SpriteViewer_FindNextSpriteWithPictures(
                layers[selectedLayer].spriteId, -steps);
        changed = 1;
      }
      if (gInput.repeat & KEY_R) {
        layers[selectedLayer].spriteId =
            SpriteViewer_FindNextSpriteWithPictures(
                layers[selectedLayer].spriteId, steps);
        changed = 1;
      }
      if (changed == 0)
        break;
      if (selectedLayer == 0)
        goto restart;
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
    }

    case 2:
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
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
      goto next;

    case 3:
      changed = 0;
      if (gInput.repeat & KEY_L) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0)
          layers[selectedLayer].colorMode = 15;
        changed = 1;
      }
      if (gInput.repeat & KEY_R) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > 15)
          layers[selectedLayer].colorMode = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
      goto next;
    }

    if (gInput.repeat & KEY_A) {
      if (editMode == 0) {
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
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].enabled != 0
                                               ? layers[selectedLayer].priority
                                               : 8);
        goto next;
      }
    }
    if (gInput.repeat & KEY_SELECT) {
      Task_ResetAll();
      Stub_ReturnZero((s32)&ResourceId_Data012, gResource_Data012);
      goto restart;
    }
    if (gInput.pressed & KEY_B) {
      volatile u16 *palette = (volatile u16 *)BG_PLTT;
      const u16 *colors = gData_0802f054;
      backdrop++;
      backdrop &= 7;
      // Spelled as an address sum: colors[backdrop] swaps the load's registers.
      *palette = *(const u16 *)((u32)colors + (backdrop << 1));
    }
    wait(1);
  }
}

extern const u16 gData_0802f064[];

/// Steps a number from 0 to 715 by delta: a step of one wraps around the
/// range, a larger step forward goes to the next stop above value and one back
/// to the last stop below it (gData_0802f064, ended by 0xffff, from which a
/// step forward wraps to 0). A delta of 0 returns value unchanged.
s32 SpriteViewer_FindNextSpriteWithPictures(s32 value, s32 delta) {
  s32 step;

  if (delta != 0) {
    step = delta;
    if (delta < 0)
      step = -delta;
    if (step == 1) {
      value += delta;
      if (value < 0)
        value = 715;
      if (value >= 716)
        value = 0;
    } else if (delta > 0) {
      u32 i;

      for (i = 0; i < 10; i++) {
        if (value < gData_0802f064[i]) {
          value = gData_0802f064[i];
          if (value == 0xffff)
            value = 0;
          break;
        }
      }
    } else if (value == 0) {
      value = gData_0802f064[8];
    } else {
      s32 i;

      for (i = 8; i >= 0; i--) {
        if (value > gData_0802f064[i]) {
          value = gData_0802f064[i];
          break;
        }
      }
    }
    return value;
  }
#ifdef UBFIX
  return value;
#else
  // UB: no return statement for a delta of 0; the native leaves value in r0.
#endif
}

#include "graphics/render_placement.h"

/// Lays out the sprite viewer's ring around pixel (x, z): one position per
/// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
/// pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
/// sprite). B held turns the ring. Called by SpriteViewer_Run when it starts
/// and while Start moves the ring.
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY,
                                 struct SpritePlacement *positions) {
  struct SpritePlacement *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = ((struct ProbeObjects *)Ram_HeapSlots->unk0c[1])->shape->kind;
  if ((gInput.held & KEY_B) != 0)
    angle = (s16)(gVBlankCount << 8);
  switch ((u32)kind) {
  case 3:
    for (i = 0; i < 6; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(px(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (i = 0; i < 8; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(px(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + TRIG_TURN / 8);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 4:
  case 6:
    for (i = 0; i < 10; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(px(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    turn = angle;
    angle = (s16)(turn + TRIG_TURN / 4);
    i = 0;
    pair = positions;
    while (i < 2) {
      pair[i * 2].x = centerX << 16;
      pair[i * 2].height = 0;
      pair[i * 2].y = centerY << 16;
      angle <<= 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(px(40), angle, &positions[0].x);
      pair[i * 2 + 1].x = centerX << 16;
      {
        s32 *point = &positions[1].x;
        pair[i * 2 + 1].height = 0;
        pair[i * 2 + 1].y = centerY << 16;
        Vector_AddPolarOffset(px(40), (u16)angle, point);
      }
      angle = (angle + TRIG_TURN / 2) << 16;
      i++;
      positions += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = ((angle << 16) + 0x20000000) >> 16;
    for (i = 0; i < 4; i++) {
      angle <<= 16;
      angle = (u32)angle >> 16;
      positions->x = centerX << 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(px(56), (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + TRIG_TURN / 4);
    }
    for (i = 4; i < 10; i++)
      positions++;
    break;
  }
}

#undef FIELD_AT_OFFSET
#undef HELD

void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[5] = value;
    base += 56;
  } while (count >= 0);
}

void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[6] = value;
    base += 56;
  } while (count >= 0);
}

void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value) {
  s32 index;
  s32 count;
  s32 offset;
  struct AnimationMetadata *metadata;
  struct UiGridEntry *entry;
  u8 *work;

  work = Ram_HeapSlots->unk0c[1];
  count = 0;
  offset = ((3 & slot) * 4) + 0x28;
  index = 0;
  do {
    entry = *(struct UiGridEntry **)(work + offset);
    if (entry->table0c != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->no);
      if (value < metadata->animationCount) {
        entry->value04 = metadata->drawKind;
        entry->value10 = entry->table0c[value];
        entry->x = count * 0x10;
        entry->value15 = 0x10;
        entry->value14 = index;
        entry->value17 = index;
        entry->value16 = 0xFF;
      }
      work[23] = (u8)metadata->adjustY;
      *(s16 *)(work + 18) = index;
    }
    count += 1;
    work += 0x38;
  } while (count <= 9);
}

void SpriteViewer_SetGridColumnNumber(s32 slot, s32 no) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset;
  s32 count;

  Resource_GetMetadataRecordFar(no);
  offset = (slot & 3) * 4 + 40;
  count = 9;
  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    *(u16 *)entry = no;
    SpriteAnimation_Reset(entry);
    base += 56;
  } while (count >= 0);
}

extern struct Vec2 gBattle_FormationPlacementScale;

void SpriteViewer_PlaceRing(void) {
  u8 *actor = Ram_HeapSlots->unk0c[1];
  u32 kind = (*(u8 **)(actor + 40))[4];
  struct Vec2 scale;
  u8 *table;
  u16 angle;
  u16 step;
  u16 odd = 0;
  u32 cnt;
  u32 i;

  scale = gBattle_FormationPlacementScale;
  table = (u8 *)Ram_HeapSlots->battleWork;

  switch (kind) {
  case 3:
    angle = 0;
    step = 0x2aaa;
    cnt = 6;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    angle = 0;
    step = 0x2000;
    cnt = 8;
    break;
  case 4:
  case 6:
    angle = 0;
    step = 0x1999;
    cnt = 10;
    break;
  case 20:
    angle = 0;
    step = 0;
    odd = 0x8000;
    cnt = 4;
    break;
  default:
    angle = TRIG_TURN / 8;
    step = 0x4000;
    cnt = 4;
    break;
  }

  for (i = 0; i < cnt; i++) {
    Render_ApplyProjectedPlacement(actor, (s32 *)(table + i * 16),
                                   (s32 *)&scale, angle);
    actor += 56;
    angle += step;
    if (i & 1)
      angle += odd;
  }
}
