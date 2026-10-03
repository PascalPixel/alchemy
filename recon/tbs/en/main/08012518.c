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
void Map_BuildProbeRing(s32, s32, void *);
void Ui_SetGridColumnByte5(s32, s32);
void Ui_SetGridColumnByte6(s32, s32);
void Ui_FillGridColumnFromMetadata(s32, s32);
void Ui_SetGridColumnNumber(s32, s32);
void Battle_PlaceActorsByFormationKind(void);

void Ui_RunIconMonitor(void)
{
    struct IconColumn ent[4];
    volatile u32 *keys;
    volatile u32 *trig;
    void *work;
    struct AnimationMetadata *metadata;
    struct AnimationObject *h;
    struct AnimationObject *tbl;
    s32 zero;
    s32 no;
    s32 x;
    s32 y;
    s32 mode;
    s32 flag;
    s32 slot;
    u32 cnt;
    s32 alt;
    s32 moved;
    u32 i;

    keys = &gKeysHeld;
    trig = &gKeysRepeat;
    x = 144;
    y = 96;
    mode = 1;
    flag = 1;
    slot = 0;
    work = Runtime_AllocateBlock(9, 160);
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
    work = Runtime_AllocateBlock(9, 160);
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
        h = (struct AnimationObject *)ResourceObject_Create(ent[0].no + alt + (alt << 12));
        ResourceMetadata_Register(h, ent[1].no);
        ResourceMetadata_Register(h, ent[2].no);
        ResourceMetadata_Register(h, ent[3].no);
        h->flags = (u8)flag;
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

    for (;;) {
        WaitFrames(1);

        if ((*keys & KEY_START) != 0) {
            if ((*keys & KEY_LEFT) != 0) {
                x--;
            }
            if ((*keys & KEY_RIGHT) != 0) {
                x++;
            }
            if ((*keys & KEY_UP) != 0) {
                y--;
            }
            if ((*keys & KEY_DOWN) != 0) {
                y++;
            }
            Map_BuildProbeRing(x, y, work);
        } else {
            /* The reference opens this arm with a load of the trigger cell
               whose result is discarded before the first test. */
            (void)*trig;
            if ((*trig & KEY_UP) != 0) {
                slot = (slot - 1) & 3;
            }
            if ((*trig & KEY_DOWN) != 0) {
                slot = (slot + 1) & 3;
            }
            if ((*trig & KEY_LEFT) != 0) {
                mode = (mode - 1) & 3;
            }
            if ((*trig & KEY_RIGHT) != 0) {
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
            if ((*trig & KEY_L) != 0) {
                ent[slot].animation--;
                if (ent[slot].animation < 0) {
                    ent[slot].animation = 0;
                }
                if (slot == 0) {
                    ent[1].animation = ent[0].animation;
                }
                continue;
            }
            if ((*trig & KEY_R) == 0) {
                break;
            }
            ent[slot].animation++;
            if (ent[slot].animation > 99) {
                ent[slot].animation = 99;
            }
            if (slot == 0) {
                ent[1].animation = ent[0].animation;
            }
            continue;

        case 1:
            cnt = 1;
            moved = 0;
            if ((*keys & KEY_START) != 0) {
                cnt = 10;
            }
            if ((*trig & KEY_L) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
                }
                moved = 1;
            }
            if ((*trig & KEY_R) != 0) {
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
                continue;
            }
            Ui_SetGridColumnNumber(slot, ent[slot].no);
            Ui_SetGridColumnByte6(slot, ent[slot].priority);
            Ui_SetGridColumnByte5(slot, ent[slot].param);
            Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
            continue;

        case 2:
            moved = 0;
            if ((*trig & KEY_L) != 0) {
                ent[slot].priority--;
                if (ent[slot].priority < 0) {
                    ent[slot].priority = 3;
                }
                moved = 1;
            }
            if ((*trig & KEY_R) != 0) {
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
                continue;
            }
            Ui_SetGridColumnByte6(slot, ent[slot].priority);
            continue;

        case 3:
            moved = 0;
            if ((*trig & KEY_L) != 0) {
                ent[slot].param--;
                if (ent[slot].param < 0) {
                    ent[slot].param = 15;
                }
                moved = 1;
            }
            if ((*trig & KEY_R) != 0) {
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
                continue;
            }
            Ui_SetGridColumnByte5(slot, ent[slot].param);
            continue;

        default:
            break;
        }

        if ((*trig & KEY_A) != 0) {
            if (mode == 0) {
                if (slot != 1) {
                    if (ent[slot].on == 0) {
                        continue;
                    }
                    Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
                    if (slot != 0) {
                        continue;
                    }
                    Ui_FillGridColumnFromMetadata(1, ent[1].animation);
                    continue;
                }
            } else if (slot != 0) {
                ent[slot].on = ent[slot].on ^ 1;
                if (ent[slot].on != 0) {
                    Ui_SetGridColumnByte6(slot, ent[slot].priority);
                } else {
                    Ui_SetGridColumnByte6(slot, 8);
                }
                continue;
            }
        }

        if ((*trig & KEY_SELECT) != 0) {
            Scheduler_ResetTaskTable();
            if ((*keys & KEY_B) != 0) {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls, FarCall_ResourceTable);
            } else {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate, FarCall_ResourceTable);
            }
            goto restart;
        }
    }
}
