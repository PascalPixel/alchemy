typedef signed short s16;
typedef unsigned int u32;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3b5, cutscene script step at 0x02000644 (164 bytes; 154 bytes of
 * code, two bytes of alignment, and the 8-byte literal pool at
 * 0x020006e0-0x020006e7).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000644 and
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020006d8-0x020006dc.  r0 holds
 * the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x02008645
 * in the handler word of twenty-seven separate 12-byte records — by far the
 * most-referenced handler in this overlay.  Under the proven 0x02008000 link
 * base that word is Func_02000644 + the Thumb bit.  It takes no arguments and
 * reads its variant from the scene counter instead, which is why one handler
 * serves so many script records.
 *
 * The table at 0x02009d00 (file offset 0x1d00) is an array of 8-byte records
 * {u32 script, u16, u16} indexed by the scene counter minus one.
 *
 * Uncertainty: r4 is used as scratch at 0x0200067a although the prologue never
 * saves it.  Recognised idiom in this codebase, reproduced as-is.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; thirteen sites,
 * matching the inventory's `calls=13`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_0808a018();
u8 *Func_0808a080();
void Func_080f9010();
void Func_08009178();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0d8();
void Func_0808a010();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_0808a020();

void Func_02000644(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s16 *counter;
    s32 scene;
    s32 id;
    u8 *actor;
    u8 *entry;

    Func_0808a018();

    /* Clear the "spoken" byte of every actor in the cutscene's id range. */
    for (id = 8; (u32)id <= 65; id++) {
        actor = Func_0808a080(id);
        if (actor != 0) {
            actor[0x55] = 0;
        }
    }

    counter = (s16 *)(workspace + 364);
    scene = *counter - 1;

    Func_080f9010(158);

    entry = (u8 *)0x02009d00 + (scene << 3);
    Func_08009178(*(s32 *)entry, *(u16 *)(entry + 4), *(u16 *)(entry + 6));

    Func_0808a090(0, 0x8000, 0x4000);

    actor = Func_0808a080(0);
    actor[0x55] = 0;

    Func_0808a100(0, 2);

    if (scene != 6) {
        Func_0808a0d8(0, 2, -8);
        Func_0808a010(10);
    }

    Func_0808a248(*counter);
    Func_0808a368();
    Func_0808a370();

    Func_0808a020();
}
