#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_RUNTIME.H"
#include "ANIMSPR.H"

s32 WaitFrames(s32);
struct SpriteEntry *ResourceMetadata_RegisterFar(void *, s32);
s32 Animation_SetWorkEntryFar(void *, s32);


void Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
u32 Battle_GetObjectTableValue(s32);
s32 Summon_IsEntryFlagged(s32);

void BattleMotion_SpawnSlotEffectAndWait(s32 id)
{
    struct MotionObject *object;
    struct SpriteEntry *effect;

    object = GetBattleObjectSlot(id)->object;
    if ((object != NULL) && ((0xF & object->record_storage_kind) == 1)) {
        effect = ResourceMetadata_RegisterFar(object->records, 0x11B);
        if (effect != NULL) {
            Animation_SetWorkEntryFar(effect, 1);
            effect->priority = 3;
        }
        WaitFrames(0xA);
    }
}

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct AnimationObject *record = GetMotionRecord(object, 0);
    s32 position[3]; /* FAKEMATCH: unused; it only reserves the 12-byte frame the reference allocates. */
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale);
    scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
    projected[1] -= scaled;
    return 0;
}

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct AnimationObject *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale);
    if (Summon_IsEntryFlagged(Owner_GetStateFar(id)->class_id) != 0)
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

    no = Owner_GetStateFar(id)->class_id;
    value = (u32)(Summon_GetEntryByte4((s32)no) << 0x18) >> 8;
    if (value == 0) {
        no = Owner_GetStateFar(id)->class_id;
        if (Summon_IsEntryFlagged((s32)no) != 0) {
            value = 0x180000;
        } else {
            value = 0x300000;
        }
    }
    return value;
}
