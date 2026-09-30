.syntax unified
	.thumb
	.global Func_080aee40
	.thumb_func
Func_080aee40:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #20
	mov r5, sp
	mov r10, r0
	adds r0, r5, #0
	bl Party_ListActiveOwners
	movs r6, #0
	mov r8, r0
	cmp r6, r8
	bge .L_080aee86
	mov r9, r5
	movs r7, #0
.L_080aee62:
	mov r2, r9
	ldrsh r5, [r7, r2]
	mov r1, r10
	adds r0, r5, #0
	bl Inventory_AddItem
	adds r1, r0, #0
	cmp r1, #0
	blt .L_080aee7e
	adds r0, r5, #0
	bl Inventory_Remove
	adds r0, r5, #0
	b .L_080aee8a
.L_080aee7e:
	adds r6, #1
	adds r7, #2
	cmp r6, r8
	blt .L_080aee62
.L_080aee86:
	movs r0, #1
	negs r0, r0
.L_080aee8a:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
