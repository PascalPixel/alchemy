.syntax unified
	.thumb
	.global Func_0803954c
	.thumb_func
Func_0803954c:
	push {r5, r6, r7, lr}
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	mov lr, r1
	movs r1, #215
	lsls r1, r1, #3
	adds r4, r6, r1
	ldr r3, [r4]
	adds r7, r0, #0
	ldr r5, [sp, #16]
	movs r0, #0
	movs r1, #0
	b .L_08039574
.L_0803956a:
	adds r1, #1
	adds r4, #40
	cmp r1, #3
	beq .L_0803957a
	ldr r3, [r4]
.L_08039574:
	cmp r3, #0
	bne .L_0803956a
	adds r0, r4, #0
.L_0803957a:
	cmp r0, #0
	beq .L_080395f8
	lsls r3, r2, #8
	mov r1, r12
	strh r3, [r0, #30]
	strh r3, [r0, #4]
	lsls r3, r1, #8
	strh r3, [r0, #6]
	mov r3, lr
	strh r3, [r0, #18]
	movs r3, #15
	strh r3, [r0, #22]
	movs r3, #10
	strh r3, [r0, #26]
	ldr r3, [sp, #20]
	movs r2, #0
	movs r1, #152
	strh r3, [r0, #36]
	str r7, [r0]
	strh r2, [r0, #20]
	strh r2, [r0, #24]
	strh r2, [r0, #32]
	strh r2, [r0, #38]
	lsls r1, r1, #5
	adds r1, #140
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080395c0
	cmp r5, #0
	beq .L_080395da
	ldrh r3, [r5, #6]
	strh r3, [r0, #38]
.L_080395c0:
	cmp r5, #0
	beq .L_080395da
	adds r2, r0, #0
	movs r1, #0
	adds r2, #8
.L_080395ca:
	ldrh r3, [r5]
	adds r1, #1
	strh r3, [r2]
	adds r5, #2
	adds r2, #2
	cmp r1, #3
	bls .L_080395ca
	b .L_080395f4
.L_080395da:
	ldr r2, .L_080395f0
	adds r3, r0, #0
	movs r1, #0
	adds r3, #8
.L_080395e2:
	adds r1, #1
	strh r2, [r3]
	adds r3, #2
	cmp r1, #3
	bls .L_080395e2
	b .L_080395f4
	.2byte 0x0000
.L_080395f0:
	.4byte 0x00000000
.L_080395f4:
	movs r3, #0
	strh r3, [r0, #16]
.L_080395f8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
