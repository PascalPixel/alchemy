#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect keyed off the shared 0x03001eec "battle
 * work" pointer, in the same family as games/THE BROKEN SEAL/src/battle/effects/
 * member_orbit/run.c (owner 080ce85c, adopted, family score 8074/10000).
 * This owner is 1068 bytes -- structurally larger than the 724-byte
 * template -- and diverges from it in several concrete ways confirmed by
 * reading recon/tbs/raw/080d3854.s and its objdump-resolved literal pools:
 *
 *   - BattleFx_BeginCanvasLayer is called with mode 1, not 0.
 *   - The palette resource id is loaded from a literal pool word 0xCE
 *     (Value_000000ce), not 0xAF, following the same absolute-link-time-
 *     constant idiom already established for Value_000000af.
 *   - The second BattleEffect_LoadWork call's third size argument is 7, not 15.
 *   - Only ONE finish-callback is ever registered (Scheduler_AddOrUpdateCallback with
 *     0x080CD261): the template's separate 0x080DBB9D registration does not
 *     happen here at all.
 *   - Instead of the template's 160-entry per-scanline BG2 affine table,
 *     this owner keeps a single per-frame "wave" scalar and drives GBA
 *     alpha blending directly: BLDALPHA (0x04000052) is set to 0x1010 once
 *     up front, then faded in/out at 0x04000052 during frames 0-16 and
 *     64-79.
 *   - It seeds a 16-entry, 28-byte-stride particle table at work+0x7080
 *     with randomized fields (via Random16()) before the main loop,
 *     something the template does not do at all.
 *   - Every 8th frame in {16,24,...,64} it clears a canvas region through
 *     an indirect call to a small runtime routine at 0x03000168 (called via
 *     the r6 slot of the __call_via_rN veneer bank at 0x080072e4, per
 *     recon/tbs/raw/080072e4.s), passing a fixed size and an 0x08080808
 *     fill pattern.
 *   - When the object's field 24 selects a non-zero row of the 3-byte-per-
 *     entry table at 0x080ee1ca, it draws three fixed 40x32 sprites through
 *     rectangle[0] and then walks the first four entries of the work+0x7080
 *     particle table, drawing each active one (field 24 >= 0) through
 *     rectangle_slot[slot & 1] with its width/height/offset looked up from the
 *     byte/halfword tables at 0x080edeca/0x080edebe/0x080eded0, then
 *     advancing (and periodically resetting) that entry's own field 24.
 *   - The per-member event loop checks frame against seven thresholds
 *     spaced 8 frames apart per member (member_offset+16, +24, ..., +64,
 *     with member_offset advancing by 3 per member) rather than the
 *     template's single frame==member*16+32 check, and calls
 *     ObjectGroup_UpdateMembers(member_id,7,5,member,4) followed by
 *     BattleMotion_ApplyVariantMotionFar(member_id,6) -- a callee the template never uses.
 *
 * The `status` bindings on the early calls are load-bearing for the same
 * reason documented in the template: they give the following BattleEffect_LoadWork
 * argument setup an output dependency on r0, which is what places the
 * reference's `movs r0, #46` / `movs r0, #47` after r1-r3 in each call.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*FillFn)(void *dest, s32 size, u32 pattern);


void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void WaitFrames(s32 frames);
u32 Random16(void);
void Audio_PlayCue(s32 id);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEventRuntime_BeginPhaseFar(s32 id);
s32 __modsi3(s32 a, s32 b);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 mode);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

