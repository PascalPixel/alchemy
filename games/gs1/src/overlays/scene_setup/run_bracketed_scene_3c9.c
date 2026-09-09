#include "types.h"

extern u8 *Data_03001ebc;

/*
 * One symbol per call site, spelled as the raw pc-relative decode of the
 * reference halfwords rather than the semantic import address. Listed in
 * call order.
 */
void Func_02009614();  /* Func_0808a018 */
void Func_020081bc();  /* Func_020048d8, this overlay's own */
void Func_02008410();  /* Func_02004b28, this overlay's own */
void Func_02009604();  /* GameFlag_Set */
void Func_020097a6();  /* Func_0808a368 */
void Func_020097b2();  /* Func_0808a370 */
void Func_02009770();  /* Func_0808a248 */
void Func_02009654();  /* Func_0808a020 */

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void FieldScene_RunBracketedSceneWithFlag282(void)
{
    u8 *workspace;

    Func_02009614();
    Func_020081bc();
    Func_02008410();
    /* The flag id is built as 141 << 1 rather than folded. */
    Func_02009604(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Func_020097a6();
    Func_020097b2();
    Func_02009770(1);
    Func_02009654();
}
