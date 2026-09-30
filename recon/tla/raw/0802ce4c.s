.syntax unified
	.thumb
	.global Func_0802ce4c
	.thumb_func
Func_0802ce4c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #4
	adds r4, r3, #0
	movs r2, #133
	movs r3, #128
	adds r5, r0, #0
	movs r6, #0
	mov r0, sp
	adds r4, #216
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r6, [r0]
	adds r3, #212
	adds r1, r4, #0
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r5]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0802ce8a
	str r5, [r4]
	str r5, [r4, #4]
	strh r6, [r4, #8]
	strh r6, [r4, #10]
	movs r6, #1
.L_0802ce8a:
	cmp r6, #0
	beq .L_0802ce98
	movs r1, #144
	ldr r0, .L_0802ce9c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_0802ce98:
	add sp, #4
	pop {r5, r6, pc}
.L_0802ce9c:
	.4byte Func_0802cd94
