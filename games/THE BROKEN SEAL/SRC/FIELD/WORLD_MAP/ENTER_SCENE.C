#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void Scene_RunScene371SequenceA(s32 direction);
void Event_SetPairWork1c0(s32 a0, s32 a1);
void PartyInventory_Discard(s32 item);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Map_SetWindowCellTile(s32 a0, s32 a1, s32 a2, s32 a3);
s32 GameFlag_GetByte(s32 flag);
void WorldMap_ActivateSite138(s32 actor);
void WorldMap_RunBlackOrbScene(void);
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
void MapActor_UpdateContact(void);
void StoryScene_UpdateSelectedActorProgress(void);

extern s32 gWorldMapTriggerActor;

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
        Event_SetPairWork1c0((s32)&SceneId_MakyuriChojo1, 1);
    } else if (*entrance == 91) {
        Scene_RunScene371SequenceA(1);
        Event_SetPairWork1c0((s32)&SceneId_VinasuChojo, 93);
    } else if (*entrance == 78) {
        Engine_EventBegin();
        PartyInventory_Discard(242);
        Engine_EventRequestExit(112);
    } else {
        Engine_GameFlagSet(0x144);
        gEventWork->start_transition = 0x400;
        gEventWork->transition_frames = 16;
        Engine_TaskWait(1);
        Engine_CameraSetSpeed(0x80000, 0x10000);
        Engine_GameFlagClear(0x12f);
        Engine_TaskAddCallback(MapActor_UpdateContact, 0xc80);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            Call4(Map_SetWindowCellTile, 128, 256, 176, 56);
        }
        switch (*entrance) {
        case 1:
            if (Value1(Engine_GameFlagIsSet, 0x815) == 0) {
                Engine_GameFlagSet(0x815);
                Engine_GameFlagSet(0x85c);
            }
            break;
        case 33:
            if (Engine_GameFlagIsSet(0x109) != 0) {
                if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x234) != 0) {
                    gWorldMapTriggerActor = 55;
                    Call3(Engine_ActorSetPosition, 55, 0x17940000, 0xd480000);
                    Engine_ActorGet(gWorldMapTriggerActor)->facing = 0x3000;
                    WorldMap_ActivateSite138(gWorldMapTriggerActor);
                }
            } else if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x9b8) == 0) {
                WorldMap_RunBlackOrbScene();
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
            if (GameFlag_GetByte(0x2f8) != 0) {
                state = (u8 *)&gGameState;
                state[0x1f2] = 2;
                Engine_TaskAddCallback(StoryScene_UpdateSelectedActorProgress, 0xc80);
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
