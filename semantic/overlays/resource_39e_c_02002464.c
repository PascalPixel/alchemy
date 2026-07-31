typedef int s32;

/*
 * resource_39e owner at 0x02002464, 32 bytes: NOT in the unindexed
 * population -- found by boundary-checking 0x02001dbc's pool end (the
 * resource_3b9 0x02001a14 precedent, third such find in this overlay
 * after 0x020041c4). A minimal scripted-scene bracket: open
 * (Func_0808a018), dialogue 0x1a58 (Func_0808a170), Func_0808a190(11,
 * 0), close (Func_0808a020).
 *
 * Complete owner: `push {lr}` at 0x02002464 through `pop {r0} / bx r0`
 * at 0x0200247c-0x0200247e, one pool word (0x00001a58) ending
 * 0x02002483; the next owner (`push {lr}`) begins at 0x02002484 --
 * yet another boundary-gap function (flag-gated, calls Func_08015040 /
 * Func_08009178, new vocabulary for this overlay), left for its own
 * read and noted in the queue file.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a190(s32 id, s32 arg1);

void Func_02002464(void)
{
    Func_0808a018();
    Func_0808a170(0x1a58);
    Func_0808a190(11, 0);
    Func_0808a020();
}
