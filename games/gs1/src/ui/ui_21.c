#include "far_runtime.h"
#include "types.h"
#include "scene.h"

/* ui/window/update_or_create.c */
s32 UiWindow_CreateFar(s32 first, s32 second, s32 third, s32 fourth, s32 flags);

s32 UiWindow_UpdateOrCreate(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags)
{
    s32 current;
    s32 masked;

    current = *handle;
    if (current != 0) {
        if (0x100 & flags) {
            return 0;
        }
        UiWindow_Commit(current);
        return 0;
    }
    masked = flags & 0xff;
    *handle = UiWindow_CreateFar(first, second, third, fourth, masked);
    return 1;
}

/* ui/window/close_if_open.c */
s32 UiWindow_Close(void *handle);

void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL) {
        UiWindow_Close(*handle);
        *handle = NULL;
    }
}

/* ui/text/draw/draw_number_right_aligned_far.c */
s32 FixedPoint_Ratio(s32 value, s32 divisor);

void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y)
{
    s32 value = number;
    s32 digits = 1;

    while (digits <= 15) {
        value = FixedPoint_Ratio(value, 10);
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    x -= digits << 3;
    Ui_SetRange(number, digits, layer, x, y);
}

/* ui/render/draw_value_pair_rows.c */
s32 UiPalette_SetColor(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

    Ui_SetMode(0x080af210, layer, 0, 40);
    Ui_SetMode2(0x080af214, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    Ui_SetMode3(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiPalette_SetColor(4);
    }
    if (val == 0) {
        UiPalette_SetColor(2);
    }
    Ui_SetMode3(val, layer, 48, 40);
    UiPalette_SetColor(15);
    Ui_SetMode(0x080af218, layer, 0, 48);
    Ui_SetMode2(0x080af214, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    Ui_SetMode3(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    Ui_SetMode3(val, layer, 88, 48);
}

/* ui/text/draw/draw_stat_comparison.c */
/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    u32 i;
    s32 tmp2;
    s32 p;
    s32 tmp;
    s32 rec;

    p = alt;
    Ui_Run(0xb1c, work, 0, 32);
    Ui_unk2_3(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(p + 60) != *(u16 *)(base + 60)) {
        Ui_unk2_3(*(u16 *)(p + 60), 3, work, 64, 40);
        if (*(u16 *)(p + 60) > *(u16 *)(base + 60)) {
            Ui_unk3(work, 44, 36, 0);
        } else {
            Ui_unk3(work, 44, 36, 1);
        }
    }
    Ui_Run(0xb1d, work, 0, 48);
    Ui_unk2_3(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(p + 62) != *(u16 *)(base + 62)) {
        Ui_unk2_3(*(u16 *)(p + 62), 3, work, 64, 56);
        if (*(u16 *)(p + 62) > *(u16 *)(base + 62)) {
            Ui_unk3(work, 44, 52, 0);
        } else {
            Ui_unk3(work, 44, 52, 1);
        }
    }
    Ui_unk4((s32)gVal, work, 0, 64);
    Ui_unk2_3(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(p + 64) != *(u16 *)(base + 64)) {
        Ui_unk2_3(*(u16 *)(p + 64), 3, work, 64, 72);
        if (*(u16 *)(p + 64) > *(u16 *)(base + 64)) {
            Ui_unk3(work, 44, 68, 0);
        } else {
            Ui_unk3(work, 44, 68, 1);
        }
    }
    tmp = *(u16 *)(base + 64);
    tmp2 = p + 64;
}

/* ui/icon/create_with_resource.c */
extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData();
extern s32 UiIcon_DrawWithFlags();
extern u8 gRom[];
s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeSlot();
    if (entry_no != 0) {
        Resource_CopyData(entry_no, 0x80, gRom);
        result = UiIcon_DrawWithFlags(entry_no, 0x40000000, first, second, third);
    }
    return result;
}

/* ui/icon/create_with_resource_variant.c */
s32 Resource_CopyData(s32 entry_no, s32 mode, s32 data);
s32 Resource_FindFreeSlot(void);
s32 UiIcon_DrawWithFlags(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
extern u8 gRom[];
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Resource_FindFreeSlot();
  resource_mode = 0x80;
  if (slot != 0)
  {
    Resource_CopyData(slot, copy_mode = resource_mode, gRom);
    icon = UiIcon_DrawWithFlags(slot, 0x40000000, first, second, third);
  }
  return icon;
}

/* ui/icon/prepare_object.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void UiIcon_PrepareObject(void *object)
{
    if (object != NULL) {
        FIELD_AT_OFFSET(object, s8, 5) = 1;
        ((struct UiIconObject *) object)->value_16 = FIELD_AT_OFFSET(object, u16, 6);
        FIELD_AT_OFFSET(object, s8, 0x14) = FIELD_AT_OFFSET(object, u16, 8);
        FIELD_AT_OFFSET(object, s8, 0x17) = -0x3F & FIELD_AT_OFFSET(object, s8, 0x17);
        FIELD_AT_OFFSET(object, s8, 0x15) = -4 & FIELD_AT_OFFSET(object, s8, 0x15);
    }
}
