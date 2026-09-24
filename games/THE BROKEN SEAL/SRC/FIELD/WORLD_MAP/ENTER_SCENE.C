#include "TYPES.H"
#include "FIELD_EVENT.H"

void Scene_RunScene371SequenceA(s32 direction);
void Main_0808a238(s32 a0, s32 a1);
void Engine_PartyRemoveItem(s32 item);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Main_080091d0(s32 a0, s32 a1, s32 a2, s32 a3);
s32 Main_080770e0(s32 flag);
void WorldMap_PrepareTriggerActor(s32 actor);
void WorldMap_RunActorSequence(void);
void RunEventScript01(void);
void FieldScene_RunActorTransferSequence(void);
void FieldScene_RunScene371_020017fc(void);
void FieldScene_RunScene371_02001888(void);
void FieldScene_RunScene371_02001938(void);
void FieldScene_RunScene371_020019e8(void);
void FieldScene_RunScene371_02001a98(void);
void FieldScene_RunScene371_02001b5c(void);
void FieldScene_RunScene371_02002274(void);
void FieldScene_RunScene371_0200357c(void);
void StoryScene_StartTransition(void);
void FieldScene_RunActorEightApproach(void);
void FieldScene_RunActorPresentationSequence(void);
void Local_02000598(void);
void Local_02003678(void);

extern s32 Data_0200e79c;
extern u8 Data_0000003a[];
extern u8 Data_000000bb[];
extern s16 Data_02000240_t[][1];

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* World map entry: record the arrival and start the map's camera and tasks, then run the scene the entrance or the story flags call for. */
s32 WorldMap_EnterScene(void)
{
    s16 *entrance;
    u8 *state;

    state = (u8 *)&gGameState;
    entrance = &((struct GameState *)state)->entrance;
    if (*entrance == 99) {
        Engine_GameFlagSet(0x160);
        Engine_GameFlagSet(0x161);
        Engine_GameFlagSet(0x163);
    }
    if (*entrance == 90) {
        Scene_RunScene371SequenceA(0);
        Main_0808a238((s32)Data_0000003a, 1);
    } else if (*entrance == 91) {
        Scene_RunScene371SequenceA(1);
        Main_0808a238((s32)Data_000000bb, 93);
    } else if (*entrance == 78) {
        Engine_EventBegin();
        Engine_PartyRemoveItem(242);
        Engine_EventRequestExit(112);
    } else {
        Engine_GameFlagSet(0x144);
        gEventWork->start_transition = 0x400;
        gEventWork->transition_frames = 16;
        Engine_TaskWait(1);
        Engine_CameraSetSpeed(0x80000, 0x10000);
        Engine_GameFlagClear(0x12f);
        Engine_TaskAddCallback(Local_02000598, 0xc80);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            Call4(Main_080091d0, 128, 256, 176, 56);
        }
        switch (*entrance) {
        case 1:
            if (Value1(Engine_GameFlagIsSet, 0x815) == 0) {
                Call1(Engine_GameFlagSet, 0x815);
                Engine_GameFlagSet(0x85c);
            }
            break;
        case 33:
            if (Engine_GameFlagIsSet(0x109) != 0) {
                if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x234) != 0) {
                    Data_0200e79c = 55;
                    Call3(Engine_ActorSetPosition, 55, 0x17940000, 0xd480000);
                    Engine_ActorGet(Data_0200e79c)->facing = 0x3000;
                    WorldMap_PrepareTriggerActor(Data_0200e79c);
                }
            } else if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x9b8) == 0) {
                WorldMap_RunActorSequence();
            }
            break;
        case 49:
            if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0) {
                RunEventScript01();
            }
            break;
        case 64:
            if (Engine_GameFlagIsSet(0x85a) == 0) {
                FieldScene_RunActorTransferSequence();
            }
            break;
        case 65:
            FieldScene_RunScene371_020017fc();
            break;
        case 66:
            FieldScene_RunScene371_02001888();
            break;
        case 67:
            FieldScene_RunScene371_02001938();
            break;
        case 68:
            FieldScene_RunScene371_020019e8();
            break;
        case 69:
            FieldScene_RunScene371_02001a98();
            break;
        case 70:
            FieldScene_RunScene371_02001b5c();
            break;
        case 71:
            FieldScene_RunScene371_02002274();
            break;
        case 72:
            FieldScene_RunScene371_0200357c();
            break;
        case 73:
            StoryScene_StartTransition();
            break;
        case 74:
        case 76:
        case 77:
            Engine_GameFlagSet(0x11c);
            if (Main_080770e0(0x2f8) != 0) {
                state = (u8 *)&gGameState;
                state[0x1f2] = 2;
                Engine_TaskAddCallback(Local_02003678, 0xc80);
            }
            break;
        case 75:
            FieldScene_RunActorEightApproach();
            break;
        case 80:
            FieldScene_RunActorPresentationSequence();
            break;
        default:
            Engine_ActorGet(53)->scale_x = 0x14000;
            Engine_ActorGet(53)->scale_y = 0x14000;
            break;
        }
    }
    return 0;
}
