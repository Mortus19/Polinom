#include <iostream>
#include "Polinom.h"
using namespace std;
int main() {
    setlocale(LC_ALL, "Russian");
    Polinom pol,pol2,pol3;
    ifstream in("input.txt");
    ofstream out("output.txt");
    in>>pol;
    in>>pol2;
    cout<<pol<<'\n';
    cout<<pol2<<'\n';
    pol3 = pol * pol2;
    cout<<pol3<<'\n';
    cout<<pol3.calc(2,0,0)<<'\n';
    return 0;
}