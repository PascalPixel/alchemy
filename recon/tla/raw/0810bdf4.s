.syntax unified
	.thumb
	.global Func_0810bdf4
	.thumb_func
Func_0810bdf4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r4, #192
	lsls r4, r4, #4
	ldr r0, .L_0810be30
	adds r4, #228
	movs r1, #14
	adds r2, r3, r4
.L_0810be0a:
	subs r1, #1
	strh r0, [r2]
	subs r2, #2
	cmp r1, #0
	bge .L_0810be0a
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #230
	adds r3, r3, r1
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #3
	strb r2, [r3]
	adds r1, #138
	ldr r0, .L_0810be34
	bl Scheduler_AddOrUpdateCallback
	b .L_0810be38
	.2byte 0x0000
.L_0810be30:
	.4byte 0x00000060
.L_0810be34:
	.4byte Func_0810bdb0
.L_0810be38:
	pop {pc}
	.2byte 0x0000
