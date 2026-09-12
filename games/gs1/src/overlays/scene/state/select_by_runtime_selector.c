#include "scene.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "types.h"
#include "add_parts_record.h"
#include "add_parts_body.inc"

/* overlays/scene/state/interaction/scene_data_select_by_runtime_selector.c */
/* overlays/scene/state/interaction/scene_data_select_by_runtime_selector.c */
/* overlays/scene/state/interaction/select_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062


s32 SceneData_SelectByRuntimeSelector(void)
{
}

/* overlays/scene/state/interaction/get_table_9c5c.c */
u8 *SceneData_GetTable9C5C(void)
{
    return (u8 *)0x02009c5c;
}

/* overlays/scene/state/interaction/select_secondary_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062


s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
}

/* overlays/scene/state/interaction/select_overlay_data_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062


s32 SceneData_SelectOverlayDataBySelector(void)
{
}

/* overlays/scene/state/interaction/interaction.c */
/* overlays/scene/state/interaction/actor_position.c */

s32 *GetActorPosition(s32 actor);
s32 *State_unk7_4(s32);

s32 IsActor9AtTile15x54(void)
{
    s32 *actor = GetActorPosition(9);
    s32 z = actor[4];
    s32 x;
    s32 z_tile;
    s32 x_tile;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = actor[2];
    z_tile = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    x_tile = x >> 20;
    if (x_tile == 15 && z_tile == 54) {
        return 1;
    }
    return 0;
}

s32 Actor_IsActor10AtTile16x12(void)
{
    s32 *p = State_unk7_4(10);
    s32 z = p[4];
    s32 x;
    s32 cz;
    s32 cx;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = p[2];
    cz = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    cx = x >> 20;
    if (cx == 16 && cz == 12) {
        return 1;
    }
    return 0;
}

void Actor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    State_unk3_3(v0, v1, pos);
    State_SetMode(rec, pos[0], pos[1], pos[2]);
}

/* overlays/scene/state/interaction/actor_search.c */
extern u8 *gWork;

s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/state/interaction/callbacks.c */
void Resource3a7_NoOpCallback(void)
{
}

/* overlays/scene/state/interaction/conditional_scene_setup.c */
extern u8 gWork[];

void Scene_RunFlag9a9GuardedScene(void)
{
    u32 i;
    s32 record;

    if (State_Check(0x9a9) == 0) {
        State_unk8_4();
        if (State_Run()!= 0) {
            State_Do(0x9a9);
            State_unk9_4(80);
            State_unk10_4();
        }
    }
}

/* overlays/scene/state/interaction/scene_primary_script.c */
extern u8 gWork[];
extern u8 gOvInteraction[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];

u8 *State_unk11_4();

u8 *State_unk12_4();
u8 *State_unk13_4();

void Scene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (State_unk2(0x9a9) == 0) {
        if (State_unk2_4()!= 0) {
            State_unk2_2(0x9a9);
            State_unk14_4(80);
            State_unk15_4();
        }
    }
}

void Scene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    State_unk16_4();
    if (State_unk3(0x9aa) == 0) {
        if (State_unk3_4()!= 0) {
            if (State_unk4(0x207) == 0) {
                State_unk17_4(80);
                State_unk18_4();
                State_unk3_2(0x9aa);
            }
        }
    }
}

void Scene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (State_unk5(0x9aa) == 0) {
        if (State_unk4_4()!= 0) {
            if (State_unk6(0x207) == 0) {
                State_SetRectAndMarkActor17(80);
                State_unk20_4();
                State_unk4_2(0x9aa);
            }
        }
    }
}

