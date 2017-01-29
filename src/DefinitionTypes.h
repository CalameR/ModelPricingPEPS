//
// Created by calamer on 1/29/17.
//

#ifndef DEFINITIONTYPES_H
#define DEFINITIONTYPES_H

typedef enum modelType {
    BLACK_SCHOLES_MODEL
} modelType;

typedef enum currencyType {
    EUR,
    USD,
    AUD
} currencyType;

typedef enum productType {
    ACTIGO,
    BASKET,
    ASIAN,
    PERFORMANCE,
    QUANTO
} productType;

typedef enum ratesMarketsType {
    CONSTANT_RATES
} ratesMarketsType;

#endif //DEFINITIONTYPES_H
