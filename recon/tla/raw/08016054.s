.syntax unified
	.thumb
	.global Func_08016054
	.thumb_func
Func_08016054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r3, [r3]
	movs r2, #192
	lsls r2, r2, #6
	adds r2, #60
	sub sp, #4
	adds r6, r3, r2
	movs r3, #0
	mov r10, r3
	movs r7, #0
	mov r8, sp
.L_08016076:
	mov r2, r8
	movs r3, #0
	str r3, [r2]
	movs r3, #128
	movs r2, #133
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r0, r8
	adds r1, r6, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r7, #0
	bl Func_08015f0c
	adds r5, r0, #0
	cmp r5, #14
	bhi .L_080160c0
	lsls r0, r5, #16
	lsrs r0, r0, #16
	movs r1, #0
	adds r2, r6, #0
	movs r3, #64
	bl ReadFlash
	adds r0, r5, #1
	lsls r0, r0, #16
	adds r2, r6, #0
	movs r3, #4
	lsrs r0, r0, #16
	adds r2, #56
	movs r1, #16
	bl ReadFlash
	movs r3, #1
	add r10, r3
.L_080160c0:
	adds r7, #1
	adds r6, #64
	cmp r7, #2
	bls .L_08016076
	mov r0, r10
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
