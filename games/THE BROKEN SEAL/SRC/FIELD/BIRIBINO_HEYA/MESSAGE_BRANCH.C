#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgBiribinoCurseOnKolimaScaryDevelopment[];
extern u8 MsgBiribinoDoBelieveTreeSpiritCan[];
extern u8 MsgBiribinoDoKnowIfHolyTree[];
extern u8 MsgBiribinoDoKnowSilk[];
extern u8 MsgBiribinoEvenFrozenImilMustFeel[];
extern u8 MsgBiribinoForSomeReasonOceanFills[];
extern u8 MsgBiribinoGrrr[];
extern u8 MsgBiribinoHaveEverHeardOcean[];
extern u8 MsgBiribinoIfWantMealSpeakWaitress[];
extern u8 MsgBiribinoImReallyWorriedAboutKolima[];
extern u8 MsgBiribinoLetsSeeServeThemWater[];
extern u8 MsgBiribinoMustUsedTypesDangerBeing[];
extern u8 MsgBiribinoOurWeaponsBestCanFind[];
extern u8 MsgBiribinoReallyThinkHeadChefHas[];
extern u8 MsgBiribinoThereWasAbleHealerIn[];
extern u8 MsgBiribinoTurnedOutWarriorsHiredBy[];
extern u8 MsgBiribinoTwoSpecialsOneDinnerOne[];
extern u8 MsgBiribinoWasSomeMealDontJust[];
extern u8 MsgBiribinoWasntCurseInKolimaHorrifying[];
extern u8 MsgBiribinoWhenSpringComesWantGo[];

/* The room's scene tables, where the overlay's data lies. */
extern u8 BiribinoHeya_PrimaryTable[];
extern u8 BiribinoHeya_SecondaryTable[];
extern u8 BiribinoHeya_TertiaryTable[];

/* Marks which of a scene's placements lie inside the current view. */
void ScenePlacement_ClipToView(s32 placements);

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* The workspace pointer this overlay reaches through. */
static __inline__ void bump_step(s32 amount)
{

    gEventWork->message += amount;
}

u8 *SceneData_GetPrimaryTable(void)
{
    return BiribinoHeya_PrimaryTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return BiribinoHeya_SecondaryTable;
}

s32 SceneData_PrepareTable86b0(void)
{
    ScenePlacement_ClipToView(0x020086B0);
    return 0x020086B0;
}

void FieldScene_RunActor16MessageBranch(void)
{

    u32 dir;

    dir = *(u16 *)((u8 *)Object_GetById(0) + 6);
    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(7, 16);
    } else {
        if (Engine_GameFlagIsSet(0x845) == 0) {
            Engine_EventSetMessage((s32)MsgBiribinoThereWasAbleHealerIn);
        } else {
            Engine_EventSetMessage((s32)MsgBiribinoOurWeaponsBestCanFind);
        }
        Engine_EventShowMessage(16, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunActor18MessageBranch(void)
{

    u32 dir;

    dir = *(u16 *)((u8 *)Object_GetById(0) + 6);
    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(9, 18);
    } else {
        if (Engine_GameFlagIsSet(0x845) == 0) {
            Engine_EventSetMessage((s32)MsgBiribinoMustUsedTypesDangerBeing);
        } else {
            Engine_EventSetMessage((s32)MsgBiribinoTurnedOutWarriorsHiredBy);
        }
        Engine_EventShowMessage(18, 0);
    }

    Engine_EventEnd();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return BiribinoHeya_TertiaryTable;
}

void FieldScene_RunActor17MessageBranch(void)
{
    u32 dir;

    dir = Object_GetById(0)->facing;
    Engine_EventBegin();
    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_ShopOpen(8, 17);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Engine_EventSetMessage((s32)MsgBiribinoHaveEverHeardOcean);
            Actor_FaceActor(17, ACTOR_PARTY_LEADER, 0);
            Engine_EventWait(10);
            Event_AskYesNo(17, 0);
            Actor_FaceDirection(17, 0x3000, 10);
        } else {
            Engine_EventSetMessage((s32)MsgBiribinoForSomeReasonOceanFills);
            Event_ShowMessage(17, 0);
        }
    }
    Engine_EventEnd();
}

