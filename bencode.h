//
// Created by lklake on 2019/7/30.
//

#ifndef DHT_BENCODE_H
#define DHT_BENCODE_H

#include "str.h"
#include "hashtable.h"


dict* bdecode(string* raw);

dict* d_dict(string* raw,int* index);

string* d_int(string* raw,int* index);

string* d_list(string* raw,int* index);

string* d_str(string* raw,int* index);

char* d_cstr(string* raw,int* index);

#endif //DHT_BENCODE_H
