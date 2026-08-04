typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

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

void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
u8 *Func_0808a080();
void Func_0808a0d0();
void Func_0808a140();
void Func_0808a1b8();
void Func_0808a1e0();
void Func_0808a210();
void Func_0808a248();
void Func_08009180();
void Func_080091e0();
u8 *Func_02000048();

void Func_020036ac(void)
{
    u8 *actor;

    Func_0808a018();

    if (*(s16 *)&Data_02000240[448] == 0x36) {
        Func_0808a0d0(0, 472, 600);
        Func_0808a1b8(0, 0x4000, 10);
        Func_0808a210(0x01d00000, -1, 0x02900000, 1);

        actor = Func_0808a080(0);
        Func_080091e0(actor, 0);

        actor = Func_0808a080(0);
        Func_02000048(*(s32 *)(actor + 8), 0, 0x02be0000, 0xdf);

        Func_08009180(92, 46, 92, 40, 3, 2);

        actor = Func_0808a080(0);
        *(s32 *)(actor + 72) = 0x8000;

        Func_0808a1e0(0, 2);
        Func_0808a140(0, 6, -1);

        *(s32 *)(Data_03001ebc + 448) = 515;
        Func_0808a010(60);
        Func_0808a248(8);
    } else {
        Func_0808a140(0, 6, -1);
    }

    Func_0808a020();
}
