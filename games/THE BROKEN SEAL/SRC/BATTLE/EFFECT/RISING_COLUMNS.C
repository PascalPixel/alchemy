#include "TYPES.H"

#define BattleEffect_RunRisingColumns Func_080dd77c

/* Sixteen columns grow and retract in staggered windows. Five image cells
 * cycle with frame and column, using the alternating cached blitters.
 * Position-query outputs are separate from the two three-word coordinates.
 */

typedef void (*DrawRectangle)(void *, const void *, s32, s32, s32, s32);
typedef struct Effect {
    s32 kind, side, actor, unknown0c, unknown10, count, layers, mode, unknown20;
    s16 actors[8];
} Effect;
typedef struct Column {
    s32 x;
    s32 unused[6];
} Column;
extern u8 Data_03001e50[];
extern s8 Data_080eeb96[];
extern u8 Value_000000a6;
void Func_080cd594(s32);
void Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e396c(s32, s32 *);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080041d8(s32, s32);
void Func_080f9010(s32);
#define Audio_PlayCue Func_080f9010
void Func_080d6888(s32, s32, s32, s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(s32);
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
s32 Func_080cdbc0(void);
#define WORK_EFFECT (*(Effect **)(work + 0x7828))

void BattleEffect_RunRisingColumns(Effect *effect)
{
    u32 *cache, *entry;
    u8 *work;
    void *dst;
    DrawRectangle draw[2];
    s32 origin_x, origin_y;
    s32 first[3], last[3];
    s32 i, frame, cell, height, offset, middle;
    Column *column;

    cache = (u32 *)(Data_03001e50 + 39 * 4);
    entry = cache;
    work = (u8 *)*entry++;
    dst = (void *)*entry;
    WORK_EFFECT = effect;
    Func_080de2f8(effect, 4, effect->side, 4, &origin_x, &origin_y);
    Func_080cd594(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Func_080e0524((s32)&Value_000000a6, work, 1, 1);
    Func_080e396c(WORK_EFFECT->actors[0], first);
    Func_080e396c(WORK_EFFECT->actors[WORK_EFFECT->count - 1], last);
    middle = first[0];
    middle += (last[0] - middle) / 2;
    first[0] = middle;
    *(s32 *)0x04000028 = (64 - first[0]) << 8;
    Func_080ed408(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)cache[7];
    Func_080ed408(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)cache[8];
    column = (Column *)(work + 0x7080);
    i = 0;
    do {
        column[i].x = Data_080eeb96[i] + 64;
        i++;
    } while (i != 16);
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = 0;
    Func_080041d8(0x080cd261, 0x480);
    frame = 0;
    do {
        if (frame == 32) {
            Audio_PlayCue(143);
            for (i = 0; i != WORK_EFFECT->count; i++)
                Func_080d6888(WORK_EFFECT->actors[i], 7, 5, i, 16);
        }
        i = 0;
        column = (Column *)(work + 0x7080);
        do {
            if (frame == i * 4 + 5)
                *(s32 *)(work + 0x77a8) = 2;
            offset = i * 2;
            if (frame > offset + 4) {
                cell = Func_080022fc(frame / 4 + i, 5);
                if (frame < offset + 32) {
                    height = (frame - offset) * 4 - 16;
                    if (height > 32) height = 32;
                } else {
                    height = 160 - (frame - offset) * 4;
                }
                if (height > 0)
                    draw[i & 1](dst, work + (cell << 10), column->x - 16,
                        (i & 7) - height + 104, 32, height);
            }
            i++;
            column++;
        } while (i != 16);
        Func_080e155c(4, 4);
        Func_080cd52c();
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 70);
    Func_08004278(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Func_080cdbc0();
}
