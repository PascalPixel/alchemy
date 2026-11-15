@ Why assembly: it zeroes r2 and r4 and branches into Sound_ResamplerRecord1
@ with no return; the records it enters take their samples in ip and fp and
@ their volumes in r1 and r6, a linkage no procedure-call standard produces.
.syntax unified
	.arm
	.global Sound_ResamplerRecords
Sound_ResamplerRecords:
	mov	r2, #0
	mov	r4, #0
	b	Sound_ResamplerRecord1
