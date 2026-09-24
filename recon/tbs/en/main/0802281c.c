/*
 * BattleLayout_HighlightPartyPanels (main:0802281c, 160 bytes)
 *
 * Clears the highlight over the whole party panel strip, then highlights
 * the panel of every unit in the 0xff-terminated id list (at most four)
 * that sits in the battle's four placed-unit slots at +88.
 *
 * Draft, not exact (2026-09-24): candidate=168 reference=160
 * differing_halfwords=81.  Block layout and both loop rotations match the
 * reference.  The reference keeps both loops unreduced: it recomputes
 * j * 2 + 88 against the battle base and reloads ids[i] through an i * 2
 * offset on every pass, and keeps the unit count in r8 and the battle base
 * in r7.  Here loop strength reduction walks both pointers instead.  Index
 * types (u8/s16/u16), a volatile id list, and a goto-built copy of the
 * reference layout (86 halfwords) did not stop the reduction.
 */
#include "TYPES.H"

extern u8 *Data_03001e74;

s32 Func_080b5090(s32 side, s32 group);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt);

s32 BattleLayout_HighlightPartyPanels(u16 *ids)
{
    u8 *battle = Data_03001e74;
    s32 count;
    s32 i;
    s32 j;

    count = Func_080b5090(1, 0);
    Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
    for (i = 0; i < 4 && ids[i] != 0xff; i++) {
        for (j = 0; j < 4; j++) {
            if (((s16 *)(battle + 88))[j] == ids[i])
                break;
            if (((s16 *)(battle + 88))[j] == 0xff) {
                j = 4;
                break;
            }
        }
        if (j != 4)
            Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
    }
    return 0;
}
