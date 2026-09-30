.syntax unified
	.thumb
	.global Func_08016348
	.thumb_func
Func_08016348:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_08016364
	ldr r6, .L_08016368
	ldrb r3, [r7, #1]
	mov r8, r0
	mov r12, r1
	ldr r5, [r6]
	cmp r3, #0
	beq .L_0801636c
	cmp r3, #1
	beq .L_080163ce
	b .L_080163f4
.L_08016364:
	.4byte Data_02005360
.L_08016368:
	.4byte 0x04000128
.L_0801636c:
	movs r2, #48
	adds r3, r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080163ca
	movs r3, #136
	adds r4, r5, #0
	ands r4, r3
	cmp r4, #8
	bne .L_080163f4
	movs r2, #4
	adds r3, r5, #0
	ands r3, r2
	lsls r3, r3, #24
	lsrs r2, r3, #24
	cmp r2, #0
	bne .L_080163ca
	ldr r3, [r7, #20]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_080163ca
	ldr r0, .L_080163e4
	strh r2, [r0]
	ldr r1, .L_080163e8
	ldrh r2, [r1]
	movs r3, #129
	negs r3, r3
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	strh r3, [r1]
	movs r3, #1
	strh r3, [r0]
	ldrb r2, [r6, #1]
	subs r3, #66
	ands r3, r2
	strb r3, [r6, #1]
	ldr r2, .L_080163ec
	ldr r3, .L_080163e0
	strh r3, [r2]
	movs r3, #201
	lsls r3, r3, #8
	subs r2, #246
	adds r3, #99
	str r3, [r2]
	strb r4, [r7]
.L_080163ca:
	movs r3, #1
	strb r3, [r7, #1]
.L_080163ce:
	mov r0, r12
	bl Func_080165b4
	mov r0, r8
	bl Func_08016544
	ldr r7, .L_080163f0
	b .L_080163f4
	.2byte 0x0000
.L_080163e0:
	.4byte 0x000000c0
.L_080163e4:
	.4byte 0x04000208
.L_080163e8:
	.4byte 0x04000200
.L_080163ec:
	.4byte 0x04000202
.L_080163f0:
	.4byte Data_02005360
.L_080163f4:
	ldrb r3, [r7, #11]
	ldrb r2, [r7, #3]
	adds r3, #1
	strb r3, [r7, #11]
	ldrb r3, [r7, #2]
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r7]
	cmp r3, #8
	bne .L_0801640c
	movs r3, #128
	orrs r2, r3
.L_0801640c:
	ldrb r3, [r7, #9]
	adds r0, r2, #0
	cmp r3, #0
	beq .L_0801641a
	movs r3, #128
	lsls r3, r3, #5
	orrs r0, r3
.L_0801641a:
	lsls r3, r5, #26
	lsrs r3, r3, #30
	cmp r3, #1
	bls .L_08016428
	movs r3, #128
	lsls r3, r3, #6
	orrs r0, r3
.L_08016428:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
