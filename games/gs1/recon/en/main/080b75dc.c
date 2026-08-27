#include "types.h"
#include "battle_work.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern const s8 Data_080c2a62[];

s32 Func_080b6a60(s16 *owners);
void *Func_080b7dd0(s32 unit);
void Func_080b6f44(void *actor, s32 unit, s32 x, s32 y);
void Func_080b7424(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);

#define BattleUnit_RefreshPlacement Func_080b75dc

void BattleUnit_RefreshPlacement(void)
{
    u16 buf[14];
    u16 *ids = buf;
    u8 *battle = BattleWorkPtr;
    u16 *cursor;
    s32 i;
    s32 count;
    s32 n;
    u8 *p;
    s32 val;
    s32 pos;
    s32 id;
    s32 x_positions[6];
    s32 z_positions[6];

    count = Func_080b6a60((s16 *)ids);

    n = 13;
    p = (u8 *)battle + 0x2e9;
    while (n >= 0) {
        n--;
        *p-- = 0xff;
    }

    n = 5;
    p = (u8 *)battle + 0x2e9;
    val = 13;
    while (n >= 0) {
        n--;
        *p = val;
        p--;
        val--;
    }

    if (count > 0) {
        cursor = ids;
        i = 0;
        pos = 0;
        n = count;
        do {
            id = *cursor;
            cursor++;
            FIELD(battle, u8, 0x2dc + id) = i;
            Func_080b6f44(Func_080b7dd0(id), id, Data_080c2a62[pos],
                          Data_080c2a62[pos + 1]);
            n--;
            pos += 2;
            i++;
        } while (n != 0);
    }

    count = 0;
    if (FIELD(battle, s16, 2 + 0x64) != 0xff) {
        i = 0;
        pos = 0x64;
        do {
            count++;
            FIELD(ids, u16, i) = FIELD(battle, u16, 2 + pos);
            pos += 2;
            i += 2;
        } while (count <= 5 && FIELD(battle, s16, 2 + pos) != 0xff);
    }

    Func_080b7424(ids, count, x_positions, z_positions);

    if (count > 0) {
        pos = 0x64;
        i = 0;
        n = count;
        do {
            id = FIELD(battle, s16, 2 + pos);
            if (id != 0xfe) {
                Func_080b6f44(Func_080b7dd0(id), id,
                              *(s32 *)((u8 *)x_positions + i),
                              *(s32 *)((u8 *)z_positions + i));
            }
            n--;
            pos += 2;
            i += 4;
        } while (n != 0);
    }
}
