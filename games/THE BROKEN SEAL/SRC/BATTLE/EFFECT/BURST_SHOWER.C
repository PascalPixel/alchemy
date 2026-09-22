#include "BATTLE_PRESENTATION.H"
#include "BATTLE_EFFECT_WORK.H"

#define BattleEffect_RunBurstShower Func_080dbc30

/* Six drawn arguments: destination, source cell, x, y, width, height.
   Called through the r4 bx bank, so it is an indirect call through a
   cached blitter entry rather than a fixed callee. */
typedef void (*DrawRectangle)(void *, const void *, s32, s32, s32, s32);

/* The word-copy entry the runtime publishes at 0x03001388; reached through
   the r3 bx bank. */
typedef s32 (*CopyWords)(void *, const void *, s32);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40, 41, 46 and 47. */
extern u8 Data_03001e50[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_00000054;
extern u8 Value_0000005a;
extern u8 Value_0000006e;
extern u8 Value_00000073;
extern u8 Value_0000007d;
extern u8 Value_0000008d;
extern u8 Value_000000a1;
extern u8 Value_000000b9;
extern u8 Value_000000ce;

void Func_080cd594(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void BattleFx_FetchRectangleBlitters(s32, u32 *);
void Func_080e0524(s32, void *, s32, s32);
#define Resource_LoadAndDecompress Func_080e0524
void *Func_08002f40(s32);
#define Resource_GetTableEntry Func_08002f40
void Func_080041d8(s32, s32);
#define Scheduler_AddOrUpdateCallback Func_080041d8
void Func_08004278(s32);
#define Scheduler_RemoveCallback Func_08004278
void Func_080e3958(s32, s32 *);
#define EffectPosition_ApplyAnimationAndYOffset Func_080e3958
void Func_080e396c(s32, s32 *);
#define EffectPosition_ApplyStepAndYOffset Func_080e396c
s32 Func_080022ec(s32, s32);
#define Math_Div Func_080022ec
s32 Func_080022fc(s32, s32);
s32 Func_08004458(void);
#define Random16 Func_08004458
void Func_080f9010(s32);
#define Audio_PlayCue Func_080f9010
void Func_080b5088(s32, s32);
void Func_080b50e8(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
#define ObjectGroup_UpdateMembers Func_080d6888
void Func_080e155c(s32, s32);
#define Camera_ApplyShake Func_080e155c
void Func_080cd52c(void);
#define ObjectGroup_TickMemberTimers Func_080cd52c
void Func_080030f8(s32);
#define WaitFrames Func_080030f8
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
s32 Func_080cdbc0(void);

/* The caller's effect state, republished at work + 0x7828. */
typedef struct BattleEffectArgument {
    s32 kind;
    s32 side;
    s32 actor;
    s32 unk0C;
    s32 unk10;
    s32 cnt;
    s32 variant;
    s32 unk1C;
    s32 unk20;
    s16 actors[8];
} Efx;

/* One 28-byte record; the array starts at work + 0x7080.  x and y are
   16.16 fixed point and their integer halves are read directly. */
typedef struct Spark {
    s32 x;
    s32 y;
    s32 unk08;
    s32 dx;
    s32 dy;
    s32 unk14;
    s32 tick;
} Spark;

#define WORK_EFX ((Efx *)work->effect)
#define SHEET ((u8 *)work + 0xC56)
#define SPARKS ((Spark *)((u8 *)work + 0x7080))

static __inline__ void CopyPalette(CopyWords copy, void *destination, const void *source, s32 size)
{
    copy(destination, source, size);
}

void BattleEffect_RunBurstShower(Efx *efx, s32 mode)
{
    u32 *cache;
    struct BattleEffectWork *work;
    void *dst;
    u8 *aux;
    void *blit[2];
    s32 pos[3];
    s32 seat[8][3];
    s32 aim[3];
    u16 *pal;
    s32 frame;
    s32 i;
    s32 pick;
    s32 lum;
    s32 id;
    s32 width;
    s32 height;

    cache = (u32 *)(Data_03001e50 + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (struct BattleEffectWork *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    work->effect = efx;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;

    if (mode == 7) {
        Func_080ed408(46, 7, 7, 3, 2);
        blit[0] = (void *)cache[46 - 40];
        Func_080ed408(47, 7, 7, 7, 2);
        blit[1] = (void *)cache[47 - 40];
    } else {
        BattleFx_FetchRectangleBlitters(WORK_EFX->side, (u32 *)blit);
    }

    Resource_LoadAndDecompress((s32)&Value_000000ce, work, 1, 0);
    if (mode == 5) {
        Resource_LoadAndDecompress((s32)&Value_0000005a, SHEET, 1, 1);
    } else if (mode == 7) {
        Resource_LoadAndDecompress((s32)&Value_00000054, SHEET, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&Value_0000007d, SHEET, 1, 1);
        Resource_LoadAndDecompress((s32)&Value_00000073, aux, 0, 0);
        if (mode == 6) {
            for (i = 0, pal = (u16 *)0x05000000; i != 64; i++) {
                lum = i / 4;
                *pal = (u16)(((lum << 10) | (lum << 5)) | lum);
                pal++;
            }
            *(s16 *)0x04000050 = 0;
        } else {
            switch (mode) {
            case 0:
                id = (s32)&Value_0000007d;
                break;
            case 1:
                id = (s32)&Value_000000b9;
                break;
            case 2:
                id = (s32)&Value_0000006e;
                break;
            case 3:
                id = (s32)&Value_000000a1;
                break;
            case 4:
            default:
                id = (s32)&Value_0000008d;
                break;
            }
            CopyPalette((CopyWords)0x03001388, (void *)0x05000000, Resource_GetTableEntry(id), 128);
        }
    }

    if (mode == 7) {
        work->transfer_mode = 2;
        work->transfer_value = 50;
    } else {
        work->transfer_mode = 2;
        work->transfer_value = 75;
    }
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);

    for (i = 0; i != 64; i++) {
        SPARKS[i].tick = -1;
    }

    EffectPosition_ApplyAnimationAndYOffset(WORK_EFX->actor, pos);
    if (mode == 3) {
        pos[1] -= 16;
    }
    if (mode == 4) {
        if (WORK_EFX->side == 1) {
            pos[0] += 28;
        } else {
            pos[0] -= 28;
        }
    }
    if (mode == 7) {
        if (WORK_EFX->side == 1) {
            pos[0] += 16;
        } else {
            pos[0] -= 16;
        }
    }
    if (mode == 5) {
        pos[0] = Math_Div(pos[0], 3);
        *(s16 *)0x04000020 = 85;
    }

    i = 0;
    while (i != WORK_EFX->cnt) {
        EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[i], seat[i]);
        i += 1;
    }

    frame = 0;
    do {
        pick = Func_080022fc(frame, WORK_EFX->cnt);
        if (frame == 4) {
            Audio_PlayCue(0x88);
        }
        if (mode != 6) {
            if (frame == 24) {
                Func_080b50e8(0x86);
            }
        } else {
            if (frame == 60) {
                Func_080b50e8(0x86);
            }
        }

        if (mode == 5) {
            /* One large cell of a nine-cell sheet; the sequence advances
               every third frame and wraps every ninth. */
            if (WORK_EFX->side == 1) {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((Func_080022fc(Math_Div(frame, 3), 3) * 9)
                            << 9),
                    pos[0] - 2, pos[1] - 32, 72, 62);
            } else {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((Func_080022fc(Math_Div(frame, 3), 3) * 9)
                            << 9),
                    pos[0] - 70, pos[1] - 32, 72, 62);
            }
        } else {
            aim[0] = (seat[pick][0] + (Random16() & 31)) - 16;
            aim[1] = (seat[pick][1] + (Random16() & 63)) - 16;
            if (frame <= 47) {
                /* The original uses half the vertical fixed-point scale for x. */
                SPARKS[frame].x = pos[0] << 15;
                SPARKS[frame].y = pos[1] << 16;
                SPARKS[frame].dx = (aim[0] - pos[0]) << 11;
                SPARKS[frame].dy = (aim[1] - pos[1]) << 11;
                SPARKS[frame].tick = 0;
            }
        }

        i = 0;
        width = 32;
        height = 64;
        do {
            Spark *spark = &SPARKS[i];
            if (spark->tick >= 0) {
                if (mode == 7) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[WORK_EFX->side])(dst, SHEET,
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                } else if (mode == 4) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                } else if (mode != 5) {
                    if (spark->tick > 1) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                }
                spark->x += spark->dx;
                spark->y += spark->dy;
                spark->tick += 1;
                if (spark->tick == 24) {
                    spark->tick = -1;
                }
            }
            i += 1;
        } while (i != 64);

        if (mode == 5) {
            i = 0;
            while (i != WORK_EFX->cnt) {
                if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
                    *(s32 *)((u8 *)work + 0x77A8) = 8;
                    ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
                }
                i += 1;
            }
        } else {
            i = 0;
            while (i != WORK_EFX->cnt) {
                if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
                    *(s32 *)((u8 *)work + 0x77A8) = 8;
                    if (mode == 6) {
                        ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 14, 5, i, 4);
                    } else {
                        ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
                    }
                    Func_080b5088(WORK_EFX->actors[i], 4);
                }
                i += 1;
            }
        }

        Camera_ApplyShake(4, 4);
        if (mode != 6) {
            ObjectGroup_TickMemberTimers();
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    } while (frame != 64);

    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Func_080cdbc0();
}
