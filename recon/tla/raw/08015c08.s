.syntax unified
	.thumb
	.global Func_08015c08
	.thumb_func
Func_08015c08:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r6, [r3]
	movs r3, #60
	adds r5, r0, #0
	adds r3, r3, r6
	mov r10, r3
	lsls r0, r5, #16
	movs r3, #128
	lsls r3, r3, #5
	mov r2, r10
	lsrs r0, r0, #16
	movs r1, #0
	sub sp, #16
	mov r8, r3
	bl ReadFlash
	movs r3, #128
	adds r0, r5, #1
	lsls r3, r3, #5
	adds r3, #60
	lsls r0, r0, #16
	adds r2, r6, r3
	lsrs r0, r0, #16
	mov r3, r8
	movs r1, #0
	bl ReadFlash
	movs r3, #128
	adds r5, #2
	lsls r3, r3, #6
	adds r3, #60
	lsls r5, r5, #16
	adds r6, r6, r3
	lsrs r5, r5, #16
	adds r0, r5, #0
	adds r2, r6, #0
	mov r3, r8
	movs r1, #0
	bl ReadFlash
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r10
	mov r1, sp
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #128
	adds r1, #212
	lsls r2, r2, #24
.L_08015c82:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_08015c82
	bl Func_08015ec8
	mov r3, sp
	ldrh r3, [r3, #8]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
