.syntax unified
	.thumb
	.global Func_080cdbf8
	.thumb_func
Func_080cdbf8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r7, #156
	movs r3, #1
	lsls r7, r7, #6
	negs r3, r3
	adds r6, r0, #0
	mov r10, r1
	adds r7, #15
	mov r8, r3
	movs r5, #0
.L_080cdc12:
	cmp r5, r6
	beq .L_080cdc30
	mov r0, r10
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_080cdac0
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080cdc30
	cmp r7, r0
	ble .L_080cdc30
	mov r8, r5
	adds r7, r0, #0
.L_080cdc30:
	adds r5, #1
	cmp r5, #80
	ble .L_080cdc12
	mov r0, r8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
