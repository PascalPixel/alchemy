#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DATA_PRESET_LAYOUT
#define ALCHEMY_ABI_OVERLAYS_SCENE_DATA_PRESET_LAYOUT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020001cc();
s32 Func_02000bb4(s32, s32);
void Func_02000c8a(s32);
void Func_020010e4(s32, s32, s32, s32, s32, s32);
void Func_0200118c(void);
void Func_020011e4(s32, s32, s32, s32, s32, s32);
void Func_020011fa(s32, s32, s32, s32, s32, s32);
void Func_0200122a();
void Func_0200123e();
void Func_02001254();
void Func_02001272();
void Func_02001284();
void Func_0200129c();
void Func_020012a6();
void Func_020012b8();
void Func_020012d0();
void Func_020012b6();
void Func_02001324();
void Func_02001340();
void Func_02001312();
void Func_02001378();
void Func_0200139e();
void Func_02000e70(s16 *records, s32 value);
void Func_020013bc();
void Func_02000e8e(s16 *records, s32 value);
void Func_020004a6(void);
void Func_02001952();
void Func_02001958();
void Func_02001982();
void Func_0200198a();
void Func_0200199a();
void Func_020019a4();
void Func_020019a4_a();
void Func_020019b4();
void Func_020019ba();
void Func_020019ba_a();
void Func_020019cc();
void Func_020019ce();
void Func_020019d2();
void Func_020019de();
void Func_020019ea();
void Func_020019f4();
void Func_02001a02();
void Func_02001a08();
void Func_02001a42();
void Func_02001a48();
void Func_02001d9e(void);
s32 Func_02001d64(s32, s32);
void Func_02001e22(void);
void Func_02001e06(void);
void Func_02001fc0(s32, s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define Effect_AdjustColorChannels Func_02000ecc
#define Effect_AdjustPaletteColors Func_02000e64
#define SceneData_Apply Func_02000bb4
#define SceneData_Apply2 Func_02000e70
#define SceneData_Apply3 Func_02000e8e
#define SceneData_Apply4 Func_02001d64
#define SceneData_Apply5 Func_02001fc0
#define SceneData_Do2 Func_02000c8a
#define SceneData_FindTileRunAt Func_02000b8c
#define SceneData_GetTable9170 Func_02000030
#define SceneData_GetTable91d0 Func_0200003c
#define SceneData_GetTable91e0 Func_02000044
#define SceneData_GetTable9240 Func_0200004c
#define SceneData_ReturnZero Func_02000038
#define SceneData_Run Func_02001ee2
#define SceneData_Run10 Func_02001284
#define SceneData_Run11 Func_0200129c
#define SceneData_Run12 Func_020012a6
#define SceneData_Run13 Func_020012b8
#define SceneData_Run14 Func_020012d0
#define SceneData_Run15 Func_020012b6
#define SceneData_Run16 Func_02001324
#define SceneData_Run17 Func_02001340
#define SceneData_Run18 Func_02001312
#define SceneData_Run19 Func_02001378
#define SceneData_Run2 Func_02001ef0
#define SceneData_Run20 Func_0200139e
#define SceneData_Run21 Func_020013bc
#define SceneData_Run22 Func_020004a6
#define SceneData_Run23 Func_02001952
#define SceneData_Run24 Func_0200199a
#define SceneData_Run25 Func_02001958
#define SceneData_Run26 Func_020019de
#define SceneData_Run27 Func_020019ba
#define SceneData_Run28 Func_020019a4
#define SceneData_Run29 Func_02001982
#define SceneData_Run3 Func_02001efe
#define SceneData_Run30 Func_020019d2
#define SceneData_Run31 Func_020019ce
#define SceneData_Run32 Func_020019a4_a
#define SceneData_Run33 Func_020019f4
#define SceneData_Run34 Func_020019ea
#define SceneData_Run35 Func_02001a02
#define SceneData_Run36 Func_02001a48
#define SceneData_Run37 Func_02001a08
#define SceneData_Run38 Func_02001d9e
#define SceneData_Run39 Func_02001e22
#define SceneData_Run4 Func_020001cc
#define SceneData_Run40 Func_02001e06
#define SceneData_Run5 Func_0200118c
#define SceneData_Run6 Func_0200122a
#define SceneData_Run7 Func_0200123e
#define SceneData_Run8 Func_02001254
#define SceneData_Run9 Func_02001272
#define SceneData_SetRect3 Func_020010e4
#define SceneData_SetRect4 Func_020011e4
#define SceneData_SetRect5 Func_020011fa
#define Scene_CallHelper Func_0200008c
#define Scene_RepaintBoardRecords Func_02000194
#define Scene_RunActorEightSequence Func_020008b0
#define State_ApplyRecordTable92c0 Func_02000054
#define State_ApplyRectByLayoutSelector Func_02000150
#define State_CheckFourCellRun Func_02000be4
#define State_CopyPresetA0d0WithOffsetB0 Func_020003c0
#define State_StampRecordCells Func_02000b3c
#define gOv Data_020092c0
#define gOv2 Data_020092c8
#define gOv3 Data_020092c4
#define gUnk Data_0202c000
#define gUnk2 Data_02010000
#define SceneData_Place(a0, a1, a2) Call3(Func_0200198a, a0, a1, a2)
#define SceneData_Place2(a0, a1, a2) Call3(Func_020019b4, a0, a1, a2)
#define SceneData_Do(a0) Call1(Func_02001a42, a0)
#define SceneData_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_020019ba_a, a0, a1, a2, a3, a4, a5)
#define SceneData_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_020019cc, a0, a1, a2, a3, a4, a5)

#endif
