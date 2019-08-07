#ifndef HN_STRINGCSS_HPP_INCLUDED
#define HN_STRINGCSS_HPP_INCLUDED


    #include "../HN_XML/HN_Xml.hpp"
    #include "../HN_Javascript/HN_Javascript.hpp"
    #include "HN_CSS.hpp"

    bool isNextCharacter(const char *buffer, int c, int &cur_pos);
    int getStyleValue(const char *buffer, std::string &value, int &cur_pos);
    int getStyleName(const char *buffer, std::string &name, int &cur_pos);
    std::vector<struct HN_Attribute *> *takeStyleAttributes(struct HN_Node *parent_node,
                                                            std::string &buffer, int &cur_pos);

    struct HN_StyleNode findSelector(const char *buffer, int &cur_pos);
    struct HN_Node *takeStyle(const char *buffer);
    bool harvestStyle(struct HN_Node *b_node, const char *buffer, int &cur_pos_t);

#endif // HN_STRINGCSS_HPP_INCLUDED
