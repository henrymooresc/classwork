lw $at, 0x0($zero)
lw $v0, 0x4($zero)
lw $v1, 0x8($zero)
add $a1, $at, $v0
beq $v1, $a1, 0x2
add $a1, $a1, $a1
sw $a1, 0xc($zero)
