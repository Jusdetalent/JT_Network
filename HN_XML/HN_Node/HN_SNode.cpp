
/*
 * Node parser from string
 * By Hénock @ Comedac :: 31/ 07/ 2017
 */

 #include "HN_SNode.hpp"
 #include <iostream>


static void update_curPos(int *__cur_pos, int &cur_pos)
{
    if(__cur_pos != NULL)*__cur_pos = cur_pos;
}

struct HN_Node *findNode(const char *buffer_t, int *__cur_pos_t)
{
    struct HN_Node *node = new struct HN_Node;
    char i, li = 0;
    int cur_pos_t = __cur_pos_t != NULL ? *__cur_pos_t : 0;

    do{
        i = buffer_t[cur_pos_t];
        cur_pos_t++;

        switch(i)
        {
            case 60: // '<'
                {
                    li = i;
                while(i != '\0')
                {
                    // Get next character
                    i = buffer_t[cur_pos_t];
                    cur_pos_t++;

                    // If end of node name
                    if(i == ' ')
                    {
                        node->attr_exist = true;
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return node;
                    }

                    if(i == '!' && li == '<')
                    {
                        // Jump comment
                        bool data_t = false;
                        int __li = 0, __lj = 0;

                        while(!data_t)
                        {
                            do{
                                i = buffer_t[cur_pos_t];
                                cur_pos_t++;

                                if(i == '\0')break;
                                __lj = __li;
                                __li = i;
                            }while(i != '>');

                            if(__lj == '-' && __li == '-' && i == '>')
                                data_t = true;
                            if(i == '\0')
                                data_t = true;
                        }
                        break;
                    }

                    if(i == '>' || i == 62)
                        {
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return node;
                    }
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
                            i = buffer_t[cur_pos_t];
                            cur_pos_t++;

                            if(i == '\0')
                                {
                                delete node;
                                update_curPos(__cur_pos_t, cur_pos_t);
                                return NULL;
                            }
                        }

                        update_curPos(__cur_pos_t, cur_pos_t);
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
                if(i != ' ' && i != '\t' && i != '\n'
                && i != '\0')
                    {
                    node->name = "text";
                    node->content+= i;
                    while(i != '\0')
                        {
                        i = buffer_t[cur_pos_t];
                        cur_pos_t++;

                        // In case of element start
                        if(i == '<' || i == 60)
                        {
                            cur_pos_t--;
                            node->loading = false;
                            update_curPos(__cur_pos_t, cur_pos_t);
                            return node;
                        }

                        // If end of file
                        if(i == '\0')
                            {
                            node->loading = false;
                            update_curPos(__cur_pos_t, cur_pos_t);
                            return node;
                        }

                        if(i == '\n'){i = ' ';}
                        if(i != '\t'){node->content+= i;}
                    }
                }

                // In case of end of file
                if(i == '\0')
                    {
                    delete node;
                    update_curPos(__cur_pos_t, cur_pos_t);
                    return NULL;
                }
            }
            break;
        }

        li = i;
    }while(i != '\0');

    delete node;
    update_curPos(__cur_pos_t, cur_pos_t);
    return NULL;
}

struct HN_Node *takeNode(const char *buffer_t, int *__cur_pos_t)
{
    int cur_pos_t = __cur_pos_t != NULL ? *__cur_pos_t : 0;
    struct HN_Node *node = findNode(buffer_t, &cur_pos_t);
    if(node == NULL)
        {
        update_curPos(__cur_pos_t, cur_pos_t);
        return NULL;
    }

    if(!node->simple)
        {
        //std::cout << cur_pos_t << ". " << node->name << " -- us been find us a paired node" << std::endl;

        if(node->attr_exist)
        node->attributes = takeAttributes
                                (
                                    buffer_t,
                                    NULL,
                                    NULL,
                                    node->simple,
                                    cur_pos_t
                                );

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
                //printf("%s: End of simple node", node->name.c_str());
                // Harvest style
                harvestStyle(node, buffer_t, cur_pos_t);
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
                node->content = getContent(buffer_t, node->childNode_exist, cur_pos_t);
                //std::cout << "Node content is: " << node->content << std::endl;
                int l = 0;

                if(node->childNode_exist)
                {
                    while(!isEndOfNode(buffer_t, node->name, cur_pos_t))
                    {
                        HN_Node *child = takeNode(buffer_t, &cur_pos_t);
                            if(child == NULL)
                                {
                                node->unlimited = true;
                                break;
                            }
                        node->children.push_back(child);
                        l++;
                        //printf("%d. %d %s\n", cur_pos_t, l, child->name.c_str());
                        //system("PAUSE");
                    }
                    //printf("\n\nChildren us been take %d\n\n", l);
                    node->loading = false;
                }
            }
        }
    }

    update_curPos(__cur_pos_t, cur_pos_t);
    return node;
}

bool isEndOfNode(const char *buffer_t, const std::string &name,
                 int &cur_pos_t)
{
    // Get data
    int actual_pos = cur_pos_t;
    int i;
    std::string node_name;

    // find end of node
    do{
        i = buffer_t[cur_pos_t];
        cur_pos_t++;

        switch(i)
        {
            case 60: // '<'
                {
                    i = buffer_t[cur_pos_t];
                    cur_pos_t++;

                    if(i != '/' && i != 47)
                        {
                        cur_pos_t = actual_pos;
                        return false;
                    }

                    while(i != '\0')
                        {
                        // Get character
                        i = buffer_t[cur_pos_t];
                        cur_pos_t++;

                        // Analyze if same node
                        if(i == '>' || i == 62)
                            {
                            if(node_name == name)
                            {
                                return true;
                            }
                            else{
                                cur_pos_t = actual_pos;
                                return false;
                            }
                        }

                        // Add to node name
                        if(i != ' ' && i != '\n' && i != '\t')
                        node_name+= i;
                    }
                }
                break;
            case ' ':       case '\n':      case '\t':
                break;
            default:
                    cur_pos_t = actual_pos;
                    return false;
                break;
        }

    }while(i != '\0');


    // Return value
    cur_pos_t = actual_pos;
    return false;
}


