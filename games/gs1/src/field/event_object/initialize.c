#include "types.h"
#include "gs1_edition.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ObjectGroup_SetActionForOthers(void *object, s32 mode, s32 value);

#if defined(GS1_EDITION_JA)
#define WORK_CELL_ADDR  0x03001F30
#define RENDER_CELL_ADDR 0x03001EBC
#define CALLBACK_1      0x0808DEFD
#define CALLBACK_2      0x0808DF39
#define MESSAGE_NO      0x94B
#elif defined(GS1_EDITION_DE)
#define WORK_CELL_ADDR  0x03001F40
#define RENDER_CELL_ADDR 0x03001ECC
#define CALLBACK_1      0x0809A5B1
#define CALLBACK_2      0x0809A5ED
#define MESSAGE_NO      0xA16
#elif defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR)
#define WORK_CELL_ADDR  0x03001F30
#define RENDER_CELL_ADDR 0x03001EBC
#define CALLBACK_1      0x0809BFA1
#define CALLBACK_2      0x0809BFDD
#define MESSAGE_NO      0xA16
#elif defined(GS1_EDITION_IT)
#define WORK_CELL_ADDR  0x03001F30
#define RENDER_CELL_ADDR 0x03001EBC
#define CALLBACK_1      0x08096FA1
#define CALLBACK_2      0x08096FDD
#define MESSAGE_NO      0xA16
#else
#define WORK_CELL_ADDR  0x03001F30
#define RENDER_CELL_ADDR 0x03001EBC
#define CALLBACK_1      0x08096F15
#define CALLBACK_2      0x08096F51
#define MESSAGE_NO      0x926
#endif

void InitializeEventObject(void)
{
    s32 zero;
    s32 event_value;
    s32 event_index;
    void *event_object;
    void *event_state;
    void *render_state;

    event_state = *(void **)WORK_CELL_ADDR;
    render_state = *(void **)RENDER_CELL_ADDR;
    zero = 0;
    event_object = *(void **)((u8 *)event_state + 0x10);
    event_value = (s32)(*(s16 *)((u8 *)(event_state) + 0x1C));
    event_index = event_value;
    Object_SetMode(event_object, 0x14);
    FIELD_AT_OFFSET(event_object, u32 *, 0x38) = (s32)*(s32 *)((u8 *)(event_object) + 8);
    FIELD_AT_OFFSET(event_object, s32 *, 0x3C) = (s32)FIELD_AT_OFFSET(event_object, s32 *, 0xC);
    FIELD_AT_OFFSET(event_object, s32 *, 0x40) = (s32)FIELD_AT_OFFSET(event_object, s32 *, 0x10);
    FIELD_AT_OFFSET(event_object, u32 *, 0x24) = 0;
    FIELD_AT_OFFSET(event_object, u32 *, 0x28) = 0;
    *(s32 *)((u8 *)(event_object) + 0x2C) = 0;
    if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x22) != 0) {
        Audio_PlayCue(212);
        FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_1;
    }
    if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x23) != 0) {
        ObjectGroup_SetActionForOthers(event_object, 1, 0);
        UiText_DrawQuantity((s32)event_index, 4);
        if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x21) != 0) {
            UiText_DrawMessage((void *)MESSAGE_NO, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        } else {
            UiText_DrawMessage((void *)MESSAGE_NO, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        }
        ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
    }
    if (GameFlag_IsSet(0x140) != 0) {
        if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x22) != 0) {
            FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_2;
        }
        Object_SetMode(event_object, 0x15);
    } else {
        EffectRuntime_StopCurrentObject();
    }
    FIELD_AT_OFFSET(render_state, s8 *, 0xCC7) = 1;
}
