.syntax unified
	.thumb
	.global Func_0803deac
	.thumb_func
Func_0803deac:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #72]
	cmp r0, #0
	beq .L_0803dede
	movs r3, #239
	lsls r3, r3, #1
	movs r1, #0
	adds r2, r4, r3
	movs r0, #0
.L_0803dec2:
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_0803ded2
	movs r3, #234
	adds r0, r4, r0
	lsls r3, r3, #1
	adds r0, r0, r3
	b .L_0803defe
.L_0803ded2:
	adds r1, #1
	adds r2, #52
	adds r0, #52
	cmp r1, #5
	bne .L_0803dec2
	b .L_0803defc
.L_0803dede:
	adds r2, r4, #0
	adds r0, r4, #0
	movs r1, #0
	adds r2, #104
	adds r0, #114
.L_0803dee8:
	ldrh r3, [r0]
	adds r0, #52
	cmp r3, #0
	bne .L_0803def4
	adds r0, r2, #0
	b .L_0803defe
.L_0803def4:
	adds r1, #1
	adds r2, #52
	cmp r1, #7
	bne .L_0803dee8
.L_0803defc:
	movs r0, #0
.L_0803defe:
	pop {pc}
