#include "types.h"

#define Scene_InitializeActorGroups Func_0200259c

struct SceneWork {
    u8 unknown_000[448];
    s32 request;
};

struct SceneState {
    u8 unknown_000[448];
    s16 scene;
    s16 phase;
    u8 unknown_1c4[124];
    s16 nextScene;
    s16 transition;
};

extern struct SceneWork *Data_03001ebc;
extern struct SceneState Data_02000240;
void Func_0200302c();
void Func_0200308c();
void Func_02003094();
void Func_0200309c();
void Func_020030ac();
s32 Func_020030d4();
void Func_020030dc();
void Func_020030e4();
void Func_020030ec();
u8 *Func_0200310c();
void Func_0200313c();
void Func_02003144();
void Func_0200315c();
void Func_0200316c();
void Func_0200319c();
void Func_020023a0();
void Func_02002410();
void Func_02002480();
void Func_0200252c();
void Func_02001abc();
void Func_02001fdc();
void Func_02000ec8();
void Func_02001a98();
void Func_02000e20();

static __inline__ void Call2(void (*f)(), s32 a, s32 b) { f(a, b); }
static __inline__ void Call3(void (*f)(), s32 a, s32 b, s32 c) { f(a, b, c); }
static __inline__ void Call6(void (*f)(), s32 a, s32 b, s32 c, s32 d, s32 e, s32 g)
{ f(a, b, c, d, e, g); }

