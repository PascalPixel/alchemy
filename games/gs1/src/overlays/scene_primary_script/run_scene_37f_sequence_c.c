#include "types.h"

#define FieldScene_RunScene37fSequenceC Func_020000ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
extern u8 Data_00000010[];
extern u8 Data_00000013[];
extern u8 Data_0200a2e4[];
extern u8 Data_0200a524[];
extern u8 Data_0200a41c[];
extern u8 Data_0200a32c[];
extern u8 Data_0200a2d8[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 FieldScene_RunScene37fSequenceC(void)
{
    s16 scene_id;
    s16 sub;

    scene_id = Data_02000240[224];
    if (scene_id == (s32)Data_00000013) {
        return (s32)Data_0200a2e4;
    } else {
        if (scene_id == (s32)Data_00000010) {
            sub = Data_02000240[225];
            switch (sub) {
            case 11:
            case 12:
            case 13:
                return (s32)Data_0200a41c;
            case 14:
            case 15:
            case 16:
                return (s32)Data_0200a524;
            default:
                return (s32)Data_0200a32c;
            }
        } else {
            return (s32)Data_0200a2d8;
        }
    }
}
