.syntax unified
	.thumb
	.section .text.x020082a4,"ax",%progbits
	.balign 4
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008380
	movs r2, #4
	negs r2, r2
	ands r3, r2
	ldr r3, [r3, #4]
	movs r2, #192
	lsls r2, r2, #18
	mov r11, r3
	ldr r3, [r2, #32]
	movs r0, #160
	lsls r0, r0, #1
	adds r1, r3, r0
	ldr r3, [r2, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r5, .L_02008384
	cmp r3, #0
	bne .L_02008374
	str r3, [r1, #28]
	ldr r3, .L_02008388
	movs r6, #0
	ldrb r2, [r3]
	movs r3, #6
	ldrsh r0, [r1, r3]
	movs r3, #2
	ldrsh r4, [r1, r3]
	movs r3, #204
	lsls r3, r3, #6
	muls r3, r0
	lsls r2, r2, #10
	movs r1, #153
	adds r3, r3, r2
	lsls r1, r1, #5
	mov r10, r3
	adds r3, r0, #0
	muls r3, r1
	mov r9, r4
	subs r7, r3, r2
	cmp r6, #160
	beq .L_02008340
.L_0200830c:
	mov r0, r10
	mov lr, r11
	.2byte 0xf800
	mov r8, r0
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	mov r4, r8
	lsls r2, r0, #3
	lsls r3, r4, #1
	subs r2, r2, r0
	add r3, r8
	adds r3, r3, r2
	asrs r3, r3, #19
	movs r0, #204
	movs r2, #153
	add r3, r9
	lsls r0, r0, #6
	lsls r2, r2, #5
	adds r6, #1
	strh r3, [r5]
	add r10, r0
	adds r5, #2
	adds r7, r7, r2
	cmp r6, #160
	bne .L_0200830c
.L_02008340:
	ldr r5, .L_02008384
	movs r1, #128
	ldrh r3, [r5]
	lsls r1, r1, #19
	adds r1, #24
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r0, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r0, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	adds r0, r5, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200838c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008374:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008380:
	.4byte Math_Sine
.L_02008384:
	.4byte Data_02006340
.L_02008388:
	.4byte gFrameCount
.L_0200838c:
	.4byte 0xa2600001
	.section .bss,"aw",%nobits
	.space 0x0000000c
	.global Data_02006340
Data_02006340:
