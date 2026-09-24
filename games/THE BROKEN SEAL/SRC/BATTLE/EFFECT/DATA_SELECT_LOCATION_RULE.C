/* Battle effect: pick the location rule for the current map and room from
   the rule table at 0x0809d170 (a room of -1 matches any room; a flag of -1
   always applies, otherwise the rule applies while the flag is clear), then
   write a zero and seven consecutive results from the rule's base into
   work + 0x1a0 (all zero when nothing applies). A rule with the shift bit
   moves the first result down a slot. Finally clear the two words after
   them and apply the special.

   FAKEMATCH: the shift reads the second slot and takes the first slot's
   address before the zero, each in its own temporary, which orders the
   zero between the two stores as the ROM does. */

#include "TYPES.H"

struct BattleLocationRule {
    s16 map;
    s16 room;
    s16 flag : 15;
    s16 shift : 1;
    s16 result;
};

struct FieldLocation {
    u8 unknown_000[448];
    s16 map;
    s16 room;
};

extern u8 *Data_03001ebc;
extern struct FieldLocation Data_02000240;

s32 Func_080770c0(s32 flag);
void BattleFx_SetSpecialFromTable(void);

void BattleFx_SelectLocationRule(s32 check)
{
    u8 *work;
    s32 map;
    s32 room;
    u8 *out;
    struct BattleLocationRule *entry;
    s32 track;
    s16 fade;
    u32 i;

    work = Data_03001ebc;
    map = Data_02000240.map;
    room = Data_02000240.room;
    out = work + 0x1a0;
    entry = (struct BattleLocationRule *)0x0809d170;
    track = 0;
    fade = 0;
    if (check != 0) {
        while (entry->map != -1) {
            if (entry->map == map && (entry->room == -1 || entry->room == room)) {
                if (entry->flag == -1 || Func_080770c0(entry->flag) == 0) {
                    track = entry->result;
                    fade = entry->shift;
                    break;
                }
            }
            entry++;
        }
    }
    *out++ = 0;
    for (i = 0; i <= 6; i++) {
        *out++ = track;
        if (track != 0)
            track++;
    }
    if (fade != 0) {
        u8 v = work[0x1a1];
        u8 *d = &work[0x1a0];
        s32 zero = 0;

        *d = v;
        work[0x1a1] = zero;
    }
    *(s32 *)(work + 0x1a8) = 0;
    *(s32 *)(work + 0x1ac) = 0x100000;
    BattleFx_SetSpecialFromTable();
}
