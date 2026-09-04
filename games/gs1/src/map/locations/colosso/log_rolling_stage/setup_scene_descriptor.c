#include "colosso_log_rolling_stage.h"

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
s32 Func_02008eb0();
void Func_02008dbe();
s32 Func_02008de2();
s32 Func_02008dee();
void Func_02008d8e();
void Func_02008ddc();
                                /* allocate/fetch a record by (id, size) */
                                /* reserve a graphics handle */
                                /* release a graphics handle */
                                /* upload image data to a handle */
                                /* upload a palette ramp */
                                /* next palette slot index */
                                /* install a per-frame task (callback, rate) */
                                /* test a story flag */
                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200cd80 - 0x8000). */
extern u8 Data_0200cd80[];

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200c0d0 - 0x8000). */
void Func_0200c0d0(void);

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first_actor);
    second_record = Func_02008f10(second_actor);

    if (Func_02008eb0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02008dbe(Data_0200cd80, handle);

    extent = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_02008dee((s16)extent, 512, handle);

    Func_02008d8e((s32)Func_0200c0d0 + 1, 0xc76);

    Func_02008ddc(handle);
}
