#include "types.h"

#define SceneEffect_InitSlotsEightToNineteen Func_02003284
#define SceneEffect_AdvanceSlotByValueBand Func_02003558
#define SceneEffect_SelectSlotValueAndPosition Func_020035ec
union Slot {
    s32 w;
    s16 h[2];
};

extern s32 Data_0200db90[];
extern u16 Data_0200db40[];
extern u16 Data_0200db30[];
extern u32 Data_0200db70[];

u8 *Func_02007596(s32);
u8 *Func_020075a2(s32);
u8 *Func_020075ac(s32);
u8 *Func_020075b6(s32);
void Func_02006634(s32);
void Func_0200663a(s32);
void Func_02006640(s32);
void Func_02006646(s32);
void Func_0200664c(s32);
void Func_02006652(s32);
void Func_02006658(s32);
void Func_0200665e(s32);
s32 *Func_020075f0(s32);
s32 *Func_020075fc(s32);
s32 *Func_02007606(s32);
s32 *Func_02007610(s32);
void Func_0200668e(s32);
void Func_02006694(s32);
void Func_0200669a(s32);
void Func_020066a0(s32);
s32 *Func_02007632(s32);
s32 *Func_0200763c(s32);
s32 *Func_02007644(s32);
s32 *Func_0200764c(s32);
s32 *Func_02007654(s32);
s32 *Func_0200765e(s32);
s32 *Func_02007668(s32);
s32 *Func_02007672(s32);
void Func_02006726(void);
void Func_02007902(s32, s32);
void Func_02007922(s32, s32);
void Func_02007936(s32, s32);
s32 Func_0200790e_a(s32);
s32 Func_0200790e_b(s32);
void Func_02007a40(s32, s32);
void Func_02007a54(s32, s32);
void Func_02007a68(s32, s32);
void Func_02007a7c(s32, s32);
s32 Func_0200791c(s32);
s32 Func_02007940(s32);
s32 Func_02007948(s32);

void SceneEffect_InitSlotsEightToNineteen(void)
{
    {
        s32 z = 0;
        *(Func_02007596(8) + 89) = z;
        *(Func_020075a2(9) + 89) = z;
        *(Func_020075ac(10) + 89) = z;
        *(Func_020075b6(11) + 89) = z;
    }
    Func_02006634(8);
    Func_0200663a(9);
    Func_02006640(10);
    Func_02006646(11);
    Func_0200664c(12);
    Func_02006652(13);
    Func_02006658(14);
    Func_0200665e(15);
    {
        Data_0200db90[0] = Func_020075f0(12)[4];
        Data_0200db90[1] = Func_020075fc(13)[4];
        Data_0200db90[2] = Func_02007606(14)[4];
        Data_0200db90[3] = Func_02007610(15)[4];
        Func_0200668e(16);
        Func_02006694(17);
        Func_0200669a(18);
        Func_020066a0(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007632(16)[6] = v;
            Func_0200763c(17)[6] = v;
            Func_02007644(18)[6] = v;
            Func_0200764c(19)[6] = v;
        }
        Data_0200db90[4] = Func_02007654(16)[4];
        Data_0200db90[5] = Func_0200765e(17)[4];
        Data_0200db90[6] = Func_02007668(18)[4];
        Data_0200db90[7] = Func_02007672(19)[4];
    }
    Func_02006726();
}

void SceneEffect_AdvanceSlotByValueBand(s32 a, s32 i)
{
    u16 v = Data_0200db40[i];

    if (v >= 0x6801 && v <= 0x6FFF) {
        Data_0200db30[i] += 0x70;
        Func_02007902(a, 3);
    } else if (v >= 0xE801 && v <= 0xEFFF) {
        Data_0200db30[i] += 0xE0;
        Func_02007902(a, 3);
    } else if (v >= 0x7001 && v <= 0xEFFF) {
        Data_0200db30[i] += 0x1C0;
        Func_02007922(a, 2);
    } else {
        Data_0200db30[i] += 0x300;
        Func_02007936(a, 1);
    }
}

void SceneEffect_SelectSlotValueAndPosition(s32 a, s32 b, s32 c)
{
    s32 *p = (s32 *)Func_0200790e_a(a);
    union Slot *q = (union Slot *)p[20];
    s32 t;

    if ((c & 2) == 0) {
        switch (Data_0200db70[b]) {
        case 1:
            Data_0200db40[b] = Data_0200db30[0];
            Func_02007a40(a, 8);
            break;
        case 2:
            Data_0200db40[b] = Data_0200db30[1];
            Func_02007a54(a, 9);
            break;
        case 3:
            Data_0200db40[b] = Data_0200db30[2];
            Func_02007a68(a, 10);
            break;
        case 4:
            Data_0200db40[b] = Data_0200db30[3];
            Func_02007a7c(a, 11);
            break;
        }
    }
    if ((c & 1) != 0) {
        t = Func_0200790e_b(Data_0200db40[b]);
        q[7].h[1] = Func_0200791c(Data_0200db40[b] + 0x8000) >> 5;
        p[4] = Data_0200db90[b] - (t << 2) - (t << 1);
    } else {
        t = Func_02007940(Data_0200db40[b] + 0x8000);
        q[7].h[1] = Func_02007948(Data_0200db40[b]) >> 5;
        p[4] = Data_0200db90[b] + (t << 2) + (t << 1);
    }
}
