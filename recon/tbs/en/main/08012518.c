/* 2026-10-04 sun w3 finisher 4: alchemy diff 70 (was 351), 1504 bytes,
 * frame 64 as native. What lined up, each measured:
 *   - the pad loop is two loops: the continues re-enter through a first
 *     WaitFrames ("next"), and the fall-through tail runs its own
 *     WaitFrames back to the inner top, so the pad cells are volatile global
 *     reads whose addresses loop.c hoists after the first WaitFrames (fp
 *     held, r9 trigger), not pointer variables set once (351 -> 277);
 *   - mode unsigned: the switch compiles to the native unsigned chain and
 *     the held-cell address outranks mode, which spills to sp+16 (-> 194);
 *   - work declared after flag gives the native spill-slot order (-> 132);
 *   - the DMA source word volatile: its stores go through r4 (-> 124);
 *   - the toggle passes on ? priority : 8 to one call (-> 92); moved is
 *     cleared before cnt is set (-> 90);
 *   - one int holds the alternate flag and then the created object, as the
 *     native shares r5 between them (-> 70).
 * Remaining: (1) the left/right arm's first test reads through the hoisted
 * r9 in the native, after a discarded read through a fresh r1 that the
 * other three tests reuse; here all four use r1, and the slot temporaries
 * swap r0/r2. Pointer variables for the trigger cell score 223. (2) the
 * third Register call sets r0 before its ldrsh and the flags store adds 38
 * to r5 in place; here r3 is a copy. Tried for (2): flag without the cast,
 * a byte store at obj + 38, an AnimationObject temporary (all 70). */
/* 2026-10-04 shared point contract: the monitor owns ten AnimationPosition
 * records, consumed at 16-byte intervals by TILE4.C. The one-word native
 * clear remains unchanged; typing the allocation preserves score 6238. */
#include "RUNTIME_MEM.H"
/* main:08012518 - interactive icon/metadata monitor loop.
 *
 * The routine never returns: it sets up a small four column editor over the
 * shared icon grid (the ui/icon helpers Ui_SetGridColumnByte5/Byte6,
 * Ui_SetGridColumnNumber and Ui_FillGridColumnFromMetadata each push one
 * field of one column) and then spins forever on the pad.
 *
 * Outer stage (label "restart" below) rebuilds the scene: it clears the
 * scheduler, reallocates the 160 byte work block, re-registers the resource
 * for the number held in column 0, rebuilds the ten grid records and pushes
 * every column's current state back through the helpers.
 *
 * Inner stage reads the shared gKeysHeld and gKeysRepeat pad cells. With
 * Start held, direction keys nudge the (x, y) placement handed to
 * Map_BuildProbeRing; otherwise
 * they move the column cursor `slot` and the edited field selector `mode`.
 * Shoulder-shaped bits 256/512 in the trigger mask decrement/increment the
 * field named by `mode`, trigger bit 1 applies or toggles a column, and
 * trigger bit 4 leaves through RuntimeDispatch_ReturnZero and restarts the outer stage.
 *
 * Reading aids the reader should not misread:
 *   - Ui_SetGridColumnByte5 and Ui_SetGridColumnByte6 are named for the grid
 *     record field they write, not for the column field they read.  This owner
 *     hands column offset 5 to Ui_SetGridColumnByte6 and column offset 6 to
 *     Ui_SetGridColumnByte5; the reference's ldrsb displacements are the
 *     evidence, and the two are not interchangeable.
 *   - The pointer ResourceObject_Create returns is the 56 byte record
 *     ResourceMetadata_Register walks.  Byte 38 of that record is the same
 *     flags field the gSpriteObjects toggle loop below rewrites.
 *   - RuntimeDispatch_ReturnZero's current body takes no arguments and
 *     returns 0, yet this call
 *     site sets up two.  Its draft declaration leaves the parameter list
 *     unspecified until that interface is recovered; neither argument is
 *     consumed by the current callee.
 *   - The first DMA fill really does transfer one word into the 160 byte work
 *     block: the pooled control word is 0x85000001.  That is the reference's
 *     behaviour, not a truncation here.
 *
 * Current model: the column selects an animation script, draw priority and
 * draw parameter through the shared AnimationEntry producers in TILE4.C.
 * The monitor registers Battle_PlaceActorsByFormationKind as its callback.
 * The restart argument is the even FarCall_ResourceTable label, not a
 * callable Thumb function pointer. The other argument chooses between
 * the two adjacent directory rows holding that table. The purpose of the
 * dormant dispatch call remains unresolved.
 *
 * Trial: the former byte-view draft scored 6798 (68 register, 23 stack,
 * 28 operand, 22 reordered, 24 inserted and 21 deleted instructions).
 * Shared declarations and named sprite fields replace the guessed views
 * below scored 6598 with a full 1504-byte extent (68 register, 23 stack,
 * 28 operand, 22 reordered, 23 inserted and 20 deleted instructions).
 * Naming the two existing resource-directory rows then scored 6238:
 * 68 register, 23 stack, 23 operand, 21 reordered, 22 inserted and 19
 * deleted instructions, with no unresolved symbols. The current extent is
 * 1512 bytes versus 1504 native; this remains an uncredited draft.
 *
 * Residuals of the former draft (the dispatch interface is unresolved):
 *   - The reference keeps the held-mask pointer in fp and the trigger-mask
 *     pointer in r9 and spills `mode` to the frame; this spelling keeps
 *     `mode` in r9 and rematerialises one of the two addresses, which costs
 *     four bytes of frame and shifts every stack displacement.
 *   - The reference dispatches the switch with an unsigned compare chain.
 *     Spelling `mode` (or the switch operand) unsigned reproduces that chain
 *     exactly but lets the compiler cross-jump the two duplicated case 0
 *     tails, dropping fourteen real instructions, so it is not used here.
 *   - Both clears use the shared DMA machine interface in DMA.H.
 *   - The resource rows now explain the literal-pool loads for 17 and 18.
 *     The remaining pooled 1 used for initial priority is still unexplained.
 */

