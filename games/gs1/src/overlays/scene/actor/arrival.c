#include "types.h"

#define SceneData_SelectTableBySceneId Func_02000030
#define SceneData_ReturnZero Func_02000070
#define SceneData_GetTable8728 Func_02000074
#define SceneData_SelectAndPrepareTable Func_0200007c
#define SceneData_SelectSubStateTable Func_020000e4
#define Dialogue_RunActor12DialogueAndSetFlag910 Func_02000144
#define Dialogue_RunActor16Dialogue Func_0200016c
#define Dialogue_RunActor8FlaggedDialogue Func_0200018c
#define Dialogue_RunActor8FacingDialogue Func_020001fc
#define Scene_RunArrivalPlacement Func_02000240
#define State_SetRuntimeWord448To521 Func_02000308
#define State_ClearSlotsBySubState Func_0200033c
#include "resource_3a9.h"

/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern s16 Data_02000240[];     /* The shared work area, in RAM. */
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_020084d0[];
extern u8 Data_020086c8[];
extern u8 Data_020084a0[];
extern Placement Data_02008ef8[];   /* In-image placement table, four entries. */
extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern u8 Data_020088d4[];
extern u8 Data_0200879c[];
extern u8 Data_02008a0c[];
extern u8 Data_02008784[];
extern u8 Data_02008c88[];
extern u8 Data_02008a48[];
extern u8 Data_02008eb0[];
extern u8 Data_02008a3c[];

void Func_02000558(void);
void Func_020005a6(s32);
s32 Func_020005b6(s32, s32);
void Func_0200055e(s32);
void Func_0200057a(void);
void Func_02000580(void);
void Func_020005ce(s32);
s32 Func_020005e6(s32, s32);
void Func_0200059a(void);
u8 *Func_020005ba(s32);
void Func_02000638(s32, s32);
void Func_020005be(void);
s32 Func_020005ac(s32);
void Func_02000616(s32);
void Func_02000626(s32, s32);
void Func_02000626_b(s32);
void Func_0200063e(s32, s32);
void Func_020005de(s32);
void Func_020005fa(void);
u8 *Func_0200062a(int);
void Func_0200069e(int);
void Func_0200062c(void);
void Func_0200068a(int, int);
void Func_02000646(void);
void Func_02000658();
void Func_02000710();
u8 *Func_020006e6();
void Func_020006e6_a(s32);
void Func_020006f0();
void Func_02000718();
void Func_02000714();
void Func_0200075c();
void Func_02000768();
void Func_02000774();
void Func_02000692(u32, u32, u32);
void Func_02000730();
void Func_0200065e(void);
void Func_020007b4();
void Func_020007ba();
void Func_020007c0();
void Func_020007c6();
void Func_020007cc();
void Func_020007d2();
void Func_020007d8();
void Func_020007de();
void Func_020007e4();
void Func_02000800();
void Func_02000806();
void Func_0200080e();
void Func_02000762(s32, s32, s32, s32, s32, s32);
s32 Func_02000772();
s32 Func_020007be();
void Func_020004d0(void *);

/* Old-style declarations where an overlay import varies in arity between its
 * call sites. */

/* Picks one of three scene tables by scene id. */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000064) {
        return (s32)Data_020084d0;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_020086c8;
    }
    return (s32)Data_020084a0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x02008728. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8728(void)
{
    return (u8 *)0x02008728;
}

/* Picks a table by scene id and sub-state, and hands the chosen one to
 * Func_020004d0 before returning it. */
u8 *SceneData_SelectAndPrepareTable(void)
{
    s32 id = Data_02000240[224];
    if (id == (s32)&Value_00000064) {
        s32 state = Data_02000240[225];
        u8 *tbl;
        switch (state) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            tbl = Data_020088d4;
            break;
        default:
            tbl = Data_0200879c;
            break;
        }
        Func_020004d0(tbl);
        return tbl;
    }
    if (id == (s32)&Value_00000065) {
        return Data_02008a0c;
    }
    return Data_02008784;
}

/* The same selection without the hand-off. Sub-state 16 falls to the default
 * arm even though it lies inside 9..17; that hole is deliberate. */
u8 *SceneData_SelectSubStateTable(void)
{
    s32 id = Data_02000240[224];
    if (id == (s32)&Value_00000064) {
        s32 state = Data_02000240[225];
        switch (state) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            return Data_02008c88;
        default:
            return Data_02008a48;
        }
    }
    if (id == (s32)&Value_00000065) {
        return Data_02008eb0;
    }
    return Data_02008a3c;
}

void Dialogue_RunActor12DialogueAndSetFlag910(void)
{
    Func_02000558();
    Func_020005a6(0x1ADD);
    Func_020005b6(0xC, 0);
    Func_0200055e(0x910);
    Func_0200057a();
}

void Dialogue_RunActor16Dialogue(void)
{
    Func_02000580();
    Func_020005ce(0x1AE3);
    Func_020005e6(16, 0);
    Func_0200059a();
}

