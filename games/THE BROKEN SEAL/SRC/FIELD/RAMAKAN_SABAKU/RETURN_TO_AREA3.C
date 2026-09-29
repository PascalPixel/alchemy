#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

/* Actor 8 repeats its motion, and the party is set to come back to the
   desert's third area by its fifth entrance. */
void FieldScene_RunScene3a5_02000c38(void)
{
    extern u8 Data_02000240[];

    Actor_RunRepeatedMotion(8, 2);
    Party_SetFields1ceAnd1d0((s32)&SceneId_RamakanSabaku3, 5);
    /* FAKEMATCH: the do/while loads the game state's base before the 0x22b
       offset, which fixes their registers and literal-pool order. */
    do {
        Data_02000240[0x22b] = 3;
    } while (0);
    BattleFx_SetWeightedResult(53, 5);
}
