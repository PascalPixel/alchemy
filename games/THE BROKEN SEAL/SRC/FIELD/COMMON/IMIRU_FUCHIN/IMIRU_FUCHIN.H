#ifndef IMIRU_FUCHIN_H
#define IMIRU_FUCHIN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 gKeysHeld[];

enum {
    /* Message 0x182 + 230. */
    ITEM_DRAGONS_EYE = 230
};

enum TransitionMessage {
    MSG_DRAGONS_FLAME_ILLUMINATES_PATH_TRUTH = 0x17e1,
    MSG_SECRET_KI_SHALL_REVEALED_DISCIPLES = 0x17e2,
    MSG_RAYS_LIGHT_GIVE_BIRTH_SHADOWS = 0x17e3
};

#define NULL ((void *)0)

/* The engine's heap-allocation cache: each slot holds its kind's block. */
extern void *gWorkSlot[];

/* The work in slot 36; its word at +24 is the actor it tracks. */
struct TrackingWork {
    u8 unknown_00[24];
    struct FieldActor *actor;
};

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService {
    u16 unknown00[3];
    u16 value06;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_39a owner at 0x02001004, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_39a {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};

/* Advance both coordinate triplets by their corresponding deltas. */
struct MovingObject {
    u8 unknown00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown14[4];
    s32 sub_x;
    s32 sub_y;
    u8 unknown20[0x10];
    s32 sub_dx;
    s32 sub_dy;
    u8 unknown38[0x0c];
    s32 dx;
    s32 dy;
    s32 dz;
};

/*
 * resource_39a owner at 0x02000ed8, 88 bytes.
 *
 * Creates an actor and, when creation succeeded, initialises it: mode field of
 * the linked record to 1, state byte at +85 cleared, two setup calls, and bit 1
 * of the flag byte at +35 raised. Returns the actor, or null.
 *
 * The mode write is a bitfield assignment; as explicit mask arithmetic the
 * compiler narrows -13 to a byte and the `movs r3,#13 / negs r3,r3` pair is
 * lost.
 */
struct Mode_39a {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Actor_39a {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Mode_39a *f80;       /* +80 */
    u8 pad51[1];
    u8 f85;                     /* +85 */
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

struct PathSubject {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

/* The cave's tables and variables, in the overlay's read-only data: the
 * headings for each held direction, the Dragon's Eye object once it is
 * placed, and the exits. */
extern s16 gImiruFuchinKeyHeadings[];
extern s16 gImiruFuchinHeadings[];
extern s32 gImiruFuchinDragonsEye[];
extern s32 gImiruFuchinExits[];

void ImiruFuchin_BlowCaveMouthDust(void);
void SceneActor_TurnTowardTableAngle(s32 z);

/* The main-image services the cave calls through its import veneers. The
 * declarations are old-style because one name is reached with different
 * argument counts. */
void BattleFx_RunPageEffectForSlot();
void BattleFx_SetPhaseRequest();
void Object_SetPosition();
void Object_CommitPosition();
s32 PartyInventory_FindOwner();
void PartyInventory_Discard();
void Battle_ResetEffectCounter();
void Audio_PlayCueFromEventWork();
void ObjectDispatch_ApplyValueToChildren();
void *ObjectTable_Get();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
s32 Map_GetTerrainHeight();

void ImiruFuchin_HopBy(s32 dx, s32 dz);

/* Imports; the queried ones are typed for their return value. */

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Engine_AudioPlayCue.
 */

/* Imports, named by the address each call site computes. */

/*
 * In-image heading table at 0x0200a464.  0x02000240 is below the link base,
 * so it is a resident table; its word at byte offset 500 selects the subject.
 */

/* Installed callback, named by the linked address of its call word. */

/*
 * Imports named by the address their call site computes, not by a runtime
 * address.  Declarations are old-style because arity varies between sites.
 */
static __inline__ void AdvanceProbe(s32 heading, s32 *probe)
{
    /*
     * Keep this call behind an inline boundary: it is what makes sp+8 be
     * rematerialized for argument 2 before the split 0x100000 constant is
     * completed.
     */
    Vector_AddPolarOffset((s32)0x100000, heading, probe);
}

#endif
