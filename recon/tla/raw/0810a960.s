.syntax unified
	.thumb
	.global Shop_MsgByMode
	.thumb_func
Shop_MsgByMode:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #129
	lsls r2, r2, #3
	adds r2, #255
	adds r3, r3, r2
	movs r1, #0
	ldrsb r1, [r3, r1]
	cmp r1, #1
	bne .L_0810a982
	ldr r3, .L_0810a99c
	ldr r2, .L_0810a9a0
	subs r3, r3, r2
	adds r0, r0, r3
.L_0810a982:
	cmp r1, #2
	bne .L_0810a98e
	ldr r3, .L_0810a9a4
	ldr r2, .L_0810a9a0
	subs r3, r3, r2
	adds r0, r0, r3
.L_0810a98e:
	cmp r1, #3
	bne .L_0810a99a
	ldr r3, .L_0810a9a8
	ldr r2, .L_0810a9a0
	subs r3, r3, r2
	adds r0, r0, r3
.L_0810a99a:
	pop {pc}
.L_0810a99c:
	.4byte 0x000012df
.L_0810a9a0:
	.4byte 0x000012d5
.L_0810a9a4:
	.4byte 0x000012e9
.L_0810a9a8:
	.4byte 0x000012f3
