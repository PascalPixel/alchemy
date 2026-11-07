// Matches: differ 0 of 665 (tla-en, 2026-11-07).
// History: 18 (11-01), 2 (11-05); matched without asm barriers through a
// once-only block, a palette pointer local and an address-sum spelling.
// The debug sprite viewer, The Broken Seal's SpriteViewer_Run
// (games/tbs/src/field/field/sprite_viewer.c), which never returns. This
// game's changes: a blue backdrop; L and R step the sprite number by ten in
// one call with Start held; Select passes only the first resource set (no B
// choice) to System_ReturnZero; a press of B cycles the backdrop through eight
// colours (gData_0802f054); a press of Start toggles the shadows of the ten
// copies, whose sprites sit 56 bytes apart from heap slot 4.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "io_reg.h"
#include "dma.h"

struct SpriteViewerLayer {
  s16 spriteId;
  s16 animationIndex;
  u8 enabled;
  s8 priority;
  s8 colorMode;
};

struct ViewerMetadata {
  u8 unk00[4];
  u8 drawKind; ///< 20 for a sprite drawn in two parts
};

struct ViewerSprite {
  u8 unk00[0x1a];
  u8 flags;
  u8 unk1b[0x1d];
};

extern u8 gDecodeFillByte;
extern u8 gDebugMode;
extern u32 gMapCellBuffer[];
extern u32 Data_02018000[];
extern u8 gResource_Data012[];
extern const u16 gData_0802f054[8];

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void Heap_Initialize(void);
void Task_ResetAll(void);
void Task_Add(void (*func)(void), s32 order);
void VramCache_Reset(void);
void Func_080230e0(s32 mode);
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
struct ViewerMetadata *Resource_GetMetadataRecordFar(s32 id);
struct ViewerSprite *ResourceObject_Create(s32 id);
s32 ResourceMetadata_Register(struct ViewerSprite *sprite, s32 id);
void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value);
void SpriteViewer_SetGridColumnNumber(s32 slot, s32 value);
s32 Func_0802e444(s32 spriteId, s32 direction);
void Func_0802e4c8(s32 x, s32 y, void *ring);
void Battle_PlaceActorsByFormationKind(void);
void Fade_FromBlack(s32 vblanks);
void wait(s32 frames);
s32 System_ReturnZero(s32 resource, void *table);

extern char ResourceId_Data012;

// The native reads the held keys afresh at each test.
#define HELD (*(volatile u32 *)&gInput.held)

void Func_0802de8c(void) {
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
  spriteId = Func_0802e444(-1, 1);
  for (i = 0; i < 4; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = 2;
  REG_BLDCNT = 0x3f42;
  *(volatile u16 *)0x05000000 = 0x7c00;
  REG_DISPCNT = 0x1140;
  Fade_FromBlack(1);

restart:
  Heap_Initialize();
  Task_ResetAll();
  ring = HeapSlot_AllocEwram(36, 160);
  VramCache_Reset();
  Func_080230e0(2);
  ResourceSlot_Load(0, gMapCellBuffer, layers[0].spriteId, 0);
  if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20)
    ResourceSlot_Load(1, Data_02018000, layers[0].spriteId + 1, 0);
  for (i = 0; i < 10; i++) {
    struct ViewerSprite *sprite;
    s32 part = 0;

    if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20 &&
        (i & 1))
      part = 1;
    sprite = ResourceObject_Create(layers[0].spriteId + part + (part << 12));
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

      // Once-only block: the animation index is read before i moves into r0.
      do {
        a = layers[i].animationIndex;
      } while (0);
      SpriteViewer_StartStaggeredAnimations(i, a);
    }
  }
  Func_0802e4c8(x, y, ring);
  Task_Add(Battle_PlaceActorsByFormationKind, 0x480);

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
      Func_0802e4c8(x, y, ring);
    } else {
      (void)gInput.repeat;
      if (gInput.repeat & 64)
        selectedLayer = (selectedLayer - 1) & 3;
      if (gInput.repeat & 128)
        selectedLayer = (selectedLayer + 1) & 3;
      if (gInput.repeat & 32)
        editMode = (editMode - 1) & 3;
      if (gInput.repeat & 16)
        editMode = (editMode + 1) & 3;
    }

    if (gInput.pressed & 8) {
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
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].animationIndex--;
        if (layers[selectedLayer].animationIndex < 0)
          layers[selectedLayer].animationIndex = 0;
        if (selectedLayer == 0)
          layers[1].animationIndex = layers[0].animationIndex;
        goto next;
      }
      if (!(gInput.repeat & 0x100))
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
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].spriteId =
            Func_0802e444(layers[selectedLayer].spriteId, -steps);
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].spriteId =
            Func_0802e444(layers[selectedLayer].spriteId, steps);
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
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0)
          layers[selectedLayer].priority = 3;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
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
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0)
          layers[selectedLayer].colorMode = 15;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
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

    if (gInput.repeat & 1) {
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
    if (gInput.repeat & 4) {
      Task_ResetAll();
      System_ReturnZero((s32)&ResourceId_Data012, gResource_Data012);
      goto restart;
    }
    if (gInput.pressed & 2) {
      volatile u16 *palette = (volatile u16 *)0x05000000;
      const u16 *colors = gData_0802f054;
      backdrop++;
      backdrop &= 7;
      // Spelled as an address sum: colors[backdrop] swaps the load's registers.
      *palette = *(const u16 *)((u32)colors + (backdrop << 1));
    }
    wait(1);
  }
}
