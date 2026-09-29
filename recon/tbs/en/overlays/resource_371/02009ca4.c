/* Draft of WorldMap_MeetVenusDjinni, resource_371 at 0x02009ca4 (FIELD/WORLD_MAP/VENUS_DJINNI.C).
 * Remaining difference (18 instructions): the two all-zero Djinni calls after
 * the leader-position join load r0 first where the ROM loads r1, r2, r0; the
 * scale-up loop counts 16..1 with bne where the ROM counts 15..0 with bge (the
 * i = 0 at the top keeps the ROM's zero in r5 but hides the loop's start
 * value); the rise to 0x100000 builds its argument earlier; the Ability
 * message's jump and message load r0 first; path B's walk loads x before z.
 * Linking it needs the IMPORT.S labels it calls, which are committed. */
/* The world map's Venus Djinni: on the first meeting it joins Isaac, grows
 * from a speck and explains itself, asking until the party agrees to listen;
 * later it offers to explain Djinn again. */
#include "STORY.H"

extern u8 MsgWorldMapOh[];
extern u8 MsgWorldMapComePromiseWont[];
extern u8 MsgWorldMapMeanieDontCare[];
extern u8 MsgWorldMapSeeWontRegret[];
extern u8 MsgWorldMapAbilityVenusDjinni[];
extern u8 MsgWorldMapSeeDjinnUseful[];
extern u8 MsgWorldMapGoSetStandby[];
extern u8 MsgWorldMapHmmmmExplainAgain[];
extern u8 MsgWorldMapYeahWantLearn[];

void Owner_RefreshActiveRatios(s32 owner);
void Djinn_AddToOwner(u8 owner, u8 element, u8 index);
void Trade_AddOffer(u8 owner, u8 element, u8 index);
void BattleEffect_CleanupSceneObjects(void);
void UiWork_PushValueSlot(s32 value, s32 slot);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 a1, s32 a2);
void FieldScene_RunScene371_02001c08(void);

#define DJINNI 8
#define FLAG_DJINNI_MET 0x16e

void WorldMap_MeetVenusDjinni(void)
{
    struct FieldActor *djinni;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 i;

    i = 0;
    djinni = Engine_ActorGet(DJINNI);
    leader = Engine_ActorGet(0);
    x = (leader->x.fixed + (s32)0xea300000) / 2 + 0x15d00000;
    z = (leader->z.fixed + (s32)0xfad00000) / 2 + 0x5300000;
    if (!Value1(Engine_GameFlagIsSet, FLAG_DJINNI_MET)) {
        Owner_RefreshActiveRatios(1);
        Call1(Engine_GameFlagSet, FLAG_DJINNI_MET);
        Event_Begin();
        leader = (struct FieldActor *)Value1((s32 (*)())Engine_ActorGet, 0);
        if (leader != 0)
            Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
        Djinn_AddToOwner(0, 0, 0);
        Trade_AddOffer(0, 0, 0);
        Battle_SetObjectFlag5bWhenMode3();
        Actor_FaceActor(0, DJINNI, 0);
        Event_Wait(10);
        Actor_ShowEmote(0, 0x101, 60);
        djinni->unknown_66 = 1;
        Actor_FaceActor(DJINNI, 0, 0);
        Task_Wait(16);
        Event_SetMessage((s32)MsgWorldMapOh);
        Event_ShowMessage(DJINNI, 0);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_ScheduleRatioTransition(0x13333, 6);
        Event_WaitForDisplayField358Clear();
        Battle_SetObjectFlag5bWhenMode3();
        djinni->motion_flags = 2;
        *(s32 *)&djinni->unknown_44[4] = 0x4000;
        djinni->speed = 0x10000;
        djinni->acceleration = 0x10000;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
        for (i = 0; i < 16; i++) {
            djinni->scale_x += 0x800;
            djinni->scale_y += 0x800;
            Task_Wait(1);
        }
        Actor_FaceActor(DJINNI, 0, 0);
        Actor_FaceActor(0, DJINNI, 0);
        Task_Wait(16);
        djinni->update = 0;
        Object_SetPartPalettes(djinni, 0);
        *(s32 *)&djinni->unknown_44[4] = 0x10000;
        Event_ShowMessage(DJINNI, 0);
        Audio_PlayCue(131);
        Psynergy_Begin(140, 0);
        for (i = 59; i >= 0; i--) {
            if (gFrameCount & 2)
                Object_SetPartPalettes(djinni, 7);
            else
                Object_SetPartPalettes(djinni, 0);
            if ((gFrameCount & 15) == 0)
                WorldMap_CreateLinkedEffects(djinni);
            Task_Wait(1);
        }
        BattleEffect_CleanupSceneObjects();
        Object_SetPartPalettes(djinni, 0);
        Actor_RunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x102, 30);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 30);
        Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
        Actor_SetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 40);
        Actor_Jump(DJINNI, 4, 30);
        UiWork_PushValueSlot(300, 4);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x100, 30);
        Event_ShowMessage(DJINNI, 0);
        Actor_RunRepeatedMotion(0, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_Jump(DJINNI, 2, 30);
        Event_ShowMessage(DJINNI, 0);
        i = 0;
        djinni->motion_flags = 0;
        Engine_ObjectSetPosition(djinni, x, 0x100000, z);
        for (; i < 16; i++) {
            djinni->facing += 0x1000;
            Task_Wait(1);
        }
        Actor_SetAnimation(0, 1);
        Event_ShowMessage(DJINNI, 0);
        djinni->motion_flags = 2;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        for (i = 7; i >= 0; i--) {
            djinni->facing += 0x1000;
            Task_Wait(1);
        }
        Actor_SetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(DJINNI, 0x102, 30);
        Actor_FaceActor(DJINNI, 0, 0);
        Actor_RunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_Jump(DJINNI, 2, 30);
        Event_OpenMessage(DJINNI, 0);
        i = 0;
    plead:
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_Jump(DJINNI, 2, 20);
            Actor_Jump(DJINNI, 2, 20);
            if (i == 6) {
                Event_SetMessage((s32)MsgWorldMapMeanieDontCare);
                Event_ShowMessage(DJINNI, 0);
                goto listened;
            }
            Event_SetMessage(i + (s32)MsgWorldMapComePromiseWont);
            Event_OpenMessage(DJINNI, 0);
            i++;
            goto plead;
        }
        Actor_SetAnimation(0, 22);
        Actor_Jump(DJINNI, 2, 20);
        Actor_Jump(DJINNI, 4, 20);
        Event_SetMessage((s32)MsgWorldMapSeeWontRegret);
        Event_ShowMessage(DJINNI, 0);
