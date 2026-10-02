#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"

struct MotionRecordState {
    u8 unknown_00[0x10];
    s32 field10;
};

s32 BattleMotion_ReleaseObjectSlotByValue();
s32 Object_Destroy(s32);

struct BattleAnchorPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleCamera *gCameraWork;
void Render_ResetTransformState(void);
s32 Graphics_PrepareTransferAndRun(void *, void *);
s32 Graphics_PrepareTransferInIwramWork(void *, void *);
s32 GameFlag_TestFar(s32);
extern u8 Camera_FlagTransformWork[];

struct BattleMotionRecord {
    u8 unknown_00[0x18];
    s32 scale_18;
};

s32 Render_ProjectPoint(const s32 *, s32 *);

s32 Camera_ApplyTransformByFlag(void);

struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id) {
    struct BattleSession *work = gBattleWork;
    struct BattleObjectSlot *slots = work->slots;
    s32 index;
    if (object_id > 7) {
        object_id -= 0x78;
    }
    index = object_id;
    if (work->placement[index] == 0xFF) {
        return NULL;
    }
    return &slots[work->placement[index]];
}

void ResetMotionRecordGroup(void *owner)
{
    s32 remaining;
    s32 zero;
    struct MotionRecordState **items;

    if (owner != NULL) {
        zero = 0;
        items = (struct MotionRecordState **)((u8 *)owner + 0x28);
        for (remaining = 3; remaining >= 0; remaining--) {
            struct MotionRecordState *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
    s32 storage_kind;
    s32 remaining;
    void **record_groups;
    void *record_group;

    if (object != NULL) {
        storage_kind = object->record_storage_kind & 0xF;
        switch (storage_kind) {
        case 1:
            ResetMotionRecordGroup(object->records);
            return;
        case 2:
            record_groups = object->records;
            remaining = 3;
            do {
                record_group = *record_groups++;
                if (record_group != NULL) {
                    ResetMotionRecordGroup(record_group);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
  struct BattleObjectSlot *slot;
  BattleMotion_ReleaseObjectSlotByValue();
  slot = GetBattleObjectSlot(object_id);
  slot->active = 1;
  return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
    s32 no;
    s32 i;
    struct BattleObjectSlot *slot;

    i = 0;
    do {
        no = i + 0x78;
        if (i <= 7) {
            no = i;
        }
        slot = GetBattleObjectSlot(no);
        if ((slot != NULL) && (slot->active != 0)) {
            Object_Destroy((s32)slot->object);
            slot->object = NULL;
            slot->active = 0;
        }
        i += 1;
    } while (i <= 0xD);
}

s32 Battle_GetWorkEntryPair(s32 no, struct BattleAnchorPosition *out)
{
    struct BattleSession *state = gBattleWork;

    out->x = state->slots[no].anchor_x;
    out->y = 0;
    out->z = state->slots[no].anchor_z;
    return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
    struct BattleCamera *state = gCameraWork;
    Render_ResetTransformState();
    if (GameFlag_TestFar(0x16B) != 0) {
        Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
        return Graphics_PrepareTransferAndRun(state, state->pos);
    } else {
        return Graphics_PrepareTransferInIwramWork(state, state->pos);
    }
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct BattleMotionRecord *record = GetMotionRecord(object, 0);
    s32 position[3];
    s32 scaled;

    Camera_ApplyTransformByFlag();
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    scaled = Render_ProjectPoint(position, projected);
    (void)Iwram_MulQ16(scaled, record->scale_18);
    return 0;
}

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
    s32 storage_kind = object->record_storage_kind & 0xF;
    if (storage_kind == 1) {
        if (record_index == 0) {
            return object->records;
        }
    } else if (storage_kind == 2) {
        return ((void **)object->records)[record_index];
    }
    return NULL;
}
