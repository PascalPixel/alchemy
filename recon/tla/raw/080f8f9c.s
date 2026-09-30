.syntax unified
	.thumb
	.global Func_080f8f9c
	.thumb_func
Func_080f8f9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	ldr r1, [sp, #28]
	adds r5, r0, #0
	adds r7, r2, #0
	movs r0, #1
	mov r2, r8
	adds r6, r3, #0
	mov r10, r1
	negs r0, r0
	cmp r2, #0
	bne .L_080f8fbe
	b .L_080f90f6
.L_080f8fbe:
	ldr r0, .L_080f9100
	bl Func_080383f8
	adds r1, r7, #0
	mov r0, r8
	bl Math_Div
	adds r1, r7, #0
	mov r9, r0
	mov r0, r8
	bl __modsi3
	cmp r0, #0
	beq .L_080f8fde
	movs r3, #1
	add r9, r3
.L_080f8fde:
	cmp r5, #0
	beq .L_080f8ffc
	ldr r2, .L_080f9104
	movs r3, #16
	ldr r4, [r2, #12]
	ldr r1, [r2, #12]
	ldr r5, [r2, #12]
	ands r4, r3
	ldr r2, [r2, #12]
	movs r3, #32
	ands r1, r3
	movs r3, #64
	ands r5, r3
	movs r3, #128
	b .L_080f9014
.L_080f8ffc:
	ldr r2, .L_080f9104
	movs r3, #128
	ldr r4, [r2, #12]
	ldr r1, [r2, #12]
	ldr r5, [r2, #12]
	ands r4, r3
	ldr r2, [r2, #12]
	movs r3, #64
	ands r1, r3
	movs r3, #32
	ands r5, r3
	movs r3, #16
.L_080f9014:
	ands r2, r3
	cmp r5, #0
	beq .L_080f904c
	movs r0, #111
	bl Audio_PlayCue
	mov r1, r10
	ldr r3, [r1]
	subs r3, #1
	str r3, [r1]
	cmp r3, #0
	bge .L_080f9032
	mov r3, r9
	subs r3, #1
	str r3, [r1]
.L_080f9032:
	mov r2, r10
	ldr r3, [r2]
	mov r2, r8
	adds r0, r7, #0
	muls r0, r3
	ldr r3, [r6]
	subs r2, #1
	adds r3, r3, r0
	cmp r3, r2
	ble .L_080f908e
	mov r1, r8
	subs r3, r1, r0
	b .L_080f9082
.L_080f904c:
	cmp r2, #0
	beq .L_080f9096
	movs r0, #111
	bl Audio_PlayCue
	mov r2, r10
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	mov r2, r9
	subs r2, #1
	cmp r3, r2
	ble .L_080f906a
	mov r3, r10
	str r5, [r3]
.L_080f906a:
	mov r1, r10
	ldr r3, [r1]
	mov r2, r8
	adds r0, r7, #0
	muls r0, r3
	ldr r3, [r6]
	subs r2, #1
	adds r3, r3, r0
	cmp r3, r2
	ble .L_080f908e
	mov r2, r8
	subs r3, r2, r0
.L_080f9082:
	subs r3, #1
	subs r1, r7, #1
	str r3, [r6]
	cmp r3, r1
	ble .L_080f908e
	str r1, [r6]
.L_080f908e:
	bl Func_080138a8
	movs r0, #1
	b .L_080f90f6
.L_080f9096:
	cmp r1, #0
	beq .L_080f90c4
	movs r0, #111
	bl Audio_PlayCue
	ldr r3, [r6]
	subs r3, #1
	str r3, [r6]
	cmp r3, #0
	bge .L_080f90f4
	subs r2, r7, #1
	str r2, [r6]
	mov r1, r10
	ldr r3, [r1]
	mov r1, r8
	muls r3, r7
	subs r3, r1, r3
	subs r3, #1
	str r3, [r6]
	cmp r3, r2
	ble .L_080f90f4
	str r2, [r6]
	b .L_080f90f4
.L_080f90c4:
	movs r0, #1
	negs r0, r0
	cmp r4, #0
	beq .L_080f90f6
	movs r0, #111
	bl Audio_PlayCue
	ldr r2, [r6]
	mov r1, r10
	adds r2, #1
	str r2, [r6]
	movs r0, #0
	ldr r3, [r1]
	mov r1, r8
	muls r3, r7
	subs r3, r1, r3
	cmp r2, r3
	bne .L_080f90ea
	str r0, [r6]
.L_080f90ea:
	ldr r3, [r6]
	subs r2, r7, #1
	cmp r3, r2
	ble .L_080f90f4
	str r0, [r6]
.L_080f90f4:
	movs r0, #0
.L_080f90f6:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080f9100:
	.4byte 0x06002500
.L_080f9104:
	.4byte gInput
