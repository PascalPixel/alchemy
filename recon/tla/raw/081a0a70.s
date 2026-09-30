.syntax unified
	.thumb
	.global Func_081a0a70
	.thumb_func
Func_081a0a70:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #128
	lsls r0, r0, #3
	sub sp, #4
	bl Runtime_BumpAllocateAlternatePool
	ldr r5, .L_081a0b90
	mov r4, sp
	movs r3, #0
	str r3, [r4]
	movs r3, #128
	lsls r3, r3, #19
	str r0, [r5]
	adds r3, #212
	adds r0, r4, #0
	ldr r1, .L_081a0b94
	ldr r2, .L_081a0b98
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081a0b9c
	movs r2, #133
	str r3, [r4]
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	ldr r1, .L_081a0ba0
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [r5]
	ldr r0, .L_081a0ba4
	movs r1, #192
	movs r6, #0
	lsls r1, r1, #2
.L_081a0abe:
	lsls r2, r6, #21
	adds r3, r5, #0
	orrs r2, r0
	stmia r3!, {r2}
	adds r6, #1
	str r1, [r3]
	adds r5, #8
	cmp r6, #7
	bls .L_081a0abe
	ldr r0, .L_081a0ba8
	movs r1, #192
	movs r6, #0
	lsls r1, r1, #2
.L_081a0ad8:
	lsls r2, r6, #21
	adds r3, r5, #0
	orrs r2, r0
	stmia r3!, {r2}
	adds r6, #1
	str r1, [r3]
	adds r5, #8
	cmp r6, #7
	bls .L_081a0ad8
	ldr r0, .L_081a0bac
	movs r1, #192
	movs r6, #0
	lsls r1, r1, #2
.L_081a0af2:
	lsls r2, r6, #21
	adds r3, r5, #0
	orrs r2, r0
	stmia r3!, {r2}
	adds r6, #1
	str r1, [r3]
	adds r5, #8
	cmp r6, #7
	bls .L_081a0af2
	movs r2, #16
	ldr r3, .L_081a0bb0
	mov lr, r2
	movs r2, #128
	lsls r2, r2, #14
	movs r6, #0
	movs r7, #0
	mov r10, r3
	mov r8, r2
.L_081a0b16:
	movs r0, #192
	adds r3, r7, r6
	movs r4, #0
	mov r12, lr
	lsls r0, r0, #13
	lsls r1, r3, #3
.L_081a0b22:
	mov r3, r12
	orrs r3, r0
	mov r2, r10
	orrs r3, r2
	adds r2, r5, #0
	stmia r2!, {r3}
	adds r4, #1
	str r1, [r2]
	adds r5, #8
	add r0, r8
	adds r1, #4
	cmp r4, #5
	bls .L_081a0b22
	movs r3, #8
	adds r6, #1
	add lr, r3
	adds r7, #2
	cmp r6, #15
	bls .L_081a0b16
	movs r1, #192
	lsls r1, r1, #16
	movs r2, #192
	movs r6, #0
	adds r1, #192
	lsls r2, r2, #2
.L_081a0b54:
	adds r3, r5, #0
	stmia r3!, {r1}
	adds r6, #1
	str r2, [r3]
	adds r5, #8
	cmp r6, #7
	bls .L_081a0b54
	ldr r2, .L_081a0b8c
	ldr r3, .L_081a0bb4
	movs r1, #200
	strh r2, [r3]
	ldr r3, .L_081a0bb8
	lsls r1, r1, #4
	strh r2, [r3]
	ldr r3, .L_081a0bbc
	ldr r0, .L_081a0bc0
	strh r2, [r3]
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	ldr r0, .L_081a0bc4
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r7, .L_081a0bc8
	movs r6, #0
	movs r5, #0
	b .L_081a0bcc
.L_081a0b8c:
	.4byte 0x00000000
.L_081a0b90:
	.4byte Data_02007510
.L_081a0b94:
	.4byte 0x06010000
.L_081a0b98:
	.4byte 0x85001800
.L_081a0b9c:
	.4byte 0x11111111
.L_081a0ba0:
	.4byte 0x06016000
.L_081a0ba4:
	.4byte 0x80004000
.L_081a0ba8:
	.4byte 0x80004088
.L_081a0bac:
	.4byte 0x40004098
.L_081a0bb0:
	.4byte 0x40004000
.L_081a0bb4:
	.4byte Data_02007504
.L_081a0bb8:
	.4byte Data_0200750c
.L_081a0bbc:
	.4byte Data_02007508
.L_081a0bc0:
	.4byte Func_081a08c4
.L_081a0bc4:
	.4byte Func_081a0a00
.L_081a0bc8:
	.4byte Data_081a20bc
.L_081a0bcc:
	adds r1, r5, #0
	ldr r0, [r7]
	movs r2, #1
	adds r6, #1
	bl Func_081a101c
	adds r5, #24
	cmp r6, #31
	bls .L_081a0bcc
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
