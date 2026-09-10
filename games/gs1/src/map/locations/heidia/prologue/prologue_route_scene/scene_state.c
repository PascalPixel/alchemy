#include "types.h"

#define SetEffectOwnerMode Func_020009dc
#define InitializePrologueSceneState Func_02000e88
#define MarkGridLeftOfSceneActor Func_0200185c
#define MarkGridAboveSceneActor Func_020018b0
#define SetSceneActorModes Func_02001904
#define InitializeEscapeSceneActors Func_0200191c
struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

extern u8 Data_02000240[];

void Func_02003efc(void);
void Func_02003f86(s32 arg0, s32 arg1);
void Func_02003f8e(s32 arg0, s32 arg1);
void Func_02003f2a(void);
struct S *Func_020048f4(void);
void Func_0200494a(s32 arg0, s32 arg1);
void Func_020048c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
struct S *Func_02004948(void);
void Func_0200499e(s32 arg0, s32 arg1);
void Func_0200491c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void Func_020049cc(int arg0, int arg1);
void Func_020049d4(int arg0, int arg1);
void Func_02003184(s32, s32, s32, s32);
void Func_02003190(s32, s32, s32, s32);
void Func_020031f0(s32, s32, s32, s32);
void Func_020031fc(s32, s32, s32, s32);
void Func_02003208(s32, s32, s32, s32);
void Func_02003214(s32, s32, s32, s32);
void Func_020031cc(s32, s32, s32, s32);
void Func_020031d8(s32, s32, s32, s32);
void Func_020031e4(s32, s32, s32, s32);
void Func_02003244(s32, s32, s32, s32);
void Func_02003250(s32, s32, s32, s32);
void Func_0200325c(s32, s32, s32, s32);

/* Copy the low two mode bits into the object's owner record. */

void SetEffectOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

void InitializePrologueSceneState(void)
{
    u8 *base;

    Func_02003efc();
    base = Data_02000240;
    Func_02003f86(*(s16 *)(base + 448), 5);
    base[555] = 3;
    Func_02003f8e(84, 5);
    Func_02003f2a();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Func_020048f4();

    if (p != 0) {
        s32 v;

        Func_0200494a(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Func_020048c8(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Func_02004948();

    if (p != 0) {
        s32 v;

        Func_0200499e(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Func_0200491c(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Func_020049cc(actor_id, 1);
    Func_020049d4(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    Func_02003184(8, 0x11, 0x1E, 0x15);
    Func_02003190(0xA, 0x11, 0x1F, 0x16);
    Func_020031f0(0xB, 0x14, 0x1E, 0x17);
    Func_020031fc(0xC, 0x15, 0x1E, 0x18);
    Func_02003208(0xD, 0x16, 0x1E, 0x19);
    Func_02003214(0xF, 0x17, 0x1E, 0x1A);
    Func_020031cc(0x11, 0, 0x1E, 0x1F);
    Func_020031d8(0x12, 0, 0x1F, 0x20);
    Func_020031e4(9, 0, 0x20, 0x21);
    Func_02003244(0x13, 4, 0x1E, 0x22);
    Func_02003250(0xE, 5, 0x1E, 0x23);
    Func_0200325c(0x10, 6, 0x1E, 0x24);
}
