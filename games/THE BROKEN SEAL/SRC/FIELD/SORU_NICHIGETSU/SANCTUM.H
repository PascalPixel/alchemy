#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Luna and Sol rooms deep in Sol Sanctum. Sukureta wonders what the two
 * rooms mean and asks Robin to look farther up the passage. Lighting the four
 * statue lights turns the picture of Luna to Sol, and the trap that follows
 * shakes the statue hall.
 */

enum InnerSanctumActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 8
};

enum InnerSanctumFlag {
    FLAG_INNER_SANCTUM_ENTERED = 0x80a,
    /* The four statue lights. */
    FLAG_STATUE_LIGHT_1 = 0x80b,
    FLAG_STATUE_LIGHT_2,
    FLAG_STATUE_LIGHT_3,
    FLAG_STATUE_LIGHT_4,
    FLAG_LUNA_PICTURE_CHANGED_TO_SOL = 0x810,
    FLAG_STATUE_TRAP_SPRUNG = 0x813,
    /* Robin agreed to look farther up the passage for Sukureta. */
    FLAG_ROBIN_SEARCHING_FOR_SUKURETA = 0x896
};

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void SetSolShindenActorStep();
void SetSolShindenActorStep();

s32 CheckAllStatueLights(void);

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

/*
 * Withdrawn on 2026-09-10 as not reproducible, and reinstated the same night
 * once the cause was found and removed.
 *
 * The pair of equal 0x6480000 arguments in the last call below made this
 * owner's compile split: thirty raw cc1 runs on one fixed preprocessed input
 * produced 27 of one output and 3 of another, the minority recomputing the
 * shift instead of sharing it through mov r2, r1. Three sessions measured it
 * independently and agreed.
 *
 * The cause was not the source. GCC 2.96 hashes LABEL_REF by rtx address and
 * SYMBOL_REF by the address of its name, so macOS ASLR reordered equal-constant
 * discovery in CSE between runs. The toolchain now spawns compilers with
 * _POSIX_SPAWN_DISABLE_ASLR and the split collapses to a single output.
 *
 * Kept as a marker: raw runs spawned outside the toolchain still split 27/3,
 * so anyone measuring this owner by hand must disable ASLR or they will
 * rediscover the artefact rather than a property of this C.
 */

#include "TYPES.H"

/* Message ids handed to the Func_0808a170 service; see the header note. */

/*
 * resource_37a owner 0x02000488..0x020009f3 (0x56c = 1388 bytes).
 *
 * Reviewed scene script for the resource_37a sequence guarded by story flag
 * 0x809.  One straight command list: it opens the scene, places and drives
 * four actor slots (0, 1, 5 and 0x10) through a long beat-by-beat schedule of
 * moves and waits, plays two audio cues, branches once on a runtime query
 * near the end to choose between two message ids, then hands the three
 * principal slots back to their walking records, sets story flags 0x144 and
 * 0x809 and closes the scene.
 *
 * Complete owner.  Prologue `push {lr}` at 0x02000488; the single epilogue
 * `pop {r0} / bx r0` at 0x020009da pops the return address, so the owner is
 * void and takes no argument.  Code runs 0x02000488..0x020009d9 with two
 * interior literal pools (0x0200081c..0x0200084f and 0x020009de..0x020009f3);
 * the next prologue is at 0x020009f4.  All 140 `bl` sites are transcribed.
 *
 * Calls are spelled as the pre-relocation call word the image holds, the
 * convention the adopted sibling
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_scene_37a_sequence_a.c uses,
 * so one target reached from several sites appears under several spellings.
 *
 * RESIDUAL (all 32 remaining differing halfwords).  Eight of those spellings
 * collide: two sites whose different call words happen to decode to the same
 * address (Func_0200316c, Func_02003234, Func_0200326c, Func_02003330,
 * Engine_ActorSetDestination, Func_02003382, Func_020033b2 and Func_02003390).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Func_0808a170 message service are spelled as
 * `Data_00000<id>` externs, the convention the adopted overlay scene scripts
 * already use for this family (see
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_scene_372_sequence_a.c,
 * which declares `extern u8 Data_00000e67[]` and casts it to s32 at the call).
 * The reference forces that reading for 0xff0: every other pooled constant in
 * this owner (0x101, 0x105, 0x2005, 0x6666 ...) is one GCC cannot synthesize,
 * while 0xff0 is 0xff << 4 and from a plain integer literal would come out as
 * `movs r0,#255 / lsls r0,r0,#4`.  The reference loads it from the pool
 * instead, so the operand was address-shaped in the original source.  The
 * other three ids are byte-identical either way; they are spelled the same
 * way because they reach the same service.
 *
 * Uncertain: none of the main-image services is established beyond its
 * argument count and access widths, so all are declared old-style and their
 * constants are transcribed rather than interpreted.  The record returned by
 * the Func_0808a080 service is read at +8/+16 as words early in the scene and
 * at +10/+18 as signed halfwords at the end, so both stay raw offsets; no
 * shared type in games/THE BROKEN SEAL/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */

/* The sixteen colliding sites reach these main-image services directly. */

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
void FieldScene_RunScene37aSequenceA();  /* FieldScene_RunScene37aSequenceA (0x02000054) */
void SetSolShindenActorStep();  /* SetSolShindenActorStep (0x020025fc) */

/*
 * Inner sanctum data getters, published in the descriptor table at the head
 * of overlay resource_37a. Sibling overlays share this layout but not the
 * addresses returned.
 */

#include "TYPES.H"

/*
 * Returns 0x0200aafc, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns zero. The four-byte owner carries no pool word, since a constant
 * that fits in an immediate needs none, unlike its eight-byte neighbours in
 * the same table. Whether the caller reads the zero as a null pointer or as
 * an integer is not established.
 */

/*
 * Returns 0x0200abec, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns 0x0200ac14, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

/*
 * Returns 0x0200ad34, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

#include "TYPES.H"

/* Declared old-style: this parameter list is not established here. */

/* Declared old-style: this parameter list is not established here. */

/*
 * Passes the step to one routine with a zero and the wait to another. Both
 * arguments are read before either incoming register is overwritten, so the
 * order of these two calls is what the reference requires. The zero is
 * transcribed, not interpreted.
 */

#include "TYPES.H"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} EntA;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern u16 SoruNichigetsu_Light1Timer;
extern s32 SoruNichigetsu_FlashState;
extern u16 SoruNichigetsu_Light2Timer;
extern u16 SoruNichigetsu_Light3Timer;
extern u16 SoruNichigetsu_Light4Timer;

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void SetSolShindenActorStep();
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

void SetSolShindenActorStep();
void SetSolShindenActorStep();
