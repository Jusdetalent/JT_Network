#include "HN_Node.hpp"
#include "HN_FNode.hpp"
#include <iostream>


 // Builder
HN_Node::HN_Node()
{
    this->simple            = false;
    this->attr_exist        = false;
    this->childNode_exist   = false;
    this->loading           = true;
    this->unlimited         = false;
    this->attributes        = NULL;
    this->dtdNode           = false;
    this->commentNode       = false;
}


// Getting methods
std::string HN_Node::getAttribute(std::string _name, bool sensitive)
{
    // Delete attributes
    if(this->attributes != NULL)
        {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            if(sensitive && attr->name == _name)
                return attr->value;
            else if(!sensitive && stricmp(attr->name.c_str(), _name.c_str()) == 0)
                return attr->value;
        }
    }

    return "";
}

std::string HN_Node::getAttributeName(std::string _value, bool sensitive)
{
    // Delete attributes
    if(this->attributes != NULL)
        {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            if(sensitive && attr->value == _value)
                return attr->name;
            else if(!sensitive && stricmp(attr->value.c_str(), _value.c_str()) == 0)
                return attr->name;
        }
    }

    return "";
}

int HN_Node::countChildNode(std::string _nodeName, bool sensitive)
{
    int n_len = 0;
    std::deque<struct HN_Node *>::iterator n_it;
    struct HN_Node *childNode;
    for
    (
        n_it = this->children.begin();
        n_it!= this->children.end();
        n_it++
    ){
        childNode = *n_it;
        if(sensitive && childNode->name == _nodeName)
            n_len++;
        else if(!sensitive && stricmp(childNode->name.c_str(), _nodeName.c_str()) == 0)
            n_len++;
    }

    return n_len;
}

bool HN_Node::isEmpty(void)
{
    int c_len = this->children.size();
    if(!this->simple            && !this->attr_exist
    && !this->childNode_exist   && this->attributes == NULL
    && c_len == 0)
        return true;
    else{
        return false;
    }

    return false;
}

void HN_Node::deleteAttribute(std::string attr_name, bool sensitive)
{
    // Find attributes
    if(this->attributes != NULL)
    {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            if(sensitive && attr->name == attr_name)
                {
                this->attributes->erase(it);
                delete attr;
                return;
            }
            else if(!sensitive && stricmp(attr->name.c_str(), attr_name.c_str()) == 0)
                {
                this->attributes->erase(it);
                delete attr;
                return;
            }
        }
    }
}

size_t HN_Node::erase(size_t curPos)
{
    // Build data
    std::deque<struct HN_Node *>::iterator it, begin_it = this->children.end();
    struct HN_Node *child;
    size_t available = 0;

    // Loop in children
    for
    (
        it = this->children.begin();
        it!= this->children.end();
        it++
    ){
        child = *it;

        if(available >= curPos)
            {
            if(begin_it == this->children.end())
                {
                begin_it = it;
            }
            delete child;
        }
        available++;
    }

    // Free unavailable data
    if(begin_it != this->children.end())
        {
        // Erase from begin to end
        this->children.erase
                (
                    begin_it,
                    this->children.end()
                );
    }

    // Return new size
    return this->children.size();
}


 // Setting methods
bool HN_Node::setAttribute(std::string attr_name, std::string new_value, bool sensitive)
{
    // Delete attributes
    if(this->attributes != NULL)
        {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            if(sensitive && attr->name == attr_name)
                {
                attr->value = new_value;
                return true;
            }
            else if(!sensitive && stricmp(attr->name.c_str(), attr_name.c_str()) == 0)
                {
                attr->value = new_value;
                return true;
            }
        }
    }

    // Return statement
    return false;
}

bool HN_Node::isAttributeExist(std::string attr_name, bool sensitive)
{
    // Delete attributes
    if(this->attributes != NULL)
        {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            if(sensitive && attr->name == attr_name)
                {
                return true;
            }
            else if(!sensitive && stricmp(attr->name.c_str(), attr_name.c_str()) == 0)
                {
                return true;
            }
        }
    }

    // Return statement
    return false;
}

void HN_Node::addAttribute
                    (
                        std::string attr_name,
                        std::string new_value
                    )
{
    // Allocate attribute if empty
    if(this->attributes == NULL)
        {
        this->attributes = new std::vector<struct HN_Attribute *>;
    }

    // Add attribute
    if(this->attributes != NULL)
        {
        struct HN_Attribute *
            attr = new struct HN_Attribute;

        if(attr != NULL)
            {
            attr->name = attr_name;
            attr->value= new_value;
            this->attributes->push_back(attr);
            this->attr_exist = true;
        }
    }
}

