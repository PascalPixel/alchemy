#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "resource.h"
#include "field_sprite.h"

void Animation_ApplyChildArgumentFar(void *, s32);

// Creates entry_count sprites of one resource in the effect work's object
// slots, each showing its own animation at the given priority (The Broken
// Seal's BattleFx_SpawnObjects).
void Func_08152404(s32 entry_count, s32 kind, u32 variant)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battle_fx_work[0];
    s32 entry_index = 0;

    if (entry_count == 0)
        return;
    do {
        struct FieldSprite *object = (struct FieldSprite *)ResourceObject_CreateFar(kind);

        work->objects[entry_index] = object;
        if (object != NULL) {
            object->next_frame = 0;
            Animation_ApplyChildArgumentFar(object, entry_index);
            ((struct FieldSprite *)work->objects[entry_index])->priority = variant;
        }
        entry_index++;
    } while (entry_index != entry_count);
}

void BattleFx_RunNoEffect(void)
{
}
