#include "TYPES.H"
extern u8 Data_0200b39c[];
extern u8 Data_0200b5f4[];
extern u8 Data_0200b7bc[];
extern u8 Data_0200b87c[];
extern u8 Data_0200b99c[];
extern u8 Data_0200b75c[];
extern u8 Data_0200be1c[];
extern u8 Data_0200bbdc[];
extern u8 Data_0200ba44[];
extern u8 Data_0200b324[];

s32 Engine_GameFlagIsSet();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_0000008c[];
extern u8 Data_0000008e[];
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

/* Colosso entrance: the event script for the stored area variant; the main
 * hall picks by the story step at +0x1c2, the second area by flags 0x950 and
 * 0x962. */
s32 KorashiamuIriguchi_GetAreaScript(void)
{
    s32 script;

    if (Data_02000240_t[224][0] == (s32)Data_0000008c) {
        switch (Data_02000240_t[225][0]) {
        case 5:
        case 69:
            script = (s32)Data_0200b39c;
            break;
        case 7:
        case 70:
            script = (s32)Data_0200b5f4;
            break;
        case 8:
        case 21:
        case 31:
        case 64:
        case 65:
        case 67:
            script = (s32)Data_0200b7bc;
            break;
        case 12:
            script = (s32)Data_0200b87c;
            break;
        case 66:
        case 68:
            script = (s32)Data_0200b99c;
            break;
        default:
            script = (s32)Data_0200b75c;
            break;
        }
    } else if (Data_02000240_t[224][0] == (s32)Data_0000008e) {
        if (Value1(Engine_GameFlagIsSet, 0x950) != 0) {
            script = (s32)Data_0200be1c;
        } else if (Value1(Engine_GameFlagIsSet, 0x962) != 0) {
            script = (s32)Data_0200bbdc;
        } else {
            script = (s32)Data_0200ba44;
        }
    } else {
        script = (s32)Data_0200b324;
    }
    return script;
}
