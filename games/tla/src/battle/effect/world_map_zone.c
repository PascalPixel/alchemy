#include "types.h"
#include "adventure.h"

/// A world map encounter rule, ended by kind -1.
struct TerrainRule {
  s16 kind;     ///< the terrain kind of the map block, 0 to 19
  s16 behavior; ///< the terrain's behavior, or -1 for any
  s16 flag;     ///< -1, or a flag the rule needs set
  s16 zone;
};

extern struct TerrainRule gData_080eedbc[];
extern struct TerrainRule gData_080eef34[];

s32 GetWorldMapTerrainBehaviorFar(void *position, s32 *kind);
s32 GameFlag_Test(s32 flag);
void Func_080ca514(s32 behavior);

/// Returns the world map encounter zone for the terrain at position (0 for
/// none) and chooses the battle backdrop for that terrain. There are no
/// encounters in render mode 8, render mode 7 reads the second rule table,
/// and a rule's flag must be set for it to apply.
// As ☀️'s function of the same name.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *position) {
  struct TerrainRule *rule = gData_080eedbc;
  s32 behavior;
  u32 kind;
  s32 zone;
  struct Adventure *party = &gAdventure;

  /* FAKEMATCH(empty asm): the barrier clobbering r1 loads gAdventure before
   * renderMode's offset is built in r1; as a plain declaration, 2 of 74
   * instructions differ in tla-en. */
  asm("" : "+l"(party) : : "r1");
  if (party->renderMode == 8)
    return 0;
  if (party->renderMode == 7)
    rule = gData_080eef34;
  behavior = GetWorldMapTerrainBehaviorFar(position, (s32 *)&kind);
  kind %= 20;
  zone = 0;
  for (; rule->kind != -1; rule++) {
    if (rule->kind != kind)
      continue;
    if (rule->behavior != -1 && rule->behavior != behavior)
      continue;
    if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
      continue;
    zone = rule->zone;
    break;
  }
  Func_080ca514(behavior);
  return zone;
}
