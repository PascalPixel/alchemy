#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "btldisp.h"
#include "fixed_point_position.h"
#include "ram_buffer.h"
#include "iwram_call.h"

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
