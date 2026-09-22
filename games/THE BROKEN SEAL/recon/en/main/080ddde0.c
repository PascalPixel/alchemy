#include "TYPES.H"
#include "BATTLE_EFX.H"

/* Staggered member effects with falling particles and randomized sprite
 * bursts. Complete reference span: 080ddde0 through080de2f8, including
 * the continuation and literal pools. Still a reconstruction draft. */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, u32 w, s32 h);
typedef void (*FillWordsFn)(void *dest, s32 size, u32 fill);

void Func_080cd594(s32 mode);
s32 Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e3980(s32 a, void *out2);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_08002304(s32 value, s32 bound);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_080e155c(s32 a, s32 b);
extern const u16 Data_080edebe[];

extern u8 Value_00000073, Value_000000ce, Value_000000c4;
extern const u8 Data_080eebd6[];      /* per-group [4] byte table: gate/count selectors */
extern const u8 Data_080edeca[];      /* per-mask byte table (announce geometry A) */
extern const u8 Data_080eded0[];      /* per-mask byte table (announce geometry B) */
extern const u8 Data_080eebe2[];     /* random draw-mode flags */
extern const u8 Data_080eebe6[];      /* effect-variant draw selector */
extern const u16 Data_080ede48[];     /* per-step halfword table (shared w/ 080d82b0.c) */

/* The particle draw uses the kind-46 callback cached at sp+32. The fill
 * routine is the entry at 0x03000168 itself, not a pointer stored there.
 * Each kind-47 draw uses the freshly allocated callback. */

