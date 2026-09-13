#include "TYPES.H"
#include "SCENE.H"
#include "RUNTIME_INTERFACES.H"

extern const u8 gRom[];

const u8 *Owner_GetRecord(s32 selector)
{
    u32 record_index;

    record_index = selector - 8;
    if (record_index > 0xF9U) {
        record_index = 0;
    }
    return gRom + record_index * 0x54;
}
