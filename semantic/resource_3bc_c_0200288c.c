#include "types.h"

/*
 * resource_3bc owner at 0x0200288c, 288 bytes (0x0200288c-0x20029ac,
 * including the trailing six-word literal pool 0x2002994-0x20029ab): pick
 * this overlay's current scene/actor id, run its scene-record entry through
 * a message/flag sequence, and either flag "actor > 3" with a message + a
 * story flag or hand the next actor index to sibling owner Func_02002640.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,fp / mov r6,sl / mov r5,r9
 * / push {r5, r6, r7} / mov r7,r8 / push {r7}` at 0x0200288c through
 * `pop {r3} / mov r8,r3 / mov r9,r5 / mov sl,r6 / mov fp,r7 / pop {r5,r6,r7}
 * / pop {r0} / bx r0` at 0x2002984-0x2002992.  The popped word is not the
 * return address on r0's path (the epilogue overwrites r0 with the popped
 * lr before `bx`), and neither branch sets a return value before the shared
 * tail, so this owner is void.
 *
 * Not found by the structural inventory walk (unindexed); twenty-one call
 * sites over fourteen distinct targets, all resolved with the `+2` rule
 * against the raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * the tool's --annotate mode against an explicit owner/end pair, `bun
 * tools/overlay-show resource_3bc 288c | cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_3bc 288c 2a3c --annotate`):
 *   0x20028a4, 0x20028aa, 0x20028ba -> veneer -> Func_0808a080(sel)   scene-record accessor (established, resource_37a/395/37b/3ba)
 *   0x20028c0                       -> veneer -> Func_0808a018()     established no-arg scene bracket (resource_37b/3c8/39a/3cb)
 *   0x20028ca, 0x20028fa, 0x2002974 -> veneer -> Func_0808a170(msg)  display message by id (established)
 *   0x20028d2                       -> veneer -> Func_0808a178(arg0, 0)
 *   0x20028ec                       -> veneer -> Func_0808a070(sel, 0)   two-arg predicate, established shape, purpose not resolved
 *   0x2002904, 0x200297c            -> veneer -> Func_0808a180(arg0, 0)
 *   0x200291c                       -> veneer -> Func_0808a368()     no-arg scene helper (established trio, resource_37b)
 *   0x2002920, 0x2002966            -> veneer -> Func_0808a370()     no-arg scene helper (established trio, resource_37b)
 *   0x2002932, 0x2002944            -> veneer -> Func_080770e8(id, value)   unestablished; same call shape as Func_080770c8 (id-keyed setter)
 *   0x200294e                       -> veneer -> Func_0808a248(sel)  scene request keyed by a selector (established, resource_3ba)
 *   0x2002956                       -> veneer -> Func_080770c8(flag)  set a story flag (established)
 *   0x200295e                       -> raw prologue -> Func_02002640(actorIndex)   sibling item-28 owner, same overlay, undrafted at time of writing
 *   0x2002962                       -> veneer -> Func_0808a360()     no-arg scene helper (established trio, resource_37b)
 *   0x2002980                       -> veneer -> Func_0808a020()     established no-arg scene bracket, paired with Func_0808a018
 * Two calls print duplicate raw hex (0x20028a4/0x20028aa are the identical
 * `f004 fd0b` encoding, and the overlay bl rule makes encoding-not-site the
 * target key), so Func_0808a080(arg0) really is called twice in a row with
 * the same argument; transcribed as written, not merged.
 *
 * 0x03001ebc is the overlay workspace pointer, same global documented in
 * semantic/overlays/resource_37b_c_02000150.c ("workspace = *(u8 **)
 * 0x03001ebc"); this owner writes workspace+448 (word, 512) and workspace+456
 * (word, 15) on entry to the id-branch, then workspace+448 again (word, the
 * predicate result -- 0 on every path that reaches the second write, since
 * that write is reachable only from the Func_0808a070(sel,0)==0 arm) before
 * falling into the shared tail; it separately restamps two halfword fields
 * of the SAME workspace object at +0xcc2 (cue id 0x2089) and +0xcc4 (4),
 * read via a second dereference of the pool pointer rather than the r9 copy
 * -- same value, no intervening write, so modelled as one variable.
 *
 * The scene/actor selector is `*(u32 *)&Data_02000240[500]`, the identical
 * word-at-500 idiom documented in semantic/overlays/resource_3cb_c_
 * 02001158.c ("0x02000240 is ... an ordinary RAM record"); this owner is the
 * first sibling seen to also INCREMENT that local copy (never written back)
 * before comparing it to 3 and passing it on to Func_02002640.
 *
 * The scene record's fields at +8 and +16 (Func_0808a080's return, indexed
 * as s32*) are each read whole and shifted right 20 before being handed to
 * Func_080770e8 -- a coarser split (12-bit integer part) than the +10/+18
 * halfword-view idiom documented in resource_37b_c_02000150.c for the same
 * two fields elsewhere, so modelled separately rather than assumed to be the
 * same fixed-point format.
 *
 * Control flow, matched to the branch shape: query the predicate for the
 * selector; if it is already set (nonzero), display message 0x2088, call
 * Func_0808a180(arg0, 0), and fall into the tail. Otherwise display message
 * 0x2087, call Func_0808a180(arg0, 0), stamp the two workspace words and the
 * two scene helpers, then take the record's two Q20 fields to
 * Func_080770e8, increment the local selector copy, and either (selector >
 * 3): request scene 10, set story flag 0x11a; or (selector <= 3): forward
 * the incremented selector to Func_02002640, run the two remaining scene
 * helpers, and restamp workspace+448 with the predicate's (zero) result.
 * Every arm reaches the shared tail: Func_0808a020().
 *
 * Uncertainties: Func_0808a070's, Func_0808a178's, Func_0808a180's and
 * Func_080770e8's exact purposes are not resolved beyond call shape;
 * Func_0808a018 is reached with the third Func_0808a080 call's leftover
 * return value in r0, which the established no-arg reading (resource_37b)
 * says it ignores, so it is spelled with none, matching every other sibling
 * call site.
 */

