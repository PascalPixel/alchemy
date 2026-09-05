#include "types.h"
#include "battle_effect_runtime.h"

u8 *Func_08077080(s32);

struct BattleEffectEventRecord {
    s32 flags;
    u16 metadata;
    s16 action_id;
    u8 unknown_08[4];
};

struct BattleEffectCharacter {
    u8 unknown_00[12];
    u8 group;
};

struct BattleEffectRuntime {
    u8 unknown_00[16];
    struct BattleEffectEventRecord *events;
};

struct BattleEffectValueRecord {
    u8 unknown_00[6];
    u16 value;
};

s32 BattleEffect_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
    struct BattleEffectRuntime *runtime =
        (struct BattleEffectRuntime *)Data_03001ebc;
    struct BattleEffectEventRecord *event = runtime->events;
    s32 reference = ((struct BattleEffectValueRecord *)Func_0808ba1c(
        Data_02000240.object_id))->value;
    s32 selected = Func_0808df1c(Data_02000240.object_id, group);
    s32 alternate;
    s32 ignore_flags = 0;

    *(s32 *)result = selected;
    alternate = Func_0808bd24();
    if (requested_flags == 0x70000005)
        ignore_flags = 1;

    while (event->flags != -1) {
        s32 high_value = (s16)event->metadata & 0xf000;
        s16 has_reference = event->metadata & 0x0800;
        s32 low_value = event->metadata & 0xff;

        if ((event->flags & 0x0f) == 5 &&
            Func_0808d428(event->action_id) != 0 &&
            (has_reference == 0 ||
             (u16)(high_value - reference + 0x17ff) <= 0x2ffe) &&
            ((struct BattleEffectCharacter *)(void *)Func_08077080(
                ((u8 *)&event->flags)[1]))->group == group &&
            (ignore_flags ||
             (event->flags & 0x7000000f) == requested_flags)) {
            if ((event->flags & 0x80) != 0)
                return (s32)event;
            if ((event->flags & 0x10) != 0) {
                if (low_value == selected)
                    return (s32)event;
            } else if (low_value == alternate) {
                return (s32)event;
            }
        }
        event++;
    }
    return 0;
}
