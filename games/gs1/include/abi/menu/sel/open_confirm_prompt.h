#ifndef ALCHEMY_ABI_MENU_SEL_OPEN_CONFIRM_PROMPT
#define ALCHEMY_ABI_MENU_SEL_OPEN_CONFIRM_PROMPT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015408(s32 x, s32 y, s32 width, s32 height);
void Func_080a1090(s32 unused);
s32 Func_08077158(const u16 *ids);
void Func_080a3354(s32, s32, s32, s32);
void Func_080a2144(s32 index);
void Func_08015418(s32 addr);
s32 Func_080a5cc0(s32 *, s32 *, s32 *);
void Func_08015278(s32 screen_handle);
void Func_080a34c0(void);
void Func_08002dd8(s32 kind);
void Func_08015410(s32 x, s32 y, s32 width, s32 height);
void Func_0808a548(void);

#define Menu_Check Func_08077158
#define Menu_Do Func_080a1090
#define Menu_Do2 Func_080a2144
#define Menu_Do3 Func_08015418
#define Menu_Do4 Func_08015278
#define Menu_Do5 Func_08002dd8
#define Menu_Place Func_080a5cc0
#define Menu_Run Func_080a34c0
#define Menu_Run2 Func_0808a548
#define Menu_SetMode Func_08015408
#define Menu_SetMode2 Func_080a3354
#define Menu_SetMode3 Func_08015410
#define gIw Data_03001e68

#endif
