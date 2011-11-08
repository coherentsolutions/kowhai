#include "kowhai.h"

#ifdef KOWHAI_DBG
#include <stdio.h>
#define KOWHAI_ERR "KOWHAI ERROR:"
#define KOWHAI_INFO "KOWHAI INFO: "
#endif

int kowhai_get_setting_size(int setting_type)
{
    setting_type = setting_type & (~SETTING_TYPE_READONLY);
    switch (setting_type)
    {
        case SETTING_TYPE_CHAR:
        case SETTING_TYPE_UCHAR:
            return 1;
        case SETTING_TYPE_INT16:
        case SETTING_TYPE_UINT16:
            return 2;
        case SETTING_TYPE_INT32:
        case SETTING_TYPE_UINT32:
        case SETTING_TYPE_FLOAT:
            return 4;
        default:
#ifdef KOWHAI_DBG
            printf(KOWHAI_ERR" unknown setting type: %d\n", setting_type);
#endif
            return -1;
    }
}

int _get_setting_offset(struct kowhai_node_t* tree, int num_symbols, uint16_t* symbols, int symbols_matched, int* finished, int* steps)
{
    int offset = 0;
    do
    {
        // increment step counter
        (*steps)++;

#ifdef KOWHAI_DBG
        printf(KOWHAI_INFO" tree->type: %d, tree->symbol: %d, tree->count: %d\n", tree->type, tree->symbol, tree->count);
#endif

        // match symbols here
        if (symbols_matched < num_symbols)
        {
            if (symbols[symbols_matched] == tree->symbol)
            {
                symbols_matched++;
#ifdef KOWHAI_DBG
                printf(KOWHAI_INFO" symbol match, tree->symbol: %d, symbols_matched: %d\n", tree->symbol, symbols_matched);
#endif
            }
        }

        // return offset if we have matched the symbols
        if (symbols_matched == num_symbols)
        {
#ifdef KOWHAI_DBG
            printf(KOWHAI_INFO" return offset: %d\n", offset);
#endif
            *finished = 1;
            return offset;
        }
        
        switch (tree->type)
        {
            case NODE_TYPE_BRANCH:
            {
                // recurse into branch
                int _steps = 0;
                int temp = _get_setting_offset(tree + 1, num_symbols, symbols, symbols_matched, finished, &_steps);
                if (temp == -1)
                {
#ifdef KOWHAI_DBG
                    printf(KOWHAI_ERR" branch parse failed, node symbol: %d, current offset: %d\n", tree->symbol, offset);
#endif
                    return -1;
                }
                if (!(*finished))
                    temp *= tree->count;
                offset += temp;
                *steps += _steps;

                // return offset if we have matched the symbols
                if (*finished)
                    return offset;
                else
                {
#ifdef KOWHAI_DBG
                    printf(KOWHAI_INFO" step tree: %d, current offset: %d\n", _steps, offset);
#endif
                    tree += _steps;
                }

                break;
            }
            case NODE_TYPE_END:
                // return out of branch
                return offset;
            case NODE_TYPE_LEAF:
                // append leaf settings to offset
                if (tree->param1 == LEAF_TYPE_SETTING)
                {
                    int temp = kowhai_get_setting_size(tree->param2);
                    offset += temp * tree->count;
                }
                break;
            default:
#ifdef KOWHAI_DBG
                printf(KOWHAI_ERR" unknown node type %d\n", tree->type);
#endif
                return -1;
        }

        // increment tree node pointer
        tree++;
    }
    while (1);
}

/* TODO, this function should take a tree count for safety */
int kowhai_get_setting_offset(struct kowhai_node_t* tree, int num_symbols, uint16_t* symbols)
{
    int finished = 0, steps = 0;
    return _get_setting_offset(tree, num_symbols, symbols, 0, &finished, &steps);
}

int _get_branch_size(struct kowhai_node_t* tree, int* steps)
{
    int size = 0;

    do
    {
        // increment step counter
        (*steps)++;

#ifdef KOWHAI_DBG
        printf(KOWHAI_INFO" tree->type: %d, tree->symbol: %d, tree->count: %d\n", tree->type, tree->symbol, tree->count);
#endif

        switch (tree->type)
        {
            case NODE_TYPE_BRANCH:
            {
                // recurse into branch
                int _steps = 0;
                int branch_size = _get_branch_size(tree + 1, &_steps);
                size += branch_size * tree->count;
                *steps += _steps;
                tree += _steps;
                break;
            }
            case NODE_TYPE_END:
                // return from branch
                return size;
            case NODE_TYPE_LEAF:
                // append leaf settings to size
                if (tree->param1 == LEAF_TYPE_SETTING)
                    size += kowhai_get_setting_size(tree->param2) * tree->count;
                break;
            default:
#ifdef KOWHAI_DBG
                printf(KOWHAI_ERR" unknown node type (#2) %d\n", tree->type);
#endif
                return -1;
        }
        
        // increment tree node pointer
        tree++;
    }
    while (1);
}

int kowhai_get_branch_size(struct kowhai_node_t* tree)
{
    int steps = 0;
    return _get_branch_size(tree + 1, &steps);
}

int kowhai_get_char(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, char* result)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        *result = *((char*)((char*)settings_buffer + offset));
        return 1;
    }
    return 0;
}

int kowhai_get_int16(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int16_t* result)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        *result = *((int16_t*)((char*)settings_buffer + offset));
        return 1;
    }
    return 0;
}

int kowhai_get_int32(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int32_t* result)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        *result = *((uint32_t*)((char*)settings_buffer + offset));
        return 1;
    }
    return 0;
}

int kowhai_get_float(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, float* result)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        *result = *((float*)((char*)settings_buffer + offset));
        return 1;
    }
    return 0;
}

int kowhai_set_char(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, char value)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        char* target_address = (char*)((char*)settings_buffer + offset);
        *target_address = value;
        return 1;
    }
    return 0;
}

int kowhai_set_int16(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int16_t value)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        int16_t* target_address = (int16_t*)((char*)settings_buffer + offset);
        *target_address = value;
        return 1;
    }
    return 0;
}

int kowhai_set_int32(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, int32_t value)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        uint32_t* target_address = (uint32_t*)((char*)settings_buffer + offset);
        *target_address = value;
        return 1;
    }
    return 0;
}

int kowhai_set_float(struct kowhai_node_t* tree, void* settings_buffer, int num_symbols, uint16_t* symbols, float value)
{
    int offset = kowhai_get_setting_offset(tree, num_symbols, symbols);
    if (offset != -1)
    {
        float* target_address = (float*)((char*)settings_buffer + offset);
        *target_address = value;
        return 1;
    }
    return 0;
}