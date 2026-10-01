/*
 * Draft of resource_3a5 0x02008e2c (Func_02000e2c), between SABAKU2 and
 * SABAKU3 of games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU; the range links
 * as disassembly (section .text.x02008e2c of the overlay listing).
 *
 * 2026-10-01 (matcher 2): 300 (24 register-only, 3 operand, 2 reordered)
 * against the listing with the overlay ELF; the three operand rows are the
 * scorer adding the Thumb bit to the safe-point labels the listing's data
 * carries, not a difference in bytes. What got it here: two separate actor
 * locals for the two sand bursts (one shared local keeps the leader out of
 * r0); the 600 and the clamp's zero go through s32 locals (a halfword
 * constant becomes a pool load); the index is doubled in place before the
 * speed call; the search loop walks a byte offset under an if guard, and
 * the height read comes first so the second word is read as [base, #4].
 * Remaining: in the search loop the reference gives the countdown r7 and the
 * byte offset r6 (here r6 and r7), and in the placement the address r3 (here
 * r2); global-alloc takes left before i by priority (13 references over 54
 * insns against 7 over 34), so the reference must exclude r6 for the
 * countdown, through a preference or registers used so far. The timer
 * loop's reload registers follow from that. Declaration order changes
 * nothing (120 orders); the permuter finds nothing below 300 in ten minutes.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "GAME_STATE.H"
#include "SCENE_IDS.H"

extern u8 *gWork;
extern const s32 RamakanSabaku_SafePoints1[];
extern const s32 RamakanSabaku_SafePoints2[];
extern const s32 RamakanSabaku_SafePointsOther[];

void SceneState_SetHalfwordB030(u16 value);
s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, const s32 *position_b);
void OverlayObject_WaitUntilField12BelowLimit(struct FieldActor *object, s32 limit);

/* The desert has overcome the party: the leader rises out of the sand at
   the nearest of the area's safe points while the crossing's progress
   drains away, then drops back down. */
void RamakanSabaku_ReturnToSafePoint(void)
{
    u8 *work = gWork;
    s32 timer = 60;
    s32 best = 0xf00000;
    const s32 *points;
    s32 count;
    s32 i;
    const s32 *point;
    s32 left;
    s32 nearest;
    s32 distance;
    s16 *progress;
    s32 zero;
    s32 shown;
    u16 *shown_addr;
    struct FieldActor *leader;
    struct FieldActor *actor;
    struct EffectOptions rising;
    struct EffectOptions landing;

    GameFlag_Set(0x200);
    SceneState_SetHalfwordB030(1);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        points = RamakanSabaku_SafePoints1;
        left = 3;
    } else if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        left = 5;
        points = RamakanSabaku_SafePoints2;
    } else {
        left = 2;
        points = RamakanSabaku_SafePointsOther;
    }
    count = left;
    point = points;
    if (left != 0) {
        i = 0;
        do {
            distance = RamakanSabaku_CalculatePlanarDistance(&Actor_Get(0)->x.fixed, point);
            if (distance <= best) {
                best = distance;
                nearest = count - left;
            }
            i += 8;
            point = (const s32 *)((const u8 *)points + i);
        } while (--left != 0);
    }
    nearest *= 2;
    Actor_SetSpeed(0, 0x20000, 0x10000);
    {
        struct FieldActor *self = Actor_Get(0);
        s32 z = points[nearest + 1];

        Engine_ObjectSetPosition(self, points[nearest], 0, z);
    }
    Actor_Get(0)->velocity_y = 0x60000;
    Audio_PlayCue(152);
    OverlayObject_WaitUntilField12BelowLimit(Actor_Get(0), Actor_Get(0)->y.fixed);
    Audio_PlayCue(241);
    actor = Actor_Get(0);
    rising.type = 214;
    rising.start_scale_x = 0x8000;
    rising.start_scale_y = 0xcccc;
    rising.target_scale_x = 0x10000;
    rising.target_scale_y = 0x13333;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0, 0x1c0000, &rising);
    Actor_ShowEmote(0, 0x104, 0);
    Actor_SetAnimation(0, 18);
    do {
        shown_addr = (u16 *)(work + 0xcba);
        shown = 600;
        *shown_addr = shown;
        timer--;
        progress = &gGameState.unknown_232;
        zero = 0;
        if (*progress != 0) {
            *progress -= 5;
            if (*progress <= 0)
                *progress = zero;
            else if (timer == 0)
                timer = 1;
        }
        Task_Wait(1);
    } while (timer != 0);
    leader = Actor_Get(0);
    landing.type = 214;
    landing.start_scale_x = 0x8000;
    landing.start_scale_y = 0xcccc;
    landing.target_scale_x = 0x8000;
    landing.target_scale_y = 0x13333;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0, 0, 0, 0x1c0000, &landing);
    Audio_PlayCue(288);
    Audio_PlayCue(152);
    Actor_Get(0)->velocity_y = 0x60000;
    Actor_SetAnimation(0, 1);
    Event_Wait(10);
    *(u16 *)(work + 0xcba) = 0;
    SceneState_SetHalfwordB030(0);
}
