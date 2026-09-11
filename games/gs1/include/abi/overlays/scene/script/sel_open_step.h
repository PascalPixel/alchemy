#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_02000FFC
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_02000FFC

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02006a5e();
void Func_02006ab6();
void Func_02006ae4();
void Func_02006aee();
void Func_02006afc();
s32 Func_02006b00();
void Func_02006b3a();
void Func_02006b6e();
void Func_02006bb6();
void Func_02006c6c();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Scene_RunSel Func_02000ffc
#define Script_Check Func_02006b00
#define Script_Run Func_02006ab6
#define Script_Run2 Func_02006c6c
#define Script_Run3 Func_02006bb6
#define Script_Run4 Func_02006a5e
#define Script_Run5 Func_02006ae4
#define Script_Run6 Func_02006b6e
#define Script_Run7 Func_02006afc
#define gWork Data_03001ebc
#define Script_Place(a0, a1, a2) Call3(Func_02006aee, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02006b3a, a0, a1, a2)

#endif
