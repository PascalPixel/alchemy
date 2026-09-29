#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gTorebiKyudenEvents2AfterColosso[];
extern const struct SceneEvent gTorebiKyudenEvents2Colosso[];
extern const struct SceneEvent gTorebiKyudenEvents2[];
extern const struct SceneEvent gTorebiKyudenEventsAfterColosso[];
extern const struct SceneEvent gTorebiKyudenEventsColosso[];
extern const struct SceneEvent gTorebiKyudenEventsOther[];

/* What the palace answers: in each scene, one table while Colosso is under
   way (flag 0x962), one once it is over (flag 0x950), one before. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gTorebiKyudenEvents2AfterColosso;
        }
        if (GameFlag_IsSet(0x962) != 0) {
            return gTorebiKyudenEvents2Colosso;
        }
        return gTorebiKyudenEvents2;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return gTorebiKyudenEventsAfterColosso;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return gTorebiKyudenEventsColosso;
    }
    return gTorebiKyudenEventsOther;
}
