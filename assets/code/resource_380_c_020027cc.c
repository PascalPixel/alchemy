typedef int s32;

/*
 * resource_380 owner at 0x020027cc, 32 bytes: a second talk handler for
 * actor 9, and the odd member of this overlay's published-callback bank.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` reaches it. Found by
 * `bun tools/overlay_published.ts resource_380`. The bank as a whole is
 * written up in resource_380_c_02002674.c.
 *
 * NOT A COPY OF ITS NEIGHBOURS, which is why it was disassembled rather
 * than swept in with them. It has the same 32-byte layout as the four
 * stubs at 0x0200274c-0x020027ac and the same scene bracket, but the
 * third call is different: where those end with the imported
 * Func_02006a26(actor, 0), this one calls the overlay's OWN
 * Func_02006a26 with (9, 10). The distinction is visible only in the
 * resolved `bl` target -- 0x02004248 is a direct intra-image branch,
 * not a veneer -- so a size-and-shape match would have mis-drafted it.
 *
 * Actor 9 therefore has two published handlers, this one and the
 * branching 0x02002674. Which script record installs which is not
 * established here.
 *
 * Func_02006a26 is already tracked byte-exact as
 * assets/code/resource_380_c_02004248.c, a two-call wrapper the file's
 * own header describes as this overlay's most-called workhorse; its
 * signature (s32, s32) is taken from there rather than re-derived.
 *
 * Complete owner: `push {lr}` at 0x020027cc through `pop {r0} / bx r0`
 * at 0x020027e4-0x020027e6, then the one-word literal pool at
 * 0x020027e8 (line id 0x1072) and a zero alignment halfword at
 * 0x020027ea; the next owner's prologue begins at 0x020027ec.
 *
 * The line id 0x1072 sits outside the 0x10c7-0x10cc block the rest of
 * the bank draws from, which is consistent with this being a different
 * kind of interaction, but the id space is not mapped and that is an
 * observation rather than a finding.
 *
 * The two imported `bl` targets resolved through the import-veneer
 * table under the +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: Func_02006a26's arguments carry no established meaning,
 * so 9 and 10 are transcribed, not interpreted -- 9 matching the actor
 * id used elsewhere in the bank is suggestive only.
 */

extern void Func_0200716c(void);
extern void Func_02007186(void);
extern void Func_0200722a(s32 dialogueId);
extern void Func_02006a26(s32 arg0, s32 arg1);

void Func_020027cc(void)
{
    Func_0200716c();
    Func_0200722a(0x1072);
    Func_02006a26(9, 10);
    Func_02007186();
}
