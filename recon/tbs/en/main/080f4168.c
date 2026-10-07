/* 080f4168 LuckyDice_Run: uncredited draft; listing 2046, draft 2032, differ 1644.
 * The shadow depth boundary now clobbers memory instead of taking the
 * stored depth as an operand. Both native stores remain, without the extra
 * address use: projection/die/depth recover r5/r6/r7,1658 ->1644. Frame768
 * and instruction count2032 are unchanged. Matrix-first still spills its
 * advancing value:2039/1773/frame772. Assigning point x/y/z gives2031/1641
 * but stores x before y unlike native; matrix-first with that order gives
 * 2039/1777/frame772. Those two source-order trials are not retained.
 * Shadow screen coordinates now have loop-local sx/sy lifetimes. Native
 * reads both coordinates before the visibility test; reading proj directly
 * delayed x and rematerialized the projection address. Locals restore those
 * loads and remove the extra address add:2033/1665 ->2032/1658, frame768.
 * With matrix-first packing, global x/y gives2043/1833/frame776, scoped
 * sx/sy2039/1797/frame772. A low point-pointer boundary before the loop
 * gives2041/1819; four ordinary projection cursors2038/1800, all four
 * constrained low2039/1805. Each still spills the matrix, frame772. Rejected.
 * Shadow matrix lifetime trials after the typed-point cleanup, none kept:
 * a plain local observed before projection gives2040/1810/frame772;
 * a high-register input triggers note_invalid_constants at arm.c:5414;
 * fixed r9 gives2036/1810/frame772, rebuilding the matrix value inside
 * each iteration and spilling attr2's offset rather than the point pointer.
 * Render_ProjectPoint now uses its current GFXUTIL.C/FIELD.H signature,
 * s32 * rather than void *, with the named x members passed as input.
 * All generated instructions remain identical. The neighboring projected
 * sprite renderer assigns its ground coordinates x/y/z; that order here
 * gives2032/1666, or2040/1810/frame772 with matrix-first packing. Not kept.
 * The projection input uses the existing FixedPointPosition type: its
 * verified x/y/z offsets0/4/8 and size12 fit this fixed-point input exactly.
 * Generated instructions are unchanged from point[3]. Matrix-first stays
 * 2040/1808 with that type; combine it with the native affine sl term:
 * 2042/1846/frame772, so no new register binding is retained.
 * Shadow follow-ups, none kept: observing the whole depth array in the
 * memory input2037/1805; matrix local inside the draw2040/1808/frame772;
 * named point pointer2039/1807, opaque before projection2041/1819, short
 * fixed-r4 copy2043/1821, ordinary register keyword2039/1807. The fixed
 * trial copies r4 back to a saved register rather than spilling around the
 * call. Unsigned inline matrix arithmetic is unchanged1665; distributing
 * the shift/add gives2035/1663 but constructs its extra constant inside
 * the loop, not native's advancing value. Function-scope mat2040/1808,
 * an explicit packed counter2040/1800, both frame772. Matrix-first OR also
 * restores its induction but spills2040/1808/frame772; matrix-last OR is
 * unchanged2033/1665. Native's point pointer, matrix and depth-address
 * lifetimes must be recovered together; no additional binding is kept.
 * Shadow depth keeps native's two stores: projected depth first, then depth
 * plus the die's height. Plain consecutive assignments delete the first
 * store. A memory-input boundary preserves it (2034/1670), but reloading
 * depth adds a load native lacks. Keep the intermediate in a local dep:
 * 2033/1665, frame768, both stores and the intervening add remain. The plain
 * dep form without the memory input is2033/1675, identical to no local.
 * A volatile first store instead is2039/1703. Hoisting the shadow matrix
 * index into a loop local restores its advancing value, but spills it and
 * enlarges the frame to772 (2038/1792); not kept. Native keeps that matrix
 * value in r9 and temporarily saves its point pointer across projection.
 * Keep squared distance in a separate local d2: native builds it directly
 * in the square-root argument r0, then keeps the resulting distance in r5.
 * Reusing dist kept both values in r5 and added an argument copy. Splitting
 * those units removes that instruction: 2034/1680 -> 2033/1675.
 * Further rejected trials from 1680: short resource increment bindings in
 * r0/r1 select native's registers but add palette-address materialization,
 * 2038/1720. A physics-condition source search (seed43, two jobs, interrupted
 * at 16 distinct candidates) only adds a redundant bool, differ1668 at the
 * same weighted26330. A two-minute spin-region search (seed47, two jobs,
 * 2841 candidates) gives weighted26260/differ1680 by moving the decrement
 * after the stopped check, opposite native's ordering. Neither is kept.
 * Empty and memory barriers before the variant addition are unchanged1680.
 * An opaque byte index at that access gives2034/1686 and restores base+index,
 * but the following decrement reconstructs its address instead of reusing
 * the cursor. Combine that with an explicit loop-local step pointer:
 * 2032/1706; with an independently advancing cursor:2031/1707. Not kept.
 * Resource setup now names its changing destination: decode into work RAM,
 * then DMA to VRAM. This restores native's repeated work-RAM address loads
 * instead of retaining that constant in r5; frame remains 768. A scoped
 * four-minute resource-source search (seed33, two jobs) reached 2033/1645,
 * but redundant casts and resource-result copy chains were removed; the
 * readable destination form is 2034/1680. One shared dst and two separate
 * destinations compile identically; initializing dst before base advances
 * is also identical. A post-decode r5..r11 clobber only moved the retained
 * constant to r7 (1731 -> 1729), so was rejected. Whole-function permute
 * cannot rewrite inline assembly; the source search excluded those blocks.
 * Affine trials from 1680, not kept: explicit halfword term before obj load
 * 2029/1663, but removes native's high-register copies and six loop instructions;
 * reuse global x 2025/1823; +h output triggers a reload_cse compiler ICE.
 * A scoped fixed sl term gives 2036/1706 and native's depth spill at sp+4,
 * but shifts all low pointer registers. Add r1 clobber: 2036/1666, restores
 * pointer registers but moves depth back to sp+32. Explicit base reloads
 * between each high-offset affine read/write: 2036/1700 with the clobber,
 * 2036/1714 without it. Explicit depth cursor: 2033/1765. Keep the verified
 * affine form while its remaining lifetime and spill ordering is unresolved.
 * Split affine pointer constraints: 1754 -> 1746, all four halfword
 * pointers and four object offsets now use native's registers. An input
 * instead of an output for pa alone was unchanged at 1754. Initialize
 * order[] with a two-iteration != loop: 2035/1731. GCC unrolls it and
 * puts the order base at native sp+44, restoring render+12 at48, flash52,
 * stopped56, projection base60. The affine depth cursor still spills at32
 * (native4), physics spin cursor at28 (native32), and its byte offset at24
 * (native28). Native's initial order[0] store retains i/r11; draft uses 0.
 * Order init with <2: 2038/1746, stores reverse and key reload returns;
 * sorting from i=0 instead of1: 2036/1738; retain init i with an asm input:
 * 2041/1771. Native's physics spin variant load uses base plus byte offset,
 * other accesses use a cursor. Local opaque base at that one access:
 * 2035/1741, but subsequent decrement becomes indexed too. Explicit step
 * pointer: 2033/1757; opaque step: 2031/1761; independent step cursor:
 * 2032/1756; step lifetime input: 2036/1892. None kept. Index inputs on the
 * two y paths: 2035/1757. Unused-output asm blocks: 2035/1731, identical
 * assembled diff to no device; native's unused index loads already survive
 * naturally. No extra CSE-block device is needed there.
 * Affine obj now has block scope: 1776 -> 1754; b/c/d halfword pointers
 * take native's r5/r4/r0. The first pointer and offset remain swapped,
 * r6/r7 instead of native's r7/r6. Sharing obj across the earlier object
 * loops remains better: scoping every copy shrinks the frame to 764 and
 * scores 2039/1935. Added affine input pa: 1766; pa at function scope:
 * 1766; fixed r7 temporary even with a regular copy-back corrupts the
 * depth giv in generated code, rejected. Explicit offa counter: 1758;
 * omit its low-register input: 1884 and frame 764; input-only pointer
 * constraints: 1758 with pa/pb/pc/pd in r7/r6/r5/r4 and offa in r0.
 * Input order does not change that; explicit offa with input-only gives
 * 1758 with the same register order as the output form. Not kept.
 * Fade-out counter increment moved before the second palette call or into
 * the scale expression: 1776 unchanged. A captured next counter before
 * WaitFrames: 1796, bound to r7: 1772; a loop-local scale: 1776. Not kept.
 * Fill count increment before the last object store: 1847, loses off1 giv.
 * Keep obj live through the count increment: 1830, fill body matches but
 * earlier allocation regresses; fill-local obj: 1847. Not kept.
 * Session 2026-10-07 (completion queue 2, affine continuation): the frame
 * now matches native's 768. Four ordinary halfword pointers constrained to
 * low registers, with the first object offset also low at the loop boundary,
 * restore the depth-pointer spill without hard-register variable overlap.
 * The depth pointer currently spills at sp+32, native sp+4. Giving each
 * object access its own loop-local copy adds native's repeated base loads:
 * 2036 instructions/differ 1776, from 2030/1892 at the previous checkpoint.
 * All 13 principal local spill slots and the arrays now have native offsets.
 * Trial: +m on affine obj: 2049/2023, wrong pointer slot; separate obj copies
 * alone: 2034/1904, frame 764. Taking &sprites and using it directly in the
 * affine loop: 2038/1820, but sprites split between sp+64 and sp+60.
 * Direct affine sprites without address-taking: 2005/1863, offset givs fold.
 * Hard-register pointer variables inside the loop also overlap offset givs;
 * earlyclobber inputs do not cure the generated value corruption. Rejected.
 * Ordinary low pointer constraints alone: 2032/1898, frame 764; add the low
 * first-offset input: 2034/1780, frame 768; separate object copies: 2036/1776.
 * Clobber r1: 2030/1756, but moves all four offsets high; making offa an
 * output: 2028/1766, but replaces its giv with a multiply each iteration.
 * Separate full-word zero temporaries: 2029/1767, immediate move and copy
 * instead of native's two pool loads. These lower-scoring forms are not kept.
 * The assembled affine zero loads are now both full-word pool loads, as
 * native; the compiler's .s spells ldrh but the assembled diff shows ldr.
 * Remaining affine differences: halfword-address register order, base
 * reloads during each read-modify-write, and the depth-pointer spill's
 * placement. Both affine loop backedges are -146 bytes; the whole native
 * function still has 10 more instructions.
 * Previous checkpoint: the frame was 764, native 768;
 * the initial draft was 1974 instructions, differ 1918, frame 756.
 * Pointer declarations follow their allocation order again, followed by
 * frame/state/bet/timer/kind/winnings/counter/digits/count: their spill slots
 * followed native's order, each at native minus 4. The missing word was
 * the affine depth-pointer spill, not another unused local.
 * The opening tile loop is instruction-exact: i counts bytes, row and j
 * have block scope, and the tagged dst/step constraints keep the address
 * add outside strh and restore native's r2/r7 choices. Direct byte indexing
 * with block-local counters was differ 1938; separating the add was 1932;
 * dst in r2 was 1872, dst/step in r2/r7 was 1886 but made the loop exact.
 * Advancing base before both resource decodes is the native source form
 * (1886 unchanged, 1978 -> 1980 instructions); do-while main: unchanged.
 * loop.c has two independent obstacles at each object loop: it folds the
 * invariant sprites pointer into DEST_ADDR givs, then derives sequential
 * offset givs from one another if their lifetimes do not overlap. Compute
 * both/four named-field offsets first, then make a loop-local copy obj of
 * sprites opaque through +rm. The offsets overlap, obj stays in the loop,
 * and sprites remains invariant outside it, so help_window can stay hoisted.
 * Faces/clear/affine loops: 2008 instructions, differ 1888, frame 756;
 * all object loops: 2024, differ 1908, frame 764. Scoped stopped: unchanged.
 * +r on sprites itself added a spill store (1981); +m moved it to a local
 * frame object (2021); volatile integer sprite base was 2065. +rm on sprites
 * itself removed parent-loop invariants and shrank the frame: not kept.
 * The two discarded gKeysRepeat reads are present in the listing before
 * gKeysHeld. (void) expressions give native's r7/r0/r3 load sequence exactly
 * where assignments to key did not: 2025 instructions, differ 1911.
 * EN.PO proves Coins 02310, Winnings 02311, Throw 02320, Quit 02321. Keeping
 * Coins as a linker symbol and using +1/+10/+11 restores native's shared
 * pool base without redefining any message name: 2030, differ 1892.
 * Named-field OBJ_OFF expressions compile identically to arithmetic offsets.
 * The affine loop still keeps its advancing depth pointer in r0 and obj in
 * r4; native spills the former at sp+4, reloads sprites, and keeps its four
 * halfword pointers in r7/r5/r4/r0. Fixed-register pointer trials were not
 * kept: direct hard bindings let loop.c reuse a hard register; copying them
 * back into ordinary pointers avoided that but added five copies (1881).
 * Broad permute (4 min, 2 jobs, seed 19, --heat), from the initial draft:
 * best weighted 48620/differ 1791, second 48720/1766. Neither kept: 93-96
 * rewrites, including unrelated shared temporaries and redundant copy chains;
 * a lower score there did not establish a readable native source form.
 * Remaining: one spill word; affine allocation and zero loads; shared
 * resource scratch constants; other register choices and message scheduling.
 * Earlier attempts:
 * Restore compilation with the current RuntimeDispatch_NoOpHook declaration.
 * Use the verified BattleEffectWork/EffectStep, BattleCamera, GameState and
 * map-scroll layouts, existing hardware/IWRAM declarations and whole messages.
 * The minimally repaired private-layout draft differed 1938 (1978 instructions).
 * Remove its unsupported unused s32[8], volatile affine locals and discarded
 * key reads rather than retaining unproven steering. The natural frame is 716
 * bytes; native is 768. Loop/register allocation and frame layout remain open.
 * Stubborn (2026-10-06): re-scored differ 1977; not first-try. Score+parked from sun100 queue-3.
 * Native locals, top down: palette 256, depth 240, results 224, point 212,
 * proj 200, 32 unused bytes 168..200, affine halfwords a..d at 166..160
 * (each strh then reloaded with ldrh), spin 152, flash 144, faces 136,
 * order 128, matches 120; spills begin at 116 (obj_palette, work, sprites,
 * render at 116..104). The ldr of 0 for affine_b/affine_c is a pool load.
 * Trial (2026-10-06): volatile affine_a..d plus unused s32[8] after proj,
 * both FAKEMATCH-tagged: differ 1977 -> 1966, frame 716 -> 756. All locals
 * now sit at native offsets minus 12; the 12 bytes are most likely three
 * more spill slots below the four pointer spills (register allocation).
 * Native's affine loop keeps four offset givs (204, 212, 220, 228, +32 each)
 * and reloads sprites per access; the draft forms one sprites+204 pointer.
 * Trial: tile loop as [i++] = j + row * 30: differ 1966, unchanged.
 * Trial: sprites as a volatile pointer: differ 2099, worse.
 * Trial: affine loop through x = i * 4 and obj[x + k]: differ 1966, unchanged.
 * Session 2026-10-06 21h: one shared `die` pointer lives across the whole
 * function, so global.c ranks it below the hoisted 127/31 constants; native
 * gives each particle loop a fresh giv (r5, then 127 in r6, 31 in r7).
 * Trial: init loop through work->particles[i]: 1966 -> 1939 (loop matches).
 * Trial: bet loop (state 0, KEY_A) indexed too: 1944, worse; kept on die.
 * Trial: state 0 marker loop indexed: 1939 -> 1931. Shadow loop: -> 1930.
 * Trial: physics loop indexed: 2446, far worse; kept on die.
 * Trial: tile loop as row * 30 + j, as < bounds, through tile = row * 30,
 * as *(u16 *)(0x06003000 + i * 2), through pos: 1930..1932; as x/y: 1964;
 * as byte offset i += 2: 1966. Pseudo order alone moves 30+ instructions.
 * Trial: greedy declaration order (one line moved per round): render first,
 * sprites and obj_palette last: 1930 -> 1918; no single move improves more.
 * Trial: fill loop as ((u32 *)sprites)[50 + count * 2] and through a u32
 * w[] member at offset 0: 1918, identical code. struct *volatile sprites: 2053.
 * Open: native's sprites->obj[count] loops (fill, state 0 markers, affine)
 * keep offset givs that include every constant (count * 8 + 200, + 204) and
 * add sprites per access ([base, off], base reloaded from sp+108); the draft
 * folds sprites into one pointer giv. loop.c folds any invariant register
 * into a giv (simplify_giv_expr returns it as USE); combine_givs_p can then
 * express an address giv as (plus offset_giv sprites) only when no constant
 * is left outside the offset. Find the source form that puts the field
 * offset inside the index; that likely also explains native's 3 extra spills.
 * Parallel session (2026-10-06, T14), its best body differ 1945 not kept; its trials:
 * Session 2026-10-06 (T14): opcode-level LCS (registers and offsets ignored)
 * now matches 1774 of 2046; the rest of the differ is allocation and frame.
 * Trial: the kind loop counts with i, as native's r11 does for every loop
 * that holds no other i: 1966 -> 1964.
 * Trial: locals declared frame, state, bet, timer, kind, winnings, counter,
 * digits, count, then die/other: spill slots take native's order (each at
 * native minus 12): 1964, unchanged.
 * Trial: messages as numbers (MSGNO): 1964 -> 1959. Native loads one pool
 * constant and derives the others (Coins = Winnings - 1, Throw + 1 = Quit,
 * Coins + 10, Coins + 11): CSE's related-value reuse of one base symbol, so
 * all four are written MsgLuckyDiceCoins + 0/1/10/11: 1959 -> 1941.
 * Trial: the two discarded gKeysRepeat loads native makes before the KEY_UP
 * test restored as key = gKeysRepeat twice: 1941 -> 1945; the dead loads get
 * r0 (native r3), so sched2 cannot hoist the gKeysHeld address load above
 * them. Kept: the loads are in the bytes. key = gKeysHeld as a third: 1945.
 * Trial: tile loop inner counter x instead of j: 1990, worse.
 * Trial: tile loop as *(u16 *)(0x06003000 + i) with i += 2: fp holds i as
 * native, but j moves to sl and the store becomes reg+reg: 2056, worse.
 * Spills (greg): both spill the 13 locals obj_palette..count first; the draft
 * then spills stopped, a hoisted 0, &help_window, &flash, &faces, &spin,
 * render+12 and copies; native has three more slots, render+12 between &flash
 * and &faces, two unknown slots between &help_window and &flash.
 * Open, likely the main cause: in every obj loop (faces, results, kind,
 * shadows, clear, affine) native keeps offset givs (count * 8 + 200 ...) and
 * addresses sprites (reloaded from its slot) + giv, reg+reg; the draft folds
 * sprites into one pointer giv with immediate offsets. Native's faces loop
 * reloads sprites inside the loop and keeps separate attr01/attr2 offset givs
 * (240, 244), so for native's loop pass sprites was not an invariant pseudo.
 * Trial: sprites address taken ((void)&sprites): load still hoisted, pointer
 * giv kept: 2038, worse. Same as u32 spr cast at each use: 2061, worse.
 * Native keeps stopped at sp+56, three slots below count (68), after
 * &help_window (64) and one more temporary: its pseudo is made after the
 * init code, as a block-scoped local would be. Trial: s32 stopped declared
 * inside if (state == 1): the draft's slot moves to count - 12 as native's,
 * differ 1947 (+2). Trial: dx, dy, dz, dist inside if (i != j): 1947, same.
 * Trial: die/other declared after render, or j before i: 1945, unchanged.
 * Lead to test (loop.c, simplify_giv_expr): a register made by the loop pass
 * itself (>= max_reg_before_loop), or one not invariant in the inner loop,
 * cannot enter a giv. The draft's loop dump processes loops last first, inner
 * before outer, so a copy hoisted from the main loop would come too late;
 * look for what makes sprites variant or new at the obj loops.
 * Session 2026-10-07 (S16): differ 1918 re-scored. --rtl on the fill loop:
 * the gcse dump already has native's shape (reg = count * 8 + 200 and
 * count * 8 + 204, stores to (plus sprites reg)); loop.c then records both
 * stores as DEST_ADDR givs with add (plus sprites 200/204), because
 * loop_invariant_p (sprites) == 1 (set_in_loop 0), combines them and
 * reduces to one pointer giv. Native kept only the two DEST_REG givs, so
 * in native's loop pass sprites was not invariant (set in that loop, or a
 * MEM load inside it that could not be hoisted). Every C spelling of the
 * address gives the same RTL; the cause is sprites' value, not the index.
 * Trial: *(u32 *)((u8 *)sprites + (count * 8 + 200)) fill loop: 1918, same.
 * Trial: affine_a..d as u16 affine[4] (no volatile): 1928, worse.
 * Trial (measurement only, not source): &sprites passed to an extern call:
 * 1992; sprites becomes a frame MEM, loop hoists its load and still folds
 * the fill loop; the affine loop gains native's two pool loads of 0.
 * Trial: render, sprites, work, obj_palette as one local struct: 2394;
 * the load is hoisted and folded as well.
 * Trial: struct *volatile sprites: the gcse load is not mem/v and is
 * hoisted (2053). Native also keeps window field addresses as pointers
 * (&sprites->help_window at sp+64), formed once and reused.
 */
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "UI.H"
#include "GAME_STATE.H"
#include "MAP_SCROLL.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLDISP.H"
#include "IWRAM_CALL.H"
#include "TRANSFORM.H"
#include "FIXED_POINT_POSITION.H"
#include "RESOURCE_IDS.H"
#include "text/MSG_IDS.H"

