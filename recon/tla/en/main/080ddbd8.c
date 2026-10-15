/* Draft, differ 239 of 208 (2026-10-15), ported from ☀️'s
 * BattleEffect_SpawnBurstParticleField: ⚓️ spawns object 672, its sprite's second
 * part sits 28 bytes in (☀️: 12), the target goes through Object_SetPosition and the
 * angle is read as a halfword. The listing keeps the loop count in a stack word and
 * two copies of the position pointer (r10, r11); this keeps the count in a register.
 * Far from matching; kept for the measured ⚓️ differences. */
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "field_event.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct BurstParticleVisual {
    u8 reserved_00[5];
    u8 flags_a_low : 5;
    u8 flip : 1;
    u8 flags_a_high : 2;
    u8 reserved_06;
    u8 flags_b_low : 6;
    u8 flags_b_high : 2;
    union {
        struct {
            u16 shape : 10;
            u16 unused : 6;
        } bits;
        struct {
            u8 lo;
            u8 high_low : 4;
            u8 palette : 4;
        } bytes;
    } attributes;
};

// ⚓️'s sprite keeps its second part 28 bytes in.
struct BurstParticleVisualGroup {
    struct BurstParticleVisual primary;
    u8 reserved_0c[16];
    struct BurstParticleVisual child;
};

void BattleEffect_InitializeSharedScene(void);
struct FieldActor *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, u32 value);
void ObjectDispatch_SetSingleChildFlagsFar(struct FieldActor *object, s32 flags);


void Func_080ddbd8(void)
{
    struct BattleFxScene *work;
    struct FieldActor *user;
    s32 pos[3];
    s32 n;

    work = Ram_HeapSlots->effect_work;
    user = work->main_object;
    BattleEffect_InitializeSharedScene();

    for (n = 0; n < 24; n++) {
        struct FieldActor *obj;
        struct BurstParticleVisual *visual;
        struct BurstParticleVisual *child;
        s32 dist;

        if ((u16)work->angle == 0x4000) {
            pos[0] = user->x;
            pos[1] = user->y + 0xa0000;
            pos[2] = user->z;
        } else if ((u16)work->angle == 0xc000) {
            pos[0] = user->x;
            pos[1] = user->y + 0x180000;
            pos[2] = user->z;
        } else {
            pos[0] = user->x;
            pos[1] = user->y + 0xa0000;
            pos[2] = user->z;
            Vector_AddPolarOffset(0xa0000, (u16)work->angle, pos);
        }

        obj = Object_Spawn(672, pos[0], pos[1], pos[2]);
        visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
        child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
        child->flip = visual->flip;
        child->flags_a_high = visual->flags_a_high;
        child->flags_b_high = visual->flags_b_high;
        child->attributes.bits.shape = visual->attributes.bits.shape;
        child->attributes.bytes.palette = visual->attributes.bytes.palette;

        if (obj != 0) {
            ((s32 *)obj)[7] = 0xb333;
            ((s32 *)obj)[6] = 0xb333;
            obj->acceleration = 0x18000;
            obj->speed = 0x18000;
            obj->motion_flags = 0;
            Animation_ApplyChildValuesFar(obj, 11);
            Object_SetMode(obj, 7);
            ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript + 1);
            ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

            pos[0] = work->x;
            pos[1] = work->y;
            pos[2] = work->z;
            if ((u16)work->angle == 0xc000)
                Vector_AddPolarOffset(0xe0000, (u16)work->angle, pos);
            dist = Random16() * 6 + 0x40000;
            Vector_AddPolarOffset(dist, Random16(), pos);
            Object_SetPosition((struct ObjectRuntime *)obj, pos[0], pos[1], pos[2]);
        }
        Audio_PlayCue(131);
        WaitFrames(2);
    }
    WaitFrames(8);
}
