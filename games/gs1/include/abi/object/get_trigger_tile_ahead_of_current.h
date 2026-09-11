#ifndef ALCHEMY_ABI_OBJECT_GET_TRIGGER_TILE_AHEAD_OF_CURRENT
#define ALCHEMY_ABI_OBJECT_GET_TRIGGER_TILE_AHEAD_OF_CURRENT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0800447c(s32, s32, struct Vec *);
s32 Func_080091a8(s32, s32, s32);
s32 Func_0808d48c(s32, s32);

#define Obj_Apply Func_0808d48c
#define Obj_Place Func_0800447c
#define Obj_Place2 Func_080091a8
#define Obj_Run Func_0808ba1c
#define Obj_Run2 Func_0808ce74
#define gCell Data_02000240

#endif
