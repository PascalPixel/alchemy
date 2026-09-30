/* The Mars star, the party's talk and entering the star room. */
#include "STAR.H"
#include "CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 MsgSoruAlexOnlyOneLeft[];
extern u8 MsgSoruGarciaSilence[];
extern u8 MsgSoruJasmineBeCareful[];
extern u8 MsgSoruJasmineMyBrotherIsAlive[];
extern u8 MsgSoruJasmineSukuretaIsGiddy[];
extern u8 MsgSoruJasmineThankYou[];
extern u8 MsgSoruJasmineWontYouPlease[];
extern u8 MsgSoruMarsStarBagged[];
extern u8 MsgSoruMenardiBringTheFinalGem[];
extern u8 MsgSoruNoStarsOutThere[];
extern u8 MsgSoruSaturosBringTheFinalStar[];
extern u8 MsgSoruSukuretaBringTheStarsHere[];
extern u8 MsgSoruSukuretaFetchTheOthers[];
extern u8 MsgSoruSukuretaIAmResponsible[];
extern u8 MsgSoruSukuretaTheElementsAreThe[];
extern u8 MsgSoruSukuretaTheWisdomStone[];
extern u8 MsgSoruSukuretaWhereAreWe[];
extern u8 MsgSoruTheBagsAreFromSukuretas[];
extern u8 MsgSoruTheStarsHaveEnormousPower[];

void SoruStar_SetupElementalRings();
void Func_0200227c();
void Scene_EnterStarRoom();

s32 PartyInventory_FindOwner(s32 item);
s32 Inventory_Find(s32 owner, s32 item);
void Engine_ItemLoadIcon(s32 item);
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
void Engine_HeapRelease(s32 slot);
void Engine_AudioPlayCue(s32 cue);
void Inventory_AddItem(s32 owner, s32 item);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
extern const s32 SoruStar_PresentItemScript[];

extern u8 MsgSoruGotFourMythrilBags[];
extern u8 MsgSoruTooManyItems[];

void Engine_ActorSetChildValue();
void Engine_EventWait(s32 frames);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

struct Actor {
    u8 pad[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[8];
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

void Scene_BagMarsStar(void)
{
    u8 pass;
    s32 star;
    struct FieldActor *leader;

    Event_Begin();
    Audio_PlayCue(141);
    for (pass = 0; pass != 6; pass++) {
        ColorBuffer_ApplyTarget(0x4039d2, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (pass == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Map_CopyCellsTo(0, 40, 13, 46, 3, 3);
    Event_Wait(20);
    star = Scene_PresentItem(222, 0xe80000, 0x100000, 0x900000);
    Event_Wait(40);
    UiWork_PushValueSlotFar(star, 1);
    Message_ShowCentered((s32)MsgSoruMarsStarBagged, 1);
    Actor_SetPosition(ACTOR_JASMINE, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1330000, 0x1150000);
    Actor_SetPosition(ACTOR_ALEX, 0x1330000, 0x1150000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
    Event_Wait(10);
    leader = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_GERALD, 218, 172);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Event_Wait(20);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 50);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 50);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Audio_PlayCue(144);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(30);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_Jump(ACTOR_GERALD, 2, 20);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventCloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(2);
}

/* Sukureta urges the party on, or apologizes once the Stars are gone. */
void Sukureta_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_STARS_GIVEN_TO_ALEX)) {
        Event_SetMessage((s32)MsgSoruSukuretaIAmResponsible);
        Event_ShowMessage(ACTOR_SUKURETA, 0);
    } else {
        if (GameFlag_IsSet(FLAG_FIRST_STAR_BAGGED) == 0)
            Event_SetMessage((s32)MsgSoruSukuretaBringTheStarsHere);
        else
            Event_SetMessage((s32)MsgSoruSukuretaFetchTheOthers);
        Actor_FaceEachOther(ACTOR_SUKURETA, ACTOR_PARTY_LEADER, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_SUKURETA, 0);
    }
    Event_End();
}

