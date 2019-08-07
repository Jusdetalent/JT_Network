/*
 * Henock @ Jusdetalent
 * 07-08-2019 17:03:00
 */


 #include "HN_JsonLoader.hpp"


struct HN_Node *parseJsonAsNode(const char *buffer, int size){

    // Parse initial link
    std::pair<std::string, std::string> array;
    std::string item;

    // Loop to parse link
    for(int i = 0; i <= size; i++){
        switch(buffer[i]){
            case '"':
            case '-':
            case ':':
            case '\0': {

                }
                break;
            default:
                break;
        }
    }

    return NULL;
}
