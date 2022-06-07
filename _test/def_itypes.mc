/* MACROS:  #define identifier { ... }
 *
 * what must be defined at that point:
 *  - nothing
 *
 * format:
 *  - identifier must begin with an alphachar (a..z, A..Z)
 *  - identifier can contain any combination of a..z, A..Z, 0..9 and _
 *  - content can be anything used in #OP signals
 */

#define iType_I {
    imm_ctrl = 0b000
    id_rs1_en
    wb_rd_en
    id_sel_a = 1  // Rs1
    id_sel_b = 1  // imm
}

#define iType_S {
    imm_ctrl = 0b001
    id_rs1_en
    id_rs2_en
    id_sel_a = 1  // Rs1
    id_sel_b = 1  // imm
}

#define iType_B {
    imm_ctrl = 0b010
    id_rs1_en
    id_rs2_en
}

#define iType_U {
    imm_ctrl = 0b011
    wb_rd_en
}

#define iType_J {
    imm_ctrl = 0b100
    wb_rd_en
}

#define iType_R {
    imm_ctrl = 0b101
    id_rs1_en
    id_rs2_en
    wb_rd_en
    id_sel_a = 1  // Rs1
    id_sel_b = 0  // Rs2
}
