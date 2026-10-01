/* NONMATCHING: 2026-10-01 Venus Djinni carriers/whole-height-early-before source-lifetime trial.
 * Fresh own EN resource371 complete native extent 1488; this candidate
 * emits 1508 bytes with 1090 differing positions, first +0x1c,
 * including every pool and actual current native call/data binding.
 * Other five complete linked proofs remain pending. The four-byte best
 * witness closes the position height handoff; scale setup still differs.
 * No compiler, routing or output change. No credit.
 */
/* Draft of WorldMap_MeetVenusDjinni, resource_371 at 0x02009ca4 (FIELD/WORLD_MAP/VENUS_DJINNI.C).
 * Remaining difference (score 230 against the listing with a temporary label,
 * 2 register-only, 5 operand, 2 reordered). Declaring Djinn_AddToOwner and
 * Trade_AddOffer unprototyped returning s32, as MAKYURI_HEYA/PARTY_SCENE.C
 * does, fixed the all-zero call order (was 260). Left: after
 * Engine_EventSetMessage(i) in the listened tail the ROM loads the jump as r2, r0,
 * r1 and the following message as r1, r0; in the later-meeting tail the walk
 * loads z (sl) before x (r9); the 0x800 scale step and the 0x100000 height
 * build one instruction later. All sit just before a branch to the shared
 * finish; direct Engine_ calls, temporaries, if/else in place of goto finish
 * and 40k permuter candidates change nothing.
 * 2026-09-29 (Mars): with the listing's calls masked the score is 130
 * (2 register-only, 2 reordered plus the five argument loads). In the greg
 * dump every Engine_ActorJump loads r0, r1, r2 in order; sched2 alone reorders
 * them, and the same calls in a small test file never reorder, so the
 * order follows the surrounding block, not the call's spelling. Every
 * inline-wrapper parameter order, unprototyped casts of Engine_ActorJump
 * and Engine_EventSetMessage, a do/while barrier, and a duplicated tail
 * left for cross-jumping to merge all leave it unchanged.
 * 2026-09-30 (Mars): built in place as WORLD_MAP/VENUS_DJINNI.C between
 * TRANSFER and CHASE (gWorldMapEvents then names it), the FAKEMATCH
 * register variables with volatile empty asm below fix the listened tail's
 * jump and message argument order. The walk pin copies z to r3 first but
 * then shifts it in place (asrs r3, r3; adds r2, r3) instead of
 * asrs r2, r3. Still left: the 0x800 scale step's lsls r7 after movs r5,
 * #15 (ROM: before) and the 0x100000 height's lsls r2 before mov r1, r9
 * (ROM: after mov r1, r9).
 * 2026-09-30 (Jupiter): pinning x to r2 and shifting z into r2 with a
 * tagged asr after the tile x fixes the walk: 1488 bytes, two differences
 * left (scale step and height). A late tagged lsl on a 0x80 height with x
 * pinned to r1 gets the lsl after mov r1, r9 but then movs r2 follows the
 * flag store and adds r0 follows mov r1 (8); pinning the height across the
 * flag store or the djinni to r0 breaks the allocation (237-264). A 0x80 step
 * shifted by a tagged lsl before the counter, plain or volatile, or pinned
 * to r7, moves the loop registers (73-365). */
/* The world map's Venus Djinni: on the first meeting it joins Isaac, grows
 * from a speck and explains itself, asking until the party agrees to listen;
 * later it offers to explain Djinn again. */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/STORY.H"
#include "CALL.H"

extern u8 MsgWorldMapOh[];
extern u8 MsgWorldMapComePromiseWont[];
extern u8 MsgWorldMapMeanieDontCare[];
extern u8 MsgWorldMapSeeWontRegret[];
extern u8 MsgWorldMapAbilityVenusDjinni[];
extern u8 MsgWorldMapSeeDjinnUseful[];
extern u8 MsgWorldMapGoSetStandby[];
extern u8 MsgWorldMapHmmmmExplainAgain[];
extern u8 MsgWorldMapYeahWantLearn[];

void Owner_RefreshActiveRatios(s32 owner);
s32 Djinn_AddToOwner();
s32 Trade_AddOffer();
void BattleEffect_CleanupSceneObjects(void);
void UiWork_PushValueSlot(s32 value, s32 slot);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 a1, s32 a2);
void FieldScene_RunScene371_02001c08(void);

#define DJINNI 8
#define FLAG_DJINNI_MET 0x16e

