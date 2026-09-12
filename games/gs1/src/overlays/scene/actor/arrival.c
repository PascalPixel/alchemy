#include "types.h"
#include "scene.h"

#include "resource_3a9.h"

/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern s16 gCell[];     /* The shared work area, in RAM. */
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern Placement gOv4[];   /* In-image placement table, four entries. */
extern u8 *gWork;
extern s16 gCell[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];

u8 *Actor_Run(s32);

u8 *Actor_unk2_4(int);

u8 *Actor_unk3_4();

/* Old-style declarations where an overlay import varies in arity between its
 * call sites. */

/* Picks one of three scene tables by scene id. */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000064) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
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
 * Actor_Do before returning it. */
u8 *SceneData_SelectAndPrepareTable(void)
{
    s32 id = gCell[224];
    if (id == (s32)&Value_00000064) {
        s32 state = gCell[225];
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
            tbl = gOv5;
            break;
        default:
            tbl = gOv6;
            break;
        }
        Actor_Do(tbl);
        return tbl;
    }
    if (id == (s32)&Value_00000065) {
        return gOv7;
    }
    return gOv8;
}

/* The same selection without the hand-off. Sub-state 16 falls to the default
 * arm even though it lies inside 9..17; that hole is deliberate. */
u8 *SceneData_SelectSubStateTable(void)
{
    s32 id = gCell[224];
    if (id == (s32)&Value_00000064) {
        s32 state = gCell[225];
        switch (state) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            return gOv9;
        default:
            return gOv10;
        }
    }
    if (id == (s32)&Value_00000065) {
        return gOv11;
    }
    return gOv12;
}

void Dialogue_RunActor12DialogueAndSetFlag910(void)
{
    Actor_unk4_4();
    Actor_unk2_2(0x1ADD);
    Actor_Apply(0xC, 0);
    Actor_unk3_2(0x910);
    Actor_unk5_4();
}

void Dialogue_RunActor16Dialogue(void)
{
    Actor_unk6_4();
    Actor_unk4_2(0x1AE3);
    Actor_Apply2(16, 0);
    Actor_unk7_4();
}

/* Actor 8's dialogue, branched on flag 0x911. Actor_Apply3 and
 * Actor_unk5_2 are two imports sharing one call word: the two-argument
 * gesture in the first arm, the one-argument message in the second. */
void Dialogue_RunActor8FlaggedDialogue(void)
{
    u8 *p = Actor_Run(0);

    /* Band guard: facing in 0x6001..0x9fff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) - 0x6001) <= 0x3FFE) {
        Actor_Apply4(7, 8);
    } else {
        Actor_unk8_4();

        if (Actor_Check(0x911) != 0) {
            Actor_unk6_2(0x1AFB);
            Actor_Apply3(8, 0);
        } else {
            Actor_unk5_2(0x1AD7);
            Actor_Apply5(8, 0);
            Actor_unk7_2(0x910);           /* 145 << 4 */
        }

        Actor_unk9_4();
    }
}

void Dialogue_RunActor8FacingDialogue(void)
{
    void Actor_unk10_4(int);

    u8 *p = Actor_unk2_4(0);

    /* Band guard: facing in 0xa001..0xdfff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) + 0x5FFF) <= 0x3FFE) {
        Actor_unk8_2(8);
    } else {
        Actor_unk11_4();
        Actor_unk10_4(0x1A8F);
        Actor_Apply6(8, 0);
        Actor_unk12_4();
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
    u8 *Actor_unk13_4();

    u8 *work = *(u8 **)0x03001ebc;
    u32 slot;
    s32 idx;
    u8 *p;

    Actor_unk14_4();

    for (slot = 8; slot <= 65; slot++) {
        u8 *rec = Actor_unk13_4(slot);

        if (rec != 0) {
            rec[85] = 0;
        }
    }

    /* The sub-state slot is read twice, here and for Actor_unk15_4 below, and
     * both reads are kept. */
    switch (*(s16 *)(work + 364)) {         /* 182 << 1 */
    case 12: idx = 0; break;
    case 13: idx = 1; break;
    case 16: idx = 2; break;
    case 19: idx = 3; break;
    default: return;
    }

    Actor_unk16_4(158);

    /* The entry address is accumulated through these locals; folding them into
     * one indexed expression does not reproduce the loads. */
    {
        u32 off = idx << 3;
        u32 value = (u32)gOv4;
        u32 addr = off + 4;
        u16 x = *(volatile u16 *)(value + addr);
        u16 y;

        addr += value;
        y = *(volatile u16 *)(addr + 2);
        value = *(volatile u32 *)(value + off);
        Actor_Place(value, x, y);
    }

    Actor_unk17_4(0, 0x00008000, 0x00004000);

    p = Actor_unk3_4(0);
    p[85] = 0;

    Actor_unk18_4(0, 2);
    Actor_unk19_4(0, 3, -8);
    Actor_unk9_2(10);

    Actor_unk15_4(*(s16 *)(work + 364));
    Actor_unk20_4();
    Actor_unk21_4();
    Actor_unk22_4();
}

/* Publishes 0x209 at +448 of the runtime record, and calls Actor_unk23_4 for
 * scene 0x64. */
s32 State_SetRuntimeWord448To521(void)
{
    *(s32 *)(gWork + 448) = 0x209;
    if (gCell[224] == (s32)(u32)&Value_00000064) {
        Actor_unk23_4();
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
    s16 sub = gCell[225];

    switch (sub) {
    case 3:
    {
        /* The last two arguments travel on the stack. */
        s32 fifth = 4;
        s32 sixth = 2;
        Actor_SetRect(30, 14, 30, 16, fifth, sixth);
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
    if (Actor_unk2(0x911) != 0) {
        /* Nine distinct call sites, not a loop; the trailing 15 is out of
         * order and is kept that way. */
        Actor_unk24_4(10);
        Actor_unk25_4(11);
        Actor_unk26_4(12);
        Actor_unk27_4(13);
        Actor_unk28_4(14);
        Actor_unk29_4(17);
        Actor_unk30_4(18);
        Actor_unk31_4(19);
        Actor_unk32_4(15);
    } else {
        Actor_unk33_4(13, 2);
    }
    return;

other:
    if (Actor_unk3(0x911) != 0) {
        Actor_unk34_4(16);
        Actor_unk35_4(17);
    }
}
