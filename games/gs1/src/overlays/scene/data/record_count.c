#include "types.h"
#include "scene.h"

extern u8 gWork[];
extern u8 gOv;
extern u8 Value_00000c20;
extern u8 gCell[];
extern u8 gOv2[];
extern u8 gOv3;
extern u8 gOv4;
extern u8 gOv5;

u8 *SceneData_Run(s32);

u8 *SceneData_unk2_4();

u8 *SceneData_unk3_4(s32 id);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02008964.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008994.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x02008998.
 */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/*
 * An interactive panel: open a 30x9 window, draw three caption lines plus the
 * current item's icon and count, then loop on the button latch until B closes
 * it.  The 296-byte owner at 0x020000ec includes the literal pool at
 * 0x02000204-0x02000213.  0x03001c94 is the button latch; reading its bits
 * 0/1/2/3 as A, B, Select and Start fits the behavior but is not otherwise
 * witnessed.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

                                /* Item or party record by id. */

                                /* Refresh one display slot. */

                                /* Open a window, return its handle. */

                                /* Close a window. */

                                /* Draw a canned caption line. */

                                /* Draw an image (image, layer, x, y). */

                                /* Draw a number (value, digits, layer, x, y). */

                                /* Clear a window's contents. */

                                /* Advance the task scheduler. */

                                /* Play a sound cue. */

                                /* In-overlay: apply a delta to every member. */

/* RAM: the shared work area, and the newly-pressed button latch. */

/* In-image: the icon drawn beside the count. */

/*
 * Set the two scene-state words at +0x1c0 and +0x1c8, then run the local
 * follow-up service.  The owner includes the literal word at 0x02000718, which
 * its opening load references directly.
 */

/* Create one scene handle and bind its three adjacent resource records. */

/*
 * Set the flag byte at +53 of the record held by the IWRAM pointer at
 * 0x03001f30.  The sixteen-byte owner at 0x020007b8 includes its one pool word
 * at 0x020007c4.  Which record that pointer holds, and what the flag means,
 * are not decided here.
 */

/*
 * The call is named at its decoded site address rather than at the main-image
 * symbol it reaches: a direct far bl cannot encode that displacement, and only
 * the RAM-resident address reproduces the reference bytes.  The return address
 * is popped into r1, so r0 is live at return and the wrapper hands its
 * callee's result back.
 */

/*
 * The return address is popped into r1, not r0, so r0 is live at return and
 * the wrapper hands its callee's result back; declaring the pair void would
 * compile pop {r0} / bx r0 instead.
 */

static __inline__ s32 Scene_ListRecords(s32 (*func)(u16 *), u16 *list)
{
    return func(list);
}

