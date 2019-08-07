# 
# Make file for JT Network
# By Henock :: 06/ 11/ 2018
#

CC = g++
AR = ar
BIN = bin/libHN_Network.a 
INC = -I/usr/local/include/SDL -I/usr/local/include 
LIBS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib 
CXXFLAGS = $(INC) -D_REENTRANT -D_THREAD_SAFE $(LIBS) 

# Base folder
CSS_OBJ_DIR = objs/HN_CSS/
CSS_DIR = HN_CSS/
JS_OBJ_DIR = objs/HN_Javascript/
JS_DIR = HN_Javascript/
ATTR_OBJ_DIR = objs/HN_XML/HN_Attributes/
ATTR_DIR = HN_XML/HN_Attributes/
DOC_OBJ_DIR = objs/HN_XML/HN_Document/
DOC_DIR = HN_XML/HN_Document/
DTD_OBJ_DIR = objs/HN_XML/HN_DTD/
DTD_DIR = HN_XML/HN_DTD/
NODE_OBJ_DIR = objs/HN_XML/HN_Node/
NODE_DIR = HN_XML/HN_Node/

# Main objects
OBJ = $(CSS_OBJ_DIR)HN_CSS.o $(CSS_OBJ_DIR)HN_StringCSS.o $(JS_OBJ_DIR)HN_Javascript.o 
OBJ+= $(JS_OBJ_DIR)HN_StringJavascript.o $(ATTR_OBJ_DIR)HN_Attributes.o $(ATTR_OBJ_DIR)HN_ExternAttributes.o 
OBJ+= $(ATTR_OBJ_DIR)HN_FAttributes.o $(DOC_OBJ_DIR)HN_Document.o $(DTD_OBJ_DIR)HN_DTD.o $(NODE_OBJ_DIR)HN_FNode.o 
OBJ+= $(NODE_OBJ_DIR)HN_Node.o $(NODE_OBJ_DIR)HN_SNode.o objs/HN_XML/HN_Xml.o  

# Assemblate link objects
LINKOBJ = $(OBJ) 

# Make all
$(BIN): $(OBJ)
	$(AR) r $(BIN) $(LINKOBJ) 

# Build CSS objects
$(CSS_OBJ_DIR)HN_CSS.o: $(CSS_DIR)HN_CSS.cpp
	$(CC) -c $(CSS_DIR)HN_CSS.cpp -o $(CSS_OBJ_DIR)HN_CSS.o $(CXXFLAGS) 
	
$(CSS_OBJ_DIR)HN_StringCSS.o: $(CSS_DIR)HN_StringCSS.cpp
	$(CC) -c $(CSS_DIR)HN_StringCSS.cpp -o $(CSS_OBJ_DIR)HN_StringCSS.o $(CXXFLAGS) 

# Build Javascript objects
$(JS_OBJ_DIR)HN_Javascript.o: $(JS_DIR)HN_Javascript.cpp
	$(CC) -c $(JS_DIR)HN_Javascript.cpp -o $(JS_OBJ_DIR)HN_Javascript.o $(CXXFLAGS) 
	
$(JS_OBJ_DIR)HN_StringJavascript.o: $(JS_DIR)HN_StringJavascript.cpp
	$(CC) -c $(JS_DIR)HN_StringJavascript.cpp -o $(JS_OBJ_DIR)HN_StringJavascript.o $(CXXFLAGS) 

# Build Xml objects
objs/HN_XML/HN_Xml.o: HN_XML/HN_Xml.cpp
	$(CC) -c HN_XML/HN_Xml.cpp -o objs/HN_XML/HN_Xml.o $(CXXFLAGS) 

# Build Attriutes objects
$(ATTR_OBJ_DIR)HN_Attributes.o: $(ATTR_DIR)HN_Attributes.cpp
	$(CC) -c $(ATTR_DIR)HN_Attributes.cpp -o $(ATTR_OBJ_DIR)HN_Attributes.o $(CXXFLAGS) 
	
$(ATTR_OBJ_DIR)HN_ExternAttributes.o: $(ATTR_DIR)HN_ExternAttributes.cpp
	$(CC) -c $(ATTR_DIR)HN_ExternAttributes.cpp -o $(ATTR_OBJ_DIR)HN_ExternAttributes.o $(CXXFLAGS) 
	
