.syntax unified
	.thumb
	.global Func_0811c6cc
	.thumb_func
Func_0811c6cc:
	push {r5, r6, lr}
	movs r2, #0
	ldrsh r5, [r0, r2]
	adds r0, r5, #0
	bl Owner_GetState
	adds r6, r0, #0
	adds r0, r5, #0
	bl BattleObject_IsValidId
	cmp r0, #0
	bge .L_0811c6ea
	movs r0, #1
	negs r0, r0
	b .L_0811c708
.L_0811c6ea:
	movs r2, #56
	ldrsh r3, [r6, r2]
	movs r0, #0
	cmp r3, #0
	ble .L_0811c708
	bl UiWork_ClearValueNameTablesFar
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0811c70c
	bl UiText_ShowMessageAndWaitCoreFar
	movs r0, #0
.L_0811c708:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811c70c:
	.4byte 0x00000c62
