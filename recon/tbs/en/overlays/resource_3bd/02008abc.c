/* NONMATCHING: resource_3bd 0x02008abc, SceneState_SetByte1004AndRunWhenIdle, from
 * FIELD/ARUTAMIRA_DOU/EXTENDED_PRESENTATION.C (2026-09-28).
 * Stores into the byte at 0x02001004 in EWRAM, which the main image does not
 * name. Remaining: a name for that byte; the call goes to
 * ArutamiraDou_ApplyFadeBlend. */
#include "ARUTAMIRA.H"

void SceneState_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        ArutamiraDou_ApplyFadeBlend();
    }
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */