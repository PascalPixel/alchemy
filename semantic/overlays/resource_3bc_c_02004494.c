typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc descriptor constructor at 0x02004494, complete 232-byte owner
 * including its branch-skipped seven-word pool.  This is the overlay-local
 * sibling of the independently reviewed resource_3bb:39fc owner.
 */

extern u8 *Func_08000148(s32 id, s32 size);
extern s32 Func_08000170(s32 size);
extern void Func_08000178(s32 handle);
extern void Func_080001a8(const void *source, s32 handle);
extern void Func_080001c8(s16 palette, s32 size, s32 handle);
extern s32 Func_080001d0(void);
extern void Func_080000d0(s32 task_address, s32 frame_budget);
extern s32 Func_080770c0(s32 flag);
extern u8 *Func_0808a080(s32 actor);
extern u8 Data_0200cd80[];

void Func_02004494(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 palette;

    descriptor = Func_08000148(59, 0x7170);
    handle = Func_08000170(0x200);

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_0808a080(first);
    second_record = Func_0808a080(second);
    if (Func_080770c0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;
    Func_080001a8(Data_0200cd80, handle);

    palette = Func_080001d0();
    *(u16 *)(descriptor + 216) = (u16)palette;
    Func_080001c8((s16)palette, 0x200, handle);

    /* Overlay link base 0x02008000: 0x0200c0d1 is Func_020040d0 + Thumb bit. */
    Func_080000d0(0x0200c0d1, 0xc76);
    Func_08000178(handle);
}
