#include "types.h"
#include "scene.h"
#include "abi/audio/command/resume_all_players.h"

void AudioCommand_ResumeAllPlayers(void)
{
    Audio_Run();
}
