.syntax unified
	.thumb
	.global Func_080dacb8
	.thumb_func
Func_080dacb8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	movs r0, #0
	sub sp, #24
	ldr r5, [r3]
	mov r9, r0
	bl Func_080cdf5c
	bl Object_GetById
	movs r1, #128
	lsls r1, r1, #1
	str r0, [sp, #20]
	str r1, [sp, #4]
	adds r5, #4
	mov r8, r5
.L_080dace8:
	mov r3, r8
	ldr r2, [r3, #24]
	cmp r2, #0
	bne .L_080dacf2
	b .L_080dae08
.L_080dacf2:
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080dacfa
	b .L_080dae08
.L_080dacfa:
	mov r0, r8
	ldr r3, [r0, #16]
	movs r1, #128
	ldr r7, [r2]
	lsls r1, r1, #2
	adds r3, r3, r1
	str r3, [r0, #16]
	ldr r2, [r7, #4]
	mov r1, r8
	str r2, [sp, #16]
	ldr r3, [r7, #8]
	ldr r2, [sp, #20]
	str r3, [sp, #12]
	ldr r0, [r7, #12]
	str r0, [sp, #8]
	ldr r0, [sp, #16]
	ldr r3, [r2, #8]
	ldr r1, [r1, #4]
	subs r2, r0, r3
	mov r11, r1
	cmp r2, #0
	blt .L_080dad2e
	ldr r1, .L_080dae28
	cmp r2, r1
	ble .L_080dad38
	b .L_080dad68
.L_080dad2e:
	ldr r2, [sp, #16]
	ldr r0, .L_080dae28
	subs r3, r3, r2
	cmp r3, r0
	bgt .L_080dad68
.L_080dad38:
	ldr r1, [sp, #20]
	ldr r2, [sp, #8]
	ldr r3, [r1, #16]
	ldr r0, .L_080dae2c
	ldr r1, .L_080dae30
	subs r3, r2, r3
	adds r3, r3, r0
	cmp r3, r1
	bhi .L_080dad68
	ldr r2, [sp, #12]
	ldr r1, [sp, #20]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r2, r0
	ldr r2, [r1, #12]
	cmp r3, r2
	ble .L_080dad68
	ldr r0, [sp, #12]
	ldr r1, .L_080dae34
	adds r3, r0, r1
	cmp r3, r2
	bge .L_080dad68
	movs r2, #32
	str r2, [sp, #4]
.L_080dad68:
	movs r0, #200
	lsls r0, r0, #2
	ldr r7, [r7]
	str r0, [sp, #0]
	movs r3, #1
	mov r9, r3
	mov r6, r11
.L_080dad76:
	mov r1, r8
	ldr r3, [r1, #8]
	ldr r0, [r1, #16]
	mov r2, r9
	muls r2, r3
	ldr r3, [sp, #0]
	mov r10, r2
	subs r0, r0, r3
	bl Trig_Sin
	lsrs r1, r6, #31
	adds r1, r6, r1
	asrs r1, r1, #1
	ldr r2, .L_080dae38
	add r1, r11
	mov lr, r2
	.2byte 0xf800
	adds r5, r0, #0
	bl Trig_Sin
	ldr r3, .L_080dae38
	mov r1, r10
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #16]
	add r6, r11
	subs r0, r1, r0
	str r0, [r7, #4]
	adds r0, r5, #0
	bl Trig_Cos
	mov r1, r10
	ldr r2, .L_080dae38
	mov lr, r2
	.2byte 0xf800
	ldr r3, [sp, #12]
	movs r2, #200
	subs r0, r3, r0
	str r0, [r7, #8]
	ldr r0, [sp, #8]
	movs r3, #1
	str r0, [r7, #12]
	ldr r1, [sp, #0]
	lsls r2, r2, #2
	add r9, r3
	adds r1, r1, r2
	mov r0, r9
	ldr r7, [r7]
	str r1, [sp, #0]
	cmp r0, #5
	ble .L_080dad76
	mov r1, r8
	ldr r0, [r1, #4]
	ldr r2, [sp, #4]
	cmp r0, r2
	ble .L_080dadf8
	movs r1, #253
	lsls r1, r1, #8
	adds r1, #112
	ldr r3, .L_080dae38
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	str r0, [r1, #4]
	b .L_080dae08
.L_080dadf8:
	movs r1, #142
	lsls r1, r1, #9
	adds r1, #40
	ldr r2, .L_080dae38
	mov lr, r2
	.2byte 0xf800
	mov r3, r8
	str r0, [r3, #4]
.L_080dae08:
	movs r0, #1
	add r9, r0
	movs r1, #28
	mov r2, r9
	add r8, r1
	cmp r2, #7
	bgt .L_080dae18
	b .L_080dace8
.L_080dae18:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dae28:
	.4byte 0x0007ffff
.L_080dae2c:
	.4byte 0x000fffff
.L_080dae30:
	.4byte 0x000ffffe
.L_080dae34:
	.4byte 0xffb00000
.L_080dae38:
	.4byte IwramMulQ16
