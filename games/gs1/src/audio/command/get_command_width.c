#include "types.h"
#include "scene.h"
#include "abi/audio/command/get_command_width.h"

s32 AudioCommand_GetWidth(s32 command)
{
    if (command == 0x46 || command == 0x4b || command == 0x43)
        return 3;
    return 2;
}
