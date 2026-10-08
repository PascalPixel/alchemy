#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "motobj.h"
#include "iwram_call.h"
#include "scene.h"
#include "btlsys.h"
#include "animspr.h"
#include "effect_step.h"

void BattleMotion_SpawnSlotEffectAndWait(s32 id)
{
    struct MotionObject *object;
    struct AnimationEntry *effect;
    s32 entry;

    object = BtlObjGet(id)->object;
    if ((object != NULL) && ((0xF & object->record_storage_kind) == 1)) {
        entry = ResourceMetadata_RegisterFar((struct AnimationObject *)object->records, 0x11b);
        if (entry != 0) {
            effect = (struct AnimationEntry *)entry;
            Animation_SetWorkEntryFar(effect, 1);
            effect->priority = 3;
        }
        WaitFrames(0xA);
    }
}

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = BtlObjGet(id)->object;
    struct AnimationObject *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;
    /* FAKEMATCH: needed to match: the input position BattleMotion_ProjectPosition fills, unused here */
    struct EffectPosition input;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale);
    scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
    projected[1] -= scaled;
    return 0;
}

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = BtlObjGet(id)->object;
    struct AnimationObject *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale);
    if (Summon_IsEntryFlagged(UnitGetFar(id)->class_id) != 0)
        scaled = Iwram_MulQ16(factor, 24);
    else
        scaled = Iwram_MulQ16(factor, 48);
    projected[1] -= scaled;
    return 0;
}

u32 Battle_GetObjectTableValue(s32 id)
{
    u32 value;
    u8 no;

    no = UnitGetFar(id)->class_id;
    value = (u8)Summon_GetEntryByte4(no) << 16;
    if (value == 0) {
        no = UnitGetFar(id)->class_id;
        if (Summon_IsEntryFlagged(no) != 0) {
            value = 0x180000;
        } else {
            value = 0x300000;
        }
    }
    return value;
}
