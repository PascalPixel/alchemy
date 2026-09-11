#include "types.h"
#include "scene.h"

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
extern u8 gOv[];
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
                State_unk19_4(80);
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
        tick = (volatile s32 *)gOv;
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
        State_Apply(rec, 0);
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
    State_unk10_2(0x300);
}

void State_SetFlag953(void)
{
    State_Apply2(0x953, 1);
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
