/* Draft of resource_38d 0x02008100 (SceneData_SelectTableac9cByState), built
 * with games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/KYUDEN.H. Remaining
 * difference: the ROM loads the scene number 0x21 from the literal pool to
 * compare it with the game state's scene, as a link-time value would; a C
 * constant compares against an immediate. The listing keeps these rows. */
#include "KYUDEN.H"

extern u8 Value_00000021;
extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];

s32 SceneData_SelectTableac9cByState(void)
{
    if (gGameState.scene == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}
