
/*
 * Attributes file reader
 * Henock @ Comedac :: 21/ 01/ 2017
 */

 #include "HN_FAttributes.hpp"
 #include "../HN_Node/HN_FNode.hpp"


 /*
  * File attributes parser management
  */

std::vector<struct HN_Attribute *> *takeAttributes(FILE *f, FILE *outf, FILE *cfgfile,
                                                   bool &simple)
{
    std::string attr_name, attr_value,
                node;
    int i;
    std::vector<struct HN_Attribute *>
        *attributes = new std::vector<struct HN_Attribute *>;

    //Taking attribute
    //printf("************* Taking attributes *************\n");

    while((attr_name = getAttributeName(f, simple)) != "")
    {
        //generation des attributs
        if(attr_name.length() > 0 && !simple)
        {
             //se positioner devant le caractaire "/
             do{
                i = fgetc(f);
             }while(i != '"' && i != EOF && i != '\'');

             //lecture de la valeur
             int end_char_t = i;
             attr_value = getAttributeValue(f, simple, end_char_t);

        }else{
            attr_value = "";
        }

        // By default :: add attribute
        struct HN_Attribute *attr = new struct HN_Attribute;
            attr->name  = attr_name;
            attr->value = attr_value;
        attributes->push_back(attr);

        // Break if simple node
        if(simple)
            {
            break;
        }
    }

    //End taking attribute
    //printf("********** End taking attribute****************\n\n");

    //Value to return
    return attributes;
}

std::string getAttributeName(FILE *f, bool &simple)
{
    int i = 0, li = 0;
    std::string attr_name;

    //printf("\n");
    while(i != EOF)
    {
       i = fgetc(f);

       //Si fin de node simple
       if(i == '>' || i == 62)
       {
            // Analyse if node is simple
            if(li == '/' || li == 47 || li == '-')
                {
                // Node simple
                simple = true;
            }

            // Return attribute name
            return attr_name;
       }

       // Si debut value begin
       if(i == '=')
            {
            return attr_name;
        }

       // Add character to name
       if(i != ' ' && i != '\n' && i != '\t')
       {
            attr_name+= i;
       }

       li = i;
    }

    //printf("{%s}\n", attr_name.c_str());
    return attr_name;
}



std::string getAttributeValue(FILE *f, bool &simple, int &end_char_t)
{
    int i = 0, li = 0;
    std::string attr_value;

    //printf("\n");
    while(i != EOF)
    {
       i = fgetc(f);

       //Si fin de balise simple
       if(i == '>' && li != '\\')
       {
            // Analyse if node is simple
            if(li == '/' || li == 47 || li == '-')
                {
                // Node simple
                simple = true;
            }

            // Return attribute name
            return attr_value;
       }

       // Si debut value end
       if(i == end_char_t)
            {
            return attr_value;
        }

       // Add character to name
       if(i != '\n' && i != '\t')
       {
            if(i == '\\' && li != '\\')
                {
                // Nothing to do here :: action will be handle on next time
            }
            else if(
                    (i == '\\' || i == '>')
                &&  li == '\\'
                ){
                attr_value+= i;
                i = 0; li = 0;
                continue;
            }
            else if(i != '\\' && li == '\\')
                {
                attr_value+= li;
                attr_value+= i;
            }
            else{
                attr_value+= i;
            }
       }

       li = i;
    }

    //printf("{%s}\n", attr_value.c_str());
    return attr_value;
}

