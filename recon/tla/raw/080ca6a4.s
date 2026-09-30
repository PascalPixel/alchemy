.syntax unified
	.thumb
	.global Func_080ca6a4
	.thumb_func
Func_080ca6a4:
	push {r5, lr}
	adds r5, r0, #0
	cmp r5, #8
	bgt .L_080ca6d6
	cmp r5, #8
	bne .L_080ca6d6
	ldr r3, .L_080ca6dc
	movs r1, #253
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_080ca6e0
	movs r5, #9
	cmp r2, r3
	beq .L_080ca6c6
	movs r5, #11
.L_080ca6c6:
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca6d6
	adds r5, #1
.L_080ca6d6:
	adds r0, r5, #0
	pop {r5, pc}
	.2byte 0x0000
.L_080ca6dc:
	.4byte gPartyState
.L_080ca6e0:
	.4byte 0x00000001
