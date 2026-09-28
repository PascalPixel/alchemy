/* Draft of resource_381 0x02009084 (SceneState_InitStateWordsAndSlots and what follows it in this file),
 * from games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA (FUNKA.H). Remaining
 * difference: it reads and writes the scene's variables that lie past the
 * overlay image (0x0200bac0 and on), which no source defines, so it cannot
 * link by name. The listing keeps these rows. */
/* The scene's state words and slots. */
#include "FUNKA.H"

void SceneState_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    p = Data_0200bac0;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}
