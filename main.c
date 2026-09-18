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

#define WARD_COUNT 4

const char *wardNames[WARD_COUNT] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU"
};

const int wardDailyRates[WARD_COUNT] = { 3000, 6000, 12000, 25000};

const int wardCapacities[WARD_COUNT] = { 20, 10, 10, 5};

int bedOccupancy[WARD_COUNT][20] = {0};

int specialtyQueueCount[SPECIALTY_COUNT] = {0};

#define MAX_PATIENTS 100

int patientID[MAX_PATIENTS];
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int patientUrgency[MAX_PATIENTS];
int patientSpecialty[MAX_PATIENTS];
int patientWard[MAX_PATIENTS];
int patientDays[MAX_PATIENTS];
double patientBill[MAX_PATIENTS];

int patientCount = 0;

void getPatientInput(int index)
{
    printf("\nEnter patient name: ");
    scanf(" %[^\n]", patientName[index]);

    printf("Enter age: ");
    scanf("%d", &patientAge[index]);

    printf("Enter urgency (1-Normal, 2-Urgent, 3-Critical): ");
    scanf("%d", &patientUrgency[index]);

    printf("Enter specialty (1-General, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    scanf("%d", &patientSpecialty[index]);

    printf("Is the patient admitted? (1-Yes, 0-No): ");
    scanf("%d", &patientWard[index]);

    if (patientWard[index] == 1)
    {
        printf("Enter ward (1-General, 2-Paediatric, 3-Surgical, 4-ICU): ");
        scanf("%d", &patientWard[index]);

        printf("Enter number of days: ");
        scanf("%d", &patientDays[index]);
    }
    else
    {
        patientWard[index] = 0;
        patientDays[index] = 0;
    }
}

void assignBed(int index)
{
    int ward;
    int bed;

    if (patientWard[index] == 0)
    {
        return;
    }

    ward = patientWard[index] - 1;

    for (bed = 0; bed < wardCapacities[ward]; bed++)
    {
        if (bedOccupancy[ward][bed] == 0)
        {
            bedOccupancy[ward][bed] = 1;

            printf("Bed allocated: %s - Bed %d\n",
                   wardNames[ward], bed + 1);

            return;
        }
    }

    printf("No available bed in %s.\n", wardNames[ward]);
    patientWard[index] = 0;
    patientDays[index] = 0;
}

int calculateWaitingTime(int index)
{
    int specialty;

    specialty = patientSpecialty[index] - 1;

    return specialtyQueueCount[specialty] * specialtyTimes[specialty];
}

int main()
{
    printf("Smart Hospital Resource Allocation System\n");
    return 0;
}
