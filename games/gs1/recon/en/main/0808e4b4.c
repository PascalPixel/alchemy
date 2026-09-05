#include "types.h"

#define BattleEffect_FindMatchingEvent Func_0808e4b4

/* u8 *-returning to match the prototype the exact owner battle_owner_23.c
 * uses under its Ability_GetData alias; this owner casts the raw pointer to
 * its own local view below. */
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

/*
 * Declared s32-returning with a u16 second parameter to match the prototype
 * agreed by the exact owners that call it (games/gs1/src/unidentified/main/
 * battle/battle_owner_23.c and .../runtime/runtime_owner_207.c both declare
 * `s32 Func_0808e4b4(s32, u16, void *)`); both callers only test the result
 * for truthiness, and the returned pointer's bit pattern is unchanged by the
 * narrower C type.
 */
s32 BattleEffect_FindMatchingEvent(
    s32 requested_flags, s32 group, void *result)
{
    struct BattleEffectRuntime *runtime =
        *(struct BattleEffectRuntime **)0x03001ebc;
    struct BattleEffectEventRecord *event = runtime->events;
    s32 context = *(s32 *)0x02000240;
    s32 reference =
        ((struct BattleEffectValueRecord *)Func_0808ba1c(context))->value;
    s32 selected = Func_0808df1c(context, group);
    s32 alternate = Func_0808bd24();
    s32 ignore_flags = requested_flags == 0x70000005;

    *(s32 *)result = selected;
    while (event->flags != -1) {
        u16 metadata = event->metadata;
        s32 low_value = metadata & 0xff;

        if ((event->flags & 0x0f) == 5 &&
            Func_0808d428(event->action_id) != 0 &&
            ((metadata & 0x0800) == 0 ||
             (u16)(((metadata & 0xf000) - reference) + 0x17ff) <= 0x2ffe) &&
            ((struct BattleEffectCharacter *)(void *)Func_08077080(
                ((u8 *)&event->flags)[1]))->group == group &&
            (ignore_flags ||
             (event->flags & 0x7000000f) == requested_flags)) {
            if ((event->flags & 0x80) != 0 ||
                ((event->flags & 0x10) != 0 && low_value == selected) ||
                ((event->flags & 0x10) == 0 && low_value == alternate)) {
                return (s32)event;
            }
        }
        event++;
    }

    return 0;
}
