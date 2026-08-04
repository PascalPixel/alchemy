#include "types.h"

extern s32 Func_080770c0(s32 flagId);
extern void Func_02000cc0(s32, s32, s32, s32, s32, s32, s32);
extern void Func_080000d0(s32 callback, s32 priority);
extern void Func_080000d8(s32 callback);
extern void Func_080000c0(s32 frames);
extern void Func_08000130(s32, s32, s32);
extern void Func_080f9010(s32 soundId);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_08009128(void);

/*
 * Complete 352-byte display-layout transition.  The body chooses one of two
 * tile-map arrangements from story flag 512, installs a temporary callback,
 * waits 101 frames, draws the matching pair of windows, and removes the
 * callback.  The interworking return at 0x02001748 owns alignment and the
 * four-word pool through 0x0200175b.  Both flag tests are observable calls and
 * intentionally remain separate.
 */
void Func_020015fc(void)
{
    volatile s32 *counter = (volatile s32 *)0x0200b738;

    if (Func_080770c0(512) == 0) {
        Func_02000cc0(10, 19, 16, 5, 0, 10, 31);
        Func_02000cc0(10, 51, 16, 5, 1, 10, 31);
        Func_02000cc0(42, 51, 16, 5, 2, 10, 31);
    } else {
        Func_02000cc0(10, 19, 16, 5, 0, 10, 31);
        Func_02000cc0(10, 83, 16, 5, 1, 10, 31);
        Func_02000cc0(42, 83, 16, 5, 2, 10, 31);
    }

    *counter = 0;
    Func_080000d0(0x020095cd, 3200);
    Func_080000c0(1);
    Func_08000130(1, 0, 0x02009579);
    Func_080f9010(231);

    *counter = 0;
    do {
        Func_080000c0(1);
        (*counter)++;
    } while (*counter <= 100);

    Func_080f9010(289);
    if (Func_080770c0(512) == 0) {
        Func_08009180(0, 32, 32, 0, 32, 32);
        Func_08009180(32, 32, 64, 0, 32, 32);
    } else {
        Func_08009180(0, 64, 32, 0, 32, 32);
        Func_08009180(32, 64, 64, 0, 32, 32);
    }

    Func_080000c0(1);
    Func_08000130(1, 0, 0);
    Func_080000c0(1);
    Func_080000d8(0x020095cd);
    Func_08009128();
    Func_080000c0(30);
}
