/*
 * DjinnMenu_SelectDjinn (main:080ab5e4, 4888 bytes)
 *
 * Draft, not exact (2026-09-24): candidate=4868 reference=4888
 * differing_halfwords=2346.  Now compiles for scoring: the message ids are
 * Value_ link symbols (base 0xb98 for the menu block, 0xc40 for the
 * tutorial block) and the cursor split calls Math_ModU / Math_DivU, which is
 * what the reference calls.  Next: the reference reads the cursor once,
 * keeps x and y as u16 (both results are truncated with lsls/lsrs #16), and
 * clears the status bytes from sp+107 down to the buffer start; most of its
 * locals live in stack slots.
 */
#include "TYPES.H"
#include "TBS_EDITION.H"

/* Packed Djinn list entry: owner, element and number, bit 15 set = Set. */
#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)

struct DjinnMenuLists {
    u16 djinn[8][10];                 /* 0x000 */
    s8 counts[8];                     /* 0x0a0 */
    u8 unknown_0a8[0x2080];
    s32 tutorial_timer;               /* 0x2128 */
    s32 tutorial_step;                /* 0x212c */
};

struct DjinnMenuIcon {
    u8 unknown_00[5];
    u8 state;                         /* 0x05 */
};

struct DjinnMenuState {
    u8 unknown_000[0x10];
    s32 status_window;                /* 0x010 */
    struct DjinnMenuIcon *cursor_icon; /* 0x014 */
    u8 unknown_018[4];
    s8 column[2];                     /* 0x01c */
    u8 unknown_01e[0x12];
    s32 djinn_window;                 /* 0x030 */
    u8 unknown_034[0xd8];
    s32 message_window;               /* 0x10c */
    u8 unknown_110[0x64];
    u16 cursor[2];                    /* 0x174 */
    u16 djinn[2];                     /* 0x178 */
    u8 unknown_17c[8];
    struct DjinnMenuLists *lists;     /* 0x184 */
    u8 unknown_188[0x80];
    u16 owner_ids[8];                 /* 0x208 */
    u8 unknown_218;
    u8 party_count;                   /* 0x219 */
    u8 source_owner;                  /* 0x21a */
    u8 target_owner;                  /* 0x21b */
    u8 unknown_21c[0x38];
    u8 djinn_number[2];               /* 0x254 */
    u8 djinn_element[2];              /* 0x256 */
    u8 djinn_owner[2];                /* 0x258 */
};

struct DjinnMenuOwner {
    char name[15];                    /* 0x000 */
    u8 level;                         /* 0x00f */
    u8 unknown_010[0x119];
    u8 class_id;                      /* 0x129 */
};

/*
 * The menu and tutorial texts are linked message identities: every edition
 * numbers them differently, while their order inside each block is fixed.
 */
extern u8 Value_00000b98;
extern u8 Value_00000c40;

enum DjinnMenuMessage {
    DJINN_MSG_SET_ALL = 0x00,
    DJINN_MSG_ALL_STANDBY = 0x01,
    DJINN_MSG_CHOOSE = 0x02,
    DJINN_MSG_STANDBY_HELP = 0x04,
    DJINN_MSG_SET_HELP = 0x05,
    DJINN_MSG_RECOVERING = 0x06,
    DJINN_MSG_CHARACTER_STATUS = 0x11,
    DJINN_MSG_UNLEASH_EFFECT = 0x15,
    DJINN_MSG_GIVE = 0x16,
    DJINN_MSG_SET = 0x17,
    DJINN_MSG_STANDBY = 0x18,
    DJINN_MSG_TRADE = 0x19,
    DJINN_MSG_OWNER = 0x1a,
    DJINN_MSG_ABILITY = 0x1b,
    DJINN_MSG_WHAT_WILL_YOU_DO = 0x1c,
    DJINN_MSG_NOW_RECOVERING = 0x26
};

enum DjinnTutorialMessage {
    TUTORIAL_MSG_GIVE = 0x0,
    TUTORIAL_MSG_SET_AFTER_GIVE = 0x1,
    TUTORIAL_MSG_POWERS = 0x4,
    TUTORIAL_MSG_UNLEASH = 0x5,
    TUTORIAL_MSG_DEMONSTRATE = 0x6,
    TUTORIAL_MSG_RECOVERY = 0xc,
    TUTORIAL_MSG_SET_AGAIN = 0xd
};

