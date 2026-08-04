#include "layout_guard.h"
#include "types.h"

typedef struct ActorScale_0808a6e4 {
    u8 padding00[0x14];
    s16 horizontal_scale;
    s16 vertical_scale;
    u8 padding18[0x1c];
    s16 horizontal_divisor;
    s16 vertical_divisor;
    s16 horizontal_factor;
    s16 vertical_factor;
} ActorScale_0808a6e4;

typedef struct MapTransitionState_0808a6e4 {
    u8 padding000[0x1c0];
    s16 current_map;
    s16 current_variant;
    s16 alternate_map;
    s16 alternate_variant;
    s16 fallback_map;
    s16 fallback_variant;
    u8 padding1cc[2];
    s16 requested_map;
    s16 requested_variant;
    s16 default_map;
    s16 default_variant;
    u8 padding1d6[0x1e];
    s32 current_actor;
    u8 padding1f8[0x3e];
    s16 requested_selection;
} MapTransitionState_0808a6e4;

LAYOUT_OFFSET_GUARD(
    ActorScale0808a6e4_HorizontalScale,
    ActorScale_0808a6e4,
    horizontal_scale,
    0x14);
LAYOUT_OFFSET_GUARD(
    ActorScale0808a6e4_HorizontalDivisor,
    ActorScale_0808a6e4,
    horizontal_divisor,
    0x34);
LAYOUT_OFFSET_GUARD(
    MapTransitionState0808a6e4_CurrentMap,
    MapTransitionState_0808a6e4,
    current_map,
    0x1c0);
LAYOUT_OFFSET_GUARD(
    MapTransitionState0808a6e4_RequestedMap,
    MapTransitionState_0808a6e4,
    requested_map,
    0x1ce);
LAYOUT_OFFSET_GUARD(
    MapTransitionState0808a6e4_CurrentActor,
    MapTransitionState_0808a6e4,
    current_actor,
    0x1f4);
LAYOUT_OFFSET_GUARD(
    MapTransitionState0808a6e4_RequestedSelection,
    MapTransitionState_0808a6e4,
    requested_selection,
    0x236);

extern MapTransitionState_0808a6e4 Data_02000240;

s32 Func_080022ec(s32, s32);
ActorScale_0808a6e4 *Func_08077008(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);

static s16 ClampedScale_0808a6e4(s16 factor, s16 divisor)
{
    s32 scale = Func_080022ec(factor * 0x4000, divisor);

    if (scale < 0)
        scale = 0;
    else if (scale > 0x4000)
        scale = 0x4000;
    if (scale == 0 && factor != 0)
        scale = 1;
    return scale;
}

static void RefreshActorScale_0808a6e4(ActorScale_0808a6e4 *actor)
{
    actor->horizontal_scale = ClampedScale_0808a6e4(
        actor->horizontal_factor,
        actor->horizontal_divisor);
    actor->vertical_scale = ClampedScale_0808a6e4(
        actor->vertical_factor,
        actor->vertical_divisor);
}

/* Resolve a transition result into the shared next-map/variant pair. */
void Func_0808a6e4(s16 selection)
{
    MapTransitionState_0808a6e4 *state = &Data_02000240;
    s16 map;
    s16 variant;

    state->requested_selection = selection;

    if (selection != -1) {
        map = state->requested_map;
        variant = state->requested_variant;
        if (map == -1 && variant == -1) {
            state->current_map = state->fallback_map;
            state->current_variant = state->fallback_variant;
            Func_080770c8(0x109);
            return;
        }
    } else {
        ActorScale_0808a6e4 *actor =
            Func_08077008(state->current_actor);

        if (actor->horizontal_factor == 0) {
            actor->horizontal_factor = 1;
            RefreshActorScale_0808a6e4(actor);
        }

        if (Func_080770c0(0x20) != 0) {
            s32 actor_index;

            for (actor_index = 0; actor_index < 2; actor_index++) {
                actor = Func_08077008(actor_index);
                actor->horizontal_factor = actor->horizontal_divisor;
                actor->vertical_factor = actor->vertical_divisor;
                RefreshActorScale_0808a6e4(actor);
            }
        }

        map = state->default_map;
        variant = state->default_variant;
        if (map == -1 && variant == -1) {
            state->current_map = state->alternate_map;
            state->current_variant = state->alternate_variant;
            return;
        }
    }

    state->current_map = map != -1 ? map : state->fallback_map;
    state->current_variant =
        variant != -1 ? variant : state->fallback_variant;
}
