#include "types.h"

#define SceneData_GetTableD8f8 Func_02000030
#define SceneData_GetTableDa60 Func_02000038
#define SceneData_GetTableDa80 Func_02000040
#define SceneData_SelectTableBySceneRange Func_02000048
#define ActorPresentation_SetSceneCellByAngle Func_02000080
#define FieldScene_RunObjectTwentySixPositionCheck Func_020000fc
#define FieldScene_RunActorNineteenAngleDialogue Func_0200013c
#define FieldScene_RunActorTwentyAngleDialogue Func_0200019c
#define FieldScene_RunActorTwentyThreeAngleDialogue Func_020001fc
#define SceneState_RunGuardedActorStep Func_02000378
#define SceneDialogue_PromptAndCountSkip Func_020003bc
struct Object {
    u8 filler00[8];
    s32 x;
};

extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern u8 Value_00001280;
extern u8 *Data_03001ebc;

void Func_02004dbc(u8 *);
u8 *Func_02004e04(s32);
u8 *Func_02004e14(s32);
u8 *Func_02004e3e(s32);
u8 *Func_02004e4e(s32);
void Func_02004f32(void);
void Func_02004d9e(s32, s32, s32, s32, s32, s32);
void Func_02004f6c(void);
void Func_02004dd8(s32, s32, s32, s32, s32, s32);
void Func_02004e46(void);
void Func_02004e46_b(s32);
struct Object *Func_02004e84(s32);
void Func_02004e08(s32, s32, s32, s32, s32, s32);
void Func_02004e7a(void);
u8 *Func_02004ebe(s32);
void Func_02004e8c(void);
void Func_02005066(s32, s32);
s32 Func_02004e76(s32);
void Func_02004f80(s32);
void Func_02004f88(s32);
void Func_02004fa0(s32, s32);
void Func_02004ecc(void);
u8 *Func_02004f1e(s32);
void Func_02004eec(void);
void Func_020050c6(s32, s32);
s32 Func_02004ed6(s32);
void Func_02004fe0(s32);
void Func_02004fe8(s32);
void Func_02005000(s32, s32);
void Func_02004f2c(void);
u8 *Func_02004f7e(s32);
void Func_02004f4c(void);
void Func_0200512e(s32, s32);
s32 Func_02004f36(s32);
void Func_02005040(s32);
void Func_02005048(s32);
void Func_02005060(s32, s32);
void Func_02004f8c(void);
s32 Func_020050fe(void);
void Func_020050d6(void);
void Func_0200516e(s32, s32);
void Func_020050dc(s32);
void Func_020051cc(s32, s32);
void Func_020050f8(void);
void Func_020049ea(s32, s32, s32);
s32 Func_020051ea(s32, s32);
s32 Func_0200514a(s32, s32);
void Func_02005216(s32, s32);

/*
 * Head export stub at 0x02000030. The eight-byte owner includes its one pool
 * word at 0x02000034, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableD8f8(void)
{
    return (u8 *)0x0200d8f8;
}

/*
 * Head export stub at 0x02000038. The eight-byte owner includes its one pool
 * word at 0x0200003c, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa60(void)
{
    return (u8 *)0x0200da60;
}

/*
 * Head export stub at 0x02000040. The eight-byte owner includes its one pool
 * word at 0x02000044, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa80(void)
{
    return (u8 *)0x0200da80;
}

u8 *SceneData_SelectTableBySceneRange(void)
{
    s16 *tbl = Data_02000240;
    u8 *ret;
    s32 v = tbl[225];
    s32 low = 15;

    if (v <= 17) {
        if (v >= low) {
            ret = Data_0200dcc8;
        } else {
            ret = Data_0200dab8;
        }
    } else {
        ret = Data_0200dab8;
    }
    Func_02004dbc(ret);
    return ret;
}

void ActorPresentation_SetSceneCellByAngle(void)
{
    s32 x;
    s32 z;

    if (*(u16 *)(Func_02004e04(0) + 6) >= 0xa000
        && *(u16 *)(Func_02004e14(0) + 6) <= 0xe000) {
        Func_02004f32();
        x = 42;
        z = 85;
        Func_02004d9e(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Func_02004e3e(0) + 6) >= 0x2000
               && *(u16 *)(Func_02004e4e(0) + 6) <= 0x6000) {
        Func_02004f6c();
        x = 42;
        z = 85;
        Func_02004dd8(43, 85, 1, 1, x, z);
    }
}

void FieldScene_RunObjectTwentySixPositionCheck(void)
{
    struct Object *obj;
    s32 x;
    s32 z;

    Func_02004e46();
    obj = Func_02004e84(26);
    if ((obj->x >> 20) == 42) {
        x = 41;
        z = 24;
        Func_02004e08(101, 24, 3, 4, x, z);
        Func_02004e46_b(0x859);
    }
    Func_02004e7a();
}

void FieldScene_RunActorNineteenAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004ebe(0) + 6);

    Func_02004e8c();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_02005066(4, 19);
    } else {
        if (Func_02004e76(0x855) == 0) {
            Func_02004f80((s32)&Value_00001280);
        } else {
            Func_02004f88(0x1370);
        }
        Func_02004fa0(19, 0);
    }
    Func_02004ecc();
}

void FieldScene_RunActorTwentyAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004f1e(0) + 6);

    Func_02004eec();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_020050c6(5, 20);
    } else {
        if (Func_02004ed6(0x855) == 0) {
            Func_02004fe0(0x1282);
        } else {
            Func_02004fe8(0x1372);
        }
        Func_02005000(20, 0);
    }
    Func_02004f2c();
}

void FieldScene_RunActorTwentyThreeAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004f7e(0) + 6);

    Func_02004f4c();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_0200512e(1, 23);
    } else {
        if (Func_02004f36(0x855) == 0) {
            Func_02005040(0x128d);
        } else {
            Func_02005048(0x137b);
        }
        Func_02005060(23, 0);
    }
    Func_02004f8c();
}

void SceneState_RunGuardedActorStep(s32 x)
{
    u8 *flag = (u8 *)Func_020050fe() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_020050d6();
    Func_0200516e(x, 1);
    Func_020050dc(2);
    Func_020051cc(x, 0);
    Func_020050f8();
    *flag = zero;
}

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    Func_020049ea(x, 0, 2);
    Func_020051ea(x, 0);
    if (Func_0200514a(0, 0) != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_02005216(x, 0);
}
