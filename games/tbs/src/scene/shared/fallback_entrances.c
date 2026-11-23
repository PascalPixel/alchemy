/// @file
/// The entrances a cave or island scene file gives for a scene it does not
/// hold: one way in at (120, 152), facing south, with no camera bounds.
/// Crossbone Isle keeps the table but never returns it.

struct SceneEntrance gScene_FallbackEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};
