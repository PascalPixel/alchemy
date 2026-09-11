#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/interaction.h"

/* overlays/scene/state/interaction/actor_position.c */

s32 *GetActorPosition(s32 actor);
s32 *State_Run7(s32);

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
    s32 *p = State_Run7(10);
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
    State_Place3(v0, v1, pos);
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
        State_Run8();
        if (State_Run()!= 0) {
            State_Do(0x9a9);
            State_Run9(80);
            State_Run10();
        }
    }
}

/* overlays/scene/state/interaction/scene_primary_script.c */
extern u8 gWork[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];

u8 *State_Run11();

u8 *State_Run12();
u8 *State_Run13();

void Scene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (State_Check2(0x9a9) == 0) {
        if (State_Run2()!= 0) {
            State_Do2(0x9a9);
            State_Run14(80);
            State_Run15();
        }
    }
}

void Scene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    State_Run16();
    if (State_Check3(0x9aa) == 0) {
        if (State_Run3()!= 0) {
            if (State_Check4(0x207) == 0) {
                State_Run17(80);
                State_Run18();
                State_Do3(0x9aa);
            }
        }
    }
}

void Scene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (State_Check5(0x9aa) == 0) {
        if (State_Run4()!= 0) {
            if (State_Check6(0x207) == 0) {
                State_Run19(80);
                State_Run20();
                State_Do4(0x9aa);
            }
        }
    }
}

void Scene_RunSupplementalSequenceOne(void)
{
    double State_Run21();

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
    State_Run22();
    if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
        *(volatile s32 *)(p10 + 24) = 1;
        *(volatile s32 *)(p10 + 28) = 1;
    } else {
        *(volatile s32 *)(p10 + 24) = -1;
        *(volatile s32 *)(p10 + 28) = -1;
    }
    State_Place(0x30000, 0x30000, 0x10000);
    State_Place2(-1, -1, 0xe666);
    State_Run23(163);
    bias = 4294967296.0;
    count = 0x1df;
    do {
        rec7 = State_Check16();
        held = State_Run24(*(volatile s32 *)(p10 + 36));
        shown = (s32)((u32)(rec7 << 11) >> 16);
        scaled = State_Run25(shown);
        if (shown < 0) {
            bias = 4294967296.0;
            scaled = State_Run21(scaled, bias);
        }
        State_Run26(held, State_Run27(4718.592, scaled));
        *(volatile s32 *)(p10 + 36) = State_Check17();
        State_Run28(1);
        count = (count + -1);
    } while (count >= 0);
    c5 = 6;
    c6 = 6;
    count = 0;
    mode = (c5 << 10);
    do {
        State_Run5(0x4000052, ((mode | (c5 << 5)) | c6));
        State_Run29(1);
        if (State_Check7(count, 20) == 0) {
            c6 = (c6 - 1);
            c5 = (c5 - 1);
        }
        count = (count + 1);
    } while (count <= 69);
    State_SetRect(19, 83, 15, 8, 19, 91);
    State_Do5(0x120);
    State_Run30();
    State_Run31();
    State_Run32();
}

