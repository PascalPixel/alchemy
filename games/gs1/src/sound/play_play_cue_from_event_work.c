#include "types.h"
#include "scene.h"

struct Party {
    u8 unk00[0x1f0];
    s16 cue;
};

extern struct Party gCell;

extern void Audio_PlayCue(s16 arg0);

void Audio_PlayCueFromEventWork(void)
{
    Audio_PlayCue(gCell.cue);
}
