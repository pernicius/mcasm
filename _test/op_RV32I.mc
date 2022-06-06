/* DEFINES:  #define identifier (value)
 *
 * what must be defined at that point:
 *  - nothing
 *
 * format:
 *  - identifier must begin with an alphachar (a..z, A..Z)
 *  - identifier can contain any combination of a..z, A..Z, 0..9 and _
 *  - value can be decimal, hex(0x..) or binary(0b..)
 */

#define fn3_JALR  (0b000)
#define fn3_BEQ   (0b000)
#define fn3_BNE   (0b001)
#define fn3_BLT   (0b100)
#define fn3_BGE   (0b101)
#define fn3_BLTU  (0b110)
#define fn3_BGEU  (0b111)
#define fn3_LB    (0b000)
#define fn3_LH    (0b001)
#define fn3_LW    (0b010)
#define fn3_LBU   (0b100)
#define fn3_LHU   (0b101)
#define fn3_SB    (0b000)
#define fn3_SH    (0b001)
#define fn3_SW    (0b010)
#define fn3_ADD   (0b000)
#define fn3_SUB   (0b000)
#define fn3_SLL   (0b001)
#define fn3_SLT   (0b010)
#define fn3_SLTU  (0b011)
#define fn3_SRL   (0b101)
#define fn3_SRA   (0b101)
#define fn3_XOR   (0b100)
#define fn3_OR    (0b110)
#define fn3_AND   (0b111)
#define fn3_FENCE (0b000)
#define fn3_PRIV  (0b000)


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

//__op___________...
#op(op_LUI,      * ) { LUI
    valid
}

//__op___________...
#op(op_AUIPC,    * ) { AUIPC
    valid
}

//__op___________...
#op(op_JAL,      * ) { JAL
    valid
}

//__op___________fn3__________...
#op(op_JALR,     fn3_JALR,    * ) { JALR
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BEQ,     * ) { BEQ
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BNE,     * ) { BNE
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BLT,     * ) { BLT
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BGE,     * ) { BGE
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BLTU,    * ) { BLTU
    valid
}

//__op___________fn3__________...
#op(op_BRANCH,   fn3_BGEU,    * ) { BGEU
    valid
}

//__op___________fn3__________...
#op(op_LOAD,     fn3_LB,      * ) { LB
    valid
}

//__op___________fn3__________...
#op(op_LOAD,     fn3_LH,      * ) { LH
    valid
}

//__op___________fn3__________...
#op(op_LOAD,     fn3_LW,      * ) { LW
    valid
}

//__op___________fn3__________...
#op(op_LOAD,     fn3_LBU,     * ) { LBU
    valid
}

//__op___________fn3__________...
#op(op_LOAD,     fn3_LHU,     * ) { LHU
    valid
}

//__op___________fn3__________...
#op(op_STORE,    fn3_SB,      * ) { SB
    valid
}

//__op___________fn3__________...
#op(op_STORE,    fn3_SH,      * ) { SH
    valid
}

//__op___________fn3__________...
#op(op_STORE,    fn3_SW,      * ) { SW
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_ADD,     * ) { ADDI
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_SLT,     * ) { SLTI
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_SLTU,    * ) { SLTIU
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_XOR,     * ) { XORI
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_OR,      * ) { ORI
    valid
}

//__op___________fn3__________...
#op(op_OP_IMM,   fn3_AND,     * ) { ANDI
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP_IMM,   fn3_SLL,     1,   0,    0,    * ) { SLLI
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP_IMM,   fn3_SRL,     1,   0,    0,    * ) { SRLI
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP_IMM,   fn3_SRA,     1,   0,    1,    * ) { SRAI
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_ADD,     1,   0,    0,    * ) { ADD
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SUB,     1,   0,    1,    * ) { SUB
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SLL,     1,   0,    0,    * ) { SLL
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SLT,     1,   0,    0,    * ) { SLT
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SLTU,    1,   0,    0,    * ) { SLTU
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_XOR,     1,   0,    0,    * ) { XOR
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SRL,     1,   0,    0,    * ) { SRL
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_SRA,     1,   0,    1,    * ) { SRA
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_OR,      1,   0,    0,    * ) { OR
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_AND,     1,   0,    0,    * ) { AND
    valid
}

//__op___________fn3__________...
#op(op_MISC_MEM, fn3_FENCE,   * ) { FENCE
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__rs2z__rs2:0__rxz__...
#op(op_SYSTEM, fn3_PRIV,      1,   0,    0,    1,    0,     1,   * ) { ECALL
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__rs2z__rs2:0__rxz__...
#op(op_SYSTEM, fn3_PRIV,      1,   0,    0,    1,    1,     1,   * ) { EBREAK
    valid
}
