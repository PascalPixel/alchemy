.syntax unified
	.thumb
	.global Func_0811cbe8
	.thumb_func
Func_0811cbe8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	cmp r3, #0
	bne .L_0811cc06
	mov r2, sp
	movs r3, #0
	strb r3, [r2]
	strb r3, [r2, #1]
	strb r3, [r2, #2]
	strb r3, [r2, #3]
.L_0811cc06:
	adds r0, r5, #0
	bl Owner_GetState
	adds r6, r0, #0
	ldrh r3, [r6, #56]
	subs r3, r3, r7
	strh r3, [r6, #56]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_0811cc1e
	movs r3, #0
	strh r3, [r6, #56]
.L_0811cc1e:
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #5
	ldr r0, [r0]
	bl Object_SetMode
	movs r0, #0
	bl UiWindow_DrawPartyStatusContentsFar
	bl Func_08038118
	cmp r5, #7
	bhi .L_0811cc5e
	mov r2, r8
	cmp r2, #0
	beq .L_0811cc46
	ldr r0, .L_0811ccd8
	bl Func_080381c0 + 0x8
.L_0811cc46:
	adds r0, r7, #0
	movs r1, #5
	bl UiText_DrawQuantity
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0811ccdc
	bl Func_080381c0 + 0x8
	b .L_0811cc88
.L_0811cc5e:
	mov r3, r8
	cmp r3, #0
	beq .L_0811cc6a
	ldr r0, .L_0811cce0
	bl Func_080381c0 + 0x8
.L_0811cc6a:
	adds r0, r7, #0
	movs r1, #5
	bl UiText_DrawQuantity
	movs r1, #1
	adds r0, r5, #0
	bl UiText_DrawQuantity
	ldr r0, .L_0811cce4
	bl Func_080381c0 + 0x8
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
.L_0811cc88:
	adds r0, r5, #0
	bl Func_0811ccf0
	cmp r5, #7
	bhi .L_0811ccaa
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bgt .L_0811ccc0
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0811cce8
	bl Func_080381c0 + 0x8
	b .L_0811ccc0
.L_0811ccaa:
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bgt .L_0811ccc0
	adds r0, r5, #0
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_0811ccec
	bl Func_080381c0 + 0x8
.L_0811ccc0:
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #1
	ldr r0, [r0]
	bl Object_SetMode
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811ccd8:
	.4byte 0x00000c6f
.L_0811ccdc:
	.4byte 0x00000c73
.L_0811cce0:
	.4byte 0x00000c6e
.L_0811cce4:
	.4byte 0x00000c72
.L_0811cce8:
	.4byte 0x00000c71
.L_0811ccec:
	.4byte 0x00000c84
