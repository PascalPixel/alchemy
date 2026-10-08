#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "motobj.h"
#include "battle_motion.h"
#include "fixed_math.h"
#include "battle_session.h"

#define Object_ResetMotion ((void (*)(struct MotionObject *))Object_ResetMotion)
#define Object_SetPosition ((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPosition)
#define Object_SetMode ((void (*)(struct MotionObject *, s32))Object_SetMode)



void BattleMotion_ReservedNoOp83B0();

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = BtlObjGet(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

void BattleMotion_NoOp(void *position, s32 speed)
{
}

void BattleMotion_NoOp(void *, s32);

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
    struct BtlObj *second_slot;
    struct MotionObject *first;
    struct MotionObject *second;
    s32 x1;
    s32 x2;
    s32 z1;
    s32 z2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    first = BtlObjGet(first_id)->object;
    second_slot = BtlObjGet(second_id);


    x1 = first->target_x;
    /* FAKEMATCH: ⚓️ loads the first target before it builds the unset marker */
    __asm__ volatile("" : "+l"(x1));
    second = second_slot->object;
    if (x1 == (s32)0x80000000) {
        x1 = first->x;
    }
    z1 = first->target_z;
    if (z1 == (s32)0x80000000) {
        z1 = first->z;
    }
    x2 = second->target_x;
    if (x2 == (s32)0x80000000) {
        x2 = second->x;
    }
    z2 = second->target_z;
    if (z2 == (s32)0x80000000) {
        z2 = second->z;
    }

    pos.x = (x1 + x2) / 2;
    pos.y = 0;
    pos.z = (z1 + z2) / 2;

    BattleMotion_NoOp(&pos, 0x1000);
}

struct SlotEffectObject {
    u8 unknown_00[0x50];
    s32 resource;
    u8 kind;
};

struct SlotEffectEntry {
    struct SlotEffectObject *object;
};

void WaitFrames(s32 frames);
u8 *ResourceMetadata_RegisterFar(s32 resource, s32 id);
void Animation_SetWorkEntryFar(u8 *effect, s32 entry);

void BattleMotion_SpawnSlotEffectAndWait(s32 slot)
{
    struct SlotEffectEntry *entry = (struct SlotEffectEntry *)BtlObjGet(slot);
    u8 *effect;

    if (entry->object != NULL) {
        if ((entry->object->kind & 0xf) == 1) {
            effect = ResourceMetadata_RegisterFar(entry->object->resource, 0x29f);
            if (effect != NULL) {
                Animation_SetWorkEntryFar(effect, 1);
                effect[6] = 3;
            }
            WaitFrames(10);
        }
    }
}

struct SlotMotionRecord {
    u8 unknown_00[12];
    s32 height;
};

s32 Camera_ApplyTransformByFlag(void);
s32 Func_0811c37c(s32 owner);

s32 Func_0811c2b4(s32 slot, s32 *screen)
{
    struct MotionObject *object = BtlObjGet(slot)->object;
    struct SlotMotionRecord *record = GetMotionRecord(object, 0);
    s32 scale;
    /* FAKEMATCH: an unused vector the size of the native frame. */
    s32 unused[3];

    Camera_ApplyTransformByFlag();
    scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->height);
    screen[1] -= Iwram_MulQ16(scale, Func_0811c37c(slot) >> 16);
    return 0;
}

void *UnitGet(s32 owner);

s32 Func_0811c314(s32 slot, s32 *screen)
{
    struct MotionObject *object = BtlObjGet(slot)->object;
    struct SlotMotionRecord *record = GetMotionRecord(object, 0);
    s32 scale;

    Camera_ApplyTransformByFlag();
    scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), record->height);
    if (Summon_IsEntryFlagged(*(u16 *)((u8 *)UnitGet(slot) + 0x14a)) != 0)
        screen[1] -= Iwram_MulQ16(scale, 24);
    else
        screen[1] -= Iwram_MulQ16(scale, 48);
    return 0;
}
