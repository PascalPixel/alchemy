#include "types.h"

/* Modal owner status screen.
 *
 * The screen draws one party owner's record: name, class glyph, level and
 * experience, the four equipment rows, the derived stat block, the status
 * icon strip, and a description window for whatever the cursor rests on.
 * It owns its own frame loop: it allocates its render slots up front, keeps
 * running until the shared screen work at 0x03001F34 reports its timer field
 * cleared (or the system flag word raises bit 1), then releases everything
 * and returns 0.
 *
 * Cursor state lives in one 32-byte record. `mode` selects which of three
 * cursor rows is live (0 = stat block, 1 = equipment, 2 = status icons) and
 * `pos[mode]` is that row's index; three small ROM tables translate the
 * live cursor into the highlighted cell and the description entry id.
 *
 * Uncertain: the exact roles of the owner-record bytes at 0x130..0x147 that
 * feed the status icon strip are only known by the icon code each one emits,
 * so they stay spelled as raw offsets. The 12-byte render entries are
 * initialised by whole words (0x80000400 / 0x40000400 / 0xC0002400) whose
 * field split is not recovered; only the parts this screen rewrites by name
 * are modelled as bit-fields.
 */

/* Only the m2c spellings this draft actually uses. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define Resource_LoadIntoFreeSlot Func_080040b4
#define Ui_LoadEntryForKind Func_08021b80
#define Resource_LoadTableEntryToBuffer Func_08021ab0
#define UiWindow_MarkVisibleTileAttributes Func_0801e318
#define UiWindow_SetTilemapEntry Func_08019000
#define UiWork_PushValueSlot Func_08019908
#define Ui_FillVramBlockPattern Func_08016738

/* Shared cells this screen reads or writes. */
#define KEY_STATE (*(u32 *)0x03001B04)
#define RENDER_FLAGS (*(u32 *)0x03001E40)
#define SYSTEM_FLAGS (*(u32 *)0x03001C94)
#define RENDER_BASE (*(u8 **)0x03001E8C)
#define SESSION_WORK (*(u8 **)0x03001E74)
#define SCREEN_WORK (*(struct UiScreenWork **)0x03001F34)
#define SLOT_TABLE ((struct UiSlotRecord *)0x03001B10)

/* Render list entry pushed through Runtime_PushSlotEntry. */
struct UiObjEntry {
    struct UiObjEntry *next; /* 0x00 */
    u8 y;                    /* 0x04 */
    u8 field_05;             /* 0x05 */
    u16 x : 9;               /* 0x06 */
    u16 shape : 5;
    u16 size : 2;
    u16 tile : 10; /* 0x08 */
    u16 prio : 2;
    u16 pal : 4;
    u16 field_0a; /* 0x0a */
};

/* Loaded render slot record indexed by a resource handle. */
struct UiSlotRecord {
    u8 field_00[2];
    u16 field_02 : 5;
    u16 tile : 10;
    u16 field_03 : 1;
};

/* Sprite record reached through the party actor table. */
struct UiSpriteRecord {
    u8 field_00[8];
    u16 tile : 10;
    u16 field_09 : 6;
};

struct UiWindow {
    u8 field_00[8];
    u16 width;  /* 0x08 */
    u16 height; /* 0x0a */
    u16 x;      /* 0x0c */
    u16 y;      /* 0x0e */
};

struct UiScreenWork {
    u8 field_00[68];
    void *work;  /* 0x44 */
    s32 busy;    /* 0x48 */
    s32 running; /* 0x4c */
};

/* Cursor state for the three selectable rows. */
struct StatusCursor {
    s8 pos[5];
    s32 entry;     /* 0x08 */
    s32 lastEntry; /* 0x0c */
    s32 mode;      /* 0x10 */
    s32 limit;     /* 0x14 */
    s32 col;       /* 0x18 */
    s32 row;       /* 0x1c */
};

