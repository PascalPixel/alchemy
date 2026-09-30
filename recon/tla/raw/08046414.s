.syntax unified
	.thumb
	.global Func_08046414
	.thumb_func
Func_08046414:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r3, sp
	mov r4, r9
	str r4, [r3]
	mov r8, r0
	mov r10, r1
	mov r11, r2
	bl Func_08038eb0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080464cc
	movs r3, #1
	strb r3, [r6, #5]
	strb r3, [r6, #4]
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	movs r3, #240
	strb r3, [r6, #15]
	movs r3, #120
	strh r3, [r6, #6]
	strh r3, [r6, #8]
	ldr r3, .L_08046494
	adds r7, r6, #0
	adds r7, #16
	movs r5, #0
	strb r0, [r6, #14]
	str r3, [r7, #4]
	str r5, [r7, #8]
	mov r5, r9
	subs r5, #4
	ldr r0, [r5]
	ldrh r1, [r7, #6]
	movs r3, #12
	ldrsh r2, [r0, r3]
	ldr r3, .L_08046490
	lsls r2, r2, #3
	add r2, r8
	ands r2, r3
	ldr r3, .L_08046498
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	movs r4, #14
	ldrsh r3, [r0, r4]
	mov r2, r11
	lsls r3, r3, #3
	add r3, r10
	strb r3, [r7, #4]
	ldrb r0, [r6, #14]
	cmp r2, #0
	beq .L_080464a0
	ldr r1, .L_0804649c
	b .L_080464a2
.L_08046490:
	.4byte 0x000001ff
.L_08046494:
	.4byte 0x40000400
.L_08046498:
	.4byte 0xfffffe00
.L_0804649c:
	.4byte Data_08059878
.L_080464a0:
	ldr r1, .L_080464c4
.L_080464a2:
	bl Resource_GetBuffer
	ldr r3, .L_080464c0
	ldrh r2, [r7, #8]
	ands r0, r3
	ldr r3, .L_080464c8
	adds r1, r6, #0
	ands r3, r2
	orrs r3, r0
	strh r3, [r7, #8]
	ldr r0, [r5]
	bl RenderOutput_AppendToList
	b .L_080464cc
	.2byte 0x0000
.L_080464c0:
	.4byte 0x000003ff
.L_080464c4:
	.4byte Data_080598f8
.L_080464c8:
	.4byte 0xfffffc00
.L_080464cc:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