void Unnamed_080d3854(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rectangle[2];
    void **rectangle_slot;
    s32 wave;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    BattleFx_BeginCanvasLayer(1);
    M2C_FIELD((void *)0x04000052, s16 *, 0) = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Resource_DecodeType01((u8 *)palette + 128, work);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    rectangle[0] = heap_cache[7];
    status = BattleEffect_LoadWork(47, 7, 7, 7, 2);
    rectangle[1] = heap_cache[8];
    rectangle_slot = rectangle;
    frame = 0;

    {
        void *entry;
        s32 i;

        i = 0;
        entry = (u8 *)work + 0x7080;
        do {
            M2C_FIELD(entry, s32 *, 0) = Random16() & 31;
            M2C_FIELD(entry, s32 *, 4) = (Random16() & 63) + 16;
            i++;
            M2C_FIELD(entry, s32 *, 24) = -(Random16() & 15);
            entry = (u8 *)entry + 28;
        } while (i != 16);
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);
    M2C_FIELD((void *)0x04000052, s16 *, 0) = 0x1000;
    WaitFrames(1);
    Audio_PlayCue(141);

    while (frame != 80) {
        s32 x_delta;
        s32 y_pos;

        wave = Trig_Sin(frame << 10) << 4;
        if (frame == 32) {
            BattleEventRuntime_BeginPhaseFar(133);
        }

        {
            s32 i;

            for (i = 0; i != 7; i++) {
                if (frame == 16 + (i << 3)) {
                    ((FillFn)0x03000168)(canvas, 0x4000, 0x08080808);
                }
            }
        }

        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
            wave += 0x200000;
        } else {
            wave += 0xffe00000;
        }

        if (frame <= 16) {
            M2C_FIELD((void *)0x04000052, s16 *, 0) = frame | 0x1000;
        }
        if (frame > 63) {
            M2C_FIELD((void *)0x04000052, s16 *, 0) = (0x4f - frame) | 0x1000;
        }

        if (*(u8 *)(0x080ee1ca
                + (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 24)
                    * 3)) != 0) {
            s32 angle;
            void *particle_base;
            s32 count;

            angle = frame << 11;
            particle_base = work;
            count = 0;
            do {
                s32 sin_val;
                s32 cos_val;
                s32 idx;
                s32 layer;
                s32 layer5;
                void *layer_base;
                s32 i;
                void *p;

                idx = M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 24)
                    * 3;
                sin_val = Trig_Sin(angle);
                y_pos = (((*(u8 *)(0x080ee1ca + idx + 1)) * sin_val + wave)
                    >> 16) + 40;
                cos_val = Trig_Cos(angle);
                x_delta = (cos_val << 1) >> 16;

                layer = __modsi3(frame / 2, 3);
                layer5 = layer * 5;
                layer_base = (u8 *)work + layer * 0xA00;
                ((DrawRectangleFn)rectangle[0])(canvas,
                    (u8 *)layer_base + 0xC56, y_pos, x_delta + 0x10, 40, 32);
                ((DrawRectangleFn)rectangle[0])(canvas,
                    (u8 *)work + (layer5 << 8) + 0x2A56, y_pos,
                    x_delta + 0x30, 40, 32);
                ((DrawRectangleFn)rectangle[0])(canvas,
                    (u8 *)layer_base + 0x1156, y_pos, x_delta + 0x50, 40, 32);

                p = (u8 *)particle_base + 0x7080;
                for (i = 0; i != 4; i++) {
                    s32 field24;

                    field24 = M2C_FIELD(p, s32 *, 24);
                    if (field24 >= 0) {
                        s32 table_idx;

                        table_idx = (field24 / 2) + ((i / 2) * 3);
                        ((DrawRectangleFn)rectangle_slot[i & 1])(canvas,
                            (u8 *)work
                                + *(u16 *)(0x080edebe + (table_idx * 2)),
                            M2C_FIELD(p, s32 *, 0) + y_pos,
                            M2C_FIELD(p, s32 *, 4) + x_delta,
                            *(u8 *)(0x080edeca + table_idx),
                            *(u8 *)(0x080eded0 + table_idx));
                    }
                    field24 = M2C_FIELD(p, s32 *, 24) + 1;
                    M2C_FIELD(p, s32 *, 24) = field24;
                    if (field24 == 6) {
                        M2C_FIELD(p, s32 *, 0) = Random16() & 31;
                        M2C_FIELD(p, s32 *, 4) = (Random16() & 63) + 16;
                        M2C_FIELD(p, s32 *, 24) = 0;
                    }
                    p = (u8 *)p + 28;
                }

                particle_base = (u8 *)particle_base + 0x70;
                angle += 0x4000;
                count++;
            } while (count != *(u8 *)(0x080ee1ca
                + (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 24)
                    * 3)));
        }

        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) != 0) {
            s32 member;
            s32 member_offset;
            s32 member_id_offset;

            member = 0;
            member_offset = 0;
            member_id_offset = 36;
            do {
                s32 j;
                s32 threshold;

                j = 0;
                threshold = member_offset + 16;
                do {
                    if (frame == threshold) {
                        ObjectGroup_UpdateMembers(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, member_id_offset),
                            7, 5, member, 4);
                        BattleMotion_ApplyVariantMotionFar(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, member_id_offset),
                            6);
                    }
                    j++;
                    threshold += 8;
                } while (j != 7);
                member++;
                member_offset += 3;
                member_id_offset += 2;
            } while (member
                != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20));
        }

        M2C_FIELD(work, s32 *, 0x77a8) = 1;
        {
            u8 sfx;

            sfx = *(u8 *)(0x080ee1ca
                + (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 24)
                    * 3) + 2);
            Camera_ApplyShake(sfx, sfx * 2);
        }
        ObjectGroup_TickMemberTimers();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
        frame++;
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((void *)0x080CD261);
    BattleFx_EndCanvasLayer();
}
