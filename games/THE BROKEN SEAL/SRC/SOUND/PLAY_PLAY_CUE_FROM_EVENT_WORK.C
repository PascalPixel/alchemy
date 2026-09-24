#include "TYPES.H"
#include "SCENE.H"

struct Party {
    u8 unk00[0x1f0];
    s16 cue;
};

extern struct Party gGameState;

extern void Audio_PlayCue(s16 arg0);

void Audio_PlayCueFromEventWork(void)
{
    Audio_PlayCue(gGameState.cue);
}
