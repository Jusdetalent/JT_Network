/*
 * Node reader by file descriptor
 * Henock @ Comedac :: 21/ 01/ 2017
 */

 #include "HN_FNode.hpp"
 #include "../../HN_CSS/HN_CSS.hpp"

static bool isDTDNode(FILE *f)
{
    long cur_pos = ftell(f);
    int i = fgetc(f);
    if(i == '-' && fgetc(f) == '-')
        {
        fseek(f, cur_pos, SEEK_SET);
        return false;
    }

    fseek(f, cur_pos, SEEK_SET);
    return true;
}

struct HN_Node *findNode(FILE *f)
{
    struct HN_Node *node = new struct HN_Node;
    int i, li = 0;

    do{
        i = fgetc(f);
        switch(i)
        {
            case 60: // '<'
                {
                    li = i;
                while(i != EOF)
                {
                    // Get next character
                    i = fgetc(f);

                    // If end of node name
                    if(i == ' ')
                    {
                        node->attr_exist = true;
                        return node;
                    }

                    if(i == '!' && li == '<')
                    {
                        // Jump comment
                        bool data_t = false;
                        int __li = 0, __lj = 0;

                        // If Comment
                        if(!isDTDNode(f))
                        {
                            while(!data_t)
                            {
                                while((i = fgetc(f)) != '>')
                                {
                                    if(i == EOF)break;
                                    __lj = __li;
                                    __li = i;
                                }

                                if(__lj == '-' && __li == '-' && i == '>')
                                    data_t = true;
                                if(i == EOF)
                                    data_t = true;
                            }
                        }

                        // If DTD
                        else{
                            while(!data_t)
                            {
                                while((i = fgetc(f)) != '>')
                                {
                                    if(i == EOF)break;
                                    __lj = __li;
                                    __li = i;
                                    //printf("%c", i);
                                    data_t = true;
                                }
                            }
                        }

                        break;
                    }

                    if(i == '>' || i == 62){return node;}
                    if(i == '/' || i == 47)
                    {
                        node->simple = true;
                        node->loading = false;
                        while
                        (i != '>' && i != 62)
                            {
                            // Build if not yet name
                            if(li == '<' || li == 60)
                                {
                                node->name+= i;
                            }

                            // Get next character
                            i = fgetc(f);
                            if(i == EOF)
                                {
                                delete node;
                                return NULL;
                            }
                        }
                        return node;
                    }

                    // Add to node name
                    node->name+= i;
                    li = i;
                }
            }
            break;
        default:
            {
                if(i != ' ' && i != '\t' && i != '\n' && i != '\r' && i != EOF)
                    {
                    node->name = "text";
                    node->content+= i;
                    while(i != EOF)
                        {
                        i = fgetc(f);

                        // In case of element start
                        if(i == '<' || i == 60)
                        {
                            fseek(f, -1, SEEK_CUR);
                            node->loading = false;
                            return node;
                        }

                        // If end of file
                        if(i == EOF)
                            {
                            node->loading = false;
                            return node;
                        }

                        if(i == '\n'){i = ' ';}
                        if(i != '\t' && i != '\r'){node->content+= i;}
                    }
                }

                // In case of end of file
                if(i == EOF)
                    {
                    delete node;
                    return NULL;
                }
            }
            break;
        }

        li = i;
    }while(i != EOF);

    delete node;
    return NULL;
}

struct HN_Node *takeNode(FILE *f)
{
    struct HN_Node *node = findNode(f);
    if(node == NULL){return NULL;}

    if(!node->simple)
        {
        //std::cout << node->name << " -- us been find us a paired node" << std::endl;

        if(node->attr_exist)
        node->attributes = takeAttributes(f, NULL, NULL, node->simple);

        if(node->simple)
            {
            //std::cout << node->name << ": " << "End of simple node" << std::endl;
        }
        else if(node->loading)
            {
            // In case of special node
            if
                (
                stricmp(node->name.c_str(), "STYLE") == 0
            ||  stricmp(node->name.c_str(), "RIGHT") == 0
            ){
                printf("%s: End of simple node", node->name.c_str());
                // Harvest style
                harvestStyle(node, f);
                node->loading = false;
            }

            // If DTD or comment
            else if(node->name[0] == '!')
                {
                // Nothing to do actually
            }

            // In case of xml node
            else{
                //read content
                node->content = getContent(f, node->childNode_exist);
                //std::cout << "Node content is: " << node->content << std::endl;
                int l = 0;

                if(node->childNode_exist)
                {
                    while(!isEndOfNode(f, node->name))
                    {
                        HN_Node *child = takeNode(f);
                            if(child == NULL)
                                {
                                node->unlimited = true;
                                break;
                            }
                        node->children.push_back(child);
                        l++;
                    }
                    //printf("\n\nChildren us been take %d\n\n", l);
                    node->loading = false;
                }
            }
        }
    }

    return node;
}

bool isEndOfNode(FILE *f, const std::string &name)
{
    // Get data
    long actual_pos = ftell(f);
    int i;
    std::string node_name;

    // find end of node
    do{
        i = fgetc(f);
        switch(i)
        {
            case 60: // '<'
                {
                    i = fgetc(f);
                    if(i != '/' && i != 47)
                        {
                        fseek(f, actual_pos, SEEK_SET);
                        return false;
                    }

                    while(i != EOF)
                        {
                        // Get character
                        i = fgetc(f);

                        // Analyze if same node
                        if(i == '>' || i == 62)
                            {
                            if(node_name == name)
                            {
                                return true;
                            }
                            else{
                                fseek(f, actual_pos, SEEK_SET);
                                return false;
                            }
                        }

                        // Add to node name
                        if(i != ' ' && i != '\n' && i != '\t')
                        node_name+= i;

                        // If end of file
                        if(i == EOF)return true;
                    }
                }
                break;
            case ' ':       case '\n':      case '\t':
                break;
            case EOF:
                return true;
            default:
                    fseek(f, actual_pos, SEEK_SET);
                    return false;
                break;
        }

    }while(i != EOF);


    // Return value
    fseek(f, actual_pos, SEEK_SET);
    return false;
}


