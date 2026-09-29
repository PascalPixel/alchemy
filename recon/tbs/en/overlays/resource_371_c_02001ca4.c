/* NONMATCHING: not-yet-c, 1488 candidate / 1488 reference bytes;
 * 118 differing halfwords, 52 normalized edits (2026-09-27).
 * Whole raw owner [02001ca4,02002274), three owned literal-pool groups.
 * Entry is referenced by the overlay's callback table as runtime 02009ca5;
 * it consumes no entry arguments. Neighbors end at 02001ca4 and begin at
 * 02002274. Main callees and both local callees were resolved from own ROM.
 * MsgWorldMapAbilityVenusDjinni identifies the Venus Djinni joining the leader, followed by
 * the Djinn tutorial; the name is descriptive, not original source spelling.
 *
 * Registered baseline: 1488 bytes, 118 halfwords, 52 edits. H1 reconstructs
 * actor pointers and shared FieldActor accesses, gives imports their engine
 * names and verified signatures, and removes address-name aliases. Complete
 * normalized diff shows unchanged anchor/motion-byte pointer allocation.
 * The value-returning GameFlag_Set interface also allows the two tail flags
 * to stay live across calls, adding four bytes. Control-flow topology stays
 * equal. These API/ownership facts are retained; this is not an adoption.
 * H2 represents midpoint locals with the shared coordinate union and reads
 * its signed pixel view at the two walking calls. The complete candidate is
 * byte-identical to H1: 1492 bytes, 190 halfwords, 81 normalized edits. No
 * stack storage is introduced, but neither midpoint lifetime nor the r2/r3
 * reload set changes. Full normalized differences and allocation were read.
 * Stop this bounded pass after these two ownership hypotheses. Next work
 * needs evidence for a distinct lifetime/call boundary, not type spellings.
 *
 * ENTRY_STATE.C boundary transfer: exact IMIRU_MURA/ENTRY_STATE.C uses
 * Call1 for flag clears to shorten constant/actor lifetimes. All four sets
 * here now use Discard1, whose s32-returning function pointer preserves the
 * canonical Engine_GameFlagSet declaration while discarding its result.
 * One bounded trial restores 1488 bytes, 118 halfwords / 52 edits, equal
 * topology, versus 1492 / 190 / 81 for direct sets. Full normalized and
 * compiler-assembly differences show only the decline-tail constant
 * sharing changed: r6/r5-held flags become per-use r0 pool loads, eliminating
 * two early loads and restoring the final pool's address. The complete
 * decline tail and pool [020021b0,02002274) are now byte-exact (196 bytes),
 * but the whole owner remains not-yet-c and earns no DONE.
 * The earlier r2 anchor reloads, r8 mode-pointer inheritance, first-pool
 * alignment, message-base lifetime and argument scheduling are unchanged.
 * In particular this boundary does not admit the reference's r1 reload set.
 * Freeze the corrected flag boundary; no further call-wrapper variations
 * without independent evidence for one of those remaining ownership sites.
 * Sol 6 2026-09-27 bounded input-family pass: H1 moved the complete midpoint
 * formula into MidpointCoordinate(pos, anchor). Extent stays 1488, but the
 * anchor no longer survives in fp/r7 across later position calls: one high
 * register disappears, the first pool moves four bytes, and the normalized
 * residual grows to 211 edits (715 differing halfwords). It still loads the
 * negative anchors through r2, so reject it against the entry invariant.
 * H2 limits the helper to (pos-anchor)/2, adding the anchor in the caller.
 * This restores the baseline binary exactly, including the shared anchors,
 * but does not admit r1. H3 returns the motion-byte pointer from an inline
 * initializer after its six stores; binary again equals baseline exactly.
 * Full normalized differences read for all three tests. Restore the plain
 * source; close midpoint and motion-return boundaries. Current complete
 * draft remains 1488 bytes / 118 halfwords / 52 edits, not-yet-c. Resume only
 * with new scheduling or ownership evidence for the r1 reload invariant.
 *
 * Producer/lifetime audit: complete normalized diff and entry RTL through
 * local/global allocation read, plus exact HAIDIA_IE/HAIDIA.C scene sibling.
 * Hypothesis: the sibling's separate nullable-actor producer could supply
 * a missing low-register lifetime. Admission requires negative-anchor loads
 * through r1, delayed r8 inheritance after the first mode store, and later
 * mode reloads through r1, with tail, pools and topology unchanged. Budget:
 * at most two source-family trials / twenty minutes; no spelling sweeps.
 * The existing record pseudo 33 already has three disjoint lifetimes and
 * dies at entry insn 43 (z load); allocation keeps it in r0. Anchors 47/56
 * cross 41 calls; midpoint values 34/35 cross 87. Reload insns 26/47 use r2;
 * mode producer 305 uses r3, copying into r8 before store 309 reloads r2.
 * Diagnostic text equals ordinary compilation. The sibling establishes no
 * additional producer or overlapping lifetime here, so no source trial is
 * admitted. Missing fact: an own-ROM-supported producer/live range that
 * changes the r1 reload admission while retaining those anchor lifetimes.
 * Stop with the canonical 1488 / 118 / 52 draft; no newly adopted bytes.
 */
