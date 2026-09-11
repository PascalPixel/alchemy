#include "types.h"

#define GameFlag_Set_1(a0) Value1(Func_02000d84, a0)
#define GameFlag_IsSet_1(a0) Value1(Func_02000d82, a0)
#define Audio_PlayCue_1(a0) Value1(Func_02000e8e, a0)
#define Battle_Reset_1() Value0(Func_02000daa)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02000e5c, a0, a1, a2, a3)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02000df8, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02000e64, a0, a1, a2)
#define Motion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Value4(Func_02000ea4, a0, a1, a2, a3)
#define Motion_CommitPos_1(a0) Value1(Func_02000e22, a0)
#define Object_CommitPositionThenWaitIfModeZero_1() Value0(Func_02000e96)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02000e6c, a0)
#define Battle_WaitMode0_1(a0) Value1(Func_02000df2, a0)
#define Motion_SetVarCbObj_1(a0, a1) Value2(Func_02000e62, a0, a1)
#define Battle_WaitMode0_2(a0) Value1(Func_02000e00, a0)
#define BattleEv_RunWait_1(a0, a1) Value2(Func_02000e98, a0, a1)
#define Battle_WaitMode0_3(a0) Value1(Func_02000e0e, a0)
#define Motion_SetVarCbObj_2(a0, a1) Value2(Func_02000e7e, a0, a1)
#define Battle_WaitMode0_4(a0) Value1(Func_02000e1c, a0)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02000e4e, a0, a1, a2)
#define Motion_CommitPositionAndActivate_1(a0, a1, a2) Call3(Func_02000f02, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Value3(Func_02000ed4, a0, a1, a2)
#define Battle_WaitMode0_5(a0) Value1(Func_02000e42, a0)
#define Motion_ArmCb_3(a0, a1, a2) Value3(Func_02000ee6, a0, a1, a2)
#define Battle_WaitMode0_6(a0) Value1(Func_02000e54, a0)
#define Motion_ArmCb_4(a0, a1, a2) Value3(Func_02000ef6, a0, a1, a2)
#define Battle_WaitMode0_7(a0) Value1(Func_02000e64_a, a0)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02000f10, a0, a1, a2)
#define BattleEv_RunWait_2(a0, a1) Value2(Func_02000f08, a0, a1)
#define Battle_WaitMode0_8(a0) Value1(Func_02000e7e_a, a0)
#define Motion_SetVarCbObj_3(a0, a1) Value2(Func_02000eee, a0, a1)
#define Battle_WaitMode0_9(a0) Value1(Func_02000e8c, a0)
#define BattleEv_RunWait_3(a0, a1) Value2(Func_02000f24, a0, a1)
#define Battle_WaitMode0_10(a0) Value1(Func_02000e9a, a0)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02000ecc, a0, a1, a2)
#define Motion_CommitPositionAndActivate_2(a0, a1, a2) Call3(Func_02000f80, a0, a1, a2)
#define Motion_CommitPositionAndActivate_3(a0, a1, a2) Call3(Func_02000f8a, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Value3(Func_02000f5e, a0, a1, a2)
#define Battle_WaitMode0_11(a0) Value1(Func_02000ecc_a, a0)
#define BattleEv_RunWait_4(a0, a1) Value2(Func_02000f64, a0, a1)
#define Battle_WaitMode0_12(a0) Value1(Func_02000eda, a0)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02000f86, a0, a1, a2)
#define BattleEv_RunWait_5(a0, a1) Value2(Func_02000f7e, a0, a1)
#define Battle_WaitMode0_13(a0) Value1(Func_02000ef4, a0)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02000fa0, a0, a1, a2)
#define BattleEv_RunWait_6(a0, a1) Value2(Func_02000f98, a0, a1)
#define Battle_WaitMode0_14(a0) Value1(Func_02000f0e, a0)
#define Motion_ArmCb_6(a0, a1, a2) Value3(Func_02000fb2, a0, a1, a2)
#define Battle_WaitMode0_15(a0) Value1(Func_02000f20, a0)
#define BattleEv_RunWait_7(a0, a1) Value2(Func_02000fb8, a0, a1)
#define Battle_WaitMode0_16(a0) Value1(Func_02000f2e, a0)
#define Motion_SetVarCbObj_4(a0, a1) Value2(Func_02000f9e, a0, a1)
#define Battle_WaitMode0_17(a0) Value1(Func_02000f3c, a0)
#define Motion_CallWaitAnim_1(a0, a1) Value2(Func_02000f9c, a0, a1)
#define Battle_WaitMode0_18(a0) Value1(Func_02000f4a, a0)
#define BattleEv_RunWait_8(a0, a1) Value2(Func_02000fe2, a0, a1)
#define Battle_WaitMode0_19(a0) Value1(Func_02000f58, a0)
#define Motion_ArmCb_7(a0, a1, a2) Value3(Func_02000ffc, a0, a1, a2)
#define Battle_WaitMode0_20(a0) Value1(Func_02000f6a, a0)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02001014, a0, a1, a2)
#define BattleEv_RunWait_9(a0, a1) Value2(Func_0200100c, a0, a1)
#define Battle_WaitMode0_21(a0) Value1(Func_02000f82, a0)
#define Motion_SetVarCb_1(a0, a1) Call2(Func_02000fea, a0, a1)
#define Motion_SetVarCbObj_5(a0, a1) Value2(Func_02000ffa, a0, a1)
#define Battle_WaitMode0_22(a0) Value1(Func_02000f98_a, a0)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02000fca, a0, a1, a2)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_02000fe6, a0, a1, a2)
#define Motion_CommitPositionAndActivate_4(a0, a1, a2) Call3(Func_02001088, a0, a1, a2)
#define Motion_ArmCb_8(a0, a1, a2) Value3(Func_0200105a, a0, a1, a2)
#define Battle_WaitMode0_23(a0) Value1(Func_02000fc8, a0)
#define Battle_WaitMode0_24(a0) Value1(Func_02000fce, a0)
#define Motion_CallWaitAnim_2(a0, a1) Value2(Func_0200102e, a0, a1)
#define Battle_WaitMode0_25(a0) Value1(Func_02000fdc, a0)
#define BattleEv_RunWait_10(a0, a1) Value2(Func_02001074, a0, a1)
#define Battle_WaitMode0_26(a0) Value1(Func_02000fea_a, a0)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_02001096, a0, a1, a2)
#define BattleEv_RunWait_11(a0, a1) Value2(Func_0200108e, a0, a1)
#define Battle_WaitMode0_27(a0) Value1(Func_02001004, a0)
#define Motion_SetVarCbObj_6(a0, a1) Value2(Func_02001074_a, a0, a1)
#define Battle_WaitMode0_28(a0) Value1(Func_02001012, a0)
#define BattleEv_RunWait_12(a0, a1) Value2(Func_020010aa, a0, a1)
#define Battle_WaitMode0_29(a0) Value1(Func_02001020, a0)
#define Motion_SetAngleToward_1(a0, a1, a2) Call3(Func_0200109a, a0, a1, a2)
#define Motion_ArmCb_9(a0, a1, a2) Value3(Func_020010ce, a0, a1, a2)
#define Battle_WaitMode0_30(a0) Value1(Func_0200103c, a0)
#define BattleEv_RunWait_13(a0, a1) Value2(Func_020010d4, a0, a1)
#define Battle_WaitMode0_31(a0) Value1(Func_0200104a, a0)
#define Motion_SetVarCbObj_7(a0, a1) Value2(Func_020010ba, a0, a1)
#define Battle_WaitMode0_32(a0) Value1(Func_02001058, a0)
#define BattleEv_RunWait_14(a0, a1) Value2(Func_020010f0, a0, a1)
#define Battle_WaitMode0_33(a0) Value1(Func_02001066, a0)
#define Motion_SetAngleToward_2(a0, a1, a2) Call3(Func_020010e0, a0, a1, a2)
#define Motion_ArmCb_10(a0, a1, a2) Value3(Func_02001114, a0, a1, a2)
#define Battle_WaitMode0_34(a0) Value1(Func_02001082, a0)
#define Motion_CallWaitAnim_3(a0, a1) Value2(Func_020010e2, a0, a1)
#define Battle_WaitMode0_35(a0) Value1(Func_02001090, a0)
#define BattleEv_RunWait_15(a0, a1) Value2(Func_02001128, a0, a1)
#define Battle_WaitMode0_36(a0) Value1(Func_0200109e, a0)
#define Motion_SetVarCbObj_8(a0, a1) Value2(Func_0200110e, a0, a1)
#define Battle_WaitMode0_37(a0) Value1(Func_020010ac, a0)
#define Motion_CallWaitAnim_4(a0, a1) Value2(Func_0200110c, a0, a1)
#define Battle_WaitMode0_38(a0) Value1(Func_020010ba_a, a0)
#define BattleEv_RunWait_16(a0, a1) Value2(Func_02001152, a0, a1)
#define Battle_WaitMode0_39(a0) Value1(Func_020010c8, a0)
#define Object_LinkPair_1(a0, a1, a2) Call3(Func_0200114a, a0, a1, a2)
#define BattleEv_RunWait_17(a0, a1) Value2(Func_0200116a, a0, a1)
#define Battle_WaitMode0_40(a0) Value1(Func_020010e0_a, a0)
#define Motion_SetVarCbObj_9(a0, a1) Value2(Func_02001150, a0, a1)
#define Battle_WaitMode0_41(a0) Value1(Func_020010ee, a0)
#define BattleEv_RunWait_18(a0, a1) Value2(Func_02001186, a0, a1)
#define Battle_WaitMode0_42(a0) Value1(Func_020010fc, a0)
#define Motion_SetAngleToward_3(a0, a1, a2) Call3(Func_02001176, a0, a1, a2)
#define Motion_ArmCb_11(a0, a1, a2) Value3(Func_020011aa, a0, a1, a2)
#define Battle_WaitMode0_43(a0) Value1(Func_02001118, a0)
#define Battle_WaitMode0_44(a0) Value1(Func_0200111e, a0)
#define Motion_SetVarCbObj_10(a0, a1) Value2(Func_0200118e, a0, a1)
#define Battle_WaitMode0_45(a0) Value1(Func_0200112c, a0)
#define BattleEv_RunWait_19(a0, a1) Value2(Func_020011c4, a0, a1)
#define Battle_WaitMode0_46(a0) Value1(Func_0200113a, a0)
#define Motion_SetAngleToward_4(a0, a1, a2) Call3(Func_020011b4, a0, a1, a2)
#define Motion_ArmCb_12(a0, a1, a2) Value3(Func_020011e8, a0, a1, a2)
#define Battle_WaitMode0_47(a0) Value1(Func_02001156, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020011ae, a0, a1)
#define Motion_CallWaitAnim_5(a0, a1) Value2(Func_020011be, a0, a1)
#define Battle_WaitMode0_48(a0) Value1(Func_0200116c, a0)
#define Battle_WaitMode0_49(a0) Value1(Func_02001172, a0)
#define Motion_CallWaitAnim_6(a0, a1) Value2(Func_020011d2, a0, a1)
#define Battle_WaitMode0_50(a0) Value1(Func_02001180, a0)
#define BattleEv_RunWait_20(a0, a1) Value2(Func_0200122e, a0, a1)
#define Battle_WaitMode0_51(a0) Value1(Func_020011a4, a0)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_02001250, a0, a1, a2)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Value3(Func_0200125c, a0, a1, a2)
#define Battle_WaitMode0_52(a0) Value1(Func_020011c2, a0)
#define Motion_SetAngleToward_5(a0, a1, a2) Call3(Func_0200123c, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Value3(Func_02001270, a0, a1, a2)
#define Battle_WaitMode0_53(a0) Value1(Func_020011de, a0)
#define Battle_WaitMode0_54(a0) Value1(Func_020011e4, a0)
#define Motion_SetVarCbObj_11(a0, a1) Value2(Func_02001254, a0, a1)
#define Battle_WaitMode0_55(a0) Value1(Func_020011f2, a0)
#define BattleEv_RunWait_21(a0, a1) Value2(Func_0200128a, a0, a1)
#define Battle_WaitMode0_56(a0) Value1(Func_02001200, a0)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Call3(Func_020012ac, a0, a1, a2)
#define Motion_SetAngleToward_6(a0, a1, a2) Value3(Func_02001286, a0, a1, a2)
#define Battle_WaitMode0_57(a0) Value1(Func_0200121c, a0)
#define Motion_SetVarCbObj_12(a0, a1) Value2(Func_0200128c, a0, a1)
#define Battle_WaitMode0_58(a0) Value1(Func_0200122a, a0)
#define Battle_WaitMode0_59(a0) Value1(Func_02001230, a0)
#define Motion_ArmCb_14(a0, a1, a2) Value3(Func_020012d4, a0, a1, a2)
#define Battle_WaitMode0_60(a0) Value1(Func_02001242, a0)
#define BattleEv_RunWait_22(a0, a1) Value2(Func_020012da, a0, a1)
#define Battle_WaitMode0_61(a0) Value1(Func_02001250_a, a0)
#define Motion_SetVarCbObj_13(a0, a1) Value2(Func_020012c0, a0, a1)
#define Battle_WaitMode0_62(a0) Value1(Func_0200125e, a0)
#define Motion_CallWaitAnim_7(a0, a1) Value2(Func_020012be, a0, a1)
#define Battle_WaitMode0_63(a0) Value1(Func_0200126c, a0)
#define Battle_WaitMode0_64(a0) Value1(Func_02001272, a0)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Call3(Func_0200131e, a0, a1, a2)
#define BattleEv_RunWait_23(a0, a1) Value2(Func_02001316, a0, a1)
#define Battle_WaitMode0_65(a0) Value1(Func_0200128c_a, a0)
#define Motion_CallWaitAnim_8(a0, a1) Value2(Func_020012ec, a0, a1)
#define Battle_WaitMode0_66(a0) Value1(Func_0200129a, a0)
#define BattleEv_RunWait_24(a0, a1) Value2(Func_02001332, a0, a1)
#define Battle_WaitMode0_67(a0) Value1(Func_020012a8, a0)
#define BattleFx_SpawnLinked_10(a0, a1, a2) Call3(Func_02001352, a0, a1, a2)
#define Battle_WaitMode0_68(a0) Value1(Func_020012b8, a0)
#define Motion_SetAngleToward_7(a0, a1, a2) Call3(Func_02001332_a, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Value3(Func_02001366, a0, a1, a2)
#define Battle_WaitMode0_69(a0) Value1(Func_020012d4_a, a0)
#define Battle_WaitMode0_70(a0) Value1(Func_020012da_a, a0)
#define Motion_SetVarCbObj_14(a0, a1) Value2(Func_0200134a, a0, a1)
#define Battle_WaitMode0_71(a0) Value1(Func_020012e8, a0)
#define BattleEv_RunWait_25(a0, a1) Value2(Func_02001380, a0, a1)
#define Battle_WaitMode0_72(a0) Value1(Func_020012f6, a0)
#define BattleFx_SpawnLinked_11(a0, a1, a2) Call3(Func_020013a2, a0, a1, a2)
#define BattleEv_RunWait_26(a0, a1) Value2(Func_0200139a, a0, a1)
#define Battle_WaitMode0_73(a0) Value1(Func_02001310, a0)
#define Motion_SetVarCbObj_15(a0, a1) Value2(Func_02001380_a, a0, a1)
#define Battle_WaitMode0_74(a0) Value1(Func_0200131e_a, a0)
#define BattleEv_RunWait_27(a0, a1) Call2(Func_020013b6, a0, a1)
#define Motion_SetVarCbObj_16(a0, a1) Value2(Func_02001396, a0, a1)
#define Battle_WaitMode0_75(a0) Value1(Func_02001334, a0)
#define Battle_WaitMode0_76(a0) Value1(Func_0200133a, a0)
#define Object_LinkPair_2(a0, a1, a2) Value3(Func_020013bc, a0, a1, a2)
#define Battle_WaitMode0_77(a0) Value1(Func_0200134a_a, a0)
#define BattleEv_RunWait_28(a0, a1) Value2(Func_020013e2, a0, a1)
#define Battle_WaitMode0_78(a0) Value1(Func_02001358, a0)
#define Motion_CallWaitAnim_9(a0, a1) Value2(Func_020013b8, a0, a1)
#define Battle_WaitMode0_79(a0) Value1(Func_02001366_a, a0)
#define BattleEv_RunWait_29(a0, a1) Value2(Func_020013fe, a0, a1)
#define Battle_WaitMode0_80(a0) Value1(Func_02001374, a0)
#define BattleFx_SpawnLinked_12(a0, a1, a2) Call3(Func_02001420, a0, a1, a2)
#define BattleFx_SpawnLinked_13(a0, a1, a2) Call3(Func_0200142c, a0, a1, a2)
#define BattleFx_SpawnLinked_14(a0, a1, a2) Call3(Func_02001438, a0, a1, a2)
#define BattleEv_RunWait_30(a0, a1) Value2(Func_02001430, a0, a1)
#define Battle_WaitMode0_81(a0) Value1(Func_020013a6, a0)
#define Motion_SetVarCbObj_17(a0, a1) Value2(Func_02001416, a0, a1)
#define Battle_WaitMode0_82(a0) Value1(Func_020013b4, a0)
#define BattleEv_RunWait_31(a0, a1) Call2(Func_0200144c, a0, a1)
#define Motion_SetVarCbObj_18(a0, a1) Value2(Func_0200142c_a, a0, a1)
#define Battle_WaitMode0_83(a0) Value1(Func_020013ca, a0)
#define Battle_WaitMode0_84(a0) Value1(Func_020013d0, a0)
#define Motion_ArmCb_16(a0, a1, a2) Value3(Func_02001474, a0, a1, a2)
#define Battle_WaitMode0_85(a0) Value1(Func_020013e2_a, a0)
#define Motion_SetVarCbObj_19(a0, a1) Value2(Func_02001452, a0, a1)
#define Battle_WaitMode0_86(a0) Value1(Func_020013f0, a0)
#define BattleEv_RunWait_32(a0, a1) Value2(Func_02001488, a0, a1)
#define Battle_WaitMode0_87(a0) Value1(Func_020013fe_a, a0)
#define Motion_SetVarCbObj_20(a0, a1) Value2(Func_0200146e, a0, a1)
#define Battle_WaitMode0_88(a0) Value1(Func_0200140c, a0)
#define BattleEv_RunWait_33(a0, a1) Value2(Func_020014a4, a0, a1)
#define Battle_WaitMode0_89(a0) Value1(Func_0200141a, a0)
#define Motion_SetAngleToward_8(a0, a1, a2) Call3(Func_02001494, a0, a1, a2)
#define Motion_ArmCb_17(a0, a1, a2) Value3(Func_020014c8, a0, a1, a2)
#define Battle_WaitMode0_90(a0) Value1(Func_02001436, a0)
#define Motion_CallWaitAnim_10(a0, a1) Value2(Func_02001496, a0, a1)
#define Battle_WaitMode0_91(a0) Value1(Func_02001444, a0)
#define BattleEv_RunWait_34(a0, a1) Value2(Func_020014dc, a0, a1)
#define Battle_WaitMode0_92(a0) Value1(Func_02001452_a, a0)
#define Motion_SetVarCb_2(a0, a1) Call2(Func_020014ba, a0, a1)
#define Motion_SetVarCbObj_21(a0, a1) Value2(Func_020014ca, a0, a1)
#define Battle_WaitMode0_93(a0) Value1(Func_02001468, a0)
#define Battle_WaitMode0_94(a0) Value1(Func_0200146e_a, a0)
#define Object_LinkPair_3(a0, a1, a2) Call3(Func_020014f0, a0, a1, a2)
#define Motion_CommitPositionAndActivate_5(a0, a1, a2) Call3(Func_02001554, a0, a1, a2)
#define Battle_WaitMode0_95(a0) Value1(Func_0200148a, a0)
#define Motion_SetSpeed_4(a0, a1) Value2(Func_0200151a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020014b2, a0, a1)
#define Battle_WaitMode0_96(a0) Value1(Func_020014a4_a, a0)
#define BattleEv_RunWait_35(a0, a1) Call2(Func_0200153c, a0, a1)
#define Battle_WaitMode0_97(a0) Call1(Func_020014c4, a0)
#define BattleEv_RunWait_36(a0, a1) Call2(Func_0200156c, a0, a1)
#define Battle_WaitMode0_98(a0) Value1(Func_020014e2, a0)
#define Motion_SetVarCbObj_22(a0, a1) Value2(Func_02001552, a0, a1)
#define Battle_WaitMode0_99(a0) Value1(Func_020014f0_a, a0)
#define BattleEv_RunWait_37(a0, a1) Value2(Func_02001588, a0, a1)
#define Battle_WaitMode0_100(a0) Value1(Func_020014fe, a0)
#define BattleFx_SpawnLinked_15(a0, a1, a2) Call3(Func_020015aa, a0, a1, a2)
#define BattleEv_RunWait_38(a0, a1) Value2(Func_020015a2, a0, a1)
#define Battle_WaitMode0_101(a0) Value1(Func_02001518, a0)
#define Motion_CallWaitAnim_11(a0, a1) Value2(Func_02001578, a0, a1)
#define Battle_WaitMode0_102(a0) Value1(Func_02001526, a0)
#define Motion_CallWaitAnim_12(a0, a1) Value2(Func_02001586, a0, a1)
#define Battle_WaitMode0_103(a0) Value1(Func_02001534, a0)
#define Audio_PlayCue_2(a0) Value1(Func_0200162a, a0)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_0200156c_a, a0, a1, a2)
#define Object_SetModeById_2(a0, a1) Call2(Func_0200159c, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_02001572, a0)
#define Motion_ResetAndSetPosition_1(a0, a1, a2) Call3(Func_02001594, a0, a1, a2)
#define Motion_CommitPos_2(a0) Value1(Func_020015aa_a, a0)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Value3(Func_020015bc, a0, a1, a2)
#define Battle_WaitMode0_104(a0) Value1(Func_0200157a, a0)
#define Audio_PlayCueForPartyMember_1() Value0(Func_0200163e)
#define Battle_SchedShoulder_1() Value0(Func_02001592)
#define STEP_COUNTER (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))
#define Scene_InitActorRecords Func_02000a78
#define SceneData_GetScriptTable Func_02000030
#define SceneData_GetTable8da4 Func_02000038
#define SceneData_GetTable8dd4 Func_02000040
#define SceneData_SelectTable8e08ByFlag96f Func_02000048
#define Dialogue_HandleFacingChoice Func_0200006c
#define Dialogue_HandleFacingBranch Func_02000100
#define Dialogue_HandleFacingAction Func_0200016c
#define Scene_RunActorCueBranch Func_020001d4
#define State_ApplyCounter16cThenCall7b Func_0200021c
#define Scene_RunPrimaryScript Func_02000240
#define SceneData_SelectTable9090ByFlag96f Func_02000a54

