.syntax unified
	.thumb
	.global Func_081b8274
	.thumb_func
Func_081b8274:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #16]
	str r3, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #180
	ldr r3, [r3]
	movs r1, #0
	str r3, [sp, #8]
	ldr r3, [sp, #56]
	mov r11, r0
	str r1, [sp, #4]
	movs r6, #0
	cmp r3, #0
	beq .L_081b833a
	movs r1, #128
	subs r3, #1
	lsls r1, r1, #1
	adds r1, r2, r1
	mov r9, r3
	movs r2, #200
	lsls r3, r0, #3
	adds r2, r2, r3
	adds r7, r3, #0
	movs r3, #255
	ands r1, r3
	str r1, [sp, #0]
	mov r10, r2
	adds r7, #196
.L_081b82bc:
	ldr r1, [sp, #56]
	ldr r0, [sp, #12]
	subs r3, r1, r6
	movs r5, #0
	cmp r3, #1
	beq .L_081b82d6
	mov r8, r9
.L_081b82ca:
	movs r1, #10
	adds r5, #1
	bl Math_Div
	cmp r5, r8
	bne .L_081b82ca
.L_081b82d6:
	movs r1, #10
	bl Math_Mod
	cmp r0, #0
	beq .L_081b82e6
	movs r2, #1
	str r2, [sp, #4]
	b .L_081b82ec
.L_081b82e6:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_081b832c
.L_081b82ec:
	movs r3, #22
	ldr r1, [sp, #16]
	muls r3, r6
	movs r2, #128
	adds r3, r1, r3
	lsls r2, r2, #2
	ldr r1, [sp, #0]
	adds r3, r3, r2
	subs r2, #1
	ands r3, r2
	lsls r3, r3, #16
	movs r2, #128
	orrs r3, r1
	lsls r2, r2, #24
	orrs r3, r2
	ldr r2, [sp, #8]
	movs r1, #160
	str r3, [r2, r7]
	lsls r1, r1, #2
	lsls r3, r0, #4
	adds r3, r3, r1
	ldr r1, [sp, #52]
	adds r7, #8
	lsls r2, r1, #12
	orrs r3, r2
	ldr r2, [sp, #8]
	mov r1, r10
	str r3, [r2, r1]
	movs r2, #8
	movs r3, #1
	add r10, r2
	add r11, r3
.L_081b832c:
	ldr r2, [sp, #56]
	movs r1, #1
	negs r1, r1
	adds r6, #1
	add r9, r1
	cmp r6, r2
	bne .L_081b82bc
.L_081b833a:
	mov r0, r11
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
