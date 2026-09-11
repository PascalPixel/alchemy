#include "types.h"

extern u8 Value_000027ba[];

extern u8 Value_000000bb[];

#define Scene_RunActorEntrySequence Func_020012c8

/* Complete scene owner 020012c8..020020db. The object effect loop runs
 * forty frames; flag-dependent branches preserve the shared step increments.
 * Calls bind to original-site-resolved loader runtime addresses. */
extern u8 Data_02000240[];
extern const s32 SceneAction_EntryGroup[];
extern const s32 SceneAction_EntryPair[];
void Func_02002350(void);
void Func_02005d34();
u8 *Func_02005e4c();
void Func_02005e34();
void Func_02005e3c();
void Func_02005d5c();
void Func_02005d9c();
void Func_020008a8();
void Func_02005ddc();
void Func_02005df4();
void Func_02000894();
u8 *Func_02005d4c();
void Func_02005dac();
void Func_02005d94();
void Func_02005db4();
void Func_02005e14();
void Func_02005e24();
void Func_02005dd4();
void Func_02005e0c();
void Func_02005dbc();
void Func_02005e2c();
void Func_02005d2c();
void Func_02005bfc();
void Func_02005dcc();
void Func_02002334();
void Func_020020dc();
s32 Func_02005c04();
s32 Func_02005c0c();
void Func_02005de4();
void Func_02002344();
s32 Func_02005dfc();
s32 Func_02005d44();
void Func_02005cdc();
void Func_02005ecc();
s32 Func_02005cfc();
s32 Func_02005d14();
s32 Func_02005d04();
void Func_02005d64();
void Func_02005d7c();
void Func_02005e6c(u16 first, u16 second);
void Func_02005e74(u16 first, u16 second);
void Func_02005e64();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_RunActorEntrySequence(void)
{
    s32 itemOwner;
    u8 *object;
    s32 hidden;
    s32 disableMask;
    s32 advanceStep;
    s32 facing;
    s32 finalMask;
    s32 actor20Key;
    s32 frame;
    s32 effectCallback;
    s32 enableMask;
    s32 actor20LateKey;
    s32 message;
    const s32 *groupActions;
    const s32 *pairActions;
    s32 sharedData;

    Func_02005d34();
    hidden = 0;
    *(u8 *)(Func_02005e4c() + 85) = hidden;
    Call2(Func_02005e34, 0xcccc, 0x1999);
    Call4(Func_02005e3c, 0x14c0000, 0x200000, 0xb40000, 1);
    Call3(Func_02005d5c, 0, 0x10000, 0x8000);
    Call3(Func_02005d9c, 0, 0x154, 184);
    Func_020008a8(0, 0x8000);
    Func_02005ddc(21, 1);
    Call1(Func_02005df4, 0x278e);
    Call1(Func_02000894, 0x9015);
    *(u8 *)(Func_02005e4c() + 85) = hidden;
    Call2(Func_02005e34, 0xcccc, 0x1999);
    Call4(Func_02005e3c, 0x1300000, 0x200000, 0xb40000, 1);
    Call3(Func_02005d5c, 1, 0x10000, 0x8000);
    Call3(Func_02005d5c, 2, 0x10000, 0x8000);
    Call3(Func_02005d5c, 3, 0x16666, 0xb333);
    object = Func_02005d4c(0);
    if (object != 0) {
        Func_02005dac(1, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Func_02005d4c(0);
    if (object != 0) {
        Func_02005dac(2, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Func_02005d4c(0);
    if (object != 0) {
        Func_02005dac(3, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    Call3(Func_02005d94, 1, 0x148, 168);
    Call3(Func_02005d94, 2, 0x154, 196);
    Call3(Func_02005d9c, 3, 0x146, 204);
    Func_02005db4(1, 1);
    Func_02005db4(2, 1);
    Func_02005e14(1, 0x8000, 0);
    Func_02005e14(2, 0x8000, 0);
    Func_020008a8(3, 0x8000);
    Func_02005e14(20, 0, 0);
    Func_02005e14(19, 0, 40);
    Func_02005ddc(20, 2);
    Func_02000894(20);
    Func_020008a8(19, 0x8000);
    Func_02000894(0x2013);
    Call3(Func_02005e24, 21, 0x103, 20);
    Func_02000894(21);
    Func_02005dd4(21, 2);
    Func_02005e0c(21, 0, 20);
    Call3(Func_02005e14, 21, 0xd000, 40);
    Func_02000894(21);
    Func_02005dbc(21, 4);
    Func_020008a8(21, 0);
    Func_02005e0c(21, 0, 20);
    actor20Key = 0x2014;
    Func_02005ddc(20, 1);
    Func_020008a8(20, 0x8000);
    Func_02000894(actor20Key);
    Call2(Func_02005e2c, 3, 0x102);
    Func_02005dd4(3, 2);
    Func_02000894(3);
    Func_02005ddc(2, 1);
    Func_02000894(2);
    Func_020008a8(19, 0);
    Func_02000894(19);
    Call3(Func_02005e24, 20, 0x106, 40);
    Func_02005e14(20, 0, 20);
    Func_02000894(20);
    Call3(Func_02005e14, 0, 0xa000, 0);
    Call3(Func_02005e14, 1, 0x2000, 0);
    Call3(Func_02005e14, 2, 0x6000, 0);
    Call3(Func_02005e14, 3, 0xe000, 0);
    Func_02005e14(21, 0x8000, 0);
    Func_020008a8(19, 0x3000);
    Call3(Func_02005e24, 19, 0x102, 40);
    Func_02000894(19);
    Func_020008a8(20, 0xb000);
    Func_02000894(actor20Key);
    Func_02005e14(20, 0x8000, 20);
    Func_02000894(actor20Key);
    Func_02005e14(19, 0x8000, 0);
    Func_02005e14(0, 0x8000, 0);
    Func_02005e14(1, 0x8000, 0);
    Func_02005e14(2, 0x8000, 0);
    Func_02005e14(3, 0x8000, 0);
    Func_02005e14(21, 0, 20);
    Call3(Func_02005e24, 6, 0x101, 40);
    Func_02000894(6);
    Call3(Func_02005e24, 20, 0x103, 20);
    Func_02000894(actor20Key);
    Func_02005ddc(6, 2);
    Func_02005d2c(20);
    Func_02005db4(6, 3);
    Func_02000894(6);
    Func_02005dbc(20, 3);
    Func_02000894(actor20Key);
    Call3(Func_02005d5c, 6, 0xcccc, 0x6666);
    Call3(Func_02005d9c, 6, 0x104, 186);
    Func_02005e14(21, 0x3000, 0);
    Call3(Func_02005d9c, 6, 0x114, 192);
    object = Func_02005d4c(19);
    {
        s32 shown = 0x5000;

        *(u16 *)((s32)object + 6) = shown;
    }
    Func_02005bfc(1);
    Func_02005dd4(19, 2);
    Func_02000894(0x2013);
    Func_02005dcc(6, 2, 20);
    Call3(Func_02005d5c, 6, 0x26666, 0x13333);
    Call3(Func_02005d9c, 6, 0x104, 186);
    Func_02005e14(21, 0x8000, 0);
    Func_02005d9c(6, 248, 172);
    Func_02005e14(19, 0x8000, 0);
    Func_02005e14(21, 0x8000, 0);
    Func_02005e14(6, 0, 20);
    Func_02005dbc(6, 3);
    Func_02005d2c(40);
    Func_02002334();
    Func_02005bfc(1);
    frame = 0;
    do {
        object = Func_02005d4c(6);
        Func_020020dc((s32)object);
        frame = (frame + 1);
        Func_02005bfc(1);
    } while ((u32)frame <= 39);
    effectCallback = (s32)Func_02002350;
    Value2(Func_02005c04, effectCallback, 0xc80);
    Func_02005d2c(80);
    Call3(Func_02005e14, 0, 0xa000, 0);
    Call3(Func_02005e14, 1, 0x2000, 0);
    Call3(Func_02005e14, 2, 0x6000, 0);
    Call3(Func_02005e14, 3, 0xe000, 20);
    Func_02005e14(21, 0, 40);
    Call3(Func_02005e14, 0, 0x8000, 0);
    Call3(Func_02005e14, 1, 0x8000, 0);
    Call3(Func_02005e14, 2, 0x8000, 0);
    Call3(Func_02005e14, 3, 0x8000, 0);
    Func_020008a8(21, 0x8000);
    Call3(Func_02005e24, 20, 0x101, 40);
    Call3(Func_02005e0c, 0x2014, 0, 20);
    Call3(Func_02005e24, 6, 0x105, 80);
    Func_02005dd4(19, 2);
    Func_02000894(0x2013);
    Func_02005c0c(effectCallback);
    Func_02005bfc(1);
    Func_02005de4(6, 0);
    Func_02005bfc(10);
    Func_02002344();
    Func_02005dcc(6, 2, 40);
    Func_02000894(6);
    Call3(Func_02005e24, 20, 0x103, 20);
    Func_02000894(0x2014);
    disableMask = 254;
    Func_02005ddc(6, 2);
    *(u8 *)(Func_02005d4c(6) + 90) &= disableMask;
    Func_02005d9c(6, 250, 176);
    enableMask = 1;
    Func_02005d2c(1);
    {
        u8 *actor = Func_02005d4c(6);
        s32 flags = actor[90];
        flags |= enableMask;
        actor[90] = flags;
    }
    Call3(Func_02005e24, 21, 0x103, 20);
    Func_02005e14(21, 0, 20);
    Func_02000894(21);
    Func_02005dbc(19, 4);
    Func_02000894(0x2013);
    Func_02005ddc(6, 2);
    Func_02005d2c(40);
    Call3(Func_02005d5c, 6, 0x9999, 0x4ccc);
    *(u8 *)(Func_02005d4c(6) + 90) &= disableMask;
    Func_02005d9c(6, 248, 172);
    Func_02005d2c(1);
    {
        u8 *object = Func_02005d4c(6);
        enableMask |= object[90];
        object[90] = enableMask;
    }
    Func_02005d2c(20);
    Func_02005dbc(6, 3);
    Func_02005d2c(20);
    Func_02005dbc(19, 3);
    Func_02000894(0x2013);
    Func_02005dbc(6, 3);
    Func_02000894(6);
    Call3(Func_02005e14, 19, 0x3000, 0);
    Call3(Func_02005e14, 20, 0xb000, 20);
    Call3(Func_02005e24, 19, 0x105, 0);
    Call3(Func_02005e24, 20, 0x105, 60);
    Func_02005db4(20, 4);
    Func_02000894(0x2014);
    Call3(Func_02005e24, 19, 0x101, 40);
    Func_02000894(19);
    Call3(Func_02005e24, 20, 0x105, 100);
    Func_02005ddc(20, 1);
    Func_02005d2c(20);
    Func_020008a8(20, 0);
    Func_020008a8(19, 0);
    Value2(Func_02005dfc, 20, 0);
    Call3(Func_02005e14, 1, 0x2000, 0);
    Call3(Func_02005e14, 2, 0xc000, 0);
    Call3(Func_02005e14, 3, 0xe000, 0);
    if (Value2(Func_02005d44, 0, 0) == 0) {
        Func_02005d2c(20);
        Func_02005dbc(20, 3);
        advanceStep = 1;
    } else {
        Func_02005d2c(20);
        Func_02005dbc(20, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        advanceStep = 0;
    }
    Func_02000894(20);
    if (advanceStep != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02005e14, 1, 0x8000, 0);
    Call3(Func_02005e14, 2, 0x8000, 0);
    Call3(Func_02005e14, 3, 0x8000, 20);
    Call3(Func_02005e24, 20, 0x108, 40);
    Value2(Func_02005dfc, 20, 0);
    Call3(Func_02005e14, 1, 0x2000, 0);
    Call3(Func_02005e14, 2, 0xc000, 0);
    Call3(Func_02005e14, 3, 0xe000, 0);
    if (Value2(Func_02005d44, 0, 0) == 0) {
        Func_02005d2c(20);
        Func_02005db4(2, 3);
        advanceStep = 1;
    } else {
        Func_02005d2c(20);
        Func_02005db4(2, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        advanceStep = 0;
    }
    Func_02000894(2);
    if (advanceStep != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02005ddc(1, 2);
    Func_020008a8(1, 0x4000);
    Func_02000894(1);
    Func_02005ddc(19, 1);
    Func_02000894(19);
    Call3(Func_02005e14, 1, 0x8000, 0);
    Call3(Func_02005e14, 2, 0x8000, 0);
    Func_020008a8(3, 0x8000);
    Func_02005dbc(1, 4);
    Func_02005dbc(20, 3);
    Func_02005e0c(20, 0, 20);
    Func_02005db4(0, 3);
    Func_02005db4(1, 3);
    Func_02005db4(2, 3);
    Func_02005dbc(3, 3);
    Func_020008a8(19, 0x3000);
    Func_02005db4(19, 3);
    Func_02000894(19);
    Call3(Func_02005e14, 20, 0xb000, 20);
    Func_02005dbc(20, 3);
    actor20LateKey = 0x2014;
    Func_02005d2c(40);
    Func_020008a8(20, 0x8000);
    Func_02000894(actor20LateKey);
    Func_02005ddc(21, 2);
    Func_02005d2c(20);
    Func_02005e0c(21, 0, 40);
    Func_02000894(actor20LateKey);
    Call3(Func_02005e24, 21, 0x103, 60);
    Func_020008a8(19, 0x8000);
    Func_02005ddc(19, 1);
    Call1(Func_02000894, 0x2013);
    Call3(Func_02005e24, 21, 0x105, 60);
    Func_02005dbc(21, 3);
    Func_02005d2c(20);
    Call3(Func_02005d5c, 21, 0xcccc, 0x6666);
    Call3(Func_02005d9c, 21, 0x120, 192);
    Func_02005e14(19, 0, 0);
    Func_02005e14(20, 0, 0);
    Call3(Func_02005d9c, 21, 0x136, 192);
    Call3(Func_02005d9c, 21, 0x148, 186);
    Func_02005d2c(20);
    Func_02005ddc(21, 2);
    /*
     * Spelled as the address of a Value_ symbol rather than the integer
     * 0x27ba. A CONST_INT that fits an immediate is materialised with mov,
     * and one that does not is still free to be hoisted and shared; a
     * SYMBOL_REF has to come from the literal pool and stays where it is
     * written. The reference loads this constant from the pool after the two
     * preceding calls, which the integer spelling does not reproduce.
     */
    message = (s32)Value_000027ba;
    Call2(Func_02005cdc, message, 1);
    Call3(Func_02005d9c, 21, 0x136, 192);
    Func_02005e14(19, 0x8000, 0);
    Func_02005e14(20, 0x8000, 0);
    Call3(Func_02005d9c, 21, 0x120, 192);
    Call3(Func_02005d9c, 21, 0x106, 176);
    Func_02005e14(21, 0, 40);
    Func_02005df4((message + 1));
    Func_02000894(21);
    Func_02005dbc(20, 3);
    Func_02000894(actor20LateKey);
    Func_02005dbc(21, 3);
    Func_020008a8(20, 0);
    Func_020008a8(21, 0x8000);
    Func_02005dbc(21, 3);
    Func_02005dbc(6, 3);
    Call3(Func_02005d5c, 6, 0xcccc, 0x6666);
    Call3(Func_02005d9c, 6, 0x104, 186);
    Func_02005e14(21, 0x3000, 0);
    Call3(Func_02005d9c, 6, 0x114, 192);
    Func_02005ecc(19);
    facing = 160;
    object = Func_02005d4c(19);
    *(u16 *)((s32)object + 6) = (facing << 7);
    Func_02005bfc(1);
    Func_02005ddc(19, 1);
    Func_02000894(19);
    Func_02005ddc(6, 2);
    Func_02005e14(21, 0, 0);
    Func_02005e14(20, (facing << 7), 0);
    Call3(Func_02005e14, 6, 0xd000, 20);
    Func_02005dcc(3, 2, 20);
    Func_020008a8(3, 0xa000);
    Func_02000894(3);
    Func_02005e14(21, 0, 0);
    Func_02005e14(6, 0, 0);
    Func_02005e14(19, 0x3000, 0);
    Call3(Func_02005e14, 20, 0xb000, 80);
    Func_02005e14(19, 0, 0);
    Func_02005e14(20, 0, 40);
    Func_02005ecc(29);
    Func_02000894(20);
    Func_02005e14(21, 0x3000, 0);
    Call3(Func_02005e14, 6, 0xb000, 20);
    Func_02005dd4(2, 2);
    Func_02000894(2);
    Func_020008a8(19, 0x3000);
    Func_02005dbc(19, 3);
    Func_02000894(19);
    Func_02005e14(21, 0, 0);
    Func_020008a8(6, 0xd000);
    Func_02005db4(3, 3);
    Func_02000894(3);
    Call3(Func_02005e24, 20, 0x100, 20);
    Func_02005e14(20, 0x3000, 20);
    Func_02000894(20);
    Call3(Func_02005e24, 6, 0x102, 0);
    Call3(Func_02005e24, 21, 0x102, 60);
    Func_02005e14(6, 0, 0);
    Func_020008a8(21, 0);
    Call3(Func_02005e24, 1, 0x103, 40);
    Func_02005dd4(1, 2);
    Func_02000894(1);
    Func_020008a8(19, 0);
    Func_02005dbc(19, 4);
    Func_02000894(19);
    Func_020008a8(20, 0);
    Func_02005db4(20, 4);
    Func_02000894(20);
    Func_02005e14(6, 0xd000, 0);
    Call3(Func_02005e24, 6, 0x101, 0);
    Call3(Func_02005e24, 3, 0x101, 60);
    Call3(Func_02005e0c, 3, 0, 20);
    Func_020008a8(19, 0x3000);
    Func_02005db4(19, 4);
    Func_02000894(19);
    Call2(Func_02005e2c, 2, 0x102);
    Func_02005e0c(2, 0, 20);
    Func_020008a8(19, 0);
    Func_02005dbc(20, 3);
    Func_02000894(20);
    Func_02005dd4(0, 2);
    Func_02005dd4(1, 2);
    Func_02005dd4(2, 2);
    Func_02005ddc(3, 2);
    Func_02005e14(0, 0xa000, 0);
    Call3(Func_02005e14, 1, 0x2000, 0);
    Call3(Func_02005e14, 2, 0x6000, 0);
    Call3(Func_02005e14, 3, 0xe000, 40);
    Func_02005ddc(3, 1);
    Func_020008a8(3, 0xa000);
    Func_02000894(3);
    Call3(Func_02005e14, 1, 0x8000, 20);
    Call3(Func_02005e24, 1, 0x103, 20);
    Func_02005dd4(1, 2);
    Func_02000894(1);
    Func_02005e14(6, 0, 0);
    Func_02005e14(0, 0x8000, 0);
    Func_02005e14(2, 0x8000, 40);
    Call3(Func_02005e24, 20, 0x103, 40);
    Func_02005dd4(20, 2);
    Func_02000894(20);
    Func_02005ddc(19, 1);
    Func_02000894(19);
    itemOwner = Func_02005cfc(65);
    Func_02005d14(itemOwner + 0x345);
    finalMask = 254;
    Func_02005d04(65);
    *(u8 *)(Func_02005d4c(0) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(1) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(2) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(3) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(19) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(20) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(21) + 90) &= finalMask;
    {
        u8 *actor = Func_02005d4c(6);
        actor += 90;
        groupActions = SceneAction_EntryGroup;
        finalMask &= *actor;
        *actor = finalMask;
    }
    Func_02005d64(0, groupActions);
    Func_02005d64(1, groupActions);
    Func_02005d64(2, groupActions);
    Func_02005d64(3, groupActions);
    pairActions = SceneAction_EntryPair;
    Func_02005d64(19, pairActions);
    Func_02005d64(20, pairActions);
    Func_02005d64(21, groupActions);
    Func_02005d7c(6, groupActions);
    /*
     * RESIDUAL, EIGHT HALFWORDS AT 0x0200207a. Size, opcodes and pool
     * contents all match; three orderings do not. The reference materialises
     * the first argument of the Func_02005d7c call before the second and the
     * second argument of the Func_02005e6c call before the first -- opposite
     * orders for the same two-argument shape -- and it loads the 0xbb pool
     * constant after the store below rather than into the load-latency gap
     * ahead of it. Wrapper clones per call site, inline and out-of-line
     * argument spellings, pointer and direct forms of this store, and moving
     * the base address earlier all leave it unchanged. Later attempts add:
     * routing the Func_02005d7c call through Call2, hoisting the actor
     * constant to function scope, and taking the store through a named u8
     * pointer -- all byte-identical to this file. Moving the base address
     * assignment above the call is worse (24 halfwords, 28 wrong). The owner
     * is size-exact with zero wrong instructions, so the residual is three
     * scheduling ties, not a wrong shape. Not adopted.
     */
    sharedData = (s32)Data_02000240;
    *(u8 *)((sharedData + 0x22b)) = 3;
    {
        s32 actor = (s32)Value_000000bb;

        /*
         * Through Call2 rather than called directly: the wrapper's parameter
         * pseudos fix the order the two argument registers are materialised
         * in, and direct calls here emit them the other way round.
         */
        Call2(Func_02005e6c, actor, 2);
        Call2(Func_02005e74, actor, 9);
    }
    Func_02005e64(98, 1);
    Value1(Func_02005d14, 0x350);
}
