typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_376 owner at 0x0200055c, 120 bytes: the actor-22 sibling of the
 * directional scene selector immediately before it.
 *
 * Complete owner: `push {lr}` at 0x0200055c through the interworking return
 * at 0x020005b4-0x020005b7, followed by seven referenced pool words through
 * 0x020005d3.  The next independent prologue starts at 0x020005d4.
 *
 * Actor zero's +6 u16 is biased by 0xffff5fff and compared unsigned with
 * 0x3ffe.  Consequently the direct handoff path covers the original closed
 * range 0xa001..0xdfff and calls Func_080b0008(2, 22).  Outside that range,
 * a scripted sequence displays dialogue 0x1c09 when flag 0x87a is set;
 * otherwise flag 0x815 selects 0x11a3 rather than 0x0f54.  All three
 * dialogue branches converge on Func_0808a180(22, 0), then close the sequence.
 *
 * All ten static call sites resolve independently under the overlay +2
 * branch rule, and their order is preserved explicitly below.
 */

extern u8 *Func_0808a080(s32 actor_id);
extern void Func_080b0008(s32 arg0, s32 actor_id);
extern void Func_0808a018(void);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a180(s32 actor_id, s32 mode);
extern void Func_0808a020(void);

void Func_0200055c(void)
{
    u8 *actor;
    u32 biased_direction;

    actor = Func_0808a080(0);
    biased_direction = *(u16 *)(actor + 6);
    biased_direction += 0xffff5fff;

    if (biased_direction <= 0x3ffe) {
        Func_080b0008(2, 22);
        return;
    }

    Func_0808a018();
    if (Func_080770c0(0x87a) != 0) {
        Func_0808a170(0x1c09);
    } else if (Func_080770c0(0x815) != 0) {
        Func_0808a170(0x11a3);
    } else {
        Func_0808a170(0x0f54);
    }
    Func_0808a180(22, 0);
    Func_0808a020();
}
