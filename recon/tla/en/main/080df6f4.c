/* Near miss, differ 4 of 130 (2026-10-15): ☀️'s RunBattleEffect11 (the item-break
 * macro around BattleFx_SpawnItemBreakMode1), the same body as draft 080ddea8 and the same two
 * pairs in the fragment loop. Replaces the earlier draft at differ 95. */
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_dispatch.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct ScatterPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern const s32 gData_080f0e78[];

void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct ObjectRuntime *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object, s32 magnitude, s32 angle);
void Object_Destroy(struct ObjectRuntime *object);

void Func_080df6f4(void)
{
    struct BattleFxScene *work;
    struct ObjectRuntime *obj;
    struct ObjectRuntime *pair[2];
    struct ScatterPosition pos;
    struct ObjectRuntime **walk;
    s32 n;
    s32 dist;

    work = Ram_HeapSlots->effectWork;
    BattleEffect_InitializeSharedScene();
    pos.x = work->x;
    pos.y = work->y + 0x100000;
    pos.z = work->z;
    pair[0] = BattleFx_SpawnItemBreakMode1(pos.x + 0x200000, pos.y, pos.z, 0x8000);
    pair[1] = BattleFx_SpawnItemBreakMode1(pos.x - 0x200000, pos.y, pos.z, 0);

    WaitFrames(15);
    walk = pair;
    for (n = 0; n < 2; n++) {
        obj = *walk++;
        if (obj != 0)
            Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
    }

    Object_CommitPosition(pair[0]);
    Audio_PlayCue(134);
    for (n = 0; n < 24; n++) {
        pos.x = work->x;
        pos.y = work->y + 0x100000;
        pos.z = work->z;
        obj = Object_Spawn(673, pos.x, pos.y, pos.z);
        if (obj != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)obj, gData_080f0e78);
            obj->speedLimit = Random16() + 0x20000;
            obj->acceleration = 0x20000;
            obj->motionFlags = 0;
            dist = Random16() * 24 + 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
        }
    }

    Object_Destroy(pair[0]);
    Object_Destroy(pair[1]);
    BattleFx_PrepareBufferInterpolation();
}
