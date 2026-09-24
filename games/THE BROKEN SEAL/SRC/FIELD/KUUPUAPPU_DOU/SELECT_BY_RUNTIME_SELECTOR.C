#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 *Data_03001ebc;

#include "TYPES.H"

u8 *Func_02001664(s32);
u8 *Func_0200174c(s32);
u8 *Func_020017f0(s32);

#include "TYPES.H"

void Func_02001bfc(s32, s32, s32 *);
void Func_02001c38(s32 *, s32, s32, s32);

#include "TYPES.H"

void Func_020004e6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

#include "TYPES.H"

u8 *Func_020016f8(s32);
u8 *Func_020017a0(s32);
void Func_02001cb6();
void Func_02001d08();

#include "TYPES.H"

extern s32 Data_0200a214[];
extern s32 Data_0200a224;
extern s32 Data_0200a228;
extern s32 Data_0200a22c;
extern s32 Data_0200a230;

void Func_02000600();
s32 Func_02001898();
void Func_020018ba();
double Func_02001bd2();
void Func_02001bde();
double Func_02001c1c();
double Func_02001c2a();
s32 Func_02001cd6();
void Func_020015b0();
s32 Func_02001af6();
s32 Func_02001b36();

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call1_02000754(void (*f)(), s32 a0)
{
    double Func_02001b8a();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    double Func_02001b8a();

    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    double Func_02001b8a();

    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    double Func_02001b8a();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_02000aa0(void (*f)(), s32 a0)
{
    void Func_02001b8a();

    f(a0);
}

#include "TYPES.H"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009d34[];
extern u8 Data_02009d4c[];
extern u8 Data_02009ecc[];
extern u8 Data_02009d1c[];

#include "TYPES.H"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009f98[];
extern u8 Data_0200a064[];
extern u8 Data_0200a190[];
extern u8 Data_02009f8c[];

#include "TYPES.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void Func_02000dfc(void);

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009c80[];
extern u8 Data_02009cc0[];
extern u8 Data_02009cfc[];
extern u8 Data_02009c7c[];

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_020098cc[];
extern u8 Data_02009a34[];
extern u8 Data_02009b9c[];
extern u8 Data_0200989c[];

#include "TYPES.H"

/* Deliberate no-op callback. */

#include "TYPES.H"

#include "TYPES.H"

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Func_020016ac(s32);

s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_00000060) {
        return (s32)Data_020098cc;
    }
    if (selector == (s32)&Value_00000061) {
        return (s32)Data_02009a34;
    }
    if (selector == (s32)&Value_00000062) {
        return (s32)Data_02009b9c;
    }
    return (s32)Data_0200989c;

}

u8 *SceneData_GetTable9C5C(void)
{
    return (u8 *)0x02009c5c;
}

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_00000060) {
        return (s32)Data_02009c80;
    }
    if (selector == (s32)&Value_00000061) {
        return (s32)Data_02009cc0;
    }
    if (selector == (s32)&Value_00000062) {
        return (s32)Data_02009cfc;
    }
    return (s32)Data_02009c7c;

}

s32 SceneData_SelectOverlayDataBySelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_00000060) {
        return (s32)Data_02009d34;
    }
    if (selector == (s32)&Value_00000061) {
        return (s32)Data_02009d4c;
    }
    if (selector == (s32)&Value_00000062) {
        return (s32)Data_02009ecc;
    }
    return (s32)Data_02009d1c;

}

s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0) {
            return p;
        }
    }
    return 0;
}

void SceneState_ApplyFlag300(void)
{
    GameFlag_Set(0x300);
}

void SceneState_SetFlag953(void)
{
    Message_ShowCentered(MSG_DOOR_TIGHTLY_LOCKED, 1);
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&Value_00000060) {
        return (s32)Data_02009f98;
    }
    if (selector == (s32)&Value_00000061) {
        return (s32)Data_0200a064;
    }
    if (selector == (s32)&Value_00000062) {
        return (s32)Data_0200a190;
    }
    return (s32)Data_02009f8c;

}

