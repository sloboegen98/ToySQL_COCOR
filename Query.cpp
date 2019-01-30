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

  /*(. std::vector<Predicate> prbr; .)
    '(' whparams<prbr> ')'
    (. 
        for (int i = 0; i < prbr.size(); ++i)
            preds.push_back(prbr[i]);
    .)

        (. Predicate pr; .)
    whparam<pr> (. preds.push_back(pr); .)
    {
        (. std::vector<Predicate> prs; .)
        ("AND"|"OR") whparams<prs> 
        (.
            for (int i = 0; i < prs.size(); ++i)
                preds.push_back(prs[i]);
        .)    
    }
    |
    |*/