typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#define NULL ((void *)0)

/* Same node shape as semantic/main/0801ba68.c. */
struct SlidingNode_0801a7f4 {
    struct SlidingNode_0801a7f4 *previous;
    struct SlidingNode_0801a7f4 *next;
    u16 field_08;
    u16 field_0a;
    u16 resource;
    u16 field_0e;
    s16 x;
    s16 y;
    s16 motion_mode;
    u16 field_16;
    s16 target_x;
    s16 target_y;
    u8 field_1c[6];
    s16 motion;
    s16 step;
    s16 scale;
};

struct SlidingNode_0801a7f4 *Func_0801a910(s32);
void Func_0801bd98(s32, s32, struct SlidingNode_0801a7f4 *, s32);
void Func_0801c188(void);

/* Builds the sliding menu list for the current page: it walks the entry table
   from the saved scroll index, allocates up to five nodes, links them into the
   list at state+0x348, then lays the whole list out from the recomputed
   origin. */
void Func_0801a7f4(void)
{
    u8 *state;
    struct SlidingNode_0801a7f4 **head;
    struct SlidingNode_0801a7f4 *node;
    struct SlidingNode_0801a7f4 *previous;
    const u16 *entry;
    u16 *origin_x;
    u16 *origin_y;
    const u16 *suppress;
    u32 limit;
    u32 index;
    s32 count;
    s32 offset;
    s32 resource;
    s32 variant;
    s32 value;

    state = *(u8 **)0x03001e98;
    limit = *(u16 *)(state + 916);
    index = *(u16 *)(state + 924);
    previous = NULL;
    count = 0;
    entry = (const u16 *)(state + index * 2 + 852);
    goto test;

populate:
    Func_0801bd98(resource, variant, node, 0);
    head = (struct SlidingNode_0801a7f4 **)(state + 840);
    if (*head != NULL)
        goto link;
    *head = node;
    node->previous = NULL;
    goto linked;
link:
    previous->next = node;
    node->previous = previous;
linked:
    node->next = NULL;
    count++;
    previous = node;
    if (count == 5)
        goto laid_out;
    entry++;
    index++;

test:
    if (index >= limit)
        goto laid_out;
    variant = entry[16];
    resource = entry[0];
    node = Func_0801a910(0);
    if (node != NULL)
        goto populate;

laid_out:
    origin_x = (u16 *)(state + 918);
    origin_y = (u16 *)(state + 920);
    *origin_x = (u16)(100 - count * 8);
    *origin_y = 140;
    suppress = (const u16 *)(state + 952);
    node = *(struct SlidingNode_0801a7f4 **)(state + 840);
    offset = 0;
    if (node == NULL)
        goto done;

place:
    node->x = (s16)(*origin_x + offset);
    node->y = (s16)*origin_y;
    node->target_y = (s16)*origin_y;
    value = node->field_0a;
    node->target_x = (s16)(*origin_x + offset);
    if (value != 6)
        goto placed;
    if (*suppress != 0)
        goto placed;
    node->y = (s16)value;
    node->target_y = (s16)value;
placed:
    node->motion_mode = 0;
    node->field_16 = 0;
    node = node->next;
    offset += 16;
    if (node != NULL)
        goto place;

done:
    Func_0801c188();
}
