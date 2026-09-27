/* NONMATCHING: 1172 bytes, candidate 1172, 541 differing halfwords, 20
 * wrong instructions, 159 halfword edits. FieldScene_RunComplexActorSequence
 * targets FIELD/COMMON/HAIDIA_BABI/F_00578.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Pointer-taking child-flag and palette calls now consume the actor
 * lookup result; the palette call receives 226. The sprite pointer is now
 * captured before calls, matching the reference's read lifetime. A typed
 * pointer bank shares map/event/auxiliary reads and removes the extra middle
 * pool. A retained nested control pointer gives the reference's +76 anchor,
 * -76 map-work adjustment and +12 auxiliary read. Existing typed inline
 * speed/render/action/dialogue services reproduce their argument reloads.
 * Typed inline open/show dialogue services restore speaker literal reloads.
 * Initial scheduling,
 * sprite/actor registers and byte-pointer copies differ; topology is equal.
 * Typed unknown_5a flag accesses emit the same bytes as offset casts.
 * 2026-09-26 bounded pass, own-ROM full extent [0x02000578,0x02000a0c).
 * Read the complete listing and normalized diff. H1: a u16 stopped local
 * for actor +85, separate from the view actor's word-sized Y zero, emits
 * exactly the baseline bytes (1172 bytes, 385 differing halfwords, 160
 * edits). The reference owns a zero at pool 0x0200063c, also holds a wide
 * zero in sl, and saves a third high register; plain scalar narrowing does
 * not recreate that ownership. Baseline binary equality checked directly.
 * H2: one-halfword aggregate recreates a zero pool word but remains four
 * bytes long, with the sprite in r5 and only two high registers saved.
 * The zero pool alone is insufficient: the reference materializes the
 * independent wide Y zero BEFORE the sprite rotation store and actor lookup.
 * Caller ENTRY_STATE.C dispatches this whole scene for entrance 20 when
 * flag 0x87a is clear, after setting flag 0x834. No arguments are passed.
 * H3: explicit wide ground initialization in a tagged setup block moves
 * that zero before sprite setup and lowers edits to 148, but remains 1176
 * bytes. Sprite stays r5, control sl, ground r8; reference needs sprite r8,
 * control r9, ground sl. Four later byte-flag updates retain extra pointer
 * copies. All three hypotheses preserved in commits; stop this pass here.
 * 2026-09-27 H1: transfer exact DECK_SEQ.C/BY_ID.C actor lookup and void
 * action-table interfaces, bind all three callback tables from this owner's
 * pool, and use FIELD_EVENT.H FieldSprite.rotation instead of a +30 cast.
 * Full listing and normalized diff read. This recovers the formerly missing
 * third high-register save: sprite r8, control r9 and ground sl all agree,
 * as do the complete save/restore sequence and the ground-position store.
 * The original callback was already void; do not attribute the gain to a
 * corrected return type. This is one combined source-interface model.
 * Compared with 1176/344/148, H1 is 1180/561/180; wrong instructions 116->80.
 * Retain the proven high-register roles as an admission check despite the
 * worse length-shifted score. Remaining: workspace entry-load ordering,
 * early narrow-zero/message loads and first pool, animation setup order,
 * and four +90 byte-flag pointer copies. Callback body and tail order agree.
 * No compiler, exact sibling or shared header was changed.
 * H2: exact WORLD_MAP/BLACK_ORB_SCENE.C and HAIDIA_MURA/REPAIR_MORNING.C
 * consume ActorGet directly in each compound +90 flag update. H1's local
 * allocator showed one user pseudo set four times and copied at every
 * byte-address adjustment. Transfer the direct lookup/update boundary at
 * those four sites, removing only p67: all four copies disappear and their
 * store/argument schedules now match. High-register roles remain exact.
 * Full normalized diff read: 1172/1172 bytes, 541 halfwords, 159 edits,
 * 20 wrong instructions. The byte count is not an adoption: first-pool
 * placement shifts the long matched body. Remaining entry and initial
 * animation order, narrow-zero/message hoisting and first pool are unchanged.
 * Do not reopen the earlier zero-width/aggregate/declaration-order axes. */
