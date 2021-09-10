//
//  printer.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#ifndef printer_hpp
#define printer_hpp


class printer {
public:
    printer() {}
    virtual ~printer() {}
    virtual void print() = 0;
    virtual void setSolver(void * slvr) = 0;
};

#endif /* printer_hpp */
