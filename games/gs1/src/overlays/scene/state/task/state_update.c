#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_StoreSlotTileXToWork832To848 Func_020000c0
#define SceneState_SetWorkByte35 Func_02000150
#define SceneState_SendWord250With6 Func_020002e8
#define SceneState_ApplyTable8715AndValue104 Func_02000950
#define SceneState_WaitForStatusWords Func_02000970
#define SceneState_InstallTask8714AndApplyTwoRects Func_020009b0
#define SceneState_InitCursorWhenUnset Func_02002e64
#define SceneState_SetHalfword1000To9 Func_0200322c
#define SceneState_WaitUntilStatusNine Func_02003238

extern s16 Data_02000240[];
extern u8 Value_0000000a;
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;
extern s16 Data_0200cb46;

void Func_02003f5e(s32, s32, s32, s32, s32, s32);
s32 *Func_0200402c(s32);
void Func_02003ff2(s32, s32);
void Func_02003f84(s32, s32, s32, s32, s32, s32);
s32 *Func_02004052(s32);
void Func_02004018(s32, s32);
void Func_02003fa8(s32, s32, s32, s32, s32, s32);
s32 *Func_02004076(s32);
void Func_0200403c(s32, s32);
void Func_02003fcc(s32, s32, s32, s32, s32, s32);
void Func_020042aa(s32, s32, s32);
s32 Func_02004700(s32, s32);
void Func_02004858(s32);
void Func_02004716();
void Func_02004724();
void Func_02004a58(s32);
void Func_020048b0(s32);
void Func_02004774(s32);
void Func_0200477a(s32);
void Func_02004790(s32);
void Func_0200487c(s32, s32, s32, s32, s32, s32);
void Func_0200488e(s32, s32, s32, s32, s32, s32);
void Func_02008714();
s32 Func_02006d3e(void);
void Func_02006fe8(s32 arg0);

/* Per-site veneers: both reach the same main-image import, but each names its
 * own loader-relocated call word rather than a runtime address. */

/* In-image status words. The 64-byte owner includes its alignment bytes and
 * the two-word literal pool that holds these two addresses. */

/* Address-taken rather than called. Overlays are LINKED at 0x02008000 and run
 * at 0x02000000, so a pooled function address carries the link-base form: the
 * reference word is 0x02008715, which is this symbol plus the Thumb bit that
 * `.thumb_set` supplies. A called symbol uses the raw run-time annotation; an
 * address-taken one does not. */

void SceneState_StoreSlotTileXToWork832To848(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    Func_02003f5e(100, 11, 12, 4, fifth, sixth);

    record = Func_0200402c(12);
    value = record[2] >> 20;
    Func_02003ff2(832, value);
    Func_02003f84(71, 16, 1, 1, value, 16);

    record = Func_02004052(13);
    value = record[2] >> 20;
    Func_02004018(840, value);
    Func_02003fa8(71, 16, 1, 1, value, 16);

    record = Func_02004076(14);
    value = record[2] >> 20;
    Func_0200403c(848, value);
    Func_02003fcc(71, 16, 1, 1, value, 16);
}

void SceneState_SetWorkByte35(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x35) = 1;
}

void SceneState_SendWord250With6(void)
{
    s16 *tbl = Data_02000240;

    Func_020042aa(*(s32 *)(tbl + 250), 6, 0);
}

void SceneState_ApplyTable8715AndValue104(void)
{
    Func_02004700(0x2008715, 0xC85);
    Func_02004858(0x104);
}

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, so they are
 * volatile and their reads must not be hoisted or merged across the poll call.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void SceneState_WaitForStatusWords(void)
{
    s32 cnt;

    /* The frame count is a literal ten. */
    Func_02004716(10);

    cnt = 0;
    while (Data_0200c834 != 0 || Data_0200c838 != 75) {
        Func_02004724(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void SceneState_InstallTask8714AndApplyTwoRects(void)
{
    Func_02004a58(31);
    Func_020048b0(820);                 /* 205 << 2 */

    if (Data_0200c834 != 0) {
        Data_0200c838 = 0;
    }

    Func_02004774(30);
    Func_0200477a(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Func_02004790((s32)Func_02008714);

    Func_0200487c(58, 28, 7, 1, 58, 13);
    Func_0200488e(57, 11, 1, 1, 58, 11);
}

void SceneState_InitCursorWhenUnset(void)
{
    s16 *cursor = &Data_0200cb46;

    if (*cursor == -1) {
        *cursor = Func_02006d3e();
    }
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void SceneState_SetHalfword1000To9(void)
{
    extern u16 Data_02001000;

    u16 *p = &Data_02001000;
    u16 v = 9;
    *p = v;
}

void SceneState_WaitUntilStatusNine(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02006fe8(1);
    }
}
