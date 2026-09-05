/* Contiguous unnamed state-owner run for resource_3a3. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 Data_02009730[];
extern u8 Data_020099f4[];
extern u8 Data_02009724[];

s32 SceneData_GetPrimaryTable(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009730;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_020099f4;
    }
    return (s32)Data_02009724;
}

#include "types.h"

void Func_02001008();       /* opens a scripted sequence */
void Func_02001096();       /* queues a cue / script id */
s32 Func_020010a6();
s32 Func_02001036();        /* result is compared against 1 */
void Func_020010c2();
void Func_020010ec();
void Func_02001050();       /* closes the scripted sequence */

extern u8 *Data_03001ebc;   /* pointer cell holding the overlay work */

void FieldScene_RunActorEightPromptDialogue(void)
{
    u8 *work;

    Func_02001008();
    Func_02001096(0x18bd);
    /* r1 is set before r0; the argument order is unchanged. */
    Func_020010a6(8, 0);

    if (Func_02001036(0, 0) == 1) {
        Func_020010c2(8, 0);
    } else {
        work = Data_03001ebc;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Func_020010ec(8, 0);
    }

    Func_02001050();
}
