/* NONMATCHING: complete owner [08029094,080291e4), 334 bytes plus the
 * two-byte pad; 18 differing halfwords, register swap only (2026-09-28).
 * The reference keeps &gKeysRepeat in r6 and the mode pointer in r0; here
 * they are the other way round. Global allocation takes the key address
 * first: 9 refs over 60 (its constant equivalence doubles the length) give
 * it priority 27/60 against the mode pointer's 14/44 (7 refs), so it lands
 * in r0 before the mode pointer is placed. The reference needs the mode
 * pointer first, which by that measure means one more mode reference or
 * two fewer key references. Parameter types, a local key pointer, a local
 * mode copy, storing *mode into *sub and name-length changes do not move it.
 * 2026-09-29: alchemy permute scores 240 (15 register-only, the pool word
 * moved and its pad). Ten minutes (67,766 candidates): none below 240;
 * the rewrites do not change reference counts or live lengths.
 */
#include "TYPES.H"

extern volatile u32 gKeysRepeat;
void Menu_DrawSelectionRow(s32 arg, s32 value, s16 *sub);

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode)
{
    if (gKeysRepeat & 1)
        return -1;
    if (gKeysRepeat & 2)
        return -2;
    if ((gKeysRepeat & 0x80) || (gKeysRepeat & 0x40)) {
        *mode ^= 1;
    } else if (gKeysRepeat & 0x10) {
        if (*mode == 0) {
            value++;
        } else {
            (*sub)++;
            if (*sub > 99)
                *sub = 0;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x20) {
        if (*mode == 0) {
            value--;
        } else {
            (*sub)--;
            if (*sub < 0)
                *sub = 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x100) {
        if (*mode == 0) {
            *sub = 0;
            value += 10;
        } else {
            *sub += 10;
            if (*sub > 99)
                *sub -= 99;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x200) {
        if (*mode == 0) {
            *sub = 0;
            value -= 10;
        } else {
            *sub -= 10;
            if (*sub < 0)
                *sub += 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow(arg, value, sub);
    }
    return value;
}
