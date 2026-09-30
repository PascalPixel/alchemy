.syntax unified
	.thumb
	.global Func_080cf424
	.thumb_func
Func_080cf424:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r1
	mov r9, r0
	bl Func_080cdf5c
	bl ObjectTable_Get
	movs r1, #193
	adds r7, r0, #0
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	ldr r2, [r7, #12]
	movs r3, #144
	lsls r3, r3, #14
	mov r8, r0
	movs r0, #234
	adds r2, r2, r3
	adds r0, #255
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	bl Func_080200c0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080cf4d0
	ldr r5, [r6, #80]
	movs r3, #0
	ldrb r2, [r5, #5]
	strb r3, [r5, #26]
	strb r3, [r5, #27]
	subs r3, #33
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	mov r0, r9
	bl Func_08038248
	movs r2, #128
	lsls r2, r2, #3
	add r2, r8
	movs r1, #128
	ldrb r0, [r5, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080cf4ac
	ldr r3, .L_080cf4dc
	str r3, [r6, #108]
.L_080cf4ac:
	movs r3, #2
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080cf4bc
	adds r0, r6, #0
	bl Func_080cf350
.L_080cf4bc:
	movs r0, #80
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetMode
	adds r0, r6, #0
	bl Func_080200c8
.L_080cf4d0:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cf4dc:
	.4byte Func_080cf0d0
