/* NONMATCHING (number-bound): resource_38f at 0x020081ac (40 bytes with its
 * pool), SceneData_SelectTableB5b8ByState; twins resource_38b:0x020080dc,
 * resource_3c4:0x020092b0 and resource_3c5:0x02008fac.
 *
 * Remaining difference: the reference loads scene number 0x26 from its
 * literal pool and compares registers, as a link-time scene number does;
 * a plain 0x26 compiles to cmp with an immediate. The table it returns is
 * the overlay's data at 0x0200b010, which needs its label there. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 gKorimaSceneTable[];

s32 SceneData_SelectTableB5b8ByState(void)
{
    if (gGameState.scene == 0x26)
        return (s32)gKorimaSceneTable;
    return 0;
}
