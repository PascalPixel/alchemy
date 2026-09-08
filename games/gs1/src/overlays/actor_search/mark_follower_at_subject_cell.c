#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_02001a78(void);
s32 Func_02001a84(s32);

s32 Func_02000944(void)
{
    s32 subject;
    s32 slot;
    s32 i;
    s32 slot_cz;
    s32 slot_cx;
    s32 subj_cz;
    s32 subj_cx;
    s32 z_diff;

    subject = Func_02001a78();
    for (i = 0; i <= 3; i++) {
        slot = Func_02001a84(i + 11);
        if ((u32)(FIELD_AT_OFFSET(slot, s32 *, 12) - 1) <= 0xffffe) {
            slot_cz = FIELD_AT_OFFSET(slot, s32 *, 16) / 0x100000;
            slot_cx = FIELD_AT_OFFSET(slot, s32 *, 8) / 0x100000;
            subj_cz = FIELD_AT_OFFSET(subject, s32 *, 16) / 0x100000;
            subj_cx = FIELD_AT_OFFSET(subject, s32 *, 8) / 0x100000;
            z_diff = subj_cz - slot_cz;
            if (subj_cx == slot_cx && z_diff == 0) {
                FIELD_AT_OFFSET(slot, s32 *, 12) = 0xff0000;
                FIELD_AT_OFFSET(slot, s32 *, 72) = z_diff;
                FIELD_AT_OFFSET(slot, s32 *, 40) = z_diff;
                return 1;
            }
        }
    }
    return 0;
}
