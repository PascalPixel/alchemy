#include "types.h"

/*
 * resource_3bc descriptor constructor at 0x02004494, complete 232-byte owner
 * including its branch-skipped seven-word pool.  This is the overlay-local
 * sibling of the independently reviewed resource_3bb:39fc owner.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at each occurrence, including the repeated Func_0808a080 calls) --
 * declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern u8 *Func_02008d2e(s32 id, s32 size);            /* Func_08000148 veneer */
extern s32 Func_02008d48(s32 size);                    /* Func_08000170 veneer */
extern u8 *Func_02008f08(s32 actor);                   /* Func_0808a080 veneer #1 */
extern u8 *Func_02008f10(s32 actor);                   /* Func_0808a080 veneer #2 */
extern s32 Func_02008eb0(s32 flag);                    /* Func_080770c0 veneer */
extern void Func_02008dbe(const void *source, s32 handle); /* Func_080001a8 veneer */
extern s32 Func_02008de2(void);                        /* Func_080001d0 veneer */
extern void Func_02008dee(s16 palette, s32 size, s32 handle); /* Func_080001c8 veneer */
extern void Func_02008d8e(s32 task_address, s32 frame_budget); /* Func_080000d0 veneer */
extern void Func_02008ddc(s32 handle);                  /* Func_08000178 veneer */
extern u8 Data_0200cd80[];

void Func_02004494(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 palette;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(0x200);

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first);
    second_record = Func_02008f10(second);
    if (Func_02008eb0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;
    Func_02008dbe(Data_0200cd80, handle);

    palette = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)palette;
    Func_02008dee((s16)palette, 0x200, handle);

    /* Overlay link base 0x02008000: 0x0200c0d1 is Func_020040d0 + Thumb bit. */
    Func_02008d8e(0x0200c0d1, 0xc76);
    Func_02008ddc(handle);
}
