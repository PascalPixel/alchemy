/// The palette cycle PaletteCycle_Step runs:
/// BG palette CYCLE_PALETTE's colours from CYCLE_FIRST_COLOR take their places
/// among CYCLE_PLACES one further every CYCLE_STEP_FRAMES frames. Each step
/// writes all but the last place, so one of the five colours is never shown.
#define CYCLE_PALETTE 12
#define CYCLE_FIRST_COLOR 2
#define CYCLE_PLACES 5
#define CYCLE_STEP_FRAMES 4
