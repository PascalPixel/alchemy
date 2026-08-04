typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Actor presentation-reset owner at 0x02002aec, complete through its return,
 * trailing alignment and six interleaved pool words before 0x02002bac.  The
 * one-shot branch, state restore and all nine ordered calls are explicit.
 */

extern u8 Data_02000240[];
extern u32 Data_0200c804;
extern u32 Data_0200c75c;
extern u32 Data_0200c788;

extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a010(s32 frames);
extern void Func_080091e0(void *record, s32 phase);
extern void Func_08009080(void *record, s32 mode);
extern void Func_080000c0(s32 frames);

void Func_02002aec(s32 actor)
{
    u8 *runtime = *(u8 **)0x03001e68;
    u8 *record = Func_0808a080(actor);

    if (Data_02000240[498] == 1) {
        Data_02000240[498] = 0;
        Func_0808a100(actor, 1);
    } else {
        Func_0808a1b8(actor, 0x4000, 30);
        Func_0808a100(actor, 3);
        Func_0808a010(30);
    }

    runtime[7] = 0;
    runtime[6] = 15;

    *(u32 *)(record + 8) = Data_0200c804;
    *(u32 *)(record + 16) = Data_0200c75c;
    *(unsigned short *)(record + 6) = (unsigned short)Data_0200c788;

    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
    record[0x55] = 3;
    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    record[0x22] = 0;
    *(u32 *)(record + 12) = 0;
    *(u32 *)(record + 20) = 0;

    Func_080091e0(record, 1);
    Func_08009080(record, 0);
    Func_08009080(record, 1);
    Func_080000c0(1);
}
