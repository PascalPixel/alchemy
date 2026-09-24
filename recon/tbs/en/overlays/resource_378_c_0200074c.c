/* NONMATCHING: 252 bytes, candidate 248, 76 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * offset constants: the reference loads 0xe5a and 0xe5c from the pool
 * separately and derives only 0xe5e by adding 2, and loads 0x2a01, 0x2a02 and
 * 0x2a03 separately; here each address is derived from the previous one. The
 * first lamp takes the pool zero (Value_00000000 through a u8), which should
 * also place the pool mid-function. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 Value_00000000;

struct ShrineWork {
    u8 unknown_0000[0xe5a];
    u16 colors[3];
    u8 unknown_0e60[0x2a00 - 0xe60];
    u8 lamps[4];
};

void Main_08015210(s32 message, s32 a1, s32 a2);
s32 Local_020027d4(void);

void Local_0200074c(void)
{
    struct ShrineWork *work;
    u8 zero;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 0);
    Engine_ActorSetAnimation(1, 0);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorSetAnimation(12, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorSetAnimation(10, 0);
    Engine_ColorBufferApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(180);
    work = *(struct ShrineWork **)0x03001ed0;
    work->colors[0] = 0x7c00;
    work->colors[1] = 0x7c00;
    work->colors[2] = 0x7c00;
    zero = (u8)(u32)&Value_00000000;
    work->lamps[0] = zero;
    work->lamps[1] = 1;
    work->lamps[2] = 1;
    work->lamps[3] = 1;
    Engine_EventWait(1);
    Main_08015210(0x116d, 1, 0);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Engine_EventWait(60);
    if (Local_020027d4() == 0) {
        Engine_EventEnd();
        Engine_EventRequestExit(20);
    } else {
        Engine_EventEnd();
        Engine_EventRequestExit(50);
    }
}
