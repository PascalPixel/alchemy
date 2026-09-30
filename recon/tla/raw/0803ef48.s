.syntax unified
	.thumb
	.global Func_0803ef48
	.thumb_func
Func_0803ef48:
	push {lr}
	movs r3, #210
	lsls r3, r3, #2
	adds r0, r0, r3
	sub sp, #12
	ldr r2, [r0]
	cmp r1, #0
	beq .L_0803ef60
.L_0803ef58:
	subs r1, #1
	ldr r2, [r2, #4]
	cmp r1, #0
	bne .L_0803ef58
.L_0803ef60:
	ldrh r3, [r2, #10]
	cmp r3, #1
	beq .L_0803ef6a
	cmp r3, #6
	bne .L_0803ef82
.L_0803ef6a:
	ldrh r0, [r2, #32]
	ldr r3, .L_0803ef88
	movs r1, #1
	subs r0, r0, r3
	ldrh r3, [r2, #12]
	str r1, [sp, #0]
	str r3, [sp, #8]
	add r2, sp, #8
	add r3, sp, #4
	movs r1, #0
	bl Ui_BuildPairedPatternsToSlot
.L_0803ef82:
	add sp, #12
	pop {pc}
	.2byte 0x0000
.L_0803ef88:
	.4byte 0x0000003a
