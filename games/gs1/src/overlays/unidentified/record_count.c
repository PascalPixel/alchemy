#include "types.h"

#define Scene_AddToListedRecordCounts Func_020000c0
#define SceneData_GetTable8964 Func_02000030
#define SceneData_ReturnZero Func_02000038
#define SceneData_GetTable8994 Func_0200003c
#define SceneData_GetTable8998 Func_02000044
#define FieldScene_RunActor13Mode102Step Func_0200004c
#define FieldScene_RunActor13Mode105Step Func_02000070
#define SceneData_GetTable8a58 Func_02000098
#define SceneState_AddToRecordCount Func_020000a0
#define FieldScene_RunCountAdjustPanel Func_020000ec
#define SceneState_SetWorkWords1c0And1c8 Func_020006f8
#define FieldScene_RunEntrySetup Func_0200071c
#define FieldScene_DrawThreeCaptionWindow Func_02000768
#define SceneState_SetRecordFlag53 Func_020007b8
#define SceneState_GetFarResult100c Func_020007c8
#define SceneState_GetFarResult1020 Func_020007d4

extern u8 Data_03001ebc[];
extern u8 Data_02008a58;
extern u8 Value_00000c20;
extern u8 Data_02000240[];
extern u8 Data_020088d0[];
extern u8 Data_02008920;
extern u8 Data_0200893c;
extern u8 Data_02008958;

void Func_020008f2();
void Func_0200090e();
void Func_0200090e_a(s32, s32);
s32 Func_0200092a(s32, s32, s32);
void Func_02000920(s32);
u8 *Func_020008f8(s32);
void Func_02000932(s32, s32);
void Func_02000910(s32);
u8 *Func_02000950();
s32 Func_02000902();
void Func_02000922();
void Func_0200093e();
void Func_02000978();
void Func_02000978_a();
void Func_02000964();
void Func_0200099c();
void Func_02000250();
void Func_02000a5e();
void Func_02000268();
void Func_02000a76();
void Func_02000a88();
void Func_020009f6();
void Func_020009b4();
void Func_020009cc();
void Func_02000a32();
void Func_02000a38();
void Func_02000a3e();
void Func_02000a44();
void Func_020009e6();
void Func_02000fca(void);
u8 *Func_02000fd2(s32 id);
void Func_02000fec(s32 id, s32 arg1);
s32 Func_02000f6a(s32, s32, s32, s32, s32);
void Func_02000fa0(const void *, s32, s32, s32);
void Func_02000fac(const void *, s32, s32, s32);
void Func_02000fb8(const void *, s32, s32, s32);
int Func_0200100c(void);
int Func_02001020(void);
s32 Func_02000942(u16 *);
void Func_0200017e(u16, s32);

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

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

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

void FieldScene_RunActor13Mode102Step(void)
{
    u32 i;
    s32 record;

    Call1(Func_020008f2, 0x23cd);
    Call3(Func_0200090e, 13, 0x102, 0);
    Func_0200090e_a(13, 0);
}

void FieldScene_RunActor13Mode105Step(void)
{
    void Func_02000930(s32, s32);

    Func_0200092a(0xD, 0x105, 0);
    Func_02000920(0x23CD);
    Func_02000930(0xD, 0);
}

void *SceneData_GetTable8a58(void)
{
    void Func_02000930(s32, s32);

    return &Data_02008a58;
}

void SceneState_AddToRecordCount(s32 arg0, s32 arg1)
{
    u8 *entry = Func_020008f8(arg0);

    Func_02000932(arg0, entry[15] + arg1);
    Func_02000910(arg0);
}

void Scene_AddToListedRecordCounts(s32 arg)
{
    u16 list[16];
    u16 *p;
    s32 n;

    n = Scene_ListRecords(Func_02000942, list);
    if (n > 0) {
        s32 count;
        p = list;
        count = n;
        do {
            Func_0200017e(*p++, arg);
        } while (--count != 0);
    }
}