void Scene_RunSupplementalSequenceOne(void)
{
    double State_unk21_4();

    s32 p10;
    s32 count;
    s32 rec7;
    s32 shown;
    double bias;
    double held;
    double scaled;
    s32 c5;
    s32 c6;
    s32 mode;

    p10 = (0x164 + *(volatile s32 *)0x03001e70);
    State_unk22_4();
    if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
        *(volatile s32 *)(p10 + 24) = 1;
        *(volatile s32 *)(p10 + 28) = 1;
    } else {
        *(volatile s32 *)(p10 + 24) = -1;
        *(volatile s32 *)(p10 + 28) = -1;
    }
    State_Place(0x30000, 0x30000, 0x10000);
    State_unk2_3(-1, -1, 0xe666);
    State_unk23_4(163);
    bias = 4294967296.0;
    count = 0x1df;
    do {
        rec7 = State_unk16();
        held = State_unk24_4(*(volatile s32 *)(p10 + 36));
        shown = (s32)((u32)(rec7 << 11) >> 16);
        scaled = State_unk25_4(shown);
        if (shown < 0) {
            bias = 4294967296.0;
            scaled = State_unk21_4(scaled, bias);
        }
        State_unk26_4(held, State_unk27_4(4718.592, scaled));
        *(volatile s32 *)(p10 + 36) = State_unk17();
        State_unk28_4(1);
        count = (count + -1);
    } while (count >= 0);
    c5 = 6;
    c6 = 6;
    count = 0;
    mode = (c5 << 10);
    do {
        State_unk5_4(0x4000052, ((mode | (c5 << 5)) | c6));
        State_unk29_4(1);
        if (State_unk7(count, 20) == 0) {
            c6 = (c6 - 1);
            c5 = (c5 - 1);
        }
        count = (count + 1);
    } while (count <= 69);
    State_SetRect(19, 83, 15, 8, 19, 91);
    State_unk5_2(0x120);
    State_unk30_4();
    State_unk31_4();
    State_unk32_4();
}

void Scene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (State_unk8(0x9a8) == 0) {
        State_unk6_4(0x1528, 1);
        State_unk6_2(0x9a8);
        v5 = 27;
        v6 = 92;
        State_unk33_4(155);
        State_unk2_5(107, 27, 1, 1, v6, v5);
        State_unk34_4(39);
        State_unk3_5(108, 27, 1, 1, v6, v5);
        State_unk35_4(50);
        v6 = 25;
        State_unk36_4(156);
        State_unk4_5(1, 24, 1, 2, v6, v5);
        State_unk37_4(40);
        State_unk5_5(2, 24, 1, 2, v6, v5);
        State_unk38_4(40);
        State_unk39_4();
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 count;
    s32 index;
    volatile s32 *tick;
    volatile s32 *slot;
    u8 *table;

    flag = *(u8 *)(State_unk18(10) + 91);
    if (flag == 0) {
        tick = (volatile s32 *)gOvInteraction;
        count = *tick + 1;
        *tick = count;
        if (count > 190) {
            *tick = flag;
        }
        slot = (volatile s32 *)gOv2;
        index = *slot;
        table = (u8 *)0x0200a214;
        if (*(s32 *)(table + (index << 2)) == *tick) {
            rec7 = State_unk11_4((index + 11));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
            count = *slot + 1;
            *slot = count;
            if (count > 3) {
                *slot = flag;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = State_unk9((i + 11));
            if (*(volatile s32 *)((s32)rec7 + 40) >= 0) {
                if (*(volatile s32 *)((s32)rec7 + 12) <= 0xffff) {
                    State_unk40_4();
                    *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                    *(s32 *)((s32)rec7 + 72) = 0;
                    *(s32 *)((s32)rec7 + 40) = 0;
                    rec7[91] = 0;
                    State_unk41_3(106);
                }
            }
        }
        if (State_unk10(10) != 0) {
            State_unk42_3(10, 1);
            if (State_unk11(0x207) == 0) {
                State_unk7_2(0x207);
                State_unk43_3(204);
            } else {
                State_unk44_3(106);
            }
        }
        if (State_unk12(9) != 0) {
            State_unk45_3(106);
        }
    }
}

    s32 i;
    u8 *rec7;
    s32 record;
    s32 count;
    volatile s32 *tick;
    s32 base5_200a230;

    rec7 = (u8 *)State_unk13(10);
    if (rec7[91] == 0) {
        tick = (volatile s32 *)gOv3;
        count = *tick + 1;
        *tick = count;
        if ((63 & count) == 0) {
            base5_200a230 = (s32)gOv4;
            record = State_unk19();
            record = State_unk14(record, 6);
            *(volatile s32 *)base5_200a230 = record;
            rec7 = State_unk12_4((record + 10));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = State_unk13_4((i + 10));
            record = State_unk15((i + 0x200));
            if (record != 0) {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                *(s32 *)((s32)rec7 + 72) = 0;
                *(s32 *)((s32)rec7 + 40) = 0;
                State_unk46_3(106);
            } else {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)((s32)rec7 + 72) = record;
                *(s32 *)((s32)rec7 + 40) = record;
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                State_unk47_3(106);
            }
        }
    }
}

