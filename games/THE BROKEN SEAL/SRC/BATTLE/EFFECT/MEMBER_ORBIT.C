#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"

#define BattleFx_RunMemberOrbit Func_080ce85c

/*
 * Battle-presentation sub-effect: entry 34 of the effect callback table at
 * 0x080ee2b4.  The single argument is the effect state pointer, which the
 * owner republishes at work + 0x7828.
 */
#define FIELD_AT_OFFSET(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

/* A small absolute link-time constant.  The resource id must be built from a
 * literal pool word, which an ordinary integer literal cannot produce. */
extern u8 Value_000000af;

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
#define Resource_GetTableEntry Func_08002f40
u32 Func_08005340(const void *source, void *destination);
void Func_080049ac(void);
#define Render_ResetTransformState Func_080049ac
void Func_080051d8(s32 a, s32 b);
#define Graphics_PrepareTransferInIwramWork Func_080051d8
void **Func_080b5098(s32 member_id);
void Func_080e3944(void *source, void *screen);
#define EffectPosition_ApplyBaseAndYOffset Func_080e3944
s32 Func_08002322(s32 angle);
#define Engine_MathSin Func_08002322
s32 Func_0800231c(s32 angle);
#define Engine_MathCos Func_0800231c
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
#define ObjectGroup_UpdateMembers Func_080d6888
void Func_080030f8(s32 frames);
#define WaitFrames Func_080030f8
void Func_08002dd8(s32 id);
#define Runtime_ReleaseHeapBlock Func_08002dd8
s32 Func_080cdbc0(void);

/*
 * Sets the BG2 affine scale, loads the palette and the 32x32 sprite frames
 * into the work block, prepares the two rectangle-blit routines, and runs
 * member_count * 16 + 48 frames of a sine-swept scanline table with four
 * sprites orbiting each member whose window is open.  Binding each callee's
 * result to `status` is load-bearing: it makes the call a set of r0 and so
 * fixes the order of the following argument setup.
 */
void BattleFx_RunMemberOrbit(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rectangle[2];
    s32 record[3];
    s32 screen[3];
    s32 member;
    s32 y_offset;
    void **rectangle_slot;
    void *rect2;
    s32 *record_slot;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    FIELD_AT_OFFSET(work, void **, 0x7828) = object;
    Func_080cd594(0);
    FIELD_AT_OFFSET((void *)0x04000020, s16 *, 0) = 0x100;
    palette = Resource_GetTableEntry((s32)&Value_000000af);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, work);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    rectangle[0] = heap_cache[7];
    status = BattleEffect_LoadWork(47, 7, 7, 15, 2);
    rect2 = heap_cache[8];
    rectangle_slot = rectangle;
    rectangle_slot[1] = rect2;
    Scheduler_AddOrUpdateCallback((void *)0x080DBB9D, 0x480);
    FIELD_AT_OFFSET(work, s32 *, 0x7780) = 2;
    FIELD_AT_OFFSET(work, s32 *, 0x7784) = 50;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);
    if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 1) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = -0x6800;
        y_offset = -112;
    } else {
        y_offset = 0;
    }
    for (frame = 0;
            frame != (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)
                * 16) + 48;
            frame++) {
        s32 facing;
        s32 *scanline;
        s32 i;
        s32 id_ofs;

        facing = *(s32 *)0x03001E80;
        scanline = (s32 *)((u8 *)work + 0x6980);
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 0) {
            s32 angle;
            s32 ceiling;

            for (i = 0, ceiling = 0x80000, angle = frame << 10;
                    i != 160; i++) {
                *scanline++ = (ceiling - (Engine_MathSin(angle) << 3)) >> 10;
                angle += 1024;
            }
        } else {
            s32 angle;

            for (i = 0, angle = frame << 10; i != 160; i++) {
                *scanline++ = ((Engine_MathSin(angle) << 3) >> 10) - 0x7000;
                angle += 1024;
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        member = 0;
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20) != 0) {
            record_slot = record;
            id_ofs = 36;
            while (member
                != FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)) {
                void *member_object;

                member_object = *Func_080b5098(
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s16 *,
                        id_ofs));
                if (frame > member * 16 && frame < (member * 16) + 60) {
                    s32 spin;

                    if (frame == (member * 16) + 32) {
                        ObjectGroup_UpdateMembers(
                            FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                                s16 *, id_ofs),
                            0, 5, -1, 0);
                    }
                    record_slot[0] = FIELD_AT_OFFSET(member_object, s32 *, 8);
                    record_slot[1] = 0x280000;
                    record_slot[2] = FIELD_AT_OFFSET(member_object, s32 *, 16);
                    EffectPosition_ApplyBaseAndYOffset(record_slot, screen);
                    for (i = 0; i != 4; i++) {
                        s32 x;
                        s32 y;
                        s32 slot;

                        spin = (frame << 9) + (i << 14);
                        x = (screen[0] + ((Engine_MathSin(spin) << 4) >> 16))
                            + y_offset;
                        y = screen[1] + ((Engine_MathCos(spin) << 4) >> 16);
                        slot = frame / 16;
                        ((DrawRectangleFn)rectangle_slot[slot & 1])(
                            canvas,
                            (u8 *)work + (((frame / 4) - (slot * 4)) << 10),
                            x - 16, y - 16, 32, 32);
                    }
                }
                id_ofs += 2;
                member++;
            }
        }
        FIELD_AT_OFFSET(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((void *)0x080CD261);
    Scheduler_RemoveCallback((void *)0x080DBB9D);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Func_080cdbc0();
}
