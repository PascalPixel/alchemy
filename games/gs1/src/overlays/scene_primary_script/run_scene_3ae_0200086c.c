#include "types.h"

#define FieldScene_RunScene3ae_0200086c Func_0200086c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000006b[];
extern u8 Data_0000006c[];
extern u8 Data_00000070[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001164();
void Func_020011fc();
void Func_02001384();
void Func_02001ddc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 FieldScene_RunScene3ae_0200086c(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 90) {
        Call1(Func_02001ddc, 0x950);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000006b) {
        Func_02001164();
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000070) {
            Func_020011fc();
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000006c) {
                Func_02001384();
            }
        }
    }
    return 0;
}