/* overlays/scene/state/interaction/scene_setup.c */

u8 *State_unk48_3(s32);

u8 *State_unk49_3(s32);

u8 *State_unk50_3(s32);

u8 *State_unk51_3(s32);

s32 *State_unk52_3();

s32 *State_unk53_3();

void State_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        State_unk6_5(87, 50, 2, 4, fifth, sixth);
    }
    State_unk7_5(23, 52, 1, 2, strip, 52);
    State_unk8_5(16, 52, 1, 1, strip, 53);
}

void State_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = State_unk48_3(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    State_unk9_5(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        State_unk49_3(18)[85] = 0;
        record[35] = 1;
    }

    State_unk8_2(0x202);
}

void Actor_SetupSlotTwenty(void)
{
    u8 *rec = State_unk50_3(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    State_unk10_5(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        State_ApplyInteraction(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        State_unk51_3(20)[85] = 0;
        rec[35] = 1;
    }

    State_unk9_2(0x204);
}

void Actor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        State_unk54_3(State_unk52_3(selector), 0);
        record = State_unk55_3(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        State_unk56_2(0x02008aa1, rank);
    }
}

void Actor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        State_unk57_2(State_unk53_3(no), 0);
        rec = State_unk58_2(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        State_unk59(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        State_unk60(0x020089c1, rate);
    }
}

/* overlays/scene/state/interaction/scene_state_interaction_tail.c */
s32 State_IsStateAtMostOneB(u32 *state)
{
    return *state <= 1;
}

s32 State_IsStateAtMostOneBB(u32 *state)
{
    s32 ret = 0;

    if (*state <= 1) {
        ret = 1;
    }
    return ret;
}

/* overlays/scene/state/interaction/state_update_head.c */

u8 *State_unk61(s32);

u8 *State_unk62(s32);

u8 *State_unk63(s32);

u8 *State_unk64(s32);

u8 *State_unk65(s32);

u8 *State_unk66(s32);

void State_ApplyFlag300(void)
{
    State_Apply2Interaction(0x300);
}

void State_SetFlag953(void)
{
    State_Apply2Interaction(0x953, 1);
}

void State_ApplyRectAndMarkActor16(void)
{
    u8 *rec = State_unk61(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    State_unk11_5(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        State_unk62(16)[85] = 0;
        rec[35] = 1;
    }

    State_unk11_2(0x200);
}

void State_ApplyRectAndSetupActor19(void)
{
    u8 *p = State_unk63(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    State_unk12_5(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        State_Apply3(p, 0);
        /* The record is reloaded with the same selector before this store. */
        State_unk64(19)[85] = 0;
        p[35] = 1;
    }

    State_unk12_2(0x203);
}

void Actor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = State_unk65(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    State_unk13_5(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        State_Apply4(record, 0);
        /* The record is reloaded with the same selector before this store. */
        State_unk66(21)[85] = 0;
        record[35] = 1;
    }

    State_unk13_2(0x205);
}

/* overlays/scene/state/interaction/select_data_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* overlays/scene/state/interaction/scene_no_op.c */
/* overlays/scene/state/interaction/no_op.c */
/* Deliberate no-op callback. */
void Scene_NoOp(void) {}

/* overlays/scene/state/interaction/apply_three_rects_rows_9_and_10.c */
void State_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        State_SetRectNoOp(90, 9, 2, 3, p5, p6);
    }
    State_unk2_5(27, 10, 1, 2, strip, 10);
    State_unk3_5(17, 10, 1, 1, strip, 11);
}

/* overlays/scene/state/interaction/state_configure_region26_30_and_mark_actor17.c */
extern u8 *State_RunAndMarkActor17(s32);

extern u8 *State_unk2_4AndMarkActor17(s32);

void State_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = State_RunAndMarkActor17(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    State_SetRectAndMarkActor17(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        State_unk2_4AndMarkActor17(17)[85] = 0;
        rec[35] = 1;
    }

    State_DoAndMarkActor17(0x201);
}

/* overlays/scene/state/interaction/state_dispatch_by_actor_zero_depth.c */
/* overlays/scene/state/interaction/dispatch_by_actor_zero_depth.c */
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern struct Actor *State_RunActorZeroDepth(s32);

void State_DispatchByActorZeroDepth(void)
{
    struct Actor *p = State_RunActorZeroDepth(0);

    if (p->f0c >= 0x100000) {
        State_unk2_4ActorZeroDepth();
    } else {
        State_unk3_4();
    }
}

/* overlays/scene/state/interaction/set_entries_16_21_byte_35.c */
extern u8 *State_RunActorZeroDepth(s32);

void State_SetEntries16To21Byte35(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = State_RunActorZeroDepth(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}

/* overlays/scene/state/interaction/add_soft_float_parts.c */
/* overlays/scene/state/interaction/add_parts.c */
extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
}

/* overlays/scene/state/interaction/add_soft_double.c */
/*
 * Soft-float double addition for resource_3a7.  Both operands are unpacked into
 * records, added, and the result repacked.
 *
 * The frame is laid out in this order and no other: second operand, first
 * operand, result record, second record, first record.  A double is carried as
 * a high (sign and exponent) word followed by a low word, so the packed value
 * is a u64 whose low half is the high word, not a native double.  The record
 * layout beyond the sign word is not established here and is kept opaque.  The
 * two unpack calls reach one routine; each site keeps its own name.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *State_RunSoftFloatParts(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftDouble AddSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packed_b;
        SoftDouble packed_a;
        SoftFloatRecord result;
        SoftFloatRecord record_b;
        SoftFloatRecord record_a;
    } frame;
    u32 *wa = (u32 *)&frame.packed_a;
    volatile u32 *wb = (u32 *)&frame.packed_b;

    wa[0] = a0;
    wa[1] = a1;
    wb[0] = b0;
    wb[1] = b1;

    State_ApplySoftFloatParts(&frame.packed_a, &frame.record_a);
    State_Apply2SoftFloatParts(&frame.packed_b, &frame.record_b);

    return State_DoSoftFloatParts(State_RunSoftFloatParts(&frame.record_a, &frame.record_b, &frame.result));
}

/* overlays/scene/state/interaction/subtract_soft_double.c */
/*
 * Soft-float double subtraction for resource_3a7.  It shares the addition
 * core, reaching it with the second operand's sign toggled.
 *
 * The frame is laid out in this order and no other: second operand, first
 * operand, result record, second record, first record.  A double is carried as
 * a high (sign and exponent) word followed by a low word, so the packed value
 * is a u64 whose low half is the high word, not a native double.  The record
 * layout beyond the sign word is not established here and is kept opaque.  The
 * two unpack calls reach one routine; each site keeps its own name.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *State_RunSoftFloatParts(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftDouble SubtractSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packed_b;
        SoftDouble packed_a;
        SoftFloatRecord result;
        SoftFloatRecord record_b;
        SoftFloatRecord record_a;
    } frame;
    u32 *wa = (u32 *)&frame.packed_a;
    volatile u32 *wb = (u32 *)&frame.packed_b;

    wa[0] = a0;
    wa[1] = a1;
    wb[0] = b0;
    wb[1] = b1;

    State_ApplySoftFloatParts(&frame.packed_a, &frame.record_a);
    {
        SoftFloatRecord *rb = &frame.record_b;

        State_Apply2SoftFloatParts(&frame.packed_b, rb);

        /*
         * Toggling the sign word of the unpacked second operand is what turns
         * the shared addition core into a subtraction, and is the only
         * difference from the addition entry.
         */
        rb->word[1] ^= 1u;

        return State_DoSoftFloatParts(State_RunSoftFloatParts(&frame.record_a, rb, &frame.result));
    }
}

