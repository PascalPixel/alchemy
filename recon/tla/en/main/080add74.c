#include "SCENE.H"
#include "INVENTORY.H"
#include "GAME_FLAGS.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "PARTY_STATE.H"

extern struct PartyState gGameState;
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_SetBit(s32 flag);

/* game_flags/refresh_lure_cap.c */

u16 Runtime_GetBuildStampTime(void)
{
    u8 *digits;
    s32 hourTens;
    s32 hourUnits;
    s32 minuteTens;
    s32 minuteUnits;
    s32 secondTens;
    s32 secondUnits;
    s32 hours;
    s32 minutes;
    s32 seconds;
    s32 packed;
    s32 shifted;
    s32 result;

    digits = Resource_GetTableEntry((s32)&ResourceId_BuildStamp);
    hourTens = *digits;
    hours = (hourTens - '0') * 10;
    digits++;
    hourUnits = *digits;
    digits++;
    hours += hourUnits - '0';
    minuteTens = *digits;
    minutes = (minuteTens - '0') * 10;
    digits++;
    minuteUnits = *digits;
    digits++;
    minutes += minuteUnits - '0';
    secondTens = digits[0];
    seconds = (secondTens - '0') * 10;
    secondUnits = digits[1];
    seconds += secondUnits - '0';
    packed = (((hours << 4) + minutes) << 6) + seconds;
    shifted = 0x80 << 21;
    shifted |= packed << 16;
    result = shifted >> 16;
    if (gDebugMode != 0) {
        result |= (s32)0xffff8000;
    }
    return (u16)result;
}
