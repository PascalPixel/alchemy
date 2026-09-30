.syntax unified
	.thumb
	.global Func_08126e00
	.thumb_func
Func_08126e00:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #96]
	sub sp, #16
	str r2, [sp, #12]
	movs r1, #158
	ldr r2, [r3, #92]
	ldr r3, [r3, #104]
	mov r9, r2
	lsls r1, r1, #5
	add r1, r9
	movs r2, #0
	mov r11, r3
	movs r3, #15
	str r2, [r1]
	movs r7, #142
	str r3, [sp, #8]
	lsls r7, r7, #5
	add r7, r9
.L_08126e34:
	ldr r0, [r7, #24]
	cmp r0, #0
	beq .L_08126eb8
	ldr r3, [r7]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r7, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, [r7, #8]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	adds r3, r2, #0
	adds r0, r0, r3
	bl Func_080149e0
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	cmp r0, r3
	bgt .L_08126e6e
	movs r3, #0
	str r3, [r7, #24]
	b .L_08126eb2
.L_08126e6e:
	movs r1, #128
	ldr r3, .L_08127024
	lsls r1, r1, #9
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #24]
	ldr r2, .L_08127028
	subs r3, #1
	str r3, [r7, #24]
	mov r10, r0
	mov r8, r2
	adds r5, r7, #0
	movs r6, #2
.L_08126e88:
	ldr r0, [r5]
	mov r1, r10
	negs r0, r0
	asrs r0, r0, #8
	mov lr, r8
	.2byte 0xf800
	movs r1, #152
	lsls r1, r1, #9
	mov lr, r8
	.2byte 0xf800
	ldr r2, [r5, #12]
	subs r6, #1
	asrs r3, r2, #7
	subs r2, r2, r3
	ldr r3, [r5]
	adds r2, r2, r0
	adds r3, r3, r2
	str r2, [r5, #12]
	stmia r5!, {r3}
	cmp r6, #0
	bge .L_08126e88
.L_08126eb2:
	ldr r0, [r7, #24]
	cmp r0, #0
	bne .L_08126f50
.L_08126eb8:
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #188
	add r3, r9
	ldr r3, [r3]
	cmp r3, #24
	bgt .L_08126f4c
	bl Random16
	adds r5, r0, #0
	bl Random16
	movs r3, #128
	lsls r3, r3, #9
	adds r3, r3, r0
	lsrs r6, r3, #1
	adds r0, r5, #0
	mov r8, r3
	bl Trig_Cos
	ldr r2, .L_08127028
	adds r1, r6, #0
	mov lr, r2
	.2byte 0xf800
	str r0, [r7]
	adds r0, r5, #0
	bl Trig_Sin
	adds r1, r6, #0
	ldr r3, .L_08127028
	mov lr, r3
	.2byte 0xf800
	ldr r2, [r7]
	movs r1, #1
	adds r3, r2, #0
	ands r3, r1
	str r0, [r7, #4]
	cmp r3, #0
	beq .L_08126f0a
	negs r3, r2
	str r3, [r7]
.L_08126f0a:
	ldr r2, [r7, #4]
	adds r3, r2, #0
	ands r3, r1
	cmp r3, #0
	beq .L_08126f18
	negs r3, r2
	str r3, [r7, #4]
.L_08126f18:
	bl Random16
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r0, r2
	lsrs r0, r0, #2
	ldr r3, [r7, #4]
	str r0, [r7, #8]
	ldr r0, [r7]
	asrs r2, r3, #8
	negs r0, r0
	negs r3, r3
	asrs r1, r0, #7
	asrs r3, r3, #7
	asrs r0, r0, #8
	adds r3, r3, r0
	adds r1, r1, r2
	str r3, [r7, #16]
	mov r2, r8
	movs r3, #0
	str r3, [r7, #20]
	lsrs r3, r2, #13
	adds r3, #1
	str r1, [r7, #12]
	str r3, [r7, #24]
	adds r0, r3, #0
.L_08126f4c:
	cmp r0, #0
	beq .L_08126f8c
.L_08126f50:
	ldr r3, [r7]
	adds r6, r0, #0
	asrs r3, r3, #10
	adds r5, r3, #0
	ldr r3, [r7, #4]
	adds r5, #64
	asrs r3, r3, #10
	adds r4, r3, #0
	adds r4, #64
	cmp r6, #0
	bge .L_08126f6a
	movs r6, #0
	b .L_08126f70
.L_08126f6a:
	cmp r6, #6
	ble .L_08126f70
	movs r6, #6
.L_08126f70:
	ldr r3, .L_0812702c
	ldr r2, .L_08127030
	ldrb r0, [r3, r6]
	lsls r3, r6, #2
	ldr r1, [r2, r3]
	lsrs r3, r0, #1
	subs r2, r5, r3
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r9
	subs r3, r4, r3
	ldr r0, [sp, #12]
	mov lr, r11
	.2byte 0xf800
.L_08126f8c:
	ldr r3, [sp, #8]
	adds r7, #28
	subs r3, #1
	str r3, [sp, #8]
	cmp r3, #0
	blt .L_08126f9a
	b .L_08126e34
.L_08126f9a:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #188
	ldr r3, [r3]
	movs r5, #156
	lsls r5, r5, #5
	mov r11, r3
	add r5, r9
	movs r7, #2
.L_08126fac:
	ldr r1, [r5]
	ldr r3, [r5, #8]
	ldr r2, [r5, #4]
	adds r1, r1, r3
	ldr r3, [r5, #12]
	str r1, [r5]
	asrs r4, r1, #10
	ldr r1, [r5, #16]
	adds r2, r2, r3
	str r2, [r5, #4]
	asrs r6, r2, #10
	adds r2, r1, #0
	cmp r1, #0
	bge .L_08126fca
	adds r2, r1, #7
.L_08126fca:
	asrs r2, r2, #3
	movs r3, #3
	subs r0, r3, r2
	cmp r0, #0
	blt .L_08126ff4
	adds r3, r1, #1
	str r3, [r5, #16]
	ldr r2, .L_08127034
	lsls r3, r0, #2
	ldr r1, [r2, r3]
	movs r0, #32
	adds r2, r4, #0
	adds r3, r6, #0
	str r0, [sp, #0]
	str r0, [sp, #4]
	add r1, r9
	adds r2, #48
	adds r3, #48
	ldr r0, [sp, #12]
	mov lr, r11
	.2byte 0xf800
.L_08126ff4:
	subs r7, #1
	adds r5, #20
	cmp r7, #0
	bge .L_08126fac
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #188
	add r2, r9
	ldr r3, [r2]
	add sp, #16
	adds r3, #1
	str r3, [r2]
	movs r2, #158
	lsls r2, r2, #5
	add r2, r9
	movs r3, #1
	str r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08127024:
	.4byte IwramRatioMulQ14
.L_08127028:
	.4byte IwramMulQ16
.L_0812702c:
	.4byte Data_08129828
.L_08127030:
	.4byte Data_0812980c
.L_08127034:
	.4byte Data_08129830
