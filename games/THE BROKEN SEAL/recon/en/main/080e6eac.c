#include "TYPES.H"
#include "BATTLE_EFX.H"

/* Runs the layered particle presentation over the shared battle-effect work buffers. */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);
typedef void (*ClearFn)(void *dest, s32 size);

s32 Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080f9010(s32 id);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
void Func_080e6d3c(void *object, s32 a, s32 b);

#define Table_080eee66 ((u16 *)0x080EEE66)
#define Table_080eee56 ((u8 *)0x080EEE56)
#define Table_080eee5e ((u8 *)0x080EEE5E)
extern const u16 Data_080ede48[];

void Func_080e6eac(void *object, s32 x_arg, s32 y_arg)
{
    void **heap_cache;
    void **cursor;
    void *work;
    s32 raw_x;
    void *canvas;
    void *rect1;
    void *rect0;
    void *aux;
    s32 half_x;
    s32 i;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    raw_x = x_arg;
    canvas = *cursor;
    half_x = (raw_x + 0x280000) / 2;
    aux = heap_cache[2];

    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x80;
    M2C_FIELD((void *)0x04000020, s32 *, 8) = 0;
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F46;

    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    rect0 = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    rect1 = heap_cache[8];

    Resource_LoadAndDecompress(0x73, aux, 0, 0);
    Resource_LoadAndDecompress(0x5E, work, 1, 0);
    Resource_LoadAndDecompress(0x5F, (u8 *)work + 0x59D8, 0, 0);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);

    {
        void *record_cursor;

        record_cursor = (u8 *)work + 0x7080;
        for (i = 0; i != 64; i++) {
            u16 seed;
            s32 amp;

            amp = (0xFF & Func_08004458()) + 0x100;
            seed = (u16)Func_08004458();
            M2C_FIELD(record_cursor, s32 *, 0) = half_x;
            M2C_FIELD(record_cursor, s32 *, 4) = y_arg;
            M2C_FIELD(record_cursor, s32 *, 0xC) =
                (amp * Func_08002322(seed)) >> 7;
            M2C_FIELD(record_cursor, s32 *, 0x10) =
                0 - ((amp * Func_0800231c(seed)) >> 6);
            M2C_FIELD(record_cursor, s32 *, 0x18) =
                (0xF & Func_08004458()) + 0x10;
            record_cursor = (u8 *)record_cursor + 0x1C;
        }
    }

    {
        void *record_cursor;
        s32 angle;

        record_cursor = (u8 *)work + 0x772C;
        angle = 0;
        for (i = 0; i != 3; i++) {
            M2C_FIELD(record_cursor, s32 *, 0) = half_x;
            M2C_FIELD(record_cursor, s32 *, 4) = y_arg;
            M2C_FIELD(record_cursor, s32 *, 0xC) =
                (Func_08002322(angle) << 5) >> 6;
            M2C_FIELD(record_cursor, s32 *, 0x10) =
                0 - ((Func_0800231c(angle) << 5) >> 5);
            angle += 0x5555;
            record_cursor = (u8 *)record_cursor + 0x1C;
        }
    }

    {
        void *record_cursor;

        record_cursor = (void *)0x02010000;
        for (i = 0; i != 64; i++) {
            u16 seed;
            s32 amp;

            amp = (0xFF & Func_08004458()) + 0x20;
            seed = (u16)Func_08004458();
            M2C_FIELD(record_cursor, s32 *, 0) = half_x;
            M2C_FIELD(record_cursor, s32 *, 4) = y_arg;
            M2C_FIELD(record_cursor, s32 *, 0xC) =
                (amp * Func_08002322(seed)) >> 6;
            M2C_FIELD(record_cursor, s32 *, 0x10) =
                0 - ((amp * Func_0800231c(seed)) >> 5);
            M2C_FIELD(record_cursor, s32 *, 0x18) =
                (0xF & Func_08004458()) + 0x14;
            record_cursor = (u8 *)record_cursor + 0x1C;
        }
    }

    for (frame = 0; frame != 72; frame++) {
        if (frame == 4) {
            Func_080f9010(0x9A);
        }
        if (frame == 32) {
            Func_080f9010(0xD4);
        }
        if (frame <= 47) {
            s32 idx;
            u8 w;
            u8 h;

            idx = Func_080022ec(frame - 8, 5);
            if (idx < 0) {
                idx = 0;
            }
            w = Table_080eee56[idx];
            h = Table_080eee5e[idx];
            ((DrawRectangleFn)rect0)(
                canvas, (u8 *)work + Table_080eee66[idx],
                (half_x >> 16) - (w >> 1), (y_arg >> 16) - (h >> 1), w, h);
        }

        {
            void *record_cursor;

            record_cursor = (u8 *)work + 0x7080;
            for (i = 0; i != 30; i++) {
                if (frame > i / 2) {
                    s32 timer;

                    timer = M2C_FIELD(record_cursor, s32 *, 0x18);
                    if (timer > 0) {
                        s32 phase;
                        s32 tile;
                        s32 half;

                        M2C_FIELD(record_cursor, s32 *, 0x18) = timer - 1;
                        Func_080e3908(record_cursor, 60, 0);
                        phase = M2C_FIELD(record_cursor, s32 *, 0x18);
                        if (phase < 0) {
                            phase += 15;
                        }
                        phase = (phase >> 4) + 3;
                        tile = phase * 2;
                        half = phase / 2;
                        ((DrawRectangleFn)rect1)(
                            canvas,
                            (u8 *)aux + Data_080ede48[phase - 1],
                            M2C_FIELD(record_cursor, s16 *, 2) - half,
                            M2C_FIELD(record_cursor, s16 *, 6) - phase,
                            phase, tile);
                    }
                }
                record_cursor = (u8 *)record_cursor + 0x1C;
            }
        }

        {
            void *record_cursor;
            const u16 *table;

            table = Data_080ede48;
            record_cursor = (void *)0x02010000;
            for (i = 0; i != 60; i++) {
                if (frame > 35) {
                    s32 timer;

                    timer = M2C_FIELD(record_cursor, s32 *, 0x18);
                    if (timer > 0) {
                        s32 phase;
                        s32 tile;
                        s32 half;

                        M2C_FIELD(record_cursor, s32 *, 0x18) = timer - 1;
                        Func_080e3908(record_cursor, 60, 0);
                        phase = M2C_FIELD(record_cursor, s32 *, 0x18);
                        if (phase < 0) {
                            phase += 15;
                        }
                        phase = (phase >> 4) + 1;
                        tile = phase * 2;
                        half = phase / 2;
                        ((DrawRectangleFn)rect1)(
                            canvas,
                            (u8 *)aux + table[phase - 1],
                            M2C_FIELD(record_cursor, s16 *, 2) - half,
                            M2C_FIELD(record_cursor, s16 *, 6) - phase,
                            phase, tile);
                    }
                }
                record_cursor = (u8 *)record_cursor + 0x1C;
            }
        }

        {
            void *record_cursor;
            s32 age;

            age = frame - 36;
            record_cursor = (u8 *)work + 0x772C;
            for (i = 0; i != 3; i++) {
                if ((u32)age <= 27) {
                    Func_080e3908(record_cursor, 64, 0);
                    ((DrawRectangleFn)rect0)(
                        canvas,
                        (u8 *)work + 0x59D8 + Func_080022ec(age, 7) * 0x120,
                        M2C_FIELD(record_cursor, s16 *, 2) - 6,
                        M2C_FIELD(record_cursor, s16 *, 6) - 12, 12, 24);
                }
                record_cursor = (u8 *)record_cursor + 0x1C;
            }
        }

        if (frame <= 35) {
            Func_080e6d3c(object, raw_x, y_arg);
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    ((ClearFn)0x03000164)((void *)0x06004000, 0x4000);
    Func_08002dd8(47);
    Func_08002dd8(46);
}
