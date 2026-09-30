.syntax unified
	.thumb
	.global Func_0810a804
	.thumb_func
Func_0810a804:
	push {r5, r6, r7, lr}
	sub sp, #32
	mov r6, sp
	adds r0, r6, #0
	bl Party_ListActiveOwnersFar
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0810a82e
.L_0810a818:
	ldrh r0, [r6]
	adds r6, #2
	bl Func_0810a7dc
	cmp r0, #0
	beq .L_0810a828
	movs r0, #1
	b .L_0810a830
.L_0810a828:
	adds r5, #1
	cmp r5, r7
	blt .L_0810a818
.L_0810a82e:
	movs r0, #0
.L_0810a830:
	add sp, #32
	pop {r5, r6, r7, pc}
