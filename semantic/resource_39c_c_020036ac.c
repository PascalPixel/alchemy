#include "types.h"

/*
 * Resource 39c owner at 0x020036ac (220 bytes).
 *
 * Complete owner: push/frame prologue at 0x020036ac through the sole
 * interworking return at 0x0200376c-0x02003774, followed by alignment and
 * four referenced pool words through 0x02003787.  The next owner starts at
 * 0x02003788.
 *
 * The routine always opens and closes the scene bracket.  Scene 0x36 runs a
 * one-time actor/camera setup, spawns the exact-C local object variant at
 * 0x02000048, publishes presentation state 515 in the shared workspace, waits
 * 60 frames and advances the scene.  Other scenes only apply the common actor
 * state call before closing.
 *
 * All sixteen calls resolve across thirteen targets.  The only local call is
 * the established four-argument spawn owner at 0x02000048; the remaining
 * sites resolve through veneers and match below in source/machine order.
 */

extern u8 Data_02000240[];
extern u8 *Data_03001ebc;















extern void Func_02009166();
extern void Func_020091de();
extern void Func_02009272();
extern void Func_020092a4();
extern u8 * Func_020091ca();
extern void Func_02009128();
extern u8 * Func_020091d6();
extern u8 * Func_02003756();
extern void Func_02009126();
extern u8 * Func_020091fc();
extern void Func_020092d2();
extern void Func_02009296();
extern void Func_02009202();
extern void Func_02009328();
extern void Func_020092be();
extern void Func_0200922a();
void Func_020036ac(void)
{
    u8 *actor;

    Func_02009166();

    if (*(s16 *)&Data_02000240[448] == 0x36) {
        Func_020091de(0, 472, 600);
        Func_02009272(0, 0x4000, 10);
        Func_020092a4(0x01d00000, -1, 0x02900000, 1);

        actor = Func_020091ca(0);
        Func_02009128(actor, 0);

        actor = Func_020091d6(0);
        Func_02003756(*(s32 *)(actor + 8), 0, 0x02be0000, 0xdf);

        Func_02009126(92, 46, 92, 40, 3, 2);

        actor = Func_020091fc(0);
        *(s32 *)(actor + 72) = 0x8000;

        Func_020092d2(0, 2);
        Func_02009296(0, 6, -1);

        *(s32 *)(Data_03001ebc + 448) = 515;
        Func_02009202(60);
        Func_02009328(8);
    } else {
        Func_020092be(0, 6, -1);
    }

    Func_0200922a();
}
