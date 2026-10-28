#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"
#include "ramcode.h"
#include "macros.h"

s32 Render_ProjectPoint(const void *point, s32 *screen);

extern const u8 Data_0813b9a0[];
extern u8 Data_0813b9a0CodeSize[];

/// Copies the ARM vertex kernel into a heap block and runs it: count packed
/// s8 vertices through the current transform into screen coordinates.
void Render_TransformVertices(s32 arg0, s32 arg1, s32 arg2) {
  void (*routine)(s32, s32, s32);
  u32 size;

  /* FAKEMATCH: the wrapper keeps the size load after the parameter copies, as
   * in Message_Find. */
  do {
    size = (u32)Data_0813b9a0CodeSize;
  } while (0);
  routine = (void (*)(s32, s32, s32))Heap_Alloc(size);
  Dma_Set((const void *)Data_0813b9a0, routine,
          (DMA_ENABLE | DMA_32BIT) << 16 | (size >> 2), REG_DMA3);
  routine(arg0, arg1, arg2);
  Heap_Release(routine);
}

/// Projects count packed s8 vertices (four bytes each) through
/// Render_ProjectPoint into screen coordinates, four halfwords each.
void Render_ProjectVertices(s8 *vertices, s16 *screen, s32 count) {
  s32 point[3];

  for (; count != 0; count--) {
    SET_VECTOR(point, vertices[0] << 16, vertices[1] << 16, vertices[2] << 16);
    Render_ProjectPoint(point, point);
    SET_VECTOR(screen, point[0], point[1], point[2]);
    vertices += 4;
    screen += 4;
  }
}

struct PoolEntry {
  s32 head;
  s32 next;
  u8 unk08[16];
  s32 tail;
};

void *Heap_AllocEwram(s32 size);

/// Allocates count model entries plus a cleared one that ends the list.
struct PoolEntry *Render_AllocateModelList(s32 count) {
  struct PoolEntry *entries =
      Heap_AllocEwram((count + 1) * sizeof(struct PoolEntry));
  s32 i;

  for (i = 0; i < count + 1; i++) {
    entries[i].head = 0;
    entries[i].next = 0;
    entries[i].tail = 0;
  }
  return entries;
}

struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  u32 pixels;
};

u32 Heap_GetRemainingIwram(void);
void *Heap_Alloc(s32 size);

/// Copies a texture's pixels into a heap block when IWRAM has room for them;
/// returns 1 with copy set up, 0 when there is no room.
s32 Render_CopyTextureToIwram(struct Texture *source, struct Texture *copy) {
  s32 size = (1 << source->logWidth) << source->logHeight;
  u32 pixels;
  s32 misalign;
  s32 mask;

  if (Heap_GetRemainingIwram() < size + 8)
    return 0;
  *(u16 *)copy = *(u16 *)source;
  copy->pixels = (u32)Heap_Alloc(size + 8);
  /* FAKEMATCH: ⚓️ stores the allocation before it starts the mask; without the
   * barrier the forced mask below is scheduled before the store */
  asm volatile("");
  mask = 4;
  /* FAKEMATCH: ⚓️ starts the -4 mask before it loads the pixels; as a plain
   * constant the mask starts after the misalignment is added (differ 2) */
  asm volatile("" : "+l"(mask));
  pixels = source->pixels;
  misalign = pixels & 3;
  Iwram_CopyWords((void *)copy->pixels, (void *)(pixels - misalign),
                  (size + misalign + 3) & -mask);
  copy->pixels += misalign;
  return 1;
}

/// One entry of a model list. Render_AllocateModelList allocates the list with a cleared
/// entry after the last.
struct Model {
  s32 kind; ///< the rasterizer kernel that draws it; 0 ends the list
  s32 flags;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  s32 fade;
  u8 unk18;
  u8 frame;
};

/// The texture stays where it is, even when IWRAM has room for a copy.
#define MODEL_TEXTURE_IN_PLACE 4

typedef void (*ModelKernel)(struct Model *model);

