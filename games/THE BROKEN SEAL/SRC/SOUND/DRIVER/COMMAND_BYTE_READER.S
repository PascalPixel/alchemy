@ Reads the next command byte of the track in r1 and advances its pointer,
@ returning the byte in r3. Its only callers, the setters in
@ COMMAND_BYTE_SETTERS.S, store r3 straight after the call; a C function
@ returns in r0, and neither approved compiler reads r3 after a call. Every
@ TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common sound command-byte reader and setters
.syntax unified
	.thumb

.global Func_080fa1c8
.thumb_func
Func_080fa1c8:
	ldr	r2, [r1, #64]
	adds	r3, r2, #1
	str	r3, [r1, #64]
	ldrb	r3, [r2, #0]
	bx	lr
