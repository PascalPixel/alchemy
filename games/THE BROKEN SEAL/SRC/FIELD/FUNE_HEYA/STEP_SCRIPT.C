#include "TYPES.H"
extern u8 Data_0200eda8[];
extern u8 Data_0200eeb0[];
extern u8 Data_0200ed78[];
extern u8 Data_0200efe8[];
extern u8 Data_0200ed48[];
extern u8 Data_0200ec58[];
extern u8 Data_0200ebf8[];
extern u8 Data_0200ebe0[];


s32 Engine_GameFlagIsSet();


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

/* Ship cabin: the event script for the story step at +0x1c2 of the game
 * state, patching the chosen script's actor bytes for the later steps. */
s32 FuneHeya_GetStepScript(void)
{
    u8 *script;

    switch (Data_02000240_t[225][0]) {
    case 1:
    case 2:
    case 11:
        if (Value1(Engine_GameFlagIsSet, 0x93e) != 0) {
            return (s32)Data_0200eda8;
        } else if (Value1(Engine_GameFlagIsSet, 0x928) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x8a0) != 0) {
                script = Data_0200eeb0;
                script[22] = 2;
                script[70] = 2;
                script[118] = 2;
                script[142] = 2;
                script[214] = 2;
                script[190] = 2;
                script[166] = 1;
                script[94] = 2;
            }
            return (s32)Data_0200eeb0;
        } else if (Value1(Engine_GameFlagIsSet, 0x911) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x925) != 0) {
                Data_0200eda8[22] = 2;
                Data_0200eda8[118] = 2;
                Data_0200eda8[46] = 2;
                Data_0200eda8[94] = 2;
            }
            return (s32)Data_0200eda8;
        } else {
            return (s32)Data_0200ed78;
        }
        break;
    case 4:
    case 12:
    case 16:
    case 18:
    case 20:
    case 21:
    case 23:
    case 24:
        return (s32)Data_0200efe8;
        break;
    case 15:
    case 17:
    case 19:
        script = Data_0200efe8;
        script[22] = 2;
        script[46] = 2;
        script[94] = 1;
        script[118] = 2;
        script[142] = 2;
        script[166] = 2;
        script[190] = 2;
        script[214] = 1;
        script[238] = 2;
        return (s32)script;
    case 5:
        if (Value1(Engine_GameFlagIsSet, 0x93e) != 0) {
            return (s32)Data_0200ed48;
        } else if (Value1(Engine_GameFlagIsSet, 0x911) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x922) != 0) {
                if (Value1(Engine_GameFlagIsSet, 0x8a0) != 0) {
                    Data_0200ec58[46] = 1;
                }
                if (Value1(Engine_GameFlagIsSet, 0x925) != 0) {
                    if (!(Value1(Engine_GameFlagIsSet, 0x8a0) != 0)) {
                        Data_0200ec58[22] = 0;
                    }
                }
                return (s32)Data_0200ec58;
            } else {
                return (s32)Data_0200ebe0;
            }
        } else {
            return (s32)Data_0200ebf8;
        }
        break;
    case 10:
    case 13:
    case 14:
    case 22:
        return (s32)Data_0200ebf8;
        break;
    default:
        return (s32)Data_0200ebe0;
        break;
    }
    return (s32)script;
}
