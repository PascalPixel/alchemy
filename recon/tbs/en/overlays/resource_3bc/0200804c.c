/* NONMATCHING: resource_3bc at 0x0200804c (316 bytes with its pool),
 * ColossoLogRollingStage_SceneTask, between FIELD/KOROSSEO_MARUTA/HOOKS.C
 * and LOG_ROLLING.C, stays listing. The label KorosseoMaruta_LogPattern in
 * the listing's rodata names the four six-byte log rows it reads.
 *
 * Remaining difference: the reference keeps the column shift `i << 21` in
 * the second loop and holds 0x31ffff and 0x13fffe in registers. As a for
 * loop the compiler folds the constant into a strength-reduced induction
 * value and reverses the loop; as a goto loop (below) the constants stay in
 * the pool, which is 1515 in alchemy permute. The first loop and the rest
 * of the function match.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/LOG_ROLLING.H"

extern s8 KorosseoMaruta_LogPattern[][6];

void ColossoLogRollingStage_SceneTask(void)
{
    u8 *work = *(u8 **)&gEventWork;
    s16 *table = (s16 *)&gGameState;
    u8 *actor = (u8 *)Object_GetById(*(s32 *)&table[250]);
    s32 row = *(s32 *)(actor + 16) >> 20;
    s32 i;
    s32 kind;

    if (gColossoSceneTaskState == 0) {
        gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
        for (i = 18; i <= 22; i++) {
            kind = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][i - 18];
            Engine_ActorSetAnimation(i, kind);
            Engine_ActorSetAnimation(i + 5, kind + 8);
            Engine_MapCopyCellAttributes(32, 11, 1, 2, (i - 18) * 2 + 33, 11);
            if (kind != 7) {
                Engine_MapCopyCellAttributes(74, 12, 1, 1, (i - 18) * 2 + 33, 11);
            }
        }
        Engine_ActorSetAnimation(28, KorosseoMaruta_LogPattern[gColossoSceneTaskStatus][5]);
    } else {
        s8 *p = KorosseoMaruta_LogPattern[gColossoSceneTaskStatus];
        s16 *slot = (s16 *)(work + 386);

        i = 18;
    next:
        kind = *p++;
        if ((u32)(*(s32 *)(actor + 8) - (i << 21) + 0x31ffff) <= 0x13fffe) {
            if (row == 11 && kind == 4)
                *slot = kind;
            if (row == 12 && kind == 5)
                *slot = kind;
        }
        if (++i <= 22)
            goto next;
    }
    if ((u32)++gColossoSceneTaskState > 17) {
        gColossoSceneTaskState = 0;
    }
}
