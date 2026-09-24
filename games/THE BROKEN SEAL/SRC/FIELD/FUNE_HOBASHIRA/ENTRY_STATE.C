#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a2b0(void);
void Local_020010a0(void);
void FieldScene_RunScene3b0_02000468(void);
void FieldScene_RunScene3b0_0200040c(void);
void Scene_RunFourActorStagingSequence(void);
void FieldScene_RunActorNinePresentationCycles(void);
void FieldScene_RunPrimarySequence(void);
void Scene_RunSevenActorEnsemble(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_0000006f[];
extern u32 Data_02009940;
extern u32 Data_02009928;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Ship entry: record the arrival, seed and start the deck task while the voyage flags allow it, then run the entrance's scene. */
s32 FuneHobashira_ApplyEntryState(void)
{
    struct FieldActor *leader;

    Engine_GameFlagSet(0x144);
    gEventWork->start_transition = 0x209;
    if ((Engine_GameFlagIsSet(0x927) != 0 || Engine_GameFlagIsSet(0x928) != 0) && Engine_GameFlagIsSet(0x93e) == 0
        && Engine_GameFlagIsSet(0x8a0) == 0) {
        Data_02009940 = (u16)Engine_RandomNext();
        Data_02009928 = (u16)Engine_RandomNext();
        Engine_TaskAddCallback(Local_020010a0, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x925) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
        Call3(Engine_ActorSetPosition, 8, 0xa40000, 0x1480000);
    }
    switch (Data_02000240_t.halves[225][0]) {
    case 1:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            leader = Engine_ActorGet(0);
            Engine_EventBegin();
            Main_0808a2b0();
            leader->y.fixed = 0x380000;
            Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
            Engine_TaskWait(1);
            Engine_CameraFollowActor(0, 0);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Engine_EventEnd();
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3b0_02000468();
        } else {
            FieldScene_RunScene3b0_0200040c();
        }
        break;
    case 11:
        Data_02000240_t.halves[226][0] = (s32)Data_0000006f;
        Data_02000240_t.halves[227][0] = 30;
        Scene_RunFourActorStagingSequence();
        break;
    case 12:
        Data_02000240_t.halves[226][0] = (s32)Data_0000006f;
        Data_02000240_t.halves[227][0] = 30;
        FieldScene_RunActorNinePresentationCycles();
        break;
    case 13:
        Data_02000240_t.halves[226][0] = (s32)Data_0000006f;
        Data_02000240_t.halves[227][0] = 30;
        FieldScene_RunPrimarySequence();
        break;
    case 14:
        Scene_RunSevenActorEnsemble();
        break;
    }
    return 0;
}
