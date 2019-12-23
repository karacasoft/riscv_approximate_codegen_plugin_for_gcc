#ifndef __AP_DEBUG_H__
#define __AP_DEBUG_H__ 1

#include <gcc-plugin.h>
#include <tree-core.h>
#include <tree.h>

/* 
 * Prints a given generic tree node and its sub nodes
 * It's not fully implemented and might have bugs. Use with
 * caution.
 */
void print_generic_tree(tree, int);

#endif // __AP_DEBUG_H__