struct SceneRecord {
    u8 unk_00[9];
    u8 flags;
    u8 unk_0a[20];
    u16 angle;
    u8 unk_20[6];
    u8 field_26;
};

/*
 * Byte and field views of one actor share this union so that stores through
 * the linked record do not make the pointer read disappear through alias
 * analysis.
 */
union SceneActor {
    struct {
        u8 unk_00[12];
        s32 y;
        u8 unk_10[64];
        struct SceneRecord *record;
        u8 unk_54;
        u8 mode;
        u8 unk_56[3];
        u8 flags;
    } fields;
    u8 bytes[90];
};

extern u8 SceneMessage_FacingChoiceBase;
extern u8 Value_00002624;
extern s16 Data_02000240[];

u16 *Func_02000bd8(s32);
void Func_02000cae(s32, s32);
s32 Func_02000bd4(s32);
void Func_02000c70(s32);
s32 Func_02000c80(s32, s32);
s32 Func_02000c18(s32, s32);
void Func_02000c0a(s32);
void Func_02000c90(s32);
void Func_02000c98(s32);
void Func_02000cb0(s32, s32);
void Func_02000ca8(s32);
void Func_02000cc0(s32, s32);
u16 *Func_02000cd8(s32);
void Func_02000db4(s32);
s32 Func_02000ccc(s32);
void Func_02000d66(s32);
void Func_02000d7e(s32, s32);
void SceneMessage_Show(s32);
void Func_02000d9c(s32, s32);
u16 *Func_02000c6c(s32);
void Func_02000d52(s32, s32);
s32 Func_02000c68(s32);
void Func_02000d02(s32);
void Func_02000d1a(s32, s32);
void Func_02000d12(s32);
void Func_02000d2a(s32, s32);
s32 Func_02000b82(s32);
void Func_02000da2(s32);
s32 Func_02000db2(s32, s32);
s32 Func_02000d4a(s32, s32);
void Func_02000d3c(s32);
void Func_02000dc2(s32);
void Func_02000dca(s32);
void Func_02000de2(s32, s32);
void Func_02000e2a(s32);
void Func_02000e68(s32);
void Func_02000d82();
void Func_02000d84();
void Func_02000daa();
void Func_02000df2();
void Func_02000df8();
void Func_02000e00();
void Func_02000e0e();
void Func_02000e1c();
void Func_02000e22();
void Func_02000e42();
void Func_02000e4e();
void Func_02000e54();
void Func_02000e5c();
void Func_02000e62();
void Func_02000e64();
void Func_02000e64_a();
void Func_02000e6c();
void Func_02000e7e();
void Func_02000e7e_a();
void Func_02000e8c();
void Func_02000e8e();
void Func_02000e96();
void Func_02000e98();
void Func_02000e9a();
void Func_02000ea4();
void Func_02000ecc();
void Func_02000ecc_a();
void Func_02000ed4();
void Func_02000eda();
void Func_02000ee6();
void Func_02000eee();
void Func_02000ef4();
void Func_02000ef6();
void Func_02000f02();
void Func_02000f08();
void Func_02000f0e();
void Func_02000f10();
void Func_02000f20();
void Func_02000f24();
void Func_02000f2e();
void Func_02000f3c();
void Func_02000f4a();
void Func_02000f58();
void Func_02000f5e();
void Func_02000f64();
void Func_02000f6a();
void Func_02000f7e();
void Func_02000f80();
void Func_02000f82();
void Func_02000f86();
void Func_02000f8a();
void Func_02000f98();
void Func_02000f98_a();
void Func_02000f9c();
void Func_02000f9e();
void Func_02000fa0();
void Func_02000fb2();
void Func_02000fb8();
void Func_02000fc8();
void Func_02000fca();
void Func_02000fce();
void Func_02000fdc();
void Func_02000fe2();
void Func_02000fe6();
void Func_02000fea();
void Func_02000fea_a();
void Func_02000ffa();
void Func_02000ffc();
void Func_02001004();
void Func_0200100c();
void Func_02001012();
void Func_02001014();
void Func_02001020();
void Func_0200102e();
void Func_0200103c();
void Func_0200104a();
void Func_02001058();
void Func_0200105a();
void Func_02001066();
void Func_02001074();
void Func_02001074_a();
void Func_02001082();
void Func_02001088();
void Func_0200108e();
void Func_02001090();
void Func_02001096();
void Func_0200109a();
void Func_0200109e();
void Func_020010aa();
void Func_020010ac();
void Func_020010ba();
void Func_020010ba_a();
void Func_020010c8();
void Func_020010ce();
void Func_020010d4();
void Func_020010e0();
void Func_020010e0_a();
void Func_020010e2();
void Func_020010ee();
void Func_020010f0();
void Func_020010fc();
void Func_0200110c();
void Func_0200110e();
void Func_02001114();
void Func_02001118();
void Func_0200111e();
void Func_02001128();
void Func_0200112c();
void Func_0200113a();
void Func_0200114a();
void Func_02001150();
void Func_02001152();
void Func_02001156();
void Func_0200116a();
void Func_0200116c();
void Func_02001172();
void Func_02001176();
void Func_02001180();
void Func_02001186();
void Func_0200118e();
void Func_020011a4();
void Func_020011aa();
void Func_020011ae();
void Func_020011b4();
void Func_020011be();
void Func_020011c2();
void Func_020011c4();
void Func_020011d2();
void Func_020011de();
void Func_020011e4();
void Func_020011e8();
void Func_020011f2();
void Func_02001200();
void Func_0200121c();
void Func_0200122a();
void Func_0200122e();
void Func_02001230();
void Func_0200123c();
void Func_02001242();
void Func_02001250();
void Func_02001250_a();
void Func_02001254();
void Func_0200125c();
void Func_0200125e();
void Func_0200126c();
void Func_02001270();
void Func_02001272();
void Func_02001286();
void Func_0200128a();
void Func_0200128c();
void Func_0200128c_a();
void Func_0200129a();
void Func_020012a8();
void Func_020012ac();
void Func_020012b8();
void Func_020012be();
void Func_020012c0();
void Func_020012d4();
void Func_020012d4_a();
void Func_020012da();
void Func_020012da_a();
void Func_020012e8();
void Func_020012ec();
void Func_020012f6();
void Func_02001310();
void Func_02001316();
void Func_0200131e();
void Func_0200131e_a();
void Func_02001332();
void Func_02001332_a();
void Func_02001334();
void Func_0200133a();
void Func_0200134a();
void Func_0200134a_a();
void Func_02001352();
void Func_02001358();
void Func_02001366();
void Func_02001366_a();
void Func_02001374();
void Func_02001380();
void Func_02001380_a();
void Func_02001396();
void Func_0200139a();
void Func_020013a2();
void Func_020013a6();
void Func_020013b4();
void Func_020013b6();
void Func_020013b8();
void Func_020013bc();
void Func_020013ca();
void Func_020013d0();
void Func_020013e2();
void Func_020013e2_a();
void Func_020013f0();
void Func_020013fe();
void Func_020013fe_a();
void Func_0200140c();
void Func_02001416();
void Func_0200141a();
void Func_02001420();
void Func_0200142c();
void Func_0200142c_a();
void Func_02001430();
void Func_02001436();
void Func_02001438();
void Func_02001444();
void Func_0200144c();
void Func_02001452();
void Func_02001452_a();
void Func_02001468();
void Func_0200146e();
void Func_0200146e_a();
void Func_02001474();
void Func_02001488();
void Func_0200148a();
void Func_02001494();
void Func_02001496();
void Func_020014a4();
void Func_020014a4_a();
void Func_020014b2();
void Func_020014ba();
void Func_020014c4();
void Func_020014c8();
void Func_020014ca();
void Func_020014dc();
void Func_020014e2();
void Func_020014f0();
void Func_020014f0_a();
void Func_020014fe();
void Func_02001518();
void Func_0200151a();
void Func_02001526();
void Func_02001534();
void Func_0200153c();
void Func_02001552();
void Func_02001554();
void Func_0200156c();
void Func_0200156c_a();
void Func_02001572();
void Func_02001578();
void Func_0200157a();
void Func_02001586();
void Func_02001588();
void Func_02001592();
void Func_02001594();
void Func_0200159c();
void Func_020015a2();
void Func_020015aa();
void Func_020015aa_a();
void Func_020015bc();
void Func_0200162a();
void Func_0200163e();
s32 Func_0200158e(s32);
void Func_020015d0(s32);
union SceneActor *Func_02001614(s32);
union SceneActor *Func_02001628(s32);
union SceneActor *Func_02001636(s32);
union SceneActor *Func_0200165e(s32);

