#include "TYPES.H"
#include "HEAP_STATE.H"
#include "BATTLE_PARTY.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PRESENTATION.H"

extern u8 gKeysHeld[];
void BattleCamera_SetRange(s32, s32, s32, s32, s32);

/* battle/presentation/cam/shoulder_alt.c */
void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
    /* FAKEMATCH: separate cell loads change the literal pool and register
       order. The existing walk reaches slot 44 from camera slot 12. */
    void **slot = (void **)&gCameraWork;
    struct BattleCamera *cam = slot[0];
    struct BattlePresentationTransition *trans = slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
    volatile u32 *keys = (volatile u32 *)gKeysHeld;

    if ((*keys & 512) != 0) {
        cam->yaw += 512;
    }
    if ((*keys & 256) != 0) {
        cam->yaw -= 512;
    }
    if (trans->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/runtime/reserved_no_op_b.c */
void Battle_ReservedNoOp9B2C(void)
{
}
