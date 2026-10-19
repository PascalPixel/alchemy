#include "types.h"

struct SceneState {
  u8 unk000[0x168];
  u16 tileEntry;
};

extern struct SceneState gSceneState;
extern const u8 gField_CachedTilesA[];

void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 handle, s32 size, void *source);
void Func_020055cc(const u8 *data, void *buffer);

static __inline__ void *Mem_Alloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static __inline__ void Vram_Load(s32 handle, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(handle, size, source);
}

void Func_020038ac(void) {
  struct SceneState *state = &gSceneState;
  void *buffer = Mem_Alloc(0xc00);
  s32 handle;

  Func_020055cc(gField_CachedTilesA, buffer);
  handle = Resource_FindFreeEntry();
  Vram_Load(handle, 0xc00, buffer);
  state->tileEntry = handle;
  MemFree(buffer);
}
