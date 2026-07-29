#include "types.h"

struct TransitionInput_080b88d0 {
    s16 selected_id;
    u8 unknown_02[6];
    s16 value;
    s16 comparison_id;
};

struct TransitionRequest_080b88d0 {
    s32 value;
    s32 mode;
    s32 selected_id;
    u8 unknown_0c[8];
    s32 count;
    u8 unknown_18[12];
    u16 ids[14];
};

struct PartyObject_080b88d0 {
    u8 unknown_00[0x38];
    s16 active_value;
};

struct TransitionState_080b88d0 {
    s32 blend_mode;
    s32 duration;
};

extern struct TransitionState_080b88d0 *Data_03001f00;

s32 Func_080b8808(s32 id);
void Func_080030f8(u32 frames);
void Func_08004458(void);
void **Func_080b7dd0(s32 id);
s32 Func_080b6b40(s32 group, u16 *ids);
void Func_08009080(void *object, s32 mode);
void Func_080c0f98(s32 id, s32 enabled);
void Func_08015130(s32 song);
struct PartyObject_080b88d0 *Func_08077008(s32 id);
void Func_080b7b6c(u16 *ids, s32 mode);
void Func_080c9010(struct TransitionRequest_080b88d0 *request);
void Func_080b6c90(void);
void Func_080c0cec(s32, s32, s32, s32);

/*
 * Run the party-side transition around a selected object, rebuilding the
 * active-ID list and passing a complete stack-backed request to the scene
 * routine between the fade-out and fade-in.
 */
s32 Func_080b88d0(struct TransitionInput_080b88d0 *input)
{
    struct TransitionRequest_080b88d0 request;
    u16 party_ids[14];
    s32 selected_id = input->selected_id;
    s32 comparison_id = input->comparison_id;
    s32 group_count;
    s32 id_base;
    s32 active_count = 0;
    s32 i;
    void *selected_object;
    volatile u16 *blend_control =
        (volatile u16 *)0x04000050;
    volatile u16 *blend_alpha =
        (volatile u16 *)0x04000052;

    if (Func_080b8808(selected_id) < 0 ||
        Func_080b8808(comparison_id) < 0)
        return -1;

    if (input->selected_id <= 4)
        Data_03001f00->blend_mode = 0x2000;
    else
        Data_03001f00->blend_mode = 0x5000;
    Data_03001f00->duration = 60;

    Func_080030f8(10);
    Func_08004458();
    selected_object = *Func_080b7dd0(selected_id);

    if ((u32)comparison_id <= 7) {
        group_count = Func_080b6b40(2, party_ids);
        id_base = 0x80;
    } else {
        group_count = Func_080b6b40(1, party_ids);
        id_base = 0;
    }

    for (i = 0; i < group_count; i++) {
        if (i + id_base == selected_id)
            Func_08009080(selected_object, 3);
    }

    Func_080030f8(30);
    *blend_control = 0x3f40;
    for (i = 0; i < group_count; i++)
        Func_080c0f98(i + id_base, 1);

    for (i = 0; i < 16; i++) {
        *blend_alpha = (u16)((16 - i) | 0x1000);
        Func_080030f8(1);
    }

    Func_08015130(9);

    if (comparison_id > 0x7f) {
        s32 expected = Func_080b6b40(2, party_ids);
        s32 candidate = 0;

        while (active_count != expected) {
            s32 id = candidate + 0x80;

            if (Func_08077008(id)->active_value > 0)
                party_ids[active_count++] = (u16)id;
            candidate++;
        }
    } else {
        s32 expected = Func_080b6b40(1, party_ids);
        s32 candidate = 0;

        while (active_count != expected) {
            if (Func_08077008(candidate)->active_value > 0)
                party_ids[active_count++] = (u16)candidate;
            candidate++;
        }
    }

    party_ids[active_count] = 0xff;
    Func_080b7b6c(party_ids, 0);

    request.value = input->value;
    request.selected_id = selected_id;
    for (i = 0; i < active_count; i++)
        request.ids[i] = party_ids[i];
    request.count = active_count;
    request.mode = (u32)comparison_id <= 7 ? 1 : 0;

    Func_080c9010(&request);
    Func_080030f8(10);
    Func_080b6c90();

    *blend_control = 0x3f40;
    for (i = 0; i < group_count; i++)
        Func_080c0f98(i + id_base, 1);

    for (i = 0; i < 16; i++) {
        *blend_alpha = (u16)(i | 0x1000);
        Func_080030f8(1);
    }

    for (i = 0; i < group_count; i++)
        Func_080c0f98(i + id_base, 0);

    Func_080c0cec(0, 0, 0, 100);
    Func_080030f8(3);
    return 0;
}