struct HN_Node *
    HN_Node::getChildNode(std::string node_name, bool recursive,
                                      bool sensitive)
{
    // Node to return
    struct HN_Node *node, *r_node;
    std::deque<struct HN_Node *>::iterator it;

    /*
     * Loop in children to find out
     * target node
     */
    for
        (
        it = this->children.begin();
        it!= this->children.end();
        it++
    ){
        // Try to return by here
        node = *it;
        if(sensitive && node->name == node_name)
            return node;
        else if(!sensitive && stricmp(node->name.c_str(), node_name.c_str()) == 0)
            {
            return node;
        }

        // Loop in children
        if((r_node = node->getChildNode(node_name, recursive, sensitive)) != NULL)
            return node;
    }

    // Can not arrive here
    return NULL;
}

struct HN_Node *
    HN_Node::getChildNode(size_t curPos)
{
   /*
    * Avoid overflow
    */
    if(curPos < this->children.size())
        {
        return this->children[curPos];
    }

    // Return empty node
    return NULL;
}

struct HN_Node *
    HN_Node::getChildByAttribute
                                (
                                    std::string node_name, std::string attr_name,
                                    std::string attr_value, bool recursive,
                                    bool sensitive
                                )
{
    // Node to return
    struct HN_Node *node, *r_node;
    std::deque<struct HN_Node *>::iterator it;

    /*
     * Loop in children to find out
     * target node
     */
    for
        (
        it = this->children.begin();
        it!= this->children.end();
        it++
    ){
        // Try to return by here
        node = *it;
        if(sensitive && node->name == node_name
        && node->getAttribute(attr_name) == attr_value)
            return node;
        else if(!sensitive && stricmp(node->name.c_str(), node_name.c_str()) == 0
            &&  stricmp(node->getAttribute(attr_name).c_str(), attr_value.c_str()) == 0)
            {
            return node;
        }

        // Loop in children
        if(
            (
                r_node = node->getChildByAttribute
                                (
                                    node_name,
                                    attr_name,
                                    attr_value,
                                    recursive,
                                    sensitive
                                )
             ) != NULL)
            return r_node;
    }

    // Can not arrive here
    return NULL;
}

struct HN_Node *
    HN_Node::getChildByContent
                                (
                                    std::string node_content, bool recursive,
                                    bool sensitive
                                )
{
    // Node to return
    struct HN_Node *node, *r_node;
    std::deque<struct HN_Node *>::iterator it;

    /*
     * Loop in children to find out
     * target node
     */
    for
        (
        it = this->children.begin();
        it!= this->children.end();
        it++
    ){
        // Try to return by here
        node = *it;
        if(sensitive && node->content == node_content)
            return node;
        else if(!sensitive && stricmp(node->content.c_str(), node_content.c_str()) == 0)
            {
            return node;
        }

        // Loop in children
        if(
            (
                r_node = node->getChildByContent
                                (
                                    node_content,
                                    recursive,
                                    sensitive
                                )
             ) != NULL)
            return r_node;
    }

    // Can not arrive here
    return NULL;
}

static void __saveStyle(struct HN_Node *node, std::ofstream &file)
{
    if(node->simple)
    {
        // Start node
        file << node->name << (!node->settings.empty() ? "[" + node->settings + "]" : "") << "{\n";

        // Print attributes
        if(node->attributes != NULL)
        {
            file << " ";
            std::vector<struct HN_Attribute *>::iterator it;
            struct HN_Attribute *attr;
            for
            (
                it = node->attributes->begin();
                it!= node->attributes->end();
                it++
            ){
                attr = *it;
                if(attr->name == "/" && attr->value == "")
                    continue;
                file << attr->name << ":" << attr->value << (!attr->settings.empty() ? "[" + attr->settings + "]" : "") << ";\n";
            }
        }

        // End node
        file << "}\n";
    }
    else{
        file << node->content << (!node->settings.empty() ? "[" + node->settings + "]" : "") << "{\n";

        // Print attributes
        if(node->attributes != NULL)
        {
            file << " ";
            std::vector<struct HN_Attribute *>::iterator it;
            struct HN_Attribute *attr;
            for
            (
                it = node->attributes->begin();
                it!= node->attributes->end();
                it++
            ){
                attr = *it;
                file << "\t" << attr->name << ":" << attr->value << (!attr->settings.empty() ? "[" + attr->settings + "]" : "") << ";\n";
            }
        }

        // Print children node
        std::deque<struct HN_Node *>::iterator n_it;
        struct HN_Node *childNode;
        for
        (
            n_it = node->children.begin();
            n_it!= node->children.end();
            n_it++
        ){
            childNode = *n_it;
            __saveStyle(childNode, file);
        }

        // End node
        file << "}\n";
    }
}

 // Managing data
