/*
 * The steps that turn the Gousei template into one blitter.  `src` walks the
 * template; a step either takes the words under it or leaves them behind.
 * SENTOU_KOUKA_GOUSEI_BUILD.C runs this list twice: once with steps that
 * only count, once with steps that write.
 */

    /* Save the registers, fetch width and height from the stack. */
    COPY(3);
    if (mode == GOUSEI_KASAN) {
        COPY_FROM(SentouKouka_Mask, 3);
    }

    /* Where the source starts and how it moves on to the next row. */
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_X) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_Y) {
        COPY_FROM(src, 4);
    }
    src += 4;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == 0) {
        PUT(src[0]);
    }
    src += 1;

    /* Rows above the canvas are skipped, rows below it dropped. */
    if ((flags & GOUSEI_CLIP_Y) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_Y) != 0) {
            PUT(src[2]);
        } else {
            PUT(src[3]);
        }
        PUT(src[4]);
        PUT(src[5]);
        PUT(src[6] + (1 << height_shift));
        PUT(src[7] + (1 << height_shift));
    }
    src += 8;

    PUT(*src++);
    MARK(no_rows);
    PUT(*src++);

    /* Columns either wrap round the canvas or are clipped at its sides. */
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0] + (1 << width_shift) - 1);
    }
    src += 1;
    if ((flags & GOUSEI_CLIP_X) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[2]);
            PUT(src[3]);
        } else {
            PUT(src[4]);
            PUT(src[5]);
        }
        PUT(src[6]);
        PUT(src[7]);
        PUT(src[8] + SentouKouka_BitOperands[width_shift]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[9]);
        } else {
            PUT(src[10]);
        }
        PUT(src[11] + SentouKouka_BitOperands[width_shift]);
    }
    src += 12;

    PUT(*src++);
    MARK(no_columns);
    PUT(*src++);

    /* Split the row at cell boundaries and find the first cell. */
    COPY(6);

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    COPY(5);

    /* Each row: the address of its first pixel in the canvas. */
    MARK(row);
    PUT(src[0]);
    PUT(src[1] + (1 << (height_shift - 3)) - 1);
    PUT(src[2] + ((width_shift - 3) << 7));
    PUT(src[3]);
    src += 4;

    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    /* The pixels up to the first cell boundary, one at a time. */
    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;
    PATCH(skip);

    /* The whole cells, eight pixels at a time. */
    PUT(*src++);
    MARK(skip);
    PUT(*src++);

    MARK(cell);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        {
            s32 i;

            for (i = 0; i < 8; i++) {
                if ((flags & GOUSEI_FLIP_X) != 0) {
                    COPY_FROM(SentouKouka_YomiGyaku, 2);
                } else {
                    COPY_FROM(SentouKouka_YomiJun, 2);
                }
            }
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
        } else {
            COPY_FROM(SentouKouka_Nuri8Jun, 25);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_Hikaku4Gyaku, 16);
        } else {
            COPY_TWICE(SentouKouka_Hikaku4Jun, 16);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_IroGyaku, 14);
        } else {
            COPY_TWICE(SentouKouka_IroJun, 14);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;

    PUT(*src++);
    PUT_BRANCH(cell);
    PATCH(skip);

    /* The pixels left after the last whole cell. */
    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PATCH(skip);

    /* Next source row, next canvas row, until the last. */
    COPY(3);

    PUT_BRANCH(row);
    PATCH(no_rows);
    PATCH(no_columns);

    /* Restore the registers and return. */
    PUT(*src++);
    PUT(*src++);
