#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080770e0(s32 flag);
void Main_0808a590(s32 actor);
void Main_080003b8(void (*callback)(void), s32 value);
void Main_080091c8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void SceneState_ApplyRectsForActors15To17(void);
s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
s32 FieldScene_BuildDescriptorAndInstallTask(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
void Korosseo_SelectSoloCompetitor(s32 index);
void SceneActor_PlacePartyAtSavedTiles(void);
void KorosseoKabe_RunScriptedTransition(s32 value);
void Korosseo_RunGreetScene(s32 actor);
void FieldScene_RunSixSteps896To936(void);
void FieldScene_RunPairedEntranceWalk(s32 direction);
void SceneState_InitControlRecordAndStartTask(s32 value);
void Engine_ActorFollow(s32 actor, s32 leader);

/* The game-state rows are read as halfwords and written as bytes through
 * one symbol, so both keep the base-plus-index address form. */
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern u8 Value_000000e5;
void FieldScene_RunSupplementalSequenceOne(void);
void KorosseoKabe_MarkSceneProgress(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Wall arena entry: record the arrival, set the pillars, ledges and item icons by the story flags, then start the entrance's opening scene. */
s32 KorosseoKabe_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 x;
    s32 i;
    s32 col;
    s32 k;
    s32 pos;
    s32 row;
    s32 zero;

    gEventWork->start_transition = 0;
    Call1(Engine_GameFlagSet, 0x144);
    Call6(Engine_MapCopyCellAttributes, 14, 11, 12, 4, 100, 11);
    Call6(Engine_MapCopyCellAttributes, 48, 10, 5, 6, 120, 10);
    for (i = 26; i <= 30; i++) {
        actor = Engine_ActorGet(i);
        Engine_ObjectSetAnimation(actor, 4);
        actor->motion_flags = 0;
        actor->y.fixed = 0;
        actor->priority_flags = 2;
    }
    Engine_ActorGet(18)->priority_flags = 2;
    if (Engine_GameFlagIsSet(0x330)) {
        actor = Engine_ActorGet(30);
        actor->x.fixed = 0x1500000;
        actor->y.fixed = -0x80000;
        actor->z.fixed = 0x1080000;
        Call6(Engine_MapCopyCellAttributes, 19, 16, 1, 1, 20, 16);
        Call6(Engine_MapCopyCellAttributes, 20, 80, 1, 1, 21, 80);
    } else {
        actor = Engine_ActorGet(30);
        Engine_ObjectSetAnimation(actor, 3);
        actor->y.fixed = 0x100000;
    }
    Engine_ActorGet(11)->priority_flags = 2;
    zero = 0;
    if (Engine_GameFlagIsSet(0x335)) {
        Call6(Engine_MapCopyCellAttributes, 35, 78, 1, 1, 35, 77);
    }
    if (Engine_GameFlagIsSet(0x333)) {
        Engine_ActorSetAnimation(19, 4);
        Call6(Engine_MapCopyCellAttributes, 32, 37, 1, 4, 32, 77);
    }
    if (Engine_GameFlagIsSet(0x331)) {
        Engine_ActorGet(20)->motion_flags = zero;
        Engine_ActorGet(20)->priority_flags = 2;
        Engine_ActorSetAnimation(20, 5);
        Call6(Engine_MapCopyCellAttributes, 46, 17, 1, 1, 44, 17);
    }
    if (Engine_GameFlagIsSet(0x332)) {
        Engine_ActorGet(21)->motion_flags = zero;
        Engine_ActorGet(21)->priority_flags = 2;
        Engine_ActorSetAnimation(21, 5);
        Call6(Engine_MapCopyCellAttributes, 46, 17, 1, 1, 50, 17);
    }
    actor = Engine_ActorGet(32);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Call6(Engine_MapCopyCellAttributes, 52, 28, 1, 3, col, 10);
    actor = Engine_ActorGet(33);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Call6(Engine_MapCopyCellAttributes, 52, 28, 1, 3, col, 13);

    x = Main_080770e0(0x340);
    if (x == 0) {
        x = 73;
    }
    actor = Engine_ActorGet(12);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Call6(Engine_MapCopyCellAttributes, 71, 16, 1, 1, x, 16);
    x = Main_080770e0(0x348);
    if (x == 0) {
        x = 76;
    }
    actor = Engine_ActorGet(13);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Call6(Engine_MapCopyCellAttributes, 71, 16, 1, 1, x, 16);
    x = Main_080770e0(0x350);
    if (x == 0) {
        x = 79;
    }
    actor = Engine_ActorGet(14);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Call6(Engine_MapCopyCellAttributes, 71, 16, 1, 1, x, 16);

    SceneState_ApplyRectsForActors15To17();
    Engine_ActorSetAnimation(31, 10);
    if (Engine_GameFlagIsSet(0x334)) {
        for (k = 22, row = 13, pos = 58; k <= 25; k++, pos += 2) {
            actor = Engine_ActorGet(k);
            actor->priority_flags = 2;
            Engine_ObjectSetAnimation(actor, 4);
            Call6(Engine_MapCopyCellAttributes, 56, 13, 1, 1, pos, row);
        }
        Engine_ActorSetAnimation(31, 10);
        Main_0808a590(31);
    } else {
        for (k = 22; k <= 25; k++) {
            actor = Engine_ActorGet(k);
            actor->priority_flags = 2;
            Engine_ObjectSetAnimation(actor, 4);
            actor->speed = 0x8000;
            actor->acceleration = 0x3333;
        }
        Engine_TaskAddCallback(FieldScene_RunSupplementalSequenceOne, 0xc85);
        Main_080003b8(FieldScene_RunSupplementalSequenceOne, 1);
    }
    Engine_ActorSetAnimation(8, 9);
    Data_02000240_t.bytes[249][0] = 0;
    Korosseo_ShowItemIcon(41, 89);
    Korosseo_ShowItemIcon(40, 77);
    Engine_ActorSetChildValue(8, 1);
    switch (Data_02000240_t.halves[225][0]) {
    case 1:
        FieldScene_BuildDescriptorAndInstallTask(0, 8, 5, 0x680000, 0x1000000, 40, 41);
        Call6(Main_080091c8, 127, 0, 1, 2, 79, 6);
        Engine_ActorDestroy(34);
        Engine_ActorDestroy(35);
        Engine_ActorDestroy(36);
        Engine_ActorDestroy(37);
        Engine_ActorDestroy(38);
        Engine_ActorDestroy(39);
        if (!Engine_GameFlagIsSet(0x109)) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            SceneActor_PlacePartyAtSavedTiles();
            KorosseoKabe_RunScriptedTransition(2);
        }
        Engine_ActorFollow(1, 0);
        Engine_ActorFollow(2, 0);
        Engine_ActorFollow(3, 0);
        SceneState_InitControlRecordAndStartTask((s32)&Value_000000e5);
        break;
    case 2:
        Engine_TaskAddCallback(KorosseoKabe_MarkSceneProgress, 0xc80);
        Engine_ActorDestroy(40);
        Engine_ActorDestroy(41);
        if (!Engine_GameFlagIsSet(0x109)) {
            SceneActor_PlacePartyAtSavedTiles();
            Korosseo_SelectSoloCompetitor(1);
            KorosseoKabe_RunScriptedTransition(0);
        }
        break;
    case 3:
        if (!Engine_GameFlagIsSet(0x109)) {
            Korosseo_RunGreetScene(34);
            FieldScene_RunSixSteps896To936();
        }
        break;
    case 4:
        FieldScene_RunPairedEntranceWalk(2);
        Engine_EventRequestExit(4);
        break;
    case 5:
        FieldScene_RunPairedEntranceWalk(-2);
        Engine_EventRequestExit(5);
        break;
    }
    return 0;
}
