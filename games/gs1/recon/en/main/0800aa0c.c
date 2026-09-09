#include "types.h"
#include "global_cells.h"
#include "video_dma_family.h"

/*
 * Composite animation frame builder at 0x0800aa0c.
 *
 * One object owns up to four animation entries.  Each entry runs a small
 * byte script (opcode + argument pairs) that selects a frame, then a
 * direction table turns the caller's 16-bit heading into a frame offset.
 * When any entry changed frame the object is redrawn: a scratch buffer of
 * width*height bytes is composed from the entries in priority order,
 * optionally outlined, and handed to the IWRAM-resident upload routine
 * that writes it into VRAM at 0x06010000 + tile*32.
 *
 * Provenance notes for the reader:
 *   - The routine copied into heap block 52 is the ROM span
 *     0x08009bb8..0x08009d9c.  The reference computes the DMA word count
 *     from the two addresses at run time, so the extents are kept as two
 *     symbols rather than one folded constant.
 *   - Heap slots live in the table at 0x03001e50 indexed by block id
 *     (Runtime_ReleaseHeapBlock uses the same table); slot 52 is
 *     0x03001e50+208 and slot 53 is 0x03001e50+212.  The reference reaches
 *     slot 52 through the 0x03001e68 cell it already had in a register and
 *     slot 53 through the 0x03001e50 base, so both spellings are kept.
 *   - 0x03000164 and 0x030005c0 are IWRAM routines called through function
 *     pointers, the way games/gs1/src/ui/render/clear_vram_block.c calls
 *     0x03000168.  The 0x03000164 call sites here set only r0 and r1, so
 *     the typedef takes destination and length; other owners pass a third
 *     fill value, which is not invented back into this one.
 *   - 0x080072f0 and 0x080072f4 are the __call_via_r3 / __call_via_r4
 *     veneers, not callees; agbcc emits them for the indirect calls above.
 *
 *   - The block size reaches the allocator from the pool, not as an
 *     immediate, so it is spelled as an absolute-address symbol the way
 *     other owners spell resource constants.
 *
 * Uncertain: the meaning of entry kinds 7 and 9..87 (they fall to the
 * "no direction table" default), and whether the entry array is really
 * capped at four - the reference's sort scratch only holds four keys, so
 * a fifth live entry would overrun it.
 *
 * Residual, measured rather than guessed.  The reference frame is 56 bytes:
 * twelve spill words plus the eight-byte sort scratch.  This spelling needs
 * sixty, because GCC strength-reduces the sort's "order[n]" into a pointer
 * induction variable and therefore spills the scratch base as a thirteenth
 * word, where the reference rematerialises "add r5, sp, #48" inside the
 * loop and carries only a byte offset.  That one extra word moves every
 * later sp-relative operand and is the largest single contributor to the
 * differing halfwords.  Three smaller allocation residuals remain: the
 * reference splits the slot-53 address into symbol plus 212 where this
 * folds it into one pool word; it keeps the pooled 32-bit 0xfffffc00 mask
 * where this narrows to movs #252 / lsls #8; and it writes the three DMA
 * registers with one "stmia r3!, {r0, r1, r2}" where three plain stores
 * come out here.  The block structure, both jump tables, every call and
 * every store are in place, so the residual is allocation, not topology.
 *
 * Negative results, so they are not retried: spelling the DMA write as an
 * aggregate assignment to a struct at 0x040000d4 (frame grows to 72 bytes,
 * markedly worse), the while-form insertion sort, giving the scratch array
 * block scope inside the redraw, and writing the tile mask as ~0x3ff.
 */

#define Animation_ComposeObjectFrame Func_0800aa0c
#define Animation_SetWorkEntry Func_0800b9f4

/* One scripted layer of the composite object. */
struct AnimationEntry {
    s16 anim_id;    /* 0x00 */
    u16 timer;      /* 0x02 compared as s16, updated as u16 */
    u8 kind;        /* 0x04 direction-table selector */
    u8 param;       /* 0x05 draw parameter */
    u8 priority;    /* 0x06 draw order, 0..3 */
    u8 mode;        /* 0x07 1 = copy, 3 = decode, else draw */
    void **frames;  /* 0x08 */
    void *field_0c;
    u8 *script;     /* 0x10 */
    u8 pos;         /* 0x14 script cursor */
    u8 step;        /* 0x15 timer decrement */
    u8 frame;       /* 0x16 selected frame, 255 = none */
    u8 frame_base;  /* 0x17 */
};

