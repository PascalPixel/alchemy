/* Draft, differ 2 of 130: The Broken Seal's MenuSelection_BuildEntries
 * unchanged, the screen being the heap slot menu_work. The native loads the
 * halfword 100 from the pool (ldr r3) before it builds the offset of base_x
 * (movs r1, #192); sched2 takes the offset first, the same one-slot load
 * order as this lane's other near misses. */
#include "types.h"
#include "ram_buffer.h"
#include "select.h"

struct SelectionNode *Resource_FindFreeTransferEntry(s32);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32);
void Menu_LoadSelectedResource(void);

void MenuSelection_BuildEntries(u32 unused)
{
	struct SelectionScreen *screen = Ram_HeapSlots->menu_work;
	u32 count = screen->count;
	u32 index = screen->top;
	struct SelectionNode *prev = 0;
	struct SelectionNode *node;
	s32 cnt = 0;

	while (index < count)
	{
		s32 base = screen->bases[index];
		u32 kind = screen->kinds[index];

		node = Resource_FindFreeTransferEntry(0);
		if (node == 0)
			break;
		MenuSelection_SetupEntry(kind, base, node, 0);
		if (screen->head == 0)
		{
			screen->head = node;
			node->prev = 0;
		}
		else
		{
			prev->next = node;
			node->prev = prev;
		}
		node->next = 0;
		cnt++;
		prev = node;
		if (cnt == 5)
			break;
		index++;
	}

	screen->base_x = 100 - cnt * 8;
	screen->base_y = 140;
	for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next)
	{
		s32 x = screen->base_x + cnt;
		s32 y;

		prev->x = x;
		y = screen->base_y;
		prev->y = y;
		prev->target_x = x;
		prev->target_y = y;
		if (prev->kind == 6 && screen->locked == 0)
		{
			prev->y = 6;
			prev->target_y = 6;
		}
		prev->dx = 0;
		prev->dy = 0;
		cnt += 16;
	}
	Menu_LoadSelectedResource();
}