// The rasterizer kernels, ARM code that runs from IWRAM, with their sizes in
// bytes and, for three of them, in DMA words.
extern const u8 Data_08138fc8[];
extern const u8 Data_081391c8[];
extern const u8 Data_081393d8[];
extern const u8 Data_081395f8[];
extern const u8 Data_08139aac[];
extern const u8 Data_08139f70[];
extern const u8 Data_0813a444[];
extern const u8 Data_0813a924[];
extern const u8 Data_0813ae14[];
extern const u8 Data_0813b2ec[];
extern const u8 Data_0813b7a0[];
extern u8 Data_08138fc8CodeSize[];
extern u8 Data_081391c8CodeSize[];
extern u8 Data_081393d8CodeSize[];
extern u8 Data_081395f8CodeSize[];
extern u8 Data_08139aacCodeSize[];
extern u8 Data_08139f70CodeSize[];
extern u8 Data_0813a444CodeSize[];
extern u8 Data_0813a924CodeSize[];
extern u8 Data_0813ae14CodeSize[];
extern u8 Data_0813b2ecCodeSize[];
extern u8 Data_0813b7a0CodeSize[];
extern u8 Data_081395f8CodeWords[];
extern u8 Data_0813ae14CodeWords[];
extern u8 Data_0813b2ecCodeWords[];

/// Copies a kernel whose DMA control is a word of its own.
// Kernels 8, 9 and 10 read it from the pool, one word each, two of them equal
// yet stored twice, so it is a symbol of each kernel: written
// 0x84000000 | (size >> 2) the two fold into one pool entry, and as a number
// the size is built in three instructions.
#define LOAD_KERNEL_WORDS(to, code, bytes, words)                              \
  {                                                                            \
    (to) = Heap_Alloc((u32)(bytes));                                             \
    Dma_Set((code), (to), 0x84000000 + (u32)(words), REG_DMA3);                \
  }

/// Copies the model's texture into IWRAM unless it is to stay in place.
#define COPY_TEXTURE()                                                         \
  {                                                                            \
    if (!(model->flags & MODEL_TEXTURE_IN_PLACE) &&                            \
        Render_CopyTextureToIwram(texture, &copy))                                         \
      model->texture = &copy;                                                  \
  }

/// Draws a list of models. A run of models of one kind loads that kind's
/// rasterizer into IWRAM once, and it is freed when the kind changes. Before
/// a textured model is drawn, its texture is copied into IWRAM where there is
/// room, and the copy is freed after the model is drawn.
void Render_DrawModelList(struct Model *model) {
  ModelKernel code = NULL;
  s32 lastKind = 0;
  struct Texture copy;
  struct Texture *texture;

  for (;;) {
    texture = model->texture;
    if (model->kind != lastKind && code != NULL)
      Heap_Release(code);
    if (model->kind == 0)
      break;
    switch (model->kind) {
    case 1:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08138fc8, Data_08138fc8CodeSize);
      code(model);
      break;
    case 11:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813b7a0, Data_0813b7a0CodeSize);
      code(model);
      break;
    case 2:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_081391c8, Data_081391c8CodeSize);
      code(model);
      break;
    case 3:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_081393d8, Data_081393d8CodeSize);
      code(model);
      break;
    case 8:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_081395f8, Data_081395f8CodeSize,
                          Data_081395f8CodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 9:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_0813ae14, Data_0813ae14CodeSize,
                          Data_0813ae14CodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 10:
      if (model->kind != lastKind)
        LOAD_KERNEL_WORDS(code, Data_0813b2ec, Data_0813b2ecCodeSize,
                          Data_0813b2ecCodeWords);
      COPY_TEXTURE();
      code(model);
      break;
    case 4:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08139aac, Data_08139aacCodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 5:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_08139f70, Data_08139f70CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 6:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813a444, Data_0813a444CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    case 7:
      if (model->kind != lastKind)
        RamCode_Load(code, Data_0813a924, Data_0813a924CodeSize);
      COPY_TEXTURE();
      code(model);
      break;
    }
    lastKind = model->kind;
    if (model->texture == &copy) {
      Heap_Release((void *)(model->texture->pixels & ~3));
      model->texture = texture;
    }
    model++;
  }
}
