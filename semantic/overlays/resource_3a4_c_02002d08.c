typedef signed int s32;

/*
 * resource_3a4 owner at 0x02002d08, 78 bytes: a fixed sequence of
 * cutscene-primitive calls with literal arguments -- select entity
 * presentation 10/mode 2 four times interleaved with five other primitive
 * calls, then play sound cue 288.
 *
 * Complete owner: `push {lr}` at 0x02002d08 through the matching
 * interworking return `pop {r0} / bx r0` at 0x02002d54-0x02002d56. No
 * literal pool, no arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes eleven outgoing
 * calls over eight distinct targets, all resolved with the `+2` rule
 * against the raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools/overlay_show.ts resource_3a4 2d08 | bun
 * tools/overlay_call_targets.ts resource_3a4 2d08 2d56 --annotate`):
 *   0x2002d0e -> veneer -> Func_0808a428(24, 1)
 *   0x2002d16 -> veneer -> Func_0808a430(10, 9)
 *   0x2002d1a -> veneer -> Func_0808a448()
 *   0x2002d22, 0x2002d30, 0x2002d3c, 0x2002d50 -> veneer -> Func_0808a158(10, 2)   established, "select an entity presentation" (resource_3a8_c_020016bc.c, resource_372_c_020031ac.c)
 *   0x2002d28 -> veneer -> Func_0808a420(1)
 *   0x2002d34 -> veneer -> Func_0808a438()
 *   0x2002d40 -> veneer -> Func_0808a440()
 *   0x2002d48 -> veneer -> Func_080f9010(288)   established, play a sound cue (288 = 144 << 1)
 *
 * Func_0808a420/428/430/438/440/448 are an adjacent-address family not
 * established elsewhere in this queue's corpus; declared old-style, arity
 * per call site, no role asserted beyond the literal arguments observed.
 */

void Func_0808a428();
void Func_0808a430();
void Func_0808a448();
void Func_0808a158();          /* select an entity presentation, established */
void Func_0808a420();
void Func_0808a438();
void Func_0808a440();
void Func_080f9010();          /* play a sound cue, established */

void Func_02002d08(void)
{
    Func_0808a428(24, 1);
    Func_0808a430(10, 9);
    Func_0808a448();
    Func_0808a158(10, 2);
    Func_0808a420(1);
    Func_0808a158(10, 2);
    Func_0808a438();
    Func_0808a158(10, 2);
    Func_0808a440();
    Func_080f9010(288);
    Func_0808a158(10, 2);
}
