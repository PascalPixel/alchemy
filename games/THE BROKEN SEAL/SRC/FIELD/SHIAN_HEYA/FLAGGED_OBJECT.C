#include "TYPES.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_020006de
#define SetOverlayObjectMode Func_02000710
#define SetOverlayObjectSlot Func_02000770
#define OverlayObject_SetEntryField Func_02000030
#define OverlayObject_SpawnWithMode14 Func_02000048
#define OverlayObject_CreateConfigured Func_020000a0
#define OverlayObject_IntegrateVelocities Func_02000104
#define SceneData_GetTable8778 Func_02000314
#define SceneData_ReturnZero Func_0200031c
#define SceneData_GetTable8868 Func_02000320
#define SceneData_SelectTable89c8Or8890 Func_02000328
#define SceneDialogue_RunActor14FlaggedDialogue Func_0200035c
#define SceneDialogue_RunActor15FlaggedDialogue Func_020003bc
#define SceneDialogue_RunActor16FlaggedDialogue Func_0200041c
#define SceneDialogue_RunActor17FlaggedDialogue Func_02000484
#define SceneDialogue_RunActor10Dialogue Func_020004e4
#define SceneDialogue_RunActor12Dialogue Func_02000504
#define SceneDialogue_RunActor9MotionDialogue Func_02000524
#define SceneData_SelectTable8d4cOr8a28 Func_02000570
#define SceneState_SetRuntimeWord448To521 Func_02000598

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Object_02000030;

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

extern u8 Data_02008778[];
extern u8 Data_02008868[];
extern s16 Data_02000240[];
extern u8 Data_020089c8[];
extern u8 Data_02008890[];
extern u8 Value_00001a40;
extern u8 Data_02008d4c[];
extern u8 Data_02008a28[];
extern u8 *Data_03001ebc;

void Func_020006b4(void *, s32);
void Func_020006cc(void *, s32);
void Func_02000714(void *, s32);
void *Func_020006de(s32, s32, s32, s32);
void Func_02000710(void *, s32);
void Func_02000770(void *, s32);
void Func_020009b0(void *);
u16 *Func_020009d6(s32);
void Func_020009c4(void);
void Func_02000a2e(s32, s32);
s32 Func_020009c6(s32);
void Func_02000a20(s32);
void Func_02000a28(s32);
void Func_02000a38(s32, s32);
void Func_02000a04(void);
u16 *Func_02000a36(s32);
void Func_02000a24(void);
void Func_02000a8e(s32, s32);
s32 Func_02000a26(s32);
void Func_02000a80(s32);
void Func_02000a88(s32);
void Func_02000a98(s32, s32);
void Func_02000a64(void);
u16 *Func_02000a96(s32);
void Func_02000a84(void);
void Func_02000aee(s32, s32);
s32 Func_02000a86(s32);
void Func_02000ae0(s32);
void Func_02000b10(s32, s32);
void Func_02000acc(void);
u16 *Func_02000afe(s32);
void Func_02000aec(void);
void Func_02000b60(s32, s32);
void Func_02000b4a(s32);
void Func_02000b52(s32);
void Func_02000b62(s32, s32);
void Func_02000b2e(void);
void Func_02000b44(void);
void Func_02000b82(s32);
void Func_02000b5e(void);
void Func_02000b64(void);
void Func_02000b7e(void);
void Func_02000b84(void);
void Func_02000bdc(s32, s32, s32);
s32 Func_02000bc6(s32, s32, s32);
void Func_02000b9c(s32);
s32 Func_02000bd6(s32, s32, s32);
void Func_02000bac(s32);
void Func_02000bfc(s32, s32);
void Func_02000bc8(void);
s32 Func_02000678(s32, s32, s32, s32);
void Func_02000c3e(s32);
void Func_02000c76(s32, s32);

void OverlayObject_SetEntryField(void *arg0, s32 arg1)
{
    Object_02000030 *obj = *(Object_02000030 **)((u8 *)arg0 + 0x50);

    obj->field = arg1;
}

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *Func_02000686(s32, s32, s32, s32);

    u8 *ret = Func_02000686(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Func_020006b4(ret, 0);
        Func_02000714(ret, 14);
        Func_020006cc(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *ret = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        SetOverlayObjectMode(ret, 0);
        SetOverlayObjectSlot(ret, 15);
        ret[0x23] = (ret[0x23] & 0xfe) | 2;
        return ret;
    }
    return NULL;
}

