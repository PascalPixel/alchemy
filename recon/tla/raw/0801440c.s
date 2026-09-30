.syntax unified
	.thumb
	.global Func_0801440c
	.thumb_func
Func_0801440c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	ldr r1, .L_080144b4
	mov r10, r2
	ldrb r2, [r1]
	adds r6, r0, #0
	mov r9, r2
	movs r0, #31
	cmp r2, #31
	bhi .L_080144a8
	lsls r3, r2, #3
	ldr r2, .L_080144b8
	adds r7, r3, r2
	mov r3, r9
	adds r3, #1
	strb r3, [r1]
	cmp r8, r10
	beq .L_08014440
	mov r3, r8
	mov r1, r10
	cmn r3, r1
	bne .L_08014468
.L_08014440:
	cmp r6, #0
	bne .L_08014468
	movs r0, #128
	ldr r3, .L_080144bc
	mov r1, r10
	lsls r0, r0, #9
	mov lr, r3
	.2byte 0xf800
	mov r2, r8
	mov r1, r10
	adds r3, r0, #0
	cmn r2, r1
	bne .L_0801445c
	negs r3, r0
.L_0801445c:
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r3, [r7]
	lsls r3, r0, #16
	str r3, [r7, #4]
	b .L_080144a6
.L_08014468:
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, r0, #0
	adds r0, r6, #0
	bl Trig_Cos
	mov r1, r8
	adds r6, r0, #0
	bl __divsi3
	mov r1, r8
	strh r0, [r7]
	adds r0, r5, #0
	bl __divsi3
	adds r7, #2
	negs r5, r5
	strh r0, [r7]
	mov r1, r10
	adds r0, r5, #0
	bl __divsi3
	adds r7, #2
	strh r0, [r7]
	mov r1, r10
	adds r0, r6, #0
	bl __divsi3
	adds r7, #2
	strh r0, [r7]
.L_080144a6:
	mov r0, r9
.L_080144a8:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080144b4:
	.4byte gObjAffineCount
.L_080144b8:
	.4byte gObjAffineMatrices
.L_080144bc:
	.4byte IwramDivide
