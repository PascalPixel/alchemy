#include "types.h"

struct FxSprite {
    u8 unk00[91];
    u8 visible;
};

struct FxWork {
    u8 unk000[394];
    u8 state;
    u8 busy;
    u8 unk18c[64];
    struct FxSprite *sprite;
};

struct FadeWork {
    u8 unk000[0x53a];
    u8 level;
    u8 step;
    u8 active;
    s8 pending;
};

void *Runtime_AllocateBlock();
void WaitFrames();
void Func_08020268();

void Func_080cb82c(void)
{
    struct FxWork *work = Runtime_AllocateBlock(108, 0xd50);

    if (*(u16 *)&work->state == 3) {
        struct FadeWork *fade;
        s32 zero;

        work->busy = 1;
        /* FAKEMATCH: a one-pass block keeps the busy store ahead of the next field offset; as plain statements the offset is advanced first (2 differ). */
        do {
        } while (0);
        work->sprite->visible = 1;
        zero = 0;
        fade = Runtime_AllocateBlock(124, 0x540);
        if (fade != NULL && fade->pending != 0) {
            fade->level = 80;
            fade->step = zero;
            fade->active = 1;
            fade->pending = zero;
            WaitFrames(2);
        }
        Func_08020268();
    }
}
