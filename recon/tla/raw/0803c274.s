.syntax unified
	.thumb
	.global Func_0803c274
	.thumb_func
Func_0803c274:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	movs r3, #192
	adds r7, r0, #0
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	ldrh r3, [r7, #10]
	adds r5, r6, #0
	subs r3, #2
	sub sp, #4
	adds r0, r2, #0
	adds r5, #8
	cmp r4, r3
	bhi .L_0803c36c
	ldrh r3, [r7, #8]
	subs r3, #2
	cmp r0, r3
	bhi .L_0803c36c
	ldr r2, [sp, #24]
	cmp r2, #1
	bne .L_0803c344
	bl RenderOutput_AcquireFree
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0803c36c
	ldr r4, .L_0803c324
	ldr r2, .L_0803c328
	subs r3, r5, r6
	adds r3, r3, r4
	adds r1, r3, #0
	muls r1, r2
	movs r3, #2
	movs r2, #152
	strb r3, [r5, #5]
	lsls r2, r2, #5
	adds r2, #70
	adds r6, r6, r2
	ldrh r3, [r6]
	adds r4, r5, #0
	mov r8, r1
	adds r4, #16
	cmp r3, #99
	bne .L_0803c2da
	str r4, [sp, #0]
	bl Resource_FindFreeEntry
	strh r0, [r6]
	ldr r4, [sp, #0]
.L_0803c2da:
	ldrh r3, [r7, #8]
	movs r1, #255
	ldrh r2, [r7, #12]
	lsls r1, r1, #8
	adds r1, #254
	adds r3, r3, r1
	adds r2, r2, r3
	ldr r3, .L_0803c320
	lsls r2, r2, #3
	adds r2, #4
	ands r2, r3
	ldr r1, .L_0803c32c
	ldrh r3, [r4, #6]
	ands r1, r3
	orrs r1, r2
	ldrb r2, [r7, #10]
	ldrb r3, [r7, #14]
	adds r2, #254
	adds r3, r3, r2
	strh r1, [r4, #6]
	lsls r3, r3, #3
	lsls r1, r1, #23
	subs r3, #1
	lsrs r1, r1, #23
	strb r3, [r4, #4]
	strh r1, [r5, #6]
	mov r2, r8
	ldrb r3, [r4, #4]
	strb r2, [r5, #14]
	strh r3, [r5, #8]
	movs r3, #0
	str r3, [r5]
	ldrb r3, [r5, #5]
	b .L_0803c330
	.2byte 0x0000
.L_0803c320:
	.4byte 0x000001ff
.L_0803c324:
	.4byte 0xfffff8d0
.L_0803c328:
	.4byte 0xb6db6db7
.L_0803c32c:
	.4byte 0xfffffe00
.L_0803c330:
	cmp r3, #0
	bne .L_0803c33a
	add r3, sp, #24
	ldrb r3, [r3]
	strb r3, [r5, #5]
.L_0803c33a:
	adds r0, r7, #0
	adds r1, r5, #0
	bl RenderOutput_AppendToList
	b .L_0803c36c
.L_0803c344:
	cmp r1, #255
	bhi .L_0803c36c
	movs r3, #14
	ldrsh r2, [r7, r3]
	adds r4, #1
	adds r2, r2, r4
	movs r4, #12
	ldrsh r3, [r7, r4]
	adds r0, #1
	lsls r2, r2, #5
	adds r3, r3, r0
	adds r0, r2, r3
	movs r2, #160
	lsls r2, r2, #2
	cmp r0, r2
	bcs .L_0803c36c
	ldr r3, .L_0803c374
	lsls r2, r0, #1
	orrs r1, r3
	strh r1, [r5, r2]
.L_0803c36c:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0803c374:
	.4byte 0x0000f000