struct LuckyDiceObj {
    u32 attr01;
    u32 attr2;
};

struct LuckyDiceSprites {
    u8 filler[200];
    struct LuckyDiceObj obj[128];
    s32 coins_window;
    s32 winnings_window;
    s32 help_window;
};

#define OBJ_OFF(n, field) ((u32)&((struct LuckyDiceSprites *)0)->obj[n].field)

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern u8 gOamCopyEnabled;
extern u8 Data_080f53fc[];
extern u8 Data_080f5400[];
extern u8 Data_080f5408[];
extern s16 Data_080f541a[];
extern char ResourceId_EffectFarCalls;
extern char ResourceId_LuckyDiceBoard;
extern char ResourceId_LuckyDiceSprites;

void Camera_ResetSceneDefaults(void);
void Scheduler_ResetTaskTable(void);
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count);
void Render_ResetTransformState(void);
s32 Render_ProjectPoint(s32 *point, s32 *out);
u32 __umodsi3(u32 numerator, u32 denominator);
void FarCall_WindowTable(void);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Party_AdjustCoinsFar(s32 amount);
s32 Party_GetAverageLevelFar(void);
void Audio_PlayCue(s32 cue);

void LuckyDice_Run(void)
{
    u16 palette[256];
    s32 depth[4];
    s32 results[4];
    struct FixedPointPosition point;
    s32 proj[3];
    /* FAKEMATCH: native frame keeps 32 unused bytes at sp+168..200, between proj and the affine halfwords */
    s32 unused[8];
    /* FAKEMATCH: native stores the four halfwords to sp+166..160 and reloads each with ldrh */
    volatile u16 affine_a;
    volatile u16 affine_b;
    volatile u16 affine_c;
    volatile u16 affine_d;
    s32 spin[2];
    s32 flash[2];
    s32 faces[2];
    s32 order[2];
    s32 matches[2];
    u16 *obj_palette;
    struct BattleEffectWork *work;
    struct LuckyDiceSprites *sprites;
    struct LuckyDiceSprites *obj;
    struct BattleCamera *render;
    struct EffectStep *die;
    struct EffectStep *other;
    s32 frame;
    s32 state;
    s32 bet;
    s32 timer;
    s32 kind;
    s32 winnings;
    s32 counter;
    s32 digits;
    s32 count;
    s32 reset;
    s32 window;
    s32 i;
    s32 j;
    s32 n;
    s32 x;
    s32 y;
    s32 z;
    s32 d;
    s32 e;
    s32 tile;
    s32 key;
    s32 row;
    s32 base;
    s32 pos;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 speed;
    u32 angle;
    s32 scale;
    s32 pairs;
    s16 value;
    void *dst;

    Runtime_AllocateHeapBlock(41, 0x60e);
    obj_palette = Runtime_AllocateHeapBlock(40, 0x200);
    work = Runtime_AllocateBlock(39, 0x782c);
    sprites = Runtime_AllocateBlock(45, 0x618);
    render = Runtime_AllocateBlock(12, 76);
    ((void (*)(s32))RuntimeDispatch_NoOpHook)((s32)&ResourceId_EffectFarCalls);
    Camera_ResetSceneDefaults();
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = 0;

    i = 0;
    {
        s32 row;
        s32 j;

        for (row = 0; row != 20; row++) {
            for (j = 0; j != 32; j++) {
                /* FAKEMATCH: native holds the separate tile address in r2; direct indexing folds the add into strh. */
                register u16 *dst asm("r2") = (u16 *)(0x06003000 + i);
                /* FAKEMATCH: native holds the two-byte stride in r7 before the store. */
                register s32 step asm("r7") = 2;
                /* FAKEMATCH: keep the address add separate and restore native's r2/r7 scheduling. */
                __asm__("" : "+r"(dst) : "r"(step));
                *dst = j + row * 30;
                i += step;
            }
        }
    }

    base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
    Dma_Set((void *)base, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, palette, 0x84000080, (volatile u32 *)0x040000d4);
    base += 0x1c0;
    dst = (void *)0x02010000;
    Resource_DecodeType01((void *)base, dst);
    dst = (void *)0x06004000;
    Dma_Set((void *)0x02010000, dst, 0x84002580, (volatile u32 *)0x040000d4);
    base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
    Dma_Set((void *)base, obj_palette, 0x84000080, (volatile u32 *)0x040000d4);
    Dma_Set((void *)base, (void *)0x05000200, 0x84000080, (volatile u32 *)0x040000d4);
    base += 0x200;
    dst = (void *)0x02010000;
    Resource_DecodeType01((void *)base, dst);
    dst = (void *)0x06010000;
    Dma_Set((void *)0x02010000, dst, 0x84001f00, (volatile u32 *)0x040000d4);
    Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, 0, 256);
    Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, 0, 256);

    REG_BG1CNT = 0x0686;
    REG_WININ = 0x3737;
    REG_WINOUT = 0x2723;
    REG_DISPCNT = 0x3340;
    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x0810;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0xff60;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0xff60;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0xa0;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0xa0;

    state = 0;
    timer = 0;
    kind = -1;
    winnings = 0;
    counter = 0;
    for (i = 0; i != 2; i++) {
        work->particles[i].x = ((Random16() & 127) - 64) << 16;
        work->particles[i].y = (Random16() & 31) << 16;
        work->particles[i].z = ((Random16() & 127) - 64) << 16;
        work->particles[i].velocity_x = ((Random16() & 127) - 64) << 14;
        work->particles[i].velocity_y = ((Random16() & 31) + 32) << 12;
        work->particles[i].velocity_z = ((Random16() & 127) - 64) << 14;
        work->particles[i].variant = 0;
        spin[i] = 300;
    }
    results[0] = results[1] = results[2] = results[3] = 0;
    flash[0] = flash[1] = 0;
    faces[0] = __umodsi3(Random16(), 6);
    do {
        faces[1] = __umodsi3(Random16(), 6);
    } while (faces[0] == faces[1]);
    work->particles[0].x = 0x280000;
    work->particles[0].y = 0x600000;
    work->particles[0].z = -0xa00000;
    work->particles[1].x = -0x280000;
    work->particles[1].y = 0x600000;
    work->particles[1].z = -0xa00000;

    FarCall_WindowTable();
    window = UiWindow_CreateFar(18, 0, 12, 3, 6);
    sprites->coins_window = window;
    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins, window, 48, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
    sprites->help_window = window;
    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);

    frame = 0;
    do {
        reset = 0;
        if (frame <= 16) {
            Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, frame << 12, 256);
            Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, frame << 12, 256);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(render->eye, render->pos);

        count = 0;
        sprites->obj[count].attr01 = 0xc0006000;
        sprites->obj[count].attr2 = 0x358;
        count++;
        sprites->obj[count].attr01 = 0x40102020;
        sprites->obj[count].attr2 = 0x398;
        count++;
        sprites->obj[count].attr01 = 0x40102030;
        sprites->obj[count].attr2 = 0x3a0;
        count++;
        sprites->obj[count].attr01 = 0x40042020;
        sprites->obj[count].attr2 = 0x3a8;
        count++;
        sprites->obj[count].attr01 = 0x40042030;
        sprites->obj[count].attr2 = 0x3a8;
        count++;
        for (i = 0; i != 2; i++) {
            u32 off0 = OBJ_OFF(count, attr01);
            u32 off1 = OBJ_OFF(count, attr2);

            obj = sprites;
            /* FAKEMATCH: native keeps the object base inside the loop and the two object offsets simultaneously live. */
            __asm__ volatile("" : "+rm"(obj));
            *(u32 *)((u8 *)obj + off0) = (32 + i * 16) | 0x40202000;
            *(u32 *)((u8 *)obj + off1) = faces[i] * 8 + 0x3b0;
            count++;
        }

        if (state == 2 || state == 3) {
            for (i = 0; i != 4; i++) {
                if (timer == i * 5) {
                    Audio_PlayCue(0x12f);
                }
                if (timer >= i * 5) {
                    u32 off0 = OBJ_OFF(count, attr01);
                    u32 off1 = OBJ_OFF(count, attr2);
                    obj = sprites;
                    /* FAKEMATCH: native retains independent object offsets. */
                    __asm__ volatile("" : "+rm"(obj));
                    *(u32 *)((u8 *)obj + off0) = 0x80002040 | ((56 + i * 32) << 16);
                    *(u32 *)((u8 *)obj + off1) = results[i] * 32 + 152;
                    count++;
                }
            }
            if (timer > 19 && kind >= 0) {
                n = Data_080f53fc[kind];
                if (n != 0) {
                    tile = Data_080f5400[kind] * 8 + 152;
                    x = 0x278 - n * 16;
                    for (j = 0; j != n; j++) {
                        u32 off0 = OBJ_OFF(count, attr01);
                        u32 off1 = OBJ_OFF(count, attr2);
                        obj = sprites;
                        /* FAKEMATCH: native retains independent object offsets. */
                        __asm__ volatile("" : "+rm"(obj));
                        *(u32 *)((u8 *)obj + off0) = (((x & 0x1ff) << 16) | 96) | 0x80002000;
                        *(u32 *)((u8 *)obj + off1) = tile;
                        count++;
                        tile += 32;
                        x += 32;
                    }
                }
            }
        }

        if (state == 2) {
            if (timer == 20 && winnings > 0) {
                Party_AdjustCoinsFar(winnings);
                digits = 1;
                if (winnings > 9) {
                    digits = 2;
                    if (winnings > 99) {
                        digits = 3;
                        if (winnings > 999) {
                            digits = 4;
                            if (winnings > 9999) {
                                digits = 5;
                            }
                        }
                    }
                }
                window = UiWindow_CreateFar(9 - digits / 2, 16, digits + 12, 3, 6);
                sprites->winnings_window = window;
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 1, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins, sprites->winnings_window, digits * 8 + 48, 0);
                UiText_DrawNumberInWindowFar(winnings, digits, sprites->winnings_window, 48, 0);
            }
            if (counter > 0) {
                counter--;
            }
            if (timer > 19 && (gKeysRepeat & KEY_A)) {
                Audio_PlayCue(112);
                if (winnings > 0) {
                    state = 3;
                    counter = 0;
                } else {
                    state = 0;
                    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
                    sprites->help_window = window;
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);
                    reset = 1;
                }
            }
            timer++;
        } else if (state == 3) {
            if (counter == 0) {
                winnings = 0;
                UiText_DrawNumberInWindowFar(0, digits, sprites->winnings_window, 40, 0);
                UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
            }
            if (counter == 15) {
                state = 0;
                UiWork_FinalizeFar(sprites->winnings_window, 1);
                window = UiWindow_CreateFar(22, 16, 8, 4, 6);
                sprites->help_window = window;
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);
                reset = 1;
            }
            counter++;
        } else if (state == 0 && frame > 17) {
            /* FAKEMATCH: native retains two unused key-repeat reads before
             * the held-key test; the blocks that used them have no code left. */
            (void)gKeysRepeat;
            (void)gKeysRepeat;
            if (gKeysHeld & KEY_UP) {
                work->particles[0].y += 0x40000;
                work->particles[1].y += 0x40000;
            }
            if (gKeysHeld & KEY_DOWN) {
                work->particles[0].y += -0x40000;
                work->particles[1].y += -0x40000;
            }
            if (work->particles[0].y < 0x200000) {
                work->particles[0].y = 0x200000;
            }
            if (work->particles[0].y > 0xa00000) {
                work->particles[0].y = 0xa00000;
            }
            if (work->particles[1].y < 0x200000) {
                work->particles[1].y = 0x200000;
            }
            if (work->particles[1].y > 0xa00000) {
                work->particles[1].y = 0xa00000;
            }
            if (gGameState.coins < (u32)(Party_GetAverageLevelFar() * 10)) {
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & KEY_B) {
                Audio_PlayCue(113);
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & KEY_A) {
                Audio_PlayCue(0x12e);
                UiWork_FinalizeFar(sprites->help_window, 1);
                state = 1;
                bet = Party_GetAverageLevelFar() * 10;
                if ((u32)bet > gGameState.coins) {
                    bet = gGameState.coins;
                }
                Party_AdjustCoinsFar(-bet);
                UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                die = work->particles;
                for (i = 0; i != 2; i++) {
                    if (i > 1) {
                        die->z = -0xb40000;
                        die->y = 0x600000;
                    }
                    die->velocity_x = ((Random16() & 127) - 64) << 12;
                    die->velocity_y = ((Random16() & 15) + 48) << 13;
                    n = Random16() & 63;
                    die->velocity_z = ((n + 140) << 12) + __divsi3(die->y, 6);
                    die->variant = __umodsi3(Random16(), 0x4800);
                    spin[i] = 300;
                    die++;
                }
            }
        }

        if (reset == 1) {
            work->particles[0].x = 0x280000;
            work->particles[0].y = 0x600000;
            work->particles[0].z = -0xa00000;
            work->particles[1].x = -0x280000;
            work->particles[1].y = 0x600000;
            work->particles[1].z = -0xa00000;
            faces[0] = __umodsi3(Random16(), 6);
            do {
                faces[1] = __umodsi3(Random16(), 6);
            } while (faces[0] == faces[1]);
        }

        for (i = 0; i != 2; i++) {
            order[i] = i;
        }
        for (i = 1; i < 2; i++) {
            key = order[i];
            for (j = i - 1; j >= 0 && work->particles[order[j]].z > work->particles[key].z; j--) {
                order[j + 1] = order[j];
            }
            order[j + 1] = key;
        }

        for (i = 0; i != 2; i++) {
            die = &work->particles[order[i]];
            Render_ProjectPoint(&die->x, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            depth[order[i]] = proj[2] - 0x15e;
            d = __divsi3(0x800, depth[order[i]] / 2 + 128);
            x = proj[0] - d;
            y = proj[1] - d;
            if (y + 96 > -16) {
                u32 off0 = OBJ_OFF(count, attr01);
                u32 off1 = OBJ_OFF(count, attr2);
                obj = sprites;
                /* FAKEMATCH: native retains independent object offsets. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + off0) = ((order[i] << 25) | (((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x40002300;
                *(u32 *)((u8 *)obj + off1) = (Data_080f5408[die->variant / 1024] << 3) | 0x400;
                count++;
            }
        }

        if (state == 0) {
            for (i = 0; i != 2; i++) {
                u32 off0 = OBJ_OFF(count, attr01);
                u32 off1 = OBJ_OFF(count, attr2);

                Render_ProjectPoint(&work->particles[i].x, proj);
                n = depth[order[i]] / 2 + 128;
                x = proj[0] - __divsi3(0x800, n);
                y = proj[1] + __divsi3(0x300, n);
                obj = sprites;
                /* FAKEMATCH: native retains independent object offsets. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + off0) = ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
                if (i == 1) {
                    *(u32 *)((u8 *)obj + off0) |= 0x10000000;
                }
                *(u32 *)((u8 *)obj + off1) = 0x958;
                count++;
            }
        }

        for (i = 0; i != 2; i++) {
            s32 dep;
            s32 sx, sy;

            point.y = 0;
            point.x = work->particles[i].x;
            point.z = work->particles[i].z;
            Render_ProjectPoint(&point.x, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            dep = proj[2] - 0x15e;
            depth[2 + i] = dep;
            /* FAKEMATCH: native stores projected depth before adding height; plain C deletes that store. Memory clobber keeps the store without an address operand, restoring r5/r6/r7 (1658 to1644). */
            __asm__ volatile("" : : : "memory");
            dep += *((s16 *)&work->particles[i].y + 1);
            depth[2 + i] = dep;
            sx = proj[0];
            sy = proj[1];
            if (sy + 88 > -16) {
                u32 off0 = OBJ_OFF(count, attr01);
                u32 off1 = OBJ_OFF(count, attr2);
                obj = sprites;
                /* FAKEMATCH: native retains independent object offsets. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + off0) = ((((sx + 0x270) & 0x1ff) << 16) | ((i + 2) << 25)) | ((sy + 0x158) & 255) | 0x40002300;
                *(u32 *)((u8 *)obj + off1) = 0x890;
                count++;
            }
        }

        if (state == 1) {
            s32 stopped;

            stopped = 0;
            die = work->particles;
            for (i = 0; i != 2; i++) {
                die->x += die->velocity_x;
                die->y += die->velocity_y;
                die->z += die->velocity_z;
                other = work->particles;
                for (j = 0; j != 2; j++) {
                    if (i != j) {
                        s32 d2;

                        dx = (die->x - other->x) >> 16;
                        dy = (die->y - other->y) >> 16;
                        dz = (die->z - other->z) >> 16;
                        d2 = dx * dx + dy * dy + dz * dz;
                        if (d2 <= 0x9c3) {
                            dist = Iwram_Sqrt(d2);
                            die->velocity_x += __divsi3(dx << 15, dist);
                            die->velocity_y += __divsi3(dy << 15, dist);
                            die->velocity_z += __divsi3(dz << 15, dist);
                        }
                    }
                    other++;
                }
                if (die->x < -0xc80000) {
                    die->x = -0xc80000;
                    die->velocity_x = -die->velocity_x * 48 / 64;
                }
                if (die->x > 0xc80000) {
                    die->x = 0xc80000;
                    die->velocity_x = -die->velocity_x * 48 / 64;
                }
                if (die->z < -0x960000) {
                    die->z = -0x960000;
                    die->velocity_z = -die->velocity_z * 48 / 64;
                }
                if (die->z > die->y + 0xb40000) {
                    die->z = die->y + 0xb40000;
                    if (die->velocity_z > 0) {
                        die->velocity_y += die->velocity_z / 2;
                        die->velocity_z = -die->velocity_z * 48 / 64;
                    }
                }
                die->velocity_y += -0x8000;
                if (die->y <= 0) {
                    die->y = 0;
                    die->velocity_y = -die->velocity_y * 48 / 64;
                    if ((Random16() & 3) == 0) {
                        speed = Iwram_Sqrt((die->velocity_x >> 8) * (die->velocity_x >> 8) + (die->velocity_z >> 8) * (die->velocity_z >> 8)) * 56 / 64;
                        angle = Random16() & 0xffff;
                        die->velocity_x = speed * Trig_Sin(angle) >> 8;
                        die->velocity_z = speed * Trig_Cos(angle) >> 8;
                    } else {
                        die->velocity_x = die->velocity_x * 56 / 64;
                        die->velocity_z = die->velocity_z * 56 / 64;
                    }
                    if (spin[i] > 19) {
                        spin[i] -= 20;
                    }
                    if (die->velocity_y < 0x30000) {
                        die->velocity_x = 0;
                        die->velocity_y = 0;
                        die->velocity_z = 0;
                        spin[i] = 0;
                        n = die->variant;
                        switch (__modsi3(n / 1024, 3)) {
                        case 1:
                            die->variant = n + 0x800;
                            break;
                        case 2:
                            die->variant = n + 0x400;
                            break;
                        }
                    }
                }
                die->velocity_x = die->velocity_x * 63 / 64;
                die->velocity_y = die->velocity_y * 63 / 64;
                die->velocity_z = die->velocity_z * 63 / 64;
                die->variant += spin[i];
                if (die->variant > 0x47ff) {
                    die->variant -= 0x4800;
                }
                if (spin[i] > 0) {
                    spin[i]--;
                }
                if (die->velocity_x == 0 && die->velocity_y == 0 && die->velocity_z == 0 && die->y == 0) {
                    results[i] = __divsi3(die->variant, 0xc00);
                    for (j = 0; j != 6; j++) {
                        if (die->z > Data_080f541a[j] << 16) {
                            results[i + 2] = j;
                            break;
                        }
                    }
                    stopped++;
                }
                die++;
            }

            if (stopped == 2) {
                state = 2;
                timer = 0;
                kind = -1;
                winnings = 0;
                counter = 60;
                if (results[0] == results[1] && results[0] == results[2] && results[0] == results[3]) {
                    flash[0] = flash[1] = 120;
                    kind = 0;
                    if (results[0] == faces[0]) {
                        winnings = bet * 15;
                    } else if (results[0] == faces[1]) {
                        winnings = bet * 25;
                    } else {
                        winnings = bet * 5;
                    }
                    UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                    Audio_PlayCue(93);
                } else {
                    pairs = 0;
                    for (i = 0; i != 3; i++) {
                        for (j = i + 1; j != 4; j++) {
                            if (results[i] == results[j]) {
                                matches[pairs] = results[i];
                                pairs++;
                            }
                        }
                    }
                    if (pairs == 1) {
                        winnings = bet;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        kind = 3;
                    }
                    if (pairs == 2) {
                        winnings = bet * 2;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        flash[Random16() & 1] = 60;
                        kind = 2;
                        Audio_PlayCue(91);
                    }
                    if (pairs == 3) {
                        winnings = bet * 3;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        flash[Random16() & 1] = 60;
                        kind = 1;
                        Audio_PlayCue(92);
                    }
                }
            }
        }

        for (; count != 128; count++) {
            u32 off0 = OBJ_OFF(count, attr01);
            u32 off1 = OBJ_OFF(count, attr2);

            obj = sprites;
            /* FAKEMATCH: native keeps the object base inside the loop and the two object offsets simultaneously live. */
            __asm__ volatile("" : "+rm"(obj));
            *(u32 *)((u8 *)obj + off0) = 0x40f02000;
            *(u32 *)((u8 *)obj + off1) = 0;
        }

        {
            struct LuckyDiceSprites *obj;
            volatile u16 *pa = &affine_a;
            volatile u16 *pb = &affine_b;
            volatile u16 *pc = &affine_c;
            volatile u16 *pd = &affine_d;

            for (i = 0; i != 4; i++) {
                u32 offa = OBJ_OFF(i * 4 + 0, attr2);
                u32 offb = OBJ_OFF(i * 4 + 1, attr2);
                u32 offc = OBJ_OFF(i * 4 + 2, attr2);
                u32 offd = OBJ_OFF(i * 4 + 3, attr2);

                /* FAKEMATCH: native's first halfword pointer is r7; a separate boundary swaps it with the first offset as native does (1754 to 1746). */
                __asm__ volatile("" : "+l"(pa));
                /* FAKEMATCH: native keeps the other three pointers and first offset low; restores the depth-pointer spill and 768-byte frame (764 without). */
                __asm__ volatile("" : "+l"(pb), "+l"(pc), "+l"(pd) : "l"(offa));
                value = depth[i] / 2 + 128;
                *pa = value;
                *pb = 0;
                *pc = 0;
                *pd = value;
                obj = sprites;
                /* FAKEMATCH: native reloads the object base for each affine field. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + offa) |= *pa << 16;
                obj = sprites;
                /* FAKEMATCH: native reloads the object base for each affine field. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + offb) |= *pb << 16;
                obj = sprites;
                /* FAKEMATCH: native reloads the object base for each affine field. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + offc) |= *pc << 16;
                obj = sprites;
                /* FAKEMATCH: native reloads the object base for each affine field. */
                __asm__ volatile("" : "+rm"(obj));
                *(u32 *)((u8 *)obj + offd) |= *pd << 16;
            }
        }

        Dma_Set(sprites->obj, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
        WaitFrames(1);
        frame++;
    } while (1);

    for (i = 0; i != 17; i++) {
        scale = 0x10000 - (i << 12);
        Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, scale, 256);
        Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, scale, 256);
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(12);
    Runtime_ReleaseHeapBlock(45);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}