s32 IsActor9AtTile15x54(void)
{
    s32 *actor = Actor_Get(9);
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

void FieldScene_RunFlag9a9GuardedScene(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        Func_020004e6();
        if (Value0(IsActor9AtTile15x54)!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void SceneState_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        Map_CopyCells(87, 50, 2, 4, fifth, sixth);
    }
    Map_CopyCells(23, 52, 1, 2, strip, 52);
    Map_CopyCellAttributes(16, 52, 1, 1, strip, 53);
}

void FieldScene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        if (Value0(IsActor9AtTile15x54)!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void FieldScene_NoOp(void) {}

void SceneState_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        Map_CopyCells(90, 9, 2, 3, p5, p6);
    }
    Map_CopyCells(27, 10, 1, 2, strip, 10);
    Map_CopyCellAttributes(17, 10, 1, 1, strip, 11);
}

s32 SceneActor_IsActor10AtTile16x12(void)
{
    s32 *p = Actor_Get(10);
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

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    Func_02000600();
    if (GameFlag_IsSet(0x9aa) == 0) {
        if (Value0(SceneActor_IsActor10AtTile16x12)!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void Resource3a7_NoOpCallback(void)
{
}

void FieldScene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9aa) == 0) {
        if (Value0(SceneActor_IsActor10AtTile16x12)!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void SceneState_ApplyRectAndMarkActor16(void)
{
    u8 *rec = Actor_Get(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        Func_02001664(16)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x200);
}

void SceneState_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = Actor_Get(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016ac(17)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x201);
}

void SceneState_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = Actor_Get(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        Func_020016f8(18)[85] = 0;
        record[35] = 1;
    }

    GameFlag_Set(0x202);
}

void SceneState_ApplyRectAndSetupActor19(void)
{
    u8 *p = Actor_Get(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    Map_CopyCellAttributes(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_0200174c(19)[85] = 0;
        p[35] = 1;
    }

    GameFlag_Set(0x203);
}

void SceneActor_SetupSlotTwenty(void)
{
    u8 *rec = Actor_Get(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        Actor_SetSpriteFlags(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        Func_020017a0(20)[85] = 0;
        rec[35] = 1;
    }

    GameFlag_Set(0x204);
}

void SceneActor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = Actor_Get(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    Map_CopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Actor_SetSpriteFlags(record, 0);
        /* The record is reloaded with the same selector before this store. */
        Func_020017f0(21)[85] = 0;
        record[35] = 1;
    }

    GameFlag_Set(0x205);
}

void SceneState_DispatchByActorZeroDepth(void)
{
    struct Actor *p = Actor_Get(ACTOR_PARTY_LEADER);

    if (p->f0c >= 0x100000) {
        Func_02000dfc();
    } else {
        SceneState_SetEntries16To21Byte35();
    }
}

void SceneState_SetEntries16To21Byte35(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = Actor_Get(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    double Func_02001b8a();

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
    Event_Begin();
    if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
        *(volatile s32 *)(p10 + 24) = 1;
        *(volatile s32 *)(p10 + 28) = 1;
    } else {
        *(volatile s32 *)(p10 + 24) = -1;
        *(volatile s32 *)(p10 + 28) = -1;
    }
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(163);
    bias = 4294967296.0;
    count = 0x1df;
    do {
        rec7 = Random_Next();
        held = Func_02001c1c(*(volatile s32 *)(p10 + 36));
        shown = (s32)((u32)(rec7 << 11) >> 16);
        scaled = Func_02001c2a(shown);
        if (shown < 0) {
            bias = 4294967296.0;
            scaled = Func_02001b8a(scaled, bias);
        }
        Func_02001bde(held, Func_02001bd2(4718.592, scaled));
        *(volatile s32 *)(p10 + 36) = Func_02001cd6();
        Event_Wait(1);
        count = (count + -1);
    } while (count >= 0);
    c5 = 6;
    c6 = 6;
    count = 0;
    mode = (c5 << 10);
    do {
        Call2(Func_020018ba, 0x4000052, ((mode | (c5 << 5)) | c6));
        Event_Wait(1);
        if (Value2(Func_02001898, count, 20) == 0) {
            c6 = (c6 - 1);
            c5 = (c5 - 1);
        }
        count = (count + 1);
    } while (count <= 69);
    Map_CopyCells(19, 83, 15, 8, 19, 91);
    Audio_PlayCue(0x120);
    Map_Redraw();
    MapRender_WaitForValues();
    Event_End();
}

void FieldScene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (GameFlag_IsSet(0x9a8) == 0) {
        Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
        GameFlag_Set(0x9a8);
        v5 = 27;
        v6 = 92;
        Audio_PlayCue(155);
        Map_CopyCells(107, 27, 1, 1, v6, v5);
        Event_Wait(39);
        Map_CopyCells(108, 27, 1, 1, v6, v5);
        Event_Wait(50);
        v6 = 25;
        Audio_PlayCue(156);
        Map_CopyCells(1, 24, 1, 2, v6, v5);
        Event_Wait(40);
        Map_CopyCells(2, 24, 1, 2, v6, v5);
        Event_Wait(40);
        FieldScene_RunSupplementalSequenceOne();
    }
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 index;

    flag = *(u8 *)(Func_02001af6(10) + 91);
    if (flag == 0) {
        if (++Data_0200a224 > 190) {
            Data_0200a224 = 0;
        }
        index = Data_0200a228;
        if (Data_0200a214[index] == Data_0200a224) {
            rec7 = Actor_Get((index + 11));
            *(s32 *)(rec7 + 72) = 0xa3d;
            if (++Data_0200a228 > 3) {
                Data_0200a228 = 0;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = Value1(Engine_ActorGet, (i + 11));
            if (*(s32 *)(rec7 + 40) >= 0) {
                if (*(s32 *)(rec7 + 12) <= 0xffff) {
                    Func_020015b0();
                    *(s32 *)(rec7 + 12) = 0xff0000;
                    *(s32 *)(rec7 + 72) = 0;
                    *(s32 *)(rec7 + 40) = 0;
                    rec7[91] = 0;
                    Audio_PlayCue(106);
                }
            }
        }
        if (Value1(SceneActor_LiftLowActorOnSubjectTile, 10) != 0) {
            Actor_SetAnimation(10, 1);
            if (GameFlag_IsSet(0x207) == 0) {
                GameFlag_Set(0x207);
                Audio_PlayCue(204);
            } else {
                Audio_PlayCue(106);
            }
        }
        if (Value1(SceneActor_LiftLowActorOnSubjectTile, 9) != 0) {
            Audio_PlayCue(106);
        }
    }
}

void Func_02000aa0(void)
{
    void Func_02001b8a();

    s32 i;
    u8 *rec7;
    s32 record;
    s32 *selected;

    rec7 = (u8 *)Value1(Engine_ActorGet, 10);
    if (rec7[91] == 0) {
        if ((++Data_0200a22c & 63) == 0) {
            selected = &Data_0200a230;
            record = Random_Next();
            record = Value2(Func_02001b36, record, 6);
            *selected = record;
            rec7 = Actor_Get((record + 10));
            *(s32 *)(rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = Actor_Get((i + 10));
            record = GameFlag_IsSet((i + 0x200));
            if (record != 0) {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 12) = 0xff0000;
                *(s32 *)(rec7 + 72) = 0;
                *(s32 *)(rec7 + 40) = 0;
                Audio_PlayCue(106);
            } else {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 72) = record;
                *(s32 *)(rec7 + 40) = record;
                *(s32 *)(rec7 + 12) = 0xff0000;
                Audio_PlayCue(106);
            }
        }
    }
}

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    Func_02001bfc(v0, v1, pos);
    Func_02001c38(rec, pos[0], pos[1], pos[2]);
}

void SceneActor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Actor_SetSpriteFlags(Actor_Get(selector), 0);
        record = Actor_Get(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Func_02001cb6(0x02008aa1, rank);
    }
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        Actor_SetSpriteFlags(Actor_Get(no), 0);
        rec = Actor_Get(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        Actor_SetSpritePriority(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        Func_02001d08(0x020089c1, rate);
    }
}