$(ATTR_OBJ_DIR)HN_FAttributes.o: $(ATTR_DIR)HN_FAttributes.cpp
	$(CC) -c $(ATTR_DIR)HN_FAttributes.cpp -o $(ATTR_OBJ_DIR)HN_FAttributes.o $(CXXFLAGS) 

# Build document objects
$(DOC_OBJ_DIR)HN_Document.o: $(DOC_DIR)HN_Document.cpp
	$(CC) -c $(DOC_DIR)HN_Document.cpp -o $(DOC_OBJ_DIR)HN_Document.o $(CXXFLAGS) 

# Build DTD objects
$(DTD_OBJ_DIR)HN_DTD.o: $(DTD_DIR)HN_DTD.cpp
	$(CC) -c $(DTD_DIR)HN_DTD.cpp -o $(DTD_OBJ_DIR)HN_DTD.o $(CXXFLAGS) 

# Build node objects
$(NODE_OBJ_DIR)HN_FNode.o: $(NODE_DIR)HN_FNode.cpp
	$(CC) -c $(NODE_DIR)HN_FNode.cpp -o $(NODE_OBJ_DIR)HN_FNode.o $(CXXFLAGS)
	
$(NODE_OBJ_DIR)HN_Node.o: $(NODE_DIR)HN_Node.cpp
	$(CC) -c $(NODE_DIR)HN_Node.cpp -o $(NODE_OBJ_DIR)HN_Node.o $(CXXFLAGS)
	
$(NODE_OBJ_DIR)HN_SNode.o: $(NODE_DIR)HN_SNode.cpp
	$(CC) -c $(NODE_DIR)HN_SNode.cpp -o $(NODE_OBJ_DIR)HN_SNode.o $(CXXFLAGS)

# Perform clean operation
clean:
	rm $(BIN) $(OBJ)

install:
	cp -f $(BIN) /usr/local/lib/libHN_Network.a
	mkdir /usr/local/include/HN_Network -p
	mkdir /usr/local/include/HN_Network/HN_CSS -p
	cp -f $(CSS_DIR)HN_CSS.hpp /usr/local/include/HN_Network/HN_CSS/HN_CSS.hpp
	cp -f $(CSS_DIR)HN_StringCSS.hpp /usr/local/include/HN_Network/HN_CSS/HN_StringCSS.hpp
	
	mkdir /usr/local/include/HN_Network/HN_Javascript -p
	cp -f $(JS_DIR)HN_Javascript.hpp /usr/local/include/HN_Network/HN_Javascript/HN_Javascript.hpp
	cp -f $(JS_DIR)HN_StringJavascript.hpp /usr/local/include/HN_Network/HN_Javascript/HN_StringJavascript.hpp
	
	mkdir /usr/local/include/HN_Network/HN_XML -p
	cp -f HN_XML/HN_Xml.hpp /usr/local/include/HN_Network/HN_XML/HN_Xml.hpp
	
	mkdir /usr/local/include/HN_Network/HN_XML/HN_Attributes -p
	cp -f $(ATTR_DIR)HN_Attributes.hpp /usr/local/include/HN_Network/HN_XML/HN_Attributes/HN_Attributes.hpp
	cp -f $(ATTR_DIR)HN_ExternAttributes.hpp /usr/local/include/HN_Network/HN_XML/HN_Attributes/HN_ExternAttributes.hpp
	cp -f $(ATTR_DIR)HN_FAttributes.hpp /usr/local/include/HN_Network/HN_XML/HN_Attributes/HN_FAttributes.hpp
	
	mkdir /usr/local/include/HN_Network/HN_XML/HN_Document -p
	cp -f $(DOC_DIR)HN_Document.hpp /usr/local/include/HN_Network/HN_XML/HN_Document/HN_Document.hpp
	
	mkdir /usr/local/include/HN_Network/HN_XML/HN_DTD -p
	cp -f $(DTD_DIR)HN_DTD.hpp /usr/local/include/HN_Network/HN_XML/HN_DTD/HN_DTD.hpp
	
	mkdir /usr/local/include/HN_Network/HN_XML/HN_Node -p
	cp -f $(NODE_DIR)HN_FNode.hpp /usr/local/include/HN_Network/HN_XML/HN_Node/HN_FNode.hpp
	cp -f $(NODE_DIR)HN_Node.hpp /usr/local/include/HN_Network/HN_XML/HN_Node/HN_Node.hpp
	cp -f $(NODE_DIR)HN_SNode.hpp /usr/local/include/HN_Network/HN_XML/HN_Node/HN_SNode.hpp