void FieldScene_ConfigureActor21Scene(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoImReallyWorriedAboutKolima);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Event_ShowMessage(21, 0);
    Actor_FaceDirection(21, 0xc000, 10);
    Engine_EventEnd();
}

void FieldScene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoGrrr);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceActor(24, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Event_OpenMessage(24, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(24, 0);
    Actor_FaceDirection(24, 0x4000, 10);
    Engine_EventEnd();
}

void FieldScene_RunActor27Sequence(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoDoBelieveTreeSpiritCan);
    Actor_FaceActor(27, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Event_OpenMessage(27, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(27, 0x4000, 10);
    Engine_EventEnd();
}

void FieldScene_RunActor8Message(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoDoKnowIfHolyTree);
    Engine_EventAskYesNo(8, 0);
    Engine_EventEnd();
}

void FieldScene_RunActor13Message(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoDoKnowSilk);
    Engine_EventAskYesNo(13, 0);
    Engine_EventEnd();
}

void FieldScene_RunActor19MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);

    u32 dir;

    dir = *(u16 *)((u8 *)Object_GetById(0) + 6);
    Engine_EventBegin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Engine_InnOpen(2, 19);
    } else if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoWasntCurseInKolimaHorrifying);
        Engine_EventAskYesNo(19, 0);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoCurseOnKolimaScaryDevelopment);
        Engine_EventShowMessage(19, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunActor21SequenceOnFlag300(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x300) == 0) {
        Engine_EventSetMessage((s32)MsgBiribinoTwoSpecialsOneDinnerOne);
        Event_ShowMessage(21, 0);
        Actor_FaceDirection(21, 0x8000, 20);
        Event_ShowMessage(21, 0);
        Engine_ActorStartRepeatedMotion(22, 2);
        Actor_SetAttachedEffect(22, 0x102);
        Engine_EventWait(60);
        Event_ShowMessage(22, 0);
        Engine_EventWait(10);
        GameFlag_Set(0x300);
    }
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage((s32)MsgBiribinoIfWantMealSpeakWaitress);
    Event_ShowMessage(21, 0);
    Actor_FaceDirection(21, 0xc000, 10);
    Engine_EventEnd();
}

void FieldScene_ConfigureActor22Scene(void)
{
    void Actor_FaceActor(s32, s32, s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoReallyThinkHeadChefHas);
    Engine_EventShowMessage(0x16, 0);
    Engine_ActorFaceActor(0x16, ACTOR_PARTY_LEADER, 0);
    Engine_EventShowMessage(0x16, 0);
    Engine_ActorFaceDirection(0x16, 0, 0xA);
    Engine_EventEnd();
}

void FieldScene_ConfigureActor23Scene(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoLetsSeeServeThemWater);
    Event_ShowMessage(23, 0);
    Actor_FaceActor(23, ACTOR_PARTY_LEADER, 0);
    Event_ShowMessage(23, 0);
    Actor_FaceDirection(23, 0xc000, 10);
    Engine_EventEnd();
}

void FieldScene_RunActor27Message(void)
{
    void Event_ShowMessage(s32, s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoWasSomeMealDontJust);
    Engine_EventAskYesNo(27, 0);
    Engine_EventEnd();
}

void FieldScene_RunActor10MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);
    void Engine_EventEnd(void);
    void Event_ShowMessage(s32, s32);

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoEvenFrozenImilMustFeel);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoWhenSpringComesWantGo);
    }
    Engine_EventShowMessage(10, 0);
    Engine_EventEnd();
}

s32 FieldScene_SetupActor27OnEntry(void)
{

    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *)((u8 *)gEventWork + 448) = 521;
    actor = (u8 *)Object_GetById(27);
    /*
     * The stored zero is also the mask's starting value: -13 is built by
     * subtracting from the register the strb already set to zero, not by
     * materializing 0xf3 or negating 13.
     */
    actor[0x23] = bits = 0;
    record = *(u8 **)(actor + 0x50);
    bits -= 13;
    bits &= record[9];
    bits |= 8;
    record[9] = bits;
    return 0;
}
