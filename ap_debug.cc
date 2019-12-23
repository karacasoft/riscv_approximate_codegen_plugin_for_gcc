#include "ap_debug.h"

#include <tree-iterator.h>

#include <stdio.h>

void print_generic_tree(tree t, int indent) {
    int i, j;
    if(!t) {
        printf("NULL\n");
        return;
    }
    for(i = 0; i < indent / 2; i++) printf("| ");
    printf("%s   ", get_tree_code_name(TREE_CODE(t)));
    if(TREE_CODE(t) == BIND_EXPR) {
        int op_count = TREE_OPERAND_LENGTH(t);
        printf("operands: %d   ", op_count);
        printf("\n");
        for(j = 0; j < op_count; j++) {
            tree op = TREE_OPERAND(t, j);
            print_generic_tree(op, indent + 2);
        }
    } else if(TREE_CODE(t) == DECL_EXPR) {
        tree decl = DECL_EXPR_DECL(t);
        tree id = DECL_NAME(decl);
        int op_count = TREE_OPERAND_LENGTH(t);
        printf("operands: %d   ", op_count);
        printf("%s,", id ? IDENTIFIER_POINTER(id) : "<noname>");
        printf("attrib: %s", DECL_ATTRIBUTES(decl));
        printf("\n");
        print_generic_tree(decl, indent + 2);
        tree value = DECL_INITIAL(decl);
        print_generic_tree(value, indent + 4);
    } else if(TREE_CODE(t) == STATEMENT_LIST) {
        printf("\n");
        tree_stmt_iterator tree_it;
        
        for(tree_it = tsi_start(t); !tsi_end_p(tree_it); tsi_next(&tree_it)) {
            tree tree_stmt = tsi_stmt(tree_it);
            print_generic_tree(tree_stmt, indent + 2);
        }
        
    } else if(TREE_CODE(t) == VAR_DECL) {
        tree id = DECL_NAME(t);
        int op_count = TREE_OPERAND_LENGTH(t);
        printf("operands: %d   ", op_count);
        printf("%s", id ? IDENTIFIER_POINTER(id) : "<noname>");
        printf("\n");
    } else if(TREE_CODE(t) == BLOCK) {
        int bn = BLOCK_NUMBER(t);
        printf("block nr: %d   ", bn);
        printf("\n");
    } else if(TREE_CODE(t) == INTEGER_CST) {
        int a = TREE_INT_CST_LOW(t);
        printf(": %d  ", a);
        printf("\n");
    } else if(TREE_CODE(t) == ASM_EXPR) {
        printf("%s     ", TREE_STRING_POINTER(ASM_STRING(t)));
        tree outputs = ASM_OUTPUTS(t);
        tree inputs = ASM_INPUTS(t);
        printf("\n");
        print_generic_tree(outputs, indent + 2);
        print_generic_tree(inputs, indent + 2);
    } else if(TREE_CODE(t) == TREE_LIST) {
        tree purpose = TREE_PURPOSE(t);
        tree value = TREE_VALUE(t);
        printf("\n");
        printf("Purpose:\n"); print_generic_tree(purpose, indent + 2);
        printf("Value:\n"); print_generic_tree(value, indent + 2);
    }

    else {
        printf("\n");
    }
    
    
   
}