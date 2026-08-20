#include "types.h"

/* DRAFT for Func_080ab5e4: 186 calls, 20 loops, 3 memory operations.
 * Written by reconstruct from the owner's own disassembly.
 * It is a starting point, not a reconstruction: read the assembly and
 * fix it. Score it before believing any of it. */

void Func_080022f4();
void Func_08002304();
s32 Func_080030f8();
void Func_08015018();
extern u8 *Data_03001f2c;                         /* 主ワークへのポインタ */
extern u32 Data_03001c94;                         /* 進行フラグ語 */
extern s16 Data_02000240[];
#define MSG_DJINN_RECOVER_1 0xc4c                  /* 召喚後のジン回復の説明 */
#define MSG_DJINN_RECOVER_2 0xc4d
#define MSG_TUTORIAL_C40 0xc40
#define MSG_TUTORIAL_C41 0xc41
void Func_08015038();
s32 Func_08015048();
void Func_08015068();
void Func_08015078();
void Func_08015080();
void Func_08015090();
void Func_080150b0();
void Func_08015120();
void Func_08015270();
void Func_08015278();
void Func_08015280();
void Func_08077008();
void Func_08077010();
void Func_080771b0();
void Func_080771b8();
void Func_080771c0();
void Func_080771c8();
s32 Func_08077208();
s32 Func_08077210();
void Func_080a1a40();
void Func_080a1ac0();
s32 Func_080aa538();
void Func_080aafb8();
void Func_080ab1f4();
void Func_080ab2ec();
void Func_080ab834();
void Func_080abd26();
void Func_080ac486();
s32 Func_080ad5b4();
void Func_080ad5f4();
void Func_080ad608();
void Func_080ae714();
void Func_080b50f8();
void Func_080f9010();

