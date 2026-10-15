#include "types.h"
#include "animspr.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct PairScene
{
    u16                   angle;  // 四方向
    u16                   angle8; // 八方向
    s32                   x;
    s32                   y;
    s32                   z;
    struct ObjectRuntime *mainObject;
    struct ObjectRuntime *child;
    s16                   firstObjectId;
    s16                   secondObjectId;
    u8                    unk1c[0x1c];
    void                 *savedCallback;
    s32                  *savedScript;
    u8                    savedPalette;
    u8                    unk41[0x6d7];
    u8                    savedTerrain;
    u8                    savedPriority;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);

void BattleFx_SetupObjectPair(s32 userId, s32 targetId)
{
    struct ObjectRuntime *user;
    struct ObjectRuntime *target;
    struct PairScene     *scene;

    scene = Ram_HeapSlots->effectWork;
    scene->firstObjectId = userId;
    user = ObjectTable_Get((s16)userId);
    scene->secondObjectId = targetId;
    scene->mainObject = user;
    target = ObjectTable_Get((s16)targetId);
    scene->angle = (user->angle + 0x2000) & 0xc000; // 四方向にまるめる
    scene->angle8 = (user->angle + 0x1000) & 0xe000;
    scene->child = target;
    if (user != NULL)
    {
        u8 terrain;

        /* FAKEMATCH: the one-pass block reads the terrain byte before the scene offset is started; stored directly the offset is started first (differ 2), and an empty asm barrier moves the literal pool ahead of this block. */
        do
        {
            terrain = user->priorityFlags;
        } while (0);

        scene->savedTerrain = terrain;
        scene->savedPriority = (u32)(((u8 *)user->animation)[9] << 28) >> 30;
    }
    if (target != NULL)
    {
        // 対象の動きと色を、あとで戻すために取っておく
        scene->savedCallback = target->update;
        scene->savedScript = target->script;
        scene->savedPalette = ((struct AnimationObject *)target->animation)->entries[0]->param;
        scene->x = target->x;
        scene->z = target->z;
        scene->y = target->y;
    }
    else
    {
        scene->x = user->x;
        scene->z = user->z;
        scene->y = user->y;
        Vector_AddPolarOffset(0x100000, scene->angle, &scene->x);
    }
}
