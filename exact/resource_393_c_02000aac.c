#include "types.h"
#include "staged_actor_effect.h"

extern struct StagedActorEffect *Func_0200193e(s32 actor_index);
extern s32 Func_02001916(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
extern void Func_02001948(void);
extern void Func_02001908(struct StagedActorEffect *actor, s32 mode);
extern void Func_02001906(s32 actor_index);
extern s32 Func_020019b4();
extern void Func_0200191c(struct StagedActorEffect *actor, s32 mode);
extern void Func_02001966(struct StagedActorEffect *actor, s32 mode);
extern void Func_02001954(struct StagedActorEffect *actor, s32 mode);
extern void Func_02001984(struct StagedActorEffect *actor, s32 mode);
extern void Func_020019e8(s32 frames, s32 mode);
extern void Func_02001974(s32 mode);
extern void Func_02001986(s32 frames);
extern void Func_0200199c(s32 mode);
extern void Func_02001a08(void);

#define GetStagedActorEffect Func_0200193e
#define CanStartStagedActorEffect Func_02001916
#define BeginStagedActorEffect Func_02001948
#define SetStagedActorEffectMode Func_02001908
#define SelectStagedActorEffectSlot Func_02001906
#define StartStagedActorEffectSound Func_020019b4
#define SetStagedActorMotionMode Func_0200191c
#define PrepareStagedActorEffect Func_02001966
#define TestStagedActorEffectCell Func_020019b4
#define StartStagedActorEffectMove Func_02001954
#define SetStagedActorEffectTransition Func_02001984
#define WaitStagedActorEffect Func_020019e8
#define AdvanceStagedActorEffect Func_02001974
#define WaitSceneFrames Func_02001986
#define RestoreStagedActorEffect Func_0200199c
#define FinishStagedActorEffect Func_02001a08
#define RunStagedActorStepEffect Func_02000aac

s32 RunStagedActorStepEffect(struct StagedActorEffectRequest *request) {
    struct StagedActorEffect *actor = GetStagedActorEffect(0);
    u8 *flags = &actor->motion_flags;
    u8 saved = *flags;
    s32 result = CanStartStagedActorEffect(actor, request);

    if (result == 0) {
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
        *flags = (u8)result;
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
