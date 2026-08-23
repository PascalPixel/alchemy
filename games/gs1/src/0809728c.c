#include "types.h"
#include "gs1_edition.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08096cdc(void *object, s32 mode, s32 value);

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

#define InitializeEventObject Func_0809728c
void Func_0809728c(void)
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
    Func_08009080(event_object, 0x14);
    M2C_FIELD(event_object, u32 *, 0x38) = (s32)*(s32 *)((u8 *)(event_object) + 8);
    M2C_FIELD(event_object, s32 *, 0x3C) = (s32)M2C_FIELD(event_object, s32 *, 0xC);
    M2C_FIELD(event_object, s32 *, 0x40) = (s32)M2C_FIELD(event_object, s32 *, 0x10);
    M2C_FIELD(event_object, u32 *, 0x24) = 0;
    M2C_FIELD(event_object, u32 *, 0x28) = 0;
    *(s32 *)((u8 *)(event_object) + 0x2C) = 0;
    if ((s8)M2C_FIELD(event_state, s8 *, 0x22) != 0) {
        Func_080f9010(212);
        M2C_FIELD(event_object, s32 *, 0x6C) = CALLBACK_1;
    }
    if ((s8)M2C_FIELD(event_state, s8 *, 0x23) != 0) {
        Func_08096cdc(event_object, 1, 0);
        Func_08015120((s32)event_index, 4);
        if ((s8)M2C_FIELD(event_state, s8 *, 0x21) != 0) {
            Func_08015040((void *)MESSAGE_NO, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        } else {
            Func_08015040((void *)MESSAGE_NO, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        }
        Func_08096cdc(event_object, 0, 0x10);
    }
    if (Func_080770c0(0x140) != 0) {
        if ((s8)M2C_FIELD(event_state, s8 *, 0x22) != 0) {
            M2C_FIELD(event_object, s32 *, 0x6C) = CALLBACK_2;
        }
        Func_08009080(event_object, 0x15);
    } else {
        Func_08097174();
    }
    M2C_FIELD(render_state, s8 *, 0xCC7) = 1;
}
