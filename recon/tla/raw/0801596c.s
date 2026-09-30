.syntax unified
	.thumb
	.global Func_0801596c
	.thumb_func
Func_0801596c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #196
	lsls r1, r1, #6
	adds r1, #68
	movs r0, #204
	sub sp, #36
	bl Runtime_AllocateBlock
	movs r3, #0
	mov r11, r0
	add r0, sp, #16
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	mov r1, r11
	ldr r2, .L_080159cc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_080159d0
	movs r0, #2
	bl SetFlashTimerIntr
	movs r0, #0
	mov r8, r0
	b .L_080159b6
.L_080159ac:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r8, r1
.L_080159b6:
	mov r2, r8
	cmp r2, #7
	bhi .L_080159c8
	bl IdentifyFlash
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_080159ac
	b .L_080159d4
.L_080159c8:
	movs r0, #1
	b .L_08015abe
.L_080159cc:
	.4byte 0x85000c51
.L_080159d0:
	.4byte Data_030001e4 + 0x14
.L_080159d4:
	mov r1, r11
	adds r1, #2
	mov r0, r11
	mov r2, r11
	movs r3, #0
	adds r0, #3
	str r1, [sp, #12]
	adds r2, #60
	movs r1, #12
	mov r8, r3
	str r0, [sp, #8]
	movs r3, #20
	mov r7, r11
	movs r0, #30
	str r2, [sp, #4]
	str r1, [sp, #0]
	add r3, sp
	adds r0, r0, r7
	mov r10, r3
	mov r9, r0
.L_080159fc:
	movs r3, #0
	strb r3, [r7]
	ldr r2, [sp, #0]
	ldr r0, [sp, #8]
	movs r3, #16
	strb r3, [r2, r0]
	ldr r3, .L_08015a2c
	mov r1, r9
	strh r3, [r1]
	mov r0, r8
	bl Func_08015c08
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r5, r0, #0
	adds r3, #212
	ldr r0, [sp, #4]
	add r1, sp, #20
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_08015a30
.L_08015a2c:
	.4byte 0x00000000
.L_08015a30:
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #212
	ldr r3, [r2, #8]
	movs r0, #128
	lsls r0, r0, #24
	ands r3, r0
	cmp r3, #0
	bne .L_08015a30
	mov r0, r10
	ldr r1, .L_08015acc
	movs r2, #7
	bl Func_08015ffc
	cmp r0, #0
	bne .L_08015aa6
	mov r1, r10
	ldrh r3, [r1, #10]
	mov r2, r9
	strh r3, [r2]
	ldrb r2, [r1, #7]
	adds r1, r2, #0
	cmp r1, #15
	bhi .L_08015aa6
	cmp r5, #0
	bne .L_08015aa6
	movs r3, #1
	strb r3, [r7]
	ldr r3, [sp, #0]
	ldr r0, [sp, #8]
	strb r2, [r3, r0]
	cmp r5, r8
	bcs .L_08015aa6
	mov lr, r1
	ldr r1, [sp, #8]
	movs r6, #0
	mov r0, r11
	movs r4, #28
	adds r1, #12
.L_08015a7e:
	ldrb r3, [r1]
	adds r1, #1
	mov r12, r3
	cmp r12, lr
	bne .L_08015a9c
	ldr r3, [sp, #12]
	ldrh r2, [r3, r4]
	mov r3, r10
	ldrh r3, [r3, #10]
	mov r12, r3
	cmp r2, r12
	bcs .L_08015a9a
	strb r6, [r0]
	b .L_08015a9c
.L_08015a9a:
	strb r6, [r7]
.L_08015a9c:
	adds r5, #1
	adds r0, #1
	adds r4, #2
	cmp r5, r8
	bcc .L_08015a7e
.L_08015aa6:
	ldr r1, [sp, #0]
	movs r2, #3
	add r8, r2
	movs r0, #6
	adds r1, #3
	mov r3, r8
	adds r7, #3
	add r9, r0
	str r1, [sp, #0]
	cmp r3, #14
	bls .L_080159fc
	movs r0, #0
.L_08015abe:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08015acc:
	.4byte Data_08017d08
