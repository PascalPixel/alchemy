.syntax unified
	.thumb
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	ldr r3, .L_02008160
	ldr r2, [r3]
	movs r3, #15
	ands r2, r3
	cmp r2, #0
	bne .L_0200815c
	ldr r1, .L_02008164
	ldr r3, [r1]
	cmp r3, #8
	bne .L_02008114
	str r2, [r1]
.L_02008114:
	ldr r3, [r1]
	ldr r0, .L_02008168
	adds r2, r3, #0
	adds r3, #1
	str r3, [r1]
	lsls r3, r2, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #194
	strh r3, [r1]
	movs r1, #7
	adds r3, r2, #1
	ands r3, r1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	movs r4, #160
	lsls r4, r4, #19
	adds r4, #196
	strh r3, [r4]
	adds r3, r2, #2
	ands r3, r1
	lsls r3, r3, #1
	adds r3, r3, r0
	ldrh r3, [r3]
	adds r4, #2
	adds r2, #3
	strh r3, [r4]
	ands r2, r1
	lsls r2, r2, #1
	adds r2, r2, r0
	ldrh r3, [r2]
	adds r4, #2
	strh r3, [r4]
.L_0200815c:
	pop {pc}
	.2byte 0x0000
.L_02008160:
	.4byte gFrameCount
.L_02008164:
	.4byte gData_020001b0
.L_02008168:
	.4byte 0x05000180
