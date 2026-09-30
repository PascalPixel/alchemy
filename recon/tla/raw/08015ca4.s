.syntax unified
	.thumb
	.global Func_08015ca4
	.thumb_func
Func_08015ca4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r7, [r3]
	sub sp, #20
	mov r10, r0
	movs r3, #0
	mov r0, sp
	adds r5, r1, #0
	str r3, [r0]
	movs r1, #60
	movs r3, #128
	adds r1, r1, r7
	lsls r3, r3, #19
	mov r9, r1
	adds r3, #212
	ldr r2, .L_08015e38
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015cde:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015cde
	mov r0, r10
	bl Func_08015f0c
	mov r8, r0
	mov r0, r10
	bl Func_08015ad0
	adds r6, r0, #0
	movs r0, #1
	cmp r6, #14
	bls .L_08015cfe
	b .L_08015e2c
.L_08015cfe:
	movs r3, #128
	adds r1, r7, #0
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	adds r1, #76
	ldr r2, .L_08015e3c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015d1a:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015d1a
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_08015e40
	add r1, sp, #4
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015d40:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015d40
	add r5, sp, #4
	mov r2, r10
	strb r2, [r5, #7]
	bl Func_08015ec8
	strh r0, [r5, #8]
	mov r0, r10
	bl Func_0801601c
	movs r3, #128
	movs r2, #132
	adds r0, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	strh r0, [r5, #10]
	adds r3, #212
	adds r0, r5, #0
	mov r1, r9
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r3, #128
	adds r1, #212
	lsls r3, r3, #24
.L_08015d7c:
	ldr r2, [r1, #8]
	ands r2, r3
	cmp r2, #0
	bne .L_08015d7c
	movs r1, #197
	lsls r1, r1, #6
	adds r3, r7, r1
	strh r2, [r3]
	movs r3, #196
	lsls r3, r3, #6
	adds r3, #66
	adds r2, r7, r3
	mov r1, r8
	movs r3, #3
	strh r3, [r2]
	cmp r1, #14
	bhi .L_08015da2
	movs r3, #6
	strh r3, [r2]
.L_08015da2:
	adds r0, r6, #0
	bl Func_08015b24
	cmp r0, #0
	beq .L_08015db0
	movs r0, #2
	b .L_08015e2c
.L_08015db0:
	mov r2, r8
	cmp r2, #14
	bhi .L_08015dc4
	mov r0, r8
	bl Func_08015f48
	cmp r0, #0
	beq .L_08015dc4
	movs r0, #3
	b .L_08015e2c
.L_08015dc4:
	movs r1, #253
	ldrh r3, [r5, #10]
	lsls r1, r1, #8
	adds r1, #232
	cmp r3, r1
	bls .L_08015e18
	movs r3, #1
	strh r3, [r5, #10]
	movs r2, #132
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	mov r1, r9
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015df2:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015df2
	mov r0, r8
	bl Func_08015b24
	cmp r0, #0
	beq .L_08015e08
	movs r0, #4
	b .L_08015e2c
.L_08015e08:
	adds r0, r6, #0
	bl Func_08015f48
	cmp r0, #0
	beq .L_08015e16
	movs r0, #5
	b .L_08015e2c
.L_08015e16:
	mov r6, r8
.L_08015e18:
	movs r3, #1
	strb r3, [r7, r6]
	mov r2, r10
	adds r3, r7, r6
	strb r2, [r3, #15]
	ldrh r2, [r5, #10]
	lsls r3, r6, #1
	adds r3, r7, r3
	strh r2, [r3, #30]
	movs r0, #0
.L_08015e2c:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08015e38:
	.4byte 0x85000c00
.L_08015e3c:
	.4byte 0x84000bfc
.L_08015e40:
	.4byte Data_08017d10
