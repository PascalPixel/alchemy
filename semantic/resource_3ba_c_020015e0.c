#include "types.h"

extern s16 Data_02000240[];
extern void Func_02001b5c(s32); extern s32 Func_02001d64(s32,s32); extern void Func_02001e20(s32,s32);
extern void Func_02001e7c(s32,s32,s32); extern void Func_02000134(void); extern void Func_02000158(void);
extern void Func_02000178(void); extern void Func_02002910(s32,s32,s32); extern void Func_02002aec(s32);
extern void Func_0808a018(void); extern void Func_0808a020(void); extern void Func_0808a170(s32);
extern void Func_0808a208(s32,s32); extern void Func_0808a210(s32,s32,s32,s32); extern void Func_0808a218(void);
extern void Func_0808a180(s32,s32); extern void Func_0808a010(s32); extern void Func_0808a1b8(s32,s32,s32);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a0d0(s32,s32,s32); extern void Func_0808a1e8(s32,s32,s32);
extern void Func_0808a200(s32,s32);

void Func_020015e0(s32 actor)
{
    s32 phase;

    if (Data_02000240[225] == 2) {
        Func_02001b5c(actor);
        return;
    }

    Func_0808a018();
    phase = Func_02001d64(actor, 3);
    if (phase == 0) {
        Func_0808a170(0x2095);
        Func_02000134();
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x03480000, -1, 0x00d80000, 1);
        Func_0808a218();
        Func_0808a180(actor, 0);
        Func_02000158();
        Func_0808a010(60);
        Func_0808a180(actor, 0);
        Func_02002910(0, 736, 200);
        Func_0808a1b8(0, 0, 0);
        Func_02000178();
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 816, 200);
        Func_0808a010(30);
        Func_0808a1e8(0, 0x105, 60);
        Func_0808a180(actor, 0);
        Func_02002aec(0);
        Func_0808a200(0, 0);
        Func_02001e20(actor, 3);
    } else if (phase == 1) {
        Func_0808a170(0x2094);
        Func_0808a180(actor, 0);
    }

    Func_02001e7c(phase, actor, 3);
    Func_0808a020();
}
