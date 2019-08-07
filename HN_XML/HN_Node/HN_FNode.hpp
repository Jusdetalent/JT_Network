#ifndef HN_FNODEREADER_HPP_INCLUDED
#define HN_FNODEREADER_HPP_INCLUDED

   #include "HN_Node.hpp"
   #include "../HN_Attributes/HN_FAttributes.hpp"

   // Standard prototypes reader
   struct HN_Node *findNode(FILE *f);
   struct HN_Node *takeNode(FILE *f);
   bool isEndOfNode(FILE *f, const std::string &name);

   #ifdef linux
      #define stricmp strcasecmp
   #endif // linux

#endif // HN_FNODEREADER_HPP_INCLUDED
