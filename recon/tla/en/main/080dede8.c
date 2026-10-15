/* Near miss, differ 206 of 409 (2026-10-15): ⚓️'s RunBattleEffect05, the
 * form of ☀️'s: an orb grows from the target to the effect's point over
 * eleven frames, sheds falling motes (10 or 24) or drifting ones (10 or
 * 30), and shrinks back. The native's frame is 52 bytes, this one 56 (one
 * more spill slot, 411 instructions against 409); the work and the anchor
 * take each other's registers (the native keeps the work in r8 and the
 * anchor in sl), and the order follows from there. Belongs between
 * fall.c's BattleFx_CallEffect05 and particles.c's Func_080df174. */
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "object_runtime.h"
#include "event_work.h"
#include "ram_buffer.h"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct Effect05Sprite {
    u32 unknown_00[2];
    u8 unknown_08;
    u32 unknown_09_0 : 2;
    u32 mode : 2;
    u32 unknown_09_4 : 4;
};

struct Effect05State {
    u16 angle;
    u8 unknown_02[2];
    s32 x;
    s32 y;
    s32 z;
    struct ObjectRuntime *target;
    struct ObjectRuntime *anchor;
    s16 first_object_id;
    s16 second_object_id;
    u8 unknown_1c[4];
    u8 high_arc;
    u8 unknown_21[0x13];
    u8 variant;
    u8 unknown_35[0x0c];
    u8 alternate;
};

u32 Random16(void);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position);
s32 Func_08020330(struct ObjectRuntime *object);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void BattleFx_LandFallingObject(struct ObjectRuntime *object);
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object);

static __inline__ s32 Lerp10(s32 start, s32 end, s32 step)
{
    return start + step * (end - start) / 10;
}

void RunBattleEffect05(void)
{
    struct EffectVector pos;
    struct EffectVector start;
    struct EffectVector end;
    struct Effect05State *work = Ram_HeapSlots->effect_work;
    struct ObjectRuntime *anchor = work->anchor;
    struct ObjectRuntime *target = work->target;
    struct ObjectRuntime *orb;
    s32 raised;
    struct EffectVector *from;
    struct EffectVector *to;
    s32 i;
    s32 steps = 11;
    s32 count;
    s32 y;
    s32 n;
    struct ObjectRuntime *mote;
    struct PendingEvent *ev;

    orb = Object_Spawn(300, i = raised = 0, 0, 0);
    if (orb == 0)
        return;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(138);
    if (work->anchor == 0) {
        work->x = target->x;
        work->z = target->z;
        Vector_AddPolarOffset(16 << 16, work->angle, (struct EffectVector *)&work->x);
    }
    from = &start;
    from->x = target->x;
    from->y = target->y + (16 << 16);
    from->z = target->z;
    to = &end;
    to->x = work->x;
    y = work->y;
    to->y = y + (32 << 16);
    to->z = work->z;
    if ((s8)work->variant != 0)
        to->y = y + (80 << 16);
    if (Func_08020330(anchor) == 380) {
        to->y += 32 << 16;
        raised = 1;
    }

    for (; i < steps; i++) {
        s32 scale;

        {
            struct EffectVector *a = &start;
            struct EffectVector *b = &end;
            s32 e, t;

            e = b->x;
            t = a->x;
            t += (e - t) * i / 10;
            orb->x = t;
            e = b->y;
            t = a->y;
            t += (e - t) * i / 10;
            orb->y = t;
            e = b->z;
            t = a->z;
            t += (e - t) * i / 10;
            orb->z = t;
        }
        scale = Lerp10(0x4000, 0x10000, i);
        orb->scale_x = scale;
        orb->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if ((s8)work->alternate == 0) {
        count = 10;
        if ((s8)work->high_arc == 0)
            count = 24;
        for (n = 0; n < count; n++) {
            pos.x = orb->x;
            pos.y = orb->y;
            pos.z = orb->z;
            Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
            if (n == count - 1) {
                WaitFrames(20);
                pos.x = orb->x;
                pos.y = orb->y;
                pos.z = orb->z;
            }
            mote = Object_Spawn(301, pos.x, pos.y, pos.z);
            if (mote != 0) {
                anchor = work->anchor;
                if (anchor != 0) {
                    mote->terrain_height = anchor->y;
                    if (raised == 1)
                        mote->terrain_height = anchor->y + (32 << 16);
                } else {
                    mote->terrain_height = pos.y - (32 << 16);
                }
                mote->update = (void (*)(void))BattleFx_LandFallingObject;
                mote->motion_flags = 2;
                mote->priority_flags = 5;
            }
            Audio_PlayCue(132);
            WaitFrames(6);
            if (n == 12) {
                ev = Func_080ce458(0x50000005, 5, Func_080ce31c(5));
                if (ev != 0)
                    EventRuntime_RunPendingEvent(ev, work->first_object_id, work->second_object_id);
            }
        }
        WaitFrames(10);
    } else {
        if ((s8)work->high_arc != 0)
            count = 10;
        else
            count = 30;
        for (n = 0; n < count; n++) {
            pos.x = orb->x;
            pos.y = orb->y;
            pos.z = orb->z;
            Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
            mote = Object_Spawn(672, pos.x, pos.y, pos.z);
            if (mote != 0) {
                mote->update = (void (*)(void))BattleFx_UpdateDriftingFallObject;
                mote->motion_flags = 0;
                ((struct Effect05Sprite *)mote->animation)->mode = 2;
                Object_SetMode(mote, 8);
                Animation_ApplyChildValuesFar((struct DispatchObject *)mote, 7);
            }
            WaitFrames(6);
        }
        WaitFrames(70);
    }

    i = 0;
    do {
        s32 scale;

        {
            struct EffectVector *a = &end;
            struct EffectVector *b = &start;
            s32 e, t;

            e = b->x;
            t = a->x;
            t += (e - t) * i / 10;
            orb->x = t;
            e = b->y;
            t = a->y;
            t += (e - t) * i / 10;
            orb->y = t;
            e = b->z;
            t = a->z;
            t += (e - t) * i / 10;
            orb->z = t;
        }
        scale = Lerp10(0x10000, 0x4000, i);
        orb->scale_x = scale;
        orb->scale_y = scale;
        WaitFrames(1);
        i++;
    } while (i < steps);
    Object_Destroy((struct DispatchObject *)orb);
    BattleFx_PrepareBufferInterpolation();
}
