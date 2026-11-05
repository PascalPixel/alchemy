.syntax unified
	.thumb
	.section .text.x0200817c,"ax",%progbits
	.global Func_0200017c
	.thumb_func
Func_0200017c:
	push {r5, r6, r7, lr}
	sub sp, #32
	adds r7, r0, #0
	mov r0, sp
	bl Party_ListActiveOwners
	movs r5, #0
	adds r6, r0, #0
	cmp r5, r6
	bge .L_020081a2
.L_02008190:
	lsls r2, r5, #1
	mov r3, sp
	ldrh r0, [r3, r2]
	adds r1, r7, #0
	adds r5, #1
	bl Func_02000160
	cmp r5, r6
	blt .L_02008190
.L_020081a2:
	add sp, #32
	pop {r5, r6, r7, pc}
	.2byte 0x0000
