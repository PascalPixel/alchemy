#include "types.h"
#include "object_runtime.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 statueId;
  u8 unk166[2];
  u16 tileEntry;
};

extern struct SceneState gSceneState;
extern const u8 gData_02005ad4[];
extern const u8 gData_02006ea8[];
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
void Func_020044b4(s32 mode) { Object_SetModeById(gSceneState.statueId, mode); }