void Func_080ddde0(void *table_param)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    s32 member_offset;
    s32 member_field_offset;
    void *table;
    void *extra_target;
    DrawRectangleFn draw_cb_46;
    s32 pass;
    s32 member_index;
    s32 local_buf[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = table_param;

    Func_080cd594(1);
    (void) BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_cb_46 = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    Resource_LoadAndDecompress((s32)&Value_000000ce, work, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_000000c4, (u8 *)work + 0xc56, 1, 1);
    Resource_LoadAndDecompress((s32) &Value_00000073, extra_target, 0, 0);

    {
        s32 *slot;
        s32 i;

        slot = (s32 *)0x02010018; /* particle_pool[0].life, stride 28 */
        i = 0;
        do {
            i++;
            *slot = 0;
            slot = (s32 *)((u8 *)slot + 28);
        } while (i != 1024);
    }

    {
        s32 *slot;
        s32 i;

        slot = (s32 *)((u8 *)work + 0x7098);
        i = 0;
        do {
            i++;
            *slot = -1;
            slot = (s32 *)((u8 *)slot + 28);
        } while (i != 64);
    }

    M2C_FIELD(work, s32 *, 0x7784) = 75;

    Func_080041d8((void *)0x080cd261, 144 << 3);
    Func_080f9010(138);

    pass = 0;
    while (pass != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20) * 8 + 40) {
        if (pass == 24)
            Func_080b50e8(133);
        member_index = 0;
        while (member_index != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)) {
            if (pass == member_index * 8)
                ((FillWordsFn)0x03000168)(draw_destination, 0x4000, 0x10101010);
            member_index++;
        }
        member_index = 0;
        member_field_offset = 36;
        member_offset = 0;
        while (member_index != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)) {
            s32 base;
            s32 group;
            s32 j;
            base = member_index * 8;
            table = M2C_FIELD(work, void **, 0x7828);
            Func_080e3980(M2C_FIELD(table, s16 *, member_field_offset), local_buf);
            local_buf[0] /= 2;
            if (pass == base + 1)
                M2C_FIELD(work, s32 *, 0x77a8) = 4;
            if (pass == base + 4) {
                table = M2C_FIELD(work, void **, 0x7828);
                Func_080d6888(M2C_FIELD(table, s16 *, member_field_offset), 7, 5, member_index, 6);
                table = M2C_FIELD(work, void **, 0x7828);
                Func_080b5088(M2C_FIELD(table, s16 *, member_field_offset), 6);
            }
            if (pass >= base && pass < base + 16) {
                s32 height;
                height = (pass - base) << 6;
                if (height > 104) height = 104;
                j = 0;
                table = M2C_FIELD(work, void **, 0x7828);
                group = M2C_FIELD(table, s32 *, 24);
                while (j != Data_080eebd6[group * 4 + 3]) {
                    s32 cell;
                    cell = ((member_index + pass + j) / 2) & 3;
                    draw_cb_46(draw_destination, (u8 *)work + 0xc56 + cell * 2880,
                        local_buf[0] - 12, 0, 24, height);
                    j++;
                    table = M2C_FIELD(work, void **, 0x7828);
                    group = M2C_FIELD(table, s32 *, 24);
                }
                if (pass == base + 2) {
                    s32 *particle;
                    particle = (s32 *)(0x02010000 + member_offset);
                    j = 0;
                    table = M2C_FIELD(work, void **, 0x7828);
                    group = M2C_FIELD(table, s32 *, 24);
                    while (j != Data_080eebd6[group * 4]) {
                        s32 magnitude, angle, value;
                        magnitude = Func_08004458() & 0x1ff;
                        angle = Func_08004458();
                        particle[0] = local_buf[0] << 16;
                        angle = (angle & 0x7fff) - 0x4000;
                        particle[1] = 208 << 15;
                        value = Func_08002322(angle);
                        magnitude += 64;
                        particle[3] = (magnitude * value) >> 5;
                        value = Func_0800231c(angle);
                        particle[4] = -(magnitude * value) >> 6;
                        particle[6] = (Func_08004458() & 7) + 32;
                        j++;
                        particle = (s32 *)((u8 *)particle + 28);
                        table = M2C_FIELD(work, void **, 0x7828);
                        group = M2C_FIELD(table, s32 *, 24);
                    }
                }
            }
            if (pass >= base + 2 && pass < base + 24) {
                j = 0;
                table = M2C_FIELD(work, void **, 0x7828);
                group = M2C_FIELD(table, s32 *, 24);
                while (j != Data_080eebd6[group * 4 + 1]) {
                    s32 mask, range, offset, x, y, random;
                    DrawRectangleFn draw47;
                    mask = j & 3;
                    random = Func_08004458();
                    table = M2C_FIELD(work, void **, 0x7828);
                    range = Data_080eebd6[M2C_FIELD(table, s32 *, 24) * 4 + 2];
                    offset = Func_08002304(random, range);
                    y = local_buf[1] - offset;
                    range -= offset;
                    y = y - Data_080eded0[mask] / 2 + 8;
                    random = Func_08004458();
                    range++;
                    x = local_buf[0] + Func_08002304(random, range);
                    x -= range / 2;
                    x -= Data_080edeca[mask] / 2;
                    random = Func_08004458();
                    table = M2C_FIELD(work, void **, 0x7828);
                    BattleEffect_LoadWork(47, 7, 7, 3 | Data_080eebe2[random & 3],
                        Data_080eebe6[M2C_FIELD(table, s32 *, 24)]);
                    draw47 = *(DrawRectangleFn *)0x03001f0c;
                    draw47(draw_destination, (u8 *)work + Data_080edebe[mask],
                        x, y, Data_080edeca[mask], Data_080eded0[mask]);
                    Func_08002dd8(47);
                    j++;
                    table = M2C_FIELD(work, void **, 0x7828);
                    group = M2C_FIELD(table, s32 *, 24);
                }
            }
            member_field_offset += 2;
            member_offset += 0xe00;
            member_index++;
        }
        {
            s32 *slot3;
            s32 i;

            slot3 = (s32 *)0x02010000;
            for (i = 0; i != 1024; i++, slot3 = (s32 *)((u8 *)slot3 + 28)) {
                s32 life = slot3[6];

                if (life <= 0) continue;

                slot3[6] = life - 1;
                Func_080e3908(slot3, 60, 128 << 5);

                if (slot3[1] > (208 << 15)) {
                    s32 vel = slot3[4];
                    slot3[4] = -vel / 2;
                    continue;
                }

                if ((u32) slot3[0] <= 0x7effffu && slot3[1] >= 0) {
                    s32 step = slot3[6];

                    if (step < 0) step += 15;
                    step = (step >> 4) + 1;

                    {
                        const u8 *source = (const u8 *)extra_target
                            + Data_080ede48[step - 1];
                        s32 half = step / 2;
                        s32 x = (slot3[0] >> 16) - half;
                        s32 y = (slot3[1] >> 16) - step;

                        draw_cb_46(draw_destination, source, x, y,
                            step, step * 2);
                    }
                }
            }
        }

        Func_080e155c(2, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        pass++;
        table = M2C_FIELD(work, void **, 0x7828);
        if (pass == M2C_FIELD(table, s32 *, 20) * 8 + 40) {
            break;
        }
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
