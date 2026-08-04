#include "types.h"

extern s32 Func_080770c0(s32 flagId);
extern s32 Func_080770e0(s32 counterId);
extern void Func_080770e8(s32 counterId, s32 value);
extern u8 *Func_0808a080(s32 actorId);

/* Synchronize the current map actor's display depth and advance the shared
 * encounter counter, preserving the special first-state marker. */
void Func_02000400(void)
{
    s32 actorId = *(volatile s32 *)0x02000434;
    u8 *actor = Func_0808a080(actorId);
    u8 *mapState = *(u8 *volatile *)0x03001ebc;
    s32 counter;

    *(u16 *)(actor + 6) = (u16)(*(volatile s32 *)0x03001e40 << 12);
    counter = Func_080770e0(0x210);

    if (counter == 1) {
        *(u16 *)(mapState + 0x182) = 99;
    } else if (counter != 0 && Func_080770c0(0x106) == 0) {
        counter--;
    }

    Func_080770e8(0x210, counter);
}
