#ifndef __APPROX_PASS_H__
#define __APPROX_PASS_H__

#include <gcc-plugin.h>
#include <context.h>
#include <tree-pass.h>

static const char *xadd_asm_string = ".insn r OP, 0, 64, %0, %1, %2";
static const char *xsub_asm_string = ".insn r OP, 0, 96, %0, %1, %2";
static const char *xmul_asm_string = ".insn r OP, 0, 65, %0, %1, %2";

static const struct pass_data approximator_pass_data = {
    .type = GIMPLE_PASS,
    .name = "approximator_pass",
    .optinfo_flags = OPTGROUP_NONE,
    .tv_id = TV_NONE,
    .properties_required = PROP_cfg,
    .properties_provided = 0,
    .properties_destroyed = 0,
    .todo_flags_start = 0,
    .todo_flags_finish = 0
};

class approx_pass : public gimple_opt_pass {
public:
    approx_pass();
    unsigned int execute(function *func);
};

void approx_pre_genericize(void *, void *);

#endif // __APPROX_PASS_H__
