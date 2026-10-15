/* Draft, differ 2 of 49: the native loads the leader x between building the 64 mask and copying it (movs r2, #64; ldr r6, [r0, #8]; adds r3, r2); this form copies the mask first. A one-pass block after the x shift is differ 4. Each coordinate written as three statements (x = leader->x; x >>= 20; x |= the masked cell) is the same differ 2. Also tried: the leader passed through an asm reading the mask (6), the mask through an asm reading the leader x or the reverse (20, registers swap), the leader x pinned to r6 (2). */
#include "types.h"

#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"

struct EventWork1ac {
    u8 unknown_000[0x1ac];
    s32 value;
};

struct CellTarget {
    u8 unknown_00[6];
    s16 x;
    s16 z;
};

s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Math_Cosine(s32 angle);
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200015c(struct ObjectRuntime *object);
void Func_020004dc(s32 mode);
struct ObjectRuntime *Func_02001c04(s32 type, s32 x, s32 y, s32 z);
void Func_02001c34(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct ObjectRuntime *Func_02001cdc(void);
void Func_02001d3c(s32 id);
void Func_02001d4c(s32 cue);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_02001654(struct CellTarget *target)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 x = (leader->x >> 20) | (s16)(target->x & 64);
    s32 z = (leader->z >> 20) | (s16)(target->z & 64);

    Func_020015cc();
    Func_02001d4c(161);
    Func_02001c34(target->x, target->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
