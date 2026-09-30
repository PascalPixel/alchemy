.syntax unified
	.thumb
	.global Func_080d085c
	.thumb_func
Func_080d085c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #124]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #60
	adds r4, r6, r1
	movs r2, #0
	ldrsb r2, [r4, r2]
	ldr r7, [r3, #32]
	cmp r2, #0
	beq .L_080d08de
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #61
	adds r1, r6, r3
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r0, [r1]
	cmp r3, r2
	blt .L_080d089e
	movs r3, #0
	strb r3, [r4]
	ldr r0, .L_080d0930
	bl Func_08014644
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_08013438
	b .L_080d0950
.L_080d089e:
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #59
	adds r3, r6, r2
	movs r2, #0
	ldrsb r2, [r3, r2]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #58
	adds r5, r6, r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	subs r2, r2, r3
	adds r3, r0, #1
	strb r3, [r1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r0, r3, #0
	muls r0, r2
	movs r1, #0
	ldrsb r1, [r4, r1]
	ldr r3, .L_080d0934
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	ldrsb r3, [r5, r3]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #42
	adds r3, r3, r0
	adds r2, r6, r1
	strh r3, [r2]
.L_080d08de:
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #42
	adds r3, r6, r2
	ldrh r5, [r3]
	cmp r5, #79
	bls .L_080d0900
	movs r1, #130
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #200
	strh r2, [r3]
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #250
	b .L_080d094e
.L_080d0900:
	cmp r5, #0
	beq .L_080d093c
	ldr r3, .L_080d0938
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080d093c
	movs r1, #130
	lsls r1, r1, #1
	adds r2, r5, #0
	adds r3, r7, r1
	adds r2, #80
	strh r2, [r3]
	ldr r3, .L_080d092c
	movs r2, #131
	lsls r2, r2, #1
	subs r3, r3, r5
	adds r1, r7, r2
	strh r3, [r1]
	b .L_080d0950
	.2byte 0x0000
.L_080d092c:
	.4byte 0x00000050
.L_080d0930:
	.4byte Func_080d085c
.L_080d0934:
	.4byte IwramDivide
.L_080d0938:
	.4byte Data_0300122c
.L_080d093c:
	movs r1, #130
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	strh r2, [r3]
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #159
.L_080d094e:
	strh r3, [r2]
.L_080d0950:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
