.syntax unified
	.thumb
	.global Func_080cdf5c
	.thumb_func
Func_080cdf5c:
	push {lr}
	ldr r2, .L_080cdf7c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #8
	cmp r3, #0
	bne .L_080cdf7a
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
.L_080cdf7a:
	pop {pc}
.L_080cdf7c:
	.4byte gPartyState
