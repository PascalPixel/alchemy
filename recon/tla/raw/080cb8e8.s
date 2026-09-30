.syntax unified
	.thumb
	.global Func_080cb8e8
	.thumb_func
Func_080cb8e8:
	push {lr}
	ldr r2, .L_080cb918
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #118
	ldr r0, [r3, #108]
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080cb912
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r3, [r3]
	lsls r3, r3, #2
	adds r3, #20
	ldr r0, [r0, r3]
	b .L_080cb914
.L_080cb912:
	ldr r0, [r0, #52]
.L_080cb914:
	pop {pc}
	.2byte 0x0000
.L_080cb918:
	.4byte gPartyState
