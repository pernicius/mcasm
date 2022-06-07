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

#define fn3_CSRRW  (0b001)
#define fn3_CSRRS  (0b010)
#define fn3_CSRRC  (0b011)
#define fn3_CSRRWI (0b101)
#define fn3_CSRRSI (0b110)
#define fn3_CSRRCI (0b111)


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
 *  - can be a macro identifier
 *  - if only the identifier is given, then it will be set all bits to '1'
 *  - if the identifier is prefixed with '!' the binary inverted default value will be used (0>1, 0b101>0b010)
 *  - identifier=value sets the signal to the given value
 *  - all ungiven signals will be set to their default value
 */

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRW,   * ) { CSRRW
    valid
}

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRS,   * ) { CSRRS
    valid
}

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRC,   * ) { CSRRC
    valid
}

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRWI,  * ) { CSRRWI
    valid
}

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRSI,  * ) { CSRRSI
    valid
}

//__op___________fn3__________...
#op(op_SYSTEM,   fn3_CSRRCI,  * ) { CSRRCI
    valid
}
