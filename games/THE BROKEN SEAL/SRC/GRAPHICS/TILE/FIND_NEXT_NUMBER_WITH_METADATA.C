/* Debug animation viewer: ten sprites show four editable columns of
 * metadata, animation, priority and draw parameters. With Start held,
 * directions move the probe ring; otherwise up/down pick a column and
 * left/right pick its field. L/R step a field, A applies or toggles it,
 * Start toggles object flags, and Select restarts through the far-call
 * table, with B choosing the alternate row.
 * The Byte6 setter receives priority at offset 5; Byte5 receives param at
 * offset 6. The initial DMA fill clears one word of the 160-byte work block. */

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
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_PARTY.H"
#include "B5_CONTEXT.H"
extern u8 gDecodeFillByte;
extern struct AnimationObject *gSpriteObjects;
extern const u8 FarCall_ResourceTable[];

/* One eight-byte editable column of the icon grid. */
struct IconColumn {
    s16 no;  /* metadata number displayed in this column */
    s16 animation; /* 0..99 script index */
    u8 on;   /* column applied */
    s8 priority; /* 0..3 draw order */
    s8 param; /* 0..15 draw parameter */
};

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
    volatile u32 *held;
    volatile u32 *repeat;
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
        /* FAKEMATCH: the native adds the flags offset to the object's own register (r5) in place; a separate address takes a copy. */
        alt = (s32)&((struct AnimationObject *)alt)->flags;
        *(u8 *)alt = flag;
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
    Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
    WaitFrames(1);
    held = &gKeysHeld;
    repeat = &gKeysRepeat;
    for (;;) {

        if ((*held & KEY_START) != 0) {
            if ((*held & KEY_LEFT) != 0) {
                x--;
            }
            if ((*held & KEY_RIGHT) != 0) {
                x++;
            }
            if ((*held & KEY_UP) != 0) {
                y--;
            }
            if ((*held & KEY_DOWN) != 0) {
                y++;
            }
            Map_BuildProbeRing(x, y, work);
        } else {
            /* The cursor keys read the trigger cell by name, after one read
               whose value is discarded; only the first test goes through the
               pointer. */
            (void)*(volatile u32 *)&gKeysRepeat;
            if ((*repeat & KEY_UP) != 0) {
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
                tbl->flags = flag;
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
            if ((*held & KEY_START) != 0) {
                cnt = 10;
            }
            if ((*repeat & KEY_L) != 0) {
                for (i = 0; i < cnt; i++) {
                    ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
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
            if ((*repeat & KEY_L) != 0) {
                ent[slot].priority--;
                if (ent[slot].priority < 0) {
                    ent[slot].priority = 3;
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
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
            if ((*repeat & KEY_L) != 0) {
                ent[slot].param--;
                if (ent[slot].param < 0) {
                    ent[slot].param = 15;
                }
                moved = 1;
            }
            if ((*repeat & KEY_R) != 0) {
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

        if ((*repeat & KEY_A) != 0) {
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

        if ((*repeat & KEY_SELECT) != 0) {
            Scheduler_ResetTaskTable();
            if ((*held & KEY_B) != 0) {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls, FarCall_ResourceTable);
            } else {
                RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate, FarCall_ResourceTable);
            }
            goto restart;
        }
        WaitFrames(1);
    }
}

/* ui/icon/find_next_number_with_metadata.c */
s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step)
{
    /* FAKEMATCH: the cancelling step initializer preserves the incoming argument moves; a plain initializer reverses them. */
    s32 value = start;
    s32 delta = step + value - value;

    for (;;) {
        value += delta;
        if (value < 0) {
            value = 0x200;
            continue;
        }
        {
            s32 limit = 0x200;
            if (value >= limit) {
                value = -1;
                continue;
            }
        }
        if (Resource_GetMetadataRecordFar(value)->width) {
            return value;
        }
    }
}