#include "FIELD_EVENT.H"
extern u8 MsgWorldMapAbilityVenusDjinni[];
extern u8 MsgWorldMapComePromiseWont[];
extern u8 MsgWorldMapGoSetStandby[];
extern u8 MsgWorldMapHmmmmExplainAgain[];
extern u8 MsgWorldMapMeanieDontCare[];
extern u8 MsgWorldMapOh[];
extern u8 MsgWorldMapSeeDjinnUseful[];
extern u8 MsgWorldMapSeeWontRegret[];
extern u8 MsgWorldMapYeahWantLearn[];

/*
 * resource_371 owner at 0x02001ca4, 1,488 bytes.
 *
 * A field scene step for actor 8.  It reads the party-leader record (slot 0),
 * halves the distance between that record's x/y and the fixed anchor
 * (0x15d00000, 0x05300000) to get the position the camera and the actor are
 * driven to, then branches on game flag 0x16e:
 *
 *   - flag clear: the long first-visit presentation.  It sets 0x16e, plays
 *     the approach animation, walks actor 8 to the anchor, runs a 60-frame
 *     alternating-mode idle driven by the shared frame counter at 0x03001e40,
 *     shows MsgWorldMapOh, then pages messages 0xc5c..0xc62 while the reader
 *     keeps confirming (MsgWorldMapSeeWontRegret when the reader cancels), draws a
 *     quantity, and finishes with message 0xc64/0xc65.
 *   - flag set: the short repeat-visit branch.  It shows message 0xc68/0xc6a,
 *     sets 0x16f and clears 0x171, and asks two yes/no questions.  Both
 *     accepted answers fall through to the shared tail; a refusal runs the
 *     decline block, which sets 0x16f and 0x171, plays cue 42 and clears
 *     0x16e/0x16f/0x171 again.
 *
 * Both accepted paths end by calling the neighbouring owner at 0x02001c08 and
 * the 0x0808a3e0 finish helper.
 *
 * The second position call's 0x100000 is fixed-point Y (height), as shown by
 * the pointer-taking Engine_ObjectSetPosition interface. Actor +20 and +72
 * remain unknown words; established fields use the shared header.
 *
 * Historical integer-offset baseline (2026-09-24): 1,488 of 1,488 bytes, topology
 * equal, 118 differing halfwords.  The wait loops written as counting-up for
 * loops now match, and page = 0 set before the flag test gives the reference
 * its early zero in r5.  What is left is reload-register choice: the reference
 * loads the two anchor pool constants and the +85 mode-pointer store address
 * through r1, so r1 is among its spill registers (the reload round robin starts
 * there); here the spill set is only r2/r3 (see the -dg dump), which also costs
 * the reload inheritance of the mode pointer (mov r8, r2 after its store).
 * Moving the mode store into the position call arguments, direct calls and
 * pointer temporaries did not add r1.  Plus the argument-setup order of a few
 * calls and the one interworking branch described below.  No branch, loop,
 * call, argument or store is missing.
 *
 * Read that figure with the caveat below: the call-shape helpers are a source
 * spelling device, not recovered structure.  The same statements written as
 * plain direct calls measure 1,496 bytes and 317 differing halfwords, so
 * roughly three fifths of the apparent agreement comes from the helpers and
 * from the block-scoped constant temporaries, not from recovered source.  The
 * control flow, calls, arguments and stores below are evidence; the residual
 * number is not a claim about how close the original spelling is.
 */

