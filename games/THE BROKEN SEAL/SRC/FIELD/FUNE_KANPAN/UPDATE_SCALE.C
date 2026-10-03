#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
#include "CALL.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 Math_RemainderUnsigned(s32, s16);

/* Briefly stretches the sprite, then waits before the next pulse. */
s32 SceneActor_UpdateScalePulse(struct FieldActor *actor)
{
    /* FAKEMATCH: In-place signed accesses preserve the countdown's load
     * and address scheduling. */
    switch (*(s16 *)&actor->unknown_64) {
    case 6:
        actor->scale_x += -0x4000;
        actor->scale_y += 0x2000;
        break;
    case 4:
        actor->scale_x += 0x2000;
        /* The loader relocates the stored pool word to -0x1000. */
        actor->scale_y -= 0x1000;
        break;
    case 2:
        actor->scale_x += 0x1000;
        actor->scale_y += -0x800;
        break;
    case 0:
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
        (*(s16 *)&actor->unknown_64) =
            (s16)(Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
        break;
    }
    (*(s16 *)&actor->unknown_64)--;
    return 1;
}

s32 SceneState_ApplyArgMode1AndReturnZero(s32 a)
{
    Engine_ActorSetSpriteFlags(a, 1);
    return 0;
}

/* A gull hovering over the deck: its flight step, its drift and the
   directions it drifts in. */
struct HoverGull {
    u8 unknown_00[0x4c];
    s32 drift;
    u8 unknown_50[0x12];
    u8 step;
    u8 right_side;
    s16 drifting_left;
    s16 sinking;
};

#define GULL(obj) ((struct HoverGull *)(obj))

/* The first hovering gull: it drifts about over the deck, and one frame in
 * a hundred starts down to land beside actor 21, who turns to it and hops
 * or starts back; then it flies up to hover again. */
s32 FuneKanpan_UpdateHoverGullA(struct FieldActor *obj)
{
    if (GULL(obj)->step != 0) {
        switch (GULL(obj)->step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0x10c0000, 0x140000, 0x2b40000);
            GULL(obj)->step++;
            break;
        case 2:
            GULL(obj)->step++;
            break;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x
                && obj->target_z == obj->target_y) {
                GULL(obj)->step++;
                Audio_PlayCue(146);
                if (GULL(obj)->right_side != 0)
                    Actor_FaceDirection(21, 0xd000, 0);
                else
                    Actor_FaceDirection(21, 0xb000, 0);
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Actor_Get(21)->velocity_y = 0x20000;
                } else {
                    Actor_ShowEmote(21, 0x103, 0);
                    Actor_Get(21)->velocity_y = 0x60000;
                }
            }
            break;
        case 4:
            GULL(obj)->step++;
            break;
        case 5:
            if (GULL(obj)->right_side != 0)
                Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0x11a0000, 0, 0x2920000);
            else
                Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0xfe0000, 0, 0x29c0000);
            GULL(obj)->step++;
            break;
        case 6:
            GULL(obj)->step++;
            break;
        case 7:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x
                && obj->target_z == obj->target_y) {
                obj->speed = 0x20000;
                obj->acceleration = 0x10000;
                GULL(obj)->drifting_left = 0;
                GULL(obj)->sinking = 0;
                GULL(obj)->step++;
                GULL(obj)->drift = 0;
            }
            break;
        case 8:
            GULL(obj)->step = 0;
            break;
        }
    } else {
        if (GULL(obj)->drifting_left != 0) {
            GULL(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
            if (GULL(obj)->drift < -0x4000)
                GULL(obj)->drifting_left = 0;
        } else {
            GULL(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
            if (GULL(obj)->drift > 0x4000)
                GULL(obj)->drifting_left = 1;
        }
        if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000)
            obj->x.fixed += GULL(obj)->drift;
        if (GULL(obj)->sinking != 0) {
            obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
            if (obj->y.fixed < 0)
                GULL(obj)->sinking = 0;
        } else {
            obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
            if (obj->y.fixed > 0x80000)
                GULL(obj)->sinking = 1;
        }
    }
    if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
        GULL(obj)->step = 1;
    return 1;
}

/* The second gull does the same beside actor 22. */
s32 FuneKanpan_UpdateHoverGullB(struct FieldActor *obj)
{
    if (GULL(obj)->step != 0) {
        switch (GULL(obj)->step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0x1000000, 0x140000, 0x2800000);
            GULL(obj)->step++;
            break;
        case 2:
            GULL(obj)->step++;
            break;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x
                && obj->target_z == obj->target_y) {
                GULL(obj)->step++;
                Audio_PlayCue(146);
                if (GULL(obj)->right_side != 0)
                    Actor_FaceDirection(22, 0xd000, 0);
                else
                    Actor_FaceDirection(22, 0xb000, 0);
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Actor_Get(22)->velocity_y = 0x20000;
                } else {
                    Actor_ShowEmote(22, 0x103, 0);
                    Actor_Get(22)->velocity_y = 0x60000;
                }
            }
            break;
        case 4:
            GULL(obj)->step++;
            break;
        case 5:
            if (GULL(obj)->right_side != 0)
                Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0x1080000, 0, 0x2580000);
            else
                Call4((void (*)())Engine_ObjectSetPosition, (s32)obj, 0xf20000, 0, 0x25c0000);
            GULL(obj)->step++;
            break;
        case 6:
            GULL(obj)->step++;
            break;
        case 7:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x
                && obj->target_z == obj->target_y) {
                obj->speed = 0x20000;
                obj->acceleration = 0x10000;
                GULL(obj)->drifting_left = 0;
                GULL(obj)->sinking = 0;
                GULL(obj)->step++;
                GULL(obj)->drift = 0;
            }
            break;
        case 8:
            GULL(obj)->step = 0;
            break;
        }
    } else {
        if (GULL(obj)->drifting_left != 0) {
            GULL(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
            if (GULL(obj)->drift < -0x4000)
                GULL(obj)->drifting_left = 0;
        } else {
            GULL(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
            if (GULL(obj)->drift > 0x4000)
                GULL(obj)->drifting_left = 1;
        }
        if (obj->x.fixed > 0xe80000 && obj->x.fixed < 0x1100000)
            obj->x.fixed += GULL(obj)->drift;
        if (GULL(obj)->sinking != 0) {
            obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
            if (obj->y.fixed < 0)
                GULL(obj)->sinking = 0;
        } else {
            obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
            if (obj->y.fixed > 0x80000)
                GULL(obj)->sinking = 1;
        }
    }
    if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
        GULL(obj)->step = 1;
    return 1;
}
