#ifndef HN_SNODE_HPP_INCLUDED
#define HN_SNODE_HPP_INCLUDED

    // Depending file
    #include "HN_FNode.hpp"
    #include "../../HN_CSS/HN_StringCSS.hpp"

    // Prototypes
    struct HN_Node *findNode(const char *buffer_t, int *cur_pos_t);
    struct HN_Node *takeNode(const char *buffer_t, int *cur_pos_t);
    bool isEndOfNode
                (
                    const char *buffer_t,
                    const std::string &name,
                    int &cur_pos_t
                );

#endif // HN_SNODE_HPP_INCLUDED
