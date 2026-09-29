/* Restaging the party. */
#include "CHOJO.H"
extern u8 MsgVinasuNoooo[];

enum {
    PARTICLE_SOURCE_ACTOR = 23
};

void SceneEffect_SpawnParticlesBesideActor(void);

/*
 * Restages the aerie after the pair's defeat: both of the pair are removed,
 * map cells are copied, the camera and actors are refreshed, and actors 0 to
 * 3 are placed with their rise stopped. The rise counters of actors 21 and 6
 * are cleared, the particle task for actor 23 starts, and the palette is
 * blended in from white over 40 frames.
 */
void FieldScene_RestageParty(void)
{
    struct FieldActor *actor;

    Actor_Destroy(ACTOR_FIRST_OF_PAIR);
    Actor_Destroy(ACTOR_SECOND_OF_PAIR);
    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Camera_MoveTo(-1, -1, -1, 0);
    Actors_Refresh();
    Map_Redraw();
    Task_Wait(1);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Actor_SetAnimation(ACTOR_GERALD, 18);
    Actor_SetAnimation(ACTOR_IVAN, 18);
    Actor_SetAnimation(ACTOR_MIA, 18);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->x.fixed = PIXELS(346);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(205);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_GERALD);
    actor->x.fixed = PIXELS(356);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(192);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_IVAN);
    actor->x.fixed = PIXELS(360);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_MIA);
    actor->x.fixed = PIXELS(334);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    Actor_Get(21)->rise_counter = 0;
    Actor_Get(6)->rise_counter = 0;
    Actor_Get(PARTICLE_SOURCE_ACTOR)->motion_flags |= 4;
    Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
    Task_AddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
    gEventWork->transition_frames = 1;
    Event_OpenScreen();
    ColorBuffer_ApplySource(0x7fff, 0);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Task_Wait(60);
}

void FieldScene_RunScene3c9_02004b28(void)
{
    u32 i;
    s32 record;

    Event_SetMessage((s32)MsgVinasuNoooo);
    VinasuChojo_ShowMessage(21);
    Audio_PlayCue(62);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Camera_SetSpeed(0x4cccc, 0x9999);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(0x2015, 0, 40);
    Actor_RunRepeatedMotion(6, 3);
    VinasuChojo_ShowMessage(6);
    Actor_SetAttachedEffect(21, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(0x2015, 0, 80);
    Actor_SetAttachedEffect(6, 0x102);
    Event_Wait(40);
    Actor_StartRepeatedMotion(6, 2);
    VinasuChojo_ShowMessage(6);
}
