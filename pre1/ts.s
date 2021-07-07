	# ts.s file:
	.globl getebp
getebp:
	movl %ebp, %eax
	ret
