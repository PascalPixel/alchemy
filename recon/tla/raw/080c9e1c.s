.syntax unified
	.thumb
	.global Func_080c9e1c
	.thumb_func
Func_080c9e1c:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_080ed804
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_080c9e42
.L_080c9e2a:
	ldrb r3, [r0]
	cmp r3, r5
	bne .L_080c9e3a
	ldrb r3, [r0, #1]
	cmp r3, #0
	beq .L_080c9e3a
	adds r0, r3, #0
	b .L_080c9e44
.L_080c9e3a:
	adds r0, #20
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_080c9e2a
.L_080c9e42:
	movs r0, #1
.L_080c9e44:
	pop {r5, pc}
	.2byte 0x0000
