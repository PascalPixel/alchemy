.syntax unified
	.thumb
	.global Func_080d20fc
	.thumb_func
Func_080d20fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	mov r10, r0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	sub sp, #8
	bl Func_080d170c
	movs r0, #24
	bl Func_080d17ac
	ldr r2, .L_080d2184
	movs r1, #9
	negs r1, r1
	movs r3, #0
	mov r11, r1
	mov r8, r3
	mov r9, r2
.L_080d212e:
	mov r4, r8
	ldrsh r3, [r2, r4]
	cmp r3, #0
	blt .L_080d216a
	ldr r4, .L_080d2184
	mov r1, r10
	lsls r6, r1, #1
.L_080d213c:
	movs r7, #0
.L_080d213e:
	movs r5, #0
.L_080d2140:
	ldrsh r2, [r6, r4]
	adds r1, r5, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl Func_080d20a4
	adds r5, #1
	ldr r4, [sp, #0]
	cmp r5, #4
	bls .L_080d2140
	adds r7, #1
	cmp r7, #25
	bls .L_080d213e
	adds r6, #2
	ldrsh r3, [r6, r4]
	movs r0, #2
	movs r1, #1
	add r8, r0
	add r10, r1
	cmp r3, #0
	bge .L_080d213c
.L_080d216a:
	movs r0, #2
	bl WaitFrames
	mov r2, r9
	mov r0, r8
	ldrsh r3, [r2, r0]
	cmp r3, r11
	beq .L_080d2188
	movs r1, #2
	movs r3, #1
	add r8, r1
	add r10, r3
	b .L_080d212e
.L_080d2184:
	.4byte Data_080f3230
.L_080d2188:
	movs r5, #128
	lsls r5, r5, #19
	ldrh r3, [r5]
	ldr r2, .L_080d21c4
	add r0, sp, #4
	eors r3, r2
	strh r3, [r5]
	movs r3, #240
	lsls r3, r3, #8
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_080d21c8
	ldr r2, .L_080d21cc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_080d21d0
	bl Resource_GetTableEntry
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	lsls r1, r1, #19
	ldr r2, .L_080d21d4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_080d21d8
	.2byte 0x0000
.L_080d21c4:
	.4byte 0x00002100
.L_080d21c8:
	.4byte 0x06002000
.L_080d21cc:
	.4byte 0x85000200
.L_080d21d0:
	.4byte 0x00000013
.L_080d21d4:
	.4byte 0x84000800
.L_080d21d8:
	ldrh r3, [r5]
	ldr r2, .L_080d21f0
	add sp, #8
	eors r3, r2
	strh r3, [r5]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d21f0:
	.4byte 0x00000100