extern u8 Data_02000240[];

s32 *Func_0808a080();          /* scene-record accessor */
void Func_0808a018();          /* no-arg scene bracket, opens with Func_0808a020 */
void Func_0808a170();          /* display message by id */
void Func_0808a178();
s32 Func_0808a070();           /* two-arg predicate, purpose not resolved */
void Func_0808a180();
void Func_0808a368();
void Func_0808a370();
void Func_080770e8();          /* id-keyed setter, shape parallel to Func_080770c8 */
void Func_0808a248();          /* scene request keyed by a selector */
void Func_080770c8();          /* set a story flag */
void Func_02002640();          /* sibling item-28 owner, same overlay */
void Func_0808a360();
void Func_0808a020();          /* no-arg scene bracket, pairs with Func_0808a018 */

void Func_0200288c(s32 arg0)
{
    u8 *workspace;
    s32 selector;
    s32 *record;
    s32 predicate;
    s32 idBase;

    Func_0808a080(arg0);
    Func_0808a080(arg0);

    record = Func_0808a080(selector);
    selector = *(u32 *)&Data_02000240[500];

    Func_0808a018();

    Func_0808a170(0x2086);
    Func_0808a178(arg0, 0);

    workspace = *(u8 **)0x03001ebc;
    *(u16 *)(workspace + 0xcc2) = 0x2089;
    *(u16 *)(workspace + 0xcc4) = 4;

    predicate = Func_0808a070(selector, 0);
    if (predicate == 0) {
        Func_0808a170(0x2087);
        Func_0808a180(arg0, 0);

        *(u32 *)(workspace + 448) = 512;
        *(u32 *)(workspace + 456) = 15;
        Func_0808a368();
        Func_0808a370();

        Func_080770e8(idBase + 0x370, record[2] >> 20);
        selector++;
        Func_080770e8(idBase + 0x378, record[4] >> 20);
        idBase = selector << 4;

        if (selector > 3) {
            Func_0808a248(10);
            Func_080770c8(0x8d << 1);
        } else {
            Func_02002640(selector);
            Func_0808a360();
            Func_0808a370();
            *(u32 *)(workspace + 448) = predicate;
        }
        goto close_scene;
    }

    Func_0808a170(0x2088);
    Func_0808a180(arg0, 0);

close_scene:
    Func_0808a020();
}
