/*
 * Draft of resource_3a5 0x02008e2c (Func_02000e2c), between SABAKU2 and
 * SABAKU3 of games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU; the range links
 * as disassembly (section .text.x02008e2c of the overlay listing).
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
    struct FieldActor *leader;
    struct EffectOptions rising;
    struct EffectOptions landing;

    GameFlag_Set(0x200);
    SceneState_SetHalfwordB030(1);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        points = RamakanSabaku_SafePoints1;
        left = 3;
    } else if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        points = RamakanSabaku_SafePoints2;
        left = 5;
    } else {
        points = RamakanSabaku_SafePointsOther;
        left = 2;
    }
    count = left;
    point = points;
    for (i = 0; left != 0; left--) {
        distance = RamakanSabaku_CalculatePlanarDistance(&Actor_Get(0)->x.fixed, point);
        if (distance <= best) {
            best = distance;
            nearest = count - left;
        }
        i += 8;
        point = (const s32 *)((const u8 *)points + i);
    }
    nearest *= 2;
    Actor_SetSpeed(0, 0x20000, 0x10000);
    Engine_ObjectSetPosition(Actor_Get(0), points[nearest], 0, points[nearest + 1]);
    Actor_Get(0)->velocity_y = 0x60000;
    Audio_PlayCue(152);
    OverlayObject_WaitUntilField12BelowLimit(Actor_Get(0), Actor_Get(0)->y.fixed);
    Audio_PlayCue(241);
    leader = Actor_Get(0);
    rising.type = 214;
    rising.start_scale_x = 0x8000;
    rising.start_scale_y = 0xcccc;
    rising.target_scale_x = 0x10000;
    rising.target_scale_y = 0x13333;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0, 0, 0, 0x1c0000, &rising);
    Actor_ShowEmote(0, 0x104, 0);
    Actor_SetAnimation(0, 18);
    do {
        *(u16 *)(work + 0xcba) = 600;
        timer--;
        if (gGameState.unknown_232 != 0) {
            gGameState.unknown_232 -= 5;
            if (gGameState.unknown_232 <= 0)
                gGameState.unknown_232 = 0;
            else if (timer == 0)
                timer = 1;
        }
        Task_Wait(1);
    } while (timer != 0);
    leader = Actor_Get(0);
    landing.type = 214;
    landing.start_scale_y = 0xcccc;
    landing.start_scale_x = 0x8000;
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
