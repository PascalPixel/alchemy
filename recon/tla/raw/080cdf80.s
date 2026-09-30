.syntax unified
	.thumb
	.global Func_080cdf80
	.thumb_func
Func_080cdf80:
	push {lr}
	cmp r0, r1
	beq .L_080cdf90
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	cmp r0, r3
	bne .L_080cdf94
.L_080cdf90:
	movs r0, #1
	b .L_080cdfae
.L_080cdf94:
	movs r3, #255
	lsls r3, r3, #1
	cmp r0, r3
	bne .L_080cdfac
	adds r0, r1, #0
	bl Item_Get
	ldrb r3, [r0, #2]
	movs r0, #1
	subs r3, #1
	cmp r3, #3
	bls .L_080cdfae
.L_080cdfac:
	movs r0, #0
.L_080cdfae:
	pop {pc}
