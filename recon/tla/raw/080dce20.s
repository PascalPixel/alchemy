.syntax unified
	.thumb
	.global Func_080dce20
	.thumb_func
Func_080dce20:
	push {r5, lr}
	lsls r0, r0, #16
	movs r1, #180
	asrs r2, r0, #16
	lsls r1, r1, #1
	adds r0, r2, r1
	bl __modsi3
	lsls r0, r0, #16
	asrs r2, r0, #16
	movs r5, #0
	cmp r2, #59
	bgt .L_080dce40
	lsls r0, r2, #5
	subs r0, r0, r2
	b .L_080dce54
.L_080dce40:
	cmp r2, #179
	bgt .L_080dce48
	movs r5, #31
	b .L_080dce5c
.L_080dce48:
	cmp r2, #239
	bgt .L_080dce5c
	movs r3, #240
	subs r3, r3, r2
	lsls r0, r3, #5
	subs r0, r0, r3
.L_080dce54:
	movs r1, #60
	bl __divsi3
	adds r5, r0, #0
.L_080dce5c:
	adds r0, r5, #0
	pop {r5, pc}
