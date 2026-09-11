#ifndef ALCHEMY_ABI_SHOP_SEL_EQUIP
#define ALCHEMY_ABI_SHOP_SEL_EQUIP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077218(s32 unit_id, s32 item_id);
s32 Func_08077228(s32 unit_id, u8 kind);
void Func_08077050(s32 unit_id, s32 slot);
void Func_08015120(u32 unit_id, u32 mode);
void Func_080b04dc(s32 message);
s32 Func_080b0634(s32 value);
void Func_080b1dec(s32 value, s32 unit_id);
void Func_080f9010(s32 cue);
void Func_08015140(void);
s32 Func_08015038(s32 a, s32 b, s32 c, s32 d);
s32 Func_08015048(void);
void Func_080b0574(s32 message);

#define Sys_Apply Func_08077218
#define Sys_Apply2 Func_08077228
#define Sys_Apply3 Func_08015120
#define Sys_Apply4 Func_08077050
#define Sys_Apply5 Func_080b1dec
#define Sys_Check Func_080b0634
#define Sys_Check2 Func_08015048
#define Sys_Do Func_080b04dc
#define Sys_Do2 Func_080f9010
#define Sys_Do3 Func_080b0574
#define Sys_Run Func_08077008
#define Sys_Run2 Func_08015140
#define Sys_SetMode Func_08015038
#define gIw Data_03001f2c

#endif