/*
 * Each pseudo symbol above names the per-site call word the overlay image
 * holds, not a runtime address -- one word can serve two sites with different
 * targets -- and the macro names the function the site reaches through the
 * veneers, keeping the site's own calling form. Names without a repository
 * binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers, in the order the reference uses. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 *Data_03001ebc;

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 *Data_03001ebc;

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 *Data_03001ebc;

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 *Data_03001ebc;

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 *Data_03001ebc;

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 *Data_03001ebc;

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 *Data_03001ebc;

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 *Data_03001ebc;

    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 *Data_03001ebc;

    return f(a0, a1, a2, a3);
}

/*
 * Returns the in-image script table at 0x02008c3c. The eight-byte owner
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x02008c3c;
}

/*
 * Returns the in-image table at 0x02008da4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8da4(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x02008da4;
}

/*
 * Returns the in-image table at 0x02008dd4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8dd4(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x02008dd4;
}

s32 SceneData_SelectTable8e08ByFlag96f(void)
{
    extern u8 *Data_03001ebc;

    if (Func_02000b82(0x96f) != 0) {
        return 0x02008f28;
    }
    return 0x02008e08;
}

void Dialogue_HandleFacingChoice(s32 no)
{
    extern u8 *Data_03001ebc;

    u16 facing = (Func_02000bd8(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Func_02000cae(31, no);
    } else if (Func_02000bd4(0x96f)) {
        s32 msg = (s32)&SceneMessage_FacingChoiceBase;
        Func_02000c70(msg);
        Func_02000c80(no, 0);
        if (Func_02000c18(0, 0) == 0) {
            Func_02000c0a(10);
            Func_02000c90(msg + 1);
        } else {
            Func_02000c98(msg + 2);
        }
        Func_02000cb0(no, 0);
    } else {
        Func_02000ca8(0x25cf);
        Func_02000cc0(no, 0);
    }
}

void Dialogue_HandleFacingBranch(s32 no)
{
    extern u8 *Data_03001ebc;

    u16 facing = (Func_02000c6c(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Func_02000d52(10, no);
    } else if (Func_02000c68(0x96f)) {
        Func_02000d02(0x2620);
        Func_02000d1a(no, 0);
    } else {
        Func_02000d12(0x25d1);
        Func_02000d2a(no, 0);
    }
}

void Dialogue_HandleFacingAction(s32 no)
{
    extern u8 *Data_03001ebc;

    u16 facing = (Func_02000cd8(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Func_02000db4(no);
    } else if (Func_02000ccc(0x96f)) {
        Func_02000d66(0x262c);
        Func_02000d7e(no, 0);
    } else {
        SceneMessage_Show(0x25d5);
        Func_02000d9c(no, 0);
    }
}

void Scene_RunActorCueBranch(s32 obj)
{
    extern u8 *Data_03001ebc;

    s32 cue = (s32)&Value_00002624;
    Func_02000da2(cue);
    Func_02000db2(obj, 0);
    if (Func_02000d4a(0, 0) == 0) {
        Func_02000d3c(10);
        Func_02000dc2(cue + 1);
    } else {
        Func_02000dca(cue + 2);
    }
    Func_02000de2(obj, 0);
}

void State_ApplyCounter16cThenCall7b(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s16 *cnt = (s16 *)(state + 0x16C);

    Func_02000e2a(*cnt);
    Func_02000e68(0x7B);
}

/*
 * Runs the primary script for this scene: a long fixed sequence driving
 * actors 10, 19, 20, 21, 30 and 40 through position, pose and timing steps,
 * guarded by an initial skip check. The calls are in machine order, and that
 * order is what reproduces the reference.
 */
