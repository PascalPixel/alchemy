.syntax unified
	.thumb
	.global Func_0803d3c0
	.thumb_func
Func_0803d3c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	sub sp, #8
	mov r10, r0
	mov r11, r1
	adds r6, r2, #0
	bl Func_0803d2f0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0803d3ee
	movs r0, #0
	b .L_0803d442
.L_0803d3ee:
	movs r3, #4
	negs r3, r3
	mov r9, r3
	ldrb r3, [r5, #4]
	mov r8, r9
	cmp r3, #0
	beq .L_0803d414
	movs r3, #2
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r3, #5
	adds r1, r7, #0
	movs r2, #6
	bl UiWindow_Create
	movs r3, #0
	adds r5, r0, #0
	mov r8, r3
	b .L_0803d426
.L_0803d414:
	movs r3, #2
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r7, #0
	movs r2, #5
	movs r3, #5
	bl UiWindow_Create
	adds r5, r0, #0
.L_0803d426:
	cmp r5, #0
	beq .L_0803d440
	mov r3, r8
	movs r2, #1
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	negs r2, r2
	mov r0, r10
	mov r1, r11
	adds r3, r5, #0
	bl Func_08042450
.L_0803d440:
	adds r0, r5, #0
.L_0803d442:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
