/* Draft, differ 42 of 44: the native keeps the scene state base in r6 and rebuilds each field offset (356, 358, 364, 366) inside the loop; this form hoists field addresses out of the loop. */
#include "types.h"

#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

struct PuzzleCell {
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState {
    struct PuzzleCell cells[88];
    u8 unknown_160[4];
    s16 first_id;
    s16 end_id;
    u16 tile_entry;
    s8 redraw_a;
    s8 redraw_b;
    s16 origin_x;
    s16 origin_z;
};

struct EventWork1cc {
    u8 unknown_000[0x1cc];
    struct ObjectRuntime *object;
};

struct IoReg16 {
    u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

extern struct SceneState gSceneState;
extern s32 Data_02005af0;
extern const s8 Data_02005940[];
extern const u8 Data_02005948[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 handle, s32 size, void *source);
struct ObjectRuntime *Func_020005b4(struct ObjectRuntime *origin, s32 id);
void Func_02001950(s32 show, s32 first, u32 count);
void Func_02002394(void);
void Func_02004380(s32 x, s32 z, s32 a);
void Func_020055cc(const u8 *data, void *buffer);
void Func_0200561c(void *animation, s32 a);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005824(void);
void Func_0200582c(s32 index, s32 a, s32 b);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
s32 Func_0200584c(void);
void Func_02005854(s32 sound);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200567c(a, b, c, d, e, f);
}

static __inline__ void *Mem_Alloc(s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return MemAlloc(size);
}

static __inline__ void Vram_Load(s32 handle, s32 size, void *source)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    VramBlock_LoadCached(handle, size, source);
}

void Func_020044d0(s32 value)
{
    s32 id;

    for (id = gSceneState.first_id; id < gSceneState.end_id; id++) {
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02004380((object->x >> 20) - gSceneState.origin_x, (object->z >> 20) - gSceneState.origin_z, value);
    }
}