void Scene_RunPrimaryScript(void)
{
    extern u8 *Data_03001ebc;

    u8 *record;
    GameFlag_Set_1(2480);
    if (GameFlag_IsSet_1(2442) == 0) {
    Audio_PlayCue_1(30);
    Battle_Reset_1();
    Motion_CamBounds_1(24117248, -1, 6815744, 1);
    Motion_SetPosReset_1(0, 368, 160);
    Motion_ArmCb_1(0, 49152, 0);
    Motion_RealignToTrackedObjectAndArmCallback_1(19, 0, -16, 49152);
    Motion_CommitPos_1(19);
    Object_CommitPositionThenWaitIfModeZero_1();
    SceneWork_SetStepValue_1(9707);
    Battle_WaitMode0_1(10);
    Motion_SetVarCbObj_1(20, 2);
    Battle_WaitMode0_2(20);
    BattleEv_RunWait_1(20, 0);
    Battle_WaitMode0_3(10);
    Motion_SetVarCbObj_2(19, 2);
    Battle_WaitMode0_4(20);
    Motion_SetSpeed_1(19, 78643, 39321);
    Motion_CommitPositionAndActivate_1(19, 0, -16);
    Motion_ArmCb_2(19, 0, 0);
    Battle_WaitMode0_5(30);
    Motion_ArmCb_3(19, 57344, 0);
    Battle_WaitMode0_6(30);
    Motion_ArmCb_4(19, 0, 0);
    Battle_WaitMode0_7(30);
    BattleFx_SpawnLinked_1(19, 256, 40);
    BattleEv_RunWait_2(19, 0);
    Battle_WaitMode0_8(10);
    Motion_SetVarCbObj_3(21, 2);
    Battle_WaitMode0_9(20);
    BattleEv_RunWait_3(21, 0);
    Battle_WaitMode0_10(10);
    Motion_SetSpeed_2(19, 78643, 39321);
    Motion_CommitPositionAndActivate_2(19, 0, -24);
    Motion_CommitPositionAndActivate_3(19, 48, 0);
    Motion_ArmCb_5(19, 49152, 0);
    Battle_WaitMode0_11(30);
    BattleEv_RunWait_4(19, 0);
    Battle_WaitMode0_12(20);
    BattleFx_SpawnLinked_2(19, 256, 40);
    BattleEv_RunWait_5(19, 0);
    Battle_WaitMode0_13(20);
    BattleFx_SpawnLinked_3(20, 258, 40);
    BattleEv_RunWait_6(20, 0);
    Battle_WaitMode0_14(10);
    Motion_ArmCb_6(19, 8192, 0);
    Battle_WaitMode0_15(20);
    BattleEv_RunWait_7(19, 0);
    Battle_WaitMode0_16(10);
    Motion_SetVarCbObj_4(21, 2);
    Battle_WaitMode0_17(20);
    Motion_CallWaitAnim_1(21, 4);
    Battle_WaitMode0_18(20);
    BattleEv_RunWait_8(21, 0);
    Battle_WaitMode0_19(10);
    Motion_ArmCb_7(19, 49152, 0);
    Battle_WaitMode0_20(30);
    BattleFx_SpawnLinked_4(19, 263, 40);
    BattleEv_RunWait_9(19, 0);
    Battle_WaitMode0_21(10);
    Motion_SetVarCb_1(20, 2);
    Motion_SetVarCbObj_5(21, 2);
    Battle_WaitMode0_22(20);
    Motion_SetSpeed_3(0, 78643, 39321);
    Motion_SetPosReset_2(0, 368, 104);
    Motion_CommitPositionAndActivate_4(0, 16, 0);
    Motion_ArmCb_8(0, 0, 0);
    Battle_WaitMode0_23(20);
    Battle_WaitMode0_24(10);
    Motion_CallWaitAnim_2(19, 4);
    Battle_WaitMode0_25(20);
    BattleEv_RunWait_10(19, 0);
    Battle_WaitMode0_26(20);
    BattleFx_SpawnLinked_5(19, 258, 50);
    BattleEv_RunWait_11(19, 0);
    Battle_WaitMode0_27(10);
    Motion_SetVarCbObj_6(20, 2);
    Battle_WaitMode0_28(20);
    BattleEv_RunWait_12(20, 0);
    Battle_WaitMode0_29(10);
    Motion_SetAngleToward_1(0, 20, 0);
    Motion_ArmCb_9(19, 8192, 0);
    Battle_WaitMode0_30(20);
    BattleEv_RunWait_13(19, 0);
    Battle_WaitMode0_31(10);
    Motion_SetVarCbObj_7(21, 2);
    Battle_WaitMode0_32(20);
    BattleEv_RunWait_14(21, 0);
    Battle_WaitMode0_33(10);
    Motion_SetAngleToward_2(0, 21, 0);
    Motion_ArmCb_10(19, 49152, 0);
    Battle_WaitMode0_34(40);
    Motion_CallWaitAnim_3(19, 3);
    Battle_WaitMode0_35(30);
    BattleEv_RunWait_15(19, 0);
    Battle_WaitMode0_36(10);
    Motion_SetVarCbObj_8(21, 2);
    Battle_WaitMode0_37(20);
    Motion_CallWaitAnim_4(21, 4);
    Battle_WaitMode0_38(20);
    BattleEv_RunWait_16(21, 0);
    Battle_WaitMode0_39(10);
    Object_LinkPair_1(19, 0, 30);
    BattleEv_RunWait_17(19, 0);
    Battle_WaitMode0_40(10);
    Motion_SetVarCbObj_9(20, 2);
    Battle_WaitMode0_41(20);
    BattleEv_RunWait_18(20, 0);
    Battle_WaitMode0_42(10);
    Motion_SetAngleToward_3(0, 20, 0);
    Motion_ArmCb_11(19, 8192, 0);
    Battle_WaitMode0_43(40);
    Battle_WaitMode0_44(10);
    Motion_SetVarCbObj_10(21, 2);
    Battle_WaitMode0_45(20);
    BattleEv_RunWait_19(21, 0);
    Battle_WaitMode0_46(10);
    Motion_SetAngleToward_4(0, 21, 0);
    Motion_ArmCb_12(19, 49152, 0);
    Battle_WaitMode0_47(40);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_5(19, 3);
    Battle_WaitMode0_48(30);
    Battle_WaitMode0_49(10);
    Motion_CallWaitAnim_6(20, 4);
    Battle_WaitMode0_50(20);
    BattleEv_RunWait_20(20, 0);
    Battle_WaitMode0_51(10);
    BattleFx_SpawnLinked_6(0, 256, 0);
    BattleFx_SpawnLinked_7(19, 256, 40);
    Battle_WaitMode0_52(10);
    Motion_SetAngleToward_5(0, 20, 0);
    Motion_ArmCb_13(19, 8192, 0);
    Battle_WaitMode0_53(40);
    Battle_WaitMode0_54(10);
    Motion_SetVarCbObj_11(21, 2);
    Battle_WaitMode0_55(20);
    BattleEv_RunWait_21(21, 0);
    Battle_WaitMode0_56(10);
    BattleFx_SpawnLinked_8(0, 256, 50);
    Motion_SetAngleToward_6(0, 21, 0);
    Battle_WaitMode0_57(30);
    Motion_SetVarCbObj_12(21, 2);
    Battle_WaitMode0_58(30);
    Battle_WaitMode0_59(10);
    Motion_ArmCb_14(19, 49152, 0);
    Battle_WaitMode0_60(30);
    BattleEv_RunWait_22(19, 0);
    Battle_WaitMode0_61(10);
    Motion_SetVarCbObj_13(21, 2);
    Battle_WaitMode0_62(20);
    Motion_CallWaitAnim_7(21, 3);
    Battle_WaitMode0_63(30);
    Battle_WaitMode0_64(10);
    BattleFx_SpawnLinked_9(19, 258, 40);
    BattleEv_RunWait_23(19, 0);
    Battle_WaitMode0_65(10);
    Motion_CallWaitAnim_8(20, 4);
    Battle_WaitMode0_66(20);
    BattleEv_RunWait_24(20, 0);
    Battle_WaitMode0_67(10);
    BattleFx_SpawnLinked_10(19, 257, 50);
    Battle_WaitMode0_68(10);
    Motion_SetAngleToward_7(0, 20, 0);
    Motion_ArmCb_15(19, 8192, 0);
    Battle_WaitMode0_69(20);
    Battle_WaitMode0_70(10);
    Motion_SetVarCbObj_14(20, 2);
    Battle_WaitMode0_71(20);
    BattleEv_RunWait_25(20, 0);
    Battle_WaitMode0_72(10);
    BattleFx_SpawnLinked_11(19, 256, 40);
    BattleEv_RunWait_26(19, 0);
    Battle_WaitMode0_73(10);
    Motion_SetVarCbObj_15(21, 2);
    Battle_WaitMode0_74(20);
    BattleEv_RunWait_27(21, 0);
    Motion_SetVarCbObj_16(21, 2);
    Battle_WaitMode0_75(20);
    Battle_WaitMode0_76(10);
    Object_LinkPair_2(19, 0, 0);
    Battle_WaitMode0_77(30);
    BattleEv_RunWait_28(19, 0);
    Battle_WaitMode0_78(10);
    Motion_CallWaitAnim_9(20, 4);
    Battle_WaitMode0_79(20);
    BattleEv_RunWait_29(20, 0);
    Battle_WaitMode0_80(10);
    BattleFx_SpawnLinked_12(0, 258, 0);
    BattleFx_SpawnLinked_13(19, 258, 80);
    BattleFx_SpawnLinked_14(21, 258, 50);
    BattleEv_RunWait_30(21, 0);
    Battle_WaitMode0_81(10);
    Motion_SetVarCbObj_17(21, 2);
    Battle_WaitMode0_82(20);
    BattleEv_RunWait_31(21, 0);
    Motion_SetVarCbObj_18(21, 3);
    Battle_WaitMode0_83(20);
    Battle_WaitMode0_84(10);
    Motion_ArmCb_16(19, 49152, 0);
    Battle_WaitMode0_85(30);
    Motion_SetVarCbObj_19(19, 2);
    Battle_WaitMode0_86(10);
    BattleEv_RunWait_32(19, 0);
    Battle_WaitMode0_87(10);
    Motion_SetVarCbObj_20(20, 2);
    Battle_WaitMode0_88(20);
    BattleEv_RunWait_33(20, 0);
    Battle_WaitMode0_89(10);
    Motion_SetAngleToward_8(0, 20, 0);
    Motion_ArmCb_17(19, 8192, 0);
    Battle_WaitMode0_90(30);
    Motion_CallWaitAnim_10(19, 3);
    Battle_WaitMode0_91(30);
    BattleEv_RunWait_34(19, 0);
    Battle_WaitMode0_92(10);
    Motion_SetVarCb_2(20, 2);
    Motion_SetVarCbObj_21(21, 2);
    Battle_WaitMode0_93(30);
    Battle_WaitMode0_94(10);
    Object_LinkPair_3(19, 0, 20);
    Motion_CommitPositionAndActivate_5(19, -12, 0);
    Battle_WaitMode0_95(20);
    Motion_SetSpeed_4(19, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
    Battle_WaitMode0_96(20);
    BattleEv_RunWait_35(19, 0);
    STEP_COUNTER++;
    } else {
    Battle_WaitMode0_97(10);
    STEP_COUNTER++;
    BattleEv_RunWait_36(19, 0);
    }
    Battle_WaitMode0_98(10);
    Motion_SetVarCbObj_22(19, 2);
    Battle_WaitMode0_99(20);
    BattleEv_RunWait_37(19, 0);
    Battle_WaitMode0_100(10);
    BattleFx_SpawnLinked_15(19, 258, 50);
    BattleEv_RunWait_38(19, 0);
    Battle_WaitMode0_101(10);
    Motion_CallWaitAnim_11(0, 3);
    Battle_WaitMode0_102(20);
    Motion_CallWaitAnim_12(19, 3);
    Battle_WaitMode0_103(30);
    Audio_PlayCue_2(30);
    Motion_SetSpeed_5(19, 78643, 39321);
    Object_SetModeById_2(19, 2);
    record = (u8 *)Scene_GetRecord_1(0);
    if (record != 0) {
    Motion_ResetAndSetPosition_1(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(19);
    Motion_SetHPosTerrain_1(19, 0, 0);
    Battle_WaitMode0_104(10);
    Audio_PlayCueForPartyMember_1();
    Battle_SchedShoulder_1();
    }
}

s32 SceneData_SelectTable9090ByFlag96f(void)
{
    extern u8 *Data_03001ebc;

    if (Func_0200158e(0x96F) != 0) {
        return 0x020091EC;
    }
    return 0x02009090;
}

s32 Scene_InitActorRecords(void)
{
    extern s32 *Data_03001ebc;

    union SceneActor *work;
    if (Data_02000240[225] == 90)
        Func_020015d0(0x96f);
    Data_03001ebc[112] = 521;
    Data_03001ebc[114] = 24;
    Func_02001614(12)->bytes[89] |= 4;
    Func_02001628(13)->bytes[89] |= 4;
    work = Func_02001636(20);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    {
        /* The mask is built in a local, not folded into the store. */
        struct SceneRecord *record = work->fields.record;
        s32 flags = ~12;

        flags = flags & record->flags;
        record->flags = flags | 4;
    }
    work = Func_0200165e(21);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    work->bytes[85] = 2;
    work->fields.y = 0;
    return 0;
}
