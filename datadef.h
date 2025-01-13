#ifndef DATADEF_H
#define DATADEF_H
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

#define LIST_INIT_SIZE 30 
#define LISTINCREMENT 10

#define ERROR 0
#define INFEASIBLE -1

enum TokenType {
    StartTag,
    EndTag,
    Comment,
    Character,
    Uninitialized,
    DOCTYPE,
    EndOfFile,
};

#endif //DATADEF_H