void WorldMap_MeetVenusDjinni(void)
{
    /* FAKEMATCH: retain the djinni carrier through the native scene lifetime while testing the local ordering handoff. */
    register struct FieldActor *djinni asm("r6");
    struct FieldActor *leader;
    /* FAKEMATCH: retain the x carrier through the native scene lifetime while testing the local ordering handoff. */
    register s32 x asm("r9");
    /* FAKEMATCH: retain the z carrier through the native scene lifetime while testing the local ordering handoff. */
    register s32 z asm("r10");
    /* FAKEMATCH: retain the i carrier through the native scene lifetime while testing the local ordering handoff. */
    register s32 i asm("r5");

    i = 0;
    djinni = Object_GetById(DJINNI);
    leader = Object_GetById(0);
    x = (leader->x.fixed + (s32)0xea300000) / 2 + 0x15d00000;
    z = (leader->z.fixed + (s32)0xfad00000) / 2 + 0x5300000;
    if (!Value1(Engine_GameFlagIsSet, FLAG_DJINNI_MET)) {
        Owner_RefreshActiveRatios(1);
        Call1(Engine_GameFlagSet, FLAG_DJINNI_MET);
        Engine_EventBegin();
        leader = (struct FieldActor *)Value1((s32 (*)())Object_GetById, 0);
        if (leader != 0)
            Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
        Djinn_AddToOwner(0, 0, 0);
        Trade_AddOffer(0, 0, 0);
        Battle_SetObjectFlag5bWhenMode3();
        Actor_FaceActor(0, DJINNI, 0);
        Engine_EventWait(10);
        Actor_ShowEmote(0, 0x101, 60);
        djinni->unknown_66 = 1;
        Actor_FaceActor(DJINNI, 0, 0);
        Engine_TaskWait(16);
        Engine_EventSetMessage((s32)MsgWorldMapOh);
        Event_ShowMessage(DJINNI, 0);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_ScheduleRatioTransition(0x13333, 6);
        Event_WaitForDisplayField358Clear();
        Battle_SetObjectFlag5bWhenMode3();
        djinni->motion_flags = 2;
        *(s32 *)&djinni->unknown_44[4] = 0x4000;
        djinni->speed = 0x10000;
        djinni->acceleration = 0x10000;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
        for (i = 15; i >= 0; i--) {
            djinni->scale_x += 0x800;
            djinni->scale_y += 0x800;
            Engine_TaskWait(1);
        }
        Actor_FaceActor(DJINNI, 0, 0);
        Actor_FaceActor(0, DJINNI, 0);
        Engine_TaskWait(16);
        djinni->update = 0;
        Engine_ObjectSetPartPalettes(djinni, 0);
        *(s32 *)&djinni->unknown_44[4] = 0x10000;
        Event_ShowMessage(DJINNI, 0);
        Audio_PlayCue(131);
        Engine_PsynergyBegin(140, 0);
        for (i = 59; i >= 0; i--) {
            if (gFrameCount & 2)
                Engine_ObjectSetPartPalettes(djinni, 7);
            else
                Engine_ObjectSetPartPalettes(djinni, 0);
            if ((gFrameCount & 15) == 0)
                WorldMap_CreateLinkedEffects(djinni);
            Engine_TaskWait(1);
        }
        BattleEffect_CleanupSceneObjects();
        Engine_ObjectSetPartPalettes(djinni, 0);
        Engine_ActorRunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x102, 30);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 30);
        Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
        Engine_ActorSetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 40);
        Engine_ActorJump(DJINNI, 4, 30);
        UiWork_PushValueSlot(300, 4);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x100, 30);
        Event_ShowMessage(DJINNI, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        Event_ShowMessage(DJINNI, 0);
        Engine_ActorJump(DJINNI, 2, 30);
        Event_ShowMessage(DJINNI, 0);
    {
        /* FAKEMATCH: retain the actor and x argument carriers at the native height shift handoff. */
        register struct FieldActor *actor asm("r0");
        /* FAKEMATCH: retain x in its position argument register. */
        register s32 pos_x asm("r1");
        /* FAKEMATCH: construct the named height seed in its argument register. */
        register s32 tall asm("r2");
        i = 0;
        tall = 128;
        djinni->motion_flags = 0;
        /* FAKEMATCH: move the ordinary actor/x operands before shifting the height, without changing their values. */
        asm("mov %0, %3\n\tmov %1, %4\n\tlsl %2, %2, #13" : "=&r"(actor), "=&r"(pos_x), "+r"(tall) : "r"(djinni), "r"(x));
        Engine_ObjectSetPosition(actor, pos_x, tall, z);
    }
        for (; i < 16; i++) {
            djinni->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 1);
        Event_ShowMessage(DJINNI, 0);
        djinni->motion_flags = 2;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        for (i = 7; i >= 0; i--) {
            djinni->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(DJINNI, 0x102, 30);
        Actor_FaceActor(DJINNI, 0, 0);
        Engine_ActorRunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Engine_ActorJump(DJINNI, 2, 30);
        Event_OpenMessage(DJINNI, 0);
        i = 0;
    plead:
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Engine_ActorJump(DJINNI, 2, 20);
            Engine_ActorJump(DJINNI, 2, 20);
            if (i == 6) {
                Engine_EventSetMessage((s32)MsgWorldMapMeanieDontCare);
                Event_ShowMessage(DJINNI, 0);
                goto listened;
            }
            Engine_EventSetMessage(i + (s32)MsgWorldMapComePromiseWont);
            Event_OpenMessage(DJINNI, 0);
            i++;
            goto plead;
        }
        Engine_ActorSetAnimation(0, 22);
        Engine_ActorJump(DJINNI, 2, 20);
        Engine_ActorJump(DJINNI, 4, 20);
        Engine_EventSetMessage((s32)MsgWorldMapSeeWontRegret);
        Event_ShowMessage(DJINNI, 0);
listened:
        UiWork_PushValueSlot(300, 4);
        Audio_PlayCue(81);
        i = (s32)MsgWorldMapAbilityVenusDjinni;
        Engine_MessageShowCentered(i++, 3);
        Engine_EventSetMessage(i);
        {
            register s32 frames asm("r2") = 20; /* FAKEMATCH: pins the frames to r2 */
            register s32 flags asm("r1"); /* FAKEMATCH: pins the flags to r1 */

            asm volatile("" : : "r"(frames)); /* FAKEMATCH: sets the jump's frames first */
            Engine_ActorJump(DJINNI, 2, frames);
            flags = 0;
            asm volatile("" : : "r"(flags)); /* FAKEMATCH: sets the message flags first */
            Event_ShowMessage(DJINNI, flags);
        }
        Audio_PlayCue(9);
        goto finish;
    }
    Engine_EventBegin();
    leader = Object_GetById(0);
    if (leader != 0)
        Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
    djinni->velocity_y = 0xa0000;
    Engine_ObjectSetPosition(djinni, x, 0, z);
    Engine_EventWait(30);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_FaceActor(DJINNI, 0, 0);
    Actor_FaceActor(0, DJINNI, 0);
    Engine_ActorSetAnimation(0, 22);
    Engine_EventSetMessage((s32)MsgWorldMapSeeDjinnUseful);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorJump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Engine_ActorRunRepeatedMotion(DJINNI, 2);
    Event_ShowMessage(DJINNI, 0);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Set(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Engine_EventSetMessage((s32)MsgWorldMapGoSetStandby);
    Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
    Engine_EventWait(30);
    Event_ShowMessage(DJINNI, 0);
    Actor_FaceActor(DJINNI, 0, 0);
    Event_ShowMessage(DJINNI, 0);
    Event_OpenMessage(DJINNI, 0);
    if (Engine_EventChooseYesNo(0, 0) != 1)
        goto learn;
    Engine_ActorSetAnimation(0, 22);
    Engine_ActorRunRepeatedMotion(DJINNI, 2);
    Engine_EventSetMessage((s32)MsgWorldMapHmmmmExplainAgain);
    Event_OpenMessage(DJINNI, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1)
        goto learn;
    Event_ShowMessage(DJINNI, 0);
    {
        register s32 far asm("r3") = z; /* FAKEMATCH: pins z to r3 */
        register s32 near asm("r2") = x; /* FAKEMATCH: pins x to r2 */
        register s32 tile_x asm("r1"); /* FAKEMATCH: pins the tile x to r1 */
        register s32 tile_z asm("r2"); /* FAKEMATCH: reuses r2 for the tile z */

        asm volatile("" : : "r"(far), "r"(near)); /* FAKEMATCH: copies z before x */
        tile_x = near >> 16;
        asm("asr %0, %1, #16" : "=l"(tile_z) : "l"(far), "l"(tile_x)); /* FAKEMATCH: shifts z into r2 after the tile x */
        Actor_WalkToAndWait(DJINNI, tile_x, tile_z);
    }
finish:
    FieldScene_RunScene371_02001c08();
    Battle_ClearObjectFlag5bWhenMode3();
    return;
learn:
    Engine_ActorSetAnimation(0, 22);
    Engine_EventSetMessage((s32)MsgWorldMapYeahWantLearn);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Actor_ShowEmote(DJINNI, 0x100, 30);
    Event_ShowMessage(DJINNI, 0);
    GameFlag_Set(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Engine_ActorJump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_RunPageEffectForSlot(DJINNI, 0, 0);
    Audio_PlayCue(42);
    Engine_EventEnd();
    GameFlag_Clear(FLAG_DJINNI_MET);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
}
