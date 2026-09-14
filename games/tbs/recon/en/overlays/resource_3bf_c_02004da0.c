/* Draft C: complete owner reviewed, but linked bytes do not yet match. */
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"

#define FieldScene_DispatchActorUpdate Func_02004da0

extern struct EventPairWork1c0 Data_02000240;
extern u32 Data_0200dfa4;
extern struct EventRuntime *Data_03001ebc;
extern u8 SceneEvent_InstallTasks[];
extern u8 SceneEvent_SetupActors[];
extern u8 SceneEvent_RestoreActors[];
extern u8 SceneEvent_UpdateActors[];

extern u32 Func_0200d5a0(void);
extern void Func_0200d5f8(u32);
extern void Func_0200cf60(void);
extern void Func_0200d0e4(void);
extern void Func_0200d324(void);
extern struct ObjectRuntime *Func_0200d650(s32);
extern void Func_0200d5e8(struct ObjectRuntime *, s32);
extern void Func_0200d708(s32, s32, s32);
extern void Func_0200d6a8(s32, s32);
extern void Func_0200d520(void);
extern void Func_0200b054(void);
extern s32 Func_0200d610(s32);
extern void Func_0200d6a0(s32, s32, s32);

static __inline__ void Scene_Call1(void (*func)(u32), u32 value)
{
    func(value);
}

static __inline__ void Scene_Call3(
    void (*func)(s32, s32, s32), s32 id, s32 x, s32 z)
{
    func(id, x, z);
}

s32 FieldScene_DispatchActorUpdate(void)
{
    struct ObjectRuntime *actor;

    Data_0200dfa4 = Func_0200d5a0() * 7 >> 16;
    if ((s16)Data_02000240.first == (s32)SceneEvent_InstallTasks) {
        Scene_Call1(Func_0200d5f8, 0xe00);
        Func_0200cf60();
    }
    if ((s16)Data_02000240.first == (s32)SceneEvent_SetupActors)
        Func_0200d0e4();
    if ((s16)Data_02000240.first == (s32)SceneEvent_RestoreActors)
        Func_0200d324();
    if ((s16)Data_02000240.first == (s32)SceneEvent_UpdateActors) {
        Data_03001ebc->value_1c0 = 0x204;
        Func_0200d5e8(Func_0200d650(12), 0);
        Func_0200d708(12, 0, 0);
        Func_0200d6a8(12, 0);
        Func_0200d520();
        actor = Func_0200d650(8);
        if (actor != 0)
            Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Func_0200d650(9);
        if (actor != 0)
            Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Func_0200d650(10);
        if (actor != 0)
            Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        Scene_Call1(Func_0200d5f8, 0xe00);
        if ((s16)Data_02000240.second == 4) {
            Scene_Call1(Func_0200d5f8, 0xc00);
            Func_0200b054();
        }
        if ((s16)Data_02000240.second == 3) {
            Scene_Call1(Func_0200d5f8, 0xc00);
            if (Func_0200d610(0x941) != 0) {
                Func_0200d6a0(12, 0, 0);
                Scene_Call3(Func_0200d6a0, 16, 0x1b00000, 0x1580000);
                Scene_Call3(Func_0200d708, 16, 0x5000, 0);
                Scene_Call3(Func_0200d6a0, 13, 0x1c80000, 0x1200000);
                Scene_Call3(Func_0200d708, 13, 0x5000, 0);
                Scene_Call3(Func_0200d6a0, 17, 0x1c80000, 0x1400000);
                Func_0200d5e8(Func_0200d650(17), 0);
            }
        }
        actor = Func_0200d650(15);
        if (actor != 0)
            Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        *(s32 *)&actor->unknown_18[0] = 0xcccc;
    }
    return 0;
}
