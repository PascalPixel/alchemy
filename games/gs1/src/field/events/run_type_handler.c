#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080984c0(void);
void MapEvent_RunTileTriggerSequence(void);
void FieldEvent_ShowStatusMessage(void);

void FieldEvent_RunTypeHandler(void) {
    u32 type;

    type = (s16) FIELD_AT_OFFSET(*(void **)ADDR_03001F30, s16 *, 0x1E);
    switch (type) {
    case 8:
        Func_080984c0();
        return;
    case 10:
        MapEvent_RunTileTriggerSequence();
        return;
    case 16:
        FieldEvent_ShowStatusMessage();
        return;
    }
}
