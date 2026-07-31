/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

void Func_0200252c();
void Func_080770c8();
void Func_080770d0();
void Func_0808a018();
void Func_0808a020();
Slot_02001f70 *Func_0808a080();

void Func_02001f70(void)
{
    Func_0808a018();

    if ((Func_0808a080(17)->w8 >> 20) == 45) {
        Func_080770c8(0x974);
    } else {
        Func_080770d0(0x974);
    }

    if ((Func_0808a080(18)->w8 >> 20) == 46) {
        Func_080770c8(0x975);
    } else {
        Func_080770d0(0x975);
    }

    Func_0200252c();
    Func_0808a020();
}
