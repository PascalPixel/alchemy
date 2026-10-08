#include "types.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "battle_session.h"

struct SummonCast {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    s16 actors[8];
};

struct SummonFxWork {
    u8 unknown_0000[0x78f0];
    struct SummonCast *cast;
};

struct SummonTransition {
    u8 unknown_00[16];
    s32 active;
};

extern u32 gCameraSceneParameters[];
struct BtlObj *GetBattleObjectSlotFar(s32 object_id);
void Func_08118088(s32 actor, s32 mode);
void PaletteDma_LoadBlock(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 mode);

void Func_08191cc4(void)
{
    struct SummonTransition *transition = (struct SummonTransition *)Ram_HeapSlots->transition_work;
    struct SummonFxWork *fx = (struct SummonFxWork *)Ram_HeapSlots->battle_fx_work[0];
    u8 *battle = (u8 *)Ram_HeapSlots->battle_work;
    s32 i;

    for (i = 0; i != fx->cast->count; i++) {
        struct MotionObject *object = GetBattleObjectSlotFar(fx->cast->actors[i])->object;

        object->x = 0;
        object->y = 0x780000;
        object->z = 0;
        object->vertical_motion_strength = 0xab85;
        Func_08118088(fx->cast->actors[i], 1);
    }
    transition->active = 0;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    PaletteDma_LoadBlock();
    BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
}