#include "TYPES.H"
#include "IO_REG.H"
#include "ANIMSPR.H"
#include "CALLBACK_SCHEDULER.H"
#include "FRAME.H"
#include "KEYSTATE.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "METADATA_LOOKUP.H"
#include "DMA.H"
extern u8 gDecodeFillByte;
extern struct AnimationObject *gSpriteObjects;
extern const u8 FarCall_ResourceTable[];


/* One editable column of the icon grid.  Eight bytes: the reference indexes
 * the array with slot << 3 and reads offsets 0, 2, 4, 5 and 6 only. */
struct IconColumn {
    s16 no;  /* metadata number displayed in this column */
    s16 animation; /* 0..99 script index */
    u8 on;   /* column applied */
    s8 priority; /* 0..3 draw order */
    s8 param; /* 0..15 draw parameter */
};

LAYOUT_SIZE_GUARD(IconColumn_Size, struct IconColumn, 8);
LAYOUT_OFFSET_GUARD(IconColumn_Priority, struct IconColumn, priority, 5);
LAYOUT_OFFSET_GUARD(IconColumn_Param, struct IconColumn, param, 6);

void Resource_InitializeTable(void);
void Blend_SetDarkenTarget0(s32);
s32 RuntimeDispatch_ReturnZero();
void ObjectSystem_Initialize(s32);
s32 ResourceSlot_Load(u32, u32 *, s32, u32);
s32 ResourceMetadata_Register(struct AnimationObject *, s32);
s32 Ui_FindNextNumberWithMetadata(s32, s32);
void Ui_SetGridColumnByte5(s32, s32);
void Ui_SetGridColumnByte6(s32, s32);
void Ui_FillGridColumnFromMetadata(s32, s32);
void Ui_SetGridColumnNumber(s32, s32);
void Battle_PlaceActorsByFormationKind(void);

void Ui_RunIconMonitor(void)
{
    struct IconColumn ent[4];
    struct AnimationMetadata *metadata;
    struct AnimationObject *tbl;
    volatile s32 zero;
    s32 no;
    s32 x;
    s32 y;
    u32 mode;
    s32 flag;
    struct AnimationPosition *work;
    s32 slot;
    u32 cnt;
    s32 alt;
    s32 moved;
    u32 i;

    x = 144;
    y = 96;
    mode = 1;
    flag = 1;
    slot = 0;
    work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
    gDecodeFillByte = 3;

    zero = 0;
    Dma_Set(&zero, work, 0x85000001, REG_DMA3);
    zero = 0;
    Dma_Set(&zero, ent, 0x85000008, REG_DMA3);

    no = Ui_FindNextNumberWithMetadata(-1, 1);
    for (i = 0; i <= 3; i++) {
        ent[i].animation = 1;
        ent[i].priority = 1;
        ent[i].no = no;
    }
    ent[0].on = 1;
    gDebugMode = 2;
    REG_BLDCNT = 0x3F42;
    PLTT_BACKDROP = 0x01E0;
    REG_DISPCNT = 0x1140;
    Blend_SetDarkenTarget0(1);

restart:
    Runtime_InitializeHeap();
    Scheduler_ResetTaskTable();
    work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
    Resource_InitializeTable();
    ObjectSystem_Initialize(2);
    ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, ent[0].no, 0);
    metadata = Resource_GetMetadataRecordFar(ent[0].no);
    if (metadata->draw_kind == 20) {
        ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, ent[0].no + 1, 0);
    }

    for (i = 0; i <= 9; i++) {
        metadata = Resource_GetMetadataRecordFar(ent[0].no);
        alt = 0;
        if (metadata->draw_kind == 20 && (i & 1) != 0) {
            alt = 1;
        }
        alt = (s32)ResourceObject_Create(ent[0].no + alt + (alt << 12));
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[1].no);
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[2].no);
        ResourceMetadata_Register((struct AnimationObject *)alt, ent[3].no);
        ((struct AnimationObject *)alt)->flags = (u8)flag;
    }

    for (i = 0; i <= 3; i++) {
        if (ent[i].on != 0) {
            Ui_SetGridColumnByte6(i, ent[i].priority);
        } else {
            Ui_SetGridColumnByte6(i, 8);
        }
        Ui_SetGridColumnByte5(i, ent[i].param);
        Ui_FillGridColumnFromMetadata(i, ent[i].animation);
    }

    Map_BuildProbeRing(x, y, work);
    Scheduler_AddOrUpdateCallback((s32)Battle_PlaceActorsByFormationKind, 3200);

