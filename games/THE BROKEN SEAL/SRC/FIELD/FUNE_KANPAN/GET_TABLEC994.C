#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 FuneKanpan_SceneTableA[];
extern u8 FuneKanpan_SceneTableB[];
extern u8 FuneKanpan_SceneTableC[];
extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3af. */
u8 *SceneData_GetTablec994(void)
{
    return FuneKanpan_SceneTableA;
}

u8 *SceneData_GetTablecb44(void)
{
    return FuneKanpan_SceneTableB;
}

u8 *SceneData_GetTablecb64(void)
{
    return FuneKanpan_SceneTableC;
}
