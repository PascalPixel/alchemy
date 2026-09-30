.syntax unified
	.thumb
	.global Func_080d607c
	.thumb_func
Func_080d607c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #136
	ldr r4, [r3]
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #1
	adds r3, r4, r0
	ldrb r0, [r3]
	ldr r3, .L_080d63fc
	sub sp, #36
	movs r5, #4
	ldrsh r1, [r3, r5]
	str r1, [sp, #32]
	movs r1, #6
	ldrsh r6, [r3, r1]
	str r6, [sp, #28]
	movs r6, #8
	ldrsh r5, [r3, r6]
	mov r11, r5
	movs r5, #10
	ldrsh r1, [r3, r5]
	str r1, [sp, #24]
	movs r6, #14
	ldrsh r5, [r3, r6]
	movs r6, #12
	ldrsh r1, [r3, r6]
	str r5, [sp, #20]
	ldr r3, [r2, #108]
	cmp r3, #0
	beq .L_080d60de
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080d60de
	b .L_080d63ec
.L_080d60de:
	cmp r0, #0
	beq .L_080d60e4
	b .L_080d626c
.L_080d60e4:
	movs r5, #240
	lsls r5, r5, #4
	adds r3, r4, r5
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r6, #241
	lsls r3, r3, #7
	lsls r6, r6, #4
	adds r5, r4, r3
	adds r3, r4, r6
	ldr r3, [r3]
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #2
	mov r8, r3
	adds r3, r4, r0
	ldrh r2, [r3]
	ldr r3, [sp, #28]
	subs r6, #8
	lsls r3, r3, #16
	str r3, [sp, #16]
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r3, r4, r6
	ldr r3, [r3]
	mov r0, r8
	adds r7, r3, #0
	muls r7, r2
	cmp r0, #0
	bne .L_080d6140
	movs r6, #0
	adds r3, r5, #0
.L_080d612a:
	add r2, sp, #32
	ldrh r2, [r2]
	mov r5, r11
	adds r6, #1
	strh r2, [r3]
	strh r5, [r3, #4]
	strh r1, [r3, #8]
	adds r3, #12
	cmp r6, #160
	bne .L_080d612a
	b .L_080d61a2
.L_080d6140:
	movs r6, #240
	lsls r6, r6, #4
	adds r6, #24
	adds r3, r4, r6
	ldr r3, [r3]
	ldr r0, [sp, #32]
	mov r2, r11
	lsls r2, r2, #16
	mov r9, r3
	str r2, [sp, #8]
	lsls r3, r0, #16
	lsls r1, r1, #16
	lsrs r3, r3, #16
	movs r6, #0
	mov r10, r1
	mov r11, r3
.L_080d6160:
	movs r2, #255
	ldr r1, .L_080d6400
	asrs r3, r7, #16
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r0, [r1, r3]
	str r4, [sp, #0]
	mov r1, r9
	ldr r3, .L_080d6404
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080d617e
	adds r0, #255
.L_080d617e:
	lsls r2, r0, #8
	lsrs r2, r2, #16
	mov r0, r11
	adds r3, r0, r2
	strh r3, [r5]
	ldr r1, [sp, #8]
	mov r0, r10
	lsrs r3, r1, #16
	adds r3, r3, r2
	strh r3, [r5, #4]
	lsrs r3, r0, #16
	adds r3, r3, r2
	adds r6, #1
	strh r3, [r5, #8]
	add r7, r8
	adds r5, #12
	cmp r6, #160
	bne .L_080d6160
.L_080d61a2:
	movs r1, #240
	lsls r1, r1, #4
	adds r3, r4, r1
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r2, #240
	lsls r3, r3, #7
	lsls r2, r2, #4
	adds r3, r4, r3
	adds r2, #20
	adds r5, r3, #2
	adds r3, r4, r2
	ldr r3, [r3]
	movs r6, #240
	lsls r6, r6, #4
	adds r6, #2
	mov r8, r3
	ldr r0, [sp, #16]
	adds r3, r4, r6
	ldrh r2, [r3]
	adds r1, #12
	lsrs r3, r0, #16
	adds r2, r2, r3
	adds r3, r4, r1
	ldr r3, [r3]
	adds r7, r3, #0
	muls r7, r2
	mov r2, r8
	cmp r2, #0
	bne .L_080d6204
	movs r6, #0
	adds r3, r5, #0
.L_080d61e8:
	add r5, sp, #28
	add r0, sp, #24
	add r1, sp, #20
	ldrh r5, [r5]
	ldrh r0, [r0]
	ldrh r1, [r1]
	adds r6, #1
	strh r5, [r3]
	strh r0, [r3, #4]
	strh r1, [r3, #8]
	adds r3, #12
	cmp r6, #160
	bne .L_080d61e8
	b .L_080d63d0
.L_080d6204:
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #28
	adds r3, r4, r2
	ldr r3, [r3]
	ldr r0, [sp, #20]
	mov r9, r3
	ldr r3, [sp, #24]
	ldr r1, [sp, #16]
	lsls r3, r3, #16
	str r3, [sp, #4]
	lsls r0, r0, #16
	lsrs r1, r1, #16
	movs r6, #0
	mov r10, r0
	mov r11, r1
.L_080d6224:
	movs r2, #255
	asrs r3, r7, #16
	ands r3, r2
	ldr r2, .L_080d6400
	lsls r3, r3, #1
	ldrsh r0, [r2, r3]
	str r4, [sp, #0]
	mov r1, r9
	ldr r2, .L_080d6404
	mov lr, r2
	.2byte 0xf800
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080d6242
	adds r0, #255
.L_080d6242:
	lsls r2, r0, #8
	lsrs r2, r2, #16
	mov r0, r11
	adds r3, r0, r2
	strh r3, [r5]
	ldr r1, [sp, #4]
	adds r5, #4
	lsrs r3, r1, #16
	adds r3, r3, r2
	mov r0, r10
	strh r3, [r5]
	lsrs r3, r0, #16
	adds r5, #4
	adds r3, r3, r2
	adds r6, #1
	strh r3, [r5]
	add r7, r8
	adds r5, #4
	cmp r6, #160
	bne .L_080d6224
	b .L_080d63d0
.L_080d626c:
	movs r2, #240
	lsls r2, r2, #4
	adds r3, r4, r2
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r6, #241
	lsls r3, r3, #7
	lsls r6, r6, #4
	adds r5, r4, r3
	adds r3, r4, r6
	ldr r3, [r3]
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #2
	mov r8, r3
	adds r3, r4, r0
	ldrh r2, [r3]
	ldr r3, [sp, #28]
	subs r6, #8
	lsls r3, r3, #16
	str r3, [sp, #12]
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r3, r4, r6
	ldr r3, [r3]
	mov r0, r8
	adds r7, r3, #0
	muls r7, r2
	cmp r0, #0
	bne .L_080d62c8
	movs r6, #0
	adds r3, r5, #0
.L_080d62b2:
	add r2, sp, #32
	ldrh r2, [r2]
	mov r5, r11
	adds r6, #1
	strh r2, [r3]
	strh r5, [r3, #4]
	strh r1, [r3, #8]
	adds r3, #12
	cmp r6, #160
	bne .L_080d62b2
	b .L_080d631a
.L_080d62c8:
	movs r6, #240
	lsls r6, r6, #4
	adds r6, #24
	adds r3, r4, r6
	lsls r1, r1, #16
	mov r10, r1
	ldr r3, [r3]
	mov r0, r10
	lsrs r0, r0, #16
	mov r9, r3
	movs r6, #0
	mov r10, r0
.L_080d62e0:
	movs r2, #255
	ldr r1, .L_080d6400
	asrs r3, r7, #16
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r0, [r1, r3]
	str r4, [sp, #0]
	mov r1, r9
	ldr r3, .L_080d6404
	mov lr, r3
	.2byte 0xf800
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080d62fe
	adds r0, #255
.L_080d62fe:
	lsls r3, r0, #8
	add r0, sp, #32
	ldrh r0, [r0]
	lsrs r3, r3, #16
	mov r1, r11
	add r3, r10
	adds r6, #1
	strh r0, [r5]
	strh r1, [r5, #4]
	strh r3, [r5, #8]
	add r7, r8
	adds r5, #12
	cmp r6, #160
	bne .L_080d62e0
.L_080d631a:
	movs r2, #240
	lsls r2, r2, #4
	adds r3, r4, r2
	ldrb r3, [r3]
	movs r2, #1
	eors r2, r3
	lsls r3, r2, #4
	subs r3, r3, r2
	movs r6, #240
	lsls r3, r3, #7
	lsls r6, r6, #4
	adds r3, r4, r3
	adds r6, #20
	adds r5, r3, #2
	adds r3, r4, r6
	ldr r3, [r3]
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #2
	mov r8, r3
	ldr r1, [sp, #12]
	adds r3, r4, r0
	ldrh r2, [r3]
	subs r6, #8
	lsrs r3, r1, #16
	adds r2, r2, r3
	adds r3, r4, r6
	ldr r3, [r3]
	mov r0, r8
	adds r7, r3, #0
	muls r7, r2
	cmp r0, #0
	bne .L_080d637c
	movs r6, #0
	adds r3, r5, #0
.L_080d6360:
	add r1, sp, #28
	add r2, sp, #24
	add r5, sp, #20
	ldrh r1, [r1]
	ldrh r2, [r2]
	ldrh r5, [r5]
	adds r6, #1
	strh r1, [r3]
	strh r2, [r3, #4]
	strh r5, [r3, #8]
	adds r3, #12
	cmp r6, #160
	bne .L_080d6360
	b .L_080d63d0
.L_080d637c:
	movs r6, #240
	lsls r6, r6, #4
	adds r6, #28
	ldr r0, [sp, #20]
	adds r3, r4, r6
	ldr r3, [r3]
	ldr r1, .L_080d6404
	lsls r0, r0, #16
	lsrs r0, r0, #16
	mov r9, r3
	movs r6, #0
	mov r11, r1
	mov r10, r0
.L_080d6396:
	movs r2, #255
	asrs r3, r7, #16
	ands r3, r2
	ldr r2, .L_080d6400
	lsls r3, r3, #1
	ldrsh r0, [r2, r3]
	str r4, [sp, #0]
	mov r1, r9
	mov lr, r11
	.2byte 0xf800
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080d63b2
	adds r0, #255
.L_080d63b2:
	lsls r3, r0, #8
	add r2, sp, #28
	add r0, sp, #24
	ldrh r2, [r2]
	ldrh r0, [r0]
	lsrs r3, r3, #16
	add r3, r10
	adds r6, #1
	strh r2, [r5]
	strh r0, [r5, #4]
	strh r3, [r5, #8]
	add r7, r8
	adds r5, #12
	cmp r6, #160
	bne .L_080d6396
.L_080d63d0:
	movs r1, #240
	lsls r1, r1, #4
	adds r1, #2
	adds r2, r4, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #240
	lsls r2, r2, #4
	adds r1, r4, r2
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
.L_080d63ec:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d63fc:
	.4byte Data_03001120
.L_080d6400:
	.4byte Data_080f08b0
.L_080d6404:
	.4byte IwramMulQ16
