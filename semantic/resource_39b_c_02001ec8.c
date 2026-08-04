typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/* Complete linked-object state transition, including six pool words. */
extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern s32 Data_02000434;
extern void Func_02001df8(void);
extern void Func_0808a018(void);
extern u8 *Func_080090c8(s32, s32, s32, s32);
extern void Func_08009098(u8 *, void *);
extern void Func_080000c0(s32);
extern void Func_08009080(u8 *, s32);
extern void Func_080770c8(s32);
extern void Func_0808a020(void);

void Func_02001ec8(void)
{
    s32 *record = *Data_03001edc;
    u8 *slot = ((u8 **)(Data_03001ebc + 0x14))[Data_02000434];
    u8 *object;
    s32 counter;
    s32 i;

    if ((u32)record[0] > 2)
        return;
    Func_0808a018();

    object = (u8 *)record[5];
    if (object == 0) {
        u8 *owner;
        object = Func_080090c8(26, *(s32 *)(slot + 8),
                               *(s32 *)(slot + 12) + 0x180000,
                               *(s32 *)(slot + 16));
        if (object != 0) {
            owner = *(u8 **)(object + 0x50);
            *(s32 *)(object + 20) = *(s32 *)(slot + 20);
            Func_08009098(object, (void *)0x0200a7e8);
            object[0x55] = 4;
            *(u8 **)(object + 0x68) = slot;
            *(s32 *)(object + 12) -= 0x8000;
            if (owner != 0) {
                owner[0x26] = 0;
                owner[9] = (owner[9] & ~0x0c) | 4;
            }
            object[0x54] = 0;
            record[5] = (s32)object;
        }
    }

    object = (u8 *)record[5];
    counter = record[0];
    if (counter <= 2) {
        object[0x54] = 1;
        for (i = counter; i <= 2; i++) {
            Func_02001df8();
            Func_080000c0(30);
            Func_08009080(object, 5 - i);
        }
    }
    record[0] = 3;
    *(s32 *)(object + 12) = (*(s32 *)(object + 8) & (s32)0xfff00000) + 0x80000;
    *(s32 *)(object + 16) = (*(s32 *)(object + 16) & (s32)0xfff00000) + 0x80000;
    Func_080770c8(0x161);
    Func_0808a020();
}
