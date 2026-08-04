#include "types.h"

extern u8 *Func_0808a080(s32 actor);
extern void Func_080000d0(void (*callback)(void), s32 interval);
extern void Func_0808a3c0(s32, s32, s32);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a100(s32 actor, s32 pose);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_08009088(u8 *actor, s32 value);
extern void Func_02001e94(void);
extern void Func_02001edc(void);
extern void Func_02001ac8(void);
extern void Func_02001c28(void);
extern void Func_02001cf8(void);
extern void Func_02001e54(void);
extern void Func_02001e74(void);
extern void Func_02001df8(void);
extern void Func_02001984(void);
extern void Func_02000aa8(void);

/* Install the scene callbacks and position its flag-selected actors. */
void Func_02002334(void)
{
    u8 *actor = Func_0808a080(14);

    actor[85] = 0;
    Func_080000d0(Func_02001e94, 2000);
    Func_080000d0(Func_02001edc, 2000);
    Func_0808a3c0(107, 0, 0);
    if (Func_080770c0(0xed9) != 0)
        Func_0808a100(14, 2);

    Func_02001ac8();
    Func_02001c28();
    Func_02001cf8();
    Func_02001e54();
    Func_02001e74();
    Func_0808a1e0(8, 3);
    actor = Func_0808a080(11);
    actor[85] = 0;
    actor = Func_0808a080(12);
    actor[85] = 0;
    Func_02001df8();

    if (Func_080770c0(0x200) != 0) {
        Func_02001984();
        Func_0808a100(13, 5);
    }
    if (Func_080770c0(0x109) != 0)
        return;

    if (Func_080770c0(0x9ca) != 0) {
        Func_0808a0f0(15, 0x3580000, 0x3380000);
        actor = Func_0808a080(15);
        *(void (**)(void))(actor + 108) = Func_02000aa8;
    } else if (Func_080770c0(0x9c9) != 0) {
        Func_0808a0f0(15, 0x3780000, 0x2980000);
        actor = Func_0808a080(15);
        *(u16 *)(*(u8 **)(actor + 80) + 30) = 0;
        Func_08009088(actor, 16);
    } else if (Func_080770c0(0x9c8) != 0) {
        Func_0808a0f0(15, 0x2480000, 0x2a80000);
    } else {
        Func_0808a0f0(15, 0x2480000, 0x2980000);
    }
}
