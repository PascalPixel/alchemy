#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

struct SceneState {
  u8 unk000[0x160];
  s32 count;
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  u8 unk170[0x80];
};

extern struct SceneState gSceneState;

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020038ac(void);
void Func_02003974(void);
s32 Func_02003a64(s32 value);
void Func_02004288(s32 id, s32 value);
void Func_02004710(void);

s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id) {
  struct SceneState *state = &gSceneState;
  s32 count;

  if (GameFlag_Test(0x109)) {
    Func_020038ac();
    Func_02003974();
    count = state->count;
    Scheduler_AddOrUpdateCallback(Func_02004710, 1152);
  } else {
    Iwram_ClearWords(state, sizeof(struct SceneState));
    state->originX = x;
    state->originZ = z;
    Func_020038ac();
    count = Func_02003a64(value);
    Func_02004288(id, 0);
    Scheduler_AddOrUpdateCallback(Func_02004710, 1152);
  }
  Object_GetById(id)->unk5c[0] = 2;
  return count;
}
