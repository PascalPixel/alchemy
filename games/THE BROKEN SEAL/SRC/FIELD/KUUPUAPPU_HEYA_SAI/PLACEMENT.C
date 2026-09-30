/*
 * Overlay resource_386: actor placement, per-actor dialogue lines and the
 * scene initialiser that installs the per-frame task.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "RESOURCE_386_STATE.H"
extern u8 MsgKuupuappuDidSomeCookingNowIve[];
extern u8 MsgKuupuappuDodonpasHenchmenNeverAttackedUs[];
extern u8 MsgKuupuappuGeeAlwaysGetHungryWhen[];
extern u8 MsgKuupuappuHaveLotLeftoverBonesFrom[];
extern u8 MsgKuupuappuHeReallyLikesBonesWonder[];
extern u8 MsgKuupuappuItsNearTimeForColosso[];
extern u8 MsgKuupuappuMmmmNothingDoNothingDo[];
extern u8 MsgKuupuappuRobinYouveComeBackVault[];
extern u8 MsgKuupuappuTheresRiverFireOnOther[];
extern u8 MsgKuupuappuTheySayVolcanoOnMt[];
extern u8 MsgKuupuappuThoseGuysReallyWantedRevenge[];
extern u8 MsgKuupuappuThoughtSawCatGetUp[];
extern u8 MsgKuupuappuWantMoreBones[];
extern u8 MsgKuupuappuWeHaventBeenGettingMany[];
extern u8 MsgKuupuappuWowHaveManyThingsArent[];

enum {
    /* Message 0x182 + 231. */
    ITEM_BONE = 231
};

extern u8 *gWork;

