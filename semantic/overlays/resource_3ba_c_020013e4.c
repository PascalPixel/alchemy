typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];
extern void Func_02001b5c(s32); extern s32 Func_02001d64(s32,s32); extern void Func_02001e20(s32,s32);
extern void Func_02001e7c(s32,s32,s32); extern void Func_02002d28(s32,s32,s32);
extern void Func_02002d8c(s32,s32,s32); extern void Func_02002de8(void);
extern void Func_0808a018(void); extern void Func_0808a020(void); extern void Func_0808a170(s32);
extern void Func_0808a208(s32,s32); extern void Func_0808a210(s32,s32,s32,s32); extern void Func_0808a218(void);
extern void Func_0808a010(s32); extern void Func_0808a180(s32,s32); extern void Func_080000c0(s32);
extern u8 *Func_0808a080(s32); extern void Func_08009150(u8 *,s32,s32,s32); extern void Func_08009158(u8 *);
extern void Func_0808a200(s32,s32);

void Func_020013e4(s32 actor)
{
    s32 phase;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_02001b5c(actor);
        return;
    }

    Func_0808a018();
    phase = Func_02001d64(actor, 2);
    if (phase == 0) {
        Func_0808a170(0x2090);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x02500000, -1, 0x00780000, 1);
        Func_0808a218();
        Func_0808a010(60);
        Func_0808a208(0x18000, 0x3000);
        Func_0808a210(0x02600000, -1, 0x00d80000, 1);
        Func_0808a218();
        Func_0808a180(actor, 0);
        Func_02002d28(56, 64, 0);
        Func_0808a010(60);
        Func_02002d8c(160, 96, 10);
        Func_0808a010(70);
        Func_0808a180(actor, 0);
        Func_02002de8();
        Func_080000c0(2);

        record = Func_0808a080(13);
        record[85] = 0;
        *(s32 *)(record + 48) = 0xcccc;
        *(s32 *)(record + 52) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x80000, *(s32 *)(record + 16));

        record = Func_0808a080(14);
        record[85] = 0;
        *(s32 *)(record + 48) = 0xcccc;
        *(s32 *)(record + 52) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x200000, *(s32 *)(record + 16));
        Func_08009158(record);
        Func_0808a010(45);

        record = Func_0808a080(13);
        record[85] = 0;
        *(s32 *)(record + 48) = 0xcccc;
        *(s32 *)(record + 52) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x180000, *(s32 *)(record + 16));

        record = Func_0808a080(14);
        record[85] = 0;
        *(s32 *)(record + 48) = 0xcccc;
        *(s32 *)(record + 52) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0, *(s32 *)(record + 16));
        Func_08009158(record);
        Func_0808a010(15);
        Func_0808a180(actor, 0);
        Func_02002d28(56, 64, 0);
        Func_0808a010(30);
        Func_02002d8c(160, 96, 10);
        Func_0808a010(40);
        Func_02002d8c(56, 64, 10);
        Func_0808a010(70);
        Func_0808a180(actor, 0);
        Func_02002de8();
        Func_080000c0(2);
        Func_0808a200(0, 0);
        Func_02001e20(actor, 2);
    } else if (phase == 1) {
        Func_0808a170(0x208f);
        Func_0808a180(actor, 0);
    }

    Func_02001e7c(phase, actor, 2);
    Func_0808a020();
}