void FieldScene_RunCountAdjustPanel(void)
{
    void Func_02000930_a();

    u8 *record;
    volatile s32 *work;
    volatile u32 *key;
    s32 win;
    s32 flag;
    s32 msg;

    work = (volatile s32 *)Data_02000240;
    record = Func_02000950(work[125]);
    win = Func_02000902(0, 0, 30, 9, 2);

    msg = ((s32)&Value_00000c20);
    Func_02000922(msg, win, 0, 0);
    Func_02000930_a(msg + 1, win, 0, 16);
    msg += 2;
    flag = 1;
    Func_0200093e(msg, win, 0, 32);

loop:
    {
        if (flag != 0) {
            Func_02000978(win);
            Func_02000964(record, win, 0, 48);
            Func_02000978_a(Data_020088d0, win, 48, 48);
            flag = 0;
            Func_0200099c(record[15], 0, win, 72, 48);
        }

        key = (volatile u32 *)0x03001c94;

        if ((*key & 8) != 0 || (*key & 4) != 0) {
            Func_02000250(5);
            Func_02000a5e(93);
            flag = 1;
        }

        if ((*key & 1) != 0) {
            Func_02000268(1);
            Func_02000a76(91);
            flag = 1;
        }

        if ((*key & 2) != 0) {
            Func_02000a88(113);
            Func_020009f6(win);
            Func_020009b4(1);
            Func_020009cc(win, 1);

            /* The refresh order 0, 1, 3, 2 is deliberate; do not sort it. */
            Func_02000a32(0);
            Func_02000a38(1);
            Func_02000a3e(3);
            Func_02000a44(2);
            return;
        }

        Func_020009e6(1);
        goto loop;
    }
}

void SceneState_SetWorkWords1c0And1c8(void)
{
    *(s32 *)((*(u8 **)Data_03001ebc) + 0x1c0) = 0x201;
    *(s32 *)((*(u8 **)Data_03001ebc) + 0x1c8) = 24;
    Func_02000fca();
}

/*
 * The overlay's entry driver, the target of the header veneer at image offset
 * 4, so the loader enters here and no bl inside the image reaches it.  The
 * 76-byte owner at 0x0200071c also owns its alignment halfword and the
 * two-word literal pool at 0x02000760-0x02000767.  Record 11 is fetched twice,
 * once per store, and must not be folded into one local.  The workspace +448
 * value and the record's +24 and +28 fields are recorded by offset only.
 */
s32 FieldScene_RunEntrySetup(void)
{
    *(s32 *)((*(u8 **)Data_03001ebc) + 448) = 516;
    *(s32 *)((*(u8 **)Data_03001ebc) + 456) = 24;
    *(s32 *)(Func_02000fc8(11) + 28) = 0x19999;
    *(s32 *)(Func_02000fd2(11) + 24) = 0x19999;
    Func_02000fe4(13, 5);
    Func_02000fec(14, 2);
    return 0;
}

void FieldScene_DrawThreeCaptionWindow(void)
{
    /*
     * The frame is 36 bytes: 4 for the stacked fifth argument, plus a 32-byte
     * local that no instruction reads or writes.  Only its size is known, not
     * its element type, so the declaration must stay at 32 bytes.
     */
    u8 buf[32];
    s32 handle = Func_02000f6a(0, 13, 30, 6, 2);

    Func_02000fa0(&Data_02008920, handle, 0, 0);
    Func_02000fac(&Data_0200893c, handle, 0, 8);
    Func_02000fb8(&Data_02008958, handle, 0, 16);
}

void SceneState_SetRecordFlag53(void)
{
    u8 *record = *(u8 **)0x03001f30;

    record[53] = 1;
}

int SceneState_GetFarResult100c(void)
{
    return Func_0200100c();
}

int SceneState_GetFarResult1020(void)
{
    return Func_02001020();
}
