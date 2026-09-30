.syntax unified
	.thumb
	.global Func_080c9de0
	.thumb_func
Func_080c9de0:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	bl Func_080ed804
	movs r2, #1
	negs r2, r2
	b .L_080c9df2
.L_080c9df0:
	adds r0, #20
.L_080c9df2:
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_080c9e18
	movs r1, #12
	ldrsh r3, [r0, r1]
	cmp r3, r6
	bne .L_080c9df0
	movs r1, #14
	ldrsh r3, [r0, r1]
	cmp r3, r2
	beq .L_080c9e0c
	cmp r5, r3
	bne .L_080c9df0
.L_080c9e0c:
	ldrb r0, [r0]
	movs r3, #192
	lsls r3, r3, #1
	adds r0, r0, r3
	bl GameFlag_SetBit
.L_080c9e18:
	pop {r5, r6, pc}
	.2byte 0x0000
