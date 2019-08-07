#ifndef HN_NODE_HPP_INCLUDED
#define HN_NODE_HPP_INCLUDED

    // Include headers
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
    #include <string>
    #include <deque>
    #include <fstream>
    #include "../HN_Attributes/HN_Attributes.hpp"
    #include "../../HN_Javascript/HN_Javascript.hpp"

    // Structure of an attribute
    /*struct HN_Attribute
    {
        std::string name;
        std::string value;
        std::string settings;
    };*/

    // Structure of a node
    struct HN_Node
    {
        HN_Node();
        bool simple;
        bool attr_exist;
        bool childNode_exist;
        bool loading;
        bool unlimited;
        bool dtdNode;
        bool commentNode;
        std::string name, content, settings;
        std::vector<struct HN_Attribute *>      *attributes;
        std::deque<struct HN_Node *>            children;
        std::deque<struct HN_JSInstruction *>   instructions;

        // Getting methods
        std::string getAttribute(std::string name, bool sensitive = false);
        std::string getAttributeName(std::string value, bool sensitive = false);
        int countChildNode(std::string _nodeName, bool sensitive = false);
        bool isEmpty(void);
        bool isAttributeExist(std::string attr_name, bool sensitive = false);
        void deleteAttribute(std::string attr_name, bool sensitive = false);
        size_t erase(size_t curPos);

        // Setting methods
        bool setAttribute(std::string attr_name, std::string new_value,
                            bool sensitive = false);
        void addAttribute(std::string attr_name, std::string new_value);
        struct HN_Node *getChildNode(std::string name, bool recursive = true,
                                        bool sensitive = false);
        struct HN_Node *getChildNode(size_t curPos);
        struct HN_Node *getChildByAttribute
                                (
                                    std::string node_name, std::string attr_name,
                                    std::string attr_value, bool recursive = true,
                                    bool sensitive = false
                                );
        struct HN_Node *getChildByContent
                                (
                                    std::string content, bool recursive = true,
                                    bool sensitive = false
                                );

        // Managing data
        void saveNode(FILE *f, int level = 0, bool indent = true);
        void saveNode(std::ofstream &file, int level = 0);
        void saveNode(std::string path, int level = 0, bool indent = true);

        // Managing data
        void saveStyle(std::ofstream &file);
        void saveStyle(std::string path);

        // Adding children by file
        void addChildren(FILE *f, bool in_front = false);
        void addChildren(std::string path, bool in_front = false);
        void addEChildren(FILE *f, bool in_front = false);
        void addEChildren(std::string path, bool in_front = false);

        virtual ~HN_Node();
    };

    // Structure of a web file
    struct HN_WebFile
    {
        HN_WebFile();
        FILE *f;
        std::string address;
        ~HN_WebFile();
    };

    // Some quick particular functions
    void initStringAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    std::string     attr_value
                );
    void initIntegerNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    int             attr_value
                );
    void initLongNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    long            attr_value
                );
    void initDoubleNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    double          attr_value
                );

#endif // HN_NODE_H_INCLUDED
