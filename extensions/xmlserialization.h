//
// Created by 58226 on 2023/2/8.
//

#ifndef XMLSERIALIZATION_H
#define XMLSERIALIZATION_H
#include "reflection.h"
#include "xml.h"
//string stream base
#define STRING_STREAM iguana::string_stream
//example: TO_XML(STRING_STREAM ss,T Object)//the filed in Object which has been REFLECTION will be writen in ss;
#define TO_XML iguana::xml::to_xml
//example:FROM_XML(T Object,const char* src)//the xml like string data will be writen in Object;
#define FROM_XML iguana::xml::from_xml

#endif //XMLSERIALIZATION_H
