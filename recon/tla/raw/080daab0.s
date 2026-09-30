.syntax unified
	.thumb
	.global Func_080daab0
	.thumb_func
Func_080daab0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r1, #0
	sub sp, #40
	movs r2, #0
	adds r3, #164
	ldr r0, [r3]
	str r2, [sp, #24]
	mov r8, r2
	ldr r3, [r0]
	ldr r2, .L_080dac9c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	adds r0, #28
	lsrs r3, r3, #5
	str r3, [sp, #20]
	movs r6, #0
	ldr r1, [r1, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #16]
	ldr r4, [sp, #16]
	ldr r3, .L_080daca0
	ands r4, r3
	str r4, [sp, #16]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #12]
	ldr r3, [r1]
	ldr r3, [r3, #4]
	str r0, [sp, #0]
	movs r0, #7
	str r3, [sp, #8]
	str r0, [sp, #28]
.L_080dab06:
	ldr r1, [sp, #0]
	ldr r5, [r1]
	cmp r5, #0
	bne .L_080dab10
	b .L_080dac6a
.L_080dab10:
	mov r2, sp
	adds r2, #32
	str r2, [sp, #4]
.L_080dab16:
	ldr r3, [r5, #4]
	ldr r4, [r5, #8]
	mov r10, r3
	ldr r0, [r5, #12]
	movs r3, #18
	ldrsb r3, [r5, r3]
	mov r9, r4
	mov r11, r0
	cmp r3, #1
	bgt .L_080dab2c
	b .L_080dac5a
.L_080dab2c:
	subs r4, r6, r0
	adds r2, r4, #0
	cmp r4, #0
	bge .L_080dab36
	subs r2, r0, r6
.L_080dab36:
	mov r3, r8
	mov r7, r9
	subs r1, r3, r7
	cmp r1, #0
	blt .L_080dab46
	cmp r2, r1
	blt .L_080dab50
	b .L_080dab58
.L_080dab46:
	mov r0, r9
	mov r7, r8
	subs r3, r0, r7
	cmp r2, r3
	bge .L_080dab58
.L_080dab50:
	ldr r2, [sp, #24]
	mov r3, r10
	subs r0, r2, r3
	b .L_080dab60
.L_080dab58:
	ldr r7, [sp, #24]
	mov r1, r10
	subs r0, r7, r1
	adds r1, r4, #0
.L_080dab60:
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r7, r0, #16
	movs r3, #18
	ldrsb r3, [r5, r3]
	ldr r2, [sp, #20]
	lsls r3, r3, #3
	adds r3, r2, r3
	adds r4, r3, #0
	ldr r3, [sp, #24]
	ldr r1, [sp, #16]
	add r3, r10
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r0, r3, r1
	mov r3, r8
	add r3, r9
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #1
	mov r1, r11
	subs r2, r3, r2
	adds r3, r6, r1
	mov r8, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r1, [sp, #8]
	asrs r3, r3, #1
	subs r3, r3, r2
	subs r6, r3, r1
	ldr r1, .L_080daca4
	mov r3, r8
	subs r2, r6, r3
	adds r3, r0, r1
	ldr r1, .L_080daca8
	subs r4, #16
	cmp r3, r1
	bhi .L_080dac5a
	ldr r3, .L_080dacac
	cmp r2, r3
	ble .L_080dac5a
	ldr r1, .L_080dacb0
	cmp r2, r1
	bgt .L_080dac5a
	movs r3, #128
	asrs r1, r0, #16
	lsls r3, r3, #1
	adds r3, #255
	subs r1, #8
	asrs r2, r2, #16
	ands r1, r3
	mov r0, r8
	movs r3, #255
	subs r2, #8
	ands r2, r3
	adds r3, r0, r6
	asrs r3, r3, #16
	adds r3, #34
	adds r6, r5, #0
	mov r8, r3
	adds r6, #20
	movs r3, #0
	str r3, [r6]
	ldr r3, .L_080dacb4
	lsls r1, r1, #16
	orrs r2, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #4
	orrs r4, r3
	str r2, [r5, #24]
	str r4, [r5, #28]
	cmp r7, #0
	beq .L_080dac50
	ldr r4, [sp, #4]
	ldr r1, .L_080daca0
	ldr r3, [r4, #4]
	lsls r2, r7, #16
	ands r3, r1
	lsrs r2, r2, #16
	orrs r3, r2
	str r3, [r4, #4]
	ldr r3, [sp, #32]
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	orrs r3, r2
	str r3, [sp, #32]
	movs r0, #4
	ldrb r3, [r5, #25]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #25]
	ldr r0, [sp, #4]
	bl Func_0801401c
	movs r3, #31
	ands r0, r3
	movs r1, #63
	ldrb r3, [r5, #27]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	strb r3, [r5, #27]
.L_080dac50:
	strh r7, [r5, #16]
	adds r0, r6, #0
	mov r1, r8
	bl Func_080140d8
.L_080dac5a:
	mov r2, r10
	str r2, [sp, #24]
	mov r8, r9
	ldr r5, [r5]
	mov r6, r11
	cmp r5, #0
	beq .L_080dac6a
	b .L_080dab16
.L_080dac6a:
	movs r3, #0
	str r3, [sp, #24]
	ldr r4, [sp, #0]
	mov r8, r3
	ldr r5, [r4]
	movs r6, #0
	cmp r5, #0
	beq .L_080dac7c
	strh r7, [r5, #16]
.L_080dac7c:
	ldr r0, [sp, #0]
	ldr r1, [sp, #28]
	adds r0, #28
	subs r1, #1
	str r0, [sp, #0]
	str r1, [sp, #28]
	cmp r1, #0
	blt .L_080dac8e
	b .L_080dab06
.L_080dac8e:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dac9c:
	.4byte ResourceTableEntries
.L_080daca0:
	.4byte 0xffff0000
.L_080daca4:
	.4byte 0x001fffff
.L_080daca8:
	.4byte 0x012ffffe
.L_080dacac:
	.4byte 0xffe00000
.L_080dacb0:
	.4byte 0x00dfffff
.L_080dacb4:
	.4byte 0x40002000
