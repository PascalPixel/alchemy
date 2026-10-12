#include "types.h"
#include "object_runtime.h"
#include "animspr.h"
#include "frame.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "party_state.h"

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

struct CompositeAnimation {
    u8 unknown_00[0x11];
    u8 display_flags;
    u8 unknown_12[7];
    u8 dirty;
    u8 unknown_1a;
    u8 count;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entries[4];
};

void Func_080dd950(void);
void Func_080debd8(void);
void Func_080e11bc(void);
void Func_080e4730(void);
void Func_080e9db4(void);
void Func_080e87c4(void);

void Func_080dca50(void);

// party_state.h の unknown_26a
struct PartyPauseView {
    u8 unknown_000[0x26a];
    s16 paused_object;
};

void BattleFx_ClearChildValueOnMismatch(void)
{
    struct BattleFxScene *scene = Ram_HeapSlots->effect_work;

    if (scene->animation == 2) {
        struct PartyPauseView *party;

        Func_080dca50();
        party = (struct PartyPauseView *)&gPartyState;
        asm volatile(""); /* FAKEMATCH: ⚓️ loads the party state's address before it builds the field offset; read as a plain field the offset's first half is scheduled ahead of the load (differ 2) */
        if (party->paused_object != scene->second_object_id)
            ((struct ObjectRuntime *)scene->child)->movement_state = 0;
    }
}

// 種類ごとの追加処理
void FieldEvent_RunTypeHandler(void)
{
    struct BattleFxScene *scene = Ram_HeapSlots->effect_work;

    switch (scene->animation) {
    case 8:
        Func_080dd950();
        break;
    case 10:
        Func_080debd8();
        break;
    case 16:
        Func_080e11bc();
        break;
    case 23:
        Func_080e4730();
        break;
    case 24:
        Func_080e9db4();
        break;
    case 28:
        Func_080e87c4();
        break;
    }
}

void ObjectGroup_ApplyRandomChildValues(struct ObjectRuntime *owner)
{
    s32 state;
    struct CompositeAnimation *target;
    s32 initial_count;
    s32 count;
    struct AnimationEntry **entry;
    struct AnimationEntry *current;
    u32 frame;
    s32 value;

    state = owner->animation_kind;
    if (state == 1) {
        target = owner->animation;
        if (target != 0 && (target->display_flags & state) == 0) {
            initial_count = target->count;
            if (initial_count != 0) {
                entry = target->entries;
                frame = gFrameCount;
                count = initial_count;
                do {
                    current = *entry++;
                    value = frame % 6;
                    count--;
                    current->param = value;
                } while (count != 0);
            }
            target->dirty = 1;
        }
    }
}

void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
    s32 values[3];

    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        Vector_AddPolarOffset(magnitude, angle, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}
