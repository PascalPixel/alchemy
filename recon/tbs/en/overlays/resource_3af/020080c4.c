/* Draft of FuneKanpan_UpdateHoverGullA, resource_3af at 0x020080c4, for
 * FIELD/FUNE_KANPAN (beside FLY_BY_21.C); FuneKanpan_UpdateHoverGullB at
 * 0x020082ec is its twin for the second gull.
 * Remaining difference: score 2155 (19 register-only, 22 operand, 7
 * reordered, 10 inserted). The control flow, calls and constants match; the
 * game keeps the step's address in r6 and the step in r8, where this swaps
 * them, so the cases' shared step++ tail is not cross-jumped and each case
 * reaches the step through r8. Tried: a step pointer, a macro cast, u32 and
 * s32 steps and the FIELD_EVENT.H services; permute reaches 1220 only with
 * an assignment inside the test and a temporary return value. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

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

/* The first hovering gull's update: drift about over the deck, and now and
 * then fly down beside actor 21, who turns and starts or hops back, then
 * fly back up to hover again. */
s32 FuneKanpan_UpdateHoverGullA(struct FieldActor *obj)
{
    u32 mode;

    mode = GULL(obj)->step;
    if (mode != 0) {
        switch (mode) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Engine_ObjectSetPosition(obj, 0x10c0000, 0x140000, 0x2b40000);
            GULL(obj)->step++;
            break;
        case 2:
        case 4:
        case 6:
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
        case 5:
            if (GULL(obj)->right_side != 0)
                Engine_ObjectSetPosition(obj, 0x11a0000, 0, 0x2920000);
            else
                Engine_ObjectSetPosition(obj, 0xfe0000, 0, 0x29c0000);
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
