/* DRAFT: needs edition-local names. This owner reused the TBS body through a
 * cross-edition catalog that renamed TBS addresses to TLA addresses; that
 * catalog is gone. Before this compiles, every callee, datum and constant the
 * TBS body names must be defined by the TLA source at its own location, and
 * the TLA-only behaviour (second actor, five refrain turns, void event push)
 * spelled in TLA source rather than switched on by macros here. The owner
 * keeps the body's own name, Battle_ResolveTargetAction, in every edition;
 * each edition's link places it. */
#include "../../../../games/THE LOST AGE/INCLUDE/TYPES.H"
/* The TBS header
 * restates the same fixed-width typedefs under its own guard, and C89 rejects
 * a repeated typedef, so mark it as present. TBS's TYPES.H adds only VERSION.H
 * (a TBS edition selector), bool and NULL, none of which the battle owner
 * uses. */
#define ALCHEMY_TYPES_H
/* TLA must supply BATTLE_WORK from its own headers; the TBS-only IWRAM
 * accessor is unused. */
#define ALCHEMY_RUNTIME_1E74_H
#include "../../../../games/THE BROKEN SEAL/SRC/BATTLE/ACTION_RESOLVE_TARGET_ACTION.C"
