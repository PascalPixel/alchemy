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

static __inline__ void SetScale(s32 actor, s32 scale, s32 duration)
{
    Actor_FaceDirection(actor, scale, duration);
}

static __inline__ void SetScale_020009d8(s32 actor, s32 scale, s32 duration)
{

    Actor_FaceDirection(actor, scale, duration);
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
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(7, 16);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage((s32)MsgBiribinoThereWasAbleHealerIn);
        } else {
            Event_SetMessage((s32)MsgBiribinoOurWeaponsBestCanFind);
        }
        Event_ShowMessage(16, 0);
    }

    Event_End();
}

void FieldScene_RunActor18MessageBranch(void)
{

    u32 dir;

    dir = *(u16 *)((u8 *)Object_GetById(0) + 6);
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(9, 18);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage((s32)MsgBiribinoMustUsedTypesDangerBeing);
        } else {
            Event_SetMessage((s32)MsgBiribinoTurnedOutWarriorsHiredBy);
        }
        Event_ShowMessage(18, 0);
    }

    Event_End();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return BiribinoHeya_TertiaryTable;
}

void FieldScene_RunActor17MessageBranch(void)
{
    u32 dir;

    dir = *(u16 *)(Value1(Object_GetById, 0) + 6);
    Event_Begin();
    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(8, 17);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage((s32)MsgBiribinoHaveEverHeardOcean);
            Actor_FaceActor(17, ACTOR_PARTY_LEADER, 0);
            Event_Wait(10);
            Event_AskYesNo(17, 0);
            Actor_FaceDirection(17, 0x3000, 10);
        } else {
            Event_SetMessage((s32)MsgBiribinoForSomeReasonOceanFills);
            Event_ShowMessage(17, 0);
        }
    }
    Event_End();
}

void FieldScene_ConfigureActor21Scene(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoImReallyWorriedAboutKolima);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Event_ShowMessage(21, 0);
    SetScale(21, 0xc000, 10);
    Event_End();
}

void FieldScene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoGrrr);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceActor(24, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_OpenMessage(24, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(24, 0);
    Actor_FaceDirection(24, 0x4000, 10);
    Event_End();
}

void FieldScene_RunActor27Sequence(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoBelieveTreeSpiritCan);
    Actor_FaceActor(27, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_OpenMessage(27, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(27, 0x4000, 10);
    Event_End();
}

void FieldScene_RunActor8Message(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoKnowIfHolyTree);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_RunActor13Message(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoKnowSilk);
    Event_AskYesNo(13, 0);
    Event_End();
}

void FieldScene_RunActor19MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);

    u32 dir;

    dir = *(u16 *)((u8 *)Object_GetById(0) + 6);
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Inn_Open(2, 19);
    } else if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage((s32)MsgBiribinoWasntCurseInKolimaHorrifying);
        Event_AskYesNo(19, 0);
    } else {
        Event_SetMessage((s32)MsgBiribinoCurseOnKolimaScaryDevelopment);
        Event_ShowMessage(19, 0);
    }

    Event_End();
}

void FieldScene_RunActor21SequenceOnFlag300(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) == 0) {
        Event_SetMessage((s32)MsgBiribinoTwoSpecialsOneDinnerOne);
        Event_ShowMessage(21, 0);
        Actor_FaceDirection(21, 0x8000, 20);
        Event_ShowMessage(21, 0);
        Actor_StartRepeatedMotion(22, 2);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(60);
        Event_ShowMessage(22, 0);
        Event_Wait(10);
        GameFlag_Set(0x300);
    }
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((s32)MsgBiribinoIfWantMealSpeakWaitress);
    Event_ShowMessage(21, 0);
    Actor_FaceDirection(21, 0xc000, 10);
    Event_End();
}

void FieldScene_ConfigureActor22Scene(void)
{
    void Actor_FaceActor(s32, s32, s32);

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoReallyThinkHeadChefHas);
    Event_ShowMessage(0x16, 0);
    Actor_FaceActor(0x16, ACTOR_PARTY_LEADER, 0);
    Event_ShowMessage(0x16, 0);
    Actor_FaceDirection(0x16, 0, 0xA);
    Event_End();
}

void FieldScene_ConfigureActor23Scene(void)
{

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoLetsSeeServeThemWater);
    Event_ShowMessage(23, 0);
    Actor_FaceActor(23, ACTOR_PARTY_LEADER, 0);
    Event_ShowMessage(23, 0);
    SetScale_020009d8(23, 0xc000, 10);
    Event_End();
}

void FieldScene_RunActor27Message(void)
{
    void Event_ShowMessage(s32, s32);

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoWasSomeMealDontJust);
    Event_AskYesNo(27, 0);
    Event_End();
}

void FieldScene_RunActor10MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);
    void Event_End(void);
    void Event_ShowMessage(s32, s32);

    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage((s32)MsgBiribinoEvenFrozenImilMustFeel);
    } else {
        Event_SetMessage((s32)MsgBiribinoWhenSpringComesWantGo);
    }
    Event_ShowMessage(10, 0);
    Event_End();
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
