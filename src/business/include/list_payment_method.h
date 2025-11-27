#ifndef LIST_PAYMENT_METHOD_H
#define LIST_PAYMENT_METHOD_H

#include "payment_method.h"

typedef struct List_Payment_Method
{
    Payment_Method *payment_method;
    struct List_Payment_Method *next_method;
} List_Payment_Method;


#endif // LIST_PAYMENT_METHOD_H