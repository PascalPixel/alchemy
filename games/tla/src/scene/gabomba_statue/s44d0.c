#include "types.h"
#include "object_runtime.h"

struct SceneState
{
    u8  unk000[0x164];
    s16 firstId;
    s16 endId;
    u16 tileEntry;
    s8  redrawA;
    s8  redrawB;
    s16 originX;
    s16 originZ;
};

extern struct SceneState gSceneState;

struct ObjectRuntime *Object_GetById(s32 id);
void                  Func_02004380(s32 x, s32 z, s32 a);

void Func_020044d0(s32 value)
{
    struct SceneState *state = &gSceneState;
    s32                id;

    for (id = state->firstId; id < state->endId; id++)
    {
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02004380(
            (object->x >> 20) - state->originX, (object->z >> 20) - state->originZ, value);
    }
}
