#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080d4ce8, joined with its tail
 * fragment at 0x080d5094 (Fragment_080d5094): the epilogue there mirrors
 * this owner's r8-r11 prologue exactly, and its internal branches
 * (sub_080d4e4c, sub_080d4ece, sub_080d4fe0) land inside this owner's own
 * byte range with live register state that only a single physical frame
 * explains.  See games/gs1/recon/en/main/080d5094.json for the full
 * boundary evidence.
 *
 * Literal-pool constants for the Fragment_080d5094 half were resolved by
 * assembling games/gs1/asm/080d5094.s standalone with arm-none-eabi-as at
 * its real load address and reading the pool with objdump -d
 * --disassemble-zeroes (0x02010000 particle table, 0x00007828/0x00007824
 * work-struct offsets, 0x080ee29a / 0x080ee2a9 byte tables, 0x080ede48
 * halfword table, 0x080cd261 teardown callback).  Region_080d4ce8's own
 * literal pool sits in the byte gap between the two retained-assembly
 * files (0x080d5054-0x080d5093) and is not present in either .s file, so
 * those constants (the kind==2 palette id, the two Func_080041d8 callback
 * addresses, and a handful of small byte-table addresses) are still
 * placeholders.
 *
 * This is a second instance of the rectangle-blit / orbiting-sprite
 * template also implemented by Func_080ce85c
 * (games/gs1/src/battle/effects/member_orbit/run.c) and by Func_080d9fc8
 * (games/gs1/src/battle/effects/puff_arc/run.c) for the Func_080e0524
 * setup-call shape and the 28-byte, work+0x7080-based per-member record.
 * This owner differs from both templates by: a fixed 96-frame run, a
 * kind-gated palette copy, two Func_080e0524 setup calls, a fixed 4-member
 * (not caller-supplied count) per-frame draw list gated by a byte-table
 * bound, and a 1024-entry *separate* global particle table (28-byte
 * records; fields at byte offsets 0/4/8/16 are fixed-point
 * position/velocity, offset 24 is a countdown) that is cleared once up
 * front, spawned into a handful of entries at a time as members reach
 * their draw window, and decayed/conditionally drawn (via Func_080072f4)
 * for all 1024 slots every frame.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

extern u8 Data_03001eec[];   /* heap-block cache window: [0]=work,[1]=canvas,[2]=extra */
extern void *Data_03001e50[]; /* heap-allocation cache: Data_03001e50[kind] holds kind's
                                  block address (see games/gs1/recon/en/main/080cc5d8.c and
                                  080e7404.c) -- rectangle46/47 and the per-frame screen
                                  pointer are read from *this* array, not from the
                                  Data_03001eec window, per the real ldr [pc,#N] targets
                                  read out of the true gap literal pool at
                                  0x080d5054-0x080d5093 (offsets 46, 47, and 12 -- the last
                                  is 0x03001e80, (0x03001e80-0x03001e50)/4 = 12). */
extern u8 Value_00000085;
extern u8 Value_00000073;
extern u8 Value_00000086;
extern u8 Value_00000087;    /* kind==2 palette id: confirmed via ROM gap-pool read (attempt_7) */
extern u8 Data_080ee2a9[];   /* per-cell frame-window byte table used by the draw do-while
                                 loop (real pool value 0x080ee2a9, offset local 0x1cc) */
extern u8 Data_080ee29a[];   /* active-member-count byte table, indexed by efx->layers */
extern s16 Data_080ede48[];  /* signed halfword lookup used by the draw-position curve */
extern s8 Data_080ee29d[];   /* per-anchor signed-byte bias table, indexed by
                                 efx->layers*4 + i (real pool value 0x080ee29d, offset
                                 local 0x104; ldrsb -- signed, not the unsigned
                                 Data_080ede9f this draft previously guessed) */

/* 1024-entry, 28-byte particle table.  See file header: resolved via
   arm-none-eabi-as + objdump against games/gs1/asm/080d5094.s. */
typedef struct Particle {
    s32 x;
    s32 y;
    s32 vx;
    s32 unk0C;
    s32 vy;
    s32 unk14;
    s32 life;
} Particle;

extern Particle Data_02010000[1024];

/* work + 0x7080: four fixed anchor records, same 28-byte stride as the
   global particle table (matches games/gs1/src/battle/effects/puff_arc's
   work + 0x7080 Puff array). */
typedef struct Anchor {
    s32 x;
    s32 y;
    s32 bias;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 tick;
} Anchor;

typedef struct Efx {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk0C;
    s32 unk10;
    s32 cnt;
    s32 layers;
    s32 unk1C;
    s32 unk20;
    s16 actors[8];
} Efx;

