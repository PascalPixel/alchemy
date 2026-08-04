#include "types.h"

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

extern struct SceneWork_020006b4 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern void Func_0808a178(s32 actor, s32 mode);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_080770c8(s32 flag);
extern void Func_0808a020(void);

void Func_020006b4(void)
{
    Func_0808a018();
    Func_0808a170(0x1cb5);
    Func_0808a150(16, 0, 2);
    Func_0808a178(16, 0);
    if (Func_0808a070(0, 0) != 0) {
        Data_03001ebc->branch_counter += 1;
    }
    Func_0808a180(16, 0);
    Func_080770c8(0x308);
    Func_0808a020();
}
