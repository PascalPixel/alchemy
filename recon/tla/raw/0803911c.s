.syntax unified
	.thumb
	.global Func_0803911c
	.thumb_func
Func_0803911c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r6, r2, #0
	mov r10, r3
	lsls r3, r1, #6
	add r3, r10
	lsls r2, r0, #1
	adds r3, r3, r2
	adds r5, r3, #0
	movs r4, #240
	adds r3, r1, r7
	sub sp, #4
	mov r8, r1
	adds r5, #8
	lsls r4, r4, #8
	cmp r3, #20
	bls .L_0803914e
	movs r3, #20
	subs r7, r3, r1
.L_0803914e:
	cmp r6, #1
	bhi .L_08039154
	movs r6, #2
.L_08039154:
	cmp r6, #30
	bls .L_0803915a
	movs r6, #30
.L_0803915a:
	cmp r7, #1
	bhi .L_08039160
	movs r7, #2
.L_08039160:
	cmp r7, #30
	bls .L_08039166
	movs r7, #30
.L_08039166:
	adds r2, r6, #0
	mov r1, r8
	adds r3, r7, #0
	str r4, [sp, #0]
	bl Func_08041abc
	movs r2, #0
	ldr r4, [sp, #0]
	cmp r2, r7
	bcs .L_080391b2
	movs r3, #32
	subs r3, r3, r6
	lsls r1, r3, #1
.L_08039180:
	mov r0, r10
	ldrb r3, [r0, #5]
	cmp r3, #0
	beq .L_0803919a
	mov r0, r8
	movs r4, #240
	lsls r4, r4, #8
	adds r3, r0, r2
	adds r4, #127
	cmp r3, #16
	bhi .L_0803919a
	movs r4, #240
	lsls r4, r4, #8
.L_0803919a:
	movs r3, #0
	cmp r3, r6
	bcs .L_080391aa
.L_080391a0:
	adds r3, #1
	strh r4, [r5]
	adds r5, #2
	cmp r3, r6
	bcc .L_080391a0
.L_080391aa:
	adds r2, #1
	adds r5, r5, r1
	cmp r2, r7
	bcc .L_08039180
.L_080391b2:
	movs r3, #1
	mov r2, r10
	strb r3, [r2, #3]
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
