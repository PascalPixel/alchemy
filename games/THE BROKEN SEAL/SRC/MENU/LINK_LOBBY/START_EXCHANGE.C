/* Leave the link lobby for the exchange: stop the serial interrupts, load
 * the sound preset and go to scene row 1's first entrance. */
#include "TYPES.H"
#include "SCENE_IDS.H"

void SerialRuntime_RemoveIrqHandlers(void);
void Sound_LoadPresetParameters(s32 preset);
s32 Event_SetPairWork1c0(s32 scene, s32 entrance);

s32 LinkLobby_StartExchange(void)
{
    SerialRuntime_RemoveIrqHandlers();
    Sound_LoadPresetParameters(2);
    /* FAKEMATCH: the do/while loads the scene number before the 1. */
    do {
        return Event_SetPairWork1c0((s32)&SceneId_Clear, 1);
    } while (0);
}