struct AnimationObject {
    u8 field_00[8];
    u16 tile;       /* 0x08 low ten bits are the VRAM tile index */
    u8 field_0a[18];
    u8 slot;        /* 0x1c */
    u8 field_1d[3];
    u8 width;       /* 0x20 */
    u8 height;      /* 0x21 */
    u8 field_22[2];
    u8 last_no;     /* 0x24 */
    u8 dirty;       /* 0x25 */
    u8 flags;       /* 0x26 bit 1 requests the outline pass */
    u8 count;       /* 0x27 */
    struct AnimationEntry *entries[4]; /* 0x28 */
};

/* Running byte total plus the two outline colours. */
struct ComposeContext {
    u16 used;       /* 0x00 */
    u8 field_02[4];
    u8 edge;        /* 0x06 */
    u8 fill;        /* 0x07 */
};

typedef void (*ClearFn)(void *dst, u32 len);
typedef void (*DrawFn)(const void *src, void *dst, s32 param);
typedef void *(*SelectFn)(const void *src, void *dst);
typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

extern u8 Data_000002c4[];
extern const u8 Data_08009bb8[];
extern const u8 Data_08009d9c[];

extern const u8 Data_0801307c[];
extern const u8 Data_0801308c[];
extern const u8 Data_08013094[];
extern const u8 Data_0801309c[];
extern const u8 Data_080130ac[];
extern const u8 Data_080130bc[];
extern const u8 Data_080130c4[];
extern const u8 Data_080130cc[];
extern const u8 Data_0801310c[];

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
u32 Runtime_BumpAllocate(s32 size);
void Func_08002df0(void *allocation);
void Runtime_ReleaseHeapBlock(s32 id);
u32 Func_08005340(const void *source, void *destination);
u8 *Resource_DecompressLz(const u8 *source, u8 *destination);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Animation_SetWorkEntry(void *work, s32 no);

