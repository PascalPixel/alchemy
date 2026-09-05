#include "types.h"
#include "staged_actor_effect.h"

#define StagedActor_RunStepEffect Func_02000b1c

extern struct StagedActorEffect *Func_0200374e(s32 actor_index);
extern s32 Func_0200371e(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
extern void Func_02003750(void);
extern void Func_020036f0(struct StagedActorEffect *actor, s32 mode);
extern void Func_020036d6(s32 actor_index);
extern void Func_0200387c(s32 effect_id);
extern void Func_02003704(struct StagedActorEffect *actor, s32 mode);
extern void Func_0200376e(struct StagedActorEffect *actor, s32 mode);
extern s32 Func_020037ec(s32 layer, s32 cell_x, s32 cell_z);
extern void Func_0200373c(struct StagedActorEffect *actor, s32 mode);
extern void Func_0200378c(struct StagedActorEffect *actor, s32 mode);
extern void Func_02003830(s32 frames, s32 mode);
extern void Func_02003744(s32 mode);
extern void Func_02003756(s32 frames);
extern void Func_0200376c(s32 mode);
extern void Func_02003810(void);
#define GetStagedActorEffect Func_0200374e
#define CanStartStagedActorEffect Func_0200371e
#define BeginStagedActorEffect Func_02003750
#define SetStagedActorEffectMode Func_020036f0
#define SelectStagedActorEffectSlot Func_020036d6
#define StartStagedActorEffectSound Func_0200387c
#define SetStagedActorMotionMode Func_02003704
#define PrepareStagedActorEffect Func_0200376e
#define TestStagedActorEffectCell Func_020037ec
#define StartStagedActorEffectMove Func_0200373c
#define SetStagedActorEffectTransition Func_0200378c
#define WaitStagedActorEffect Func_02003830
#define AdvanceStagedActorEffect Func_02003744
#define WaitSceneFrames Func_02003756
#define RestoreStagedActorEffect Func_0200376c
#define FinishStagedActorEffect Func_02003810

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request) {
    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 ret = CanStartStagedActorEffect(actor, request);

    if (ret == 0) {
        BeginStagedActorEffect();
        SetStagedActorEffectMode(actor, 6);
        SelectStagedActorEffectSlot(6);
        StartStagedActorEffectSound(152);
        SetStagedActorMotionMode(actor, 7);
        actor->move_rate_x = 0x30000;
        actor->move_rate_z = 0x20000;
        actor->elevation_rate = 0x40000;
        *flags &= 0x7e;
        PrepareStagedActorEffect(actor, 0);
        TestStagedActorEffectCell(0, request->cell_x, request->cell_z);
        StartStagedActorEffectMove(actor, 6);
        SetStagedActorEffectTransition(actor, 1);
        *flags = (u8)ret;
        WaitStagedActorEffect(10, 7);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        AdvanceStagedActorEffect(2);
        actor->position_x += 0xffff0000;
        actor->position_z += 0xffff0000;
        WaitSceneFrames(10);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        RestoreStagedActorEffect(4);
        actor->position_x += 0x10000;
        actor->position_z += 0x10000;
        *flags = saved;
        FinishStagedActorEffect();
        return 1;
    }
    return 0;
}
