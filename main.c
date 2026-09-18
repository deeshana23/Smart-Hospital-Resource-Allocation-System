#include <stdio.h>

#define SPECIALTY_COUNT 4

const char *specialtyNames[SPECIALTY_COUNT] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const int specialtyFees[SPECIALTY_COUNT] = {1500, 2500, 4500, 5000};

const int specialtyTimes[SPECIALTY_COUNT] = {15, 20, 30, 30};

const int specialtyDailyCaps[SPECIALTY_COUNT] = { 30, 20, 12, 10};


int main()
{
    printf("Smart Hospital Resource Allocation System\n");
    return 0;
}
