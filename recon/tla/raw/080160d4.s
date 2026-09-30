.syntax unified
	.thumb
	.global Func_080160d4
	.thumb_func
Func_080160d4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r7, [r3]
	movs r3, #192
	lsls r3, r3, #6
	sub sp, #4
	adds r3, #252
	adds r6, r7, r3
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r6, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #3
	bl Func_08015f0c
	adds r5, r0, #0
	cmp r5, #14
	bhi .L_08016136
	lsls r0, r5, #16
	lsrs r0, r0, #16
	movs r1, #0
	adds r2, r6, #0
	movs r3, #64
	bl ReadFlash
	movs r3, #196
	adds r0, r5, #1
	lsls r3, r3, #6
	adds r3, #52
	lsls r0, r0, #16
	movs r1, #136
	lsrs r0, r0, #16
	adds r2, r7, r3
	lsls r1, r1, #1
	movs r3, #4
	bl ReadFlash
	movs r0, #1
	b .L_08016138
.L_08016136:
	movs r0, #0
.L_08016138:
	add sp, #4
	pop {r5, r6, r7, pc}
