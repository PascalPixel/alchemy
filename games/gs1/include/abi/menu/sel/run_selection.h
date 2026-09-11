#ifndef ALCHEMY_ABI_MENU_SEL_RUN_SELECTION
#define ALCHEMY_ABI_MENU_SEL_RUN_SELECTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0801c0dc(struct TextObject *object, s32 *resource);
s16 Func_08029094(
    struct Work *work,
    s16 primary,
    s16 *secondary,
    s16 *mode);
void Func_0801c17c(s32 resource);
void Func_0808a238(s16 primary, s16 secondary);
void Func_0801c154(struct TextObject *object, s32 x, s32 y);

#define Menu_Apply Func_0801c0dc
#define Menu_Apply2 Func_0808a238
#define Menu_Do Func_0801c17c
#define Menu_Place Func_0801c154
#define Menu_SetMode Func_08029094
#define gCell Data_02000240
#define gIw Data_03001ae8

#endif