s32 Scene_InitializeActorGroups(void)
{
    u8 *actor;
    s32 x;
    s32 zero;
    s32 mode;
    s16 scene = 0xac;
    s16 alternateScene = 0xad;
    s32 actorControl;
    s32 tileX;
    s32 tileY;
    s32 tileRow;

    Data_03001ebc->request = 0x204;
    if (Data_02000240.scene == scene || Data_02000240.scene == alternateScene) {
        Func_0200319c(0);
        Data_02000240.transition = 1;
        Data_02000240.nextScene = scene;
    }
    if (Data_02000240.scene == scene) {
        switch (Data_02000240.phase) {
        case 1: case 2:
            if (Func_020030d4(0x982)) {
                Call6(Func_0200308c, 121, 4, 74, 9, 5, 8);
                Call6(Func_0200308c, 18, 83, 9, 73, 3, 2);
                Call6(Func_0200308c, 18, 81, 9, 75, 3, 2);
                Call6(Func_0200308c, 18, 83, 9, 77, 3, 2);
                tileX = 83;
                tileY = 9;
                tileRow = 79;
            } else {
                if (!Func_020030d4(0x983)) break;
                Call6(Func_0200308c, 121, 13, 74, 9, 5, 8);
                Call6(Func_0200308c, 18, 85, 11, 74, 3, 2);
                Call6(Func_0200308c, 18, 83, 13, 75, 3, 2);
                tileX = 85;
                tileY = 11;
                tileRow = 76;
            }
            Call6(Func_0200308c, 18, tileX, tileY, tileRow, 3, 2);
            Call6(Func_0200308c, 18, 83, 11, 78, 3, 2);
            Call6(Func_0200308c, 18, 83, 13, 79, 3, 2);
            break;
        case 3: case 4:
            Func_020023a0();
            Func_0200310c(8)[85] = 0;
            Func_0200310c(9)[85] = 0;
            actor = Func_0200310c(8); Func_020030ac(actor, 0);
            actor = Func_0200310c(9); Func_020030ac(actor, 0);
            *(s32 *)(Func_0200310c(8) + 108) = (s32)Func_02000ec8;
            *(s32 *)(Func_0200310c(9) + 108) = (s32)Func_02000ec8;
            Call2(Func_0200302c, (s32)Func_02000e20, 3200);
            break;
        case 5: case 6: case 7:
            if (Func_020030d4(0x982))
                Call6(Func_02003094, 23, 17, 1, 2, 30, 8);
            if (Func_020030d4(0x983))
                Call6(Func_02003094, 23, 17, 1, 2, 32, 10);
            break;
        case 8: case 9:
            Func_02002410();
            Func_0200310c(10)[85] = 0;
            Func_0200310c(11)[85] = 0;
            actor = Func_0200310c(10); Func_020030ac(actor, 0);
            actor = Func_0200310c(11); Func_020030ac(actor, 0);
            *(s32 *)(Func_0200310c(10) + 108) = (s32)Func_02000ec8;
            *(s32 *)(Func_0200310c(11) + 108) = (s32)Func_02000ec8;
            Call2(Func_0200302c, (s32)Func_02000e20, 3200);
            break;
        case 10: case 11:
            actor = Func_0200310c(18); Func_020030ac(actor, 0);
            actor = Func_0200310c(19); Func_020030ac(actor, 0);
            Func_02003144(18, 2);
            actor = Func_0200310c(20); Func_020030ac(actor, 0);
            actor = Func_0200310c(21); Func_020030ac(actor, 0);
            Func_0200315c(20, 15);
            Func_0200315c(21, 15);
            if (Func_020030d4(0x971)) {
                Call6(Func_0200308c, 59, 8, 49, 8, 1, 3);
                Call6(Func_0200309c, 51, 8, 1, 1, 49, 8);
                Func_0200310c(18)[35] |= 2;
                Func_02003144(18, 3);
                Call6(Func_0200309c, 45, 4, 1, 1, 46, 8);
                Call3(Func_0200313c, 18, 0x2e80000, 0x880000);
                *(s32 *)(Func_0200310c(18) + 12) = -0x100000;
                Call3(Func_0200313c, 20, 0x2e80000, 0x880000);
            }
            if (Func_020030d4(0x200)) {
                Func_0200315c(20, 0);
                Func_02003144(20, 5);
            }
            if (Func_020030d4(0x202)) Func_02003144(19, 2);
            if (Func_020030d4(0x972)) {
                Call6(Func_0200308c, 59, 8, 45, 14, 1, 3);
                Call6(Func_0200309c, 51, 8, 1, 1, 45, 14);
                Func_0200310c(19)[35] |= 2;
                Func_02003144(19, 3);
                Call6(Func_0200309c, 45, 4, 1, 1, 48, 14);
                Call3(Func_0200313c, 19, 0x3080000, 0xe80000);
                *(s32 *)(Func_0200310c(19) + 12) = -0x100000;
                Call3(Func_0200313c, 21, 0x3080000, 0xe80000);
                Func_020030dc(0x202);
            }
            if (Func_020030d4(0x201)) {
                Func_0200315c(21, 0);
                Func_02003144(21, 5);
            }
            break;
        case 12: case 13:
            Func_02002480();
            Func_0200310c(12)[85] = 0;
            Func_0200310c(13)[85] = 0;
            actor = Func_0200310c(15); Func_020030ac(actor, 0);
            actor = Func_0200310c(16); Func_020030ac(actor, 0);
            actor = Func_0200310c(17); Func_020030ac(actor, 0);
            actor = Func_0200310c(12); Func_020030ac(actor, 0);
            actor = Func_0200310c(13); Func_020030ac(actor, 0);
            actor = Func_0200310c(14); Func_020030ac(actor, 0);
            *(s32 *)(Func_0200310c(12) + 108) = (s32)Func_02000ec8;
            *(s32 *)(Func_0200310c(13) + 108) = (s32)Func_02000ec8;
            *(s32 *)(Func_0200310c(14) + 108) = (s32)Func_02000ec8;
            Call2(Func_0200302c, (s32)Func_02000e20, 3200);
            break;
        }
    } else {
        switch (Data_02000240.phase) {
        case 0: break;
        case 1: case 2: case 3:
            Data_02000240.transition = 1;
            Data_02000240.nextScene = 0xb0;
            Func_020030e4(0x12f);
            Func_0200315c(17, 6);
            Func_0200315c(18, 6);
            if (Func_020030d4(0x974)) Call3(Func_0200313c, 17, 0x2d80000, 0x1380000);
            if (Func_020030d4(0x975)) Call3(Func_0200313c, 18, 0x2e80000, 0x1380000);
            Func_0200252c();
            break;
        case 6: case 7:
            Func_0200316c(8, 1);
            Func_0200310c(8)[85] = 0;
            actor = Func_0200310c(8); Func_020030ac(actor, 0);
            Func_0200316c(9, 1);
            Func_0200315c(9, 15);
            actor = Func_0200310c(9); Func_020030ac(actor, 0);
            Func_0200310c(9)[85] = 0;
            if (Func_020030d4(0x204)) {
                Func_0200315c(9, 0);
                Func_02003144(9, 5);
                x = *(s32 *)(Func_0200310c(9) + 8);
                actor = Func_0200310c(9);
                x >>= 20;
                Call6(Func_0200309c, 26, 8, 1, 1, x, *(s32 *)(actor + 16) >> 20);
                *(s32 *)(Func_0200310c(9) + 108) = (s32)Func_02000ec8;
                *(s32 *)(Func_0200310c(8) + 108) = (s32)Func_02000ec8;
            }
            break;
        case 4: case 5:
            zero = Func_020030d4(0x109);
            if (zero == 0) {
                Func_0200310c(10)[85] = zero;
                Func_0200310c(11)[85] = zero;
                *(s32 *)(Func_0200310c(10) + 12) = -0x100000;
                *(s32 *)(Func_0200310c(11) + 12) = -0x100000;
                mode = 2;
                Func_0200310c(10)[35] |= mode;
                Func_0200310c(11)[35] |= mode;
                Func_0200310c(10)[89] &= 254;
                Func_0200310c(11)[89] &= 254;
                actorControl = 3;
                *(u16 *)(Func_0200310c(10) + 100) = actorControl;
                *(u16 *)(Func_0200310c(11) + 100) = actorControl;
                Func_0200316c(10, 1);
                Func_0200316c(11, 1);
                Func_0200310c(12)[85] = 0;
                Func_0200310c(13)[85] = 0;
                Func_0200310c(14)[85] = 0;
                actor = Func_0200310c(12); Func_020030ac(actor, 0);
                actor = Func_0200310c(13); Func_020030ac(actor, 0);
                actor = Func_0200310c(14); Func_020030ac(actor, 0);
                *(u16 *)(Func_0200310c(12) + 100) = zero;
                *(u16 *)(Func_0200310c(13) + 100) = zero;
                *(u16 *)(Func_0200310c(14) + 100) = zero;
                if (Data_02000240.phase != 5) break;
                *(s32 *)(Func_0200310c(10) + 12) = -0x200000;
                *(s32 *)(Func_0200310c(11) + 12) = -0x400000;
                actorControl = 2;
                *(u16 *)(Func_0200310c(10) + 100) = actorControl;
                actorControl = 4;
                *(u16 *)(Func_0200310c(11) + 100) = actorControl;
                Call3(Func_0200313c, 12, 0xc80000, 0x980000);
                actorControl = 11;
                *(u16 *)(Func_0200310c(12) + 100) = actorControl;
                *(s32 *)(Func_0200310c(12) + 108) = (s32)Func_02001a98;
                Func_0200310c(12)[35] |= mode;
                Call3(Func_0200313c, 13, 0xc80000, 0x980000);
                actorControl = 12;
                *(u16 *)(Func_0200310c(13) + 100) = actorControl;
                *(s32 *)(Func_0200310c(13) + 108) = (s32)Func_02001a98;
                Func_0200310c(13)[35] |= mode;
                Call3(Func_0200313c, 14, 0x880000, 0x980000);
                actorControl = 10;
                *(u16 *)(Func_0200310c(14) + 100) = actorControl;
                *(s32 *)(Func_0200310c(14) + 108) = (s32)Func_02001a98;
                Func_0200310c(14)[35] |= mode;
                Func_020030ec(2);
                Func_020030dc(0x200);
                Func_020030dc(0x201);
                Func_020030dc(0x202);
            }
            Func_02001abc(0);
            break;
        case 8: case 9: case 10: case 11:
            if (Func_020030d4(0x982)) Call6(Func_02003094, 10, 30, 1, 2, 16, 30);
            if (Func_020030d4(0x983)) Call6(Func_02003094, 10, 30, 1, 2, 22, 30);
            Func_020030dc(0x973);
            break;
        case 12:
            Call6(Func_0200309c, 8, 49, 1, 1, 8, 113);
            Func_02001fdc();
            Call2(Func_0200302c, (s32)Func_02000e20, 3200);
            break;
        case 13: case 14:
            Func_020030ec(1);
            if (Func_020030d4(0x984)) {
                Call6(Func_02003094, 24, 59, 1, 2, 32, 46);
                Call3(Func_0200313c, 19, 0x1980000, 0x3180000);
                Call3(Func_0200313c, 20, 0x1780000, 0x3180000);
                Call3(Func_0200313c, 21, 0x1980000, 0x2f80000);
                Call3(Func_0200313c, 22, 0x1780000, 0x2f80000);
                Call3(Func_0200313c, 23, 0x1880000, 0x3080000);
            }
            Func_0200310c(19)[85] &= 254;
            Func_0200310c(20)[85] &= 254;
            Func_0200310c(21)[85] &= 254;
            Func_0200310c(22)[85] &= 254;
            Func_0200310c(23)[85] &= 254;
            Func_0200315c(19, 4);
            Func_0200315c(20, 1);
            Func_0200315c(21, 4);
            Func_0200315c(22, 10);
            Func_0200315c(23, 0);
            Func_02003144(19, 2);
            Func_02003144(23, 2);
            x = *(s32 *)(Func_0200310c(19) + 8);
            actor = Func_0200310c(19);
            x >>= 20;
            Call6(Func_0200309c, 20, 56, 1, 1, x, *(s32 *)(actor + 16) >> 20);
            x = *(s32 *)(Func_0200310c(20) + 8);
            actor = Func_0200310c(20);
            x >>= 20;
            Call6(Func_0200309c, 20, 56, 1, 1, x, *(s32 *)(actor + 16) >> 20);
            x = *(s32 *)(Func_0200310c(21) + 8);
            actor = Func_0200310c(21);
            x >>= 20;
            Call6(Func_0200309c, 20, 56, 1, 1, x, *(s32 *)(actor + 16) >> 20);
            x = *(s32 *)(Func_0200310c(22) + 8);
            actor = Func_0200310c(22);
            x >>= 20;
            Call6(Func_0200309c, 20, 56, 1, 1, x, *(s32 *)(actor + 16) >> 20);
            x = *(s32 *)(Func_0200310c(23) + 8);
            actor = Func_0200310c(23);
            x >>= 20;
            Call6(Func_0200309c, 20, 56, 1, 1, x, *(s32 *)(actor + 16) >> 20);
            break;
        case 17:
            Call6(Func_0200309c, 49, 43, 1, 1, 49, 107);
            Func_02001fdc();
            break;
        }
    }
    return 0;
}