s32 Animation_ComposeObjectFrame(struct AnimationObject *obj, s16 dir)
{
    struct AnimationEntry *e;
    struct ComposeContext *ctx;
    struct DmaChannel *dma;
    void *block;
    u8 *script;
    u8 *buf;
    u8 *mask;
    u8 *src;
    u8 *dst;
    u8 *tmp;
    void *decoded;
    DrawFn draw;
    UploadFn upload;
    u16 order[4];
    s32 held;
    s32 changed;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    u32 key;
    s32 op;
    s32 arg;
    s32 base;
    u32 attr;
    s32 tile;
    u32 size;
    u32 w;
    u32 h;
    u32 x;
    u32 y;
    u8 edge;
    u8 fill;

    changed = 0;
    ctx = *(struct ComposeContext **)Data_03001e68_a;
    held = 1;
    draw = *(DrawFn *)&Data_03001e68_a[184];
    if (draw == 0) {
        block = (void *)Runtime_AllocateHeapBlock(52, (s32)Data_000002c4);
        dma = (struct DmaChannel *)0x040000d4;
        dma->source = Data_08009bb8;
        dma->destination = block;
        dma->control =
            (((u32)Data_08009d9c - (u32)Data_08009bb8) >> 2) | 0x84000000;
        draw = *(DrawFn *)&Data_03001e68_a[184];
        held = 0;
    }

    for (i = 0; i < obj->count; i++) {
        e = obj->entries[i];
        if (e == 0) {
            continue;
        }
        if (e->script == 0) {
            continue;
        }
        for (;;) {
            if ((s16)e->timer > 0) {
                break;
            }
            script = e->script;
            op = script[e->pos++];
            arg = script[e->pos++];
            switch (op) {
            case 254:
                Animation_SetWorkEntry(e, arg);
                obj->last_no = (u8)arg;
                break;
            case 253:
                e->pos = (u8)arg;
                break;
            case 245:
                e->timer += arg << 4;
                break;
            case 241:
                e->pos -= 2;
                goto framed;
            case 240:
                e->kind = (u8)arg;
                break;
            case 255:
                e->frame_base = 255;
                e->timer += arg << 4;
                goto framed;
            case 239:
                e->frame_base = 255;
                e->script = 0;
                obj->count--;
                goto framed;
            case 242:
            case 243:
            case 244:
            case 246:
            case 247:
            case 248:
            case 249:
            case 250:
            case 252:
                break;
            default:
                e->frame_base = (u8)op;
                e->timer += arg << 4;
                goto framed;
            }
        }
        e->timer = e->timer - e->step;
    framed:
        base = e->frame_base;
        switch (e->kind) {
        case 1:
            attr = Data_0801307c[(u16)dir >> 13];
            break;
        case 2:
        case 20:
            attr = Data_08013094[(u16)dir >> 13];
            break;
        case 22:
            attr = Data_0801308c[(u16)dir >> 13];
            break;
        case 3:
            attr = Data_0801309c[(u16)dir >> 12];
            break;
        case 4:
            attr = Data_080130cc[(u16)dir >> 10];
            break;
        case 5:
            attr = Data_080130ac[(u16)dir >> 12];
            break;
        case 6:
            attr = Data_0801310c[(u16)dir >> 10];
            break;
        case 8:
            attr = Data_080130bc[(u16)(dir + 0x1000) >> 13];
            break;
        case 88:
            attr = Data_080130c4[(u16)(dir + 0x1000) >> 13];
            break;
        default:
            attr = 0;
            break;
        }
        base += attr & 7;
        if (i == 0 && (attr >> 7) != 0) {
            changed = 1;
        }
        if (e->frame != base) {
            e->frame = (u8)base;
            obj->dirty = 1;
        }
    }

    if (obj->dirty != 0) {
        size = obj->width * obj->height;
        buf = (u8 *)Runtime_BumpAllocate(size);
        ((ClearFn)0x03000164)(buf, size);

        /* Insertion sort of (priority, index) keys, low key drawn first. */
        n = -1;
        for (j = obj->count - 1; j >= 0; j--) {
            e = obj->entries[j];
            if (e == 0) {
                continue;
            }
            if (e->frames == 0) {
                continue;
            }
            if (e->frame == 255) {
                continue;
            }
            if (e->priority > 3) {
                continue;
            }
            key = (e->priority << 8) | j;
            for (k = n; k >= 0; k--) {
                if (order[k] <= key) {
                    break;
                }
                order[k + 1] = order[k];
            }
            order[k + 1] = (u16)key;
            n++;
        }
        n++;

        for (i = 0; i < n; i++) {
            e = obj->entries[(u8)order[i]];
            if (e->mode == 1) {
                Func_08005340(e->frames[e->frame], buf);
            } else if (e->mode == 3) {
                if (e->param != 0) {
                    tmp = (u8 *)Runtime_BumpAllocate(0x400);
                    draw(Resource_DecompressLz(e->frames[e->frame], tmp),
                         buf, e->param);
                    Func_08002df0(tmp);
                } else {
                    decoded = ((SelectFn)0x030005c0)(e->frames[e->frame], buf);
                    if (decoded != 0) {
                        draw(decoded, buf, 0);
                    }
                }
            } else {
                draw(e->frames[e->frame], buf, e->param);
            }
        }

        if ((obj->flags & 2) != 0) {
            /* Erode the composed silhouette; interior keeps the fill
               colour, everything else that is set becomes the edge. */
            mask = (u8 *)Runtime_BumpAllocate(size);
            w = obj->width;
            h = obj->height;
            edge = ctx->edge;
            fill = ctx->fill;
            ((ClearFn)0x03000164)(mask, size);
            src = buf + w + 1;
            dst = mask + w + 1;
            for (y = 1; y < h - 1; y++) {
                for (x = 1; x < w - 1; x++) {
                    if (src[-1] != 0 && src[1] != 0 &&
                        *(src - w) != 0 && src[w] != 0) {
                        *dst = 1;
                    }
                    dst++;
                    src++;
                }
                dst += 2;
                src += 2;
            }
            src = buf;
            dst = mask;
            for (x = 0; x < size; x++) {
                if (*dst != 0) {
                    *src = fill;
                } else if (*src != 0) {
                    *src = edge;
                }
                dst++;
                src++;
            }
            Func_08002df0(mask);
        }

        tile = VramBlock_LoadCached(obj->slot, size, 0);
        upload = ((UploadFn *)Data_03001e50_a)[53];
        upload(buf, obj->width, obj->height,
               (void *)(0x06010000 + (tile << 5)));
        obj->tile = (u16)((tile & 0x3ff) | (obj->tile & 0xfffffc00));
        obj->dirty = 0;
        ctx->used += size;
        Func_08002df0(buf);
    }

    if (held == 0) {
        Runtime_ReleaseHeapBlock(52);
    }
    return changed;
}
