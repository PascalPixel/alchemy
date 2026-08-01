typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3b9 owner at 0x0200007c, 444 bytes: pick this overlay's script
 * block for the current map and sub-state.  Map 0x8c selects one of six blocks
 * through a 66-entry jump table; map 0x8e selects one of three by event flag;
 * anything else falls back to a single block.
 *
 * WHY THE INVENTORY UNDERSTATES THIS ROW: it advertises 98 code bytes and 2
 * calls against a 444-byte span, because the linear walk stops at the
 * 'mov pc, r3' dispatch and never sees the 70 bytes of arms behind the table.
 * This is the documented hidden-dispatcher tier - a prologue row whose 'calls'
 * is small against a large span - and it converts normally.  Real total: 118
 * code bytes, still 2 call sites.
 *
 * Complete owner: 'push {lr}' and the single interworking epilogue at
 * 0x020001f6.  Layout from the control-flow walk, seeding the jump table FIRST
 * as the pool guard requires:
 *
 *   0x007c .. 0x00ac    48 bytes   entry, map test, range test, dispatch
 *   0x00ac .. 0x01b4   264 bytes   jump table, 66 entries          (not code)
 *   0x01b4 .. 0x01fa    70 bytes   the six arms, the 0x8e chain, epilogue
 *   0x01fa .. 0x01fc     2 bytes   alignment halfword              (not code)
 *   0x01fc .. 0x0238    60 bytes   fifteen-word literal pool       (not code)
 *
 * 118 code + 326 non-code = 444, the advertised span.
 *
 * The table's base pool word is 0x020080ac - file offset 0x00ac under the
 * proven 0x02008000 link base, which is physically where the table sits.  Its
 * entries carry no Thumb bit, the mark of a 'mov pc' jump table rather than a
 * handler table, and all 66 resolve into the 0x01b4..0x01c8 arm block.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives and is the result - an in-image block address.
 *
 * Calls: 2 sites, one target, from
 * 'bun tools/overlay_call_targets.ts resource_3b9 007c'.
 *
 * STYLE AND LAYOUT CONFIRMED AGAINST tracked byte-exact SIBLINGS.
 * assets/code/resource_3b9_c_02000030.c and _02000238.c are the same family in
 * this same overlay and already reproduce the ROM: they establish that
 * Data_02000240[224] is the map id, that Data_02000240[225] is the sub-state,
 * that the result is spelled '(s32)Data_0200bxxx', and that 0x8c/0x8e are the
 * two interesting maps.  _02000238 even tests sub-state 12 - the same value
 * that selects a dedicated arm here.  Their '(s32)&Value_0000008c' spelling is
 * the exact reconstruction's pooling device for the integer 0x8c and carries no meaning,
 * so this file writes the integer.
 *
 * All ten result words are EVEN, so under the proven base they are in-image
 * data at file offsets 0x3324, 0x339c, 0x35f4, 0x375c, 0x37bc, 0x387c, 0x399c,
 * 0x3a44, 0x3bdc and 0x3e1c - script blocks, not callbacks.  The parity test
 * settles that in one bit.
 *
 * The 66 table entries reduce to six distinct arms; 57 of them are the same
 * fallback the out-of-range branch uses, so the arms are spelled as grouped
 * cases over the RAW sub-state, with the table's own 'default' as the switch
 * default.  Writing one arm per entry would be the inflation error in its
 * purest form.
 */

extern s16 Data_02000240[];

extern u8 Data_0200b324[];          /* offset 0x3324, the map fallback */
extern u8 Data_0200b39c[];          /* offset 0x339c */
extern u8 Data_0200b5f4[];          /* offset 0x35f4 */
extern u8 Data_0200b75c[];          /* offset 0x375c, the sub-state fallback */
extern u8 Data_0200b7bc[];          /* offset 0x37bc */
extern u8 Data_0200b87c[];          /* offset 0x387c */
extern u8 Data_0200b99c[];          /* offset 0x399c */
extern u8 Data_0200ba44[];          /* offset 0x3a44 */
extern u8 Data_0200bbdc[];          /* offset 0x3bdc */
extern u8 Data_0200be1c[];          /* offset 0x3e1c */

s32 Func_080770c0();                /* read an event flag */

s32 Func_0200007c(void)
{
    s16 map = Data_02000240[224];
    s16 state;

    if (map == 0x8c) {
        state = Data_02000240[225];
        if ((unsigned int)(state - 5) > 65) {
            return (s32)Data_0200b75c;
        }
        switch (state) {
        case 5:
        case 69:
            return (s32)Data_0200b39c;
        case 7:
        case 70:
            return (s32)Data_0200b5f4;
        case 8:
        case 21:
        case 31:
        case 64:
        case 65:
        case 67:
            return (s32)Data_0200b7bc;
        case 12:
            return (s32)Data_0200b87c;
        case 66:
        case 68:
            return (s32)Data_0200b99c;
        default:
            return (s32)Data_0200b75c;
        }
    }

    if (map != 0x8e) {
        return (s32)Data_0200b324;
    }
    if (Func_080770c0(0x950) != 0) {
        return (s32)Data_0200be1c;
    }
    if (Func_080770c0(0x962) != 0) {
        return (s32)Data_0200bbdc;
    }
    return (s32)Data_0200ba44;
}
