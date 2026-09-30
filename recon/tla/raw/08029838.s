.syntax unified
	.thumb
	.global Func_08029838
	.thumb_func
Func_08029838:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	adds r6, r0, #0
	movs r0, #0
	cmp r5, #0
	beq .L_080298b6
	ldr r1, .L_080298b8
	ldr r4, .L_080298bc
	movs r2, #141
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802987e
	movs r2, #140
	lsls r2, r2, #2
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_0802987e
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #54
	adds r3, r1, r2
	ldrh r2, [r3]
	ldr r3, [r4, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08029880
.L_0802987e:
	movs r0, #1
.L_08029880:
	cmp r0, #0
	beq .L_080298b4
	adds r0, r6, #0
	bl Func_080c8828
	cmp r0, #0
	beq .L_08029896
	movs r0, #113
	bl Audio_PlayCue
	b .L_080298b4
.L_08029896:
	movs r0, #163
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080298b4
	movs r3, #179
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #128
	lsls r3, r3, #6
	adds r3, #139
	strh r3, [r2]
	movs r0, #1
	b .L_080298b6
.L_080298b4:
	movs r0, #0
.L_080298b6:
	pop {r5, r6, pc}
.L_080298b8:
	.4byte gPartyState
.L_080298bc:
	.4byte gInput
