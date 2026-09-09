#include "types.h"

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */

/* An EWRAM global, not overlay data: the module is linked above this address,
 * so it cannot be in-image. */
extern s16 Data_02000240[];

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */
u8 *Func_02004008();
void Func_02003ff6();
void Func_02002102();
void Func_02001ffc();
/* Four register arguments plus four stack words. */
void Func_02001d72();
void Func_020040e6();
void Func_020040b8_a();
void Func_020040b8_b();
void Func_0200403e();
void Func_020040ae();
void Func_020040f8();
void Func_0200416e();
void Func_0200405c();
u8 *Func_02004082_a();
u8 *Func_0200408c();
void Func_02002070();
void Func_02004082_b();
void Func_02004080();
void Func_020040ea();
void Func_02004184();

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * Data_02000240 + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void FieldScene_RunActorElevenPresentationBeat(void)
{
    u8 *slot;
    s32 offset;

    slot = Func_02004008(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Func_02003ff6();

    Func_02002102(11, 0);
    Func_02001ffc(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_020040e6(11, 1);
    Func_020040b8_a(11, 0, 0);
    Func_0200403e(30);
    Func_020040ae(11, 2);
    Func_020040f8(11, 0x103, 0);
    Func_0200416e(147);
    Func_0200405c(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02002070(11,
                  *(s16 *)(Func_02004082_a(0) + 10),
                  *(s16 *)(Func_0200408c(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Func_02004082_b(10);
    Func_02004080(0x301);
    Func_020040ea(14, 0, 0);

    offset = 0x22b;
    ((u8 *)Data_02000240)[offset] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Func_020040b8_b();
}