u8 *SceneData_GetTable8964(void)
{
    return (u8 *)0x02008964;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable8994(void)
{
    return (u8 *)0x02008994;
}

u8 *SceneData_GetTable8998(void)
{
    return (u8 *)0x02008998;
}

void Scene_RunActor13Mode102Step(void)
{
    u32 i;
    s32 record;

    SceneData_Do(0x23cd);
    SceneData_Place(13, 0x102, 0);
    SceneData_Apply(13, 0);
}

void Scene_RunActor13Mode105Step(void)
{
    void SceneData_unk4_4(s32, s32);

    SceneData_unk2_3(0xD, 0x105, 0);
    SceneData_unk2_2(0x23CD);
    SceneData_unk4_4(0xD, 0);
}

void *SceneData_GetTable8a58(void)
{
    void SceneData_unk4_4(s32, s32);

    return &gOv;
}

void State_AddToRecordCount(s32 arg0, s32 arg1)
{
    u8 *entry = SceneData_Run(arg0);

    SceneData_Apply2(arg0, entry[15] + arg1);
    SceneData_unk3_2(arg0);
}

void Scene_AddToListedRecordCounts(s32 arg)
{
    u16 list[16];
    u16 *p;
    s32 n;

    n = Scene_ListRecords(SceneData_Check, list);
    if (n > 0) {
        s32 count;
        p = list;
        count = n;
        do {
            SceneData_Apply3(*p++, arg);
        } while (--count != 0);
    }
}

void Scene_RunCountAdjustPanel(void)
{
    void SceneData_unk5_4();

    u8 *record;
    volatile s32 *work;
    volatile u32 *key;
    s32 win;
    s32 flag;
    s32 msg;

    work = (volatile s32 *)gCell;
    record = SceneData_unk2_4(work[125]);
    win = SceneData_unk2(0, 0, 30, 9, 2);

    msg = ((s32)&Value_00000c20);
    SceneData_unk6_3(msg, win, 0, 0);
    SceneData_unk5_4(msg + 1, win, 0, 16);
    msg += 2;
    flag = 1;
    SceneData_unk7_3(msg, win, 0, 32);

loop:
    {
        if (flag != 0) {
            SceneData_unk8_3(win);
            SceneData_unk9_3(record, win, 0, 48);
            SceneData_unk10_3(gOv2, win, 48, 48);
            flag = 0;
            SceneData_unk11_3(record[15], 0, win, 72, 48);
        }

        key = (volatile u32 *)0x03001c94;

        if ((*key & 8) != 0 || (*key & 4) != 0) {
            SceneData_unk12_3(5);
            SceneData_unk13_3(93);
            flag = 1;
        }

        if ((*key & 1) != 0) {
            SceneData_unk14_3(1);
            SceneData_unk15_3(91);
            flag = 1;
        }

        if ((*key & 2) != 0) {
            SceneData_unk16_3(113);
            SceneData_unk17_3(win);
            SceneData_unk18_3(1);
            SceneData_unk19_3(win, 1);

            /* The refresh order 0, 1, 3, 2 is deliberate; do not sort it. */
            SceneData_unk20_3(0);
            SceneData_unk21_3(1);
            SceneData_unk22_3(3);
            SceneData_unk23_3(2);
            return;
        }

        SceneData_unk24_3(1);
        goto loop;
    }
}

void State_SetWorkWords1c0And1c8(void)
{
    *(s32 *)((*(u8 **)gWork) + 0x1c0) = 0x201;
    *(s32 *)((*(u8 **)gWork) + 0x1c8) = 24;
    SceneData_unk25_3();
}

/*
 * The overlay's entry driver, the target of the header veneer at image offset
 * 4, so the loader enters here and no bl inside the image reaches it.  The
 * 76-byte owner at 0x0200071c also owns its alignment halfword and the
 * two-word literal pool at 0x02000760-0x02000767.  Record 11 is fetched twice,
 * once per store, and must not be folded into one local.  The workspace +448
 * value and the record's +24 and +28 fields are recorded by offset only.
 */
s32 Scene_RunEntrySetup(void)
{
    *(s32 *)((*(u8 **)gWork) + 448) = 516;
    *(s32 *)((*(u8 **)gWork) + 456) = 24;
    *(s32 *)(SceneData_unk26_2(11) + 28) = 0x19999;
    *(s32 *)(SceneData_unk3_4(11) + 24) = 0x19999;
    SceneData_unk27_2(13, 5);
    SceneData_Apply4(14, 2);
    return 0;
}

void Scene_DrawThreeCaptionWindow(void)
{
    /*
     * The frame is 36 bytes: 4 for the stacked fifth argument, plus a 32-byte
     * local that no instruction reads or writes.  Only its size is known, not
     * its element type, so the declaration must stay at 32 bytes.
     */
    u8 buf[32];
    s32 handle = SceneData_SetRange(0, 13, 30, 6, 2);

    SceneData_SetMode(&gOv3, handle, 0, 0);
    SceneData_SetMode2(&gOv4, handle, 0, 8);
    SceneData_SetMode3(&gOv5, handle, 0, 16);
}

void State_SetRecordFlag53(void)
{
    u8 *record = *(u8 **)0x03001f30;

    record[53] = 1;
}

int State_GetFarResult100c(void)
{
    return SceneData_unk28_2();
}

int State_GetFarResult1020(void)
{
    return SceneData_unk29_2();
}
