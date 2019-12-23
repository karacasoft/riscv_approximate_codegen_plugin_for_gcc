#ifndef __APPROX_PRAGMA_H__
#define __APPROX_PRAGMA_H__ 1

#include <gcc-plugin.h>

static const char *xadd_marker_start = "# $$ADD_APPROX_START_MARKER";
static const char *xadd_marker_end   = "# $$ADD_APPROX_END_MARKER";
static const char *xsub_marker_start = "# $$SUB_APPROX_START_MARKER";
static const char *xsub_marker_end   = "# $$SUB_APPROX_END_MARKER";
static const char *xmul_marker_start = "# $$MUL_APPROX_START_MARKER";
static const char *xmul_marker_end   = "# $$MUL_APPROX_END_MARKER";

/**
 * Handler function for xadd varant of our pragma.
 * Marks beginning and end of the xadd sections.
 * This allows the plugin pass to find the marks and
 * convert addition expressions into xadd statements.
 * 
 * Usage:
 * ```
 *     #pragma APPROX xadd start
 *     int a = 3 + 4;
 *     #pragma APPROX xadd end
 * ```
 * 
 * When an error occurs, plugin ignores the directive
 * and issues a warning.
 */
void approx_xadd_handler(struct cpp_reader *);

/**
 * Handler function for xsub varant of our pragma.
 * Marks beginning and end of the xsub sections.
 * This allows the plugin pass to find the marks and
 * convert addition expressions into xsub statements.
 * 
 * Usage:
 * ```
 *     #pragma APPROX xsub start
 *     int a = 150 - 100;
 *     #pragma APPROX xsub end
 * ```
 * 
 * When an error occurs, plugin ignores the directive
 * and issues a warning.
 */
void approx_xsub_handler(struct cpp_reader *);

/**
 * Handler function for xmul varant of our pragma.
 * Marks beginning and end of the xmul sections.
 * This allows the plugin pass to find the marks and
 * convert addition expressions into xmul statements.
 * 
 * Usage:
 * ```
 *     #pragma APPROX xmul start
 *     int a = 150 * 100;
 *     #pragma APPROX xmul end
 * ```
 * 
 * When an error occurs, plugin ignores the directive
 * and issues a warning.
 */
void approx_xmul_handler(struct cpp_reader *);

/**
 * Register callback for GCC plugin. Called once by the plugin
 * to register the pragmas.
 */
void register_approx_pragmas(void *, void *);

#endif // __APPROX_PRAGMA_H__
