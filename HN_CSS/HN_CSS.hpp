#ifndef HN_CSS_HPP_INCLUDED
#define HN_CSS_HPP_INCLUDED

    #include "../HN_XML/HN_Xml.hpp"
    #include "../HN_Javascript/HN_Javascript.hpp"

    bool isNextCharacter(FILE *f, int c);
    int getStyleValue(FILE *f, std::string &value, std::string &settings);
    int getStyleName(FILE *f, std::string &name);
    std::vector<struct HN_Attribute *> *takeStyleAttributes(struct HN_Node *parent_node, FILE *f);

    struct HN_StyleNode
    {
        int type;
        struct HN_Node *node;
    };

    struct HN_StyleNode findSelector(FILE *f);
    struct HN_Node *takeStyle(FILE *f);
    bool harvestStyle(struct HN_Node *b_node, FILE *f);

#endif // HN_CSS_HPP_INCLUDED
