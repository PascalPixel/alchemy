.syntax unified
	.thumb
	.global RenderOutput_Create
	.thumb_func
RenderOutput_Create:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	mov r10, r1
	mov r8, r2
	adds r6, r3, #0
	bl Func_08038eb0
	adds r5, r0, #0
	cmp r5, #0
	bne .L_08042338
	adds r0, r7, #0
	bl Resource_ResetEntry
	movs r0, #0
	b .L_08042390
.L_08042338:
	mov r2, r8
	movs r0, #14
	ldrsh r3, [r2, r0]
	movs r0, #12
	ldrsh r1, [r2, r0]
	ldr r2, [sp, #24]
	lsls r3, r3, #3
	adds r2, r2, r3
	lsls r1, r1, #3
	movs r3, #128
	adds r1, r6, r1
	lsls r3, r3, #1
	adds r3, #255
	adds r1, #8
	ands r1, r3
	adds r2, #8
	movs r3, #255
	ands r2, r3
	lsls r3, r1, #16
	orrs r3, r2
	mov r0, r10
	orrs r3, r0
	ldr r0, .L_08042398
	str r3, [r5, #20]
	lsls r3, r7, #2
	adds r3, r3, r0
	ldrh r3, [r3, #2]
	movs r0, #0
	lsrs r3, r3, #5
	str r3, [r5, #24]
	movs r3, #254
	strb r3, [r5, #15]
	movs r3, #1
	strh r1, [r5, #6]
	str r0, [r5]
	strh r2, [r5, #8]
	strb r7, [r5, #14]
	strb r3, [r5, #4]
	strb r3, [r5, #5]
	mov r0, r8
	adds r1, r5, #0
	bl RenderOutput_AppendToList
	adds r0, r5, #0
.L_08042390:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08042398:
	.4byte ResourceTableEntries
