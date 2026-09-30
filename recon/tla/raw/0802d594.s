.syntax unified
	.thumb
	.global Func_0802d594
	.thumb_func
Func_0802d594:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r1
	adds r6, r2, #0
	adds r7, r0, #0
	mov r10, r3
	bl Func_0802d45c
	mov r2, r8
	mov r3, r10
	adds r5, r0, #0
	subs r1, r2, r3
	adds r0, r7, #0
	adds r2, r6, #0
	bl Func_0802d45c
	cmp r5, r0
	bge .L_0802d5be
	adds r5, r0, #0
.L_0802d5be:
	mov r1, r8
	add r1, r10
	adds r0, r7, #0
	adds r2, r6, #0
	bl Func_0802d45c
	cmp r5, r0
	bge .L_0802d5d0
	adds r5, r0, #0
.L_0802d5d0:
	mov r3, r10
	subs r2, r6, r3
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802d45c
	cmp r5, r0
	bge .L_0802d5e2
	adds r5, r0, #0
.L_0802d5e2:
	mov r3, r10
	adds r2, r6, r3
	adds r0, r7, #0
	mov r1, r8
	bl Func_0802d45c
	cmp r5, r0
	bge .L_0802d5f4
	adds r5, r0, #0
.L_0802d5f4:
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
