#include "object_runtime.h"
#include "battle_effect_runtime.h"

struct GridTileCell_08093fa0 {
    u8 unknown_00[2];
    u8 kind;
    u8 unknown_03;
};

extern struct GridTileCell_08093fa0 Data_0200fe00[];
extern struct GridTileCell_08093fa0 Data_02010000[];
extern u8 Value_00000001;

struct ObjectRuntime *Object_GetById(u32 object_id);
void Func_080916b0(void);
s32 Func_08009220(const s32 *position);
void Func_08092158(s32 object_id, s32 x, s32 z);
void Func_080091e0(struct ObjectRuntime *object, s32 value);
void Func_08009080(struct ObjectRuntime *object, s32 command);
void WaitFrames(s32 frames);
void Func_08009150(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_080923c4(s32 object_id);
void Func_0809163c(s32 should_wait);
void Func_08091750(void);

s32 Func_08093fa0(void)
{
    struct BattleWork *work = &Data_02000240;
    struct ObjectRuntime *object = Object_GetById(work->object_id);
    s32 variant = 1;
    s32 tile_x = *(s16 *)((u8 *)object + 10);
    s32 tile_z = *(s16 *)((u8 *)object + 18);
    s32 tile_mask = 0xfff0;
    s32 grid_x;
    s32 grid_z;
    s32 result;

    tile_x &= tile_mask;
    tile_z &= tile_mask;
    grid_x = 8 + tile_x;
    grid_z = 8 + tile_z;

    Func_080916b0();

    if (object->animation_kind == 1) {
        variant = *((u8 *)object->animation + 0x26);
    }

    if (work->mode_1f2 == 0) {
        s32 index_x = grid_x;
        s32 index_z;
        s32 index;

        if (index_x < 0)
            index_x = tile_x + 23;
        index_x >>= 4;

        index_z = grid_z;
        if (index_z < 0)
            index_z = tile_z + 23;
        index_z >>= 4;

        index = index_x + (index_z << 7);

        if (Data_02010000[index].kind == Data_0200fe00[index].kind) {
            s32 position[6];

            position[0] = object->x;
            position[1] = object->y;
            position[2] = object->z;
            result = Func_08009220(position);
            if (result != 0) {
                goto fail;
            }

            object->action_flags = 0;
            Func_08092158(work->object_id, grid_x, grid_z);
            Func_08009080(object, 6);
            WaitFrames(4);
            Func_08009080(object, 7);
            object->velocity_y = 0x40000;
            WaitFrames(4);
            object->flags = 0;
            variant &= 0xfe;
            Func_080091e0(object, variant);
            object->speed_limit = 0x10000;
            object->velocity_y = 0;
            Func_08009080(object, 12);
            WaitFrames(4);
            work->mode_1f2 = 1;
            object->action_flags = 1;
            WaitFrames(8);
        } else {
            goto fail;
        }
    } else {
        object->flags = 0;
        Func_08009080(object, 11);
        Func_08009150(object, grid_x << 16, object->y + 0x80000,
            (grid_z << 16) + (s32)0xfff00000);
        Func_080923c4(work->object_id);
        object->flags = 3;
        {
            s32 active = (s32)&Value_00000001;

            variant |= active;
            object->terrain_height = object->y;
            Func_080091e0(object, variant);
            Func_0809163c(4);
            work->mode_1f2 = 0;
            object->action_flags = active;
        }
    }

    Func_08091750();
    return 0;

fail:
    Func_08091750();
    return -1;
}
