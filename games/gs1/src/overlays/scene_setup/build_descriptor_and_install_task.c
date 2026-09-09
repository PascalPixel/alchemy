#include "types.h"

/*
 * Scene setup for resource_3bb: allocates a descriptor, stamps its parameter
 * block, binds the in-image data to a graphics handle, and installs the
 * per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020077f6();
s32 Func_02007810();
u8 *Func_020079a8();
u8 *Func_020079b0();
s32 Func_02007950();
void Func_02007886();
s32 Func_020078aa();
s32 Func_020078b6();
void Func_02007856();
void Func_020078a4();

/* This overlay's own routine at file offset 0x3638, installed as a task. */
void Func_0200b638();

/* In-image descriptor at file offset 0x4194. */
extern u8 Data_0200c194[];

/*
 * Seven arguments: four in registers, three from the caller's stack; five are
 * stored as halfwords and two as words, which is how they are typed here. The
 * 232-byte owner includes the seven-word literal pool the body branches over.
 * When the story flag is clear, `second` is mirrored about `centre`. The
 * extent stored at +216 is passed back sign-extended from sixteen bits.
 * Descriptor layout is asserted only for the fields written here.
 */
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_020077f6(59, 0x7170);
    handle = Func_02007810(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_020079a8(first);
    second_record = Func_020079b0(second);

    if (Func_02007950(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02007886(Data_0200c194, handle);

    extent = Func_020078aa();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_020078b6((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_02007856((s32)Func_0200b638 + 1, 0xc76);

    Func_020078a4(handle);
}