void HN_Node::saveNode(FILE *file, int level, bool indent)
{
    // Loop writing data
    if(COMPARE(this->name.c_str(), "STYLE")
    && !this->content.empty())
        {
        //__saveStyle(this, file);
    }
    else{
        if(this->simple)
            {
            // Start node
            if(level > -1)
                {
                for(int i = 0; i < level; i++)
                    fprintf(file, "\t");
            }
            fprintf(file, "<%s", this->name.c_str());

            // Print attributes
            if(this->attributes != NULL)
            {
                fprintf(file, " ");
                std::vector<struct HN_Attribute *>::iterator it;
                struct HN_Attribute *attr;
                for
                (
                    it = this->attributes->begin();
                    it!= this->attributes->end();
                    it++
                ){
                    attr = *it;
                    if(attr->name == "/" && attr->value == "")
                        continue;
                    fprintf(file, "%s%s%s%s", attr->name.c_str(), "=\"", attr->value.c_str(), "\"");
                }
            }

            // End node
            fprintf(file, "/>");
            if(indent)fprintf(file, "\n");
        }
        else{
            // Start node
            if(level > -1)
                {
                for(int i = 0; i < level; i++)
                    fprintf(file, "\t");
            }
            fprintf(file, "<%s", this->name.c_str());

            // Print attributes
            if(this->attributes != NULL)
            {
                fprintf(file, " ");
                std::vector<struct HN_Attribute *>::iterator it;
                struct HN_Attribute *attr;
                for
                (
                    it = this->attributes->begin();
                    it!= this->attributes->end();
                    it++
                ){
                    attr = *it;
                    fprintf(file, "%s%s%s%s", attr->name.c_str(), "=\"", attr->value.c_str(), "\"");
                }
            }

            // End node begin and content
            fprintf(file, ">");
            if(!this->content.empty())
                {
                fprintf(file, "%s", this->content.c_str());
            }else if(indent){
                fprintf(file, "\n");
            }

            // Print children node
            std::deque<struct HN_Node *>::iterator n_it;
            struct HN_Node *childNode;
            for
            (
                n_it = this->children.begin();
                n_it!= this->children.end();
                n_it++
            ){
                childNode = *n_it;
                childNode->saveNode(file, level > -1 ? level + 1 : -1);
            }

            // End node
            if(level > -1 && this->content.empty())
                {
                for(int i = 0; i < level; i++)
                    fprintf(file, "\t");
            }
            fprintf(file, "%s%s%s", "</", this->name.c_str(), ">");
            if(indent)fprintf(file, "\n");
        }
    }
}

void HN_Node::saveNode(std::ofstream &file, int level)
{
    // Loop writing data
    if(COMPARE(this->name.c_str(), "STYLE")
    && !this->content.empty())
        {
        __saveStyle(this, file);
    }
    else{
        if(this->simple)
            {
            // Start node
            if(level > -1)
                {
                for(int i = 0; i < level; i++)
                    file << "\t";
            }
            file << "<" << this->name;

            // Print attributes
            if(this->attributes != NULL)
            {
                file << " ";
                std::vector<struct HN_Attribute *>::iterator it;
                struct HN_Attribute *attr;
                for
                (
                    it = this->attributes->begin();
                    it!= this->attributes->end();
                    it++
                ){
                    attr = *it;
                    if(attr->name == "/" && attr->value == "")
                        continue;
                    file << attr->name << "=\"" << attr->value << "\"";
                }
            }

            // End node
            file << "/>\n";
        }
        else{
            // Start node
            if(level > -1)
                {
                for(int i = 0; i < level; i++)
                    file << "\t";
            }
            file << "<" << this->name;

            // Print attributes
            if(this->attributes != NULL)
            {
                file << " ";
                std::vector<struct HN_Attribute *>::iterator it;
                struct HN_Attribute *attr;
                for
                (
                    it = this->attributes->begin();
                    it!= this->attributes->end();
                    it++
                ){
                    attr = *it;
                    file << attr->name << "=\"" << attr->value << "\"";
                }
            }

            // End node begin and content
            file << ">";
            if(!this->content.empty())
                {
                file << this->content;
            }else{
                file << std::endl;
            }

            // Print children node
            std::deque<struct HN_Node *>::iterator n_it;
            struct HN_Node *childNode;
            for
            (
                n_it = this->children.begin();
                n_it!= this->children.end();
                n_it++
            ){
                childNode = *n_it;
                childNode->saveNode(file, level > -1 ? level + 1 : -1);
            }

            // End node
            if(level > -1 && this->content.empty())
                {
                for(int i = 0; i < level; i++)
                    file << "\t";
            }
            file << "</" << this->name << ">" << std::endl;
        }
    }
}

