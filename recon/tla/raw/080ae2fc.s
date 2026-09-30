.syntax unified
	.thumb
	.global Func_080ae2fc
	.thumb_func
Func_080ae2fc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r1
	mov r10, r2
	bl Owner_GetState
	movs r6, #1
	adds r7, r0, #0
	negs r6, r6
	movs r3, #1
	adds r7, #248
	adds r5, r6, #0
	movs r0, #0
	mov r12, r3
.L_080ae31c:
	mov r1, r12
	movs r4, #0
	lsls r1, r0
	adds r2, r7, #0
.L_080ae324:
	ldmia r2!, {r3}
	ands r3, r1
	cmp r3, #0
	beq .L_080ae330
	adds r5, r4, #0
	adds r6, r0, #0
.L_080ae330:
	adds r4, #1
	cmp r4, #3
	ble .L_080ae324
	adds r0, #1
	cmp r0, #19
	ble .L_080ae31c
	movs r3, #1
	negs r3, r3
	adds r0, r5, #0
	cmp r5, r3
	beq .L_080ae350
	mov r3, r8
	str r5, [r3]
	mov r3, r10
	str r6, [r3]
	movs r0, #0
.L_080ae350:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
