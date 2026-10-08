#ifndef SHIAN_JIIN_TEMPLE_H
#define SHIAN_JIIN_TEMPLE_H
extern struct EventWork *gEventWork;

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) ((void (*)())Engine_ActorSetAnimationAndWait)(args)

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor_02000484 {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor_020041ec {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern u8 ShianJiin_XianMessages[];

s32 ArcTan2(s32, s32);

void FieldScene_SpawnRandomizedParticle();
void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn();
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void ShianJiin_SpinAway(void);
void ShianJiin_RunGatheringScene(void);
void ShianJiin_RunMasterScene(s32 arg0);
void ShianJiin_SpinAway(void);
void ShianJiin_RunMasterScene(s32 variant);
void ShianJiin_SpinAway(void);
void ShianJiin_RunMasterScene();
void ShianJiin_RunGatheringScene(void);
void ShianJiin_RunMasterScene();
void FieldScene_RunOpeningAuxiliarySequence(s32 arg0, s32 arg1);
void FieldScene_RunOpeningAuxiliarySequence(s32 arg0, s32 arg1);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void UiText_DrawQuantityPairWithCue();

void FieldScene_RunForwardArcBurst(void);

void FieldScene_RunDescentBurst(void);

void Effect_Spawn(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

extern u8 ShianJiin_ActorTwelveScript[];
extern u8 ShianJiin_CellStepsA[];
extern u8 ShianJiin_CellStepsB[];
extern u8 ShianJiin_CellStepsC[];
extern u8 ShianJiin_ShotScript[];

#endif
