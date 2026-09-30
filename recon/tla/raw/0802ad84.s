.syntax unified
	.thumb
	.global Func_0802ad84
	.thumb_func
Func_0802ad84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r6, #132
	ldr r1, [r3]
	mov r8, r3
	lsls r6, r6, #1
	sub sp, #4
	add r6, r8
	cmp r1, #0
	bne .L_0802adaa
	b .L_0802af70
.L_0802adaa:
	ldmia r1!, {r3}
	ldr r2, .L_0802af80
	ldr r5, .L_0802af84
	adds r2, r2, r3
	mov r10, r2
	ldmia r1!, {r2}
	ldr r3, [r1]
	mov r1, r8
	subs r3, r3, r2
	adds r7, r3, r5
	mov r3, r8
	adds r3, #236
	ldr r1, [r1, #4]
	ldr r3, [r3]
	ldr r2, .L_0802af88
	adds r0, r3, r1
	mov r3, r8
	adds r3, #244
	ldr r3, [r3]
	mov r12, r1
	subs r3, r3, r1
	adds r1, r3, r2
	mov r3, r8
	mov r5, r8
	adds r3, #240
	ldr r4, [r5, #8]
	ldr r3, [r3]
	ldr r5, .L_0802af8c
	adds r2, r3, r4
	mov r3, r8
	adds r3, #248
	ldr r3, [r3]
	subs r3, r3, r4
	adds r3, r3, r5
	cmp r0, r1
	ble .L_0802adf4
	adds r1, r0, #0
.L_0802adf4:
	cmp r2, r3
	ble .L_0802adfa
	adds r3, r2, #0
.L_0802adfa:
	cmp r10, r0
	bge .L_0802ae00
	mov r10, r0
.L_0802ae00:
	cmp r10, r1
	ble .L_0802ae06
	mov r10, r1
.L_0802ae06:
	cmp r7, r2
	bge .L_0802ae0c
	adds r7, r2, #0
.L_0802ae0c:
	cmp r7, r3
	ble .L_0802ae12
	adds r7, r3, #0
.L_0802ae12:
	mov r1, r12
	cmp r1, #0
	beq .L_0802ae42
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r3, .L_0802af90
	subs r5, r5, r0
	mov r2, r8
	adds r1, r5, #0
	ldr r0, [r2, #4]
	mov r9, r3
	mov lr, r9
	.2byte 0xf800
	mov r5, r8
	add r10, r0
	ldr r1, [r5, #12]
	ldr r0, [r5, #4]
	mov lr, r9
	.2byte 0xf800
	ldr r4, [r5, #8]
	str r0, [r5, #4]
.L_0802ae42:
	cmp r4, #0
	beq .L_0802ae70
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r2, .L_0802af90
	subs r5, r5, r0
	mov r1, r8
	ldr r0, [r1, #8]
	mov r9, r2
	adds r1, r5, #0
	mov lr, r9
	.2byte 0xf800
	mov r3, r8
	adds r7, r7, r0
	ldr r1, [r3, #12]
	ldr r0, [r3, #8]
	mov lr, r9
	.2byte 0xf800
	mov r5, r8
	str r0, [r5, #8]
.L_0802ae70:
	movs r1, #232
	add r1, r8
	ldr r2, [r1]
	movs r5, #128
	subs r3, r2, r7
	lsls r5, r5, #13
	mov r9, r1
	cmp r3, r5
	ble .L_0802ae88
	ldr r1, .L_0802af94
	adds r7, r2, r1
	subs r3, r2, r7
.L_0802ae88:
	ldr r5, .L_0802af94
	cmp r3, r5
	bge .L_0802ae94
	movs r1, #128
	lsls r1, r1, #13
	adds r7, r2, r1
.L_0802ae94:
	movs r2, #228
	add r2, r8
	mov r3, r10
	mov r5, r9
	str r3, [r2]
	str r7, [r5]
	movs r1, #0
	mov r11, r2
	mov r8, r1
.L_0802aea6:
	mov r2, r11
	ldr r0, [r2]
	ldr r1, [r6, #16]
	ldr r3, .L_0802af90
	mov lr, r3
	.2byte 0xf800
	mov r5, r9
	ldr r2, .L_0802af90
	mov r10, r0
	ldr r1, [r6, #20]
	ldr r0, [r5]
	mov lr, r2
	.2byte 0xf800
	ldr r2, [r6, #24]
	adds r7, r0, #0
	cmp r2, #0
	beq .L_0802aed0
	ldr r3, [r6, #32]
	adds r3, r3, r2
	str r3, [r6, #32]
	add r10, r3
.L_0802aed0:
	ldr r2, [r6, #28]
	cmp r2, #0
	beq .L_0802aede
	ldr r3, [r6, #36]
	adds r3, r3, r2
	str r3, [r6, #36]
	adds r7, r7, r3
.L_0802aede:
	ldr r3, [r6, #8]
	ldr r1, [r6]
	add r10, r3
	ldr r3, [r6, #12]
	mov r2, r10
	adds r7, r7, r3
	mov r3, r10
	lsrs r4, r3, #19
	adds r3, r1, #0
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r2
	lsrs r5, r7, #19
	cmp r3, #0
	beq .L_0802af1c
	cmp r1, r10
	bge .L_0802af0e
	adds r1, r4, #0
	adds r1, #30
	mov r0, r8
	adds r2, r5, #0
	str r4, [sp, #0]
	b .L_0802af16
.L_0802af0e:
	adds r1, r4, #0
	mov r0, r8
	adds r2, r5, #0
	str r4, [sp, #0]
.L_0802af16:
	bl Func_0802ac90
	ldr r4, [sp, #0]
.L_0802af1c:
	ldr r1, [r6, #4]
	movs r2, #128
	adds r3, r1, #0
	eors r3, r7
	lsls r2, r2, #13
	ands r3, r2
	cmp r3, #0
	beq .L_0802af48
	cmp r1, r7
	bge .L_0802af3e
	adds r2, r5, #0
	adds r2, #20
	mov r0, r8
	adds r1, r4, #0
	bl Func_0802abbc
	b .L_0802af48
.L_0802af3e:
	mov r0, r8
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_0802abbc
.L_0802af48:
	mov r3, r10
	mov r5, r8
	str r3, [r6]
	movs r3, #3
	subs r3, r3, r5
	ldr r5, .L_0802af98
	mov r1, r10
	asrs r2, r1, #16
	lsls r3, r3, #2
	movs r1, #1
	strh r2, [r5, r3]
	add r8, r1
	asrs r2, r7, #16
	adds r3, r3, r5
	strh r2, [r3, #2]
	mov r2, r8
	str r7, [r6, #4]
	adds r6, #56
	cmp r2, #2
	bls .L_0802aea6
.L_0802af70:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802af80:
	.4byte 0xff880000
.L_0802af84:
	.4byte 0xffa00000
.L_0802af88:
	.4byte 0xff100000
.L_0802af8c:
	.4byte 0xff600000
.L_0802af90:
	.4byte IwramMulQ16
.L_0802af94:
	.4byte 0xfff00000
.L_0802af98:
	.4byte Data_03001120
