#include "types.h"

extern u8 *Data_03001e74;
extern u8 Data_000000fe[];

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
    s32 mark;
    s32 empty;
    u32 i;

    work = (s32)Data_03001e74;
    actor = Runtime_GetObject(actor_id);
    mark = (s32)Data_000000fe;
    actor[0x12a] = 0;

    offset = 0x58;
    for (;;) {
        value = *(s16 *)(offset + work);
        if (value == actor_id) {
            *(s16 *)(offset + work) = mark;
            break;
        }
        if (value == 0xff) {
            i = 0;
            list = work + 2;
            do {
                offset = i * 2 + 0x64;
                value = *(s16 *)(list + offset);
                if (value == actor_id) {
                    *(s16 *)(list + offset) = mark;
                    goto removed;
                }
                i++;
            } while (value != 0xff);
            return;
        }
        offset += 2;
    }

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
