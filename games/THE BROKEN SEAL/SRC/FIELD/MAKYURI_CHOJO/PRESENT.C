#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

void FieldScene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Owner_RefreshRatiosOnFlagFar(); /* main:08077268 */
    record = ((u8 *)Engine_ActorGet(0)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Engine_ActorGet(1)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Engine_ActorGet(2)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    record = ((u8 *)Engine_ActorGet(3)); /* main:0808a080 */
    SetOverlayObjectMode(record, 0); /* main:080091e0 */
    Camera_MoveTo(0x1300000, -1, 0x780000, 0);
    Task_Wait(1); /* main:080000c0 */
    Map_Redraw(); /* main:08009128 */
    Task_Wait(1); /* main:080000c0 */
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */
    MapRender_WaitForValues(); /* main:080091f8 */
    Event_Wait(30);
    /* Clear the byte at offset 85 of the record Battle_GetWorkObject1e0Far() returns. */
    *(u8 *)(Battle_GetWorkObject1e0Far() + 85) = 0;
    Camera_SetSpeed(0xcccc, 0x1999); /* speed_limit, acceleration */
    Camera_MoveTo(0x2000000, -0x180000, 0xa00000, 1);
    Camera_WaitForMove();
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(50); /* main:0808a348 */
    Event_Wait(50);
    ColorBuffer_ApplyTarget(0x7fff, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1f80000, 0xa80000);
    Actor_SetPosition(ACTOR_GERALD, 0x2100000, 0x900000);
    Actor_SetPosition(ACTOR_IVAN, 0x1e80000, 0x900000);
    Actor_SetPosition(ACTOR_MIA, 0x2000000, 0x980000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19); /* object 0, action 19 */
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    Actor_SetAnimation(ACTOR_MIA, 19);
    Event_Wait(10);
    ColorBuffer_ApplyTarget(0x10000, 0); /* main:0808a330 */
    ColorBuffer_Interpolate(30); /* main:0808a348 */
    Event_Wait(30);
    Event_Wait(80);
    record = ((u8 *)Engine_ActorGet(0));
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(60);
    record = ((u8 *)Engine_ActorGet(1)); /* main:0808a080 */
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    record = ((u8 *)Engine_ActorGet(2));
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Event_Wait(40);
    record = ((u8 *)Engine_ActorGet(3)); /* main:0808a080 */
    SetOverlayObjectMode(record, 1); /* main:080091e0 */
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = ((u8 *)Engine_ActorGet(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = ((u8 *)Engine_ActorGet(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = ((u8 *)Engine_ActorGet(0)); /* main:0808a080 */
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_End();
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 FindNearestF2Actor(void)
{
    u8 *work = *(u8 **)&gEventWork;
    Actor **actor_slot;
    Actor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = (Actor *)Engine_ActorGet(0);
    actor_id = 8;
    actor_slot = (Actor **)(work + 0x34);
    do {
        Actor *actor = *actor_slot++;
        if (actor != 0) {
            if (*actor->data->kind == 0xf2) {
                s32 dist = MeasureFixedPointPositionDistance(
                    (u8 *)origin + 8, (u8 *)actor + 8);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}
