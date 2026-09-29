#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gKuupuappuHeyaEventsEntrances15To17[];
extern const struct SceneEvent gKuupuappuHeyaEventsFlag855[];
extern const struct SceneEvent gKuupuappuHeyaEvents[];

/* What the Vault houses answer: entrances 15 to 17 have their own events,
   and the others change once flag 0x855 is set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    /* FAKEMATCH: a temporary holding the first entrance keeps the compiler
       from rewriting the test as greater than 14. */
    s32 first = 15;

    if (gGameState.entrance <= 17) {
        if (gGameState.entrance >= first) {
            return gKuupuappuHeyaEventsEntrances15To17;
        }
    }
    if (GameFlag_IsSet(0x855) != 0) {
        return gKuupuappuHeyaEventsFlag855;
    }
    return gKuupuappuHeyaEvents;
}
