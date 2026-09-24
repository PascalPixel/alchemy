/* NONMATCHING: 188 of 176 bytes, 47 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 47 halfword edits, 188 of 176 bytes: the link-reset scene (flag 0x302 picks which handshake runs first) is written with one exit. The fail block writes IME through its own address register (the reference stores r0 = 0x04000208 into IME, spelled here as *ime_reg = (u32)ime_reg, FAKEMATCH when closed) but the IME read and write schedule after the state stores, the zero result is rematerialised after the flag call instead of living in r6 across it, and a literal pool is dumped after the first branch. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Local_02000580(void);
s32 Local_02000398(void);
void Main_080770e8(s32 flag, s32 value);

struct LinkState {
    u8 unknown_0;
    u8 mode;
    u8 unknown_2;
    u8 unknown_3;
};

extern u8 Data_020023a0;
extern struct LinkState Data_02002220;
extern s32 Data_02002080;
extern u16 Data_02002008;
extern s32 Data_020023ac;
extern u16 Data_02002238;

s32 Local_020007b0(void)
{
    s32 result;
    s32 first;
    u16 ime;
    s32 flag;
    volatile u16 *ime_reg;

    result = 0;
    flag = Engine_GameFlagIsSet(0x302);
    Data_020023a0 = result;
    if (!flag) {
        Engine_TaskWait(5);
        result = Local_02000580();
        if (result < 0)
            goto fail;
        Engine_TaskWait(5);
        first = result = Local_02000398();
        if (result < 0)
            goto check;
    } else {
        first = result = Local_02000398();
        if (result < 0)
            goto fail;
        Engine_TaskWait(10);
        result = Local_02000580();
        if (result < 0)
            goto fail;
    }
    Main_080770e8(0x3f0, first);
    result = first;
check:
    if (first < 0) {
fail:
        ime_reg = (volatile u16 *)0x04000208;
        ime = *ime_reg;
        *ime_reg = (u32)ime_reg;
    Data_02002220.mode = 0x80;
    Data_02002080 = 0;
    Data_02002008 = 0;
    Data_020023ac = 0;
    Data_02002220.unknown_3 = 0;
    Data_02002220.unknown_2 = 0;
    Data_02002238 = 0;
        *ime_reg = ime;
    }
    return result;
}
