#include "types.h"

struct Party {
    u8 pad[0x1f0];
    s16 member;
};

extern struct Party Data_02000240;

extern void Audio_PlayCue(s16 arg0);

void Func_0808acc4(void) {
    Audio_PlayCue(Data_02000240.member);
}
