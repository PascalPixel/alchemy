.syntax unified
	.thumb
	.global Func_081969ac
	.thumb_func
Func_081969ac:
	push {r5, r6, r7, lr}
	sub sp, #16
	adds r7, r0, #0
	adds r6, r1, #0
	cmp r2, #0
	beq .L_081969f4
	add r5, sp, #4
.L_081969ba:
	movs r3, #0
	ldrsb r3, [r7, r3]
	adds r0, r5, #0
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #1
	ldrsb r3, [r7, r3]
	adds r1, r5, #0
	lsls r3, r3, #16
	str r3, [r5, #4]
	movs r3, #2
	ldrsb r3, [r7, r3]
	str r2, [sp, #0]
	lsls r3, r3, #16
	str r3, [r5, #8]
	bl Render_ProjectPoint
	ldr r3, [r5]
	ldr r2, [sp, #0]
	strh r3, [r6]
	ldr r3, [r5, #4]
	subs r2, #1
	strh r3, [r6, #2]
	ldr r3, [r5, #8]
	adds r7, #4
	strh r3, [r6, #4]
	adds r6, #8
	cmp r2, #0
	bne .L_081969ba
.L_081969f4:
	add sp, #16
	pop {r5, r6, r7, pc}