/* The scene's tables, laid out after the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
extern u8 Placement_Effects[];

void FieldScene_PrepareActors(u8 *);
s32 PartyInventory_HasSpace(void);
void SceneState_CheckPositionWindow(void);
void OverlayObject_InitObject22(s32, s32, s32, s32);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

void SceneState_CheckPositionWindow(void)
{
    s32 v1;
    s32 v0;

    v0 = ((struct Resource386FirstView *)Actor_Get(0))->sample_08;
    v1 = (s32)((struct Resource386SecondView *)Actor_Get(0))->sample_10 >> 0x14;
    if (((u32)((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A)) {
        GameFlag_Set(0x250);
        return;
    }
    GameFlag_Clear(0x250);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

u8 *SceneData_GetPreparedActors(void)
{
    u8 *slot = Placement_Actors;

    FieldScene_PrepareActors(slot);
    return slot;
}

void SceneActor_RunActorStep(s32 arg0)
{
    Event_Begin();
    Actor_SetAnimation(arg0, 1);
    Event_ShowMessage(arg0, 0);
    Event_End();
}

void SceneDialogue_RunActor9Line(void)
{
    Event_SetMessage((s32)MsgKuupuappuThoughtSawCatGetUp);
    Actor_FaceEachOther(9, ACTOR_PARTY_LEADER, 2);
    SceneActor_RunActorStep(9);
}

void SceneDialogue_RunActor11Line(void)
{
    Event_SetMessage((s32)MsgKuupuappuDodonpasHenchmenNeverAttackedUs);
    Actor_FaceEachOther(11, ACTOR_PARTY_LEADER, 2);
    SceneActor_RunActorStep(11);
}

void SceneDialogue_RunActor12Line(void)
{
    Event_SetMessage((s32)MsgKuupuappuItsNearTimeForColosso);
    Actor_FaceEachOther(12, ACTOR_PARTY_LEADER, 2);
    SceneActor_RunActorStep(12);
}

void FieldScene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuRobinYouveComeBackVault);
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimation(16, 1);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(16, 0x102, 60);
    Event_ShowMessageAndWait(16, 0, 30);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(16, 0, 20);
    GameFlag_Set(0x300);
    GameFlag_Set(0x868);
    Event_End();
}

void SceneDialogue_RunActor16Line(void)
{
    void Actor_FaceEachOther(s32, s32, s32);

    Event_SetMessage((s32)MsgKuupuappuThoseGuysReallyWantedRevenge);
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 2);
    SceneActor_RunActorStep(16);
}

void SceneDialogue_RunActor23Line(void)
{
    void Actor_FaceEachOther(s32, s32, s32);

    Event_SetMessage((s32)MsgKuupuappuDidSomeCookingNowIve);
    Actor_FaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    SceneActor_RunActorStep(23);
}

void FieldScene_RunActor18FlaggedSequence(void)
{
    void Event_Wait();
    void Actor_FaceEachOther();
    void Event_ShowMessage();

    u32 i;
    s32 record;

    Event_Begin();
    Actor_FaceEachOther(18, ACTOR_PARTY_LEADER, 0);
    if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage((s32)MsgKuupuappuHaveLotLeftoverBonesFrom);
        Event_OpenMessage(18, 0);
    } else {
        Event_SetMessage((s32)MsgKuupuappuWantMoreBones);
        Event_OpenMessage(18, 0);
    }
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessage(18, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(18, 2);
        Event_Wait(20);
        if (PartyInventory_HasSpace() == 0) {
            Actor_SetAnimationAndWait(18, 4);
            Event_Wait(20);
            Event_SetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
            Event_ShowMessage(18, 0);
            goto L_020002d4;
        }
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
        GameFlag_Set(0x85b);
    } else {
        bump_step(1);
        Event_Wait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(20);
        Event_ShowMessage(18, 0);
    }
    L_020002d4:;
    Actor_FaceDirection(18, 0x4000, 0);
    Event_End();
}

void SceneActor_RunActor16StepWithFlag91(void)
{
    u8 *slot;
    u8 clear = 0;

    Event_Begin();
    Actor_RunRepeatedMotion(16, 1);
    Event_End();
    slot = (u8 *)Actor_Get(16) + 91;
    *slot = 1;
    FieldScene_RunActor16Sequence();
    slot = (u8 *)Actor_Get(16) + 91;
    *slot = clear;
    Actor_EnableActionCallback(16, 2);
}

void FieldScene_RunActor18ConditionalCue(void)
{
    void Event_Wait(s32);
    void Actor_SetAnimationAndWait(s32, s32);

    Event_Begin();

    if (PartyInventory_HasSpace() == 0) {
        Actor_SetAnimationAndWait(18, 4);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
        Event_ShowMessage(18, 0);
    } else {
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
    }

    Event_End();
}

void FieldScene_RunActor19StepByPlace(void)
{
    void Event_Begin(void);

    u32 place;

    place = *(u16 *)((u8 *)Actor_Get(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(4, 19);
    } else {
        Event_SetMessage((s32)MsgKuupuappuTheresRiverFireOnOther);
        Event_ShowMessage(19, 0);
    }

    Event_End();
}

void FieldScene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *)((u8 *)Actor_Get(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(5, 20);
    } else {
        Event_SetMessage((s32)MsgKuupuappuTheySayVolcanoOnMt);
        Event_ShowMessage(20, 0);
    }

    Event_End();
}

void FieldScene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *)((u8 *)Actor_Get(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(6, 21);
    } else {
        Event_SetMessage((s32)MsgKuupuappuWeHaventBeenGettingMany);
        Event_ShowMessage(21, 0);
    }

    Event_End();
}

void FieldScene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *)((u8 *)Actor_Get(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Inn_Open(1, 22);
    } else {
        Event_SetMessage((s32)MsgKuupuappuMmmmNothingDoNothingDo);
        Event_ShowMessage(22, 0);
    }

    Event_End();
}

void SceneDialogue_RunActor18FlaggedLine(void)
{
    s32 GameFlag_IsSet(s32);

    Event_Begin();
    if (GameFlag_IsSet(0x85B) == 0) {
        Event_SetMessage((s32)MsgKuupuappuGeeAlwaysGetHungryWhen);
    } else {
        Event_SetMessage((s32)MsgKuupuappuHeReallyLikesBonesWonder);
    }
    Event_ShowMessage(18, 0);
    Event_End();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    s32 GameFlag_IsSet(s32);

    return Placement_Effects;
}

/*
 * Overlay entry point: selects the scene from the global block and runs the
 * matching setup. It returns a constant zero status. The entrance the party
 * came in by selects the scene.
 */
s32 FieldScene_InitSceneStateByStep(void)
{
    s32 scene;
    s32 zero;

    *(s32 *)(gWork + 448) = 521;
    scene = gGameState.entrance;

    if (scene == 5) {
        s32 fifth = 4;
        s32 sixth = 3;

        /*
         * The fifth and sixth arguments go on the stack. The two locals
         * are what put them there, so they must stay locals.
         */
        Map_CopyCellsTo(0, 120, 8, 67, fifth, sixth);
        zero = 0;
        ((u8 *)Actor_Get(8))[0x55] = zero;
        *(s32 *)((u8 *)Actor_Get(8) + 12) = zero;
        *(s32 *)((u8 *)Actor_Get(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        OverlayObject_InitObject22(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /* 200 << 4 is the period. */
        Task_AddCallback(SceneState_CheckPositionWindow, 0xc80);
    }

    return 0;
}

/* Creates object 22 where given and shows the item's icon on it. */
void OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = (u8 *)Object_Create(22, fixed_x, fixed_y, fixed_z);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Heap_Allocate(17, 0x608);
        Item_LoadIcon(a);
        v += 0x400;
        Vram_Load(q[28], 0x80, v);
        Heap_Release(17);
    }
}
