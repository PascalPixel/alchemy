.syntax unified
	.thumb
	.global Func_080cf554
	.thumb_func
Func_080cf554:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	bl Func_080ceba8
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r8, r0
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r3, .L_080cf6f0
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	mov r11, r0
	ldr r3, [r3, #4]
	mov r9, r2
	str r3, [sp, #4]
	mov r3, r8
	cmp r3, #0
	bne .L_080cf590
	b .L_080cf6dc
.L_080cf590:
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r0, #0
	movs r2, #2
	mov r10, r0
	add r8, r2
	cmp r6, #0
	bne .L_080cf5a2
	b .L_080cf6dc
.L_080cf5a2:
	movs r3, #128
	lsls r3, r3, #7
	ands r3, r6
	cmp r3, #0
	beq .L_080cf5ae
	b .L_080cf6dc
.L_080cf5ae:
	mov r0, r8
	movs r2, #6
	movs r3, #0
	ldrsh r5, [r0, r3]
	movs r1, #2
	ldrsh r7, [r0, r1]
	add r8, r2
	cmp r6, #128
	beq .L_080cf5d2
	movs r3, #1
	negs r3, r3
	cmp r7, r3
	beq .L_080cf5d2
	adds r0, r7, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cf6b8
.L_080cf5d2:
	adds r0, r5, #0
	add r1, sp, #8
	bl Func_080cec24
	cmp r0, #0
	bne .L_080cf6b8
	mov r0, r10
	adds r0, #64
	bl Func_080ceffc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cf6b8
	ldr r3, [r5, #76]
	ldr r0, .L_080cf6f4
	cmp r3, r0
	bne .L_080cf61a
	adds r3, r5, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #0
	lsls r4, r3, #16
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r4, #0
	lsls r5, r3, #16
	adds r2, r5, #0
	str r4, [sp, #0]
	bl Map_GetTerrainHeightFar
	adds r6, r0, #0
	ldr r4, [sp, #0]
	b .L_080cf63e
.L_080cf61a:
	adds r3, r6, #0
	subs r3, #128
	movs r0, #128
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bls .L_080cf6b8
	cmp r6, #130
	beq .L_080cf6b8
	cmp r6, #132
	beq .L_080cf6b8
	cmp r6, #133
	beq .L_080cf6b8
	cmp r6, #131
	beq .L_080cf6b8
	ldr r4, [r5, #8]
	ldr r6, [r5, #12]
	ldr r5, [r5, #16]
.L_080cf63e:
	mov r1, r11
	subs r2, r4, r1
	mov r0, r9
	subs r3, r5, r0
	asrs r2, r2, #16
	movs r1, #135
	subs r3, r3, r6
	adds r2, #15
	lsls r1, r1, #1
	asrs r3, r3, #16
	cmp r2, r1
	bhi .L_080cf6b8
	adds r3, #47
	cmp r3, #238
	bhi .L_080cf6b8
	movs r2, #1
	negs r2, r2
	cmp r7, r2
	beq .L_080cf6b8
	adds r0, r7, #0
	str r4, [sp, #0]
	bl GameFlag_Test
	adds r7, r0, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bne .L_080cf6b8
	movs r0, #209
	lsls r0, r0, #1
	adds r3, r5, #0
	adds r0, #255
	adds r1, r4, #0
	adds r2, r6, #0
	bl Func_080200c0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cf6b8
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	ldr r1, .L_080cf6f8
	bl Object_SetCallback
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r2, [r1, #9]
	negs r0, r0
	adds r3, r0, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #9]
.L_080cf6b8:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #15
	bgt .L_080cf6dc
	mov r0, r8
	movs r3, #0
	ldrsh r6, [r0, r3]
	movs r1, #2
	add r8, r1
	cmp r6, #0
	beq .L_080cf6dc
	ldr r2, .L_080cf6ec
	adds r3, r6, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080cf6dc
	b .L_080cf5ae
.L_080cf6dc:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cf6ec:
	.4byte 0x00004000
.L_080cf6f0:
	.4byte 0xffff0000
.L_080cf6f4:
	.4byte 0x31415927
.L_080cf6f8:
	.4byte Data_080f01b8
