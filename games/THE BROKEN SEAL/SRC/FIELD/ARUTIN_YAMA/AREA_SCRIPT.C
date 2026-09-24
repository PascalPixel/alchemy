#include "TYPES.H"

extern u8 Data_0200cd6c[];
extern u8 Data_0200cd24[];
extern u8 Data_0200cd9c[];
extern u8 Data_0200cdc0[];
extern u8 Data_0200ce5c[];
extern u8 Data_0200cebc[];
extern u8 Data_0200cf34[];
extern u8 Data_0200cfb8[];
extern u8 Data_0200d06c[];
extern u8 Data_0200d0cc[];
extern u8 Data_0200d12c[];
extern u8 Data_0200d150[];
extern u8 Data_0200cd18[];

s32 Engine_GameFlagIsSet();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_0000004d[];
extern u8 Data_0000004e[];
extern u8 Data_0000004f[];
extern u8 Data_00000050[];
extern u8 Data_00000051[];
extern u8 Data_00000052[];
extern u8 Data_00000053[];
extern u8 Data_00000054[];
extern u8 Data_00000055[];
extern u8 Data_00000056[];
extern u8 Data_00000057[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Altin Peak: the event script table for the area variant stored at +0x1c0
 * of the game state; the first area switches table once flag 0x8fd is set. */
s32 ArutinYama_GetAreaScript(void)
{
    s32 table;

    if (Data_02000240_t[224][0] == (s32)Data_0000004d) {
        if (Value1(Engine_GameFlagIsSet, 0x8fd) != 0) {
            table = (s32)Data_0200cd6c;
        } else {
            table = (s32)Data_0200cd24;
        }
    } else if (Data_02000240_t[224][0] == (s32)Data_0000004e) {
        table = (s32)Data_0200cd9c;
    } else if (Data_02000240_t[224][0] == (s32)Data_0000004f) {
        table = (s32)Data_0200cdc0;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000050) {
        table = (s32)Data_0200ce5c;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000051) {
        table = (s32)Data_0200cebc;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000052) {
        table = (s32)Data_0200cf34;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000053) {
        table = (s32)Data_0200cfb8;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000054) {
        table = (s32)Data_0200d06c;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000055) {
        table = (s32)Data_0200d0cc;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000056) {
        table = (s32)Data_0200d12c;
    } else if (Data_02000240_t[224][0] == (s32)Data_00000057) {
        table = (s32)Data_0200d150;
    } else {
        table = (s32)Data_0200cd18;
    }
    return table;
}
