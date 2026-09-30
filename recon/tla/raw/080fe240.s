.syntax unified
	.thumb
	.global Func_080fe240
	.thumb_func
Func_080fe240:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #0
	movs r1, #180
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r2, [r3]
	movs r0, #0
	bl Func_080fe580
	movs r3, #1
	negs r3, r3
	adds r2, r0, #0
	cmp r0, r3
	beq .L_080fe26e
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r5, r1
	ldrb r2, [r3]
.L_080fe26e:
	adds r0, r2, #0
	pop {r5, pc}
	.2byte 0x0000
