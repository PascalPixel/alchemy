/* The leader drops into the entrance from high above: raised far over the
   floor with the words at +68 and +72 set, the motion byte at +85 lets the
   fall play once the screen has opened. */
#include "IRIGUCHI.H"

void Effect_AdvanceMotion(struct MotionEffect *effect);
void OverlayObject_WaitUntilIdle(s32 *obj);

/* The arrival by entrance 3 the scene start plays until its flag is set: the
   leader lands in a ring of seventeen sparks and recovers. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    struct FieldActor *leader;
    struct EffectOptions options;
    s32 vec[3];

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 12) = 0x820000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x8000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = 0;
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 0;
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(30);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 3;
    Iriguchi_Wait(24);
    leader = Engine_ActorGet(ACTOR_PARTY_LEADER);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i < 17; i++) {
        vec[0] = Math_Cos(i << 12);
        vec[1] = 0;
        vec[2] = Math_Sin(i << 12);
        vec[0] += vec[0] / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vec[0], vec[1], vec[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Iriguchi_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x10000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = 0x4000;
    Event_End();
}

/* The leader falls in, hidden, and at once leaves by the exit given. */
void FieldScene_RunScene3c5SequenceA(s32 exit)
{
    u8 *leader;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 12) = 0x820000;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 72) = 0x4000;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 68) = 0;
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 0;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(10);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 3;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 40) = -0x50000;
    OverlayObject_WaitUntilIdle((s32 *)Actor_Get(ACTOR_PARTY_LEADER));
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Event_RequestExit(exit);
    Event_End();
}
