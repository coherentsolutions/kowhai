#ifndef _KOWHAI_H_
#define _KOWHAI_H_

#include <stdint.h>

#pragma pack(1)

/**
 * @brief basic types found in a tree descriptor
 * @todo should we namespace these
 */
enum kowhai_node_type
{
    // meta tags to denote structure
    KOW_BRANCH_START = 0x0000,
    KOW_BRANCH_END,

    // types to describe buffer layout
    KOW_INT8 = 0x0070,        ///@todo make this a reasonable balance
    KOW_UINT8,
    KOW_INT16,
    KOW_UINT16,
    KOW_INT32,
    KOW_UINT32,
    KOW_FLOAT,
};

/**
 * @brief base tree descriptor node entry
 */
struct kowhai_node_t
{
    uint16_t type;          ///< what is this node
    uint16_t symbol;        ///< index to a name for this node
    uint16_t count;         ///< if this is an array, this is the number of elements in the array (otherwise 1)
    uint16_t tag;           ///< user defined tag
};

/**
 * @brief a list of these (a path) will specify a unique address in the tree
 */
union kowhai_symbol_t
{
    uint32_t symbol;            ///< symbol of this node (really this is only 16bits max)
    struct
    {
        uint16_t name;          ///< symbol of this node
        uint16_t array_index;   ///< zero based array index of this node
    } parts;
};
#define KOWHAI_SYMBOL(name, array_index) ((array_index << 16) + name)

#pragma pack()

#define KOW_STATUS_OK                       0
#define KOW_STATUS_INVALID_SYMBOL_PATH      1
#define KOW_STATUS_INVALID_DESCRIPTOR       2
#define KOW_STATUS_INVALID_OFFSET           3
#define KOW_STATUS_NODE_DATA_TOO_SMALL      4
#define KOW_STATUS_INVALID_NODE_TYPE        5
#define KOW_STATUS_PACKET_BUFFER_TOO_SMALL  6
#define KOW_STATUS_INVALID_PROTOCOL_COMMAND 7
#define KOW_STATUS_PACKET_BUFFER_TOO_BIG    8

/**
 * @brief return the size for a given node type
 * @param type, a node type to find the size of
 * @return the size in bytes
 */
int kowhai_get_node_type_size(uint16_t type);

/** 
 * @brief find a item in the tree given its path
 * @param node, to start searching from for the given item
 * @param num_symbols, number of items in the symbols path (@todo should we just terminate the path instead)
 * @param symbols, the path of the item to find
 * @param offset, set to number of bytes from the current branch to the item
 * @param target_node, if return is successful this is the node that matches the symbol path
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_node(const struct kowhai_node_t *node, int num_symbols, const union kowhai_symbol_t *symbols, uint16_t *offset, struct kowhai_node_t **target_node);

/**
 * @brief calculate the complete size of a node including all the sub-elements and array items.
 * @param node to find the size of
 * @param size size of the node in bytes
 * @return kowhai status
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_node_size(const struct kowhai_node_t *node, int *size);

/**
 * @brief calculate the complete count of nodes including any child nodes up to a branch end, ie count all the child nodes + this one - branch end nodes
 * @param node start counting from here
 * @param count number of child nodes + this node - branch end nodes
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_node_count(const struct kowhai_node_t *node, int *count);

/**
 * Read from a tree data buffer starting at a symbol path
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to read the value from, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the read from (not including the read_offset below)
 * @param read_offset, the offset into the node data to start reading from
 * @param result, the buffer to read the result into
 * @param read_size, the number of bytes to read into the result
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_read(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int read_offset, void* result, int read_size);

/**
 * Write to a tree data buffer starting at a symbol path
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to write the values in to, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the write from (not including the wirte_offset below)
 * @param write_offset, the offset into the node data to start writing at
 * @param value, the buffer to write from
 * @param write_size, the number of bytes to write into the settings buffer
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_write(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int write_offset, void* value, int write_size);

/**
 * @brief Get a single byte char setting specified by a symbol path from a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to read the value from, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param result, the value of the node if found
 * @return kowhia status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_int8(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int8_t* result);

/**
 * @brief Get a 16 bit integer setting specified by a symbol path from a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to read the value from, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param result, the value of the node if found
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_int16(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int16_t* result);

/**
 * @brief Get a 32 bit integer setting specified by a symbol path from a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to read the value from, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param result, the value of the node if found
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_int32(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int32_t* result);

/**
 * @brief Get a 32 bit floating point setting specified by a symbol path from a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to read the value from, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param result, the value of the node if found
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_get_float(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, float* result);

/**
 * @brief Set a single byte char setting specified by a symbol path in a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to write the values in to, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param value, the new value to change the node to
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_set_char(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, char value);

/**
 * @brief Set a 16 bit integer setting specified by a symbol path in a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to write the values in to, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param value, the new value to change the node to
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_set_int16(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int16_t value);

/**
 * @brief Set a 32 bit integer setting specified by a symbol path in a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to write the values in to, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param value, the new value to change the node to
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_set_int32(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, int32_t value);

/**
 * @brief Set a 32 bit floating point setting specified by a symbol path in a settings buffer
 * @param tree_descriptor, collection of kowhai_node_t's that describe the layout of the tree_data
 * @param tree_data, binary dump of the data described by tree_descriptor to write the values in to, ie values of a structure or other object
 * @param num_symbols, number of symbols that make up the symbols path below
 * @param symbols, a collection of symbols that forms a path to the node to start the get from
 * @param value, the new value to change the node to
 * @return kowhai status value, ie KOW_STATUS_OK on success or other on error
 */
int kowhai_set_float(struct kowhai_node_t* tree_descriptor, void* tree_data, int num_symbols, union kowhai_symbol_t* symbols, float value);

#endif

