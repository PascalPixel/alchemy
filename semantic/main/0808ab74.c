#include "layout_guard.h"
#include "types.h"

struct MapDescriptor_0808ab74 {
    s16 resource;
    s8 flag_group;
    s8 map_mode;
    u16 display_resource;
    u16 unknown_06;
};

struct MapLoadState_0808ab74 {
    u8 unknown_000[0x1c0];
    s16 map_id;
    s16 map_variant;
    u8 unknown_1c4[0x08];
    s16 previous_flag_group;
    u8 unknown_1ce[0x50];
    s16 fallback_sequence;
    u8 unknown_220[0x12];
    s16 field_232;
    u8 unknown_234[0x04];
    s32 field_238;
    u8 unknown_23c[0x02];
    s16 map_mode;
    u16 saved_map_id;
    u16 saved_map_variant;
};

LAYOUT_SIZE_GUARD(
    MapDescriptor0808ab74_Size,
    struct MapDescriptor_0808ab74,
    8);
LAYOUT_SIZE_GUARD(
    MapLoadState0808ab74_Size,
    struct MapLoadState_0808ab74,
    0x244);

extern struct MapLoadState_0808ab74 Data_02000240;
extern const struct MapDescriptor_0808ab74 Data_0809f1a8[];

void Func_080770c8(s32 flag);
void Func_080770d0(s32 flag);
void Func_080772f0(void);

/*
 * Rebuild the global flag set for the current map.  A resumed map load keeps
 * the broad flag ranges intact, while a fresh load clears transient flags and
 * resets state that cannot cross between map flag groups.
 */
void Func_0808ab74(s16 mapId, s32 preserveMapFlags)
{
    struct MapLoadState_0808ab74 *state = &Data_02000240;
    s8 flagGroup = Data_0809f1a8[state->map_id].flag_group;
    s32 flag;
    s8 mapMode;

    (void)mapId;

    if (preserveMapFlags == 0) {
        for (flag = 0x200; flag <= 0x2ff; flag++)
            Func_080770d0(flag);

        if (flagGroup != state->previous_flag_group) {
            for (flag = 0x300; flag <= 0x3ff; flag++)
                Func_080770d0(flag);

            Func_080770c8(0x12f);
            state->field_238 = 0;
            state->field_232 = 0;
            Func_080770d0(0x110);
            Func_080770d0(0x111);
            Func_080770d0(0x112);
            Func_080770d0(0x113);
            state->saved_map_id = (u16)state->map_id;
            state->saved_map_variant = (u16)state->map_variant;
        }

        for (flag = 0x80; flag <= 0xdf; flag++)
            Func_080770d0(flag);

        Func_080770d0(0x16c);
        Func_080770d0(0x144);
        Func_080770d0(0x161);
        Func_080770d0(0x123);
        Func_080770d0(0x11c);
        state->fallback_sequence = -1;
    }

    state->previous_flag_group = flagGroup;
    Func_080770c8((flagGroup & 0x7f) + 0x180);

    mapMode = Data_0809f1a8[state->map_id].map_mode;
    state->map_mode = mapMode;
    if (mapMode == 2)
        Func_080770c8(0x123);

    Func_080772f0();
}
