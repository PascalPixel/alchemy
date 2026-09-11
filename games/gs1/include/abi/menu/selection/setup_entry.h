#ifndef ALCHEMY_ABI_MENU_SELECTION_SETUP_ENTRY
#define ALCHEMY_ABI_MENU_SELECTION_SETUP_ENTRY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void Func_08019fcc(u32 glyph, s32 includeBase, s32 *sourceIndex,
                   s32 *result, s32 reuseSource);
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#define Menu_Run Func_0801bd98
#define Menu_SetRange Func_08019ee4
#define Menu_SetRange2 Func_08019fcc
#define Menu_SetRange3 Func_0801a3d0

#endif
