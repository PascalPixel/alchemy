#include "types.h"
#include "scene.h"
#include "abi/audio/command/stop_all_players.h"

void AudioCommand_StopAllPlayers(void)
{
    Audio_Run();
}
