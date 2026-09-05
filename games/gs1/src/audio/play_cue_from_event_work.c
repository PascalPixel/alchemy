#include "types.h"

struct Party {
    u8 unk00[0x1f0];
    s16 cue;
};

extern struct Party Data_02000240;

extern void Audio_PlayCue(s16 arg0);

void Audio_PlayCueFromEventWork(void) {
    Audio_PlayCue(Data_02000240.cue);
}
