#include "types.h"

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

extern struct LevelCheckRecord *Func_02005552();   /* record by slot index */
extern struct LevelCheckRecord *Func_02004d68();   /* occupant one square ahead */
extern void Func_020027f2();                       /* push the object ahead */

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct LevelCheckRecord *p = Func_02005552(0);
    struct LevelCheckRecord *q = Func_02004d68(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    Func_020027f2();
}
