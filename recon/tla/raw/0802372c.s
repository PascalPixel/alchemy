.syntax unified
	.thumb
	.global Func_0802372c
	.thumb_func
Func_0802372c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	cmp r0, #0
	beq .L_080237c0
	movs r3, #0
	adds r2, r0, #0
	adds r2, #92
	mov r8, r3
	movs r3, #1
	strb r3, [r2]
	adds r3, r0, #0
	adds r3, #98
	strb r7, [r3]
	ldr r6, [r0, #80]
	cmp r6, #0
	beq .L_080237c0
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_08038248
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #16]
	bl VramBlock_LoadCached
	adds r5, r0, #0
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	mov r3, r8
	strb r3, [r6, #27]
	strb r3, [r6, #25]
	movs r3, #16
	strb r3, [r6, #20]
	strb r3, [r6, #21]
	mov r3, r8
	strb r3, [r6, #22]
	movs r3, #4
	strb r3, [r6, #23]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #12]
	ldr r3, .L_080237b8
	ldrh r2, [r6, #8]
	ands r5, r3
	ldr r3, .L_080237bc
	ands r3, r2
	orrs r3, r5
	strh r3, [r6, #8]
	ldrb r2, [r6, #5]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	b .L_080237c0
	.2byte 0x0000
.L_080237b8:
	.4byte 0x000003ff
.L_080237bc:
	.4byte 0xfffffc00
.L_080237c0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