#define WORK_EFX (*(Efx **)(work + 0x7828))

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 id);
s32 Func_080072f0(s32 dest, s32 src, s32 count, s32 routine);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080041d8(s32 callback, s32 interval);
void Func_08004278(s32 callback);
void Func_080049ac(void);
void Func_080051d8(void *a, void *b);
void **Func_080b5098(s32 member_id);
void Func_080e3944(void *source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022fc(s32 angle, s32 radius);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Func_08004458(void);
void Func_080b50e8(s32 id);
void Func_080b5088(s32 slot, s32 flag);
void Func_080f9010(s32 id);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080072f4(void *dest, void *work, s32 sprite, s32 arg1, s32 x,
    s32 y, s32 arg2, s32 w, s32 h);

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080d4ce8(void *object)
{
    u32 *heap;
    u8 *work;
    void *canvas;
    void *extra;
    void *palette;
    s32 status;
    void *rectangle[2];
    void **rectangle_slot;
    void *second_rectangle;
    s32 frame;
    s32 i;
    Efx *efx;
    void *member_object;
    Anchor *anchor;
    s32 spawn_index;
    s32 rng;

    /* draw-list state for the current frame */
    s32 slot;
    u8 *cell;
    void *cursor;
    void *cursor2;
    s32 screen[3];
    s32 flag;
    s32 member_index;
    s32 scan_index;
    Particle *particle;

    heap = (u32 *)Data_03001eec;
    work = (u8 *)heap[0];
    canvas = (void *)heap[1];
    extra = (void *)heap[2];
    WORK_EFX = (Efx *)object;
    Func_080cd594(1);

    M2C_FIELD((void *)0x04000052, u16 *, 0) = 0x73;
    Func_080e0524((s32)&Value_00000085, work, 1, 1);
    Func_080e0524((s32)&Value_00000073, extra, 0, 0);

    efx = WORK_EFX;
    if (efx->layers == 0) {
        palette = Func_08002f40((s32)&Value_00000086);
        status = Func_080072f0(0x05000000, (s32)palette, 128, 0x03001388);
    } else if (efx->layers == 2) {
        palette = Func_08002f40((s32)&Value_00000087);
        status = Func_080072f0(0x05000000, (s32)palette, 128, 0x03001388);
    }
    (void)status;

    status = Func_080ed408(46, 7, 7, 3, 2);
    rectangle[0] = Data_03001e50[46];
    status = Func_080ed408(47, 7, 7, 7, 2);
    second_rectangle = Data_03001e50[47];
    rectangle_slot = rectangle;
    rectangle_slot[1] = second_rectangle;

    for (spawn_index = 0; spawn_index != 1024; spawn_index++) {
        Data_02010000[spawn_index].x = 0;
    }

    member_object = *Func_080b5098(efx->actors[0]);
    anchor = (Anchor *)(work + 0x7080);
    for (i = 0; i != 4; i++) {
        rng = Func_08004458();
        anchor->x = (s32)((rng & 15) + 72) << 16;
        anchor->unk0C = 0;
        anchor->bias =
            (s32)Data_080ee29d[(efx->layers << 2) + i] << 16;
        if (M2C_FIELD(member_object, s32 *, 8) < 0) {
            anchor->x = -anchor->x;
        }
        anchor++;
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((s32)0x080cd261, 0x480);

    member_index = 0;
    scan_index = 0;

    for (frame = 0; frame != 96; frame++) {
        efx = WORK_EFX;
        /* Re-read every frame from a *different* heap-cache slot than the
           startup canvas read: real target address is 0x03001e80, which is
           Data_03001e50[12] ((0x03001e80-0x03001e50)/4), not Data_03001eec[1].
           Confirmed by mapping the real ldr [pc,#N] target at asm local
           offset 0x164 (loop re-entry point sub_080d4e4c) into the ROM gap
           pool word list from attempt_7. */
        canvas = Data_03001e50[12];

        if (efx->layers == 2 && frame <= 63) {
            if (efx->unk0C != 0) {
                M2C_FIELD(canvas, u16 *, 54) += 192;
            } else {
                M2C_FIELD(canvas, u16 *, 54) -= 192;
            }
        }

        if (frame == 16) {
            Func_080b50e8(134);
        }
        Func_080049ac();
        Func_080051d8(canvas, (u8 *)canvas + 12);

        if (frame <= 63) {
            if (Data_080ee29a[efx->layers] != 0) {
                flag = frame & 1;
                cursor = work + 0x7080;
                cursor2 = work + 0x6e80;
                cell = &Data_080ee2a9[0];
                slot = 0;

                do {
                    Func_080e3944(cursor, screen);
                    screen[0] = screen[0] / 2;
                    screen[1] -= 8;

                    if (frame == cell[0]) {
                        Func_080f9010(145);
                    }
                    if (frame >= cell[0] + 4) {
                        s32 index;
                        s32 wide;
                        s32 spin;

                        index = (slot * 25) + (frame << 4);
                        spin = Func_080022fc(index, 104);
                        wide = spin;
                        (void)wide;

                        Func_080072f4(canvas, work, screen[0] - 17,
                            screen[1] - 104, 34, 0, 34, 0, 0);
                        Func_080072f4(canvas, work, screen[0] - 17,
                            screen[1] - 104 + wide, 34, 0, 34, 0, 0);

                        if (flag != 0) {
                            Func_080072f4(canvas, work, screen[0] - 20,
                                screen[1] - 24, 20, 37, 20, 37, 0);
                            Func_080072f4(canvas, work, screen[0] - 20,
                                screen[1] - 24, 20, 37, 20, 37, 0);
                        } else {
                            Func_080072f4(canvas, work, screen[0] - 20,
                                screen[1] - 24, 20, 37, 20, 37, 0);
                            Func_080072f4(canvas, work, screen[0] - 20,
                                screen[1] - 24, 20, 37, 20, 37, 0);
                        }
                    }

                    if (frame == cell[0] || frame < cell[0] + 16) {
                        /* continue to next anchor slot */
                    }

                    slot++;
                    cursor = (u8 *)cursor + 28;
                    cell++;
                } while (slot != Data_080ee29a[efx->layers]);

                /* Scan the 1024-entry particle table for a free (life==0)
                   slot and spawn one particle per completed member, using
                   the last-computed screen[] position as the spawn origin
                   and a random angle for the initial velocity. */
                if (member_index != 4) {
                    Particle *free_slot;
                    s32 spawn_mask;
                    s32 spawn_angle;
                    s32 magnitude;
                    s32 svx;
                    s32 svy;

                    free_slot = &Data_02010000[scan_index];
                    while (free_slot->life != 0) {
                        scan_index++;
                        free_slot++;
                        if (scan_index == 1024) {
                            break;
                        }
                    }
                    if (scan_index != 1024) {
                        /* Real masks read from the ROM gap literal pool
                           (attempt_7's Value_000003ff / Value_00007fff /
                           Value_ffffc000, mapped to asm locals 0x302/0x312/
                           0x316): magnitude is rand()&0x3ff (not a byte-table
                           mask), and the angle argument is rand()&0x7fff
                           biased by 0xffffc000, i.e. rand()&0x7fff - 0x4000.
                           The +32 magnitude bump happens *after* the cosine
                           call, matching the real instruction order (cos is
                           called on the un-biased magnitude register). */
                        rng = Func_08004458();
                        spawn_mask = rng & 0x3ff;
                        rng = Func_08004458();
                        free_slot->x = screen[0] << 8;
                        magnitude = spawn_mask;
                        free_slot->y = (screen[1] << 8) + 4096;
                        spawn_angle = (rng & 0x7fff) + (s32)0xffffc000;
                        svx = Func_08002322(spawn_angle);
                        magnitude += 32;
                        free_slot->vx = (magnitude * svx) >> 15;
                        svy = Func_0800231c(spawn_angle);
                        free_slot->vy = -((magnitude * svy) << 1) >> 15;
                        member_index++;
                        if (member_index == 200) {
                            scan_index = 1024;
                        }
                    }
                }
            }
        }

        particle = Data_02010000;
        for (i = 0; i != 1024; i++) {
            if (particle->life > 0) {
                s32 new_x;
                s32 new_y;
                s32 decayed_vx;
                s32 decayed_vy;
                s32 px;
                s32 py;

                particle->life--;
                new_x = particle->x + particle->vx;
                new_y = particle->y + particle->vy;
                particle->x = new_x;
                particle->y = new_y;

                decayed_vx = (particle->vx * 15) << 2;
                if (decayed_vx < 0) {
                    decayed_vx += 63;
                }
                particle->vx = decayed_vx >> 6;

                decayed_vy = (particle->vy * 15) << 2;
                if (decayed_vy < 0) {
                    decayed_vy += 63;
                }
                particle->vy = (decayed_vy >> 6) - 16;

                py = new_y;
                if (py < 0) {
                    py += 255;
                }
                py >>= 8;

                if (py <= 120 && new_x >= 0) {
                    px = new_x >> 8;
                    if (px <= 126 && new_y >= 0) {
                        s32 size;
                        s32 size2;
                        s32 draw_x;
                        s32 draw_y;
                        void *sprite;

                        size = particle->life - 17;
                        if (size < 0) {
                            size += 7;
                        }
                        size >>= 3;
                        if (size <= 0) {
                            size = 1;
                        }
                        size2 = size << 1;
                        draw_x = px - size;
                        draw_y = py - ((size + (size < 0)) >> 1) +
                            (s32)extra;
                        sprite = rectangle_slot[(px & 1) << 2];
                        Func_080072f4(canvas, work, draw_x, draw_y,
                            size, size2, (s32)sprite, 0, 0);
                    }
                }
            }
            particle++;
        }
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((s32)0x080cd261);
    Func_080cdbc0();
}
