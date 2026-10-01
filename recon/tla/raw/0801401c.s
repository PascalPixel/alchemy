.syntax unified
	.thumb
	.global AffineMatrix_BuildForEffect
	.thumb_func
AffineMatrix_BuildForEffect:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #0
	ldrsh r1, [r0, r2]
	ldrh r6, [r0, #4]
	mov r8, r1
	movs r1, #2
	ldrsh r3, [r0, r1]
	ldr r1, .L_080140cc
	mov r10, r3
	ldrb r2, [r1]
	movs r0, #31
	mov r9, r2
	cmp r2, #31
	bhi .L_080140c0
	lsls r3, r2, #3
	ldr r2, .L_080140d0
	adds r7, r3, r2
	mov r3, r9
	adds r3, #1
	strb r3, [r1]
	cmp r8, r10
	beq .L_08014058
	mov r3, r8
	mov r1, r10
	cmn r3, r1
	bne .L_08014080
.L_08014058:
	cmp r6, #0
	bne .L_08014080
	movs r0, #128
	ldr r3, .L_080140d4
	mov r1, r10
	lsls r0, r0, #9
	mov lr, r3
	.2byte 0xf800
	mov r2, r8
	mov r1, r10
	adds r3, r0, #0
	cmn r2, r1
	bne .L_08014074
	negs r3, r0
.L_08014074:
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r3, [r7]
	lsls r3, r0, #16
	str r3, [r7, #4]
	b .L_080140be
.L_08014080:
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
.L_080140be:
	mov r0, r9
.L_080140c0:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080140cc:
	.4byte gObjAffineCount
.L_080140d0:
	.4byte gObjAffineMatrices
.L_080140d4:
	.4byte IwramSignedDivide
