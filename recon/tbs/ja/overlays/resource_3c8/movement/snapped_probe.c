/* NONMATCHING: Japanese grid-centered Venus movement, 2026-10-01.
 * The complete two-tile movement owner compiles with approved TBS flags but
 * snaps its probes to grid centers and calls the polar-offset service. That
 * makes it twenty-four bytes longer than the Japanese direction-table form.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgVinasuThereWordsCarvedIntoRelief);

extern u8 *gActorEffectWork;

extern const struct SceneEntrance gVinasuHeyaEntrances1[];
extern const struct SceneEntrance gVinasuHeyaEntrances3[];
extern const struct SceneEntrance gVinasuHeyaEntrances4[];
extern const struct SceneEntrance gVinasuHeyaEntrances5[];
extern const struct SceneEntrance gVinasuHeyaEntrances6[];
extern const struct SceneEntrance gVinasuHeyaEntrancesOther[];

extern u8 gVinasuHeyaPlacements1[];
extern u8 gVinasuHeyaPlacements2[];
extern u8 gVinasuHeyaPlacements3[];
extern u8 gVinasuHeyaPlacements4[];
extern u8 gVinasuHeyaPlacements5[];
extern u8 gVinasuHeyaPlacements6[];
extern u8 gVinasuHeyaPlacementsOther[];
void FieldScene_PrepareActors(u8 *placements);

extern u8 gVinasuLeaderApproachScript[];
s32 SceneEffect_SpawnRandomEveryEightFramesB();
extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgFieldVenusLighthouseWasAttackedBy[];
extern u8 MsgVinasuHmmmWeCantPushBlock[];
extern u8 MsgVinasuIveWaitedLongSeeIts[];
extern u8 MsgVinasuStatueSpeaksRobinSoulYe[];

extern u8 MsgVinasuThoughtIdExploreAfterDoor[];

void OverlayObject_WaitUntilIdle();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_MapCopyCellAttributes();
void Engine_ActorMoveToAndWait();
void Engine_ActorSetAnimation();
void Engine_EventEnd();
void Engine_ActorRunRepeatedMotion();
void Object_SetActionById();
void Engine_CameraMoveTo();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetChildValue();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject_02000cc8 *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Actor_Get(ACTOR_PARTY_LEADER);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Vector_AddPolarOffset(0x100000, m, &vec);
    if (Object_CheckMovementCollision(obj, &vec) != 1 && SceneData_FindSlotAtPosition(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (SceneData_FindSlotAtPosition(&vec, obj) == 0 && Object_CheckMovementCollision(obj, &vec) == 0) {
            Engine_EventBegin();
            Object_SetMode(obj, 6);
            Engine_TaskWait(6);
            Audio_PlayCue(152);
            Object_SetMode(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Engine_ActorSetSpriteFlags(obj, 0);
            Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Object_SetMode(obj, 6);
            Engine_ActorSetSpriteFlags(obj, 1);
            *state = old;
            Engine_EventEnd();
            return 1;
        }
    }
    return 0;
}