#include "FIELD_EVENT.H"

struct SceneMapState {
    u8 unknown_0000[0x1f84];
    u16 active;
};

struct SceneControlPointers {
    struct EventWork *event_work;
    u8 unknown_04[8];
    struct SceneMapState *map_state;
};

struct ScenePointerBank {
    u8 *map_work;
    u8 unknown_04[0x48];
    struct SceneControlPointers control;
};

extern struct ScenePointerBank gScenePointers;
extern const u8 HaidiaBabi_SharedAction[];
extern const u8 HaidiaBabi_ActorExitAction[];
extern const u8 HaidiaBabi_LeaderExitAction[];

struct SceneHalf {
    u16 value;
};

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunComplexActorSequence. */

void Local_020017e4();
void Main_080000c0();
void Main_08009128();
void Main_08009188();
void Main_08009190();
void Main_080091e0(struct FieldActor *actor, s32 flags);
void Main_08009208();
void Main_08009210();
void Main_08009228(struct FieldActor *actor, s32 palette);
void Main_08015210();
void Main_080770c8();
void Main_0808a010();
void Main_0808a018();
s32 Main_0808a070();
void Main_0808a0a0();
void Main_0808a0f0();
void Main_0808a100();
void Main_0808a110();
void Main_0808a128();
void Main_0808a130();
void Main_0808a138();
void Main_0808a158();
void Main_0808a170();
void Main_0808a1d8();
void Main_0808a1e0();
void Main_0808a200();
void Main_0808a248();
void Main_0808a2c8();
void Main_0808a2d8();
void Main_0808a2e0();
void Main_0808a360();
void Main_0808a368();
void Main_0808a370();
void Main_080f9010();

