/* Near miss, differ 2 of 69, one reordered pair: the native loads the menu before it starts the count's offset
 * 0x214 (ldr r6, [r3]; movs r2, #133); this form starts the constant first (sched2 gives movs priority 38, the
 * load 37). An empty asm or a one-pass block after the load pulls the two parameter copies before it (differ 4).
 * The permuter found nothing in five minutes (4,701 candidates). */
#include "types.h"
#include "ram_buffer.h"

// エナジーメニューの作業領域（わかっている欄だけ）
struct PsynergyMenuState {
    u8 unk000[0x24];
    s32 listWindow;
    u8 unk028[8];
    s32 infoWindow;
    u8 unk034[0x190];
    u16 psynergies[32];
    u8 unk204[0x10];
    u8 psynergyCount;
    u8 unk215;
    u8 paneOwner[2];
    u8 unk218[0x4c];
    s8 selectedIndexByOwner[8];
};

struct MenuResult {
    s32 ownerState;
    s32 unk04;
    s32 page;
    s32 pageCount;
    s32 row;
    s32 entryCount;
    s32 selectedIndex;
};

s32 UnitGet(s32 owner);

// index 番目の欄の持ち主のエナジーを 5 行ずつのページに割る
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 ownerState;
    struct PsynergyMenuState *menu = Ram_HeapSlots->menuRuntime;
    s32 entryCount;
    s32 row;
    s32 page;
    s32 pageCount;
    s32 selectedIndex;
    s32 owner;

    ownerState = UnitGet(menu->paneOwner[index]);
    entryCount = menu->psynergyCount;
    owner = menu->paneOwner[index];
    selectedIndex = menu->selectedIndexByOwner[owner];
    if ((selectedIndex + 1) > entryCount) {
        selectedIndex = entryCount - 1;
    }
    page = selectedIndex / 5;
    row = selectedIndex % 5;
    pageCount = entryCount / 5;
    if (entryCount % 5 != 0) {
        pageCount++;
    }
    result->ownerState = ownerState;
    result->page = page;
    result->pageCount = pageCount;
    result->row = row;
    result->entryCount = entryCount;
    result->selectedIndex = selectedIndex;
    return 1;
}