void Scene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (State_Check8(0x9a8) == 0) {
        State_Run6(0x1528, 1);
        State_Do6(0x9a8);
        v5 = 27;
        v6 = 92;
        State_Run33(155);
        State_SetRect2(107, 27, 1, 1, v6, v5);
        State_Run34(39);
        State_SetRect3(108, 27, 1, 1, v6, v5);
        State_Run35(50);
        v6 = 25;
        State_Run36(156);
        State_SetRect4(1, 24, 1, 2, v6, v5);
        State_Run37(40);
        State_SetRect5(2, 24, 1, 2, v6, v5);
        State_Run38(40);
        State_Run39();
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

    flag = *(u8 *)(State_Check18(10) + 91);
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
            rec7 = State_Run11((index + 11));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
            count = *slot + 1;
            *slot = count;
            if (count > 3) {
                *slot = flag;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = State_Check9((i + 11));
            if (*(volatile s32 *)((s32)rec7 + 40) >= 0) {
                if (*(volatile s32 *)((s32)rec7 + 12) <= 0xffff) {
                    State_Run40();
                    *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                    *(s32 *)((s32)rec7 + 72) = 0;
                    *(s32 *)((s32)rec7 + 40) = 0;
                    rec7[91] = 0;
                    State_Run41(106);
                }
            }
        }
        if (State_Check10(10) != 0) {
            State_Run42(10, 1);
            if (State_Check11(0x207) == 0) {
                State_Do7(0x207);
                State_Run43(204);
            } else {
                State_Run44(106);
            }
        }
        if (State_Check12(9) != 0) {
            State_Run45(106);
        }
    }
}

    s32 i;
    u8 *rec7;
    s32 record;
    s32 count;
    volatile s32 *tick;
    s32 base5_200a230;

    rec7 = (u8 *)State_Check13(10);
    if (rec7[91] == 0) {
        tick = (volatile s32 *)gOv3;
        count = *tick + 1;
        *tick = count;
        if ((63 & count) == 0) {
            base5_200a230 = (s32)gOv4;
            record = State_Check19();
            record = State_Check14(record, 6);
            *(volatile s32 *)base5_200a230 = record;
            rec7 = State_Run12((record + 10));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = State_Run13((i + 10));
            record = State_Check15((i + 0x200));
            if (record != 0) {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                *(s32 *)((s32)rec7 + 72) = 0;
                *(s32 *)((s32)rec7 + 40) = 0;
                State_Run46(106);
            } else {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)((s32)rec7 + 72) = record;
                *(s32 *)((s32)rec7 + 40) = record;
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                State_Run47(106);
            }
        }
    }
}

/* overlays/scene/state/interaction/scene_setup.c */

u8 *State_Run48(s32);

u8 *State_Run49(s32);

u8 *State_Run50(s32);

u8 *State_Run51(s32);

s32 *State_Run52();

s32 *State_Run53();

void State_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        State_SetRect6(87, 50, 2, 4, fifth, sixth);
    }
    State_SetRect7(23, 52, 1, 2, strip, 52);
    State_SetRect8(16, 52, 1, 1, strip, 53);
}

void State_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = State_Run48(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    State_SetRect9(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        State_Run49(18)[85] = 0;
        record[35] = 1;
    }

    State_Do8(0x202);
}

void Actor_SetupSlotTwenty(void)
{
    u8 *rec = State_Run50(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    State_SetRect10(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        State_Apply(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        State_Run51(20)[85] = 0;
        rec[35] = 1;
    }

    State_Do9(0x204);
}

void Actor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        State_Run54(State_Run52(selector), 0);
        record = State_Run55(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        State_Run56(0x02008aa1, rank);
    }
}

void Actor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        State_Run57(State_Run53(no), 0);
        rec = State_Run58(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        State_Run59(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        State_Run60(0x020089c1, rate);
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

u8 *State_Run61(s32);

u8 *State_Run62(s32);

u8 *State_Run63(s32);

u8 *State_Run64(s32);

u8 *State_Run65(s32);

u8 *State_Run66(s32);

void State_ApplyFlag300(void)
{
    State_Do10(0x300);
}

void State_SetFlag953(void)
{
    State_Apply2(0x953, 1);
}

void State_ApplyRectAndMarkActor16(void)
{
    u8 *rec = State_Run61(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    State_SetRect11(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        State_Run62(16)[85] = 0;
        rec[35] = 1;
    }

    State_Do11(0x200);
}

void State_ApplyRectAndSetupActor19(void)
{
    u8 *p = State_Run63(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    State_SetRect12(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        State_Apply3(p, 0);
        /* The record is reloaded with the same selector before this store. */
        State_Run64(19)[85] = 0;
        p[35] = 1;
    }

    State_Do12(0x203);
}

void Actor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = State_Run65(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    State_SetRect13(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        State_Apply4(record, 0);
        /* The record is reloaded with the same selector before this store. */
        State_Run66(21)[85] = 0;
        record[35] = 1;
    }

    State_Do13(0x205);
}
