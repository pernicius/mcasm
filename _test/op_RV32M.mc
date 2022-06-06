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

#define fn3_MUL    (0b000)
#define fn3_MULH   (0b001)
#define fn3_MULHSU (0b010)
#define fn3_MULHU  (0b011)
#define fn3_DIV    (0b100)
#define fn3_DIVU   (0b101)
#define fn3_REM    (0b110)
#define fn3_REMU   (0b111)


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

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_MUL,     1,   1,    0,    * ) { MUL
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_MULH,    1,   1,    0,    * ) { MULH
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_MULHSU,  1,   1,    0,    * ) { MULHSU
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_MULHU,   1,   1,    0,    * ) { MULHU
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_DIV,     1,   1,    0,    * ) { DIV
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_DIVU,    1,   1,    0,    * ) { DIVU
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_REM,     1,   1,    0,    * ) { REM
    valid
}

//__op___________fn3__________f7z__f7:0__f7:5__...
#op(op_OP,       fn3_REMU,    1,   1,    0,    * ) { REMU
    valid
}
