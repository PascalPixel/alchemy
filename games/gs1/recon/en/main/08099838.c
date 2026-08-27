#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

void *Func_0808ba1c(u32);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, s32);
void Func_080072f8(void);
void Func_08099738(void);
void Func_08099678(void);

#define RunBattleEffect10 Func_08099838

typedef struct {
    u8 pad[37];
    u8 flag_a;
    u8 flag_b;
} Record;

void RunBattleEffect10(void)
{
    u8 *state = Data_03001ebc;
    s32 index = 250;
    u8 *object = Func_0808ba1c(*(u32 *)&Data_02000240[index]);
    Record *record = *(Record **)(object + 80);
    u8 *entry = *(u8 **)((u8 *)record + 40);
    u32 cycle;

    Func_080f9010(0x82);
    Func_08009080(object, 0);
    *(void **)(object + 108) = 0;
    cycle = 0;
    do {
        entry[5] = 7;
        record->flag_a = 1;
        record->flag_b = 2;
        Func_080030f8(2);
        record->flag_a = 1;
        record->flag_b = 0;
        Func_080030f8(2);
        cycle++;
    } while (cycle <= 9);
    cycle = 0;
    entry[5] = cycle;
    record->flag_b = 2;
    record->flag_a = 1;
    Func_080041d8(Func_08099678, 0xc80);
    index = 147;
    *(s16 *)&((s32 *)Data_02000240)[index] = 1;
    Func_080072f8();
    if (*(s16 *)(state + 382) == 0x2092) {
        Func_08099738();
        *(s16 *)(state + 382) = cycle;
    }
}