/* Jasmine teases Sukureta, or wonders why her brother kept silent. */
void Jasmine_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_STARS_GIVEN_TO_ALEX)) {
        Event_SetMessage((s32)MsgSoruJasmineMyBrotherIsAlive);
        Event_ShowMessage(ACTOR_JASMINE, 0);
    } else {
        if (GameFlag_IsSet(FLAG_FIRST_STAR_BAGGED) == 0)
            Event_SetMessage((s32)MsgSoruJasmineBeCareful);
        else
            Event_SetMessage((s32)MsgSoruJasmineSukuretaIsGiddy);
        Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_PARTY_LEADER, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_JASMINE, 0);
    }
    Event_End();
}

/* Saturos demands the last Elemental Star. */
void Saturos_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgSoruSaturosBringTheFinalStar);
    Event_ShowMessage(ACTOR_SATUROS, 0);
    Event_End();
}

/* Menardi demands the last Elemental Star. */
void Menardi_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgSoruMenardiBringTheFinalGem);
    Event_ShowMessage(ACTOR_MENARDI, 0);
    Event_End();
}

/* Garcia says nothing. */
void Garcia_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgSoruGarciaSilence);
    Event_ShowMessage(ACTOR_GARCIA, 0);
    Event_End();
}

/* Alex counts the one Star left. */
void Alex_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgSoruAlexOnlyOneLeft);
    Event_ShowMessage(ACTOR_ALEX, 0);
    Event_End();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void SceneDialogue_RunLine1072WithPair9And10(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgSoruNoStarsOutThere);
    Event_SayThenWait(9, 10);
    Event_End();
}

void FieldScene_Forward72b4(void)
{
    Leader_CheckAhead();
}

