#include "types.h"
#include "object_runtime.h"

struct SceneState {
    u8 unknown_000[0x164];
    s16 first_id;
    s16 end_id;
    u16 tile_entry;
    s8 redraw_a;
    s8 redraw_b;
    s16 origin_x;
    s16 origin_z;
};

extern struct SceneState gSceneState;

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004380(s32 x, s32 z, s32 a);

void Func_020044d0(s32 value)
{
    struct SceneState *state = &gSceneState;
    s32 id;

    for (id = state->first_id; id < state->end_id; id++) {
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02004380((object->x >> 20) - state->origin_x, (object->z >> 20) - state->origin_z, value);
    }
}
