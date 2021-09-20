

#ifndef EX4_STANDARDIO_H
#define EX4_STANDARDIO_H
#include "commands.h"

class StandardIO : public DefaultIO{

public:
    string read() override;
    void write(string text) override;
    void write(float f) override;
    void read(float *f) override;
//    void read(int *f) override;
    ~StandardIO();
    StandardIO();


};


#endif //EX4_STANDARDIO_H
