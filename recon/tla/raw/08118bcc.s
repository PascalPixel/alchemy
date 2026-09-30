.syntax unified
	.thumb
	.global Func_08118bcc
	.thumb_func
Func_08118bcc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #16
	adds r7, r0, #0
	mov r10, r3
	bl Func_08038118
	mov r6, sp
	adds r0, r6, #0
	bl BattleParty_ListPresentEnemies
	movs r5, #0
	cmp r7, #0
	beq .L_08118c1c
	subs r3, r7, #1
	mov r8, r3
.L_08118bf6:
	ldrh r0, [r6]
	movs r1, #1
	adds r6, #2
	bl UiText_DrawQuantity
	cmp r5, r8
	bne .L_08118c0c
	ldr r0, .L_08118c58
	bl Func_080381c0 + 0x8
	b .L_08118c12
.L_08118c0c:
	ldr r0, .L_08118c5c
	bl Func_080381c0 + 0x8
.L_08118c12:
	adds r5, #1
	bl BattlePresentation_WaitForAdvance
	cmp r5, r7
	bne .L_08118bf6
.L_08118c1c:
	bl Func_08038218
	mov r3, r10
	adds r3, #69
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_08118c3a
	bl Func_08038118
	ldr r0, .L_08118c60
	bl Func_080381c0 + 0x8
	bl BattlePresentation_WaitForAdvance
	b .L_08118c4c
.L_08118c3a:
	cmp r3, #2
	bne .L_08118c4c
	bl Func_08038118
	ldr r0, .L_08118c64
	bl Func_080381c0 + 0x8
	bl BattlePresentation_WaitForAdvance
.L_08118c4c:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08118c58:
	.4byte 0x00000c5d
.L_08118c5c:
	.4byte 0x00000c5c
.L_08118c60:
	.4byte 0x00000c5e
.L_08118c64:
	.4byte 0x00000c5f
