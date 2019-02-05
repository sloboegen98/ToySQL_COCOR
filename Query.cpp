#include "Query.h"

#include <iostream>

void Query::print(){
    //std::wofstream outfile ("test_result.txt", std::ios_base::out);
    std::wcout << "ATTRIBUTES:\n";
    for (int i = 0; i < attrs.size(); ++i){
        std::wcout << '\t' << attrs[i] << '\n';
    }
    
    std::wcout << "TABLES:\n";
    for (int i = 0; i < from.size(); ++i){
        std::wcout << '\t' << from[i] << '\n';
    }

    if (where.size()){
        std::wcout << "PREDICATE:\n";
        for (int i = 0; i < where.size(); ++i){
            std::wcout << '\t' << where[i].left << " " <<
            where[i].relation << " " << where[i].right << '\n';
        }
    }

    //outfile.close();
}
