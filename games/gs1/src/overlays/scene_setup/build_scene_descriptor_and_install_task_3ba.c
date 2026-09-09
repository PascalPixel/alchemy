#include "types.h"

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020072c6();            /* allocate a record by (id, size) */
s32 Func_020072e0();            /* reserve a graphics handle */
u8 *Func_02007478();            /* scene record for an actor selector */
u8 *Func_02007480();            /* scene record for an actor selector */
s32 Func_02007428();            /* test a story flag */
void Func_02007356();           /* upload image data to a handle */
s32 Func_0200737a();            /* next palette slot index */
s32 Func_02007386();            /* upload a palette ramp */
void Func_02007326();           /* install a per-frame task (callback, rate) */
void Func_02007374();           /* release a graphics handle */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */
extern u8 Data_0200bf14[];

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */
void Func_0200b3a0(void);

/*
 * Seven arguments: four in registers, three from the caller's stack. The
 * 232-byte owner includes the six-word literal pool the body branches over.
 * The palette index stored at +216 is passed on sign-extended from sixteen
 * bits, so the narrowing is deliberate. Descriptor layout is asserted only
 * for the fields written here, and the actor records are touched at +8 and
 * +16 only on the flag-clear path.
 */
void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = Func_020072c6(59, 0x7170);
    handle = Func_020072e0(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = Func_02007478(first);
    rec1 = Func_02007480(second);

    if (Func_02007428(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Func_02007356(Data_0200bf14, handle);

    pal = Func_0200737a();
    *(u16 *)(desc + 216) = (u16)pal;
    Func_02007386((s16)pal, 512, handle);

    Func_02007326((s32)Func_0200b3a0 + 1, 0xc76);

    Func_02007374(handle);
}
