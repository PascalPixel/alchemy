#include "types.h"
#include "animspr.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct PairScene {
    u16 angle;   // 四方向
    u16 angle8;  // 八方向
    s32 x;
    s32 y;
    s32 z;
    struct ObjectRuntime *main_object;
    struct ObjectRuntime *child;
    s16 first_object_id;
    s16 second_object_id;
    u8 unknown_1c[0x1c];
    void *saved_callback;
    s32 *saved_script;
    u8 saved_palette;
    u8 unknown_41[0x6d7];
    u8 saved_terrain;
    u8 saved_priority;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);

void BattleFx_SetupObjectPair(s32 user_id, s32 target_id)
{
    struct ObjectRuntime *user;
    struct ObjectRuntime *target;
    struct PairScene *scene;

    scene = Ram_HeapSlots->effect_work;
    scene->first_object_id = user_id;
    user = ObjectTable_Get((s16)user_id);
    scene->second_object_id = target_id;
    scene->main_object = user;
    target = ObjectTable_Get((s16)target_id);
    scene->angle = (user->angle + 0x2000) & 0xc000; // 四方向にまるめる
    scene->angle8 = (user->angle + 0x1000) & 0xe000;
    scene->child = target;
    if (user != NULL) {
        u8 terrain;

        /* FAKEMATCH: the one-pass block reads the terrain byte before the scene offset is started; stored directly the offset is started first (differ 2), and an empty asm barrier moves the literal pool ahead of this block. */
        do {
            terrain = user->priority_flags;
        } while (0);

        scene->saved_terrain = terrain;
        scene->saved_priority = (u32)(((u8 *)user->animation)[9] << 28) >> 30;
    }
    if (target != NULL) {
        // 対象の動きと色を、あとで戻すために取っておく
        scene->saved_callback = target->update;
        scene->saved_script = target->script;
        scene->saved_palette = ((struct AnimationObject *)target->animation)->entries[0]->param;
        scene->x = target->x;
        scene->z = target->z;
        scene->y = target->y;
    } else {
        scene->x = user->x;
        scene->z = user->z;
        scene->y = user->y;
        Vector_AddPolarOffset(0x100000, scene->angle, &scene->x);
    }
}
