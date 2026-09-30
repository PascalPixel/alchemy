.syntax unified
	.thumb
	.global Func_08015ffc
	.thumb_func
Func_08015ffc:
	push {lr}
	adds r4, r2, #0
	movs r2, #0
	b .L_0801600a
.L_08016004:
	subs r4, #1
	adds r0, #1
	adds r1, #1
.L_0801600a:
	cmp r4, #0
	beq .L_08016018
	ldrb r2, [r0]
	ldrb r3, [r1]
	subs r2, r2, r3
	cmp r2, #0
	beq .L_08016004
.L_08016018:
	adds r0, r2, #0
	pop {pc}