std::string getContent(FILE *f, bool &childNode_exist)
{
    int i = 0;
    std::string content;

    //printf("\n");
    while(i != EOF)
    {
       i = fgetc(f);

       //Si debut de fin de balise
       if(i == '<' || i == 60)
       {
            // Analyze if child node exist
            i = fgetc(f);
            if(i == '/' || i == 47 || i == '-')
                {
                //printf("No child attach to this node\n");
                while(i != '>' && i != EOF && i != 62)
                    {
                    i = fgetc(f);
                }
            }else{
                fseek(f, -2, SEEK_CUR);
                //printf("Child node exist %c\n", i);
                childNode_exist = true;
            }

            // Return attribute name
            return content;
       }

       // Add character to name
       if(i != '\n' && i != '\t')
       {
            content+= i;
       }
    }

    //printf("{%s}\n", content.c_str());
    return content;
}


 /*
  * String attribute parser management
  */

  std::vector<struct HN_Attribute *> *takeAttributes
                                        (
                                            const char  *buffer_t,
                                            FILE        *outf,
                                            FILE        *cfgfile,
                                            bool        &simple,
                                            int         &cur_pos_t
                                        )
{
    std::string attr_name, attr_value,
                node;
    int i = 1;
    std::vector<struct HN_Attribute *>
        *attributes = new std::vector<struct HN_Attribute *>;

    //Taking attribute
    //printf("************* Taking attributes *************\n");

    while((attr_name = getAttributeName(buffer_t, simple, cur_pos_t)) != "")
    {
        //generation des attributs
        if(attr_name.length() > 0 && !simple)
        {
             // Se positioner devant le caractaire "/
             do{
                i = buffer_t[cur_pos_t];
                cur_pos_t++;
             }while(i != '"' && i != '\0' && i != '\'');

             //lecture de la valeur
             int end_char = i;
             attr_value = getAttributeValue(
                                            buffer_t, simple, cur_pos_t,
                                            end_char
                                        );
        }else{
            attr_value = "";
        }

        // By default :: add attribute
        struct HN_Attribute *attr = new struct HN_Attribute;
            attr->name  = attr_name;
            attr->value = attr_value;
        attributes->push_back(attr);

        // Break if simple node
        if(simple)
            {
            break;
        }
    }

    //End taking attribute
    //printf("********** End taking attribute****************\n\n");

    //Value to return
    return attributes;
}

std::string getAttributeName(const char *buffer_t, bool &simple, int &cur_pos_t)
{
    int i = 1, li = 0;
    std::string attr_name;

    //printf("\n");
    while(i != '\0')
    {
       i = buffer_t[cur_pos_t];
       cur_pos_t++;

       //Si fin de node simple
       if(i == '>' || i == 62)
       {
            // Analyse if node is simple
            if(li == '/' || li == 47 || li == '-')
                {
                // Node simple
                simple = true;
            }

            // Return attribute name
            return attr_name;
       }

       // Si debut value begin
       if(i == '=')
            {
            return attr_name;
        }

       // Add character to name
       if(i != ' ' && i != '\n' && i != '\t')
       {
            attr_name+= i;
       }

       li = i;
    }

    //printf("{%s}\n", attr_name.c_str());
    return attr_name;
}

std::string getAttributeValue(const char *buffer_t, bool &simple,
                              int &cur_pos_t, int &end_char_t)
{
    int i = 1, li = 0;
    std::string attr_value;

    //printf("\n");
    while(i != '\0')
    {
       i = buffer_t[cur_pos_t];
       cur_pos_t++;

       //Si fin de balise simple
       if(i == '>' && li != '\\')
       {
            // Analyse if node is simple
            if(li == '/' || li == 47 || li == '-')
                {
                // Node simple
                simple = true;
            }

            // Return attribute name
            return attr_value;
       }

       // Si debut value end
       if(i == end_char_t)
            {
            return attr_value;
        }

       // Add character to name
       if(i != '\n' && i != '\t')
       {
            if(i == '\\' && li != '\\')
                {
                // Nothing to do here :: action will be handle on next time
            }
            else if(
                    (i == '\\' || i == '>')
                &&  li == '\\'
                ){
                attr_value+= i;
                i = 0; li = 0;
                continue;
            }
            else if(i != '\\' && li == '\\')
                {
                attr_value+= li;
                attr_value+= i;
            }
            else{
                attr_value+= i;
            }
       }

       li = i;
    }

    //printf("{%s}\n", attr_value.c_str());
    return attr_value;
}

std::string getContent(const char *buffer_t, bool &childNode_exist,
                       int &cur_pos_t)
{
    char i = 1;
    std::string content;

    while(i != '\0')
    {
       i = buffer_t[cur_pos_t];
       cur_pos_t++;

       //Si debut de fin de balise
       if(i == '<' || i == 60)
       {
            // Analyze if child node exist
            i = buffer_t[cur_pos_t];
            cur_pos_t++;

            if(i == '/' || i == 47 || i == '-')
                {
                //printf("No child attach to this node\n");
                while(i != '>' && i != '\0' && i != 62)
                    {
                    i = buffer_t[cur_pos_t];
                    cur_pos_t++;
                }
            }else{
                cur_pos_t-= 2;
                //printf("Child node exist %c\n", i);
                childNode_exist = true;
            }

            // Return attribute name
            return content;
       }

       // Add character to name
       if(i != '\n' && i != '\t')
       {
            content+= i;
       }
    }

    //printf("{%s}\n", content.c_str());
    return content;
}