#define DJINN_MESSAGE(n) ((s32)&Value_00000b98 + (n))
#define TUTORIAL_MESSAGE(n) ((s32)&Value_00000c40 + (n))

#define gDjinnMenu (*(struct DjinnMenuState **)ADDR_03001F2C)
#define gUiWork (*(u8 **)ADDR_03001E8C)
#define gKeysHeld (*(u32 *)ADDR_03001AE8)
#define gKeysShoulderLatch (*(u32 *)ADDR_03001AF8)
#define gKeysRepeat (*(u32 *)ADDR_03001B04)
#define gKeysPressed (*(u32 *)ADDR_03001C94)
extern const char Data_080af28c[];
extern u8 Data_02000240[];
#define gGameState Data_02000240

struct DjinnMenuOwner *Runtime_GetObject(s32 owner);
void BattleUnit_Recalculate(s32 owner);
s32 Func_08077210(s32 owner, s32 element, s32 number);
s32 Func_08077208(s32 owner, s32 element, s32 number);
s32 Func_080771b0(s32 owner, s32 element, s32 number);
s32 Func_080771b8(s32 owner, s32 element, s32 number);
void Func_080771c0(s32 owner, s32 element, s32 number);
void Func_080771c8(s32 owner, s32 element, s32 number);
void UiWindow_Commit(s32 window);
void Func_08015278(s32 window);
void Func_08015068(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const char *text, s32 window, s32 x, s32 y);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 flags);
s32 UiWork_Create(s32 message, s32 x, s32 y, s32 flags);
s32 UiWork_IsCompleteFar(void);
void UiWindow_Close(s32 work, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Func_080a1ac0(s32 x, s32 y);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
void Menu_DrawAtWindowOffset(s32 window, s32 x, s32 y, s32 width, s32 kind, s32 tile);
void UiWindow_ApplyRectAtObjectOrigin(s32 window, s32 x, s32 y, s32 width, s32 height, s32 kind);
void Func_080aafb8(struct DjinnMenuLists *lists);
s32 Djinn_MarkBalancedEntries(u8 *balanced, s32 self);
void FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
void FourObjectMotion_SetSlotPhase(s32 slot, s32 phase);
void FourObjectMotion_ReplaceSlot(s32 slot, s32 element, s32 kind);
void Func_080b50f8(void);

u32 Math_ModU(u32, u32);
u32 Math_DivU(u32, u32);

s32 DjinnMenu_SelectDjinn(s32 mode)
{
    s8 buf[8];
    u8 balanced[16];
    s8 *status;
    struct DjinnMenuState *state;
    struct DjinnMenuLists *lists;
    struct DjinnMenuOwner *owner;
    s32 window;
    s32 redraw;
    u32 refresh;
    s32 x;
    s32 y;
    s32 savedY;
    s32 sel;
    u32 djinn;
    s32 groupMode;
    s32 setAll;
    s32 i;
    s32 j;
    s32 ok;
    s32 result;
    s32 count;
    s32 message;
    u32 pressed;
    u32 repeat;
    s32 work;
    s32 step;

    state = gDjinnMenu;
    lists = state->lists;
    redraw = 1;
    x = Math_ModU(state->cursor[mode], 10);
    y = Math_DivU(state->cursor[mode], 10);
    status = buf;
    savedY = 0;
    djinn = 0;
    groupMode = 0;
    setAll = 0;
    sel = -1;
    for (i = 7; i >= 0; i--)
        status[i] = 0;

    if (mode == 0) {
        Func_080aafb8(lists);
        for (i = 0; i < state->party_count; i++) {
            if (lists->counts[i] == 0)
                status[i] = 4;
        }
        savedY = y;
        for (i = 0; i < state->party_count; i++) {
            if (status[x] == 4) {
                x++;
                x = Menu_GetModuloOfSum(x, state->party_count);
            }
        }
    } else {
        Djinn_MarkBalancedEntries(balanced, state->column[0]);
        for (i = 0; i < state->party_count; i++) {
            if (i == state->column[0]) {
                status[i] = 7;
            } else if (balanced[i] != 0) {
                status[i] = 0;
            } else {
                status[i] = 3;
                if (lists->counts[i] == 0)
                    status[i] |= 4;
            }
        }
    }

    if (mode == 1) {
        s32 fromX;
        s32 fromY;

        fromX = Math_ModU(state->cursor[0], 10);
        fromY = Math_DivU(state->cursor[0], 10);
        Menu_DrawAtWindowOffset(state->djinn_window, fromX * 7 + 1, fromY + 2, 6, 1, 14);
        UiWindow_ApplyRectAtObjectOrigin(state->djinn_window, fromX * 7 + 1, 2, 6, 7, 6);
        for (i = 0; i < state->party_count; i++) {
            if (i == state->column[0]) {
                if (state->djinn[0] & 0x8000)
                    message = DJINN_MESSAGE(DJINN_MSG_STANDBY);
                else
                    message = DJINN_MESSAGE(DJINN_MSG_SET);
            } else if (buf[i] & 2) {
                message = DJINN_MESSAGE(DJINN_MSG_GIVE);
            } else {
                message = DJINN_MESSAGE(DJINN_MSG_TRADE);
            }
            UiText_DrawAt(message, state->djinn_window, i * 56 + 8, 8);
        }
    }

    UiWindow_Commit(state->message_window);
    state->cursor_icon->state = 1;

    for (;;) {
        if (redraw != 0) {
            refresh = redraw;
            redraw = 0;
            sel = -1;
            if (!(buf[x] & 1))
                sel = y;
            window = state->status_window;
            owner = Runtime_GetObject(state->owner_ids[x]);
            UiWindow_Commit(window);
            UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
            UiText_DrawAt(owner->class_id + 0x741, window, 0, 8);
            UiText_DrawStringAtOffsetFar(Data_080af28c, window, 48, 0);
            UiNumber_DrawAt(owner->level, 2, window, 72, 0);
            if (mode == 0)
                UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_CHARACTER_STATUS), window, 0, 16);
            if (sel != -1)
                djinn = lists->djinn[x][sel];
            UiWindow_Commit(state->message_window);
            if (mode == 1) {
                UiText_DrawQuantity(state->source_owner, 1);
                UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_OWNER), state->message_window, 0, 0);
                UiText_DrawQuantity(DJINN_ELEMENT(state->djinn[0]) * 20 + DJINN_NUMBER(state->djinn[0]) + 300, 4);
                UiWindow_SetTilemapEntryFar(state->message_window, DJINN_ELEMENT(state->djinn[0]) + 0x5001, 6, 0, 0);
                UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_ABILITY), state->message_window, 56, 0);
                UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_WHAT_WILL_YOU_DO), state->message_window, 0, 8);
            }
            if (sel == -1) {
                FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
            } else if (mode == 0) {
                if (groupMode) {
                    if (setAll == 0)
                        UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_SET_ALL), state->message_window, 0, 0);
                    else
                        UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_ALL_STANDBY), state->message_window, 0, 0);
                    if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                        || Func_08077208(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        else
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_RECOVERING), state->message_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                } else {
                    UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_CHOOSE), state->message_window, 0, 0);
                    if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                        || Func_08077208(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                            UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_SET_HELP), state->message_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        } else {
                            UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_STANDBY_HELP), state->message_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        }
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_RECOVERING), state->message_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                }
            } else {
                if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                    || Func_08077208(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                    if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    else
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 0);
                } else {
                    FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 1);
                }
                if (refresh & 2)
                    FourObjectMotion_SetSlotPhase(mode, 0);
            }
            Func_08015278(state->djinn_window);
            if (sel != -1) {
                UiText_DrawAt(DJINN_MESSAGE(DJINN_MSG_UNLEASH_EFFECT), state->djinn_window, 0, 80);
                Func_08015068(state->djinn_window, 0, 96, 224, 104);
                UiText_DrawAt(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + 0x666, state->djinn_window, 0, 96);
            }
            if (!(buf[x] & 1))
                Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 14);
            gUiWork[RENDER_DIRTY_OFS] = 1;
        }

        if (buf[x] & 1)
            UiMenu_PositionCursor(x * 56 - 8, 52);
        else
            UiMenu_PositionCursor(x * 56 - 8, y * 8 + 60);
        WaitFrames(1);
        if (!(gKeysHeld & 0x100) || (gKeysShoulderLatch & 0x100)) {
            if (groupMode)
                redraw = 1;
            groupMode = 0;
            setAll = 0;
        }

        repeat = gKeysRepeat;
        pressed = gKeysPressed;
        step = lists->tutorial_step;
        if (step != 0) {
            lists->tutorial_timer++;
            pressed = 0;
            repeat = 0;
            switch (step) {
            case 28:
                while (!(gKeysPressed & 1)) {
                    UiMenu_PositionCursor(150, 26);
                    WaitFrames(1);
                }
                repeat = 2;
                pressed = 2;
                break;
            case 27:
                if (lists->tutorial_timer == 60) {
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_RECOVERY), 9, 9, 1);
                    gGameState[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    WaitFrames(1);
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_SET_AGAIN), 9, 9, 1);
                    gGameState[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    lists->tutorial_timer = 0;
                    Func_080b50f8();
                    Func_080b50f8();
                    Func_080b50f8();
                    Func_080771b8(0, 0, 0);
                    Func_080771c8(0, 0, 0);
                    BattleUnit_Recalculate(0);
                    repeat = 2;
                    pressed = 2;
                }
                break;
            case 1:
                if (lists->tutorial_timer == 60) {
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_GIVE), 9, 9, 1);
                    gGameState[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 2;
                }
                break;
            case 2:
            case 4:
            case 8:
            case 9:
            case 22:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    pressed = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 3:
                if (lists->tutorial_timer == 90) {
                    lists->tutorial_timer = 0;
                    repeat = 16;
                    pressed = 16;
                    lists->tutorial_step = 4;
                }
                break;
            case 6:
            case 7:
                if (lists->tutorial_timer == 60) {
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_SET_AFTER_GIVE), 9, 9, 1);
                    gGameState[0x20c] = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 8;
                }
                break;
            case 13:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                    repeat = 2;
                    pressed = 2;
                }
                break;
            case 14:
            case 16:
            case 17:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 15:
                if (lists->tutorial_timer == 60) {
                    gGameState[0x20c] = 1;
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_POWERS), 9, 9, 1);
                    Func_080a1ac0(2, 146);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeysPressed & 1)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    WaitFrames(1);
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_UNLEASH), 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeysPressed & 1)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    WaitFrames(1);
                    work = UiWork_Create(TUTORIAL_MESSAGE(TUTORIAL_MSG_DEMONSTRATE), 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeysPressed & 1)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWindow_Close(work, 1);
                    Func_080aafb8(lists);
                    WaitFrames(1);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 16;
                }
                break;
            case 18:
            case 20:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 21;
                    pressed = 1;
                }
                break;
            case 21:
                if (lists->tutorial_timer == 90) {
                    lists->tutorial_timer = 0;
                    repeat = 32;
                    pressed = 32;
                    lists->tutorial_step = 22;
                }
                break;
            case 24:
                if (lists->tutorial_timer == 60) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 25;
                    repeat = 2;
                    pressed = 2;
                }
                break;
            }
        }

        if (mode == 0 && (pressed & 0x100)) {
            if (sel == -1) {
                Audio_PlayCue(114);
                continue;
            }
            ok = 0;
            if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                || Func_08077208(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                ok = 1;
            groupMode = 1;
            gKeysShoulderLatch = 0;
            if (ok == 0) {
                Audio_PlayCue(114);
                Func_08015278(state->djinn_window);
                Func_08015068(state->djinn_window, 0, 80, 216, 104);
                UiText_DrawMessageAt(DJINN_MESSAGE(DJINN_MSG_NOW_RECOVERING), state->djinn_window, 0, 96);
                redraw = 1;
                continue;
            }
            if (djinn & 0x8000) {
                Audio_PlayCue(175);
                Func_080771b8(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Func_080771c8(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            } else {
                Audio_PlayCue(139);
                Func_080771b0(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Func_080771c0(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            }
            BattleUnit_Recalculate(DJINN_OWNER(djinn));
            Func_080aafb8(lists);
            redraw = 1;
        }
        if (mode == 0 && (pressed & 0x200)) {
            Audio_PlayCue(112);
            result = 7;
            break;
        }
        if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
            ok = 1;
            if (!(buf[x] & 1)) {
                if (sel == -1) {
                    Audio_PlayCue(114);
                    continue;
                }
                ok = 0;
                if (Func_08077210(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
                    || Func_08077208(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    ok = 1;
            }
            if (ok == 0) {
                Audio_PlayCue(114);
                Func_08015278(state->djinn_window);
                Func_08015068(state->djinn_window, 0, 80, 216, 104);
                UiText_DrawMessageAt(DJINN_MESSAGE(DJINN_MSG_NOW_RECOVERING), state->djinn_window, 0, 96);
                continue;
            }
            if (mode == 1) {
                if (!(buf[x] & 1))
                    result = 4;
                else if (x != state->column[0])
                    result = 3;
                else if (state->djinn[0] & 0x8000)
                    result = 2;
                else
                    result = 1;
            } else {
                result = 1;
            }
            Audio_PlayCue(112);
            break;
        }
        if (pressed & 8) {
            Audio_PlayCue(113);
            result = -2;
            break;
        }
        if (pressed & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (mode == 0 && (pressed & 4)) {
            if (groupMode) {
                setAll ^= 1;
                if (setAll)
                    Audio_PlayCue(139);
                else
                    Audio_PlayCue(175);
                for (i = 0; i < state->party_count; i++) {
                    for (j = 0; j < lists->counts[i]; j++) {
                        u32 entry = lists->djinn[i][j];
                        s32 usable;

                        usable = 0;
                        if (Func_08077210(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry))
                            || Func_08077208(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
                            usable = 1;
                        if (usable) {
                            if (setAll) {
                                if (!(entry & 0x8000)) {
                                    Func_080771b0(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    Func_080771c0(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    BattleUnit_Recalculate(DJINN_OWNER(entry));
                                }
                            } else if (entry & 0x8000) {
                                Func_080771b8(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                Func_080771c8(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                BattleUnit_Recalculate(DJINN_OWNER(entry));
                            }
                        }
                    }
                }
                Func_080aafb8(lists);
                redraw = 1;
            } else {
                Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 15);
                Audio_PlayCue(112);
                result = 10;
                break;
            }
        }

        if (repeat & 0x40) {
            Audio_PlayCue(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 15);
            if (buf[x] & 4)
                continue;
            if (buf[x] & 1) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
                redraw = 2;
                continue;
            }
            y--;
            count = lists->counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x80) {
            Audio_PlayCue(111);
            if (buf[x] & 4)
                continue;
            Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 15);
            y++;
            count = lists->counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            if ((buf[x] & 1) && !(buf[x] & 4)) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
            }
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x20) {
            Audio_PlayCue(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 15);
            x--;
            x = Menu_GetModuloOfSum(x, state->party_count);
            if (mode == 0) {
                for (i = 0; i < state->party_count; i++) {
                    if (buf[x] == 4) {
                        x--;
                        x = Menu_GetModuloOfSum(x, state->party_count);
                    }
                }
            }
        } else if (repeat & 0x10) {
            Audio_PlayCue(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->djinn_window, x * 7 + 1, y + 2, 6, 1, 15);
            x++;
            x = Menu_GetModuloOfSum(x, state->party_count);
            if (mode == 0) {
                for (i = 0; i < state->party_count; i++) {
                    if (buf[x] == 4) {
                        x++;
                        x = Menu_GetModuloOfSum(x, state->party_count);
                    }
                }
            }
        } else {
            continue;
        }
        y = savedY;
        count = lists->counts[x];
        if (count == 0)
            count = 1;
        y = Menu_GetModuloOfSum(y, count);
        redraw = 2;
    }

    state->column[mode] = x;
    if (sel != -1) {
        u16 entry = lists->djinn[x][sel];

        state->djinn[mode] = entry;
        state->djinn_number[mode] = DJINN_NUMBER(entry);
        state->djinn_element[mode] = DJINN_ELEMENT(entry);
        state->djinn_owner[mode] = DJINN_OWNER(entry);
    }
    state->cursor[mode] = x + y * 10;
    return result;
}
