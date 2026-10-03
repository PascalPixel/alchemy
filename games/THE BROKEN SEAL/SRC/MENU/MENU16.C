#include "CHARACTER_MENU.H"
#include "BATTLE_UNIT.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_RUNTIME.H"

extern u8 gNumberTextBuffer[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void Text_FormatSignedDecimalToWork(s32 out);

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    /* FAKEMATCH: no caller uses a result. The original scalar boundary keeps
       pop {r1}; the ordinary void definition changes it to pop {r0}. */
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = (u8 *)((u32)&gNumberTextBuffer);
    n = 0xD;
    do {
        n -= 1;
        *dst = (s16)*src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}

/* Lists the djinn that differ between two 32-slot grids: first those only in
   the new grid (flagged 0x8000), then those only in the old one (0x4000). */
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost)
{
    s32 i;
    s32 j;
    s32 count;
    s32 added;
    s32 removed;

    added = 0;
    removed = 0;
    count = 0;
    for (i = 0; i < 32 && newGrid[i].encoded_action != 0; i++) {
        if (BattleAction_Get(newGrid[i].encoded_action)->target_flags & 0x80) {
            out[count] = newGrid[i].encoded_action & 0x3fff;
            count++;
            for (j = 0; j < 32; j++) {
                if (((newGrid[i].encoded_action ^ oldGrid[j].encoded_action) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                added++;
                out[count - 1] |= 0x8000;
            }
        }
    }
    for (i = 0; i < 32 && oldGrid[i].encoded_action != 0; i++) {
        if (BattleAction_Get(oldGrid[i].encoded_action)->target_flags & 0x80) {
            for (j = 0; j < 32; j++) {
                if (((oldGrid[i].encoded_action ^ newGrid[j].encoded_action) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                removed++;
                out[count] = (oldGrid[i].encoded_action & 0x3fff) | 0x4000;
                count++;
            }
        }
    }
    *gained = added;
    *lost = removed;
    return count;
}
