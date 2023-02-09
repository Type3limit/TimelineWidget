//
// Created by 58226 on 2023/2/8.
//

#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H
#include "json.h"
#include "reflection.h"
//string stream base
#define STRING_STREAM iguana::string_stream
// example:TO_JSON(STRING_STREAM ss,T Object)//the filed in Object which has been REFLECTION will be writen in ss;
#define TO_JSON iguana::json::to_json
// example:FROM_JSON(T Object,const char* src)//the json like string data will be writen in Object;
#define FROM_JSON iguana::json::from_json
#endif //JSONSERIALIZATION_H
