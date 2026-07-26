#include "types.h"

typedef int bool;
#define NULL ((void *)0)
struct Party {
    u8 pad[0x1f0];
    s16 member;
};

extern struct Party Data_02000240;

extern void Func_080f9010(s16 arg0);

void Func_0808acc4(void) {
    Func_080f9010(Data_02000240.member);
}
