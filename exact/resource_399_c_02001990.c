#include "types.h"

/*
 * Prepare the scene service selected by index zero, set its halfword at +6,
 * and run the four follow-up services in the order present in the overlay.
 *
 * This owner starts at the saved-link prologue at 0x02001990 and returns at
 * 0x020019b8.  The next saved-link prologue is at 0x020019bc, so the two zero
 * bytes between them are alignment and are deliberately outside this source.
 */

struct SceneService_02001990 {
    u16 unknown00[3];
    u16 value06;
};

extern void Func_02003bc4(void);
extern struct SceneService_02001990 *Func_02003be2(s32 index);
extern void Func_02003d8e(s32 value);
extern void Func_02003d32(void);
extern void Func_02003d3e(void);
extern void Func_02003d14(s32 value);

void Func_02001990(void)
{
    struct SceneService_02001990 *service;

    Func_02003bc4();
    service = Func_02003be2(0);
    service->value06 = 0xc000;
    Func_02003d8e(123);
    Func_02003d32();
    Func_02003d3e();
    Func_02003d14(8);
}
