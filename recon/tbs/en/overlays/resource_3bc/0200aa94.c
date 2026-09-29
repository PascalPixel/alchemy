/* NONMATCHING: resource_3bc at 0x0200aa94..0x0200abac (280 bytes with their
 * pools), ColossoLogRollingStage_RunStateInteraction and
 * InitializeStateInteraction, between FIELD/KOROSSEO_MARUTA/SAVED_POSITIONS.C
 * and MIDDLE.C, stay listing.
 *
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Func_0200760c,
 * Func_0200741c, Func_0200747e, Value_0000008f, Value_00000090,
 * Func_020074d2).
 */
/* The per-site declarations this draft needs (formerly SITES.H). */
/* Draft scaffolding for the resource_3bc drafts beside this file: the
 * per-site call names, per-site macros and address-named data the old
 * reconstruction of FIELD/KOROSSEO_MARUTA/LOG_ROLLING.C used. None of it is
 * built; linked code names these places once, in LOG_ROLLING.H and the
 * overlay's IMPORT.S. */
#ifndef RESOURCE_3BC_SITES_H
#define RESOURCE_3BC_SITES_H

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/LOG_ROLLING.H"
extern u8 MsgKorosseoStageFirstFinalsMatch[];
extern u8 MsgKorosseoStageSecondFinalsMatch[];
extern u8 MsgKorosseoStageThirdFinalsMatch[];
extern u8 MsgKorosseoWouldYouLikeHearDescription[];

