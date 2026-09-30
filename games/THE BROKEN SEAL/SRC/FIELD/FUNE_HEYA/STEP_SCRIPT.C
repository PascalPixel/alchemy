#include "TYPES.H"
#include "HEYA.H"
#include "CALL.H"
extern u8 FuneHeya_StepScriptF[];
extern u8 FuneHeya_StepScriptG[];
extern u8 FuneHeya_StepScriptE[];
extern u8 FuneHeya_StepScriptH[];
extern u8 FuneHeya_StepScriptD[];
extern u8 FuneHeya_StepScriptC[];
extern u8 FuneHeya_StepScriptB[];
extern u8 FuneHeya_StepScriptA[];

s32 Engine_GameFlagIsSet();

extern u8 Data_02000240[];
extern s16 gGameState[][1];

s32 FuneHeya_GetStepScript(void)
{
    u8 *script;

    switch (gGameState[225][0]) {
    case 1:
    case 2:
    case 11:
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return (s32)FuneHeya_StepScriptF;
        } else if (Engine_GameFlagIsSet(0x928) != 0) {
            if (Engine_GameFlagIsSet(0x8a0) != 0) {
                script = FuneHeya_StepScriptG;
                script[22] = 2;
                script[70] = 2;
                script[118] = 2;
                script[142] = 2;
                script[214] = 2;
                script[190] = 2;
                script[166] = 1;
                script[94] = 2;
            }
            return (s32)FuneHeya_StepScriptG;
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            if (Engine_GameFlagIsSet(0x925) != 0) {
                FuneHeya_StepScriptF[22] = 2;
                FuneHeya_StepScriptF[118] = 2;
                FuneHeya_StepScriptF[46] = 2;
                FuneHeya_StepScriptF[94] = 2;
            }
            return (s32)FuneHeya_StepScriptF;
        } else {
            return (s32)FuneHeya_StepScriptE;
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
        return (s32)FuneHeya_StepScriptH;
        break;
    case 15:
    case 17:
    case 19:
        script = FuneHeya_StepScriptH;
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
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return (s32)FuneHeya_StepScriptD;
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            if (Engine_GameFlagIsSet(0x922) != 0) {
                if (Value1(Engine_GameFlagIsSet, 0x8a0) != 0) {
                    FuneHeya_StepScriptC[46] = 1;
                }
                if (Engine_GameFlagIsSet(0x925) != 0) {
                    if (!(Engine_GameFlagIsSet(0x8a0) != 0)) {
                        FuneHeya_StepScriptC[22] = 0;
                    }
                }
                return (s32)FuneHeya_StepScriptC;
            } else {
                return (s32)FuneHeya_StepScriptA;
            }
        } else {
            return (s32)FuneHeya_StepScriptB;
        }
        break;
    case 10:
    case 13:
    case 14:
    case 22:
        return (s32)FuneHeya_StepScriptB;
        break;
    default:
        return (s32)FuneHeya_StepScriptA;
        break;
    }
    return (s32)script;
}
