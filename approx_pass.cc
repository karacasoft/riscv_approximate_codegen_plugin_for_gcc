#include "approx_pass.h"
#include "ap_debug.h"
#include "approx_pragma.h"
#include "approx_attribute.h"

#include <cstdio>

#include <stringpool.h>
#include <attribs.h>
#include <gimple.h>
#include <gimple-iterator.h>
#include <gimple-pretty-print.h>

#include <tree-ssa.h>
#include <tree-ssa-operands.h>
#include <tree-ssa-loop-niter.h>

#include <c-family/c-common.h>
#include <c-family/c-pragma.h>

void transform_statement(gimple *g, gasm **ret, const char *asm_string);
void transform_add_statement(gimple *g, gasm **ret);
void transform_sub_statement(gimple *g, gasm **ret);
void transform_mul_statement(gimple *g, gasm **ret);

approx_pass::approx_pass() : gimple_opt_pass(approximator_pass_data, g) {}

void approx_pre_genericize(void *gcc_data, void *user_data) {
    tree fndecl = (tree) gcc_data;

    if(TREE_CODE(fndecl) == FUNCTION_DECL) {
        tree fnbody = DECL_SAVED_TREE(fndecl);
        tree attr = DECL_ATTRIBUTES(fndecl);
        
        if(lookup_attribute(APPROX_ATTRIBUTE_NAME, attr) != NULL_TREE) {
            //printf("Has 'approx' attribute   ");
        }

        //printf("%s\n", get_tree_code_name(FUNCTION_DECL));

        //print_generic_tree(fnbody, 2);
        
    }
}

void transform_statement(gimple *g, gasm **ret, const char *asm_string) {
    uint num_ops = gimple_num_ops(g);

    if(num_ops < 3) {
        printf("num_ops < 3 ???");
    }

    struct __asm_statement_io {
        vec<tree, va_gc, vl_embed> inputs;
        vec<tree, va_gc, vl_embed> outputs;
    } asm_statement_io;

    vec<tree, va_gc, vl_embed> *inputs = &asm_statement_io.inputs;
    vec<tree, va_gc, vl_embed> *outputs = &asm_statement_io.outputs;

    vec_alloc(inputs, 2);
    vec_alloc(outputs, 1);

    tree op1 = gimple_op(g, 1);
    tree op2 = gimple_op(g, 2);
    
    vec_safe_push(inputs, build_tree_list(build_tree_list(NULL_TREE, build_string(2, "r")), op1));
    vec_safe_push(inputs, build_tree_list(build_tree_list(NULL_TREE, build_string(2, "r")), op2));
    vec_safe_push(outputs, build_tree_list(build_tree_list(NULL_TREE, build_string(3, "=r")), gimple_op(g, 0)));

    gasm *__gasm = gimple_build_asm_vec(asm_string, inputs, outputs, NULL, NULL);
    
    vec_free(inputs);
    vec_free(outputs);

    *ret = __gasm;
}

void transform_add_statement(gimple *g, gasm **ret) {
    transform_statement(g, ret, xadd_asm_string);
}

void transform_sub_statement(gimple *g, gasm **ret) {
    transform_statement(g, ret, xsub_asm_string);
}

void transform_mul_statement(gimple *g, gasm **ret) {
    transform_statement(g, ret, xmul_asm_string);
}

void process_gimple_stmt(gimple *g, gimple_stmt_iterator *gsi, bool do_xadd, bool do_xsub, bool do_xmul) {
    tree lhs = gimple_get_lhs(g);
                
    uint num_ops = gimple_num_ops(g);
    
    gasm *_gasm = 0;
    if(is_gimple_assign(g)) {
        tree_code code = gimple_expr_code(g);
        if(do_xadd && code == PLUS_EXPR) {
            transform_add_statement(g, &_gasm);
        } else if(do_xsub && code == MINUS_EXPR) {
            transform_sub_statement(g, &_gasm);
        } else if(do_xmul && code == MULT_EXPR) {
            transform_mul_statement(g, &_gasm);
        }
        if(_gasm) {
            if(TREE_CODE(lhs) == SSA_NAME) {
                // Update the SSA def to new gimple asm statement.
                SSA_NAME_DEF_STMT(lhs) = _gasm;
            }
            gsi_replace(gsi, _gasm, GSI_SAME_STMT);
        }
    } else if(is_gimple_call(g)) {
        if(do_xadd || do_xsub || do_xmul) {
            cpp_warning_with_line(parse_in, CPP_W_NONE, gimple_location(g), 0,
                "Called function inside the approximable region will "
                "not be approximated. You may want to use __attribute__((approx))");
        }
    }
}

unsigned int approx_pass::execute(function *func) {
    
    if(lookup_attribute(APPROX_ATTRIBUTE_NAME, DECL_ATTRIBUTES(func->decl)) != NULL_TREE) {
        int i;
        basic_block bb;
        gimple_stmt_iterator gsi;
        FOR_EACH_BB_FN(bb, func) {
            
            for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
                gimple *g = gsi_stmt(gsi);
                process_gimple_stmt(g, &gsi, true, true, true);
            }
        }
    } else {
        int i;
        basic_block bb;
        gimple_stmt_iterator gsi;

        bool process_xadd_marker = false;
        bool process_xsub_marker = false;
        bool process_xmul_marker = false;

        FOR_EACH_BB_FN(bb, func) {
            
            for(gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
                gimple *g = gsi_stmt(gsi);
                tree lhs = gimple_get_lhs(g);
                
                uint num_ops = gimple_num_ops(g);
                if(gimple_code(g) == GIMPLE_ASM) {
                    const char *str = gimple_asm_string((gasm *)g);
                    if(!strcmp(str, xadd_marker_start)) {
                        process_xadd_marker = true;
                    } else if(!strcmp(str, xadd_marker_end)) {
                        process_xadd_marker = false;
                    } else if(!strcmp(str, xsub_marker_start)) {
                        process_xsub_marker = true;
                    } else if(!strcmp(str, xsub_marker_end)) {
                        process_xsub_marker = false;
                    } else if(!strcmp(str, xmul_marker_start)) {
                        process_xmul_marker = true;
                    } else if(!strcmp(str, xmul_marker_end)) {
                        process_xmul_marker = false;
                    }
                } else {
                    process_gimple_stmt(g, &gsi, process_xadd_marker, process_xsub_marker, process_xmul_marker);
                }
            }
        }
    }
    
    


    return 0;
}