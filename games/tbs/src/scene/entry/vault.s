@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Vault_RestoreEntryState
	overlay_veneer Vault_GetEntrances
	overlay_veneer Vault_GetExits
	overlay_veneer Vault_SelectPlacements
	overlay_veneer Vault_SelectEvents
	overlay_veneer SceneData_ReturnZero