/* overlays/scene/state/interaction/get_table_9888.c */
extern u8 gOvSoftFloatParts[];

u8 *SceneData_GetTable9888(void)
{
    return gOvSoftFloatParts;
}

/* overlays/scene/state/interaction/is_state_at_most_one.c */
s32 State_IsStateAtMostOne(u32 *state)
{
    return *state <= 1;
}

/* overlays/scene/state/interaction/is_state_4.c */
s32 State_IsState4(s32 *state)
{
    return *state == 4;
}

/* overlays/scene/state/interaction/is_state_two_c.c */
s32 State_IsStateTwoC(s32 *state)
{
    return *state == 2;
}

/* overlays/scene/state/interaction/signed_int_to_soft_double.c */
/*
 * Signed integer to soft-float conversion in resource_3a7: build the
 * library's unpacked record on the stack and hand it to the packer.
 */

/*
 * The packed double is carried as a u64 whose low half is the r0 word,
 * because r0 holds the high word of the double here; a `double` would not
 * spell that.
 */
typedef u64 SoftDouble;

/*
 * The unpacked record is 20 bytes: class at +0 (2 = zero, 3 = finite),
 * sign at +4, exponent seed at +8, and the significand at +12 and +16.
 */
typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

/*
 * Zero takes class 2 straight to the pack call; 0x80000000 returns the
 * literal -2147483648.0 without packing. Otherwise normalisation loops on
 * an unsigned compare, so a significand already above the threshold is
 * left alone. The 124-byte owner includes its three pool words.
 */
