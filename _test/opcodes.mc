/* DEFINES:  #define identifier (value)
 *
 * what must be defined at that point:
 *  - nothing
 *
 * format:
 *  - identifier must begin with an alphachar (a..z, A..Z)
 *  - identifier can contain any combination of a..z, A..Z, 0..9 and _
 *  - value can be a decimal, hex(0x..) or binary(0b..)
 */

#define op_OP     (0b01100)
#define op_OP_IMM (0b00100)
#define op_BRANCH (0b11000)

#define fn3_ADD  (0b000)
#define fn3_SUB  (0b000)
#define fn3_SLLI (0b001)
#define fn3_BEQ  (0b000)


/* OPCODES:  #op(inputs, ..) { opname\n signals,.. \n}
 *
 * what must be defined at that point:
 *  - #inputs { ... }
 *  - #signals { ... }
 *  - #defaults { ... }  - (optional)
 *  - #define ...        - (optional)
 *
 * notes:
 *  - (optional) you can set a opcode name after '{' ... (useful for debugging)
 *
 * format of inputs:
 *  - MUST be ordered the same way as defined in #inputs{}
 *  - can be a decimal, hex(0x..) or binary(0b..) value
 *  - can be a defined identifier
 *  - can be 'x' which marks the input as ignored
 *  - can be '*' which marks all following inputs as ignored
 *
 * format of signals:
 *  - order doesn't matter
 *  - they must be ',' separated
 *  - multiple lines are possible
 *  - if only the identifier is given, then it will be set all bits to '1'
 *  - if the identifier is prefixed with '!' the binary inverted default value will be used (0>1, 0b101>0b010)
 *  - identifier=value sets the signal to the given value
 *  - all ungiven signals will be set to their default value
 */

#op(op_OP, fn3_ADD, 1, 0, 0, *) { ADD
    valid
    wb_rd_en, id_rs1_en, id_rs2_en
}

#op(0b01100, fn3_SUB, 1, 1, 0, *) { SUB
    valid=1,
    wb_rd_en, id_rs1_en, id_rs2_en
}

#op(op_OP_IMM, fn3_SLLI, 1, 0, 0, *) { SLLI
    !valid,
    wb_rd_en, id_rs1_en
}

#op(op_BRANCH, fn3_BEQ, *) { BEQ
    valid
    id_rs1_en, id_rs2_en
    id_imm_ctrl=0b101
}
