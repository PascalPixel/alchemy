/*
 * Scene setup for resource_3b7: four actor records built from three parallel
 * in-image tables, a camera block reset, and the per-frame task install.
 */

#include "types.h"
#include "scene.h"

extern u8 gOv[];      /* in-image 0x205a: four X tile coordinates */
extern u8 gOv2[];      /* in-image 0x205e: four Z tile coordinates */
extern u16 gOv3[];     /* in-image 0x2062: four headings */
extern u8 gOv4[];      /* scratch EWRAM above the image */
extern u8 gOv5[];      /* scratch EWRAM above the image: 4 x 24 bytes */

u8 *Talk_Run();

u8 *Talk_unk2_4();

void Scene_RunSecondaryScript();           /* the installed per-frame task */

/*
 * The 148-byte owner includes its eight-word literal pool: those words lie
 * past the return and are read only by the pc-relative loads.
 * Field names are descriptive only: the 24-byte record stride and the cleared
 * halfwords at +14..+20 are read off the stores alone, and the second heading
 * is 0x0001 rather than a multiple of 0x4000 -- the byte is certain, its
 * meaning is not.
 */
void State_InitFourActorRecordsAndInstallTask(void)
{
    u8 *work = gOv4;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    xtbl = gOv;
    rec = gOv5;
    htbl = gOv3;
    ztbl = gOv2;

    do {
        *(s32 *)(rec + 0) = (s32)*xtbl << 16;
        *(s32 *)(rec + 8) = (s32)*ztbl << 16;
        *(s32 *)(rec + 4) = 0;
        *(u16 *)(rec + 12) = *htbl;
        *(u16 *)(rec + 14) = 0;
        *(u16 *)(rec + 16) = 0;
        *(u16 *)(rec + 18) = 0;
        *(u16 *)(rec + 20) = 0;

        i++;
        xtbl++;
        ztbl++;
        htbl++;
        rec += 24;
    } while (i != 4);

    *(s32 *)(work + 4) = (s32)0xffe20000;      /* -30.0 in 16.16 */
    *(s32 *)(work + 8) = 0;
    *(s32 *)(work + 12) = 0x640000;            /* 200 << 15, i.e. 100.0 */
    *(s32 *)(work + 64) = 0;
    *(s32 *)(work + 68) = 0;
    *(s32 *)(work + 72) = 0;
    *(s32 *)(work + 76) = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Talk_unk4_4(Talk_Run(20), 2);
    Talk_unk5_4(Talk_unk2_4(21), 2);

    /* The task word names in-image code with the Thumb bit set, not a runtime
     * address; the locals keep it and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = (void (*)(void))0x02008e5d;

        Talk_unk6_4(task, budget);
    }
}