SoftDouble SignedIntToSoftDouble(s32 value)
{
    SoftFloatRecord record;
    u32 sign;

    record.cls = 3u;
    sign = (u32)value >> 31;
    record.sign = sign;

    if (value == 0) {
        record.cls = 2u;
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            return (SoftDouble)0xC1E00000u;
        }
        record.fraction = (u64)(s64)(-value);
    } else {
        record.fraction = (u64)(s64)value;
    }

    while (record.fraction <= 0x0FFFFFFFFFFFFFFFu) {
        record.fraction = record.fraction << 1;
        record.exponent -= 1;
    }

pack:
    return State_DoSoftFloatParts(&record);
}

/* overlays/scene/state/interaction/soft_double_to_signed_int.c */
typedef u64 SoftDouble;

typedef union FloUnion {
    SoftDouble value;
    struct {
        u32 lo;
        u32 hi;
    } words;
} FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exp;
    u64 fraction;
} SoftFloatRecord;

s32 Runtime_SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord rec;
    volatile FloUnion u;
    volatile FloUnion *p;
    s32 exp;
    u64 frac;

    p = &u;
    p->words.lo = high;
    p->words.hi = low;
    State_ApplySoftFloatParts((FloUnion *)p, &rec);

    if (State_DoSoftFloatParts(&rec) != 0u) {
        return 0;
    }
    if (State_unk2_2(&rec) != 0u) {
        return 0;
    }
    if (State_unk3_2(&rec) == 0u) {
        exp = rec.exp;
        if (exp < 0) {
            return 0;
        }
        if (exp <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(rec.sign != 0u);

convert:
    frac = State_Apply2SoftFloatParts(rec.fraction, (u32)(60 - exp));
    if (rec.sign != 0u) {
        return -(s32)frac;
    }
    return (s32)frac;
}

/* overlays/scene/state/interaction/state_is_state_four.c */
/* overlays/scene/state/interaction/is_state_four.c */
s32 State_IsStateFour(s32 *state)
{
    s32 ret = 0;

    if (*state == 4) {
        ret = 1;
    }
    return ret;
}

/* overlays/scene/state/interaction/is_state_two.c */
s32 State_IsStateTwo(s32 *state)
{
    return *state == 2;
}

/* overlays/scene/state/interaction/logical_shift_right_64.c */
/*
 * resource_3a7 soft-float library: 64-bit logical right shift.  The overlay
 * carries its own copy of this library.
 */

/*
 * r0/r1 hold the low and high words and r2 the count -- the ordinary pair
 * placement for an unsigned long long, so it is spelled that way here.  A
 * count of 32 or more takes the `32 - n <= 0' path with a shift distance of
 * zero, avoiding an undefined `>> 32'; counts of 64 and over are unmasked
 * here and undefined in the original too.  The else arm's statement order --
 * carry, shifted low, high, then the OR -- is load-bearing, as is zeroing the
 * high half first in the other arm.
 */
typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;
u64 Runtime_LogicalShiftRight64(u64 value, u32 count)
{
    SplitU64 in;
    SplitU64 out;
    s32 spare;
    u32 carry;
    u32 shifted;

    if (count == 0u) {
        return value;
    }
    in.whole = value;
    spare = 32 - (s32)count;
    if (spare <= 0) {
        spare = -spare;
        out.parts.hi = 0u;
        out.parts.lo = in.parts.hi >> spare;
    } else {
        carry = in.parts.hi << spare;
        shifted = in.parts.lo >> count;
        out.parts.hi = in.parts.hi >> count;
        out.parts.lo = shifted | carry;
    }
    return out.whole;
}

/* overlays/scene/state/interaction/pack_soft_double.c */
/*
 * Double-precision packer from the GCC soft-float runtime shape.  The overlay
 * copy uses the shared class predicates and 64-bit logical-right-shift helper.
 * Its packed-word order is reversed before return, matching the runtime's
 * FLOAT_WORD_ORDER_MISMATCH path and the independently observed callers.
 */
typedef u64 SoftDouble;

enum SoftFloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
};