extern void Func_02004d72(void);       /* site 0x20024e4 -> Func_0200288c veneer */
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern void Func_020062e0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_02006324(s32 mode);          /* Func_02002e54 veneer #2 */
extern s32 Func_02008092(void);               /* Func_080f9048 veneer (loop check) */
extern void Func_0200634e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_0200637c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_02006396(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_020080cc(void);              /* Func_0808a4f0 veneer */
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];
StageActor *Func_02007198(s32);
StageActor *Func_020071a6(s32);
void Func_02007210_b();
void Func_02007200(s32, s32);
void Func_02004e9a(s32);
void Func_0200742a(s32, s32);
void Func_02007434(s32, s32);
void Func_0200743e(s32, s32);
void Func_02007448(s32, s32);
void Func_02007452(s32, s32);
void Func_0200745c(s32, s32);
void Func_0200760c(void);
void Func_0200741c(s32, s32);
s32 Func_0200747e(s32);
void Func_020074d2(s32, s32);
void Func_02007c38(s32 taskAddress, s32 frameBudget);
void Func_020082d4(s32 taskAddress, s32 frameBudget);
void Func_02008330(s32 taskAddress, s32 frameBudget);
void Func_0200b91c(void);
void Func_0200836e(void (*callback)(void));
void Func_020083c8(s32 slot);
ScaledStageObject *Func_020086b0();
void Func_02008464();
void Func_02008488();
u8 *Func_020086f0(s32 object_id);
void Func_020084a4(void);
void Func_020084c8(u8 *object, s32 x, s32 y, s32 z);
void Func_020084d6(u8 *object);
void Func_020085a6(s32, s32, s32 *);
StageEffect *Func_0200863a(s32, s32, s32, s32);
StageEffect *Func_020087b4(s16);
void Func_020086be(StageEffect *, s32, s32, s32);
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
void Func_02008dbe();
s32 Func_02008de2();
void Func_02008d8e();
void Func_02008ddc();
void Func_0200c0d0(void);
s32 Func_02008e56(void);
void Func_02008e26(s32, s32);
void Func_02008dfa(s32, s32);
SceneRecord *Func_0200905c();
SceneRecord *Func_02008c56(Position3 *, SceneRecord *);
SceneRecord *Func_02008c80(Position3 *, SceneRecord *);
SceneRecord *Func_02008cac(Position3 *, SceneRecord *);
s32 Func_0200905e(SceneRecord *, Position3 *);
void Func_02009042(SceneRecord *, s32, s32, s32);
void Func_02009052(SceneRecord *, s32, s32, s32);
void Func_02009066(SceneRecord *);
u8 *Func_020091bc();
void Func_02009046();
s32 *Func_02008dc6();
void Func_02009078();
s32 *Func_02008df8();
s32 *Func_02006e64();          /* entity by selector, established */
void Func_02006c06();          /* unestablished */
void Func_02006c2a();          /* established (record, x, y, z) */
void Func_02006c38();          /* unestablished, single argument */
s32 Func_02006f98();           /* Func_080770e0 veneer #1 */
s32 Func_02006fa2();           /* Func_080770e0 veneer #2 */
s32 Func_02006fc0();           /* Func_080770e0 veneer #3 */
s32 Func_02006fca();           /* Func_080770e0 veneer #4 */
s32 Func_02006fe4();           /* Func_080770e0 veneer #5 */
s32 Func_02006fee();           /* Func_080770e0 veneer #6 */
u8 *Func_020077bc();           /* veneer, item/party record by id, established */
s32 Func_020077ce();           /* veneer, established (handle, item) */
void Func_020077ea();          /* veneer, established (handle, slot) */
u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Scene_GetRecord) */
void Func_020081e6();          /* unestablished */
s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */
void Func_02007362();          /* sibling item-28 owner, via per-site veneer */
u8 *Func_02008818_a();         /* veneer to Scene_GetRecord */
void Func_0200876a();          /* veneer to Object_SetPosition */
s32 Func_02006ca2();           /* local thunk to Func_020020e8, site A */
s32 Func_02006cb4();           /* local thunk to Func_020020e8, site B */
void Func_0200880a();          /* veneer to UiText_DrawQuantity, site A */
void Func_0200881c();          /* veneer to UiText_DrawQuantity, site B */
void Func_0200882c_a();          /* shared veneer, selector refresh + 0x96a */
void Func_020087ca();          /* veneer to Func_08009148 */
s32 *Func_02006e34_position();          /* entity by selector, established */
void Func_02006bd6_position();          /* unestablished */
void Func_02006bfa_position();          /* established (record, x, y, z) */
void Func_0200469a_arrival();
s32 Func_020048b0_arrival();
void Func_02004aaa_arrival();
void Func_02004b24_arrival();
s32 Func_020054a8_arrival();
void Func_02005766_arrival();
void Func_02005960_arrival();
void Func_020059ce_arrival();
void Func_020059de_arrival();
void Func_020059ee_arrival();
s32 Func_02005a00_arrival();
void Func_02005a0c_arrival();
void Func_02005a5c_arrival();
void Func_02004c6a_head();
void Func_02004c8a_head();
void Func_02004cae_head();
s32 Func_02004d36_head();
s32 Func_02004d54_head();
s32 Func_02004d78_head();
void Func_02004d96_a_head();
void Func_02004d96_b_head();
s32 Func_02004d98_head();
void Func_02004e0e_head();
void Func_02004e2e_head();
void Func_02004e46_head();
s32 Func_02004ecc_head();
u8 *Func_02004ee4_head();
s32 Func_02004f1c_head();
u8 *Func_02004f3c_head();
s32 Func_02000f3a_head();
s32 Func_02000f68_head();
void Func_02000fd0_head();
void Func_02005014_head();
void Func_02005036_head();
s32 Func_02005154_head();
s32 Func_020051a0_head();
s32 Func_020051a8_head();
void Func_020051d6_head();
void Func_020051e2_head();
void Func_0200522a_head();
void Func_02005238_head();
void Func_02005246_head();
void Func_0200527a_head();
void Func_02003262_head();
void Func_02005e7e_head();
s32 Func_02005ef0_head();
void Func_02005f22_a_head();
void Func_02005f22_b_head();
s32 Func_02005f5e_head();
s32 Func_02005f66_head();
s32 Func_02005f6e_head();
s32 Func_02005fba_head();
s32 Func_02005fc8_head();
s32 Func_02005fda_head();
void Func_02005fee_head();
s32 Func_02005ff0_head();
void Func_02006002_head();
void Func_02006014_head();
void Func_02006028_head();
void Func_02006034_a_head();
void Func_02006034_b_head();
void Func_0200603e_head();
void Func_02006052_head();
void Func_02006056_a_head();
void Func_02006056_b_head();
void Func_0200606a_head();
void Func_0200607e_head();
void Func_020060bc_head();
void Func_020060d0_head();
s32 Func_020060fc_a_head();
s32 Func_020060fc_b_head();
void Func_02006112_a_head();
void Func_02006112_b_head();
void Func_02006140_head();
void Func_020061b4_head();
void Func_02006214_head();
void Func_02006238_head();
void Func_02006240_head();
void Func_02006254_head();
void Func_0200625c_head();
void Func_02006270_head();
void Func_02006278_head();
void Func_02006294_head();
void Func_0200629c_head();
void Func_020062c2_head();
void Func_020062e0_head();
void Func_020062fe_head();
void Func_02006320_a_head();
void Func_02006320_b_head();
void Func_02006322_a_head();
void Func_02006322_b_head();
void Func_02006332_a_head();
void Func_02006332_b_head();
void Func_020063d0_a_head();
void Func_020063d0_b_head();
void Func_020063d8_head();
void Func_020063e6_head();
void Func_020063ee_head();
void Func_0200643a_head();
void Func_02006472_head();
void Func_020064f0_head();
void Func_020093c1();
extern void Func_020049ea_reset_and_run_scene_task(SceneTaskEntry);
extern s32 Func_02004a06_start_scene_task(s32, s32);
void Func_02004a0e_wait_for_scene_task();
void Func_02004a1e_wait_for_scene_task();
StageObstacleActor *Func_02004c36_nudge_stage_actors_left();
extern void Func_02004c0e_configure_grid_region(s32);
extern void Func_02004baa_configure_grid_region(s32, s32, s32, s32, s32, s32);
void Func_02004bae_configure_primary_object_set(PrimaryStageObject *, s32, s32, s32);
void Func_02004bc4_configure_primary_object_set(PrimaryStageObject *, s32, s32, s32);
void Func_02004c0e_configure_primary_object_set(s32, s32, s32, s32, s32, s32);
void Func_02004c20_configure_primary_object_set(s32, s32, s32, s32, s32, s32);
void Func_02004c72_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004c94_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004ca2_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004cbc_configure_primary_object_set(s32);
void Func_02004e8a_configure_secondary_object_set();
SecondaryStageObject *Func_02004e38_configure_secondary_object_set();
void Func_02004d50_configure_secondary_object_set();
SecondaryStageObject *Func_02004e5e_configure_secondary_object_set();
void Func_02004d6e_configure_secondary_object_set();
void Func_02004ec4_configure_secondary_object_set();
void Func_02004db8_configure_secondary_object_set();
void Func_02004cbe_configure_secondary_object_set();
void Func_02004e44_configure_secondary_object_set();
void Func_02000a28_run_setup_completion_hooks(void);
void Func_02004ba8_run_setup_completion_hooks(void);
extern StageObstacleActor *Func_02004fac_configure_actor_thirteen(s32);
extern void Func_02004f72_configure_actor_thirteen(s32, s32);
extern void Func_02004ef6_configure_actor_thirteen(s32, s32, s32, s32, s32, s32);
extern void Func_02004f06_configure_actor_thirteen(s32, s32, s32, s32, s32, s32);
extern void Func_02004c00_run_setup_hook(void);
extern StageObstacleActor *Func_02005002_activate_clear_obstacle_actors(s32);
extern s32 Func_02004f26_activate_clear_obstacle_actors(s32, s32, s32);
extern void Func_02004f60_activate_clear_obstacle_actors(s32, s32, s32, s32, s32, s32);
extern void Func_02004f7a_activate_clear_obstacle_actors(s32, s32, s32, s32, s32, s32);
extern void Func_02004ffa_activate_clear_obstacle_actors(s32);
extern StageObstacleActor *Func_0200507e_show_actor_position_message(s32);
extern void Func_02004ffe_show_actor_position_message(s32, s32, s32, s32);
extern s32 Func_02004fea_check_obstacle_destination(s32, s32, s32);
extern StageObstacleActor *Func_020050da_check_obstacle_destination(s32);
extern StageObstacleActor *Func_020050f4_check_obstacle_destination(s32);
extern StageObstacleActor *Func_0200510a_check_obstacle_destination(s32);
extern s32 Func_02000dc4_check_path_clearance(s32, s32);
extern s32 Func_02000dd2_check_path_clearance(s32, s32);
extern s32 Func_02000de2_check_path_clearance(s32, s32);
extern s32 Func_02000df2_check_path_clearance(s32, s32);
extern void Func_0200534e_set_scene_event_values(s32);
extern void Func_0200534c_set_scene_event_values(s32);
extern void Func_020055d4_set_scene_event_values(s32);
extern void Func_020055da_set_scene_event_values(s32);
extern StageMotionEffect *Func_02005462_configure_scene_event_effect(s32);
extern void Func_02005340_configure_scene_event_effect(StageMotionEffect *, s32);
extern void Func_02005350_configure_scene_event_effect(StageMotionEffect *, s32);
extern void Func_02005426_configure_scene_event_effect(s32);
extern void Func_0200561a_wait_for_scene_event_task(s32);
extern void Func_02005448_wait_for_scene_event_task(s32);
extern void Func_020052d6_wait_for_scene_event_task(s32);
extern void Func_020052ea_wait_for_scene_event_task(s32);
extern void Func_020052fa_wait_for_scene_event_task(s32);
extern void Func_02005310_wait_for_scene_event_task(s32);
extern StageObstacleActor *Func_020054fa_offset_active_actor(s32);
extern void Func_020055e2_offset_active_actor(s32, s32);
extern void Func_020053e2_offset_active_actor(StageObstacleActor *, s32);
extern void Func_02005428_offset_active_actor(StageObstacleActor *, s32, s32, s32);
extern void Func_02005436_offset_active_actor(StageObstacleActor *);
extern StageObstacleActor *Func_02005556_clamp_and_offset_active_actor(s32);
extern void Func_02005440_clamp_and_offset_active_actor(StageObstacleActor *, s32);
extern void Func_02005486_clamp_and_offset_active_actor(StageObstacleActor *, s32, s32, s32);
extern void Func_02005494_clamp_and_offset_active_actor(StageObstacleActor *);
extern void Func_0200566e_clamp_and_offset_active_actor(s32, s32);
extern void Func_02005620_clamp_and_offset_active_actor(s32, s32, s32);
extern s32 Func_02005504_run_scene_event_if_ready(void);
extern void Func_020058ac_run_scene_event_if_ready(void);
extern s32 Func_02005518_finish_or_continue_scene_event(void);
extern void Func_020058c0_finish_or_continue_scene_event(void);
extern void Func_02001312_finish_or_continue_scene_event(void);
extern SceneParticle *Func_02005de2_spawn_periodic_scene_particle(s32);
extern s32 Func_02005c1a_spawn_periodic_scene_particle(s32, s32);
extern SceneParticle *Func_02005e1a_spawn_periodic_scene_particle(s32);
extern SceneParticle *Func_02005e26_spawn_periodic_scene_particle(s32);
extern void Func_02005e84_spawn_periodic_scene_particle(s32, s32, s32);
extern s32 Func_02005e3a_spawn_periodic_scene_particle(s32);
extern void Func_02005d80_spawn_periodic_scene_particle(s32, s32);
extern void Func_02005e8a_spawn_periodic_scene_particle(s32, s32);
void Func_0200499e_motion();
s32 Func_02004bb4_motion();
void Func_02004dd0_motion();
s32 Func_02004e4c_motion();
s32 Func_020057b2_motion();
void Func_02005a8c_motion();
void Func_02006b02_motion();
void Func_02006b10_motion();
void Func_02006b24_motion();
void Func_02006b32_motion();
void Func_02006b44_motion();
void Func_02006b52_motion();
void Func_02006b64_motion();
void Func_02006b72_motion();
s32 Func_02006bda_motion();
void Func_02006cd4_motion();
void Func_0200469e_opening();
void Func_020046b8_opening();
s32 Func_02004710_opening();
void Func_02004bd6_opening();
s32 Func_02004dec_opening();
void Func_02004fcc_opening();
s32 Func_02005048_opening();
s32 Func_020059ea_opening();
void Func_02005c88_opening();
void Func_02005cd0_opening();
void Func_02005d36_opening();
s32 Func_02006e2e_opening();
s32 Func_020075b4_middle();
s32 Func_020075de_middle();
void Func_02007640_middle();
s32 Func_0200764c_a_middle();
s32 Func_0200764c_b_middle();
void Func_02007654_middle();
void Func_0200767e_middle();
void Func_02007690_middle();
s32 Func_0200772a_middle();
void Func_0200778a_a_middle();
void Func_0200778a_b_middle();
void Func_020077a0_middle();
void Func_020077bc_middle();
s32 Func_020077e2_middle();
s32 Func_02007816_middle();
void Func_020078fc_middle();
void Func_020077ae_middle();
void Func_0200640a_sequence();
void Func_02006436_sequence();
s32 Func_02006996_sequence();
s32 Func_020069c2_sequence();
s32 Func_020081b0_sequence();
#define Audio_PlayCue_1(args...) Func_02006034_a_head(args)
#define Object_SetModeById_1(args...) Func_02005f22_a_head(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Scene_GetRecord_1(args...) Func_02005ef0_head(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005f22_b_head, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02005f5e_head, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02005f66_head, a0)
#define Scene_GetRecord_4(a0) Value1(Func_02005f6e_head, a0)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02006056_a_head(args)
#define Scene_GetRecord_5(args...) Func_02005fba_head(args)
#define Scene_GetRecord_6(args...) Func_02005fc8_head(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006034_b_head(args)
#define Scene_GetRecord_7(args...) Func_02005fda_head(args)
#define Scene_GetRecord_8(args...) Func_02005ff0_head(args)
#define ObjectMotion_EnableActionAndSetCallback_4(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02006056_b_head, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Func_020060fc_a_head, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_1(args...) Func_020061b4_head(args)
#define Object_LinkObjectAndSetCallback_2(args...) Func_02006214_head(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02006112_a_head(args)
#define Object_LinkObjectAndSetCallback_3(args...) Func_02006238_head(args)
#define Object_LinkObjectAndSetCallback_4(args...) Func_02006240_head(args)
#define Object_LinkObjectAndSetCallback_5(args...) Func_02006254_head(args)
#define Object_LinkObjectAndSetCallback_6(args...) Func_0200625c_head(args)
#define Object_LinkObjectAndSetCallback_7(args...) Func_02006270_head(args)
#define Object_LinkObjectAndSetCallback_8(args...) Func_02006278_head(args)
#define BattleRuntime_WaitIfModeZero_12(args...) ((void (*)())Func_020060fc_b_head)(args)
#define Object_LinkObjectAndSetCallback_9(args...) Func_02006294_head(args)
#define Object_LinkObjectAndSetCallback_10(args...) Func_0200629c_head(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02006112_b_head(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LinkObjectAndSetCallback_11(args...) Func_020062c2_head(args)
#define Object_LinkObjectAndSetCallback_12(args...) Func_020062e0_head(args)
#define Object_LinkObjectAndSetCallback_13(args...) Func_020062fe_head(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006320_a_head, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_14(args...) Func_020063d0_a_head(args)
#define Object_LinkObjectAndSetCallback_15(args...) Func_020063d8_head(args)
#define Object_LinkObjectAndSetCallback_16(args...) Func_020063e6_head(args)
#define Object_LinkObjectAndSetCallback_17(args...) Func_020063ee_head(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02006322_a_head(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_02006332_a_head(args)
#define Object_LinkObjectAndSetCallback_18(args...) Func_0200643a_head(args)
#define Object_LinkObjectAndSetCallback_19(args...) Func_02006472_head(args)
#define BattleRuntime_WaitIfModeZero_30(args...) Func_02006320_b_head(args)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_02006332_b_head(args)
#define Object_SetModeById_8(args...) Func_020063d0_b_head(args)
#define Object_LinkObjectAndSetCallback_20(args...) Func_020064f0_head(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)&gEventWork + 0x1c0))
#define PENDING_CALLBACK_FLAG (*(s32 *)0x0200db80)

#endif


s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = gGameState.scene;
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        script_id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(script_id);
    Event_ShowMessage(actor_handle, 0);
    if (GameFlag_IsSet(interaction_base + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    GameFlag_Set(interaction_base + 520);
    Event_SetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
    Event_OpenMessage(actor_handle, 0);
    return Event_ChooseYesNo(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = gGameState.scene;
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)MsgKorosseoStageFirstFinalsMatch;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)MsgKorosseoStageSecondFinalsMatch;
    } else {
        script_id = (s32)MsgKorosseoStageThirdFinalsMatch;
    }
    Event_SetMessage(script_id + 1);
    Event_ShowMessage(actor_handle, 0);
}
