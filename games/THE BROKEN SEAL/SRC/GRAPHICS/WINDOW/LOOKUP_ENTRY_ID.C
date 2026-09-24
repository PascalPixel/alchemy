#include "TYPES.H"

extern const s16 SideObject_CharacterIdMap[];
extern const s16 SideObject_ActorKindIdMap[];

/* Looks up the entry id paired with value: values below 20 search the
 * character pairs, others the second pair table, whose ids start at 128.
 * Returns -1 when the value is not listed. */
s32 Localization_LookupEntryId(u32 value)
{
    s32 result = -1;
    s32 i = 0;

    if (value < 20) {
        for (;;) {
            s32 key = SideObject_CharacterIdMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = SideObject_CharacterIdMap[i];
                break;
            }
            i += 2;
        }
    } else {
        for (;;) {
            s32 key = SideObject_ActorKindIdMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = SideObject_ActorKindIdMap[i];
                result += 128;
                break;
            }
            i += 2;
        }
    }
    return result;
}
