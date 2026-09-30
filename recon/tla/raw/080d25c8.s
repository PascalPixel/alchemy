.syntax unified
	.thumb
	.global Func_080d25c8
	.thumb_func
Func_080d25c8:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Party_CountActiveOwnersFar
	adds r0, r6, #0
	bl Func_080ad2f0
	cmp r0, #0
	bge .L_080d2604
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	ldr r5, .L_080d2608
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r5, #1
	adds r0, r6, #0
	movs r1, #2
	bl UiText_DrawQuantity
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r0, #1
	negs r0, r0
	b .L_080d2606
.L_080d2604:
	movs r0, #0
.L_080d2606:
	pop {r5, r6, pc}
.L_080d2608:
	.4byte 0x00000e26
