#include "render_input.h"
#include "types.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define TABLE_ADDR 0x03001B20
#else
#define TABLE_ADDR 0x03001B10
#endif

void *RenderOutput_AcquireFree(void);
void Resource_ResetEntry(u32);
s32 RenderOutput_AppendToList(void *, void *);

struct TableEntry {
    u16 unused;
    u16 value;
};

struct RenderOutput *RenderOutput_Create(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 x;
    struct RenderOutput *output;
    s32 y;

    output = RenderOutput_AcquireFree();
    if (output == NULL) {
        Resource_ResetEntry((u32)arg0);
        return 0;
    }
    x = arg3 + (arg2->x * 8) + 8;
    y = arg4 + (arg2->y * 8) + 8;
    x &= 0x1ff;
    y &= 0xff;
    /* Xをbit16～24、Yをbit0～7へ置き、arg1のフラグを重ねる。 */
    output->packed = (x << 16) | y | arg1;
    output->table.value =
        ((struct TableEntry *)TABLE_ADDR)[arg0].value >> 5;
    output->sentinel = 0xff;
    output->zero = 0;
    output->x = x;
    output->y = (s16)y;
    output->index = (s8)arg0;
    output->one4 = 1;
    output->one5 = 1;
    RenderOutput_AppendToList(arg2, output);
    return output;
}

s32 Func_0801bc34(s32 mode, s32 value);
void *RenderOutput_CreateFromResource(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;

    no = Func_0801bc34(arg0, arg1);
    if (no < 0) {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
}

s32 Func_08004080(void);
s32 Func_0801a2a4(s32 arg0, s32 arg1, s32 arg2);
void *RenderOutput_CreateLoaded(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;
    void *result;

    no = Func_08004080();
    result = NULL;
    if (no != 0x60) {
        Func_0801a2a4(arg0, arg1, no);
        result = RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
    }
    return result;
}

void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    u8 *result;

    count = Func_08004080();
    if (count == 0x60) {
        return NULL;
    }
    Func_08019ee4(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result[15] = 251;
    return result;
}

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination);
void *RenderOutput_CreateFromTable(
    s32 table_entry,
    struct RenderInput *input,
    s32 x,
    s32 y)
{
    s32 slot;
    void *output;

    slot = Func_08004080();
    output = NULL;
    if (slot != 0x60) {
        RenderResource_LoadTableEntry(table_entry, 0, (void *)slot);
        output = RenderOutput_Create(slot, 0x40000000, input, x, y);
    }
    return output;
}
