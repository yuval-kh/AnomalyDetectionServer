
#include "StandardIO.h"


StandardIO::StandardIO() {}
StandardIO::~StandardIO(){}
string StandardIO::read() {
    string input;
//    cin >>input;
//    std::getline(std::cin, input, '\n');
    std::getline(std::cin, input, '\n');
    if(input.compare("")==0)
        std::getline(std::cin, input, '\n');
    return input;
}
void StandardIO::read(float *f) {
    float input;
    cin >>input;
    *f=input;
}
void StandardIO::write(string text){
    cout<<text;
}
void StandardIO::write(float f) {
//    cout<<fixed;
//    cout<<cout.precision(3);
    cout<<f;
}

//void StandardIO::read(int *f) {
//    int input;
//    cin >>input;
//    *f=input;
//}