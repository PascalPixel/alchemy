#include "TYPES.H"
#include "EVENT_RUNTIME.H"
extern struct EventRuntime *Data_03001ebc;

#define FieldScene_SelectActorPair Func_02001f50
void Func_0200cd44();
void Func_0200cc7c();
void Func_0200cd24();
void Func_0200cdd4();
void Func_0200ce04();
void Func_0200ce0c();
void Func_0200cd3c();
void Func_0200ce14();
void Func_0200ce44();
void Func_0200cdfc();
void Func_0200c5f4();
void Func_0200c658();
void Func_0200cefc();
void Func_0200ce3c();
void Func_0200c684();
void Func_0200cdf4();
void Func_0200cddc();
u8 *Func_0200cd7c();
void Func_0200cea4();
void Func_0200ceac();
void Func_0200a1c8();
void Func_0200cce4();
void Func_0200cd1c();
void Func_0200cc74();
void Func_0200cd4c();

/* Constant-bearing scene calls use shared inline argument helpers. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

struct SceneObjectFlags {
    u8 unknown_000[9];
    unsigned char unk_low : 2;
    unsigned char mode : 2;
    unsigned char unk_high : 4;
};

void FieldScene_SelectActorPair(void)
{
    u8 *record;
    s32 actor;
    u8 *work;

    work = (u8 *)Data_03001ebc;
    Func_0200cd44();
    Call1(Func_0200cc7c, 0x200c8c9);
    Call1(Func_0200cd24, 0x107);
    Call1(Func_0200cd24, 0x250);
    Func_0200cdd4(24, 1);
    Func_0200cdd4(25, 1);
    Func_0200cdd4(2, 1);
    Func_0200cdd4(0, 1);
    Func_0200cdd4(1, 1);
    Func_0200ce04(1, 2, 0);
    Func_0200ce0c(0, 2, 0);
    Func_0200ce04(24, 2, 0);
    Func_0200ce04(25, 2, 0);
    Func_0200cd3c(10);
    actor = 24;
    switch (*(s16 *)(((s32)work + 0x182))) {
    case 202:
    case 203:
        Call1(Func_0200ce14, 0x12a4);
        Call2(Func_0200ce44, 25, 0x102);
        Func_0200cdfc(25, 2);
        Func_0200c5f4(25, 20);
        if (*(s16 *)(((s32)work + 0x182)) == 202) {
            actor = 25;
            break;
        }
        /* fall through */
    case 201:
        Call1(Func_0200ce14, 0x12a3);
        Call2(Func_0200ce44, 24, 0x102);
        Func_0200cdfc(24, 2);
        actor = 24;
        Func_0200c5f4(24, 20);
        break;
    }
    Func_0200cdd4(2, 1);
    Func_0200ce04(2, actor, 0);
    Func_0200ce04(1, 2, 0);
    Func_0200cdfc(1, 2);
    Func_0200cd3c(20);
    Call1(Func_0200ce14, 0x12a5);
    Func_0200c5f4(1, 20);
    Func_0200cdfc(2, 2);
    Func_0200cd3c(20);
    Func_0200c5f4(2, 20);
    Func_0200c658(2, actor);
    Func_0200cefc();
    Func_0200cd3c(60);
    Func_0200cdfc(24, 2);
    Func_0200c5f4(24, 20);
    Func_0200cdfc(25, 2);
    Func_0200c5f4(25, 20);
    Call3(Func_0200ce3c, 2, 0x100, 60);
    Func_0200c684();
    Func_0200cdf4(1, 2);
    Func_0200cdfc(0, 2);
    Func_0200cd3c(20);
    Func_0200c5f4(1, 20);
    Func_0200ce04(2, 1, 0);
    Func_0200cd3c(20);
    Func_0200cddc(2, 3);
    Func_0200cd3c(40);
    record = Func_0200cd7c(0);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Func_0200cd7c(1);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = Func_0200cd7c(2);

    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    Data_03001ebc->value_1c8 = 24;
    Data_03001ebc->value_1c0 = 0x201;
    Func_0200cea4();
    Func_0200ceac();
    Func_0200a1c8();
    Call6(Func_0200cce4, 14, 45, 3, 1, 14, 44);
    Call1(Func_0200cd1c, 0x853);
    {
        u8 *record = Func_0200cd7c(24);
        s32 shown = 5;

        *(u16 *)((s32)record + 100) = shown;
    }
    {
        u8 *record = Func_0200cd7c(25);
        s32 shown = 4;

        *(u16 *)((s32)record + 100) = shown;
    }
    Call2(Func_0200cc74, 0x200aba1, 0xc80);
    Data_03001ebc->value_1c0 = 0x209;
    Func_0200cd4c();
}
