@ Far-call stub table heading the window and text code at 0x08015000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08015000
	.thumb_func
Func_08015000:
	.irp target, 0x08015f31, 0x08016019, 0x080162d5, 0x08016419
	overlay_veneer \target
	.endr
	.irp target, 0x0801964d, 0x080196c5, 0x0801965d, 0x08017659
	overlay_veneer \target
	.endr
	.irp target, 0x0801776d, 0x08017365, 0x08017395, 0x08017621
	overlay_veneer \target
	.endr
	.irp target, 0x08016479, 0x080164d5, 0x0801e41d, 0x0801e74d
	overlay_veneer \target
	.endr
	.irp target, 0x0801e7c1, 0x0801e859
	overlay_veneer \target
	.endr
	.global UiText_DrawStringAtOffsetFar
	.global Func_08015090
	.thumb_func
UiText_DrawStringAtOffsetFar:
Func_08015090:
	.irp target, 0x0801e8b1, 0x0801e941, 0x0801e9a1
	overlay_veneer \target
	.endr
	.global UiText_DrawNumberAtOffsetFar
	.global Func_080150a8
	.thumb_func
UiText_DrawNumberAtOffsetFar:
Func_080150a8:
	.irp target, 0x0801e9d5, 0x0801ea09, 0x0801e71d, 0x0801e739
	overlay_veneer \target
	.endr
	.global RenderOutput_CreateFar
	.global Func_080150c8
	.thumb_func
RenderOutput_CreateFar:
Func_080150c8:
	.irp target, 0x0801eadd, 0x0801eb65, 0x0801ec6d, 0x0801ed41
	overlay_veneer \target
	.endr
	.irp target, 0x0801edcd, 0x0801eddd
	overlay_veneer \target
	.endr
	.global UiWindow_CreateWithSideObjectFar
	.global Func_080150f8
	.thumb_func
UiWindow_CreateWithSideObjectFar:
Func_080150f8:
	.irp target, 0x08019da9, 0x08019e49, 0x080187ad, 0x080187fd
	overlay_veneer \target
	.endr
	.irp target, 0x080198dd, 0x08019909, 0x0801ef09, 0x0801f201
	overlay_veneer \target
	.endr
	.irp target, 0x0801f5d5, 0x08019a55, 0x080197c5, 0x0801a66d
	overlay_veneer \target
	.endr
	.irp target, 0x0801a779, 0x0801a7c1, 0x0801a969, 0x0801a97d
	overlay_veneer \target
	.endr
	.irp target, 0x0801b011, 0x0801b149, 0x0801b229, 0x0801b399
	overlay_veneer \target
	.endr
	.irp target, 0x0801b425, 0x0801c0c9, 0x0801c0cd, 0x0801a7f5
	overlay_veneer \target
	.endr
	.irp target, 0x0801a90d
	overlay_veneer \target
	.endr
	.global Battle_CollectPartyCommandsFar
	.global Func_080151c0
	.thumb_func
Battle_CollectPartyCommandsFar:
Func_080151c0:
	.irp target, 0x08027115, 0x080175a1, 0x080174f9, 0x0801c0d9
	overlay_veneer \target
	.endr
	.global Func_080151e0
	.thumb_func
Func_080151e0:
	.irp target, 0x08019d2d, 0x0801c245, 0x080289e9, 0x08028c05
	overlay_veneer \target
	.endr
	.global Func_08015200
	.thumb_func
Func_08015200:
	.irp target, 0x0801c34d, 0x0801c429, 0x08019aa1
	overlay_veneer \target
	.endr
	.global UiWork_FinalizeSharedSlotFar
	.global Func_08015218
	.thumb_func
UiWork_FinalizeSharedSlotFar:
Func_08015218:
	.irp target, 0x080174d9, 0x08016759, 0x0801c0dd, 0x0801c155
	overlay_veneer \target
	.endr
	.irp target, 0x0801c17d, 0x0801c925, 0x0801c459
	overlay_veneer \target
	.endr
	.global ItemIcon_LoadTilesFar
	.global Func_08015250
	.thumb_func
ItemIcon_LoadTilesFar:
Func_08015250:
	.irp target, 0x0801a371, 0x08017aa5, 0x0801cf45
	overlay_veneer \target
	.endr
	.global Func_08015268
	.thumb_func
Func_08015268:
	.irp target, 0x0801eded, 0x08016499, 0x080164ad, 0x08019001
	overlay_veneer \target
	.endr
	.irp target, 0x08028f99, 0x080291e5, 0x0801bcd5, 0x0801c49d
	overlay_veneer \target
	.endr
	.global Func_080152a8
	.thumb_func
Func_080152a8:
	.irp target, 0x0801e319, 0x0801a089
	overlay_veneer \target
	.endr
	.global BattleLayout_HighlightPartyPanelsFar
	.global Func_080152b8
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
Func_080152b8:
	.irp target, 0x0802281d, 0x08019fcd, 0x0801a405
	overlay_veneer \target
	.endr
	.global Func_080152d0
	.thumb_func
Func_080152d0:
	.irp target, 0x0801eb91, 0x0801ec25, 0x0801f9b5, 0x080207c5
	overlay_veneer \target
	.endr
	.irp target, 0x080208e5, 0x0801fba9, 0x0801fc85, 0x0801faa9
	overlay_veneer \target
	.endr
	.irp target, 0x0801fa3d, 0x0801f77d, 0x08020bd9, 0x08020aed
	overlay_veneer \target
	.endr
	.irp target, 0x08020b01, 0x0801b1ed, 0x08029555, 0x0802977d
	overlay_veneer \target
	.endr
	.irp target, 0x0801c469
	overlay_veneer \target
	.endr
	.global Djinn_ShowJoinedMessageFar
	.global Func_08015358
	.thumb_func
Djinn_ShowJoinedMessageFar:
Func_08015358:
	.irp target, 0x08021229, 0x0801ccc1, 0x0801d4cd
	overlay_veneer \target
	.endr
	.global Func_08015370
	.thumb_func
Func_08015370:
	.irp target, 0x08029505, 0x0801ee69
	overlay_veneer \target
	.endr
	.global Func_08015380
	.thumb_func
Func_08015380:
	.irp target, 0x08028d75, 0x08028db5, 0x08028df5, 0x08028ea9
	overlay_veneer \target
	.endr
	.irp target, 0x0802106d, 0x0801a32d
	overlay_veneer \target
	.endr
	.global Menu_RunConfirmSelectionAtFar
	.global Func_080153b0
	.thumb_func
Menu_RunConfirmSelectionAtFar:
Func_080153b0:
	.irp target, 0x08028e55, 0x080209b1, 0x08018791, 0x08022769
	overlay_veneer \target
	.endr
	.irp target, 0x0801a3d1, 0x08021a19, 0x0801e3c9, 0x0802899d
	overlay_veneer \target
	.endr
	.global Func_080153f0
	.thumb_func
Func_080153f0:
	.irp target, 0x08021bc9, 0x080175c1, 0x0801f731
	overlay_veneer \target
	.endr
	.global UiWindow_DrawFrameFar
	.global Func_08015408
	.thumb_func
UiWindow_DrawFrameFar:
Func_08015408:
	.irp target, 0x080170f9, 0x08016179, 0x080219c9, 0x08021391
	overlay_veneer \target
	.endr
	.irp target, 0x08021489
	overlay_veneer \target
	.endr
