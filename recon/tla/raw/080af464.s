.syntax unified
	.thumb
	.global Func_080af464
	.thumb_func
Func_080af464:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #20
	mov r8, sp
	mov r10, r0
	mov r0, r8
	bl Party_ListActiveOwners
	movs r5, #0
	adds r7, r0, #0
	mov r9, r8
	movs r6, #0
	b .L_080af488
.L_080af484:
	adds r6, #2
	adds r5, #1
.L_080af488:
	cmp r5, r7
	bge .L_080af49a
	mov r1, r9
	ldrsh r0, [r6, r1]
	mov r1, r10
	bl Func_080af4b8
	cmp r0, #0
	beq .L_080af484
.L_080af49a:
	cmp r5, r7
	bne .L_080af4a4
	movs r0, #1
	negs r0, r0
	b .L_080af4aa
.L_080af4a4:
	lsls r3, r5, #1
	mov r1, r8
	ldrsh r0, [r1, r3]
.L_080af4aa:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
