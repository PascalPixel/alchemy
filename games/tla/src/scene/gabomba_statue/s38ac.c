#include "types.h"
#include "event_inline.h"

struct SceneState {
  u8 unk000[0x168];
  u16 tileEntry;
};

extern struct SceneState gSceneState;
extern const u8 gField_CachedTilesA[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 Resource_FindFreeEntry(void);
void Func_020055cc(const u8 *data, void *buffer);

static __inline__ void *Mem_Alloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

void Func_020038ac(void) {
  struct SceneState *state = &gSceneState;
  void *buffer = Mem_Alloc(0xc00);
  s32 handle;

  Func_020055cc(gField_CachedTilesA, buffer);
  handle = Resource_FindFreeEntry();
  VramCache_Load(handle, 0xc00, buffer);
  state->tileEntry = handle;
  Heap_Release(buffer);
}
