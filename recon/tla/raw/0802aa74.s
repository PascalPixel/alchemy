.syntax unified
	.thumb
	.global Func_0802aa74
	.thumb_func
Func_0802aa74:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #0
	ldr r1, [r3]
	sub sp, #4
	mov lr, r0
	mov r8, r0
	mov r12, r0
	str r1, [sp, #0]
	cmp r1, #0
	bne .L_0802aa96
	b .L_0802ab98
.L_0802aa96:
	ldmia r1!, {r2}
	adds r3, #252
	mov lr, r2
	adds r2, r1, #0
	str r2, [sp, #0]
	ldmia r1!, {r5}
	mov r8, r5
	adds r5, r1, #0
	str r5, [sp, #0]
	ldmia r1!, {r7}
	mov r12, r7
	adds r7, r1, #0
	str r7, [sp, #0]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0802ab98
	adds r2, r3, #0
	movs r0, #0
	ldrsh r3, [r2, r0]
	movs r1, #1
	negs r1, r1
	ldrh r4, [r2]
	cmp r3, r1
	beq .L_0802ab88
	mov r10, r1
.L_0802aac8:
	ldr r5, .L_0802abb4
	lsls r3, r4, #16
	adds r1, r3, r5
	movs r7, #2
	ldrsh r3, [r2, r7]
	ldr r0, .L_0802abb8
	lsls r3, r3, #16
	add r3, r8
	adds r7, r3, r0
	movs r5, #4
	ldrsh r3, [r2, r5]
	movs r0, #240
	lsls r3, r3, #16
	lsls r0, r0, #15
	adds r4, r3, r0
	movs r5, #6
	ldrsh r3, [r2, r5]
	movs r5, #192
	lsls r3, r3, #16
	add r3, r8
	lsls r5, r5, #15
	adds r0, r3, r5
	cmp lr, r1
	blt .L_0802ab7c
	cmp r12, r7
	blt .L_0802ab7c
	cmp lr, r4
	bgt .L_0802ab7c
	cmp r12, r0
	bgt .L_0802ab7c
	adds r5, r1, #0
	mov r1, lr
	subs r6, r5, r1
	adds r2, r6, #0
	cmp r6, #0
	bge .L_0802ab12
	subs r2, r1, r5
.L_0802ab12:
	mov r1, lr
	subs r3, r4, r1
	cmp r3, #0
	blt .L_0802ab20
	cmp r2, r3
	bgt .L_0802ab28
	b .L_0802ab2e
.L_0802ab20:
	mov r1, lr
	subs r3, r1, r4
	cmp r2, r3
	ble .L_0802ab2e
.L_0802ab28:
	adds r5, r4, #0
	mov r2, lr
	subs r6, r5, r2
.L_0802ab2e:
	adds r1, r7, #0
	mov r3, r12
	subs r4, r1, r3
	adds r2, r4, #0
	cmp r4, #0
	bge .L_0802ab3c
	subs r2, r3, r1
.L_0802ab3c:
	mov r7, r12
	subs r3, r0, r7
	cmp r3, #0
	blt .L_0802ab4a
	cmp r2, r3
	bgt .L_0802ab52
	b .L_0802ab58
.L_0802ab4a:
	mov r7, r12
	subs r3, r7, r0
	cmp r2, r3
	ble .L_0802ab58
.L_0802ab52:
	adds r1, r0, #0
	mov r0, r12
	subs r4, r1, r0
.L_0802ab58:
	adds r2, r6, #0
	cmp r2, #0
	bge .L_0802ab62
	mov r3, lr
	subs r2, r3, r5
.L_0802ab62:
	cmp r4, #0
	blt .L_0802ab6c
	cmp r2, r4
	ble .L_0802ab74
	b .L_0802ab78
.L_0802ab6c:
	mov r7, r12
	subs r3, r7, r1
	cmp r2, r3
	bgt .L_0802ab78
.L_0802ab74:
	mov lr, r5
	b .L_0802ab88
.L_0802ab78:
	mov r12, r1
	b .L_0802ab88
.L_0802ab7c:
	adds r2, #8
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r4, [r2]
	cmp r3, r10
	bne .L_0802aac8
.L_0802ab88:
	ldr r3, [sp, #0]
	mov r1, lr
	subs r3, #12
	str r1, [r3]
	ldr r3, [sp, #0]
	mov r2, r12
	subs r3, #4
	str r2, [r3]
.L_0802ab98:
	mov r3, r12
	mov r5, r8
	subs r1, r3, r5
	mov r0, lr
	bl Func_0802af9c
	bl Func_0802ad84
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802abb4:
	.4byte 0xff880000
.L_0802abb8:
	.4byte 0xffc00000