listened:
        UiWork_PushValueSlot(300, 4);
        Audio_PlayCue(81);
        i = (s32)MsgWorldMapAbilityVenusDjinni;
        Message_ShowCentered(i++, 3);
        Event_SetMessage(i);
        Actor_Jump(DJINNI, 2, 20);
        Event_ShowMessage(DJINNI, 0);
        Audio_PlayCue(9);
        goto finish;
    }
    Event_Begin();
    leader = Engine_ActorGet(0);
    if (leader != 0)
        Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
    djinni->velocity_y = 0xa0000;
    Engine_ObjectSetPosition(djinni, x, 0, z);
    Event_Wait(30);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_FaceActor(DJINNI, 0, 0);
    Actor_FaceActor(0, DJINNI, 0);
    Actor_SetAnimation(0, 22);
    Event_SetMessage((s32)MsgWorldMapSeeDjinnUseful);
    Actor_Jump(DJINNI, 2, 20);
    Actor_Jump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Actor_RunRepeatedMotion(DJINNI, 2);
    Event_ShowMessage(DJINNI, 0);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Set(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Event_SetMessage((s32)MsgWorldMapGoSetStandby);
    Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
    Event_Wait(30);
    Event_ShowMessage(DJINNI, 0);
    Actor_FaceActor(DJINNI, 0, 0);
    Event_ShowMessage(DJINNI, 0);
    Event_OpenMessage(DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) != 1)
        goto learn;
    Actor_SetAnimation(0, 22);
    Actor_RunRepeatedMotion(DJINNI, 2);
    Event_SetMessage((s32)MsgWorldMapHmmmmExplainAgain);
    Event_OpenMessage(DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) == 1)
        goto learn;
    Event_ShowMessage(DJINNI, 0);
    Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
finish:
    FieldScene_RunScene371_02001c08();
    Battle_ClearObjectFlag5bWhenMode3();
    return;
learn:
    Actor_SetAnimation(0, 22);
    Event_SetMessage((s32)MsgWorldMapYeahWantLearn);
    Actor_Jump(DJINNI, 2, 20);
    Actor_Jump(DJINNI, 2, 20);
    Actor_SetAnimationAndWait(0, 3);
    Actor_ShowEmote(DJINNI, 0x100, 30);
    Event_ShowMessage(DJINNI, 0);
    GameFlag_Set(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Actor_Jump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_RunPageEffectForSlot(DJINNI, 0, 0);
    Audio_PlayCue(42);
    Event_End();
    GameFlag_Clear(FLAG_DJINNI_MET);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
}
