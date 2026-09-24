#include "TYPES.H"

/* The status panel beside the item and ability lists: the owner's name,
   ailments or level, then a page chosen by the low byte of mode (class and
   stats, the stat change of equipping the selected item, whether its
   Psynergy is already known, or the four base stats). Bit 8 redraws the
   page without the window, name and side portrait. */

struct PanelPsynergy {
    u16 id;
    u16 unk_02;
};

struct PanelOwner {
    u8 name[15];
    u8 level;
    u8 unk_10[0x48];
    struct PanelPsynergy psynergy[32];
    u16 inventory[15];
    u8 unk_f6[0x2e];
    s32 experience;
    u8 unk_128;
    u8 klass;
};

struct PanelItem {
    u8 unk_00[40];
    u16 psynergy;
};

struct PanelState {
    u8 unk_000[36];
    s32 window;
    u8 unk_028[0x154];
    s32 side_object;
    u8 unk_180[0xdc];
    s8 unequip;
};

extern struct PanelState *Data_03001f2c;
extern u8 Data_03001388[];
extern u8 Data_080af20c[];
extern u8 Value_00000741[], Value_00000333[];
extern u8 Value_00000bd6[], Value_00000bd7[], Value_00000bd8[], Value_00000bd9[];
extern u8 Value_00000b0e[], Value_00000b21[], Value_00000b22[], Value_00000b23[];
extern u8 Value_00000b1c[];

struct PanelOwner *Owner_GetStateFar(s32 owner);
struct PanelItem *Item_Get(s32 item);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void WaitFrames(s32 frames);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void CharacterMenu_BuildAvailability(u8 *out, s32 kind, s32 owner);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void RenderOutput_ClearListFar(s32 window);
s32 SideObject_CreateFar(s32 owner, s32 a, s32 b, s32 window, s32 c, s32 d);
void Ui_DrawValuePairRows(struct PanelOwner *unit, s32 window);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
void *Runtime_BumpAllocate(s32 size);
s32 Func_080072f0(void *, void *, s32, void *);
void Inventory_EquipFar(s32 owner, s32 slot);
void Owner_RecalculateStatsFar(s32 owner);
void UiText_DrawStatComparison(struct PanelOwner *unit, void *backup, s32 window);
void Runtime_BumpFree(void *block);

static inline void Owner_Copy(void *dst, void *src)
{
    Func_080072f0(dst, src, 0x14c, Data_03001388);
}

void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode)
{
    s32 created;
    struct PanelItem *def;
    u32 item;
    struct PanelState *state;
    struct PanelOwner *unit;
    s32 window;
    s32 cnt;
    s32 value;
    u8 avail[8];

    created = 0;
    state = Data_03001f2c;
    unit = Owner_GetStateFar(owner);
    item = unit->inventory[slot];
    def = Item_Get(item & 0x1ff);
    if (!(mode & 0x100))
        created = UiWindow_UpdateOrCreate(&state->window, 0, 5, 13, 12, 258);
    window = state->window;
    if (!(mode & 0x100)) {
        if (!created) {
            WaitFrames(1);
            UiWindow_ClearInteriorTilesFar(state->window, 0, 0, 88, 32);
        }
        UiText_DrawStringAtOffsetFar(unit, window, 32, 0);
        CharacterMenu_BuildAvailability(avail, 1, owner);
        cnt = 0;
        if (avail[1]) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000bd6, window, 32, cnt * 8 + 8);
            cnt++;
        }
        if (avail[2]) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000bd7, window, 32, cnt * 8 + 8);
            cnt++;
        }
        if (avail[3]) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000bd8, window, 32, cnt * 8 + 8);
            cnt++;
        }
        if (avail[4]) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000bd9, window, 32, cnt * 8 + 8);
            cnt++;
        }
        if (cnt < 2) {
            value = unit->level;
            UiText_DrawStringAtOffsetFar(Data_080af20c, window, 40, 16);
            UiText_DrawNumberInWindowFar(value, 4, window, 56, 16);
        }
    }
    if (!created) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(state->window, 0, 32, 88, 80);
    }
    RenderOutput_ClearListFar(window);
    if (!(mode & 0x100))
        state->side_object = SideObject_CreateFar(owner, 0, 0, window, 0, 0);
    switch (mode & 0xff) {
    case 0:
        value = unit->klass + (s32)Value_00000741;
        UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
        Ui_DrawValuePairRows(unit, window);
        value = unit->experience;
        UiText_DrawCharacterAtOffsetFar((s32)Value_00000b0e, window, 0, 64);
        UiText_DrawNumberInWindowFar(value, 8, window, 24, 72);
        break;
    case 6:
        value = unit->klass + (s32)Value_00000741;
        UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
        Ui_DrawValuePairRows(unit, window);
        break;
    case 2:
    case 3: {
        void *backup;

        if (!Item_CanOwnerEquip(owner, item)) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b21, window, 0, 48);
            break;
        }
        backup = Runtime_BumpAllocate(0x14c);
        Owner_Copy(backup, unit);
        if (state->unequip)
            unit->inventory[slot] &= 0xfdff;
        else
            Inventory_EquipFar(owner, slot);
        Owner_RecalculateStatsFar(owner);
        UiText_DrawStatComparison(unit, backup, window);
        Owner_Copy(unit, backup);
        Runtime_BumpFree(backup);
        break;
    }
    case 4: {
        s32 id = def->psynergy;
        s32 found = 0;
        s32 i;

        for (i = 0; i < 32; i++) {
            if ((unit->psynergy[i].id & 0x3fff) == id) {
                found = 1;
                break;
            }
        }
        if (found) {
            UiText_DrawCharacterAtOffsetFar(id + (s32)Value_00000333, window, 0, 48);
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b23, window, 0, 56);
        } else {
            UiText_DrawCharacterAtOffsetFar(id + (s32)Value_00000333, window, 0, 48);
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b22, window, 0, 56);
        }
        break;
    }
    case 8: {
        u8 *base = Value_00000b1c;

        UiText_DrawCharacterAtOffsetFar((s32)base, window, 0, 40);
        value = *(u16 *)((u8 *)unit + 60);
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 40);
        UiText_DrawCharacterAtOffsetFar((s32)base + 1, window, 0, 48);
        value = *(u16 *)((u8 *)unit + 62);
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 48);
        UiText_DrawCharacterAtOffsetFar((s32)base + 4, window, 0, 56);
        value = *(u16 *)((u8 *)unit + 64);
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 56);
        UiText_DrawCharacterAtOffsetFar((s32)base + 3, window, 0, 64);
        value = *((u8 *)unit + 66);
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 64);
        break;
    }
    }
}
