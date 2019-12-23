#include <gcc-plugin.h>
#include <plugin-version.h>

#include "approx_attribute.h"
#include "approx_pragma.h"
#include "approx_pass.h"

int plugin_is_GPL_compatible;

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    if(!plugin_default_version_check(version, &gcc_version)) {
        return 1;
    }

    struct register_pass_info info;

    info.pass = new approx_pass();
    info.reference_pass_name = "ssa";
    info.ref_pass_instance_number = 1;
    info.pos_op = PASS_POS_INSERT_BEFORE;

    register_callback("approx", PLUGIN_PRE_GENERICIZE, approx_pre_genericize, NULL);
    register_callback("approx", PLUGIN_PRAGMAS, register_approx_pragmas, NULL);
    register_callback("approx", PLUGIN_ATTRIBUTES, register_approx_attribute, NULL);
    register_callback("approx", PLUGIN_PASS_MANAGER_SETUP, NULL, &info);

    return 0;
}