#include "approx_attribute.h"

#include <c-family/c-common.h>
#include <c-family/c-pragma.h>

tree handle_approx_attribute(tree *node, tree name, tree args, int flags, bool *no_add_attrs) {
    if(TREE_CODE(*node) != FUNCTION_DECL) {
        cpp_warning(parse_in, CPP_W_NONE, "Attribute \"" APPROX_ATTRIBUTE_NAME "\" must be given to a function_decl node. The attribute is ignored.");
    }
    return NULL_TREE;
}

void register_approx_attribute(void *, void *) {
    register_attribute(&approx_attribute_spec);
}