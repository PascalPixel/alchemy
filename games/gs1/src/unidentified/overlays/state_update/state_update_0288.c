#include "types.h"

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

extern struct LevelCheckRecord *Func_02005552();   /* record by slot index */
extern struct LevelCheckRecord *Func_02004d68();   /* occupant one square ahead */
extern void Func_020027f2();                       /* push the object ahead */

void Func_020026fc(void)
{
    struct LevelCheckRecord *player = Func_02005552(0);
    struct LevelCheckRecord *ahead = Func_02004d68(player);
    s32 difference;

    if (ahead == 0) {
        return;
    }

    difference = ahead->y - player->y;

    if (difference >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (difference >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (difference < 0x80000) {
        } else {
            return;
        }
    } else if (player->y - ahead->y >= 0x80000) {
        return;
    }

    Func_020027f2();
}
