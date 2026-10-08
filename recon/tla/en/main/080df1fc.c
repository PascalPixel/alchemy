#include "types.h"
#include "object_dispatch.h"
#include "input.h"
#include "objdisp.h"
#include "ram_buffer.h"
#include "battle_efx.h"
#include "callback_scheduler.h"
#include "system.h"

struct FxPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct FxObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    u8 unknown_18[10];
    u8 mode;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 lift;
    u8 unknown_38[12];
    s32 gravity;
    u8 unknown_48[13];
    u8 flag;
    u8 unknown_56[4];
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c[12];
    struct FxObject *linked;
    void *callback;
};

struct EffectWork {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
    struct FxObject *source;
    struct FxObject *target;
    u8 unknown_18[0x1c];
    u8 falls;
    u8 unknown_35[3];
    void *callback;
    const u8 *script;
    u8 palette;
};

extern const u8 Data_080f0e60[];
extern const u8 Data_080f0e78[];
extern u32 Data_0300122c;

void BattleEffect_InitializeSharedScene(void);
struct FxObject *Func_080df820(s32 x, s32 y, s32 z, s32 angle);
void BattleFx_PrepareBufferInterpolation(void);
void Object_SetPosition(struct FxObject *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FxObject *object);
void ObjectGroup_ApplyRandomChildValues(void);
void Func_080df174(void);
void Audio_PlayCue(s32 cue);
void Object_SetMode(struct FxObject *object, s32 mode);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FxPosition *position);
s32 ScriptObject_CheckOverlapFar(struct FxObject *object, struct FxPosition *position);
s32 Func_08020210(struct FxObject *object, struct FxPosition *position);
struct FxObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct FxObject *object, s32 magnitude, s32 angle);
void Object_Destroy(struct FxObject *object);
void Func_080dfb0c(struct FxObject *object);

static __inline__ s32 Object_ProbeRaised(struct FxObject *object, struct FxPosition *position)
{
    s32 hit;

    object->ground += 0x100000;
    hit = Func_08020210(object, position);
    object->ground -= 0x100000;
    return hit;
}

