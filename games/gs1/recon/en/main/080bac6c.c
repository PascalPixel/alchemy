#include "types.h"

extern u8 *Data_03001e74;

u8 *Runtime_GetObject(s32 actor_id);
void Func_080c1ebc(s32 actor_id);

#define BattleActor_RemoveFromLists Func_080bac6c

void BattleActor_RemoveFromLists(s32 actor_id)
{
    u8 *actor;
    s32 work;
    s32 list;
    s32 offset;
    s32 value;
    s32 empty;
    u32 i;

    work = (s32)Data_03001e74;
    actor = Runtime_GetObject(actor_id);
    actor[0x12a] = 0;

    offset = 0x58;
    while ((value = *(s16 *)(offset + work)) != actor_id) {
        if (value != 0xff) {
            offset += 2;
            continue;
        }

        i = 0;
        list = work + 2;
        do {
            offset = i * 2 + 0x64;
            value = *(s16 *)(list + offset);
            if (value == actor_id) {
                *(s16 *)(list + offset) = 0xfe;
                goto removed;
            }
            i++;
        } while (value != 0xff);
        return;
    }
    *(s16 *)(offset + work) = 0xfe;

removed:
    Func_080c1ebc(actor_id);
    offset = 0x2ec;
    i = 0;
    empty = 0xff;
    do {
        if (*(s16 *)(offset + work) == actor_id)
            *(s16 *)(offset + work) = empty;
        i++;
        offset += 16;
    } while (i <= 19);
}
