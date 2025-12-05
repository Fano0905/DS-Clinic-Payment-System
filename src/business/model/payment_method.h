#ifndef PAYMENT_METHOD_H
#define PAYMENT_METHOD_H

typedef enum PaymentType {
    PAYPAL,
    BANK_CARD,
    CHECK,
    INVALID = -1
} PaymentType;

typedef struct Payment_Method
{
    int ID;
    PaymentType payment;
    char *username;
    char *password;
} Payment_Method;


#endif //PAYMENT_METHOD