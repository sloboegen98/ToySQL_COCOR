#include "Query.h"

void Query::print(){
    //std::wofstream outfile ("test_result.txt", std::ios_base::out);
    std::wcout << "ATTRIBUTES:\n";
    for (size_t i = 0; i < (size_t)attrs.size(); ++i){
        std::wcout << '\t' << attrs[i] << '\n';
    }
    
    std::wcout << "TABLES:\n";
    for (size_t i = 0; i < (size_t)from.size(); ++i){
        std::wcout << '\t' << from[i] << '\n';
    }

    if (where.size()){
        std::wcout << "PREDICATE:\n";
        for (size_t i = 0; i < (size_t)where.size(); ++i){
            std::wcout << '\t' << where[i].left << " " <<
            where[i].relation << " " << where[i].right << '\n';
        }
    }

    //outfile.close();
}
