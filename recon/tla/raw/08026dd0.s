.syntax unified
	.thumb
	.global Func_08026dd0
	.thumb_func
Func_08026dd0:
	push {r5, r6, r7, lr}
	movs r7, #252
	lsls r7, r7, #6
	adds r7, #255
	movs r3, #192
	lsrs r6, r0, #14
	ands r7, r0
	movs r0, #8
	lsls r3, r3, #18
	adds r0, #255
	ldr r5, [r3, #108]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08026dfa
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #250
	strh r3, [r2]
	b .L_08026e58
.L_08026dfa:
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_08026e42
	ldr r1, .L_08026e5c
	subs r2, #138
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_08026e26
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #136
	strh r3, [r2]
	b .L_08026e58
.L_08026e26:
	ldr r3, [r1]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08026e58
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #135
	strh r3, [r2]
	b .L_08026e58
.L_08026e42:
	cmp r6, #0
	beq .L_08026e4c
	cmp r6, #1
	beq .L_08026e50
	b .L_08026e58
.L_08026e4c:
	movs r2, #179
	b .L_08026e52
.L_08026e50:
	movs r2, #180
.L_08026e52:
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r7, [r3]
.L_08026e58:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
.L_08026e5c:
	.4byte gInput
