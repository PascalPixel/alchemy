#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef void (*ObjectCallback_0809537c)(void *);

void *Func_080090c8(s16, s32, s32, s32);
void Func_08009080(void *, s32);
void Func_08009158(void *);
void Func_080091e0(void *, s32);
void Func_08009240(void *, s32);
void Func_080030f8(s32);
void Func_080916b0(void);
void Func_08091750(void);
void *Func_08092054(s32);
void Func_08092560(s32, s32, s32);
void Func_080925cc(s32);
void Func_08092adc(s32, s32, s32);
void *Func_08096c48(void *, void *);
void Func_08096bec(void *, s32, s32);
void Func_08095348(void *);
void Func_080f9010(s32);

void Func_0809537c(s32 slot)
{
    void *primary = Func_08092054(slot);
    s32 current_slot = *(s32 *)0x02000434;
    void *current = Func_08092054(current_slot);
    s32 phase = (FIELD(current, u16, 6) + 0x2000) & 0xc000;
    s32 heading = phase + 0x8000;
    void *children[8];
    void *previous = 0;
    void *parent = primary;
    s16 kind;
    s32 animation = 9;
    s32 index;

    Func_080916b0();
    Func_080030f8(10);
    Func_080f9010(173);
    Func_080925cc(slot);
    Func_080f9010(175);
    Func_080925cc(slot);
    Func_080030f8(20);
    Func_08092adc(slot, heading, 0);
    Func_080030f8(10);

    FIELD(FIELD(primary, void *, 0x50), u8, 9) &= (u8)~0x0c;
    FIELD(primary, u16, 6) = heading;
    FIELD(Func_08092054(slot), u8, 0x5a) &= 0xfe;
    FIELD(primary, u8, 0x55) = 2;

    for (index = 0; index < 3; index++) {
        Func_08096bec(primary, 0x100000, phase);
        Func_080f9010(152);
        Func_08092560(slot, 4, 0);
        Func_08009158(primary);
    }
    Func_080030f8(20);

    kind = *FIELD(FIELD(primary, void *, 0x50), s16 *, 0x28);
    if (kind == 90)
        animation = 2;
    if (kind == 92)
        animation = 10;
    if (kind == 91)
        animation = 9;

    for (index = 0; index < 8; index++) {
        void *child = Func_080090c8(
            kind,
            FIELD(primary, s32, 8),
            FIELD(primary, s32, 12),
            FIELD(primary, s32, 16));
        children[index] = child;
        if (child != 0) {
            void *part;

            FIELD(child, s32, 0x18) = 0xf000;
            FIELD(child, s32, 0x1c) = 0xf000;
            FIELD(child, u8, 0x55) = 0;
            FIELD(child, u8, 0x23) = 2;
            FIELD(child, u8, 0x5a) |= 1;
            FIELD(child, ObjectCallback_0809537c, 0x6c) =
                Func_08095348;
            FIELD(child, u16, 6) = FIELD(primary, u16, 6);
            part = FIELD(child, void *, 0x50);
            FIELD(part, u8, 9) &= (u8)~0x0c;
            Func_08009240(child, animation);
            Func_08009080(child, 0);
            Func_080091e0(child, 0);
            previous = Func_08096c48(part, previous);
            FIELD(child, void *, 0x68) = parent;
            parent = child;
        }
    }

    Func_08096bec(primary, 0x400000, phase + 0x8000);
    Func_080f9010(136);
    Func_08092560(slot, 12, 0);
    Func_080030f8(24);
    FIELD(primary, u8, 0x55) = 0;
    FIELD(primary, s32, 0x24) = 0;
    FIELD(primary, s32, 0x2c) = 0;
    FIELD(primary, s32, 0x28) = 0;
    FIELD(primary, s32, 0x38) = (s32)0x80000000;
    FIELD(primary, s32, 0x40) = (s32)0x80000000;
    FIELD(primary, s32, 0x3c) = (s32)0x80000000;
    Func_08009080(primary, 0);
    FIELD(FIELD(primary, void *, 0x50), u8, 9) =
        (FIELD(FIELD(primary, void *, 0x50), u8, 9) & (u8)~0x0c) | 8;
    Func_08091750();
}
