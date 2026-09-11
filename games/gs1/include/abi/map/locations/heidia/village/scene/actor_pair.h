#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SCENE_ACTOR_PAIR
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SCENE_ACTOR_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02007cd6();
void Func_02007d22();
void Func_02007d30();
void Func_02007d34();
void Func_02007d64();
void Func_02007d70();
void Func_02007d96();
void Func_02007d9c();
void Func_02007dae();
void Func_02007db4();
void Func_02007de8();
void Func_02007dec();
void Func_02007e20();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Map_Run Func_02007dec
#define Map_Run2 Func_02007de8
#define Map_Run3 Func_02007cd6
#define Map_Run4 Func_02007db4
#define Map_Run5 Func_02007e20
#define Map_Run6 Func_02007d96
#define Map_Run7 Func_02007dae
#define Map_Run8 Func_02007d34
#define Scene_RunScene3bf Func_0200269c
#define gWork Data_03001ebc
#define Map_Do(a0) Call1(Func_02007d9c, a0)
#define Map_Place(a0, a1, a2) Call3(Func_02007d22, a0, a1, a2)
#define Map_Place2(a0, a1, a2) Call3(Func_02007d30, a0, a1, a2)
#define Map_Place3(a0, a1, a2) Call3(Func_02007d64, a0, a1, a2)
#define Map_Place4(a0, a1, a2) Call3(Func_02007d70, a0, a1, a2)

#endif
