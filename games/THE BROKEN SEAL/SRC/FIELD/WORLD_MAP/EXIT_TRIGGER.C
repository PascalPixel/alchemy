#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern struct SceneEvent gWorldMapEvents[];

void WorldMap_RestoreExitTrigger(void)
{
    s32 i;

    i = 0;
    while (1) {
        if (gWorldMapEvents[i].control == EVENT_TOUCH && gWorldMapEvents[i].trigger == 138) {
            gWorldMapEvents[i].control = EVENT_EXIT;
            gWorldMapEvents[i].value = 33;
            break;
        }
        if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
            break;
        i++;
    }
}
