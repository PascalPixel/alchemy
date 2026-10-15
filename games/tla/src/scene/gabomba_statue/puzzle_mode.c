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
    u8 unknown_160[4];
    s16 statue_id;
    u8 unknown_166[2];
    u16 tile_entry;
};

extern struct SceneState gSceneState;
extern const u8 Data_02005ad4[];
extern const u8 Data_02006ea8[];
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004288(s32 id, s32 value);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
void Func_020044b4(s32 mode)
{
    Object_SetModeById(gSceneState.statue_id, mode);
}