typedef struct SoftFloatRecord {
    enum SoftFloatClass cls;
    u32 sign;
    s32 exponent;
    union {
        u64 ll;
        u32 words[2];
    } fraction;
} SoftFloatRecord;

typedef union SoftDoubleUnion {
    SoftDouble value;
    u64 raw;
    u32 words[2];
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exponent : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} SoftDoubleUnion;

SoftDouble Runtime_PackSoftDouble(SoftFloatRecord *src)
{
    SoftDoubleUnion dst;
    u64 fraction = src->fraction.ll;
    s32 sign = (s32)src->sign;
    s32 exp = 0;

    if (State_Check(src)) {
        exp = 0x7ff;
        if (src->cls == CLASS_QNAN || 1) {
            fraction |= 0x8000000000000ULL;
        }
    } else if (State_unk2(src)) {
        exp = 0x7ff;
        fraction = 0;
    } else if (State_unk3(src)) {
        exp = 0;
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->exponent < -1022) {
            s32 shift = -1022 - src->exponent;

            exp = 0;
            if (shift > 56) {
                fraction = 0;
            } else {
                s32 lowbit = (fraction & ((1 << shift) - 1)) ? 1 : 0;
                fraction = State_ApplyIsStateFour(fraction, (u32)shift) | (u32)lowbit;
            }
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 60)) {
                exp += 1;
            }
            fraction >>= 8;
        } else if (src->exponent > 1023) {
            exp = 0x7ff;
            fraction = 0;
        } else {
            exp = src->exponent + 1023;
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 61)) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exponent = (u32)exp;
    dst.bits.sign = (u32)sign;

    {
        u32 tmp = dst.words[0];
        dst.words[0] = dst.words[1];
        dst.words[1] = tmp;
    }

    return dst.value;
}

