#include "types.h"
#include "party_state.h"
#include "field_scene.h"

s32 GameFlag_Test(s32 flag);

extern struct SceneEntries gOverlayArea;

// The scene overlay's exit table: a header word holding a scene id, then that
// scene's exits, one packed word each (and a flag word after it when
// EXIT_HAS_FLAG is set). Scene id EXIT_TABLE_END ends the exits of a scene and
// the whole table.
#define EXIT_SCENE_MASK 0xfff
#define EXIT_ENTRANCE(word) (((word) & 0xff000) >> 12)
#define EXIT_KIND(word) (((word) & 0x0ff00000) >> 20)
#define EXIT_HAS_FLAG 0x10000000
#define EXIT_TABLE_END 0x1ff
#define EXIT_KIND_ANY 0xff
#define EXIT_NONE 999 // Field_RunScene's result when the scene was not left

// Chooses the scene and entrance to go to when the party leaves the current
// scene, as ☀️'s function of the same name: exitKind is the kind of exit
// Field_RunScene ended on, and the scene's exit table gives the first exit of
// that kind (or of any kind) whose flag is not set. ⚓️ also takes an exit
// whose flag word is -1. An exit of kind 0 or the end of the scene's exits
// falls back to the scene of its first exit.
void Scene_TakeExit(s32 exitKind) {
  s16 currentScene;
  const u32 *exitTable;
  const u32 *firstExit;
  u32 word;
  s32 scene;
  s32 nextScene;
  s32 nextEntrance;
  s32 entrance;
  s32 kind;
  s32 flag;

  currentScene = gPartyState.scene;
  exitTable = gOverlayArea.exits();
  nextScene = EXIT_NONE;
  nextEntrance = 0;
  if (exitKind == EXIT_NONE)
    return;

  for (;;) {
    // Find the header of the current scene: exit words have high bits set.
    scene = *exitTable++;
    if (scene & ~EXIT_SCENE_MASK)
      continue;
    scene &= EXIT_SCENE_MASK;
    if (scene == EXIT_TABLE_END)
      break;
    if (scene != currentScene)
      continue;

    firstExit = exitTable;
    for (;;) {
      word = *exitTable++;
      scene = word & EXIT_SCENE_MASK;
      entrance = EXIT_ENTRANCE(word);
      kind = EXIT_KIND(word);
      flag = word & EXIT_HAS_FLAG;
      if (flag)
        flag = *exitTable++;
      if (scene == EXIT_TABLE_END || kind == 0) {
        nextScene = *firstExit & 0x1ff; // only 9 bits, unlike EXIT_SCENE_MASK
        break;
      }
      if (kind == EXIT_KIND_ANY || kind == exitKind) {
        // As one condition the two flag tests fold into a range check.
        if (flag == 0)
          goto take;
        if (flag == -1)
          goto take;
        if (GameFlag_Test(flag))
          continue;
      take:
        nextScene = scene;
        nextEntrance = entrance;
        break;
      }
    }
    break;
  }

  // Commit the chosen exit only after a matching scene header was found.
  if (nextScene != EXIT_NONE) {
    gPartyState.scene = nextScene;
    gPartyState.entrance = nextEntrance;
  }
}
