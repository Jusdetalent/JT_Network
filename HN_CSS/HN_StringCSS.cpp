
/*
 * Style source file
 * Henock @ Comedac :: 18/ 02/ 2017
 */


 #include "HN_StringCSS.hpp"
 #include <iostream>


bool isNextCharacter(const char *buffer, int c, int &cur_pos_t)
{
    int cur_pos = cur_pos_t;
    int i;
    for(i = buffer[cur_pos_t]; i != '\0'; i = buffer[cur_pos_t])
        {
        cur_pos_t++;
        if(i == c)
            {
            cur_pos_t = cur_pos;
            return true;
        }
        else if(i != '\n' && i != '\t')
            {
            cur_pos_t = cur_pos;
            return false;
        }
    }

    cur_pos_t = cur_pos;
    return false;
}

int getStyleValue(const char *buffer, std::string &value, int &cur_pos_t)
{
    int li, i;
    while((i = buffer[cur_pos_t]) != '\0')
        {
        // Take character
        cur_pos_t++;
        if(i != ';' &&  i != '}' && i != '\n'
        && i != '\t')
            {
            value+= i;
        }

        // Catch the end
        if(i == ';' || i == '}')
            {
            break;
        }

        // In case of special character
        else if(i == '('    || i == '['
            ||  i == '"'    || i == '\'')
            {
            // Fix end character
            if(i == '('){li = ')';}
            else if(i == '['){li = ']';}
            else{li = i;}

            // Loop
            while((i = buffer[cur_pos_t]) != '\0')
                {
                // Take character
                cur_pos_t++;
                if(i != ';' &&  i != '}' && i != '\n'
                && i != '\t')
                    {
                    value+= i;
                }

                // Avoid infinite loop
                if(li == i)
                    {
                    break;
                }
            }
        }
    }

    return i;
}

int getStyleName(const char *buffer, std::string &name, int &cur_pos_t)
{
    int li, i;
    bool yes = false;
    while((i = buffer[cur_pos_t]) != '\0')
        {
        // Take character
        cur_pos_t++;
        if(i != '\n' && i != '\t' && i != '{')
            {
            name+= i;
        }

        // Catch the end
        if(i == '{' || i == ';')
            {
            return i;
        }

        // In case of new line
        if(i == '\n')
            {
            if(!isNextCharacter(buffer, '{', cur_pos_t) && !yes)
                return i;
            yes = false;
        }

        // In case of quote
        else if(i == ',')
            {
            if(isNextCharacter(buffer, '\n', cur_pos_t))
                yes = true;
        }

        // In case of special character
        else if(i == '('    || i == '['
            ||  i == '"'    || i == '\'')
            {
            // Fix end character
            if(i == '('){li = ')';}
            else if(i == '['){li = ']';}
            else{li = i;}

            // Loop
            while((i = buffer[cur_pos_t]) != '\0')
                {
                // Take character
                cur_pos_t++;
                if(i != '\n' && i != '\t' && i != '{')
                    {
                    name+= i;
                }

                // Avoid infinite loop
                if(li == i)
                    {
                    break;
                }
            }
        }
    }

    return i;
}

std::vector<struct HN_Attribute *> *takeStyleAttributes(struct HN_Node *parent_node,
                                                        const char *buffer, int &cur_pos_t)
{
    // Allocate data
    std::vector<struct HN_Attribute *>
        *attributes = new std::vector<struct HN_Attribute *>;

    // Read data
    int i = 0;
    std::string attr_name, attr_value;
    while((i = buffer[cur_pos_t]) != '\0')
    {
        cur_pos_t++;
        switch(i)
        {
                /*
                    In case of value begin
                    or node style end
                    */
            case ':':
            case '}':
                {
                    // Treat data
                    if(strlen(attr_name.c_str()) > 0)
                        {
                        // Get value and work on data
                        i = getStyleValue(buffer, attr_value, cur_pos_t);

                        // By default :: add attribute
                        struct HN_Attribute *attr = new struct HN_Attribute;
                            attr->name  = attr_name;
                            attr->value = attr_value;
                        attributes->push_back(attr);

                        // Reset data
                        attr_name.clear();
                        attr_value.clear();
                    }

                    // If end of style :: return
                    if(i == '}'){return attributes;}
                }
                break;

                /*
                    In case of invalid
                    character
                    */
            case '\n':  case '\t':  case '\b':  case ' ':
                break;

                // In case of comment
            case '/': i = JumpComment(buffer, cur_pos_t);
                break;

                // If another node
            case '{':
                {
                    struct HN_Node *__node = new HN_Node;
                    if(__node != NULL)
                        {
                        __node->name        = attr_name;
                        __node->simple      = true;
                        __node->attributes  = takeStyleAttributes(__node, buffer, cur_pos_t);
                        parent_node->simple = false;
                        parent_node->children.push_back(__node);
                    }
                    attr_name.clear();
                }
                break;

                // In default
            default :
                    attr_name+= i;
                break;
        }
    }

    // Return attributes
    return attributes;
}

