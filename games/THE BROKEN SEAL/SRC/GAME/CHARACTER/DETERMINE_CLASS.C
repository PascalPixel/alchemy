#include "TYPES.H"

/* An 84-byte class record: the class series it belongs to and the Djinn
   level each element needs, in tens. */
struct ClassDefinition {
    s32 series;                 /* 0x00 */
    u8 required[4];             /* 0x04 */
    u8 unknown_08[0x4c];
};

extern struct ClassDefinition Data_08084b1c[];

void Owner_GetDigitValues(s32 character, const u8 *djinn, s32 *levels);
s32 GameFlag_Test(s32 flag);
s32 Owner_LookupFourColumnTable(s32 primary, s32 j);

/* The class a character holds with the given set Djinn: the two strongest
   elements pick the class series, and the last class of that series whose
   element requirements are met wins. Some characters have fixed classes. */
s32 Owner_DetermineClass(s32 character, const u8 *djinn)
{
    s32 levels[4];
    s32 primary;
    s32 best;
    s32 series;
    s32 result;
    s32 j; /* the second element, then each requirement */
    s32 i;

    result = -1;
    if (character > 7)
        return 0;
    Owner_GetDigitValues(character, djinn, levels);
    if (GameFlag_Test(32)) {
        if (character == 0)
            return 200;
        if (character == 1)
            return 201;
    }
    if (character == 5)
        return 202;
    if (result != -1)
        return result;
    best = result;
    primary = result;
    for (i = 0; i < 4; i++) {
        if (best < levels[i]) {
            best = levels[i];
            primary = i;
        }
    }
    j = -1;
    best = -1;
    for (i = 0; i < 4; i++) {
        if (i != primary && best < levels[i]) {
            best = levels[i];
            j = i;
        }
    }
    series = Owner_LookupFourColumnTable(primary, levels[j] > 9 ? j : primary);
    for (i = 202; i >= 0; i--) {
        if (Data_08084b1c[i].series != series)
            continue;
        for (j = 0; j < 4; j++) {
            if (levels[j] < Data_08084b1c[i].required[j] * 10)
                break;
        }
        if (j == 4) {
            result = i;
            break;
        }
    }
    if (result == -1)
        result = 0;
    return result;
}
