.syntax unified
	.thumb
	.global Func_08016430
	.thumb_func
Func_08016430:
	push {r5, r6, r7, lr}
	ldr r7, .L_080164b4
	ldr r6, .L_080164b8
	ldrb r3, [r7, #1]
	ldr r5, [r6]
	cmp r3, #0
	bne .L_0801649c
	movs r3, #136
	adds r4, r5, #0
	ands r4, r3
	cmp r4, #8
	bne .L_08016496
	movs r2, #4
	adds r3, r5, #0
	ands r3, r2
	lsls r3, r3, #24
	lsrs r2, r3, #24
	cmp r2, #0
	bne .L_08016492
	ldr r3, [r7, #20]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_08016492
	ldr r0, .L_080164bc
	strh r2, [r0]
	ldr r1, .L_080164c0
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
	ldr r2, .L_080164c4
	ldr r3, .L_080164b0
	strh r3, [r2]
	movs r3, #201
	lsls r3, r3, #8
	subs r2, #246
	adds r3, #99
	str r3, [r2]
	strb r4, [r7]
.L_08016492:
	movs r3, #1
	strb r3, [r7, #1]
.L_08016496:
	ldrb r3, [r7, #11]
	adds r3, #1
	strb r3, [r7, #11]
.L_0801649c:
	ldrb r3, [r7, #2]
	ldrb r2, [r7, #3]
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r7]
	cmp r3, #8
	bne .L_080164c8
	movs r3, #128
	orrs r2, r3
	b .L_080164c8
.L_080164b0:
	.4byte 0x000000c0
.L_080164b4:
	.4byte gSerialRuntime
.L_080164b8:
	.4byte 0x04000128
.L_080164bc:
	.4byte 0x04000208
.L_080164c0:
	.4byte 0x04000200
.L_080164c4:
	.4byte 0x04000202
.L_080164c8:
	ldrb r3, [r7, #9]
	adds r0, r2, #0
	cmp r3, #0
	beq .L_080164d6
	movs r3, #128
	lsls r3, r3, #5
	orrs r0, r3
.L_080164d6:
	lsls r3, r5, #26
	lsrs r3, r3, #30
	cmp r3, #1
	bls .L_080164e4
	movs r3, #128
	lsls r3, r3, #6
	orrs r0, r3
.L_080164e4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
