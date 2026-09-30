.syntax unified
	.thumb
	.global Func_081b834c
	.thumb_func
Func_081b834c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r2, #0
	mov lr, r2
	ldr r3, [r6, #4]
	ldr r2, [r6]
	sub sp, #8
	cmp r2, r3
	bne .L_081b8370
	ldr r3, [r6, #8]
	cmp r2, r3
	bne .L_081b8370
	ldr r3, [r6, #12]
	cmp r2, r3
	bne .L_081b8370
	movs r3, #5
	mov lr, r3
	b .L_081b83be
.L_081b8370:
	add r2, sp, #8
	movs r7, #0
	movs r3, #0
	mov r12, r2
.L_081b8378:
	adds r5, r3, #1
	adds r4, r5, #0
	cmp r5, #4
	beq .L_081b83a0
	lsls r3, r3, #2
	ldr r0, [r3, r6]
	mov r2, r12
	lsls r3, r5, #2
	adds r1, r3, r6
	subs r2, #8
.L_081b838c:
	ldmia r1!, {r3}
	cmp r0, r3
	bne .L_081b839a
	stmia r2!, {r0}
	movs r3, #4
	add r12, r3
	adds r7, #1
.L_081b839a:
	adds r4, #1
	cmp r4, #4
	bne .L_081b838c
.L_081b83a0:
	adds r3, r5, #0
	cmp r3, #3
	bne .L_081b8378
	cmp r7, #1
	bne .L_081b83ae
	movs r2, #2
	mov lr, r2
.L_081b83ae:
	cmp r7, #2
	bne .L_081b83b6
	movs r3, #3
	mov lr, r3
.L_081b83b6:
	cmp r7, #3
	bne .L_081b83be
	movs r2, #4
	mov lr, r2
.L_081b83be:
	mov r0, lr
	add sp, #8
	pop {r5, r6, r7, pc}
