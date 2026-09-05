#include "types.h"
#include "object_lookup.h"

s32 Func_0808ddec(s32 object_id);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_0808bd24(void);

/* 12-byte trigger table, terminated by a sentinel entry whose first word is
   -1. flags: bits0-3 = type (must be 4), bit4 = which candidate id `id`
   is compared against, bits8-15 = a source/party id to match against the
   caller's masked argument. metadata: bits0-7 = id to compare against
   `id`, bit11 = enable the facing-quantum check, bits12-15 = required
   facing quantum (top nibble of a 0-0xffff angle, matching the angle unit
   RotateVectorByMagnitude and Object_0808bd24.kind use elsewhere). */
struct Trigger_0808e14c {
    s32 flags;
    u16 metadata;
    s16 condition;
    s32 effect;
};

struct Runtime_0808e14c {
    u8 padding000[0x10];
    struct Trigger_0808e14c *triggers;
};

struct Object_0808e14c {
    u8 padding00[6];
    u16 kind;
};

struct Global_0808e14c {
    u8 padding000[0x1f4];
    s32 object_id;
};

extern struct Global_0808e14c Data_02000240;
extern struct Runtime_0808e14c *Data_03001ebc;

void *Func_0808e14c(u32 arg0)
{
    struct Trigger_0808e14c *trigger = Data_03001ebc->triggers;
    struct Object_0808e14c *object = ObjectTable_Get(Data_02000240.object_id);
    s32 facing = object->kind;
    s32 id = Func_0808ddec(Data_02000240.object_id);
    s32 masked = arg0 & 0x1ff;
    s32 collision = Func_0808bd24();

    for (; trigger->flags != -1; trigger++) {
        s32 dirNibble = (s16)trigger->metadata & 0xf000;
        s16 dirCheckEnabled = trigger->metadata & 0x800;
        s32 matchId = trigger->metadata & 0xff;
        s32 type = trigger->flags & 0xf;
        s32 flags;
        u8 src;

        if (type != 4)
            continue;
        if (GameFlag_IsConditionActive(trigger->condition) == 0)
            continue;
        if (dirCheckEnabled != 0) {
            s32 diff = dirNibble - facing + 0x17ff;
            if ((u16)diff > 0x2ffe)
                continue;
        }
        flags = trigger->flags;
        src = ((u8 *)&trigger->flags)[1];
        if (masked != 0 && src != masked)
            continue;
        if (flags & 0x10) {
            if (matchId == id)
                return trigger;
        } else if (matchId == collision)
            return trigger;
    }

    return 0;
}
