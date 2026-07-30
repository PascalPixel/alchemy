#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct DmaTransfer_0809c138 {
    const void *source;
    void *destination;
    u32 control;
};

struct RegisterRestore_0809c138 {
    u32 value;
    volatile void *register_address;
    u32 mask;
};

u8 *Func_080048f4(s32, s32);
s32 Func_08002f40(s32);
void Func_08005340(const void *, void *);
void Func_08091df4(void);
void Func_08091dc8(void);
void Func_08091e20(void);
void Func_0809bb64(void);
void Func_0809bcd4(void);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
s32 Func_080770c0(s32);
void Func_08015040(const void *, s32);
void Func_080030f8(s32);
void Func_08009238(void);

/*
 * Run the palette-and-background interlude used by scene state three.
 * Object priorities are temporarily forced to one, the interlude assets and
 * callback are installed, and every touched display/object state is restored
 * before returning.
 */
void Func_0809c138(void)
{
    u8 *records = *(u8 **)0x03001E70;
    u8 *scene = Func_080048f4(0x1B, 0xCCC);
    u8 *runtime = *(u8 **)0x03001E68;
    u8 saved_priorities[16];
    u32 saved_scene_mode;
    s16 saved_blend;
    s32 index;

    if (M2C_FIELD(scene, s16 *, 0x19E) != 3) {
        return;
    }

    M2C_FIELD(M2C_FIELD(scene, u8 **, 0x1E0), u8 *, 0x5B) = 1;
    saved_scene_mode = M2C_FIELD(scene, u32 *, 0x1C8);
    M2C_FIELD(scene, u32 *, 0x1C8) = 6;
    Func_08091df4();
    Func_08091e20();

    for (index = 0; index < 16; index++) {
        u8 *record = records + 0x18 + index * 0xC;
        saved_priorities[index] = (u8)M2C_FIELD(record, u16 *, 0xA);
        M2C_FIELD(record, u16 *, 0xA) = 1;
    }

    M2C_FIELD(runtime, u16 *, 4) = 1;
    Func_080030f8(1);
    saved_blend = *(volatile s16 *)0x04000052;

    {
        void *palette = (void *)Func_08002f40(0x1B);
        struct DmaTransfer_0809c138 *dma =
            (struct DmaTransfer_0809c138 *)0x040000D4;

        dma->source = palette;
        dma->destination = (void *)0x05000000;
        dma->control = 0x84000070;
        *(volatile u16 *)0x05000000 = 0;
        Func_08005340((u8 *)palette + 0x1C0, (void *)0x02010000);
        dma->source = (const void *)0x02010000;
        dma->destination = (void *)0x06006A00;
        dma->control = 0x84002580;
    }

    *(volatile u16 *)0x0400000A = 0x682;
    *(volatile u16 *)0x04000000 = 0x1340;
    Func_0809bb64();
    Func_080041d8((const void *)0x0809BCF9, 0xC80);
    if (Func_080770c0(0x11C) != 0) {
        Func_08015040((const void *)0x985, 1);
    }
    do {
        Func_080030f8(1);
    } while ((*(volatile u32 *)0x03001B04 & 3) == 0);

    Func_08004278((const void *)0x0809BCF9);
    Func_0809bcd4();
    *(volatile u16 *)0x04000000 = 0x40;
    Func_08009238();

    {
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u16 saved_ime = *ime;
        volatile u16 *count = (volatile u16 *)0x02002090;
        u16 slot = *count;

        *ime = (u16)0x04000208;
        if (slot <= 0x1F) {
            struct RegisterRestore_0809c138 *restore =
                (struct RegisterRestore_0809c138 *)
                    (0x02002090 + slot * 0xC + 4);
            *count = slot + 1;
            restore->value = (u16)saved_blend;
            restore->register_address = (volatile void *)0x04000052;
            restore->mask = 0x20000;
        }
        *ime = saved_ime;
    }

    for (index = 0; index < 16; index++) {
        u8 *record = records + 0x18 + index * 0xC;
        M2C_FIELD(record, u16 *, 0xA) = saved_priorities[index];
    }

    M2C_FIELD(runtime, u16 *, 4) = 0;
    Func_08091dc8();
    Func_08091e20();
    M2C_FIELD(scene, u32 *, 0x1C8) = saved_scene_mode;
    M2C_FIELD(M2C_FIELD(scene, u8 **, 0x1E0), u8 *, 0x5B) = 0;
}