void OverlayObject_IntegrateVelocities(void *arg0)
{
    union Slot *s = (union Slot *)arg0;
    union Slot *obj;

    s[2].w += s[17].w;
    s[3].w += s[18].w;
    s[4].w += s[19].w;
    s[6].w += s[12].w;
    s[7].w += s[13].w;
    obj = (union Slot *)s[20].p;
    obj[7].h[1] += s[25].h[0];
}

s32 SceneData_GetTable8778(void)
{
    return (s32)Data_02008778;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTable8868(void)
{
    return (s32)Data_02008868;
}

s32 SceneData_SelectTable89c8Or8890(void)
{
    if (Data_02000240[225] == 8) {
        return (s32)Data_020089c8;
    }
    Func_020009b0(Data_02008890);
    return (s32)Data_02008890;
}

void SceneDialogue_RunActor14FlaggedDialogue(void)
{
    u16 *state = Func_020009d6(0);
    u32 value = state[3];

    Func_020009c4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a2e(16, 14);
    } else {
        if (Func_020009c6(0x895) == 0) {
            Func_02000a20(0x1817);
        } else {
            Func_02000a28(0x1a46);
        }
        Func_02000a38(14, 0);
    }
    Func_02000a04();
}

void SceneDialogue_RunActor15FlaggedDialogue(void)
{
    u16 *state = Func_02000a36(0);
    u32 value = state[3];

    Func_02000a24();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a8e(17, 15);
    } else {
        if (Func_02000a26(0x895) == 0) {
            Func_02000a80(0x1819);
        } else {
            Func_02000a88(0x1a48);
        }
        Func_02000a98(15, 0);
    }
    Func_02000a64();
}

void SceneDialogue_RunActor16FlaggedDialogue(void)
{
    void Func_02000af0();
    void Func_02000af0_a(s32);

    u16 *state = Func_02000a96(0);
    u32 value = state[3];

    Func_02000a84();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000aee(18, 16);
    } else if (Func_02000a86(0x895) == 0) {
        Func_02000ae0(0x181b);
        Func_02000af0(16, 0);
    } else {
        Func_02000af0_a(0x1a4a);
        Func_02000b10(16, 0);
    }
    Func_02000acc();
}

void SceneDialogue_RunActor17FlaggedDialogue(void)
{
    s32 Func_02000af0_b(s32);

    u16 *state = Func_02000afe(0);
    u32 value = state[3];

    Func_02000aec();
    if (value < 0x2000 || value > 0xe000) {
        Func_02000b60(5, 17);
    } else {
        if (Func_02000af0_b(0x895) == 0) {
            Func_02000b4a(0x181d);
        } else {
            Func_02000b52(0x1a4e);
        }
        Func_02000b62(17, 0);
    }
    Func_02000b2e();
}

void SceneDialogue_RunActor10Dialogue(void)
{
    s32 Func_02000ba2(s32, s32);

    Func_02000b44();
    Func_02000b82(0x1a3a);
    Func_02000ba2(10, 0);
    Func_02000b5e();
}

void SceneDialogue_RunActor12Dialogue(void)
{
    void Func_02000ba2_a(s32);
    s32 Func_02000bc2(s32, s32);

    Func_02000b64();
    Func_02000ba2_a((s32)&Value_00001a40);
    Func_02000bc2(12, 0);
    Func_02000b7e();
}

void SceneDialogue_RunActor9MotionDialogue(void)
{
    void Func_02000bc2_a(s32);

    Func_02000b84();
    Func_02000bc2_a(0x1a64);
    Func_02000bdc(9, 0, 20);
    Func_02000bc6(9, 10, 0);
    Func_02000b9c(60);
    Func_02000bd6(9, 0, 0);
    Func_02000bac(20);
    Func_02000bfc(9, 0);
    Func_02000bc8();
}

s32 SceneData_SelectTable8d4cOr8a28(void)
{
    if (Data_02000240[225] == 8) {
        return (s32)Data_02008d4c;
    }
    return (s32)Data_02008a28;
}

s32 SceneState_SetRuntimeWord448To521(void)
{
    void Func_02000686_a(s32, s32, s32, s32);

    s16 scene;

    *(s32 *)(Data_03001ebc + 448) = 521;
    scene = Data_02000240[225];
    if (scene == 4 || scene == 7) {
        Func_02000686_a(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        Func_02000678(0x01cc0000, 0, 0x02380000, 20);
        Func_02000686_a(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        Func_02000c3e(0x12f);
        Func_02000c76(10, 6);
    }
    return 0;
}