/* overlays/scene/state/interaction/state_is_state_four_direct.c */
/* overlays/scene/state/interaction/is_state_four_direct.c */
s32 State_IsStateFourDirect(s32 *state)
{
    return *state == 4;
}

/* overlays/scene/state/interaction/is_state_two_b.c */
s32 State_IsStateTwoB(s32 *state)
{
    return *state == 2;
}

/* overlays/scene/state/interaction/unpack_soft_double.c */
/*
 * Soft-float double unpack -- resource_3a7.  Splits a packed pair into the
 * class, sign, exponent and normalised significand record the rest of the
 * family works on.
 */
typedef float DoubleType __attribute__((mode(DF)));
typedef unsigned int HalfFractionType __attribute__((mode(SI)));
typedef unsigned int FractionType __attribute__((mode(DI)));

typedef enum FloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} FloatClass;

/*
 * Record layout: class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite,
 * 4 infinity), sign taken from bit 31 of the high word, unbiased exponent,
 * then the significand normalised so that bit 28 of its high word is set.
 */
typedef struct SoftFloatRecord {
    FloatClass cls;
    unsigned int sign;
    int exponent;
    union {
        FractionType whole;
        HalfFractionType halves[2];
    } fraction;
} SoftFloatRecord;

/*
 * The packed value is a pair of words whose first word is the high word
 * carrying the sign and exponent, so it is copied into a local and read back
 * through the narrower bitfields below.
 */
typedef union PackedDouble {
    DoubleType value;
    FractionType raw;
    HalfFractionType words[2];
    struct {
        FractionType fraction : 52 __attribute__((packed));
        unsigned int exponent : 11 __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} PackedDouble;

/*
 * The four cases are taken in this order: zero; a denormal, seeded to -1022
 * and normalised with an unsigned compare; exponent 0x7ff, which stores the
 * significand unshifted for both NaN classes; and the normal case, which
 * re-inserts the implicit leading bit.  One constant materialised in the
 * normal path is never used, so it is left unmodelled.
 */
void Runtime_UnpackSoftDouble(PackedDouble *source, SoftFloatRecord *record)
{
    FractionType fraction;
    int exponent;
    int sign;
    PackedDouble swapped;

    swapped.words[0] = source->words[1];
    swapped.words[1] = source->words[0];
    source = &swapped;

    fraction = source->bits.fraction;
    exponent = source->bits.exponent;
    sign = source->bits.sign;

    record->sign = sign;
    if (exponent == 0) {
        if (fraction == 0) {
            record->cls = CLASS_ZERO;
        } else {
            record->exponent = exponent - 1023 + 1;
            fraction <<= 8;
            record->cls = CLASS_NUMBER;
            while (fraction < (((FractionType)1) << (52 + 8))) {
                fraction <<= 1;
                record->exponent--;
            }
            record->fraction.whole = fraction;
        }
    } else if (exponent == 0x7ff) {
        if (fraction == 0) {
            record->cls = CLASS_INFINITY;
        } else {
            if (fraction & 0x8000000000000LL) {
                record->cls = CLASS_QNAN;
            } else {
                record->cls = CLASS_SNAN;
            }
            record->fraction.whole = fraction;
        }
    } else {
        record->exponent = exponent - 1023;
        record->cls = CLASS_NUMBER;
        record->fraction.whole = (fraction << 8) | (((FractionType)1) << (52 + 8));
    }
}