/* Resolved overlay-local and engine calls; bindings live in the unit. */
union PairObject;
void FieldScene_RunScene371_02001c08(void);
void WorldMap_CreateLinkedEffects(union PairObject *parent);
void Engine_UiWorkPushValueSlot(s32 value, s32 slot);
s32 Engine_DjinnAddToOwner(s32 owner, s32 element, s32 djinn);
s32 Engine_TradeAddOffer(s32 owner, s32 item, s32 count);
void Main_08077260(s32 mode);
s32 Main_0808a070(s32 speaker, s32 flags);
void Main_0808a3d8(void);
void Main_0808a3e0(void);
void Main_0808a428(s32 cue, s32 mode);
void Main_0808a440(void);
void Main_0808a4f8(s32 actor, s32 first, s32 second);
void Main_0808a5c0(s32 speed, s32 acceleration);
void Main_0808a5c8(void);
void Main_080a1040(void);

/* The shared frame counter the idle loop samples. */
#define FRAME_COUNTER (*(s32 *)0x03001e40)

/* The actor this scene step drives, and the party-leader record slot. */
#define ACTOR 8
#define LEADER 0

/* The anchor the actor and camera are driven to, in 16.16 field units. */
#define ANCHOR_X 0x15d00000
#define ANCHOR_Y 0x05300000

/* FAKEMATCH: Call-shape helpers, following the convention already used by most of the
 * drafted overlay scene files in this directory.  A site spelled through one
 * of these passes its constants straight into the argument registers, while a
 * direct call precomputes a costly constant into a pseudo the compiler then
 * shares with later uses in the block; a value-returning helper also sets r0
 * last of its arguments.
 *
 * These are a reading and spelling aid, not a recovered interface: the
 * original almost certainly wrote every site as a plain direct call.  Which
 * helper a site uses is chosen per call site to follow the reference's
 * argument-setup order, so the mixture below carries no meaning.  Every
 * function keeps one declared type across the whole file; no site casts a
 * veneer to a different signature. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/* FAKEMATCH: transfer ENTRY_STATE.C's per-call constant boundary while
 * retaining the flag setter's canonical s32 return type. */
