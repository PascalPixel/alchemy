.syntax unified
	.thumb
	.global Func_0802cb64
	.thumb_func
Func_0802cb64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r2, .L_0802cc64
	mov r8, r3
	movs r3, #255
	lsls r3, r3, #8
	ldr r7, .L_0802cc68
	movs r1, #0
	movs r6, #132
	mov r5, r8
	adds r3, #255
	mov r10, r1
	lsls r6, r6, #24
	mov r12, r2
	adds r5, #24
	mov lr, r3
.L_0802cb8e:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0802cc50
	ldrh r3, [r5, #10]
	cmp r3, #0
	bne .L_0802cc50
.L_0802cb9a:
	ldrh r2, [r5, #8]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0802cc4a
	ldr r4, [r5, #4]
	ldrh r0, [r4]
	adds r4, #2
	cmp r0, lr
	bne .L_0802cbb2
	ldr r3, [r5]
	str r3, [r5, #4]
	b .L_0802cb9a
.L_0802cbb2:
	movs r3, #255
	lsls r3, r3, #8
	movs r1, #254
	ands r3, r0
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_0802cbd2
	movs r2, #255
	ands r2, r0
	cmp r2, #255
	beq .L_0802cc50
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0802cb9a
.L_0802cbd2:
	ldrh r2, [r4]
	adds r4, #2
	ldrh r3, [r4, #2]
	ldrh r1, [r4]
	strh r3, [r5, #8]
	mov r4, r8
	ldrb r3, [r4, #22]
	cmp r3, #0
	bne .L_0802cc12
	movs r3, #192
	lsls r3, r3, #3
	cmp r0, r3
	bcc .L_0802cc00
	ldr r4, .L_0802cc6c
	movs r3, #128
	lsls r2, r2, #3
	lsls r0, r0, #5
	lsls r1, r1, #5
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r0, r4
	add r1, r12
	b .L_0802cc3c
.L_0802cc00:
	movs r3, #128
	lsls r2, r2, #3
	lsls r0, r0, #5
	lsls r1, r1, #5
	lsls r3, r3, #19
	adds r3, #212
	add r0, r12
	add r1, r12
	b .L_0802cc3c
.L_0802cc12:
	movs r3, #128
	lsls r3, r3, #2
	cmp r0, r3
	bcc .L_0802cc2c
	ldr r4, .L_0802cc70
	movs r3, #128
	lsls r2, r2, #4
	lsls r0, r0, #6
	lsls r1, r1, #6
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r0, r4
	b .L_0802cc3a
.L_0802cc2c:
	movs r3, #128
	lsls r2, r2, #4
	lsls r0, r0, #6
	lsls r1, r1, #6
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r0, r7
.L_0802cc3a:
	adds r1, r1, r7
.L_0802cc3c:
	orrs r2, r6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r5, #4]
	adds r3, #8
	str r3, [r5, #4]
	b .L_0802cb9a
.L_0802cc4a:
	mov r1, lr
	adds r3, r2, r1
	strh r3, [r5, #8]
.L_0802cc50:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r5, #12
	cmp r3, #15
	bls .L_0802cb8e
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0802cc64:
	.4byte 0x06004000
.L_0802cc68:
	.4byte 0x06008000
.L_0802cc6c:
	.4byte Data_0201c000
.L_0802cc70:
	.4byte gMapBlocks