void Func_080ab5e4(void)
{
    u8 *seq;
    s32 adv;
    s32 base;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;
    s32 i11;
    s32 i12;
    s32 i13;
    s32 i14;
    s32 i15;
    s32 i16;
    s32 i17;
    s32 i18;
    s32 i19;
    s32 i20;

    Func_08002304(372, 10, 1, 50339628);
    Func_080022f4(372, 10);
    for (i1 = 0; i1 >= 0; i1--) {
    }
    Func_080aafb8();
    base = 537;
    if (0) {
    }
    base = 537;
    if (0) {
        Func_080aa538();
    }
    Func_080ae714();
    base = 537;
    if (0) {
    }
    Func_08002304(7, 10, 372);
    Func_080022f4(7, 10);
    Func_080ab1f4(14);
    Func_080ab2ec(7, 6);
    base = 537;
    if (0) {
        Func_08015080(2993);
        base = 537;
    }
    Func_08015270(base);
    Func_08077008();
    base = 297;
    Func_08015270(64);
    Func_08015090();
    Func_08015080();
    Func_08015090();
    Func_080150b0(64);
    Func_08015080(2985, 64, 0, 16);
    Func_08015270();
    Func_08015120();
    base = 2994;
    Func_08015080(base, base);
    Func_08015120();
    Func_08015280(4);
    Func_08015080(base + 1);
    Func_08015080(base + 2);
    if (Func_080ad5b4() != 0) {
        Func_08015080(2968);
        Func_08015080(2969);
        if (Func_08077210(376) == 0) {
            if (Func_08077208(376) != 0) {
            }
            if (Func_08077210(376) != 0) {
                Func_080ad608();
            } else {
                Func_080ad608();
            }
            Func_080ad5b4();
        } else {
            Func_08015080(2974, 268, 0, 16);
            Func_080ad608();
            Func_080ad5b4();
        }
    } else {
        base = 2970;
        Func_08015080(base, base);
        if (Func_08077210() == 0) {
            if (Func_08077208() != 0) {
            }
            if (Func_08077210(base) != 0) {
                Func_08015080(base + 3);
                Func_080ad608(0);
            } else {
                Func_08015080(base + 2);
                Func_080ad608(0);
            }
            Func_080ad5b4();
        } else {
            Func_08015080(base + 4);
            Func_080ad608(0);
            Func_080ad5b4(0);
        }
    }
    Func_080ad5f4();
    if (Func_08077210() == 0) {
        if (Func_08077208() != 0) {
        }
        if (Func_08077210() != 0) {
            Func_080ad608();
        } else {
            Func_080ad608();
        }
        Func_080ad5b4();
    } else {
        Func_080ad608();
        Func_080ad5b4();
    }
    Func_080ad5f4();
    Func_08015278();
    Func_08015080(2989);
    Func_08015068(104);
    Func_08015080();
    Func_080ab1f4(1, 14);
    base = 3747;
    Func_080a1a40();
    Func_080a1a40();
    if (Func_080030f8(1) != 0) {
    }
    /* ディスパッチ。seq+0x212c の状態値 1..28 で分岐し、tick 毎に seq+0x2128 を
       加算する。空欄スロット 8 件は切替後アドレス共有。表は +0x74c。 */
    seq = *(u8 **)((u8 *)Data_03001f2c + 388);
    {
        s32 state;

        state = *(s32 *)(seq + 0x212c);
        if (state != 0) {
            (*(s32 *)(seq + 0x2128))++;
            adv = 0;
            switch (state) {
            case 28:
                if ((Data_03001c94 & 1) == 0) {
                    Func_080a1a40(150, 26);
                    Func_080030f8(1);
                }
                break;
            case 27:
                if (*(s32 *)(seq + 0x2128) != 60)
                    break;
                Func_08015038(MSG_DJINN_RECOVER_1, 9, 9, 1);
                *((u8 *)Data_02000240 + 524) = 1;
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_08015018(0, 1);
                Func_080aafb8(seq);
                Func_080030f8(1);
                Func_08015038(MSG_DJINN_RECOVER_2, 9, 9, 1);
                *((u8 *)Data_02000240 + 524) = 1;
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_08015018(0, 1);
                Func_080aafb8(seq);
                Func_080b50f8();
                Func_080b50f8();
                Func_080b50f8();
                break;
            case 1:
                if (*(s32 *)(seq + 0x2128) != 60)
                    break;
                Func_08015038(MSG_TUTORIAL_C40, 9, 9, 1);
                *((u8 *)Data_02000240 + 524) = 1;
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_08015018(0, 1);
                Func_080aafb8(seq);
                break;
            case 2:
            case 4:
            case 8:
            case 9:
            case 22:
                if (*(s32 *)(seq + 0x2128) != 90)
                    break;
                adv = 1;
                *(s32 *)(seq + 0x2128) = 0;
                *(s32 *)(seq + 0x212c) += 1;
                break;
            case 3:
                if (*(s32 *)(seq + 0x2128) != 90)
                    break;
                break;
            case 6:
            case 7:
                if (*(s32 *)(seq + 0x2128) != 60)
                    break;
                Func_08015038(MSG_TUTORIAL_C41, 9, 9, 1);
                *((u8 *)Data_02000240 + 524) = 1;
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_08015018(0, 1);
                Func_080aafb8(seq);
                break;
            case 13:
                if (*(s32 *)(seq + 0x2128) != 40)
                    break;
                break;
            case 14:
            case 16:
            case 17:
                if (*(s32 *)(seq + 0x2128) != 40)
                    break;
                break;
            case 15:
                if (*(s32 *)(seq + 0x2128) != 60)
                    break;
                Func_08015038(3140, 9, 9, 1);
                Func_080a1ac0(2, 146);
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_080a1a40(2, 146);
                Func_08015018(2, 1);
                Func_080aafb8(seq);
                Func_080030f8(1);
                Func_08015038(3141, 9, 9, 1);
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_080a1a40(2, 146);
                Func_08015018(2, 1);
                Func_080aafb8(seq);
                Func_080030f8(1);
                Func_08015038(3142, 9, 9, 1);
                while (Func_08015048() == 0)
                    Func_080030f8(1);
                Func_080a1a40(2, 146);
                Func_08015018(2, 1);
                Func_080aafb8(seq);
                break;
            case 18:
            case 20:
                if (*(s32 *)(seq + 0x2128) != 90)
                    break;
                break;
            case 21:
                if (*(s32 *)(seq + 0x2128) != 90)
                    break;
                break;
            case 24:
                if (*(s32 *)(seq + 0x2128) != 60)
                    break;
                break;
            }
        }
    }
    if (Func_080030f8(1) != 0) {
    } else {
        if (Func_08077210(2, 32, 2, 0) == 0) {
            if (Func_08077208(2, 32) != 0) {
            }
        }
        Func_080f9010(114);
        Func_08015278();
        Func_08015068(104);
        Func_08015078(3006);
        Func_080ab834(1);
        Func_080f9010(175, 2);
        Func_080771b8();
        Func_080771c8();
        Func_080f9010(139, 2);
        Func_080771b0();
        Func_080771c0();
        Func_08077010(2);
        Func_080aafb8();
        Func_080f9010(112);
    }
    Func_080f9010(114);
    Func_080ab834();
    if (Func_08077210(1, 0) == 0) {
        if (Func_08077208(1) != 0) {
        }
    }
    Func_080f9010(114);
    Func_08015278();
    Func_08015068(104);
    Func_08015078(3006);
    Func_080ab834();
    Func_080f9010(112);
    Func_080f9010(113);
    Func_080f9010(139);
    Func_080f9010(175);
    base = 537;
    Func_080ac486(0);
    if (0) {
        if (0) {
            if (Func_08077210(2, 2) == 0) {
                if (Func_08077208(2) != 0) {
                }
            }
            Func_080771b0();
            Func_080771c0();
            Func_08077010();
        } else {
            Func_080771b8();
            Func_080771c8();
            Func_08077010();
        }
    }
    base = 537;
    Func_080aafb8();
    Func_080ab1f4(1, 15);
    Func_080f9010(112);
    Func_080f9010(111);
    Func_080ab834();
    Func_080ab1f4(1, 15);
    Func_080ab834();
    Func_080ab834(2, 2);
    Func_080aa538();
    Func_080ab834(2);
    Func_080f9010(111);
    Func_080ab834();
    Func_080ab1f4(1, 15);
    if (Func_080aa538() == 0) {
    }
    Func_080ab834(2, 0, 2);
    Func_080f9010(111);
    Func_080ab1f4(1, 15);
    Func_080aa538(6, 6);
    if (0) {
        Func_080aa538();
    }
    Func_080ab834();
    Func_080f9010(111);
    Func_080ab1f4(1, 15);
    Func_080aa538(7, 7);
    if (0) {
        Func_080aa538();
    }
    Func_080aa538();
    Func_080ab834(2);
}
