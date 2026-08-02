typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Complete 832-byte current-cell transition owner through the two tail pool
 * words at 0x02002368.  The literal island at 0x020021f2 is branch-skipped;
 * the indirect call at 0x02002344 is the relocated multiply helper, not a
 * return.  The reviewed resource_39c:55c0 owner supplies field-level context,
 * while every branch, constant and all 31 calls below were rechecked here.
 */
extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern s32 Data_02000434;

extern s32 Func_080000c0();
extern void Func_08000128(s32, s32, s32 *);
extern s32 Func_08009020();
extern s32 Func_08009080();
extern s32 Func_08009098();
extern u8 *Func_080090c8(s32, s32, s32, s32);
extern s32 Func_080090d0();
extern s32 Func_08009150();
extern s32 Func_080091d8();
extern s32 Func_080091e0();
extern s32 Func_080770d0();
extern s32 Func_0808a018();
extern s32 Func_0808a020();
extern s32 Func_0808a0c0();
extern s32 Func_080f9010();
extern void Func_02001bc8(u8 *);
typedef s32 (*MultiplyHelper)(s32, s32);

static s32 Fixed20_02002030(s32 value)
{
    if (value < 0)
        value += 0xfffff;
    return value >> 20;
}

void Func_02002030(void)
{
    s32 *record = *Data_03001edc;
    u8 *runtime = Data_03001ebc;
    u8 *slot = ((u8 **)(runtime + 0x14))[Data_02000434];
    u8 saved_flags = slot[0x55];
    s32 variant = *(short *)(0x0200a7f4 + (((*(u32 *)0x03001ae8 >> 4) & 15) * 2));
    s32 probe[3];
    u8 *first_cell;
    u8 *cell;
    s32 result;
    u8 *object;

    if (variant == -1)
        return;

    probe[0] = (*(s32 *)(slot + 8) & (s32)0xfff00000) + 0x80000;
    probe[1] = *(s32 *)(slot + 20);
    probe[2] = (*(s32 *)(slot + 16) & (s32)0xfff00000) + 0x80000;
    first_cell = (u8 *)0x02010000 +
        (Fixed20_02002030(probe[2]) * 128 + Fixed20_02002030(probe[0])) * 4;
    Func_08000128(0x200000, variant, probe);
    cell = (u8 *)0x02010000 +
        (Fixed20_02002030(probe[2]) * 128 + Fixed20_02002030(probe[0])) * 4;

    if (first_cell[2] != record[1] && cell[2] == record[1] && record[0] == 0)
        return;

    Func_0808a018();
    result = Func_080091d8(slot, probe);
    if (result != 0)
        return;

    object = (u8 *)record[6];
    if (object != 0) {
        *(u16 *)(object + 0x64) = (u16)result;
        Func_08009098(object, (void *)0x0200a7dc);
        Func_08009080(object, 7);
        record[6] = result;
    }

    if (cell[2] == record[1] && record[0] != 0) {
        u8 *source = (u8 *)record[5];
        object = Func_080090c8(26, *(s32 *)(source + 8),
                              *(s32 *)(source + 12), *(s32 *)(source + 16));
        if (object != 0) {
            u8 *owner = *(u8 **)(object + 0x50);
            *(s32 *)(object + 20) = *(s32 *)(source + 20);
            Func_08009098(object, (void *)0x0200a7d0);
            object[0x55] = (u8)result;
            *(u16 *)(object + 0x64) = (u16)result;
            object[0x23] = 2;
            *(s32 *)(object + 0x30) = 0x40000;
            *(s32 *)(object + 0x34) = 0x20000;
            Func_08009150(object, probe[0], probe[1], probe[2]);
            if (owner != 0) {
                Func_08009020(owner, 6);
                owner[0x26] = 0;
            }
            record[6] = (s32)object;
        }
    }

    record[0]--;
    if (record[0] == 0) {
        Func_080090d0((void *)record[5]);
        record[5] = 0;
        Func_080770d0(0x161);
    } else if (record[5] != 0) {
        Func_08009080((void *)record[5], 6 - record[0]);
    }

    Func_08009080(slot, 6);
    Func_080000c0(3);
    Func_080f9010(0x98);
    Func_08009080(slot, 7);
    *(s32 *)(slot + 0x30) = 0x30000;
    *(s32 *)(slot + 0x34) = 0x20000;
    *(s32 *)(slot + 0x28) = 0x40000;
    slot[0x55] &= 0x7e;
    Func_080091e0(slot, 0);
    Func_0808a0c0(0, *(short *)((u8 *)probe + 2), *(short *)((u8 *)probe + 10));
    Func_08009080(slot, 6);
    Func_080000c0(2);

    if (cell[2] != record[1])
        Func_080091e0(slot, 1);
    else
        Func_080f9010(0xd7);
    Func_080000c0(1);
    slot[0x55] = saved_flags;

    if (cell[2] == record[1] && record[6] == 0) {
        s32 frame = 0;
        Func_08009080(slot, 18);
        Func_080f9010(0xf1);
        goto frame_test;

frame_wait:
            Func_080000c0(1);
            frame++;

frame_test:
        if ((frame & 15) == 0)
            Func_02001bc8(slot);
        if (frame <= 31 || *(volatile u32 *)0x03001c94 == 0)
            goto frame_wait;

        Func_080f9010(0x120);
        Func_080000c0(1);
        *(s32 *)(slot + 8) = record[3];
        *(s32 *)(slot + 16) = record[4];
        Func_080091e0(slot, 1);
    }

    record[2] = 0;
    Func_0808a020();
    *(s32 *)(runtime + 0x1b4) +=
        ((MultiplyHelper)0x03000118)(*(s32 *)(runtime + 0x1b0), 0x200000);
}
