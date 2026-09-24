#include "TYPES.H"

struct EffectSprite {
    u8 unknown_00[29];
    u8 enabled : 1;
    u8 flags : 7;
};

struct EffectObject {
    u8 unknown_00[80];
    struct EffectSprite *sprite;
    u8 hidden;
};

struct EffectRuntime {
    u8 unknown_00[4];
    s32 event_id;
    s32 unknown_08;
    s32 unknown_0c;
    u8 unknown_10[0x34 - 0x10];
    struct EffectObject *objects[58];
};

extern struct EffectRuntime *Data_03001ebc;

void Func_080090d0(struct EffectObject *object);
s32 ObjectTable_FindLastActiveId(void);
void Event_SpawnObjectTable(s32 event_id, s32 last_id);

/* Hides and releases every live effect object, then clears the pending
   event and respawns its object table if one was set. */
void BattleEffect_ClearAllObjects(void)
{
    struct EffectRuntime *runtime = Data_03001ebc;
    s32 event_id;
    s32 i;

    for (i = 0; i < 58; i++) {
        struct EffectObject *object = runtime->objects[i];

        if (object != 0) {
            object->hidden = 1;
            object->sprite->enabled = 0;
            Func_080090d0(object);
            runtime->objects[i] = 0;
        }
    }
    event_id = runtime->event_id;
    runtime->event_id = 0;
    runtime->unknown_08 = 0;
    runtime->unknown_0c = 0;
    if (event_id != 0)
        Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
