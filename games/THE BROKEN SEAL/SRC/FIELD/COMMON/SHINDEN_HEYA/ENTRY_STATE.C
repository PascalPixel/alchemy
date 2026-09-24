#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyTwoRects(void);
void FieldScene_RunScriptedSceneSequence(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Sanctum entry: record the arrival, fade in, then restore the room or run the entrance's scene. */
s32 ShindenHeya_ApplyEntryState(void)
{
    gEventWork->start_transition = 0x209;
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    switch (Data_02000240_t.halves[225][0]) {
    case 10:
    case 11:
    case 12:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3(Engine_ActorSetPosition, 10, 0xc80000, 0x500000);
        }
        Engine_GameFlagClear(0x12f);
        break;
    case 20:
        SceneState_ApplyTwoRects();
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
    case 29:
    case 32:
    case 35:
        Engine_GameFlagClear(0x12f);
        break;
    case 21:
        SceneState_ApplyTwoRects();
        Engine_GameFlagSet(0x201);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
        Engine_GameFlagClear(0x12f);
        break;
    }
    return 0;
}
