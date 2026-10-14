/* Draft, differ 6 of 51: written with goto so that loop.c keeps the 0xffff test in the loop (a while loop hoists it into sl, differ 28). Left: the native loads the entry between the first two instructions that build 0xffff (movs, ldrh, lsls) at both tests, which agscc never schedules (its tie-break prefers the lsls, which has more dependants); no listing in TLA has the opposite order. Also entry += 3 sits after the call here, before it in the native. */
#include "../../../../games/tla/src/field/overlay_661/scene.h"
void Func_02003820(const void *script)
{
    const u16 *entry;

    if (*(const u16 *)script == 0xffff)
        return;
    entry = script;
loop:
    {
        s32 id = entry[0];
        struct FieldActor *object = Object_GetById(id);
        s32 kind = (s16)entry[1];
        s32 flag = (s16)entry[2];

        object->collision_flags &= 0xfe;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02003910(id, (u16)kind, (u16)flag, object->unknown_18[10]);
        entry += 3;
    }
    if (entry[0] != 0xffff)
        goto loop;
}
