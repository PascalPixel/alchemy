typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3ca double-buffer sample generator at 0x02000f80, complete
 * 340-byte span through its nine-word pool. Two 160-entry interleaved
 * halfword streams are generated in the inactive 1,920-byte bank from a
 * signed lookup table and a relocated fixed-point helper. The phase counter
 * advances once, then the active bank bit flips for the DMA leaf at 0x0f30.
 *
 * The apparent returns at 0x02000ff6 and 0x0200106a are calls through the
 * relocated helper at 0x03000118, not owner boundaries. They are ordinary
 * typed C calls; this owner contains no BL sites and no assembly embedding.
 */

typedef s32 (*FixedScale_02000f80)(s32 value, s32 scale);

extern u8 *Data_03001ed8;

void Func_02000f80(void)
{
    FixedScale_02000f80 fixedScale = (FixedScale_02000f80)0x03000118;
    const s16 *wave = (const s16 *)0x020094c8;
    u8 *runtime = Data_03001ed8;
    s16 state = *(volatile s16 *)0x03001ade;
    s32 phase = *(u16 *)(runtime + 0x0f02) + (u16)state;
    u8 *inactiveBank = runtime + (runtime[0x0f00] ^ 1) * 1920;
    s32 position;
    s32 step;
    s32 scale;
    s32 base;
    s32 index;

    position = (s32)((u32)*(s32 *)(runtime + 0x0f08) * (u32)phase);
    step = *(s32 *)(runtime + 0x0f10);
    scale = *(s32 *)(runtime + 0x0f18);
    base = *(volatile u16 *)0x03001adc;

    for (index = 0; index < 160; index++) {
        s32 value = fixedScale(wave[(position >> 16) & 0xff], scale);

        if (value < 0) {
            value += 255;
        }
        *(u16 *)(inactiveBank + index * 4) =
            (u16)(base + (((u32)value << 8) >> 16));
        position = (s32)((u32)position + (u32)step);
    }

    position = (s32)((u32)*(s32 *)(runtime + 0x0f0c) * (u32)phase);
    step = *(s32 *)(runtime + 0x0f14);
    scale = *(s32 *)(runtime + 0x0f1c);
    base = (u16)state;

    for (index = 0; index < 160; index++) {
        s32 value = fixedScale(wave[(position >> 16) & 0xff], scale);

        if (value < 0) {
            value += 255;
        }
        *(u16 *)(inactiveBank + index * 4 + 2) =
            (u16)(base + (((u32)value << 8) >> 16));
        position = (s32)((u32)position + (u32)step);
    }

    (*(u16 *)(runtime + 0x0f02))++;
    runtime[0x0f00] ^= 1;
}
