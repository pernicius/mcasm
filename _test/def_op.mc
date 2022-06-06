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

#define op_LOAD     (0b00000)
#define op_MISC_MEM (0b00011)
#define op_OP_IMM   (0b00100)
#define op_AUIPC    (0b00101)
#define op_STORE    (0b01000)
#define op_OP       (0b01100)
#define op_LUI      (0b01101)
#define op_BRANCH   (0b11000)
#define op_JALR     (0b11001)
#define op_JAL      (0b11011)
#define op_SYSTEM   (0b11100)
