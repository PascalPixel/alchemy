/* Draft: Japanese debug command source alternative.
 * 2026-10-01: Compiled owner 50 bytes; argument scheduling differs from the 52-byte aligned Japanese owner.
 * This reduced attempt includes its current source declaration context.
 * Ordinary TBS compiler and options; no scene placement or credit claimed.
 */
/* The item and level debug room: its scene tables, actor 13's steps and the
 * record counts it adjusts. */
#include "../../../../games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/LEVEL.H"
#include "CALL.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "INVENTORY.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "ITEM.H"
#include "EDITION.H"

extern u8 MsgDebugWontStopYou[];
extern u8 MsgDebugRaiseEveryonesLevel[];
extern volatile u32 gKeyState;

extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_AudioPlayCue(s32 cue);
void Engine_TaskWait(s32 frames);
extern u8 gItemLevelItemPrompt[];
extern u8 gItemLevelItemHelp[];
extern u8 gItemLevelItemFull[];
extern volatile u32 gKeysRepeat;
void UiWork_Finalize(struct RenderInput *window, s32 mode);
void RenderOutput_RedrawSavedRect(struct RenderInput *window);
void Engine_DebugClearWindow(struct RenderInput *window);
void UiText_DrawStringInWindow(u8 *text, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 format, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 text, struct RenderInput *window, s32 x, s32 y);
void Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);
#define ITEM_COUNT 270
extern u8 MsgAbilityName[];
extern u8 MsgAbilityDescription[];
u8 *Engine_DebugGetAbility(s32 ability);
extern u8 gItemLevelPsyPrompt[];
extern u8 gItemLevelPsyHelp[];
void UiText_DrawResource(s32 text, struct RenderInput *window, s32 x, s32 y);
#define ABILITY_COUNT 270


void ItemLevel_RunMotionTest(void)
{
    /* FAKEMATCH: measured actor pin alternative changes argument scheduling. */
    register u32 actor asm("r0");
    s32 x;
    s32 z;

    Battle_Reset();
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorSetAnimation(11, 4);
    ObjectMotion_WaitForAnimationChange(10);
    x = 156;
    z = 208;
    actor = 11;
    /* FAKEMATCH: measured empty-constraint alternative changes compiler scheduling or register sharing. */
    asm("" : "+r"(x), "+r"(z) : "r"(actor));
    ObjectMotion_SetHorizontalPositionWithTerrain(actor, x << 17, z << 17);
    BattleFx_FinishAction();
}
