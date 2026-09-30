.syntax unified
	.thumb
	.global Func_080ae0f0
	.thumb_func
Func_080ae0f0:
	push {lr}
	ldr r3, .L_080ae114
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #4
	str r2, [r3]
	movs r0, #4
	bl Func_080afdd8
	movs r0, #5
	bl Func_080afe1c
	movs r0, #6
	bl Func_080afe1c
	pop {pc}
	.2byte 0x0000
.L_080ae114:
	.4byte gPartyState
