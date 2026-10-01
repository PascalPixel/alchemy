#include "TYPES.H"
#include "DMA.H"
#include "BATTLE_EFX.H"

/*
 * Runtime code assembler (one function).  It emits an ARM routine into a
 * freshly allocated heap block from the instruction template that starts at
 * SentouKouka_Gousei, in two passes: the first counts the words the second
 * will write.
 */

/* DMA enable, 32-bit transfer width and word count. */
#define DMA_WORDS(n) (0x84000000 | (n))
#define DMA3 ((volatile u32 *)0x040000d4)

/* ARM branch displacement folded into an already-assembled template word. */
#define BRANCH_OFFSET(site, target) \
    ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)


/* One emit step each.  Every step is its own statement block. */
#define PUT(word) do { *dst++ = (word); } while (0)
#define COPY_FROM(from, n) do { \
    Dma_Set(from, dst, DMA_WORDS(n), DMA3); \
    dst += (n); } while (0)
#define COPY_TWICE(from, n) \
    Dma_Set(from, dst, DMA_WORDS(n), DMA3); \
    dst += (n); \
    Dma_Set(from, dst, DMA_WORDS(n), DMA3); \
    dst += (n)
#define COPY(n) do { \
    Dma_Set(src, dst, DMA_WORDS(n), DMA3); \
    dst += (n); src += (n); } while (0)
#define PUT_BRANCH(target) do { \
    *dst = *src++ + BRANCH_OFFSET(dst, target); \
    dst++; } while (0)
#define FIX(site) do { *(site) |= BRANCH_OFFSET(site, dst); } while (0)

void *Runtime_AllocateHeapBlock(s32 id, s32 size);

extern const u32 SentouKouka_Gousei[];
extern const u32 ParticleStreams_CellOffsets[];
extern const u16 Data_080ef034[];
extern const u32 SentouKouka_YomiGyaku[];
extern const u32 SentouKouka_YomiJun[];
extern const u32 SentouKouka_NuriJun[];
extern const u32 SentouKouka_NuriGyaku[];
extern const u32 SentouKouka_Nuri8Jun[];
extern const u32 SentouKouka_Nuri8Gyaku[];
extern const u32 SentouKouka_HikakuJun[];
extern const u32 SentouKouka_HikakuGyaku[];
extern const u32 SentouKouka_Hikaku4Jun[];
extern const u32 SentouKouka_Hikaku4Gyaku[];
extern const u32 SentouKouka_KasanJun[];
extern const u32 SentouKouka_KasanGyaku[];
extern const u32 SentouKouka_Mask[];
extern const u32 SentouKouka_IroJun[];
extern const u32 SentouKouka_IroGyaku[];

