#include "types.h"
#include "resource.h"
#include "animspr.h"
#include "metadata_lookup.h"
#include "ram_buffer.h"
#include "mem.h"
#include "io_reg.h"
#include "dma.h"

#define ANIMATION_OBJECT_COUNT 64

s32 Animation_LookupValueByKey(s32 key);
void Palette_LoadBasicObjColors(void);

extern const u8 gData_0802e89c[];
extern u8 Render_CopyStridedCode[];
extern u8 Render_CopyStridedCodeSize[];

/// Takes the sprite and layer pools (from the fixed heap blocks in mode 3,
/// the heap otherwise) and clears them, loads the shadow tiles into VRAM cache
/// entry 93 and copies the strided tile copier into RAM.
// As ☀️'s function of the same name.
void ObjectSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  if (mode == 3) {
    sprites = Runtime_AllocateBlock(16, ANIMATION_OBJECT_COUNT *
                                            sizeof(struct AnimationObject));
    layers = Runtime_AllocateBlock(12, ANIMATION_OBJECT_COUNT *
                                           sizeof(struct AnimationEntry));
  } else {
    sprites = Runtime_AllocateHeapBlock(16, ANIMATION_OBJECT_COUNT *
                                                sizeof(struct AnimationObject));
    layers = Runtime_AllocateHeapBlock(12, ANIMATION_OBJECT_COUNT *
                                               sizeof(struct AnimationEntry));
  }
  Palette_LoadBasicObjColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramCache_Load(93, 128, gData_0802e89c);
  {
    u32 size = (u32)Render_CopyStridedCodeSize;

    Dma_Set(Render_CopyStridedCode, Runtime_AllocateHeapBlock(84, size),
            (DMA_ENABLE | DMA_32BIT) << 16 | (size >> 2), REG_DMA3);
  }
}

void VramBlock_Reset93(void) { Resource_ResetEntry(93); }

void VramBlock_Load93(void) {
  const u8 *data = gData_0802e89c;
  s32 size = 128;

  VramCache_Load(93, size, data);
}

struct AnimationEntry *AnimationObject_Allocate(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationEntry *entry;
  struct AnimationEntry *found;
  struct AnimationEntry *object;
  s32 i;
  void **pictures;
  u8 **animation;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  entry = Ram_HeapSlots->unk0c[0];
  object = NULL;

  if (metadata->width != 0) {
    for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, entry++) {
      if (entry->kind == 0) {
        found = entry;
        break;
      }
    }
    if (found != NULL) {
      /* FAKEMATCH: instruction order only: read as a word, the pictures load
       * does not wait on the animId store, so the zero for pos and param loads first
       * as native; the typed read swaps the two (differ 2). */
      pictures = (void **)*(s32 *)&metadata->pictures;
      object = found;
      object->animId = id;
      if (pictures == 0)
        pictures = (void **)Animation_LookupValueByKey(id);

      animation = metadata->animation;
      object->scripts = animation;
      object->pictures = pictures;
      object->mode = metadata->pictureCodec;
      object->picture = 0xff;
      object->script = animation[0];
      object->pos = 0;
      object->kind = metadata->drawKind;
      object->param = 0;
    }
  }
  return object;
}
