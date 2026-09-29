/* H5 (2026-09-27): initialize the search byte offset before choosing a
 * landing table, testing whether its phase begins before the count's.
 * Complete score 552/552, 240 differing halfwords / 83 aligned edits.
 * Frame 104 and the eleven pool words remain, but the zero becomes a
 * long-lived sl value, the count remains r6 and the selected table moves
 * out of r8. The search and later hold block both regress. Reject this
 * phase model; H4's 17-edit body is restored below. */
/* NONMATCHING: 552 of 552 bytes, 17 differing halfwords / 17 aligned edits
 * (2026-09-27). Whole owner 02000e2c..02001054, return at 02001026 and all
 * eleven literal words at 02001028..02001050. Complete layout/pool exact.
 * Three bounded trials: phase-local actors remove the wait and both burst
 * pointer copies (45 to 26 differing halfwords); choosing an immutable total
 * before the countdown regresses to 560 bytes / 80 edits; one LandingSpot
 * record restores separate coordinate loads instead of ldmia (23 differences).
 * Retain the phase-local actors and typed coordinate record.
 * Hand-written: picks the landing spot nearest the leader for this scene
 * (RamakanSabaku1 or 2 selects the table), drops the leader onto it with two
 * Effect_Spawn bursts and holds the meter at gGameState+0x232 down by 5 a
 * frame for 60 frames. Binds the scene unit's calls plus gFrameCount-free
 * data: the two scene rows and Data_02000240_t. Remaining: global
 * allocation swaps the spot counter and byte offset (r6/r7); the hold-store
 * and timer-decrement scratch registers swap r2/r3. The second parameter
 * block is now exact through H4 below. Further work needs a new structural
 * counter/hold lifetime fact, not actor/coordinate/parameter spelling sweeps.
 * Family transfer (2026-09-27): exact resource_3a5:0200013c instances
 * FIELD/COMMON/EFFECT/SPAWN.C and consumes FIELD_EFFECT.H EffectOptions.
 * The former speed/spread members at +0x10/+0x14 are target x/y scales,
 * not motion inputs; the +0x18 field is a signed effect type. Reuse that
 * complete record and the canonical eight-argument void prototype here.
 * Both 40-byte option records, the 104-byte frame, all eleven pool words
 * and every emitted instruction are identical to the prior candidate.
 * Whole score remains 552/552, 23 halfwords / 23 edits. This corrects the
 * source interface but unlocks 0 DONE bytes. Stop record/prototype-only
 * variants: the three residual regions are unchanged. Exact SPAWN.C and
 * FIELD_EFFECT.H are reused without editing either shared owner.
 * Lamakan H1 (2026-09-27): reference r7 carries the landing countdown,
 * then the meter-clamp zero; reuse count in those disjoint phases. The
 * baseline global allocator chooses countdown pseudo 35 before byte-offset
 * pseudo 38 and gives them r6/r7; its later zero pseudo 45 already has r7.
 * Prediction: the shared carrier admits countdown r7 and offset r6 while
 * retaining the 104-byte frame and all eleven pool words. Result: 552/552,
 * 108 differing halfwords / 38 aligned edits. Countdown stays r6; the known
 * zero left by its loop eliminates the later movs-zero, swaps the hold
 * pointer to r7, and shifts all code after the clamp entry by two bytes.
 * Reject: a shared source local does not recreate the reference phases.
 * This commit preserves the rejected witness; restore the 23-edit baseline
 * before testing a distinct parameter lifetime. No new DONE bytes.
 * Lamakan H2: baseline restored. CSE pseudos 134/136/140 separately hold
 * start y, shared x scale and target y; both x stores precede target-y's
 * definition, allowing all three constants to reuse r3. Extend the shared
 * x scale across the target-y store. Prediction: scale and target-y become
 * interfering pseudos, admitting r2/r3 and the reference's interleaved pool
 * load without disturbing the first burst, frame or layout. Result: 552/552,
 * 19 halfwords / 19 edits. The predicted r2 shared x scale and early r3
 * target-y load are exact, with two final option stores still swapped.
 * Every other block and all pools are unchanged from the 23-edit baseline.
 * Retain this admitted constant-interference witness. A scalar target-y
 * temporary before the x stores can now test the reference store order.
 * Lamakan H3: keep target y in a scalar defined before the x stores;
 * consume it after both x stores. Prediction: retain H2's interfering
 * constants while restoring target-x then target-y stores. One follow-up;
 * exact extent, pools and production gates remain the acceptance contract.
 * H3 result: 552/552, 23 halfwords / 23 edits. Store order is restored,
 * but target_y's user pseudo takes r2 while shared scale takes r3, losing
 * H2's admitted register invariant. This is an explained regression, not
 * an accepted shape. Preserve the negative witness in this commit, then
 * restore H2. The one remaining supported boundary is a scale user local,
 * whose lifetime crosses the start-y store rather than target-y's store.
 * Lamakan H4, final parameter trial: make the shared x scale the user
 * local, define it before start-y, and preserve reference store order.
 * Prediction: the user carrier takes r2 while the two pool constants reuse
 * r3, matching the reference option region. Result: 552/552, 17 halfwords
 * and 17 aligned edits. Whole second option region 02000faa..02000fd8 is
 * exact, and so are the 104-byte frame and all eleven literal pool words.
 * H4 retained; parameter lifetime axis closed with an admitted witness.
 * Current residual: eleven countdown/offset register halfwords and six
 * hold/timer scratch halfwords. H1 falsifies countdown/zero phase merging;
 * no further loop or parameter spelling sweeps. No new DONE or alignment. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "SCENE_IDS.H"

void SceneState_SetHalfwordB030(s32 value);
s32 RamakanSabaku_CalculatePlanarDistance(s32 *from, s32 *to);
void OverlayObject_WaitUntilField12BelowLimit(struct FieldActor *actor, s32 limit);

struct LandingSpot {
    s32 x;
    s32 z;
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02000e2c(void)
{
    struct EventWork *event;
    s32 timer;
    s32 best;
    s32 count;
    s32 *spots;
    s32 left;
    s32 offset;
    s32 *spot;
    s32 pick;
    s32 distance;
    struct FieldActor *actor;
    struct EffectOptions first;
    struct EffectOptions second;
    s16 *meter;
    u16 *hold;
    s32 zero;
    s32 hold_frames = 600;

    event = gEventWork;
    timer = 60;
    best = 0xf00000;
    Call1((void (*)())Engine_GameFlagSet, 0x200);
    SceneState_SetHalfwordB030(1);
    if (Data_02000240_t.halves[224][0] == (s32)&SceneId_RamakanSabaku1) {
        count = 3;
        spots = (s32 *)0x02009f30;
    } else if (Data_02000240_t.halves[224][0] == (s32)&SceneId_RamakanSabaku2) {
        count = 5;
        spots = (s32 *)0x02009f48;
    } else {
        count = 2;
        spots = (s32 *)0x02009f70;
    }
    left = count;
    spot = spots;
    if (count != 0) {
        offset = 0;
        do {
            distance = RamakanSabaku_CalculatePlanarDistance(&Engine_ActorGet(0)->x.fixed, spot);
            if (distance <= best) {
                best = distance;
                pick = left - count;
            }
            offset += 8;
            spot = (s32 *)((u8 *)spots + offset);
        } while (--count != 0);
    }
    pick <<= 1;
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
    {
        struct FieldActor *actor = Engine_ActorGet(0);
        struct LandingSpot *pos = (struct LandingSpot *)&spots[pick];

        Engine_ObjectSetPosition(actor, pos->x, 0, pos->z);
    }
    Engine_ActorGet(0)->velocity_y = 0x60000;
    Engine_AudioPlayCue(152);
    actor = Engine_ActorGet(0);
    OverlayObject_WaitUntilField12BelowLimit(actor, Engine_ActorGet(0)->y.fixed);
    Engine_AudioPlayCue(241);
    {
        struct FieldActor *actor = Engine_ActorGet(0);

        first.type = 214;
        first.start_scale_x = 0x8000;
        first.start_scale_y = 0xcccc;
        first.target_scale_x = 0x10000;
        first.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0, 0x1c0000, &first);
    }
    Call3((void (*)())Engine_ActorShowEmote, 0, 0x104, 0);
    Engine_ActorSetAnimation(0, 18);
    hold = (u16 *)((u8 *)event + 0xcba);
    meter = Data_02000240_t.halves[281];
    zero = 0;
    do {
        *hold = hold_frames;
        timer--;
        if (*meter != 0) {
            *meter -= 5;
            if (*meter <= 0) {
                *meter = zero;
            } else if (timer == 0) {
                timer = 1;
            }
        }
        Engine_TaskWait(1);
    } while (timer != 0);
    {
        struct FieldActor *actor = Engine_ActorGet(0);
        s32 scale;

        second.type = 214;
        /* FAKEMATCH: share one x-scale local across the start-y store. */
        scale = 0x8000;
        second.start_scale_y = 0xcccc;
        second.start_scale_x = scale;
        second.target_scale_x = scale;
        second.target_scale_y = 0x13333;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, timer, timer, 0x1c0000, &second);
    }
    Engine_AudioPlayCue(0x120);
    Engine_AudioPlayCue(152);
    Engine_ActorGet(0)->velocity_y = 0x60000;
    Engine_ActorSetAnimation(0, 1);
    Engine_EventWait(10);
    *(u16 *)((u8 *)event + 0xcba) = timer;
    SceneState_SetHalfwordB030(0);
}
