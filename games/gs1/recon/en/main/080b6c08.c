#include "types.h"

extern u8 *Data_03001e74;

#define BattleParty_ListActorIds Func_080b6c08

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
    s32 work;
    s32 list;
    s32 offset;
    s32 count;
    s32 actor_id;
    u16 raw_id;

    work = (s32)Data_03001e74;
    count = 0;

    if (groups & 1) {
        offset = 0x58;
        if (*(s16 *)(work + offset) != 0xff) {
            do {
                actor_id = *(s16 *)(work + offset);
                raw_id = *(u16 *)(work + offset);
                if (actor_id != 0xfe) {
                    if (dst != 0) {
                        *dst = raw_id;
                        dst++;
                    }
                    count++;
                }
                offset += 2;
            } while (*(s16 *)(work + offset) != 0xff);
        }
    }

    if (groups & 2) {
        list = work + 2;
        offset = 100;
        if (*(s16 *)(list + offset) != 0xff) {
            do {
                actor_id = *(s16 *)(list + offset);
                raw_id = *(u16 *)(list + offset);
                if (actor_id != 0xfe) {
                    if (dst != 0) {
                        *dst = raw_id;
                        dst++;
                    }
                    count++;
                }
                offset += 2;
            } while (*(s16 *)(list + offset) != 0xff);
        }
    }

    if (dst != 0)
        *dst = 0xff;
    return count;
}
