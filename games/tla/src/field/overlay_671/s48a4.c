#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

struct SceneState {
    u8 unknown_000[0x160];
    s32 count;
    s16 first_id;
    s16 end_id;
    u16 tile_entry;
    s8 redraw_a;
    s8 redraw_b;
    s16 origin_x;
    s16 origin_z;
    u8 unknown_170[0x80];
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

s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id)
{
    struct SceneState *state = &gSceneState;
    s32 count;

    if (GameFlag_Test(0x109)) {
        Func_020038ac();
        Func_02003974();
        count = state->count;
        Scheduler_AddOrUpdateCallback(Func_02004710, 1152);
    } else {
        Iwram_ClearWords(state, sizeof(struct SceneState));
        state->origin_x = x;
        state->origin_z = z;
        Func_020038ac();
        count = Func_02003a64(value);
        Func_02004288(id, 0);
        Scheduler_AddOrUpdateCallback(Func_02004710, 1152);
    }
    Object_GetById(id)->unknown_5c[0] = 2;
    return count;
}