void FieldScene_RunComplexActorSequence(void)
{
    s32 base;
    struct FieldSprite *sprite;
    struct FieldActor *p12;
    struct FieldActor *p89;
    u8 *work;
    struct FieldActor *scene_actor;
    struct SceneControlPointers *control;
    struct SceneHalf stopped;
    s32 ground;

    control = &gScenePointers.control;
    work = gScenePointers.map_work;
    scene_actor = control->event_work->view_center;
    sprite = Engine_ActorGet(17)->sprite;
    Main_0808a018();
    Main_0808a0f0(11, 0, 0);
    Main_0808a0f0(12, 0, 0);
    Main_0808a0f0(13, 0, 0);
    Main_0808a0f0(14, 0, 0);
    Main_0808a0f0(15, 0, 0);
    Main_0808a0f0(16, 0, 0);
    Main_080091e0(Engine_ActorGet(0), 0);
    Main_0808a100(0, 18);
    /* FAKEMATCH candidate: delimit the wide position initialization from
     * the narrow actor flag so its lifetime starts in the setup phase. */
    do {
        ground = 0;
    } while (0);
    sprite->rotation = 1365;
    p12 = Engine_ActorGet(17);
    /* FAKEMATCH candidate: keep the byte flag's halfword zero separate
     * from the wide scene-position zero, as the two reference loads are. */
    stopped.value = 0;
    p12->motion_flags = stopped.value;
    Main_080091e0(Engine_ActorGet(17), 0);
    Main_0808a0f0(17, 37748736, 42598400);
    Main_08009188(7);
    Main_0808a0f0(8, 34996224, 45088768);
    Main_08009208();
    Main_0808a1d8(8);
    base = 3666;
    Main_08015210(base, 1, 0);
    Main_0808a010(40);
    MapRender_SetValues(65536, 65536, 65536);
    Main_0808a1d8(8);
    Main_08015210(base + 1, 1, 0);
    Main_08009210();
    Main_0808a010(40);
    *(u32 *)(work + 236) = 0x01480000;
    *(u32 *)(work + 240) = 0x02580000;
    *(u32 *)(work + 244) = 0x02700000;
    *(u32 *)(work + 248) = 0x03300000;
    scene_actor->x.fixed = 0x02340000;
    scene_actor->y.fixed = ground;
    scene_actor->z.fixed = 0x02b30000;
    Main_08009128();
    Main_080000c0(1);
    control->event_work->start_transition = 521;
    control->event_work->transition_frames = 64;
    Main_0808a2c8();
    control->map_state->active = 1;
    Main_0808a2d8();
    Main_080000c0(30);
    Main_0808a360();
    Main_0808a370();
    Main_0808a2e0();
    Main_0808a110(8, 4);
    Main_0808a170(base + 2);
    Event_ShowMessageAndWait(36872, 0, 60);
    Main_0808a138(0, 2);
    Main_0808a010(40);
    Main_0808a138(8, 1);
    Main_0808a010(40);
    Event_ShowMessageAndWait(36872, 0, 20);
    Main_0808a138(0, 2);
    Main_08009190(7);
    Main_0808a010(20);
    Main_08009188(8);
    Actor_SetSpeed(0, 65536, 32768);
    Main_0808a100(0, 19);
    Actor_MoveToAndWait(0, 557, 679);
    Main_08009190(8);
    Main_08009188(9);
    Actor_MoveToAndWait(0, 555, 680);
    Main_0808a010(30);
    Actor_Jump(8, 53248, 0);
    Main_080091e0(Engine_ActorGet(0), 1);
    Main_0808a128(0, 4, 0);
    Actor_WalkToAndWait(0, 543, 674);
    Main_0808a1e0(0, 3);
    Actor_Jump(0, 16384, 40);
    Main_0808a110(8, 4);
    Main_0808a010(20);
    Event_ShowMessage(36872, 0);
    Local_020017e4();
    Main_0808a130(8, 2);
    Event_ShowMessageAndWait(36872, 0, 20);
    Engine_ActorGet(8)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(8, 542, 680);
    Main_0808a010(1);
    Engine_ActorGet(8)->unknown_5a |= 0x1;
    Main_0808a010(10);
    Main_0808a138(8, 2);
    Main_08009228(Engine_ActorGet(0), 226);
    Main_080770c8(33);
    Main_080f9010(126);
    Main_0808a158(0, 7);
    Main_0808a010(10);
    Main_0808a158(0, 0);
    Main_0808a010(20);
    Engine_ActorGet(8)->unknown_5a &= 0xfe;
    Actor_WalkToAndWait(8, 534, 688);
    Main_0808a010(1);
    Engine_ActorGet(8)->unknown_5a |= 0x1;
    Main_0808a010(20);
    Actor_SetSpeed(8, 98304, 49152);
    Actor_SetSpeed(0, 98304, 49152);
    Main_0808a200(8, 1);
    p89 = Engine_ActorGet(0);
    p89->priority_flags |= 0x1;
    Engine_ActorEnableActionCallback(8, HaidiaBabi_SharedAction);
    Main_0808a010(20);
    Engine_ActorEnableActionCallback(0, HaidiaBabi_SharedAction);
    Main_0808a0a0(8);
    Actor_WalkToAndWait(8, 419, 661);
    Actor_WalkToAndWait(8, 408, 661);
    Main_0808a100(8, 1);
    Main_0808a100(0, 1);
    Actor_Jump(8, 16384, 10);
    Event_OpenMessage(32776, 0);
    if (Main_0808a070(0, 0) == 0) {
        control->event_work->message++;
    }
    Main_0808a010(20);
    Event_ShowMessageAndWait(32776, 0, 20);
    Main_0808a100(0, 3);
    Main_0808a110(8, 3);
    Main_0808a010(20);
    Engine_ActorEnableActionCallback(8, HaidiaBabi_ActorExitAction);
    Engine_ActorEnableActionCallback(0, HaidiaBabi_LeaderExitAction);
    Main_0808a010(20);
    control->event_work->start_transition = 513;
    control->event_work->transition_frames = 16;
    Main_0808a368();
    Main_0808a370();
    Main_0808a248(20);
}
