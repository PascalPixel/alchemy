#include "overlay_object.h"

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

#define SetOverlayObjectRecordField1 Func_02000030

void SetOverlayObjectRecordField1(struct OverlayObject *object, s32 value) {
    struct OverlayObjectRecordFieldView *record;
    record = (struct OverlayObjectRecordFieldView *)object->record;
    record->field_01 = value;
}
