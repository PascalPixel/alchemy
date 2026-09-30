.syntax unified
	.thumb
	.global Func_0803e998
	.thumb_func
Func_0803e998:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #72]
	adds r7, r0, #0
.L_0803e9a2:
	movs r0, #1
	bl WaitFrames
	movs r1, #232
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_0803e9a2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	cmp r7, r2
	beq .L_0803ea34
	ldr r6, .L_0803ea50
	movs r2, #16
	ldr r3, [r6, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0803e9d8
	movs r0, #111
	bl Audio_PlayCue
	adds r0, r5, #0
	bl Func_0803ebdc
	b .L_0803e9ee
.L_0803e9d8:
	ldr r3, [r6, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0803e9ee
	movs r0, #111
	bl Audio_PlayCue
	adds r0, r5, #0
	bl Func_0803ed98
.L_0803e9ee:
	ldr r3, [r6, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0803ea34
	movs r1, #231
	lsls r1, r1, #2
	adds r3, r5, r1
	adds r1, #2
	ldrh r2, [r3]
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r6, r2, r3
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r3, [r3]
	ldrh r3, [r3, #10]
	cmp r3, #6
	bne .L_0803ea2a
	cmp r6, #0
	bne .L_0803ea22
	movs r0, #112
	bl Audio_PlayCue
	b .L_0803ea30
.L_0803ea22:
	movs r0, #113
	bl Audio_PlayCue
	b .L_0803ea30
.L_0803ea2a:
	movs r0, #112
	bl Audio_PlayCue
.L_0803ea30:
	adds r0, r6, #0
	b .L_0803ea4e
.L_0803ea34:
	cmp r7, #0
	beq .L_0803e9a2
	ldr r3, .L_0803ea50
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0803e9a2
	movs r0, #113
	bl Audio_PlayCue
	movs r0, #1
	negs r0, r0
.L_0803ea4e:
	pop {r5, r6, r7, pc}
.L_0803ea50:
	.4byte gInput