extern u8 Data_08037328[];  /* stat-block cursor cells, 8 bytes each */
extern u8 Data_080373a8[];  /* equipment cursor cells, 4 bytes each */
extern u8 Data_080373b8[];  /* status icon cursor cells, 4 bytes each */
extern u8 Data_080373d8[];  /* "Lv" label */
extern u8 Data_080373dc[];  /* attack label */
extern u8 Data_080373e0[];  /* separator */
extern u8 Data_080373e4[];  /* defence label */
extern u8 Data_080310a4[];  /* cursor frame resource */

extern u8 Value_00000741;  /* element glyph base */
extern u8 Value_000008ba;  /* stat label glyph base */
extern u8 Value_000008bd;  /* alive state glyph */
extern u8 Value_000008be;  /* downed state glyph */
extern u8 Value_000008bf;  /* experience-to-next description */
extern u8 Value_000008c0;  /* stat description table base */
extern u8 Value_000008d2;  /* status icon description base */


s32 Resource_LoadIntoFreeSlot(s32 size);
void Resource_ResetEntry(s32 handle);
s32 Resource_GetBuffer(s32 handle, u8 *resource);
void Runtime_PushSlotEntry(struct UiObjEntry *entry, s32 slot);
void *Runtime_BumpAllocate(s32 size);
void Func_08002df0(void *block);
void Func_080072f0(void *dst, const void *src, s32 size, s32 tag);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(struct UiWindow *window, s32 mode);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void Ui_FillVramBlockPattern(void);
void Func_08017248(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_08017aa4(u16 *text, struct UiWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntry(
    struct UiWindow *window, s32 id, s32 x, s32 y, s32 style);
s32 Func_0801965c(s32 key, u16 *destination, u32 capacity);
void UiWork_PushValueSlot(s32 value, s32 slot);
void UiWindow_MarkVisibleTileAttributes(void);
void Func_0801e41c(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacter(s32 id, struct UiWindow *window, s32 x, s32 y);
void UiText_DrawStringAtOffset(
    const u8 *text, struct UiWindow *window, s32 x, s32 y);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, struct UiWindow *window, s32 x, s32 y);
void Func_0801f200(s32 id);
void Resource_LoadTableEntryToBuffer(s32 icon, s32 handle);
s32 Ui_LoadEntryForKind(s32 owner, s32 handle);
void Func_08022768(s32 x, s32 y, s32 width, s32 height, s32 style);
u8 *Runtime_GetObject(s32 owner);
void BattleUnit_Recalculate(s32 owner);
s32 Func_080771f8(s32 owner, s32 index);
s32 Func_08077258(s32 owner, s32 level);
s32 Func_08077290(s32 request);
struct UiSpriteRecord **Func_080b5098(s32 owner);

#define Ui_RunOwnerStatusScreen Func_08023178

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct UiObjEntry objFace;
    struct UiObjEntry objCursor;
    s32 tbl[11];
    s8 sel[11];
    struct UiObjEntry objIcon[11];
    struct UiObjEntry objName;
    struct StatusCursor cur;
    struct UiWindow *winMain;
    struct UiWindow *winDesc;
    u8 *object;
    u8 *base;
    struct UiScreenWork *screen;
    u8 *cell;
    s32 *q;
    u16 *text;
    s32 handleFrame;
    s32 handleCursor;
    s32 extended;
    s32 redraw;
    s32 keys;
    s32 cnt;
    s32 idx;
    s32 iconIdx;
    s32 colPixels;
    s32 rowPixels;
    s32 i;
    s32 n;
    s32 code;
    s32 hpDelta;
    s32 ppDelta;
    s32 spDelta;
    s8 *out;
    u8 *saved;
    struct UiObjEntry *p;

    base = RENDER_BASE;
    idx = -1;
    redraw = 1;
    handleFrame = Resource_LoadIntoFreeSlot(0x200);
    cnt = 0;
    extended = Func_08077290(-1);

    screen = SCREEN_WORK;
    screen->busy = 1;
    if (screen->work != NULL) {
        UiWork_Finalize(screen->work, 1);
        screen->work = NULL;
    }
    Audio_PlayCue(0x70);

    out = &cur.pos[4];
    for (i = 4; i >= 0; i--) {
        *out-- = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    q = tbl;
    out = sel;
    for (i = 10; i >= 0; i--) {
        *q++ = Resource_LoadIntoFreeSlot(0x80);
        *out++ = -1;
    }

    if (listCount != 0) {
        for (n = 0; n <= 5; n++) {
            code = list[n];
            if (code == 0xFF) {
                break;
            }
            if (code != 0xFE) {
                if (code == owner) {
                    idx = n;
                    break;
                }
            }
        }
    }

    colPixels = 0;
    rowPixels = 0;
    winMain = UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;) {
        keys = KEY_STATE;

        M2C_FIELD(&objName, u32 *, 4) = 0x80000400;
        M2C_FIELD(&objName, u32 *, 8) = 0;
        objName.tile = Ui_LoadEntryForKind(owner, handleFrame) & 0x3FF;
        objName.x = 8;
        objName.y = 24;
        objName.pal = 14;
        Runtime_PushSlotEntry(&objName, 240);

        switch (cur.entry) {
        default:
            cur.mode = 0;
            if (extended != 0) {
                cur.limit = 9;
            } else {
                cur.limit = 7;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            cur.mode = 1;
            cur.limit = 4;
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            cur.mode = 2;
            cur.limit = cnt;
            break;
        }

        Func_08022768(
            winMain->x, winMain->y, winMain->width, winMain->height, 15);

        if (cnt != 0) {
            if ((u32)cur.mode <= 1) {
                n = cur.pos[cur.mode];
                if ((keys & 0x80) != 0) {
                    keys = 0;
                    Audio_PlayCue(0x6F);
                    n++;
                    if (n >= cur.limit) {
                        n = 0;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[2];
                        }
                    }
                } else if ((keys & 0x40) != 0) {
                    keys = 0;
                    n--;
                    Audio_PlayCue(0x6F);
                    if (n < 0) {
                        n = cur.limit - 1;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[2];
                        }
                    }
                } else if ((keys & 0x31) != 0) {
                    keys = 0;
                    Audio_PlayCue(0x6F);
                    cur.mode ^= 2;
                    if ((KEY_STATE & 1) != 0) {
                        n = cur.pos[cur.mode];
                    } else if ((KEY_STATE & 0x20) != 0) {
                        n = cnt - 1;
                    } else {
                        n = 0;
                    }
                }
                cur.pos[cur.mode] = (s8)n;
            } else if (cur.mode == 2) {
                n = cur.pos[2];
                if (n >= cur.limit) {
                    n = cur.limit - 1;
                }
                if (n < 0) {
                    cur.mode = 0;
                    n = cur.pos[0];
                } else if ((keys & 0x10) != 0) {
                    keys = 0;
                    n++;
                    if (n >= cur.limit) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    Audio_PlayCue(0x6F);
                } else if ((keys & 0x20) != 0) {
                    keys = 0;
                    n--;
                    if (n < 0) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    Audio_PlayCue(0x6F);
                } else if ((keys & 0xC1) != 0) {
                    keys = 0;
                    cur.mode = 0;
                    n = cur.pos[0];
                    Audio_PlayCue(0x6F);
                }
                cur.pos[cur.mode] = (s8)n;
            }
        }

        if (cur.mode == 0) {
            i = cur.pos[0];
            if (extended == 0) {
                i += 9;
            }
            cell = &Data_08037328[i * 8];
            cur.entry = cell[0];
            cur.col = cell[1];
            cur.row = cell[2];
            Func_08022768(
                winMain->x + cell[3] + 1, winMain->y + cell[4] + 1, cell[5], 1,
                14);
        } else if (cur.mode == 1) {
            cell = &Data_080373a8[cur.pos[1] * 4];
            cur.entry = cell[0];
            cur.col = cell[1];
            cur.row = cell[2];
        } else if (cur.mode == 2) {
            cell = &Data_080373b8[cur.pos[2] * 4];
            cur.entry = cell[0];
            cur.col = cell[1];
            cur.row = cell[2];
        }

        if (cur.lastEntry != cur.entry) {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        colPixels = cur.col * 8;
        rowPixels = cur.row * 8;

        M2C_FIELD(&objFace, u32 *, 4) = 0xC0002400;
        M2C_FIELD(&objFace, u32 *, 8) = 0;
        objFace.tile = M2C_FIELD(*Func_080b5098(owner),
            struct UiSpriteRecord **, 0x50)->tile;
        objFace.x = 0xAC;
        objFace.y = 56;
        Runtime_PushSlotEntry(&objFace, 240);

        M2C_FIELD(&objCursor, u32 *, 4) = 0x40000400;
        M2C_FIELD(&objCursor, u32 *, 8) = 0;
        objCursor.tile = Resource_GetBuffer(handleCursor, Data_080310a4)
            & 0x3FF;
        objCursor.x = (colPixels + winMain->x * 8
                          - ((RENDER_FLAGS & 4) >> 2))
            + 16;
        objCursor.y = (u8)((rowPixels + winMain->y * 8
                               - ((RENDER_FLAGS & 4) >> 2))
            + 16);
        objCursor.shape = 8;
        Runtime_PushSlotEntry(&objCursor, 241);

        if (redraw != 0) {
            object = Runtime_GetObject(owner);
            UiWindow_MarkVisibleTileAttributes();
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0) {
                RenderOutput_RedrawSavedRect(winMain);
                Func_08017248(winDesc->x, winDesc->y, winDesc->width,
                    winDesc->height, 0);
                Func_0801e41c(winMain, 0, 14, 29, 14);

                UiText_DrawStringAtOffset(object, winMain, 0, 0);
                UiText_DrawStringAtOffset(Data_080373d8, winMain, 56, 0);
                UiText_DrawNumberInWindow(object[15], 2, winMain, 72, 0);
                UiText_DrawCharacter((s32)&Value_000008ba, winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, s32 *, 0x124), 8, winMain, 40, 8);

                UiText_DrawStringAtOffset(Data_080373dc, winMain, 40, 24);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, s16 *, 0x38), 4, winMain, 56, 24);
                UiText_DrawStringAtOffset(Data_080373e0, winMain, 88, 24);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, s16 *, 0x34), 4, winMain, 96, 24);
                UiText_DrawStringAtOffset(Data_080373e4, winMain, 40, 32);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, s16 *, 0x3A), 4, winMain, 56, 32);
                UiText_DrawStringAtOffset(Data_080373e0, winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, s16 *, 0x36), 4, winMain, 96, 32);

                UiText_DrawCharacter((s32)&Value_000008ba - 10, winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, u16 *, 0x3C), 3, winMain, 184, 16);
                UiText_DrawCharacter((s32)&Value_000008ba - 9, winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, u16 *, 0x3E), 3, winMain, 184, 24);
                UiText_DrawCharacter((s32)&Value_000008ba - 8, winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    M2C_FIELD(object, u16 *, 0x40), 3, winMain, 184, 32);
                UiText_DrawCharacter((s32)&Value_000008ba - 7, winMain, 136, 40);
                UiText_DrawNumberInWindow(object[0x42], 3, winMain, 184, 40);

                UiText_DrawCharacter(
                    (s32)&Value_00000741 + object[0x129], winMain, 0, 48);
                if (extended != 0) {
                    UiText_DrawCharacter((s32)&Value_000008ba - 1, winMain, 0, 72);
                }
                UiText_DrawCharacter((s32)&Value_000008ba - 5, winMain, 0, 80);
                UiText_DrawCharacter((s32)&Value_000008ba - 4, winMain, 0, 88);
                UiText_DrawCharacter((s32)&Value_000008ba - 3, winMain, 0, 96);

                for (i = 0; i <= 3; i++) {
                    if (extended != 0) {
                        code = 8;
                    } else {
                        code = 9;
                    }
                    UiWindow_SetTilemapEntry(
                        winMain, 0x5001 + i, 7 + i * 4, code, 0);
                    if (extended != 0) {
                        UiText_DrawNumberInWindow(object[0x11C + i], 1,
                            winMain, 40 + i * 32, 72);
                        UiText_DrawStringAtOffset(
                            Data_080373e0, winMain, 48 + i * 32, 72);
                        UiText_DrawNumberInWindow(object[0x118 + i], 1,
                            winMain, 48 + i * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Func_080771f8(owner, i), 2,
                        winMain, 48 + i * 32, 80);
                    UiText_DrawNumberInWindow(
                        M2C_FIELD(object, s16 *, 0x48 + i * 4), 3, winMain,
                        40 + i * 32, 88);
                    UiText_DrawNumberInWindow(
                        M2C_FIELD(object, s16 *, 0x4A + i * 4), 3, winMain,
                        40 + i * 32, 96);
                }

                n = 0;
                if (M2C_FIELD(object, s16 *, 0x38) == 0) {
                    sel[0] = 16;
                    n = 1;
                }
                out = &sel[n];
                do {
                    if (M2C_FIELD(object, s8 *, 0x130) != 0) {
                        *out++ = 15;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x141) != 0) {
                        *out++ = 8;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, s8 *, 0x131) == 1) {
                        *out++ = 1;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, s8 *, 0x131) == 2) {
                        *out++ = 2;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x13D) != 0) {
                        *out++ = 4;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x13B) != 0) {
                        *out++ = 3;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x13C) != 0) {
                        *out++ = 5;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x140) != 0) {
                        *out++ = 7;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x138) != 0) {
                        *out++ = 6;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x132) != 0) {
                        if ((s8)M2C_FIELD(object, u8 *, 0x133) > 0) {
                            *out++ = 9;
                            n++;
                        }
                        if ((s8)M2C_FIELD(object, u8 *, 0x133) < 0) {
                            *out++ = 10;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x134) != 0) {
                        if ((s8)M2C_FIELD(object, u8 *, 0x135) > 0) {
                            *out++ = 11;
                            n++;
                        }
                        if ((s8)M2C_FIELD(object, u8 *, 0x135) < 0) {
                            *out++ = 12;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if (M2C_FIELD(object, u8 *, 0x136) != 0) {
                        if ((s8)M2C_FIELD(object, u8 *, 0x137) > 0) {
                            *out++ = 13;
                            n++;
                        }
                        if ((s8)M2C_FIELD(object, u8 *, 0x137) < 0) {
                            *out++ = 14;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if ((s8)M2C_FIELD(object, u8 *, 0x147) > 0) {
                        *out++ = 17;
                        n++;
                    }
                    if ((s8)M2C_FIELD(object, u8 *, 0x147) < 0) {
                        *out = 18;
                        n++;
                    }
                } while (0);

                if (n > 0) {
                    i = n;
                    out = sel;
                    q = tbl;
                    do {
                        Resource_LoadTableEntryToBuffer(*out, *q++);
                        i--;
                        out++;
                    } while (i != 0);
                }
                if (n == 0) {
                    sel[0] = 0;
                    n = 1;
                }
                if (n <= 10) {
                    for (i = n; i < 11; i++) {
                        sel[i] = -1;
                    }
                }
                cnt = n;
                if (sel[0] == 0) {
                    if (M2C_FIELD(object, s16 *, 0x38) != 0) {
                        code = (s32)&Value_000008bd;
                    } else {
                        code = (s32)&Value_000008be;
                    }
                    UiText_DrawCharacter(code, winMain, 112, 0);
                }
            }

            text = (u16 *)Runtime_BumpAllocate(0x100);
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && M2C_FIELD(object, s16 *, 0x38) == 0) {
                    iconIdx = 16;
                }

                saved = (u8 *)Runtime_BumpAllocate(0x14C);
                code = 0;
                Func_080072f0(saved, object, 0x14C, 0x03001388);

                hpDelta = M2C_FIELD(object, u16 *, 0x3C);
                ppDelta = M2C_FIELD(object, u16 *, 0x3E);
                spDelta = M2C_FIELD(object, u16 *, 0x40);
                M2C_FIELD(object, s8 *, 0x133) = 0;
                M2C_FIELD(object, s8 *, 0x135) = 0;
                M2C_FIELD(object, s8 *, 0x147) = 0;
                BattleUnit_Recalculate(owner);
                hpDelta -= M2C_FIELD(object, u16 *, 0x3C);
                ppDelta -= M2C_FIELD(object, u16 *, 0x3E);
                spDelta -= M2C_FIELD(object, u16 *, 0x40);
                Func_080072f0(object, saved, 0x14C, 0x03001388);
                Func_08002df0(saved);

                switch (iconIdx) {
                case 8:
                    code = M2C_FIELD(object, u8 *, 0x141);
                    break;
                case 9:
                    code = hpDelta;
                    break;
                case 10:
                    code = -hpDelta;
                    break;
                case 11:
                    code = ppDelta;
                    break;
                case 12:
                    code = -ppDelta;
                    break;
                case 17:
                    code = spDelta;
                    break;
                case 18:
                    code = -spDelta;
                    break;
                case 13:
                case 14:
                    code = M2C_FIELD(object, s8 *, 0x137) * 20;
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                Func_0801965c(iconIdx + (s32)&Value_000008d2, text, 0x80);
            } else if (cur.entry == 2 && object[15] <= 98) {
                UiWork_PushValueSlot(
                    Func_08077258(owner, object[15] + 1)
                        - M2C_FIELD(object, s32 *, 0x124),
                    5);
                Func_0801965c((s32)&Value_000008bf, text, 0x80);
            } else {
                Func_0801965c(cur.entry + (s32)&Value_000008c0, text, 0x80);
            }

            Func_08017aa4(text, winDesc, 0, 4);
            Func_08002df0(text);
            base[0xEA3] = 1;
            redraw = 0;
        }

        q = tbl;
        p = objIcon;
        colPixels = 112;
        for (i = 0; i <= 10; i++) {
            M2C_FIELD(p, u32 *, 4) = 0x40000400;
            M2C_FIELD(p, u32 *, 8) = 0;
            p->tile = SLOT_TABLE[*q++].tile;
            p->x = colPixels;
            p->y = (u8)(winMain->y * 8 + 8);
            if (sel[i] > 0) {
                Runtime_PushSlotEntry(p, 240);
            }
            colPixels += 15;
            p++;
        }

        if (SCREEN_WORK->running == 0) {
            break;
        }
        if ((SYSTEM_FLAGS & 2) != 0) {
            break;
        }

        if (listCount != 0) {
            if ((keys & 0x100) != 0) {
                idx++;
                if (idx >= listCount) {
                    idx = 0;
                }
                owner = list[idx];
                redraw = 1;
                Audio_PlayCue(0x6F);
            } else if ((keys & 0x200) != 0) {
                idx--;
                if (idx < 0) {
                    idx = listCount - 1;
                }
                owner = list[idx];
                redraw = 1;
                Audio_PlayCue(0x6F);
            }
        }
        WaitFrames(1);
    }

    q = tbl;
    for (i = 10; i >= 0; i--) {
        Resource_ResetEntry(*q++);
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    Func_0801f200(SESSION_WORK[65]);
    SCREEN_WORK->busy = 0;
    WaitFrames(1);
    return 0;
}
