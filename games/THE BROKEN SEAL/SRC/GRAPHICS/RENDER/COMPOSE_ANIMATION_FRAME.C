#include "TYPES.H"
#include "ANIMSPR.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
extern u8 gMenuCtrlWork[];
extern u8 gWorkSlot[];

/* Steps the scripts of a composite sprite's layers, picks each layer's frame
   for the facing direction and, when a frame changed, draws the layers in
   priority order, outlines the result and uploads it. Returns whether the
   first layer's facing entry asks for a flip. */

/* Running byte total plus the two outline colours. */
struct ComposeContext {
    u16 used;       /* 0x00 */
    u8 field_02[4];
    u8 edge;        /* 0x06 */
    u8 fill;        /* 0x07 */
};

typedef void (*DrawFn)(const void *src, void *dst, s32 param);
typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

extern const u8 Render_DecodeFrame[];
extern const u8 Render_DecodeFrameBuffer[];
extern u8 Render_DecodeFrameCodeSize[];

extern const u8 AnimationFacing_Kind1[];
extern const u8 AnimationFacing_Kind22[];
extern const u8 AnimationFacing_Kind2[];
extern const u8 AnimationFacing_Kind3[];
extern const u8 AnimationFacing_Kind5[];
extern const u8 AnimationFacing_Kind8[];
extern const u8 AnimationFacing_Kind88[];
extern const u8 AnimationFacing_Kind4[];
extern const u8 AnimationFacing_Kind6[];

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
u32 Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *allocation);
void Runtime_ReleaseHeapBlock(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
u8 *Resource_DecompressLz(const u8 *source, u8 *destination);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Animation_SetWorkEntry(void *work, s32 no);

s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir)
{
    s32 changed;
    u32 size;
    struct ComposeContext *ctx;
    DrawFn draw;
    s32 held;
    struct AnimationEntry *e;
    void *block;
    u8 *buf;
    u8 *tmp;
    void *decoded;
    u16 order[4];
    s32 i;
    s32 k;
    s32 n;
    u32 key;
    s32 arg;
    s32 base;
    u32 attr;
    s32 tile;

    changed = 0;
    ctx = *(struct ComposeContext **)gMenuCtrlWork;
    held = 1;
    draw = *(DrawFn *)&gMenuCtrlWork[184];
    if (draw == 0) {
        block = (void *)Runtime_AllocateHeapBlock(52, (s32)Render_DecodeFrameCodeSize);
        Dma_Set(Render_DecodeFrame, block,
                (((u32)Render_DecodeFrameBuffer - (u32)Render_DecodeFrame) >> 2) | 0x84000000,
                (volatile u32 *)0x040000d4);
        draw = *(DrawFn *)&gMenuCtrlWork[184];
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
            base = 255;
            e->timer += arg << 4;
            goto select_direction;
        case 239:
            e->frame_base = 255;
            e->script = 0;
            base = 255;
            obj->count--;
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
            attr = AnimationFacing_Kind1[(u16)dir >> 13];
            break;
        case 2:
        case 20:
            attr = AnimationFacing_Kind2[(u16)dir >> 13];
            break;
        case 22:
            attr = AnimationFacing_Kind22[(u16)dir >> 13];
            break;
        case 3:
            attr = AnimationFacing_Kind3[(u16)dir >> 12];
            break;
        case 4:
            attr = AnimationFacing_Kind4[(u16)dir >> 10];
            break;
        case 5:
            attr = AnimationFacing_Kind5[(u16)dir >> 12];
            break;
        case 6:
            attr = AnimationFacing_Kind6[(u16)dir >> 10];
            break;
        case 8:
            attr = AnimationFacing_Kind8[((u32)(dir << 16) + 0x10000000) >> 29];
            break;
        case 88:
            attr = AnimationFacing_Kind88[((u32)(dir << 16) + 0x10000000) >> 29];
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
        Iwram_ClearWords(buf, size);

        /* Insertion sort of (priority, index) keys, low key drawn first. */
        n = -1;
        for (i = obj->count - 1; i >= 0; i--) {
            e = obj->entries[i];
            if (e == 0) {
                continue;
            }
            if (e->frames == 0) {
                continue;
            }
            if (e->frame == 255) {
                continue;
            }
            key = e->priority;
            if (key > 3) {
                continue;
            }
            key = (key << 8) | i;
            /* FAKEMATCH: the first shift written apart and the scan left by
               goto keep the scan unrotated, with its step at the top. */
            k = n;
            if (k >= 0 && order[k] > key) {
                order[k + 1] = order[k];
                for (;;) {
                    k--;
                    if (k < 0)
                        goto insert;
                    if (order[k] <= key)
                        goto insert;
                    order[k + 1] = order[k];
                }
            }
        insert:
            order[k + 1] = (u16)key;
            n++;
        }
        n++;

        for (i = 0; i < n; i++) {
            e = obj->entries[*(u8 *)&order[i]];
            if (e->mode == 1) {
                Resource_DecodeType01(e->frames[e->frame], buf);
            } else if (e->mode == 3) {
                if (e->param != 0) {
                    tmp = (u8 *)Runtime_BumpAllocate(0x400);
                    draw(Resource_DecompressLz(e->frames[e->frame], tmp),
                         buf, e->param);
                    Runtime_BumpFree(tmp);
                } else {
                    decoded = Iwram_Decompress(e->frames[e->frame], buf);
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
            u8 *mask;
            u8 edge;
            u8 fill;
            u32 w;
            u32 h;
            u8 *src;
            u8 *dst;

            mask = (u8 *)Runtime_BumpAllocate(size);
            w = obj->width;
            h = obj->height;
            edge = ctx->edge;
            fill = ctx->fill;
            Iwram_ClearWords(mask, size);
            src = buf + w + 1;
            dst = mask + w + 1;
            for (i = 1; i < h - 1; i++) {
                for (k = 1; k < w - 1; k++) {
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
            for (i = 0; i < size; i++) {
                if (*dst != 0) {
                    *src = fill;
                } else if (*src != 0) {
                    *src = edge;
                }
                dst++;
                src++;
            }
            Runtime_BumpFree(mask);
        }

        tile = VramBlock_LoadCached(obj->slot, size, 0);
        {
            void *vram = (void *)(0x06010000 + (tile << 5));
            UploadFn *slots = (UploadFn *)gWorkSlot;

            slots[53](buf, obj->width, obj->height, vram);
        }
        obj->part[0].tile = tile;
        obj->dirty = 0;
        ctx->used += size;
        Runtime_BumpFree(buf);
    }

    if (held == 0) {
        Runtime_ReleaseHeapBlock(52);
    }
    return changed;
}