void BattleFx_SteerLiftedTarget(s32 target_id)
{
    struct EffectWork *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxPosition position;
    struct FxObject *anchors[2];
    struct FxObject *object;
    struct FxObject *right;
    s32 i;
    s32 angle;
    s32 magnitude;
    struct FxPosition *p;

    work = Ram_HeapSlots->effect_work;
    source = work->source;
    target = work->target;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar((struct DispatchObject *)source, (u32)Data_080f0e60);
    /* FAKEMATCH: the position is reached through this pointer everywhere but
       in the steering loop's first part, which names the position itself;
       that keeps the pointer in r10 and gives the loop its own copy. */
    p = &position;
    p->x = work->x;
    p->y = work->y + 0x100000;
    p->z = work->z;
    object = Func_080df820(p->x + 0x200000, p->y, p->z, 0x8000);
    anchors[0] = object;
    right = Func_080df820(p->x - 0x200000, p->y, p->z, 0);
    anchors[1] = right;
    if (anchors[0] == NULL || right == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    WaitFrames(15);
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    Object_SetPosition(object, p->x + 0x100000, p->y, p->z);
    Object_SetPosition(right, p->x - 0x100000, p->y, p->z);
    Object_CommitPosition(object);
    Object_CommitPosition(right);
    object->x = p->x + 0x100000;
    object->velocity_x = 0;
    right->x = p->x - 0x100000;
    right->velocity_x = 0;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    Scheduler_AddOrUpdateCallback((s32)Func_080df174, 0x480);
    Audio_PlayCue(130);
    target->flag = 4;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)target, 0);
    if (anchors[0] != NULL && anchors[1] != NULL) {
        while (target->y - target->ground <= 0x180000) {
            object->y += 0x6000;
            right->y += 0x6000;
            target->y += 0x6000;
            WaitFrames(1);
        }
    }
    anchors[0]->speed = 0x40000;
    anchors[0]->lift = 0x8000;
    anchors[1]->speed = 0x40000;
    anchors[1]->lift = 0x8000;
    target->speed = 0x6666;
    target->lift = 0x3333;
    target->unknown_5a = 0;
    target->mode = 2;
    for (;;) {
        WaitFrames(1);
        if ((gInput.pressed & 0x303) != 0)
            break;
        angle = (u16)BattleFx_GetCycledTableWord(gInput.held);
        if (angle == 0xffff) {
            position.x = target->x;
            position.y = target->y + 0x100000;
            position.z = target->z;
            Object_SetPosition(anchors[0], position.x + 0x100000, position.y, position.z);
            Object_SetPosition(anchors[1], position.x - 0x100000, position.y, position.z);
            Object_SetMode(anchors[0], 1);
            Object_SetMode(anchors[1], 1);
            continue;
        }
        position.x = target->x;
        position.y = target->y + 0x100000;
        position.z = target->z;
        Vector_AddPolarOffset(0x20000, angle, &position);
        Object_SetPosition(anchors[0], position.x + 0x100000, position.y, position.z);
        Object_SetPosition(anchors[1], position.x - 0x100000, position.y, position.z);
        Object_CommitPosition(anchors[0]);
        Object_CommitPosition(anchors[1]);
        position.x = target->x;
        position.y = target->ground;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, angle, &position);
        i = ScriptObject_CheckOverlapFar(target, &position);
        /* FAKEMATCH: entering the blocked path from both tests keeps it ahead
           of the drop path, as the ROM lays them out. */
        if (i != 0)
            goto blocked;
        if (Object_ProbeRaised(target, p) > 0) {
        blocked:
            Object_SetMode(anchors[0], 4);
            Object_SetMode(anchors[1], 4);
            if ((Data_0300122c & 15) == 0)
                Audio_PlayCue(114);
            continue;
        }
        {
            struct FxObject *second;
            s32 x;
            s32 z;

            Audio_PlayCue(175);
            x = p->x;
            z = p->z;
            Object_SetMode(anchors[0], 4);
            Object_SetMode(anchors[1], 4);
            WaitFrames(15);
            target->unknown_5b = i;
            target->speed = 0x3333;
            target->lift = 0x3333;
            Object_SetPosition(target, p->x, p->y, p->z);
            anchors[0]->speed = 0x3333;
            anchors[0]->lift = 0x3333;
            second = anchors[1];
            second->speed = 0x3333;
            second->lift = 0x3333;
            Object_SetPosition(anchors[0], p->x + 0x100000, p->y, p->z);
            Object_SetPosition(second, p->x - 0x100000, p->y, p->z);
            Object_CommitPosition(target);
            target->x = x;
            target->z = z;
            target->velocity_x = i;
            target->velocity_z = i;
            WaitFrames(10);
            break;
        }
    }
    Object_SetMode(anchors[0], 4);
    Object_SetMode(anchors[1], 4);
    Scheduler_RemoveCallback((u32)Func_080df174);
    Audio_PlayCue(135);
    WaitFrames(15);
    Audio_PlayCue(135);
    WaitFrames(15);
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    {
        for (i = 0; i < 20; i++) {
            object = Object_Spawn(0x2a1, p->x, p->y, p->z);
            if (object != NULL) {
                ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)Data_080f0e78);
                object->speed = Random16() + 0x20000;
                object->lift = 0x20000;
                object->flag = 0;
                magnitude = Random16() * 24 + 0x80000;
                Motion_SetTargetPositionFromMagnitudeAngle(object, magnitude, Random16());
            }
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(anchors[0]);
    Object_Destroy(anchors[1]);
    Animation_ApplyChildValuesFar((struct DispatchObject *)target, work->palette);
    ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)(work->script));
    {
        /* FAKEMATCH: one zero, set before these stores, clears the mode
           byte and the source's callback from the same register. */
        s32 zero = 0;

        target->callback = work->callback;
        target->flag = 3;
        target->velocity_y = 0xa0000;
        target->gravity = 0x3333;
        target->mode = zero;
        source->callback = (void *)zero;
    }
    Animation_ApplyChildValuesFar((struct DispatchObject *)source, 0);
    if ((s8)work->falls != 0) {
        for (i = 0; i < 90 && target->velocity_y >= 0; i++)
            WaitFrames(1);
        WaitFrames(1);
        for (i = 0; i < 90 && target->velocity_y < 0; i++)
            WaitFrames(1);
        Func_080dfb0c(target);
        BattleFx_PrepareBufferInterpolation();
        WaitFrames(30);
    } else {
        BattleFx_PrepareBufferInterpolation();
    }
}
