#ifndef __APPROX_ATTRIBUTE_H__
#define __APPROX_ATTRIBUTE_H__

#include <gcc-plugin.h>
#include <tree.h>

#define APPROX_ATTRIBUTE_NAME "approx"

tree handle_approx_attribute(tree *node, tree name, tree args, int flags, bool *no_add_attrs);

static const struct attribute_spec approx_attribute_spec = {
    APPROX_ATTRIBUTE_NAME,
    0, // min_arg_count
    0, // max_arg_count
    true, //decl_required,
    false, //type_required
    false, //func_required
    false,
    handle_approx_attribute,
    NULL
};

void register_approx_attribute(void *, void *);

#endif // __APPROX_ATTRIBUTE_H__
