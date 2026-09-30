.syntax unified
	.thumb
	.global Func_08105300
	.thumb_func
Func_08105300:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r4, [r3]
	movs r5, #136
	lsls r3, r0, #2
	lsls r5, r5, #2
	adds r3, r3, r5
	ldr r3, [r4, r3]
	cmp r3, #0
	beq .L_08105328
	lsls r0, r0, #1
	adds r5, #16
	adds r3, r0, r5
	strh r1, [r4, r3]
	movs r1, #142
	lsls r1, r1, #2
	adds r3, r0, r1
	strh r2, [r4, r3]
.L_08105328:
	pop {r5, pc}
	.2byte 0x0000
