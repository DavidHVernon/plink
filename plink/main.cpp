#include <iostream>

#include "plink.h"

// Usage: plink [result_code sound ... [ default_sound ] -- ] command ...
// plink 0 yeah boo -- build
int main(int argc, const char * argv[]) {
    auto plink = build_plink(argc, argv);
    if (!plink){
        exit(1);
    }
    return plink.value().exec();
}
