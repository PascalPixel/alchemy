/// @file
/// The visual_attributes object, its parts in visual_attributes/ in ROM order,
/// ending with the event commands that follow it: the effect scene, Mind Read
/// to Cloak, the rest of Cloak and Carry. visual_attributes_internal.h holds
/// the includes every part shares.

#include "visual_attributes_internal.h"

// The parts, in ROM order.
#include "visual_attributes/actor_commands.c"
#include "visual_attributes/messages.c"
#include "visual_attributes/camera.c"
#include "visual_attributes/view.c"
#include "visual_attributes/movement_types.c"
#include "visual_attributes/exit_animations.c"
#include "visual_attributes/line_scroll.c"
#include "visual_attributes/weather.c"
#include "visual_attributes/ground_particles.c"
#include "visual_attributes/motes.c"
#include "visual_attributes/djinni.c"
#include "visual_attributes/jupiter.c"
#include "visual_attributes/mars.c"
#include "visual_attributes/venus.c"
#include "visual_attributes/djinni_2.c"
#include "visual_attributes/mercury.c"
#include "visual_attributes/chains.c"
#include "visual_attributes/psynergy_dispatch.c"
#include "visual_attributes/field_psynergy.c"

#undef Actor_ShowEmoteById
#undef BattleEffect_ApplyColorToTargetBuffer
#undef Actor_ShowItemIconById
#undef FieldCamera_MoveToActor
#undef Actor_SetSpriteRotationById
#undef MapGrid_SetCellBehavior
#undef FieldPsynergy_SteerPushedTarget
#undef FieldPsynergy_ForceOnTarget
#undef FieldPsynergy_FrostOnTarget
#undef FieldPsynergy_DouseOnTarget
#undef Actor_ReleaseFar

#include "visual_attributes/scene.c"
#include "visual_attributes/mind_read.c"

#undef SE_CLOAK_END
#undef CLOAK_FLICKER_PALETTE

#undef MapGrid_CheckCollisionFar
#undef Actor_ApplyRandomChildValues
#undef EffectPosition
#undef EffectWork
#undef MapGrid_IsCloakCell

#include "visual_attributes/cloak.c"

#undef BattleEffect_GetCycledTableWord

#include "visual_attributes/carry.c"
