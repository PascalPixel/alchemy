#ifndef ALCHEMY_ABI_SHOP_SEL_BUY_DONE
#define ALCHEMY_ABI_SHOP_SEL_BUY_DONE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077028(s32, s32);
s32 Func_08077228(s32, u8);
s32 Func_08077230(s32);
s32 Func_08077298(s16);
void Func_080b0574(s32 message);

#define Sys_Apply Func_08077228
#define Sys_Apply2 Func_08077028
#define Sys_Check Func_08077230
#define Sys_Check2 Func_08077298
#define Sys_Do Func_080b0574

#endif