next:
    WaitFrames(1);
    for (;;) {

        if ((*(volatile u32 *)&gKeysHeld & KEY_START) != 0) {
            if ((*(volatile u32 *)&gKeysHeld & KEY_LEFT) != 0) {
                x--;
            }
            if ((*(volatile u32 *)&gKeysHeld & KEY_RIGHT) != 0) {
                x++;
            }
            if ((*(volatile u32 *)&gKeysHeld & KEY_UP) != 0) {
                y--;
            }
            if ((*(volatile u32 *)&gKeysHeld & KEY_DOWN) != 0) {
                y++;
            }
            Map_BuildProbeRing(x, y, work);
        } else {
            /* The reference opens this arm with a load of the trigger cell
               whose result is discarded before the first test. */
            (void)*(volatile u32 *)&gKeysRepeat;
            if ((*(volatile u32 *)&gKeysRepeat & KEY_UP) != 0) {
                slot = (slot - 1) & 3;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_DOWN) != 0) {
                slot = (slot + 1) & 3;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_LEFT) != 0) {
                mode = (mode - 1) & 3;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_RIGHT) != 0) {
                mode++;
                mode = mode & 3;
            }
        }

        if ((gKeyState & KEY_START) != 0) {
            tbl = gSpriteObjects;
            flag = flag ^ 1;
            for (i = 0; i <= 9; i++) {
                tbl->flags = (u8)flag;
                tbl++;
            }
        }

        switch (mode) {
        case 0:
            if (slot == 1) {
                break;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_L) != 0) {
                ent[slot].animation--;
                if (ent[slot].animation < 0) {
                    ent[slot].animation = 0;
                }
                if (slot == 0) {
                    ent[1].animation = ent[0].animation;
                }
                goto next;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_R) == 0) {
                break;
            }
            ent[slot].animation++;
            if (ent[slot].animation > 99) {
                ent[slot].animation = 99;
            }
            if (slot == 0) {
                ent[1].animation = ent[0].animation;
            }
            goto next;

        case 1:
            moved = 0;
            cnt = 1;
            if ((*(volatile u32 *)&gKeysHeld & KEY_START) != 0) {
                cnt = 10;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_L) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
                }
                moved = 1;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_R) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, 1);
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (slot == 0) {
                goto restart;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnNumber(slot, ent[slot].no);
            Ui_SetGridColumnByte6(slot, ent[slot].priority);
            Ui_SetGridColumnByte5(slot, ent[slot].param);
            Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
            goto next;

        case 2:
            moved = 0;
            if ((*(volatile u32 *)&gKeysRepeat & KEY_L) != 0) {
                ent[slot].priority--;
                if (ent[slot].priority < 0) {
                    ent[slot].priority = 3;
                }
                moved = 1;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_R) != 0) {
                ent[slot].priority++;
                if (ent[slot].priority > 3) {
                    ent[slot].priority = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnByte6(slot, ent[slot].priority);
            goto next;

        case 3:
            moved = 0;
            if ((*(volatile u32 *)&gKeysRepeat & KEY_L) != 0) {
                ent[slot].param--;
                if (ent[slot].param < 0) {
                    ent[slot].param = 15;
                }
                moved = 1;
            }
            if ((*(volatile u32 *)&gKeysRepeat & KEY_R) != 0) {
                ent[slot].param++;
                if (ent[slot].param > 15) {
                    ent[slot].param = 0;
                }
                moved = 1;
            }
            if (moved == 0) {
                break;
            }
            if (ent[slot].on == 0) {
                goto next;
            }
            Ui_SetGridColumnByte5(slot, ent[slot].param);
            goto next;

        default:
            break;
        }

        if ((*(volatile u32 *)&gKeysRepeat & KEY_A) != 0) {
            if (mode == 0) {
                if (slot != 1) {
                    if (ent[slot].on == 0) {
                        goto next;
                    }
                    Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
                    if (slot != 0) {
                        goto next;
                    }
                    Ui_FillGridColumnFromMetadata(1, ent[1].animation);
                    goto next;
                }
            } else if (slot != 0) {
                ent[slot].on = ent[slot].on ^ 1;
                Ui_SetGridColumnByte6(slot, ent[slot].on != 0 ? ent[slot].priority : 8);
                goto next;
            }
        }

        if ((*(volatile u32 *)&gKeysRepeat & KEY_SELECT) != 0) {
            Scheduler_ResetTaskTable();
            if ((*(volatile u32 *)&gKeysHeld & KEY_B) != 0) {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls, FarCall_ResourceTable);
            } else {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate, FarCall_ResourceTable);
            }
            goto restart;
        }
        WaitFrames(1);
    }
}
