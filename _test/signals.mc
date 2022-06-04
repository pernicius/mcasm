/* SIGNALS:  #signals {\n chip:bit..bit = id \n}
 *
 * what must be defined at that point:
 *  - nothing
 *
 * format:
 *  - chip:bit = identifier
 *  - chip:startbit..endbit = identifier
 * notes:
 *  - order doesn't matter
 *  - identifier must begin with an alphachar (a..z, A..Z)
 *  - identifier can contain any combination of a..z, A..Z, 0..9 and _
 */

 #signals {
    0:0     = valid         // 1 bit  - valid opcode
    0:1     = wb_rd_en      // 1 bit  - [stage:wb] enable rd
    0:2     = id_rs1_en     // 1 bit  - [stage:id] enable rs1
    0:3     = id_rs2_en     // 1 bit  - [stage:id] enable rs2
    0:4     = id_sel_a      // 1 bit  - [stage:id] ALU input a selector 0:xxx / 1:xxx
    0:5     = id_sel_b      // 1 bit  - [stage:id] ALU input b selector 0:xxx / 1:xxx
    0:6..8  = id_imm_ctrl   // 3 bits - [stage:id] how to decode the immediate bits
    1:0 = test;
}


/* DEFAULTS:  #defaults {\n id=value \n}
 *
 * what must be defined at that point:
 *  - #signals { ... }
 *
 * format:
 *  - signal = value
 * notes:
 *  - order doesn't matter
 *  - value can be decimal, hex(0x..) or binary(0b..)
 *  - any not given signal get '0' as default value
 */

#defaults {
    valid       = 0
    wb_rd_en    = 0
    id_rs1_en   = 0
    id_rs2_en   = 0
    id_sel_a    = 0
    id_sel_b    = 0
    id_imm_ctrl = 0b010
    test = 1
}
