#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
s32 Audio_Check(void);

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}