void HN_Node::saveNode(std::string path, int level, bool indent)
{
    // Open output stream
    std::ofstream file(path.c_str());

    if(!file)
        return;

    // Write data
    this->saveNode(file, indent ? level : -1);
}

// Managing data
void HN_Node::saveStyle(std::ofstream &file)
{
    if(this->simple)
        {
        // Write data
        __saveStyle(this, file);
    }
    else{
        // Print children node
        std::deque<struct HN_Node *>::iterator n_it;
        struct HN_Node *childNode;
        for
        (
            n_it = this->children.begin();
            n_it!= this->children.end();
            n_it++
        ){
            childNode = *n_it;
            __saveStyle(childNode, file);
        }
    }
}

void HN_Node::saveStyle(std::string path)
{
    // Open output stream
    std::ofstream file(path.c_str());

    if(!file)
        return;

    this->saveStyle(file);
}


 // Adding children by file
void HN_Node::addChildren(FILE *f, bool in_front)
{

}

void HN_Node::addChildren(std::string path, bool in_front)
{
    // Open bile
    FILE *f = fopen(path.c_str(), "r");
    std::deque<struct HN_Node *> _children;

    // Do not work if file is empty
    if(f == NULL)
        return;

    // Start taking node
    bool isNodeEmpty = false;

    // While node exist
    while(!isNodeEmpty)
    {
        // Read node in file
        struct HN_Node *node = takeNode(f);
        if(node == NULL)
            {
            break;
        }

        isNodeEmpty = node->isEmpty();
        if(isNodeEmpty)
            {
            delete node;
            break;
        }

        // Place in deque top
        if(in_front)
        {
            // Prepare the next node taken
            _children.push_back(node);
        }

        // Place in deque bottom
        else{
            this->children.push_back(node);
        }
    }

    if(in_front)
    {
        // Add children node to actual node
        std::deque<struct HN_Node *>::iterator n_it;
        struct HN_Node *childNode;
        for
        (
            n_it = this->children.begin();
            n_it!= this->children.end();
            n_it++
        ){
            childNode = *n_it;
            _children.push_back(childNode);
        }

        // Clear children
        this->children.clear();
        this->children = _children;
    }
}


 // Destroyer
HN_Node::~HN_Node()
{
    //printf("\n%s :: ready to destroy node\n{\n", this->name.c_str());

    // Delete attributes
    if(this->attributes != NULL)
        {
        std::vector<struct HN_Attribute *>::iterator it;
        struct HN_Attribute *attr;
        for
        (
            it = this->attributes->begin();
            it!= this->attributes->end();
            it++
        ){
            attr = *it;
            //printf("\t%s=%s\n", attr->name.c_str(), attr->value.c_str());
            delete attr;
        }

        delete this->attributes;
    }
    //printf("\tcontent=\"%s\"", this->content.c_str());
    //printf("\n}\n");

    // Delete children
    std::deque<struct HN_Node *>::iterator n_it;
    struct HN_Node *childNode;
    for
    (
        n_it = this->children.begin();
        n_it!= this->children.end();
        n_it++
    ){
        childNode = *n_it;
        delete childNode;
    }

    // Delete instructions
    std::deque<struct HN_JSInstruction *>::iterator js_it;
    struct HN_JSInstruction *js_i;
    for
    (
        js_it = this->instructions.begin();
        js_it!= this->instructions.end();
        js_it++
    ){
        js_i = *js_it;
        delete js_i;
    }
}


 /*
  * Web file Management
  */

 // Builder
HN_WebFile::HN_WebFile()
{
    this->f = NULL;
}

 // Destroyer
HN_WebFile::~HN_WebFile()
{

}


 // Some quik functions
void initStringAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    std::string     attr_value
                )
{
    if(!node->isAttributeExist(attr_name))
        node->addAttribute(attr_name, attr_value);
    else{
        node->setAttribute(attr_name, attr_value);
    }
}

void initIntegerNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    int             attr_value
                )
{
    char __c[32];
    sprintf(__c, "%d", attr_value);

    if(!node->isAttributeExist(attr_name))
        node->addAttribute(attr_name, __c);
    else{
        node->setAttribute(attr_name, __c);
    }
}

void initLongNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    long            attr_value
                )
{
    char __c[32];
    sprintf(__c, "%ld", attr_value);

    if(!node->isAttributeExist(attr_name))
        node->addAttribute(attr_name, __c);
    else{
        node->setAttribute(attr_name, __c);
    }
}

void initDoubleNumberAttribute
                (
                    struct HN_Node  *node,
                    std::string     attr_name,
                    double          attr_value
                )
{
    char __c[260];
    sprintf(__c, "%f", attr_value);

    if(!node->isAttributeExist(attr_name))
        node->addAttribute(attr_name, __c);
    else{
        node->setAttribute(attr_name, __c);
    }
}



