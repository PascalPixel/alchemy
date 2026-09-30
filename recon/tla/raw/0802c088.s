.syntax unified
	.thumb
	.global Func_0802c088
	.thumb_func
Func_0802c088:
	push {r5, r6, lr}
	movs r1, #192
	lsls r1, r1, #18
	ldr r3, [r1, #28]
	movs r2, #200
	lsls r2, r2, #4
	adds r6, r3, r2
	movs r3, #128
	lsls r3, r3, #19
	ldr r1, [r1, #32]
	ldrh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	mov r12, r1
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #19
	adds r1, #176
	lsls r3, r3, #16
	ldrh r2, [r1, #10]
	asrs r5, r3, #16
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1, #10]
	movs r3, #254
	ldrh r2, [r1, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r2
	strh r3, [r1, #10]
	movs r4, #128
	lsls r4, r4, #19
	adds r4, #32
	ldrh r3, [r1, #10]
	cmp r6, #0
	beq .L_0802c112
	ldr r3, .L_0802c16c
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #10
	adds r0, r6, r0
	ldmia r0!, {r3}
	ldr r2, .L_0802c170
	str r3, [r4]
	adds r4, #4
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	str r3, [r4]
	adds r3, r1, #0
	subs r1, #144
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0802c112:
	movs r3, #130
	lsls r3, r3, #1
	add r3, r12
	ldrh r3, [r3]
	movs r2, #132
	lsls r2, r2, #1
	add r2, r12
	strh r3, [r2]
	movs r3, #131
	lsls r3, r3, #1
	add r3, r12
	ldrh r0, [r3]
	movs r3, #133
	lsls r3, r3, #1
	add r3, r12
	strh r0, [r3]
	movs r3, #0
	ldrh r1, [r2]
	cmp r1, #199
	bhi .L_0802c152
	lsls r2, r0, #16
	lsrs r2, r2, #16
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	lsls r3, r3, #1
	cmp r1, r2
	bhi .L_0802c152
	movs r3, #0
	cmp r1, #0
	bne .L_0802c152
	movs r3, #2
.L_0802c152:
	orrs r5, r3
	lsls r3, r5, #16
	movs r2, #128
	lsrs r3, r3, #16
	lsls r2, r2, #19
	strh r3, [r2]
	movs r2, #134
	lsls r2, r2, #1
	add r2, r12
	movs r3, #0
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0802c16c:
	.4byte Data_0300122c
.L_0802c170:
	.4byte 0xa6600008
