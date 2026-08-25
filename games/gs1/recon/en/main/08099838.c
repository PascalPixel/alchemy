#include "types.h"

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

void *Func_0808ba1c(void *);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, s32);
void Func_080072f8(void);
void Func_08099738(void);
void Func_08099678(void);

#define RunBattleEffect10 Func_08099838

void RunBattleEffect10(void)
{
    u8 *state = Data_03001ebc;
    u8 *object = Func_0808ba1c(*(void **)(Data_02000240 + 500));
    u8 *record = *(u8 **)(object + 80);
    u8 *entry = *(u8 **)(record + 40);
    u8 *first_flag;
    u32 cycle;

    Func_080f9010(0x82);
    Func_08009080(object, 0);
    *(void **)(object + 108) = 0;
    cycle = 0;
    first_flag = record + 37;
    do {
        entry[5] = 7;
        record[37] = 1;
        record[38] = 2;
        Func_080030f8(2);
        record[37] = 1;
        record[38] = 0;
        Func_080030f8(2);
        cycle++;
    } while (cycle <= 9);
    entry[5] = 0;
    record[38] = 2;
    *first_flag = 1;
    Func_080041d8(Func_08099678, 0xc80);
    *(s16 *)(Data_02000240 + 588) = 1;
    Func_080072f8();
    if (*(s16 *)(state + 382) == 0x2092) {
        Func_08099738();
        *(s16 *)(state + 382) = 0;
    }
}