static __inline__ void Discard1(s32 (*f)(s32), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void PositionActor(struct FieldActor *actor, s32 x, s32 y, s32 z)
{
    Engine_ObjectSetPosition(actor, x, y, z);
}

void WorldMap_RunVenusDjinniMeeting(void)
{
    struct FieldActor *rec8;
    struct FieldActor *record;
    union FieldCoordinate mid_x;
    union FieldCoordinate mid_y;
    s32 cnt;
    s32 page;
    s32 text;
    u8 *mode;

    rec8 = Engine_ActorGet(ACTOR);
    record = Engine_ActorGet(LEADER);
    mid_x.fixed = (record->x.fixed - ANCHOR_X) / 2 + ANCHOR_X;
    mid_y.fixed = (record->z.fixed - ANCHOR_Y) / 2 + ANCHOR_Y;
    page = 0;
    if (Value1(Engine_GameFlagIsSet, 0x16e) == 0) {
        /* First visit: claim the flag and play the long presentation. */
        Main_08077260(1);
        Discard1(Engine_GameFlagSet, 0x16e);
        Engine_EventBegin();
        record = Engine_ActorGet(LEADER);
        if (record != 0) {
            Engine_ActorSetPosition(ACTOR, record->x.fixed, record->z.fixed);
        }
        Value3(Engine_DjinnAddToOwner, 0, 0, 0);
        Value3(Engine_TradeAddOffer, 0, 0, 0);
        Main_0808a3d8();
        Engine_ActorFaceActor(0, ACTOR, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 0, 0x101, 60);
        {
            s32 shown = 1;

            rec8->unknown_66 = shown;
        }
        Engine_ActorFaceActor(ACTOR, 0, 0);
        Engine_TaskWait(16);
        Call1(Engine_EventSetMessage, (s32)MsgWorldMapOh);
        Engine_EventShowMessage(ACTOR, 0);
        Main_0808a3e0();
        Call2(Main_0808a5c0, 0x13333, 6);
        Main_0808a5c8();
        Main_0808a3d8();
        mode = &rec8->motion_flags;
        *mode = 2;
        *(s32 *)(rec8->unknown_44 + 4) = 0x4000;
        rec8->speed = 0x10000;
        rec8->acceleration = 0x10000;
        rec8->velocity_y = page;
        *(s32 *)(rec8->unknown_14) = page;
        Engine_ObjectSetPosition(rec8, ANCHOR_X, 0, ANCHOR_Y);
        for (cnt = 0; cnt < 16; cnt++) {
            rec8->scale_x += 0x800;
            rec8->scale_y += 0x800;
            Engine_TaskWait(1);
        }
        Engine_ActorFaceActor(ACTOR, 0, 0);
        Engine_ActorFaceActor(0, ACTOR, 0);
        Engine_TaskWait(16);
        rec8->update = 0;
        Engine_ObjectSetPartPalettes(rec8, 0);
        *(s32 *)(rec8->unknown_44 + 4) = 0x10000;
        Call2(Engine_EventShowMessage, ACTOR, 0);
        Engine_AudioPlayCue(131);
        Main_0808a428(140, 0);
        /* Sixty frames of the alternating idle mode, refreshed every 16. */
        for (cnt = 0; cnt < 60; cnt++) {
            if ((FRAME_COUNTER & 2) != 0) {
                Engine_ObjectSetPartPalettes(rec8, 7);
            } else {
                Engine_ObjectSetPartPalettes(rec8, 0);
            }
            if ((FRAME_COUNTER & 15) == 0) {
                WorldMap_CreateLinkedEffects((union PairObject *)rec8);
            }
            Engine_TaskWait(1);
        }
        Main_0808a440();
        Engine_ObjectSetPartPalettes(rec8, 0);
        Engine_ActorRunRepeatedMotion(ACTOR, 2);
        Engine_EventShowMessage(ACTOR, 0);
        Call3(Engine_ActorShowEmote, 0, 0x102, 30);
        Engine_EventShowMessage(ACTOR, 0);
        Call3(Engine_ActorShowEmote, 0, 0x101, 30);
        Engine_ActorWalkToAndWait(ACTOR, mid_x.part.pixel, mid_y.part.pixel);
        Engine_ActorSetAnimation(0, 22);
        Engine_EventShowMessage(ACTOR, 0);
        Call3(Engine_ActorShowEmote, 0, 0x101, 40);
        Engine_ActorJump(ACTOR, 4, 30);
        Call2(Engine_UiWorkPushValueSlot, 0x12c, 4);
        Engine_EventShowMessage(ACTOR, 0);
        Call3(Engine_ActorShowEmote, 0, 0x100, 30);
        Engine_EventShowMessage(ACTOR, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        Engine_EventShowMessage(ACTOR, 0);
        Engine_ActorJump(ACTOR, 2, 30);
        page = 0;
        Engine_EventShowMessage(ACTOR, 0);
        *mode = page;
        PositionActor(rec8, mid_x.fixed, 0x100000, mid_y.fixed);
        for (cnt = 0; cnt < 16; cnt++) {
            rec8->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 1);
        Call2(Engine_EventShowMessage, ACTOR, 0);
        *mode = 2;
        rec8->velocity_y = 0;
        *(s32 *)(rec8->unknown_14) = 0;
        for (cnt = 0; cnt < 8; cnt++) {
            rec8->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 22);
        Engine_EventShowMessage(ACTOR, 0);
        Call3(Engine_ActorShowEmote, ACTOR, 0x102, 30);
        Engine_ActorFaceActor(ACTOR, 0, 0);
        Engine_ActorRunRepeatedMotion(ACTOR, 2);
        Engine_EventShowMessage(ACTOR, 0);
        Engine_ActorJump(ACTOR, 2, 30);
        Engine_EventOpenMessage(ACTOR, 0);
        /* Page through 0xc5c..0xc62 while the reader keeps confirming. */
        page = 0;
    page_loop:
        if (Value2(Main_0808a070, 0, 0) == 1) {
            Engine_ActorJump(ACTOR, 2, 20);
            Engine_ActorJump(ACTOR, 2, 20);
            if (page == 6) {
                Call1(Engine_EventSetMessage, (s32)MsgWorldMapMeanieDontCare);
                Engine_EventShowMessage(ACTOR, 0);
                goto paged;
            }
            Engine_EventSetMessage(page + (s32)MsgWorldMapComePromiseWont);
            Engine_EventOpenMessage(ACTOR, 0);
            page = page + 1;
            goto page_loop;
        }
        /* The reader cancelled before the last page. */
        Engine_ActorSetAnimation(0, 22);
        Engine_ActorJump(ACTOR, 2, 20);
        Engine_ActorJump(ACTOR, 4, 20);
        Call1(Engine_EventSetMessage, (s32)MsgWorldMapSeeWontRegret);
        Engine_EventShowMessage(ACTOR, 0);
    paged:
        Call2(Engine_UiWorkPushValueSlot, 0x12c, 4);
        Engine_AudioPlayCue(81);
        text = (s32)MsgWorldMapAbilityVenusDjinni;
        Call2(Engine_MessageShowCentered, text, 3);
        text = text + 1;
        Call1(Engine_EventSetMessage, text);
        Engine_ActorJump(ACTOR, 2, 20);
        Call2(Engine_EventShowMessage, ACTOR, 0);
        Engine_AudioPlayCue(9);
        goto play;
    }
    /* Repeat visit: the short branch with two confirmations. */
    Engine_EventBegin();
    record = Engine_ActorGet(LEADER);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR, record->x.fixed, record->z.fixed);
    }
    rec8->velocity_y = 0xa0000;
    Engine_ObjectSetPosition(rec8, mid_x.fixed, 0, mid_y.fixed);
    Engine_EventWait(30);
    Main_0808a3d8();
    Engine_ActorFaceActor(ACTOR, 0, 0);
    Engine_ActorFaceActor(0, ACTOR, 0);
    Engine_ActorSetAnimation(0, 22);
    Call1(Engine_EventSetMessage, (s32)MsgWorldMapSeeDjinnUseful);
    Engine_ActorJump(ACTOR, 2, 20);
    Engine_ActorJump(ACTOR, 2, 20);
    Engine_EventShowMessage(ACTOR, 0);
    Engine_ActorRunRepeatedMotion(ACTOR, 2);
    Call2(Engine_EventShowMessage, ACTOR, 0);
    Engine_AudioPlayCue(111);
    Engine_UiWorkWaitThenFinalizeCapacity(0, 2);
    Discard1(Engine_GameFlagSet, 0x16f);
    Call1(Engine_GameFlagClear, 0x171);
    Main_080a1040();
    Call1(Engine_EventSetMessage, (s32)MsgWorldMapGoSetStandby);
    Engine_ObjectSetPosition(rec8, ANCHOR_X, 0, ANCHOR_Y);
    Engine_EventWait(30);
    Engine_EventShowMessage(ACTOR, 0);
    Engine_ActorFaceActor(ACTOR, 0, 0);
    Engine_EventShowMessage(ACTOR, 0);
    Value2(Engine_EventOpenMessage, ACTOR, 0);
    if (Value2(Main_0808a070, 0, 0) == 1) {
        Engine_ActorSetAnimation(0, 22);
        Engine_ActorRunRepeatedMotion(ACTOR, 2);
        Call1(Engine_EventSetMessage, (s32)MsgWorldMapHmmmmExplainAgain);
        Value2(Engine_EventOpenMessage, ACTOR, 0);
        if (Value2(Main_0808a070, 0, 0) != 1) {
            Engine_EventShowMessage(ACTOR, 0);
            Engine_ActorWalkToAndWait(ACTOR, mid_x.part.pixel, mid_y.part.pixel);
        play:
            FieldScene_RunScene371_02001c08();
            Main_0808a3e0();
            return;
        }
    }
    /* Declined: undo the branch flags and leave. */
    Engine_ActorSetAnimation(0, 22);
    Call1(Engine_EventSetMessage, (s32)MsgWorldMapYeahWantLearn);
    Engine_ActorJump(ACTOR, 2, 20);
    Engine_ActorJump(ACTOR, 2, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Call3(Engine_ActorShowEmote, ACTOR, 0x100, 30);
    Call2(Engine_EventShowMessage, ACTOR, 0);
    Discard1(Engine_GameFlagSet, 0x16f);
    Discard1(Engine_GameFlagSet, 0x171);
    Main_080a1040();
    Engine_ActorJump(ACTOR, 2, 20);
    Engine_EventShowMessage(ACTOR, 0);
    Main_0808a3e0();
    Main_0808a4f8(ACTOR, 0, 0);
    Engine_AudioPlayCue(42);
    Engine_EventEnd();
    Call1(Engine_GameFlagClear, 0x16e);
    Call1(Engine_GameFlagClear, 0x16f);
    Call1(Engine_GameFlagClear, 0x171);
}
