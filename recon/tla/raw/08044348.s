.syntax unified
	.thumb
	.global Func_08044348
	.thumb_func
Func_08044348:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r3, #192
	movs r0, #192
	lsls r3, r3, #18
	lsls r0, r0, #2
	ldr r5, [r3, #60]
	mov r8, r1
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	mov r1, r10
	mov r0, r8
	bl Resource_DecodeType01
	movs r1, #14
	ldrsh r3, [r6, r1]
	movs r1, #12
	ldrsh r2, [r6, r1]
	lsls r3, r3, #5
	adds r3, r3, r2
	ldr r2, .L_080443d4
	lsls r3, r3, #1
	ldrh r4, [r6, #10]
	adds r5, r5, r3
	adds r0, r3, r2
	movs r3, #0
	mov r12, r3
	mov r7, r10
	adds r5, #8
	cmp r12, r4
	bge .L_080443c4
	ldrh r2, [r6, #8]
	movs r1, #32
	mov lr, r1
.L_08044394:
	movs r1, #0
	cmp r1, r2
	bge .L_080443b2
.L_0804439a:
	movs r2, #0
	ldrsh r3, [r7, r2]
	adds r1, #1
	strh r3, [r0]
	strh r3, [r5]
	adds r7, #2
	ldrh r2, [r6, #8]
	adds r0, #2
	adds r5, #2
	cmp r1, r2
	blt .L_0804439a
	ldrh r4, [r6, #10]
.L_080443b2:
	mov r1, lr
	subs r3, r1, r2
	lsls r3, r3, #1
	adds r0, r0, r3
	adds r5, r5, r3
	movs r3, #1
	add r12, r3
	cmp r12, r4
	blt .L_08044394
.L_080443c4:
	mov r0, r10
	bl Sys_Free
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080443d4:
	.4byte 0x06002000
