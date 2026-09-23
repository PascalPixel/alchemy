/* Draft, not exact (2026-09-24): 5 differing halfwords, 272 of 272 bytes.
   Residual: in the fade shift the reference holds &work[0x1a1] in r0 and
   the zero in r1; this candidate uses r1 and r7. The "track = 0" is a
   search artefact that fixes the statement order. */

#include "TYPES.H"

struct AreaMusic {
    s16 map;
    s16 room;
    s16 flag : 15;
    s16 fade : 1;
    s16 track;
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

void Func_0808ace0(s32 check)
{
    u8 *work;
    s32 map;
    s32 room;
    u8 *out;
    struct AreaMusic *entry;
    s32 track;
    s16 fade;
    u32 i;

    work = Data_03001ebc;
    map = Data_02000240.map;
    room = Data_02000240.room;
    out = work + 0x1a0;
    entry = (struct AreaMusic *)0x0809d170;
    track = 0;
    fade = 0;
    if (check != 0) {
        while (entry->map != -1) {
            if (entry->map == map && (entry->room == -1 || entry->room == room)) {
                if (entry->flag == -1 || Func_080770c0(entry->flag) == 0) {
                    track = entry->track;
                    fade = entry->fade;
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
        work[0x1a0] = work[0x1a1];
        work[0x1a1] = track = 0;
    }
    *(s32 *)(work + 0x1a8) = 0;
    *(s32 *)(work + 0x1ac) = 0x100000;
    BattleFx_SetSpecialFromTable();
}
