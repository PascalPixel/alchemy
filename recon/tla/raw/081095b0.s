.syntax unified
	.thumb
	.global Func_081095b0
	.thumb_func
Func_081095b0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r5, r0, #0
	adds r0, r7, #0
	adds r6, r2, #0
	bl Owner_GetState
	mov r8, r0
	cmp r5, #0
	beq .L_08109616
	adds r0, r5, #0
	bl RenderOutput_PrepareForRedrawFar
	adds r0, r7, #0
	adds r1, r6, #0
	bl PartyInventory_AddFar + 0x8
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_081095fe
	lsls r3, r0, #1
	adds r3, #216
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r1, #5
	lsrs r0, r0, #11
	adds r0, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0810961c
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_0810960a
.L_081095fe:
	ldr r0, .L_08109620
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_0810960a:
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	adds r3, r7, #0
	bl Func_0810bf98
.L_08109616:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0810961c:
	.4byte 0x0000123d
.L_08109620:
	.4byte 0x0000123c
