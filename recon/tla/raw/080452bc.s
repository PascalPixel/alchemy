.syntax unified
	.thumb
	.global Func_080452bc
	.thumb_func
Func_080452bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r3, #0
	negs r3, r6
	str r1, [sp, #0]
	str r0, [sp, #4]
	adds r7, r2, #0
	movs r0, #0
	lsls r3, r3, #2
	lsls r2, r6, #2
	mov r12, r0
	mov r8, r3
	mov lr, r2
.L_080452da:
	ldr r0, [sp, #0]
	movs r2, #0
	ldmia r0!, {r4}
	adds r3, r0, #0
	str r3, [sp, #0]
	ldr r0, [sp, #4]
	ldmia r0!, {r1}
	adds r3, r0, #0
	str r3, [sp, #4]
	cmp r6, #0
	bge .L_080452f6
	mov r3, r8
	lsrs r4, r3
	b .L_080452fa
.L_080452f6:
	mov r0, lr
	lsls r4, r0
.L_080452fa:
	ldr r5, .L_0804532c
	movs r0, #7
.L_080452fe:
	lsls r2, r2, #4
	cmp r4, r5
	bls .L_08045308
	lsrs r3, r4, #28
	b .L_0804530a
.L_08045308:
	lsrs r3, r1, #28
.L_0804530a:
	adds r2, r2, r3
	subs r0, #1
	lsls r4, r4, #4
	lsls r1, r1, #4
	cmp r0, #0
	bge .L_080452fe
	stmia r7!, {r2}
	movs r2, #1
	add r12, r2
	mov r3, r12
	cmp r3, #7
	ble .L_080452da
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804532c:
	.4byte 0x0fffffff
