#include "layout_guard.h"
#include "types.h"

struct BattleMenu_080aa56c {
    u8 unknown_000[0x1c];
    u8 primary_index;
    u8 secondary_index;
    u8 unknown_01e[0xee];
    s32 window;
    u8 unknown_110[0x64];
    u16 chosen_entry;
    u16 encoded_selection;
    u16 selected_kind;
    u8 unknown_17a[0x0a];
    u8 *database;
    u8 unknown_188[0x80];
    u16 member_ids[8];
    u8 has_secondary;
    u8 line_count;
};

struct UiState_080aa56c {
    u8 unknown_00[4];
    u16 menu_active;
};

LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_Window,
    struct BattleMenu_080aa56c,
    window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_ChosenEntry,
    struct BattleMenu_080aa56c,
    chosen_entry,
    0x174);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_SelectedKind,
    struct BattleMenu_080aa56c,
    selected_kind,
    0x178);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_Database,
    struct BattleMenu_080aa56c,
    database,
    0x184);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_MemberIds,
    struct BattleMenu_080aa56c,
    member_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    BattleMenu080aa56c_LineCount,
    struct BattleMenu_080aa56c,
    line_count,
    0x219);

typedef void (*Transfer_080aa56c)(
    void *destination,
    const void *source,
    s32 size);

void Func_08002dd8(s32);
void Func_08002df0(void *);
void Func_080030f8(s32);
struct BattleMenu_080aa56c *Func_080048b0(s32, s32);
u8 *Func_08004970(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_080152a8(void);
void Func_080153e0(s32);
void Func_08015408(s32, s32, s32, s32);
void Func_08015410(s32, s32, s32, s32);
void Func_08015418(s32);
s32 Func_080770c0(s32);
s8 Func_08077158(void *);
void Func_0808a548(void);
void Func_080a1050(void);
void Func_080a1070(void);
void Func_080a1090(s32);
void Func_080a2144(s32);
void Func_080a3354(s32, s32, s32, s32);
void Func_080a34c0(void);
void Func_080aa544(s32);
s32 Func_080aa768(void);
void Func_080ad508(s32, s32);
void Func_080ad658(void);
void Func_080ae88c(void);
void Func_080ae8dc(void);

s32 Func_080aa56c(void)
{
    struct BattleMenu_080aa56c *menu = Func_080048b0(55, 0xa70);
    struct UiState_080aa56c *ui =
        *(struct UiState_080aa56c **)0x03001e68;
    Transfer_080aa56c transfer =
        (Transfer_080aa56c)0x03001388;
    u8 *database;
    u8 saved_mode = *(u8 *)0x0200044c;
    s32 initial_selection;

    *(u8 *)0x0200044c = 2;
    ui->menu_active = 1;
    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080a1090(0);

    database = Func_08004970(0x2130);
    menu->database = database;
    *(s32 *)(database + 0x2128) = 0;
    *(s32 *)(database + 0x212c) = 0;

    if (Func_080770c0(0x16e) != 0) {
        if (Func_080770c0(0x16f) == 0) {
            if (Func_080770c0(0x171) == 0) {
                initial_selection = 1;
            } else {
                initial_selection = 14;
            }
        } else if (Func_080770c0(0x171) == 0) {
            initial_selection = 27;
        } else {
            initial_selection = 28;
        }
        *(s32 *)(database + 0x212c) = initial_selection;
    }

    Func_080a1070();
    Func_080153e0(1);
    Func_08015418(0x06002500);
    menu->line_count = Func_08077158(menu->member_ids);
    Func_080ae88c();
    Func_080a3354(0, 3, 0, 7);
    Func_080aa544(0);
    Func_080a2144(14);

    menu->window = Func_08015010(13, 0, 17, 5, 2);
    menu->selected_kind = 0xff;
    menu->primary_index = 0;
    menu->secondary_index = 0;
    menu->chosen_entry = 0;
    menu->encoded_selection = 0;
    Func_080ad508(menu->window, 0);
    Func_080aa768();
    Func_080ad658();
    Func_080ae8dc();
    Func_080030f8(1);
    Func_080a34c0();

    Func_08015408(0, 0, 30, 20);
    ui->menu_active = 0;
    Func_080152a8();
    Func_080153e0(0);
    transfer((void *)0x06004000, database + 0xa8, 0x2000);
    transfer((void *)0x05000080, database + 0x20a8, 0x80);
    Func_080030f8(1);
    Func_080a1050();
    Func_08015410(0, 0, 30, 20);
    Func_08002df0(menu->database);
    Func_08002dd8(55);
    Func_0808a548();
    *(u8 *)0x0200044c = saved_mode;
    return 1;
}
