#ifndef _KOWHAI_H_
#define _KOWHAI_H_

#include <stdint.h>

// base tree node entry
struct kowhai_node_t
{
    uint16_t type;
    uint16_t symbol;
    uint16_t count;
    uint16_t param1;
    uint16_t param2;
};

// tree node types
#define NODE_TYPE_BRANCH 0
#define NODE_TYPE_LEAF   1
#define NODE_TYPE_END    2

// leaf node types
#define LEAF_TYPE_SETTING 0
#define LEAF_TYPE_ACTION  1

// leaf settings types
#define SETTING_TYPE_CHAR     0
#define SETTING_TYPE_UCHAR    1
#define SETTING_TYPE_INT16    2
#define SETTING_TYPE_UINT16   3
#define SETTING_TYPE_INT32    4
#define SETTING_TYPE_UINT32   5
#define SETTING_TYPE_FLOAT    6
#define SETTING_TYPE_READONLY 0x8000

/* 
 * Return the size of a setting type
 */
int kowhai_get_setting_size(int setting_type);

/* Return the memory offset of a setting in the tree specified by
 * a symbol path (array of symbols)
 */
int kowhai_get_setting_offset(struct kowhai_node_t* tree, int num_symbols, uint16_t* symbols);

/* 
 * Return the memory size of a branch of settings
 */
int kowhai_get_branch_size(struct kowhai_node_t* tree);

/*
 * Get a single byte char setting specified by a symbol path from a settings buffer
 */
int kowhai_get_char(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, char* result);

/*
 * Get a 16 bit integer setting specified by a symbol path from a settings buffer
 */
int kowhai_get_int16(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int16_t* result);

/*
 * Get a 32 bit integer setting specified by a symbol path from a settings buffer
 */
int kowhai_get_int32(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int32_t* result);

/*
 * Get a 32 bit floating point setting specified by a symbol path from a settings buffer
 */
int kowhai_get_float(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, float* result);

/*
 * Set a single byte char setting specified by a symbol path in a settings buffer
 */
int kowhai_set_char(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, char value);

/*
 * Set a 16 bit integer setting specified by a symbol path in a settings buffer
 */
int kowhai_set_int16(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int16_t value);

/*
 * Set a 32 bit integer setting specified by a symbol path in a settings buffer
 */
int kowhai_set_int32(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int32_t value);

/*
 * Set a 32 bit floating point setting specified by a symbol path in a settings buffer
 */
int kowhai_set_float(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, float value);

#endif