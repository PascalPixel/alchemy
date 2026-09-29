#include "TOPIC.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gTorebiIzumiEvents2[];
extern const struct SceneEvent gTorebiIzumiEventsOther[];

/* What the spring answers; its second row answers its own way. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        return gTorebiIzumiEvents2;
    }
    return gTorebiIzumiEventsOther;
}
