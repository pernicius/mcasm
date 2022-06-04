/* INPUTS:  #inputs {\n bit..bit = id \n}
 *
 * what must be defined at that point:
 *  - nothing
 *
 * format:
 *  - bit = name
 *  - startbit..endbit = name
 * notes:
 *  - order DOES matter!!!
 *  - order is relevant in #op
 */

#inputs {
    0..4 = op       // 5 bits opcode
    5..7 = fn3      // 3 bits fn3
    8    = fn7zero  // fn7       (0b0x0000x)
    9    = fn7b5    // fn7[bit5] (0bx1xxxxx)
    10   = fn7b0    // fn7[bit0] (0bxxxxxx1)
}

/*
RISC-V instruction:

     3322222222221111111111
bit: 10987654321098765432109876543210
     |     ||   ||   || ||   ||   |^^ instr.length ('11' for 32bit instruction)
     |     ||   ||   || ||   |^^^^^ opcode
     |     ||   ||   || |^^^^^ rd
     |     ||   ||   |^^^ fn3
     |     ||   |^^^^^ rs1
     |     |^^^^^ rs2
     ^^^^^^^ fn7
*/