void Scene_EnterStarRoom(void)
{
    u16 i;
    u8 *leader_motion;
    u8 *sukureta_motion;
    u8 *leader_sprite;
    u8 *sukureta_sprite;
    struct FieldActor *rising;
    struct FieldActor *glow;
    struct FieldActor *leader;
    u8 *sukureta_flags;
    s32 say_4009;
    s32 height;
    s32 say_8009;
    s32 zero;

    Event_Begin();
    Map_CopyCellAttributes(27, 16, 5, 1, 27, 17);
    Camera_MoveTo(0x1d70000, -1, 0x1050000, 0);
    Camera_WaitForMove();
    Map_Redraw();
    glow = Actor_Get(8);
    glow->scale_x = 0x1999;
    glow->scale_y = 0x1999;
    rising = Actor_Get(ACTOR_PARTY_LEADER);
    leader_sprite = &rising->sprite->flags;
    *leader_sprite = 0;
    rising->scale_x = 0x1999;
    rising->scale_y = 0x1999;
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1d70000, 0x1220000);
    leader_motion = &rising->motion_flags;
    *leader_motion = 0;
    rising->y.fixed = 0x280000;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    for (i = 0; i != 90; i++) {
        rising->y.fixed += -0x1999;
        rising->scale_x += 0x28f;
        rising->scale_y += 0x28f;
        glow->scale_x += 0x28f;
        glow->scale_y += 0x28f;
        Event_Wait(1);
    }
    *leader_motion = 5;
    Event_Wait(80);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x1d70000, -1, 0x1220000, 1);
    for (i = 0; i != 60; i++) {
        rising->y.fixed += -0x8000;
        Event_Wait(1);
    }
    *leader_motion = 3;
    Event_Wait(20);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
        s32 flags = 1 | actor->priority_flags;
        actor->priority_flags = flags;
    }
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    {
        s32 shown = 1;

        *leader_sprite = shown;
    }
    ((void (*)())Engine_ActorSetPosition)(8, 0, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d7, 0x136);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    zero = 0;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_JASMINE, leader->x.fixed, leader->z.fixed);
    }
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_JASMINE, 0x1c5, 0x12e);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1e9, 0x12e);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_Jump(ACTOR_JASMINE, 2, 0);
    Actor_Jump(ACTOR_GERALD, 2, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d7, 0x15a);
    Actor_WalkTo(ACTOR_JASMINE, 0x1af, 0x152);
    Actor_WalkTo(ACTOR_GERALD, 0x1ff, 0x152);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 60);
    rising = Actor_Get(ACTOR_SUKURETA);
    sukureta_sprite = &rising->sprite->flags;
    *sukureta_sprite = zero;
    rising->scale_x = 0x1999;
    rising->scale_y = 0x1999;
    glow->scale_x = 0x1999;
    glow->scale_y = 0x1999;
    Actor_SetChildValue(ACTOR_SUKURETA, 0x100);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1220000);
    sukureta_motion = &rising->motion_flags;
    *sukureta_motion = zero;
    rising->y.fixed = 0x280000;
    Event_Wait(1);
    Event_SetMessage((s32)MsgSoruSukuretaWhereAreWe);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_Jump(ACTOR_GERALD, 4, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 5, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Actor_SetSpritePriority(ACTOR_SUKURETA, 2);
    for (i = 0; i != 90; i++) {
        rising->y.fixed += -0x1999;
        rising->scale_x += 0x28f;
        rising->scale_y += 0x28f;
        glow->scale_x += 0x28f;
        glow->scale_y += 0x28f;
        Event_Wait(1);
    }
    *sukureta_motion = 5;
    Event_Wait(80);
    for (i = 0; i != 60; i++) {
        rising->y.fixed += -0x8000;
        Event_Wait(1);
    }
    *sukureta_motion = 3;
    Event_Wait(30);
    Actor_SetSpritePriority(ACTOR_SUKURETA, 1);
    {
        struct FieldActor *actor = Actor_Get(ACTOR_SUKURETA);
        s32 flags = 1 | actor->priority_flags;
        actor->priority_flags = flags;
    }
    Actor_SetChildValue(ACTOR_SUKURETA, 0);
    {
        s32 shown = 1;

        *sukureta_sprite = shown;
    }
    Actor_SetPosition(8, 0, 0);
    ((void (*)())Engine_EventWait)(30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x132);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
    Actor_Jump(ACTOR_SUKURETA, 2, 80);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Event_SayThenWait(9, 20);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x26666, 0x13333);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1a7, 0x132);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x207, 0x132);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    say_4009 = 0x4009;
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_Wait(40);
    Event_SayThenWait(say_4009, 30);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Event_SayThenWait(1, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
    Event_SayThenWait(say_4009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_SayThenWait(say_4009, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x2150000, -1, 0x1530000, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x215, 0x153);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 60);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Camera_FollowActor(ACTOR_SUKURETA, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_SUKURETA, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 0);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 30);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(30);
    Event_SayThenWait(9, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Event_SayThenWait(9, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1b8, 0x15a);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 10);
    Event_SayThenWait(5, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1ef, 0x15a);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Event_SayThenWait(ACTOR_SUKURETA, 30);
    Event_SetMessage((s32)MsgSoruSukuretaTheElementsAreThe);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(9, 10);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_SayThenWait(9, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(9, 10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Event_Wait(40);
    Event_SayThenWait(9, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x2000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Actor_Stop(ACTOR_SUKURETA);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_JASMINE);
    Actor_Stop(ACTOR_GERALD);
    Camera_SetSpeed(0x80000, 0x10000);
    Camera_MoveTo(0x2c70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x24d0000, 0x1610000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 20);
    Camera_MoveTo(0x2c70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x2540000, 0xee0000);
    Event_Wait(40);
    Event_ShowMessage(0x1009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 4, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Camera_MoveTo(0xe70000, -1, 0x930000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x1340000, 0xfa0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Camera_MoveTo(0xe70000, -1, 0x1d00000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(ACTOR_SUKURETA, 0x1320000, 0x16a0000);
    Event_Wait(40);
    Event_ShowMessage(0x2009, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
    Event_Wait(80);
    Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x104, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_SayThenWait(5, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(0xa009, 10);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_SayThenWait(0xa009, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    Actor_SetAnimation(ACTOR_SUKURETA, 4);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Event_SayThenWait(ACTOR_GERALD, 10);
    Event_SetMessage((s32)MsgSoruSukuretaTheWisdomStone);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    Event_SayThenWait(0x8009, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Event_Wait(40);
    Event_SayThenWait(0x8009, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x106, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x106, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Event_OpenMessage(0x8009, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_SayThenWait(0x8009, 40);
    Event_SetMessage((s32)MsgSoruTheStarsHaveEnormousPower);
    Actor_SetAnimation(ACTOR_JASMINE, 4);
    Event_SayThenWait(5, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    Event_SayThenWait(0xa009, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_SayThenWait(0x8009, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 120);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 0);
    Actor_Jump(ACTOR_GERALD, 4, 40);
    Event_SayThenWait(1, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 80);
    Event_SayThenWait(0x8009, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Event_SayThenWait(0x8009, 40);
    rising = Actor_Get(ACTOR_SUKURETA);
    height = 192;
    Task_Wait(6);
    rising->speed = 0x30000;
    rising->acceleration = 0x20000;
    Audio_PlayCue(153);
    rising->velocity_y = (height << 11);
    Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Task_Wait(6);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
    sukureta_flags = &rising->unknown_5a;
    *sukureta_flags &= 254;
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d9, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d5, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Event_SayThenWait(9, 10);
    Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x19b);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
    *sukureta_flags &= 254;
    Actor_SetDestination(ACTOR_SUKURETA, 0x1da, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d4, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
    Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x19b);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 3);
    Event_SayThenWait(9, 10);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
    Actor_EnableActionCallback(ACTOR_SUKURETA, 1);
    Event_Wait(30);
    {
        s32 flags = 1 | *sukureta_flags;
        *sukureta_flags = flags;
    }
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    Task_Wait(6);
    rising->speed = 0x30000;
    rising->acceleration = 0x20000;
    Audio_PlayCue(153);
    rising->velocity_y = (height << 11);
    Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
    Task_Wait(6);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 80);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_ShowEmote(ACTOR_JASMINE, 0x107, 40);
    Event_SayThenWait(5, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_SayThenWait(0xa009, 30);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 30);
    Actor_ShowEmote(ACTOR_JASMINE, 0x106, 60);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 30);
    Event_SayThenWait(5, 20);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 30);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
    Event_Wait(20);
    Event_SayThenWait(0x8009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 30);
    Event_OpenMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage((s32)MsgSoruJasmineWontYouPlease);
        Actor_ShowEmote(ACTOR_JASMINE, 0x107, 0);
        Actor_Jump(ACTOR_JASMINE, 4, 60);
        Event_OpenMessage(ACTOR_JASMINE, 0);
    }
    Event_SetMessage((s32)MsgSoruJasmineThankYou);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x2000, 30);
    Event_SayThenWait(5, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x160);
    Event_Wait(20);
    say_8009 = 0x8009;
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Event_SayThenWait(say_8009, 60);
    *sukureta_flags &= 254;
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c8, 0x168);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgSoruTheBagsAreFromSukuretas);
    Event_SayThenWait(5, 30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_SayThenWait(1, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_SayThenWait(say_8009, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(say_8009, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_Wait(10);
    Event_SayThenWait(say_8009, 30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    leader = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(FLAG_STAR_ROOM_EXPLAINED);
    Party_RemoveOwnerRestored(5);
    Scene_GiveMythrilBags();
    Map_CopyCellAttributes(8, 0, 5, 1, 27, 17);
    gEventWork->transition_frames = 16;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

/* Elemental Star Room entry: set flag 0x144, show actors 15..24 above the
 * floor and start their ring, then restore the actors and the opened cells
 * each story flag records. */
s32 SoruStar_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_f;

    Engine_ColorBufferApplySource(0x10000, 0);
    Engine_GameFlagSet(0x144);
    base5_f = 15;
    do {
        *(u8 *)((u8 *)Object_GetById(base5_f) + 89) = 0;
        Engine_ActorSetSpritePriority(base5_f++, 1);
    } while ((u32)base5_f <= 24);
    SoruStar_SetupElementalRings(15, 16);
    if (Value1(Engine_GameFlagIsSet, 0x83b) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x1c80000, 0x1680000);
        Call3(Engine_ActorSetPosition, 5, 0x1b80000, 0x15a0000);
    }
    if (Engine_GameFlagIsSet(0x83c) != 0) {
        Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 96, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
        Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 74, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
        Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 96, 10, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 10);
        Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
    }
    if (Engine_GameFlagIsSet(0x83d) != 0) {
        Engine_MapCopyCellsTo(0, 40, 43, 46, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 84, 4, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 29, 4);
        Call6(Engine_MapCopyCellsTo, 87, 42, 29, 6, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 76, 21, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 21);
        Engine_MapCopyCellsTo(87, 42, 21, 23, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 76, 29, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 29);
        Engine_MapCopyCellsTo(87, 42, 21, 31, 1, 2);
    }
    if (Engine_GameFlagIsSet(0x83e) != 0) {
        Engine_MapCopyCellsTo(0, 40, 13, 66, 3, 3);
        Engine_MapCopyCellsTo(83, 40, 65, 31, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 10, 31);
        Call6(Engine_MapCopyCellsTo, 87, 42, 10, 33, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 79, 9, 3, 4);
        Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 24, 9);
        Engine_MapCopyCellsTo(87, 42, 24, 11, 1, 2);
        Engine_MapCopyCellsTo(83, 40, 91, 10, 3, 4);
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 36, 10);
        Engine_MapCopyCellsTo(87, 42, 36, 12, 1, 2);
        Func_0200227c();
    }
    if (Engine_GameFlagIsSet(0x83b) == 0) {
        if (gGameState.entrance == 10) {
            Scene_EnterStarRoom();
        }
    }
    return 0;
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}

s32 Scene_PresentItem(s32 item)
{
    register u8 *buf asm("r8"); /* FAKEMATCH: pins the buffer to r8 */
    u8 *obj;
    s32 owner;
    s32 slot;
    u8 *sprite;
    u8 *p;
    s32 mask;

    {
        register s32 zero asm("r0"); /* FAKEMATCH: builds the zero in r0 */

        asm("mov %0, #0" : "=l"(zero)); /* FAKEMATCH: the zero is not a reloadable constant */
        buf = (u8 *)zero;
    }
    obj = ((u8 * (*)(s32))Engine_ObjectCreate)(22);
    owner = PartyInventory_FindOwner(224);
    slot = Inventory_Find(owner, 224);

    if (obj == 0) {
        return owner;
    }
    {
        ((void (*)(u8 *, const s32 *))Engine_ObjectSetScript)(obj, SoruStar_PresentItemScript);
        sprite = *(u8 **)(obj + 80);
        p = sprite + 38;
        *p = (u32)buf;
        p++;
        *p = (u32)buf;
        mask = 33;
        mask = -mask;
        sprite[5] &= mask;
        sprite[9] &= 0xf;
        *(s32 *)(obj + 40) = 163840;
        *(s32 *)(obj + 72) = 16384;
        buf = (u8 *)Engine_HeapAllocate(17, 1544);
        Engine_ItemLoadIcon(item);
        Engine_VramLoad(sprite[28], 128, buf + 1024);
        Engine_HeapRelease(17);
        Engine_AudioPlayCue(83);
        ((void (*)(u8 *, s32))Engine_RunRisingObjectSequence)(obj, 3);
        ((s32 (*)(s32, s32))Inventory_Discard)(owner, slot);
        Inventory_AddItem(owner, item);
        ((void (*)(u8 *))Engine_ObjectDispatchRelease)(obj);
        Engine_ActorSetAnimation(0, 1);
    }
    return owner;
}

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void Scene_GiveMythrilBags(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Audio_PlayCue(0x53);
    Item_ShowFound(ITEM_MYTHRIL_BAG, 3);
    Message_ShowCentered((s32)MsgSoruGotFourMythrilBags, 1);
    do {
        cnt = 30 - Inventory_Count(0);
        cnt -= Inventory_Count(1);

        if (cnt <= 3) {
            Message_ShowCentered((s32)MsgSoruTooManyItems, 1);
            if (Shop_PickUnitItem(&second, &first) != -1)
                Inventory_Discard(second, first);
        }
    } while (cnt <= 3);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    *(s16 *)(work + 472) = saved;
}

/* Actor 14 rises back into view, throwing sparks every other frame. */
void SoruStar_RiseActorFourteenSparks(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct Actor *actor;
    u32 i;
    s32 x;
    s32 r;

    actor = (struct Actor *)Object_GetById(14);
    Engine_AudioPlayCue(190);
    ((void (*)(struct Actor *, s32))Engine_ActorSetSpriteFlags)((struct Actor *)Object_GetById(14), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11c;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (!(i & 1)) {
            r = (((u32)Engine_RandomNext() * 24) >> 16) << 16;
            x = actor->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, actor->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, actor->z, 0, -0x40000, 0, 0x1b0000, p);
        }
        if (i == 20)
            Engine_ActorSetChildValue(14, 0x100);
    }
    Engine_ActorSetChildValue(14, 0);
    ((void (*)(struct Actor *, s32))Engine_ActorSetSpriteFlags)((struct Actor *)Object_GetById(14), 1);
}
