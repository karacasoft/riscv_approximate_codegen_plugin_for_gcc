#include "approx_pragma.h"

#include <tree.h>
#include <tree-core.h>
#include <tree-iterator.h>
#include <stringpool.h>
#include <c-family/c-common.h>
#include <c-family/c-pragma.h> 

enum marker_type {
    XADD,
    XSUB,
    XMUL
};

void approx_add_marker(struct cpp_reader *reader, enum marker_type type);

void approx_add_marker(struct cpp_reader *reader, enum marker_type type) {
    const char *start_marker = (type == XADD) ? xadd_marker_start :
                               (type == XSUB) ? xsub_marker_start :
                               (type == XMUL) ? xmul_marker_start :
                               NULL;
    const char *end_marker = (type == XADD) ? xadd_marker_end :
                             (type == XSUB) ? xsub_marker_end :
                             (type == XMUL) ? xmul_marker_end :
                             NULL;
    enum cpp_ttype token;
    tree x;
    token = pragma_lex(&x);
    if(token == CPP_NAME) {
        const char *id = IDENTIFIER_POINTER(x);
        tree stmt_list = cur_stmt_list;
        tree_stmt_iterator tsi;
        tsi = tsi_last(stmt_list);
        if(!strcmp(id, "start")) {
            tree marker_start = build_stmt(input_location, ASM_EXPR, build_string(27, start_marker), NULL_TREE, NULL_TREE, NULL_TREE, NULL_TREE);
            tsi_link_after(&tsi, marker_start, TSI_NEW_STMT);
        } else if(!strcmp(id, "end")) {
            tree marker_end = build_stmt(input_location, ASM_EXPR, build_string(25, end_marker), NULL_TREE, NULL_TREE, NULL_TREE, NULL_TREE);
            tsi_link_after(&tsi, marker_end, TSI_NEW_STMT);
        } else {
            cpp_warning_with_line(reader, CPP_W_NONE, input_location, 0, "Unknown command for xadd pragma %s", id);
        }

    } else {
        cpp_warning_with_line(reader, CPP_W_NONE, input_location, 0, "Malformed xadd pragma, ignoring it.");
    }
}

void approx_xadd_handler(struct cpp_reader *reader) {
    approx_add_marker(reader, XADD);
}

void approx_xsub_handler(struct cpp_reader *reader) {
    approx_add_marker(reader, XSUB);
}

void approx_xmul_handler(struct cpp_reader *reader) {
    approx_add_marker(reader, XMUL);
}

void register_approx_pragmas(void *gcc_data, void *user_data) {
    c_register_pragma("APPROX", "xadd", approx_xadd_handler);
    c_register_pragma("APPROX", "xsub", approx_xsub_handler);
    c_register_pragma("APPROX", "xmul", approx_xmul_handler);
}