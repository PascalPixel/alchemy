.syntax unified
	.thumb
	.global Func_080d3b28
	.thumb_func
Func_080d3b28:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r6, r1, #0
	bl Object_GetById
	mov r8, r0
	cmp r0, #0
	beq .L_080d3bdc
	mov r3, r8
	adds r3, #84
	ldrb r3, [r3]
	mov r9, r3
	cmp r3, #1
	bne .L_080d3bdc
	movs r3, #128
	lsls r3, r3, #8
	mov r2, r8
	ands r3, r6
	ldr r7, [r2, #80]
	cmp r3, #0
	beq .L_080d3b5c
	movs r6, #0
.L_080d3b5c:
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	movs r2, #128
	movs r3, #0
	adds r5, r0, #0
	lsls r2, r2, #3
	mov r0, sp
	adds r5, r5, r2
	mov r10, r3
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r5, #0
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	bl Func_08038248
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r7, #16]
	bl VramBlock_LoadCached
	adds r5, r0, #0
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	mov r3, r8
	adds r3, #92
	mov r2, r9
	strb r2, [r3]
	ldr r0, [r7, #40]
	bl Func_08020070
	mov r3, r10
	ldrb r2, [r7, #5]
	str r3, [r7, #40]
	strb r3, [r7, #27]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	strb r3, [r7, #5]
	ldr r3, .L_080d3bd4
	ldrh r2, [r7, #8]
	ands r5, r3
	ldr r3, .L_080d3bd8
	ands r3, r2
	orrs r3, r5
	mov r2, r10
	strh r3, [r7, #8]
	strb r2, [r7, #25]
	strb r2, [r7, #26]
	b .L_080d3bdc
.L_080d3bd4:
	.4byte 0x000003ff
.L_080d3bd8:
	.4byte 0xfffffc00
.L_080d3bdc:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
