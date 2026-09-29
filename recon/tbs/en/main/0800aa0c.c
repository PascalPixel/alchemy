/* Draft whole owner [0800aa0c,0800b074), 1640 bytes including two switch
   tables and three own pool islands. The following sprite placement owner
   is now split and exact; it is not part of this experiment.
   2026-09-26 baseline: 1648 bytes, 523 differing halfwords, 338 aligned
   edits, 60-byte frame versus reference 56. Complete normalized diff read.
   Exact caller b388 supplies the sprite-family object and a direction;
   exact callee Animation_SetWorkEntry confirms script/cursor/timer fields
   and its void prototype. Direct and call-via sites inspected in the owner.
   H1: command parsing is a reload/test loop, not a duplicated while test.
   Carry the opcode/frame local through terminal commands into direction
   selection; only timer-expiry and hold paths reload frame_base. Predict
   one timer test and no frame reload for commands 239/255/default. This
   targets the extra back-edge before any allocation/stack spelling work.
   One bounded trial before the 23:55 checkpoint; exact 1640 bytes plus
   compare/coverage/verify required for adoption. Record full-diff result
   here; no second trial this checkpoint.
   H1 result: 1644/1640 bytes, 561 differing halfwords / 379 aligned edits;
   frame still 60/56. All 17 command-table destinations now match, the
   duplicated timer test is gone, and the terminal frame value reaches r0
   without the old reload. This local structural repair is retained despite
   the worse whole-owner alignment score; no whole-CFG equivalence claim.
   Remaining coherent defects: direction kinds 8/88 add before narrowing
   rather than after shifting; insertion sort re-reads an already loaded
   halfword, adds a frame slot and has different exit/store ownership;
   upload dispatch rematerializes 03001f24 instead of base 03001e50 + 212.
   Read the complete difference. The next experiment must audit typed sort
   lifetimes or the existing dispatch-family declaration, not permutations. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "video_dma_family.h"

/* Builds and uploads one composite animation frame. */

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
void Sys_Free(void *allocation);
void Runtime_ReleaseHeapBlock(s32 id);
u32 Func_08005340(const void *source, void *destination);
u8 *Resource_DecompressLz(const u8 *source, u8 *destination);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Animation_SetWorkEntry(void *work, s32 no);

s32 Func_0800aa0c(struct AnimationObject *obj, s16 dir)
{
    struct AnimationEntry *e;
    struct ComposeContext *ctx;
    void *block;
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
        Dma_Set(Data_08009bb8, block,
                (((u32)Data_08009d9c - (u32)Data_08009bb8) >> 2) | 0x84000000,
                (volatile u32 *)0x040000d4);
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
        /* FAKEMATCH: explicit timer-test lifetime keeps command back-edges
           and the decoded frame value separate from the hold reload. */
    read_command:
        if ((s16)e->timer > 0)
            goto advance_timer;
        base = e->script[e->pos++];
        arg = e->script[e->pos++];
        switch (base) {
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
            base = 255;
            goto select_direction;
        case 239:
            e->frame_base = 255;
            e->script = 0;
            obj->count--;
            base = 255;
            goto select_direction;
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
            e->frame_base = (u8)base;
            e->timer += arg << 4;
            goto select_direction;
        }
        goto read_command;
    advance_timer:
        e->timer = e->timer - e->step;
    framed:
        base = e->frame_base;
    select_direction:
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
                    Sys_Free(tmp);
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
            Sys_Free(mask);
        }

        tile = VramBlock_LoadCached(obj->slot, size, 0);
        upload = ((UploadFn *)Data_03001e50_a)[53];
        upload(buf, obj->width, obj->height,
               (void *)(0x06010000 + (tile << 5)));
        obj->tile = (u16)((tile & 0x3ff) | (obj->tile & 0xfffffc00));
        obj->dirty = 0;
        ctx->used += size;
        Sys_Free(buf);
    }

    if (held == 0) {
        Runtime_ReleaseHeapBlock(52);
    }
    return changed;
}
