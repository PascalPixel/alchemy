/* Matches (differ 0): ☀️'s Camera_ApplyTransformByFlag with the camera
   from its heap slot, the near GameFlag_Test and the flag matrix at
   Data_08128868. Not adopted only because it would sit alone between the
   Battle_GetWorkEntryPair and BattleMotion_ProjectPosition listings; it
   goes in RESET_BATTLE_OBJECT_RECORD_GROUPS.C once Battle_GetWorkEntryPair
   does. */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_SESSION.H"
#include "BATTLE_PRESENTATION.H"
#include "FIXED_POINT_POSITION.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"

void Object_Destroy(void *object);
void Render_ResetTransformState(void);
s32 GameFlag_Test(s32 flag);
s32 Graphics_PrepareTransferAndRun(void *, void *);
s32 Graphics_PrepareTransferInIwramWork(void *, void *);
s32 Render_ProjectPoint(const s32 *, s32 *);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);
extern const s32 Data_08128868[];

struct MotionRecordScale {
    u8 unknown_00[0xc];
    s32 scale;
};

/* ☀️'s: loads the battle camera's view, through the flag 0x16b matrix
   when that flag is up. */
s32 Camera_ApplyTransformByFlag(void)
{
    struct BattleCamera *state = Ram_HeapSlots->camera_work;

    Render_ResetTransformState();
    if (GameFlag_Test(0x16B) != 0) {
        Iwram_TransformMatrix(Data_08128868);
        return Graphics_PrepareTransferAndRun(state, state->pos);
    } else {
        return Graphics_PrepareTransferInIwramWork(state, state->pos);
    }
}
