
/*
 * Style source file
 * Henock @ Comedac :: 18/ 02/ 2017
 */


 #include "HN_CSS.hpp"
 #include <iostream>


bool isNextCharacter(FILE *f, int c)
{
    long cur_pos = ftell(f);
    int i;
    for(i = fgetc(f); i != EOF; i = fgetc(f))
        {
        if(i == c)
            {
            fseek(f, cur_pos, SEEK_SET);
            return true;
        }
        else if(i != '\n' && i != '\t' && i != ' ')
            {
            fseek(f, cur_pos, SEEK_SET);
            return false;
        }
    }

    fseek(f, cur_pos, SEEK_SET);
    return false;
}

int getStyleValue(FILE *f, std::string &value, std::string &settings)
{
    int li, i;
    bool UP =false;

    while((i = fgetc(f)) != EOF)
        {

        // Catch settings
        if(i == '[')
            {
            UP = true;
            continue;
        }
        else if(i == ']')
            {
            UP = false;
            continue;
        }

        // Take character
        if(i != ';' &&  i != '}' && i != '\n'
        && i != '\t')
            {
            !UP ? value+= i : settings+= i;
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
            while((i = fgetc(f)) != EOF)
                {
                // Take character
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

int getStyleName(FILE *f, std::string &name)
{
    int li, i;
    bool yes = false;
    while((i = fgetc(f)) != EOF)
        {
        // Take character
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
            if(!isNextCharacter(f, '{') && !yes)
                return i;
            yes = false;
        }

        // In case of quote
        else if(i == ',')
            {
            if(isNextCharacter(f, '\n'))
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
            while((i = fgetc(f)) != EOF)
                {
                // Take character
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

std::vector<struct HN_Attribute *> *takeStyleAttributes(struct HN_Node *parent_node, FILE *f)
{
    // Allocate data
    std::vector<struct HN_Attribute *>
        *attributes = new std::vector<struct HN_Attribute *>;
    bool UP = false;

    // Read data
    if(f != NULL)
    {
        int i = 0;
        std::string attr_name, attr_value, attr_settings;
        while((i = fgetc(f)) != EOF)
        {
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
                            i = getStyleValue(f, attr_value, attr_settings);

                            // By default :: add attribute
                            struct HN_Attribute *attr = new struct HN_Attribute;
                                attr->name      = attr_name;
                                attr->value     = attr_value;
                                attr->settings  = attr_settings;
                            attributes->push_back(attr);

                            // Reset data
                            attr_name.clear();
                            attr_value.clear();
                            attr_settings.clear();
                            //std::cout
                              //  << attr->name << " -- " << attr->value << " -- ["
                                //<< attr->settings << "]" << std::endl;
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
                case '/': i = JumpComment(f);
                    break;

                    // If another node
                case '{':
                    {
                        struct HN_Node *__node = new HN_Node;
                        if(__node != NULL)
                            {
                            __node->name        = attr_name;
                            __node->simple      = true;
                            __node->settings    = attr_settings;
                            __node->attributes  = takeStyleAttributes(__node, f);
                            parent_node->simple = false;
                            parent_node->children.push_back(__node);
                        }
                        attr_name.clear();
                        attr_settings.clear();
                    }
                    break;
                case '[':
                        UP = true;
                    break;
                case ']':
                        UP = false;
                    break;

                    // In default
                default :
                       !UP ? attr_name+= i : attr_settings+= i;
                    break;
            }
        }
    }

    // Return attributes
    return attributes;
}

struct HN_StyleNode findSelector(FILE *f)
{
    // node to return
    struct HN_Node *b_node = new struct HN_Node;
    struct HN_StyleNode r_node = {-1, b_node};
        r_node.node->name = "style";
    bool UP = false;

    if(f != NULL)
        {
        int i = 0;
        std::string selector;
        while((i = fgetc(f)) != EOF)
            {
            switch(i)
            {
                case 123: // '{'
                    {
                        !UP ? r_node.node->content = selector : r_node.node->settings = selector;
                        r_node.type = 0;
                        return r_node;
                    }
                    break;
                case 47: i = JumpComment(f); // '/'
                    break;
                case '\n':  case '\t':
                    break;
                case 64: // '@'
                    {
                        // Generate data
                        selector+= i;
                        i = getStyleName(f, selector);
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
                        while(((i = fgetc(f)) != '>') && i != 62)
                            {
                            if(i == EOF){break;}
                        }

                        // Return node
                        return r_node;
                    }
                break;
                case '[':
                    {
                        if(!UP)r_node.node->content = selector;
                        UP = true;
                        selector.clear();
                    }
                    break;
                case ']':
                        if(!UP)selector+= i;
                    break;
                default : selector+= i;
                    break;
            }
        }
    }

    // Return node
    return r_node;
}

struct HN_Node *takeStyle(FILE *f)
{
    // Build data pointer
    struct HN_StyleNode s_node = {0, NULL};
    struct HN_Node *b_node = new struct HN_Node;
            b_node->name = "style";
    bool isData = false;

    // Loop reading data
    while(s_node.type != -1)
    {
        // Read style
        s_node = findSelector(f);

        // Get data
        if(s_node.type == 0)
            {
            // Read attributes
            s_node.node->attributes = takeStyleAttributes(s_node.node, f);
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

bool harvestStyle(struct HN_Node *b_node, FILE *f)
{
    // Build data pointer
    struct HN_StyleNode s_node = {0, NULL};
    bool isData = false;

    // Loop reading data
    while(s_node.type != -1)
    {
        // Get selector
        s_node = findSelector(f);

        // Get data
        if(s_node.type == 0)
            {
            // Read attributes
            s_node.node->attributes = takeStyleAttributes(s_node.node, f);
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