s32 BattleEffect_LoadWork(s32 id, s32 a, s32 b, s32 flags, u32 mode)
{
    const u32 *src;
    u32 *dst;
    u32 *br_a;
    u32 *br_b;
    u32 *mark_c;
    u32 *fix;
    u32 *mark;
    u32 *mark_g;
    const u32 *body;
    s32 n;
    s32 i;
    s32 j;
    const u32 *end;

    /* Word budget: the emit sequence below, counted instead of written. */
    src = SentouKouka_Gousei;
    n = 3;
    src += 3;
    if (mode == 3) {
        n += 3;
    }
    if ((flags & 12) == 4) {
        n += 3;
    }
    src += 3;
    if ((flags & 12) == 8) {
        n += 4;
    }
    src += 4;
    if ((flags & 12) == 12) {
        n += 3;
    }
    src += 3;
    if ((flags & 12) == 0) {
        n++;
    }
    src += 1;
    if ((flags & 2) != 0) {
        n++;
        n++;
        if ((flags & 8) != 0) {
            n++;
        } else {
            n++;
        }
        n++;
        n++;
        n++;
        n++;
    }
    src += 8;
    n++;
    n++;
    if ((flags & 1) == 0) {
        n++;
    }
    src += 1;
    if ((flags & 1) != 0) {
        n++;
        n++;
        if ((flags & 4) != 0) {
            n++;
            n++;
        } else {
            n++;
            n++;
        }
        n++;
        n++;
        n++;
        if ((flags & 4) != 0) {
            n++;
        } else {
            n++;
        }
        n++;
    }
    src += 12;
    n++;
    n++;
    n += 6;
    src += 6;
    n++;
    if ((flags & 1) == 0) {
        n++;
    }
    src += 1;
    n += 5;
    src += 5;
    n++;
    n++;
    n++;
    n++;
    src += 4;
    if ((flags & 1) == 0) {
        n++;
    }
    src += 1;
    n++;
    n++;
    if ((flags & 4) != 0) {
        n++;
    } else {
        n++;
    }
    src += 2;
    switch (mode) {
    default:
    case 0:
        if ((flags & 4) != 0) {
            n += 2;
        } else {
            n += 2;
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            n += 4;
        } else {
            n += 4;
        }
        break;
    case 2:
        if ((flags & 4) != 0) {
            n += 4;
        } else {
            n += 4;
        }
        break;
    case 3:
        if ((flags & 4) != 0) {
            n += 6;
        } else {
            n += 6;
        }
        break;
    }
    src += 4;
    n++;
    n++;
    n++;
    if ((flags & 1) == 0) {
        n++;
        n++;
    }
    src += 2;
    n++;
    n++;
    switch (mode) {
    default:
    case 0:
        for (j = 0; j < 8; j++) {
            if ((flags & 4) != 0) {
                n += 2;
            } else {
                n += 2;
            }
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            n += 25;
        } else {
            n += 25;
        }
        break;
    case 2:
        n += 16;
        n += 16;
        break;
    case 3:
        n += 14;
        n += 14;
        break;
    }
    src += 4;
    n++;
    if ((flags & 1) == 0) {
        n++;
        n++;
    }
    src += 2;
    n++;
    n++;
    n++;
    n++;
    if ((flags & 4) != 0) {
        n++;
    } else {
        n++;
    }
    src += 2;
    switch (mode) {
    default:
    case 0:
        if ((flags & 4) != 0) {
            n += 2;
        } else {
            n += 2;
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            n += 4;
        } else {
            n += 4;
        }
        break;
    case 2:
        if ((flags & 4) != 0) {
            n += 4;
        } else {
            n += 4;
        }
        break;
    case 3:
        if ((flags & 4) != 0) {
            n += 6;
        } else {
            n += 6;
        }
        break;
    }
    src += 4;
    n++;
    n++;
    n += 3;
    src += 3;
    n++;
    n++;
    n++;

    dst = (u32 *)Runtime_AllocateHeapBlock(id, n << 2);
    src = SentouKouka_Gousei;

    /* Entry sequence. */
    COPY(3);
    if (mode == 3) {
        COPY_FROM(SentouKouka_Mask, 3);
    }

    /* Source-format prologue: one of four variants, or a single word. */
    if ((flags & 12) == 4) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & 12) == 8) {
        COPY_FROM(src, 4);
    }
    src += 4;
    if ((flags & 12) == 12) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & 12) == 0) {
        PUT(src[0]);
    }
    src += 1;

    if ((flags & 2) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & 8) != 0) {
            PUT(src[2]);
        } else {
            PUT(src[3]);
        }
        PUT(src[4]);
        PUT(src[5]);
        PUT(src[6] + (1 << b));
        PUT(src[7] + (1 << b));
    }
    src += 8;

    PUT(*src++);
    br_a = dst;
    PUT(*src++);

    if ((flags & 1) == 0) {
        PUT(src[0] + (1 << a) - 1);
    }
    src += 1;
    if ((flags & 1) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & 4) != 0) {
            PUT(src[2]);
            PUT(src[3]);
        } else {
            PUT(src[4]);
            PUT(src[5]);
        }
        PUT(src[6]);
        PUT(src[7]);
        PUT(src[8] + Data_080ef034[a]);
        if ((flags & 4) != 0) {
            PUT(src[9]);
        } else {
            PUT(src[10]);
        }
        PUT(src[11] + Data_080ef034[a]);
    }
    src += 12;

    PUT(*src++);
    br_b = dst;
    PUT(*src++);

    COPY(6);

    PUT(*src++);
    if ((flags & 1) == 0) {
        PUT(src[0]);
    }
    src += 1;

    COPY(5);

    mark_c = dst;
    PUT(src[0]);
    PUT(src[1] + (1 << (b - 3)) - 1);
    PUT(src[2] + ((a - 3) << 7));
    PUT(src[3]);
    src += 4;

    if ((flags & 1) == 0) {
        PUT(src[0]);
    }
    src += 1;

    PUT(*src++);
    fix = dst;
    PUT(*src++);
    if ((flags & 4) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    /* First per-mode body; the loop below branches back to it. */
    mark = dst;
    switch (mode) {
    default:
    case 0:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case 2:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case 3:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(mark);
    PUT(*src++);
    if ((flags & 1) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;
    FIX(fix);

    PUT(*src++);
    fix = dst;
    PUT(*src++);

    /* Second per-mode body; the run below it branches back here. */
    mark_g = dst;
    switch (mode) {
    default:
    case 0:
        for (i = 0; i < 8; i++) {
            if ((flags & 4) != 0) {
                COPY_FROM(SentouKouka_YomiGyaku, 2);
            } else {
                COPY_FROM(SentouKouka_YomiJun, 2);
            }
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
        } else {
            COPY_FROM(SentouKouka_Nuri8Jun, 25);
        }
        break;
    case 2:
        if ((flags & 4) != 0) {
            body = SentouKouka_Hikaku4Gyaku;
            COPY_TWICE(body, 16);
        } else {
            body = SentouKouka_Hikaku4Jun;
            COPY_TWICE(body, 16);
        }
        break;
    case 3:
        if ((flags & 4) != 0) {
            body = SentouKouka_IroGyaku;
            COPY_TWICE(body, 14);
        } else {
            body = SentouKouka_IroJun;
            COPY_TWICE(body, 14);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    if ((flags & 1) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;

    PUT(*src++);
    PUT_BRANCH(mark_g);
    FIX(fix);

    PUT(*src++);
    fix = dst;
    PUT(*src++);
    if ((flags & 4) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    /* Third per-mode body, same four variants as the first. */
    mark = dst;
    switch (mode) {
    default:
    case 0:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case 1:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case 2:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case 3:
        if ((flags & 4) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(mark);
    FIX(fix);

    COPY(3);

    PUT_BRANCH(mark_c);
    FIX(br_a);
    FIX(br_b);

    PUT(*src++);
    *dst = *src++;

    end = ParticleStreams_CellOffsets;
    return src == end;
}