/* Actor 8's dialogue, branched on flag 0x911. Func_02000626 and
 * Func_02000626_b are two imports sharing one call word: the two-argument
 * gesture in the first arm, the one-argument message in the second. */
void Dialogue_RunActor8FlaggedDialogue(void)
{
    u8 *p = Func_020005ba(0);

    /* Band guard: facing in 0x6001..0x9fff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) - 0x6001) <= 0x3FFE) {
        Func_02000638(7, 8);
    } else {
        Func_020005be();

        if (Func_020005ac(0x911) != 0) {
            Func_02000616(0x1AFB);
            Func_02000626(8, 0);
        } else {
            Func_02000626_b(0x1AD7);
            Func_0200063e(8, 0);
            Func_020005de(0x910);           /* 145 << 4 */
        }

        Func_020005fa();
    }
}

void Dialogue_RunActor8FacingDialogue(void)
{
    void Func_0200067a(int);

    u8 *p = Func_0200062a(0);

    /* Band guard: facing in 0xa001..0xdfff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) + 0x5FFF) <= 0x3FFE) {
        Func_0200069e(8);
    } else {
        Func_0200062c();
        Func_0200067a(0x1A8F);
        Func_0200068a(8, 0);
        Func_02000646();
    }
}

/*
 * Scene arrival: clears the residue byte at +85 of every scene slot from 8 to
 * 65, then looks the sub-state up in the placement table and places the
 * player from that entry. Sub-states other than 12, 13, 16 and 19 return
 * without touching anything. The loop skips a null record but the later clear
 * of the player's own +85 does not test for null; that asymmetry is real.
 * 158 is read as a cue id from its argument position and is not verified.
 */
void Scene_RunArrivalPlacement(void)
{
    u8 *Func_0200067a_a();

    u8 *work = *(u8 **)0x03001ebc;
    u32 slot;
    s32 idx;
    u8 *p;

    Func_02000658();

    for (slot = 8; slot <= 65; slot++) {
        u8 *rec = Func_0200067a_a(slot);

        if (rec != 0) {
            rec[85] = 0;
        }
    }

    /* The sub-state slot is read twice, here and for Func_0200075c below, and
     * both reads are kept. */
    switch (*(s16 *)(work + 364)) {         /* 182 << 1 */
    case 12: idx = 0; break;
    case 13: idx = 1; break;
    case 16: idx = 2; break;
    case 19: idx = 3; break;
    default: return;
    }

    Func_02000730(158);

    /* The entry address is accumulated through these locals; folding them into
     * one indexed expression does not reproduce the loads. */
    {
        u32 off = idx << 3;
        u32 value = (u32)Data_02008ef8;
        u32 addr = off + 4;
        u16 x = *(volatile u16 *)(value + addr);
        u16 y;

        addr += value;
        y = *(volatile u16 *)(addr + 2);
        value = *(volatile u32 *)(value + off);
        Func_02000692(value, x, y);
    }

    Func_020006f0(0, 0x00008000, 0x00004000);

    p = Func_020006e6(0);
    p[85] = 0;

    Func_02000714(0, 2);
    Func_02000718(0, 3, -8);
    Func_020006e6_a(10);

    Func_0200075c(*(s16 *)(work + 364));
    Func_02000768();
    Func_02000774();
    Func_02000710();
}

/* Publishes 0x209 at +448 of the runtime record, and calls Func_0200065e for
 * scene 0x64. */
s32 State_SetRuntimeWord448To521(void)
{
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    if (Data_02000240[224] == (s32)(u32)&Value_00000064) {
        Func_0200065e();
    }
    return 0;
}

/*
 * Clears the set of scene slots this sub-state leaves behind. Sub-state 16
 * takes the last arm even though it lies inside 9..17, so the test is not
 * written as a range. 0x911 is read as an event-flag id from its argument
 * position, and the six-argument call's argument meanings are not
 * established.
 */
void State_ClearSlotsBySubState(void)
{
    s16 sub = Data_02000240[225];

    switch (sub) {
    case 3:
    {
        /* The last two arguments travel on the stack. */
        s32 fifth = 4;
        s32 sixth = 2;
        Func_02000762(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    /* sub is 9..15 or 17. */
    if (Func_02000772(0x911) != 0) {
        /* Nine distinct call sites, not a loop; the trailing 15 is out of
         * order and is kept that way. */
        Func_020007b4(10);
        Func_020007ba(11);
        Func_020007c0(12);
        Func_020007c6(13);
        Func_020007cc(14);
        Func_020007d2(17);
        Func_020007d8(18);
        Func_020007de(19);
        Func_020007e4(15);
    } else {
        Func_0200080e(13, 2);
    }
    return;

other:
    if (Func_020007be(0x911) != 0) {
        Func_02000800(16);
        Func_02000806(17);
    }
}