struct HN_StyleNode findSelector(const char *buffer, int &cur_pos_t)
{
    // node to return
    struct HN_Node *b_node = new struct HN_Node;
    struct HN_StyleNode r_node = {-1, b_node};
        r_node.node->name = "style";

    int i = 0;
    std::string selector;
    while((i = buffer[cur_pos_t]) != '\0')
        {
        cur_pos_t++;
        switch(i)
        {
            case 123: // '{'
                {
                    r_node.node->content = selector;
                    r_node.type = 0;
                    return r_node;
                }
                break;
            case 47: i = JumpComment(buffer, cur_pos_t); // '/'
                break;
            case '\n':  case '\t':
                break;
            case 64: // '@'
                {
                    // Generate data
                    selector+= i;
                    i = getStyleName(buffer, selector, cur_pos_t);
                    r_node.node->content = selector;

                    // Return style node
                    if(i == '\n')
                        {
                        r_node.type = 1;
                        return r_node;
                    }
                    else{
                        r_node.type = 0;
                        return r_node;
                    }
                }
                break;
            case 60: // '<'
                {
                    // Break reading
                    delete r_node.node;
                    r_node.node = NULL;
                    r_node.type = -1;
                    while(((i = buffer[cur_pos_t]) != '>') && i != 62)
                        {
                        cur_pos_t++;
                        if(i == '\0'){break;}
                    }

                    // Return node
                    return r_node;
                }
            break;
            default : selector+= i;
                break;
        }
    }

    // Return node
    return r_node;
}

struct HN_Node *takeStyle(const char *buffer)
{
    // Build data pointer
    struct HN_StyleNode s_node = {0, NULL};
    struct HN_Node *b_node = new struct HN_Node;
            b_node->name = "style";
    bool isData = false;
    int cur_pos_t = 0;

    // Loop reading data
    while(s_node.type != -1)
    {
        // Read style
        s_node = findSelector(buffer, cur_pos_t);

        // Get data
        if(s_node.type == 0)
            {
            // Read attributes
            s_node.node->attributes = takeStyleAttributes(s_node.node,
                                                          buffer, cur_pos_t);
        }

        // In case of empty data
        else if(s_node.type == -1)
            {
            break;
        }

        // Delete node
        b_node->children.push_back(s_node.node);
        isData = true;
    }

    // Verify if data loaded
    if(!isData)
        {
        delete b_node;
        b_node = NULL;
    }

    // Return data
    return b_node;
}

bool harvestStyle(struct HN_Node *b_node, const char *buffer,
                  int &cur_pos_t)
{
    // Build data pointer
    struct HN_StyleNode s_node = {0, NULL};
    bool isData = false;

    // Loop reading data
    while(s_node.type != -1)
    {
        // Get selector
        s_node = findSelector(buffer, cur_pos_t);

        // Get data
        if(s_node.type == 0)
            {
            // Read attributes
            s_node.node->attributes = takeStyleAttributes(s_node.node,
                                                          buffer, cur_pos_t);
        }

        // In case of empty data
        else if(s_node.type == -1)
            {
            break;
        }

        // Delete node
        b_node->children.push_back(s_node.node);
        isData = true;
    }

    // Verify if data loaded
    if(!isData){return false;}

    // Return data
    return true;
}


