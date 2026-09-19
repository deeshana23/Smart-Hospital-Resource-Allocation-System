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
int patientBed [MAX_PATIENTS];
double patientBill[MAX_PATIENTS];

int patientCount = 0;

void saveBedStatus();

void getPatientInput(int index)
{
    printf("\nEnter patient name: ");
    scanf(" %[^\n]", patientName[index]);

    do
    {

       printf("Enter age: ");
       scanf("%d", &patientAge[index]);


       if (patientAge[index] < 0)

       {
         printf("Invalid age. Please enter a valid age.\n");
       }

    } while (patientAge[index] < 0);

    do
    {

       printf("Enter urgency (1-Normal, 2-Urgent, 3-Critical): ");
       scanf("%d", &patientUrgency[index]);

        if (patientUrgency[index] < 1 || patientUrgency[index] > 3)
        {
            printf("Invalid urgency. Please enter 1-3.\n");
        }

    } while (patientUrgency[index] < 1 || patientUrgency[index] > 3);

    do
    {

       printf("Enter specialty (1-General, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
       scanf("%d", &patientSpecialty[index]);

       if (patientSpecialty[index] < 1 || patientSpecialty[index] > 4)
        {
            printf("Invalid specialty. Please enter 1-4.\n");
        }

    } while (patientSpecialty[index] < 1 || patientSpecialty[index] > 4);

    do
    {

        printf("Is the patient admitted? (1-Yes, 0-No): ");
        scanf("%d", &patientWard[index]);

        if (patientWard[index] != 0 && patientWard[index] != 1)
        {
            printf("Invalid choice. Please enter 1 or 0.\n");
        }

    } while (patientWard[index] != 0 && patientWard[index] != 1);

       if (patientWard[index] == 1)
    {
        do
        {
            printf("Enter ward (1-General, 2-Paediatric, 3-Surgical, 4-ICU): ");
            scanf("%d", &patientWard[index]);

            if (patientWard[index] < 1 || patientWard[index] > 4)
            {
                printf("Invalid ward. Please enter 1-4.\n");
            }

        } while (patientWard[index] < 1 || patientWard[index] > 4);


        do
        {
            printf("Enter number of days: ");
            scanf("%d", &patientDays[index]);

            if (patientDays[index] <= 0)
            {
                printf("Invalid number of days. Please enter a positive number.\n");
            }

        } while (patientDays[index] <= 0);
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
        patientBed[index]=0;
        return;
    }

    ward = patientWard[index] - 1;

    for (bed = 0; bed < wardCapacities[ward]; bed++)
    {
        if (bedOccupancy[ward][bed] == 0)
        {
            bedOccupancy[ward][bed] = 1;
            patientBed [index]=bed+1;

            printf("Bed allocated: %s - Bed %d\n",
                   wardNames[ward], patientBed  [index] );

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

double calculateSurcharge(int index)
{
    int specialty;
    double baseFee;

    specialty = patientSpecialty[index] - 1;
    baseFee = specialtyFees[specialty];

    if (patientUrgency[index] == 2)
    {
        return baseFee * 0.20;
    }
    else if (patientUrgency[index] == 3)
    {
        return baseFee * 0.50;
    }
    else
    {
        return 0.0;
    }
}

double calculateSubsidy(int index, double grossAmount)
{
    if (patientAge[index] < 5 || patientAge[index] > 65)
    {
        return grossAmount * 0.15;
    }
    else
    {
        return 0.0;
    }
}

double calculateBill(int index)
{
    int specialty;
    double baseFee;
    double surcharge;
    double wardCost;
    double grossAmount;
    double subsidy;
    double finalAmount;

    specialty = patientSpecialty[index] - 1;

    baseFee = specialtyFees[specialty];
    surcharge = calculateSurcharge(index);

    if (patientWard[index] != 0)
    {
        wardCost = patientDays[index] * wardDailyRates[patientWard[index] - 1];
    }
    else
    {
        wardCost = 0.0;
    }

    grossAmount = baseFee + surcharge + wardCost;

    subsidy = calculateSubsidy(index, grossAmount);

    finalAmount = grossAmount - subsidy;

    patientBill[index] = finalAmount;

    return finalAmount;
}

void printReceipt(int index)
{
    int specialty;

    specialty = patientSpecialty[index] - 1;

    printf("\n========== PATIENT RECEIPT ==========\n");
    printf("Patient ID       : %d\n", patientID[index]);
    printf("Patient Name     : %s\n", patientName[index]);
    printf("Age              : %d\n", patientAge[index]);
    printf("Specialty        : %s\n", specialtyNames[specialty]);

    printf("Urgency          : ");

    if (patientUrgency[index] == 1)
    {
        printf("Normal\n");
    }
    else if (patientUrgency[index] == 2)
    {
        printf("Urgent\n");
    }
    else
    {
        printf("Critical\n");
    }

    printf("Waiting Time : %d minutes\n", calculateWaitingTime(index));

    if (patientWard[index] != 0)
    {
        printf("Ward  : %s\n",

            wardNames[patientWard[index] - 1]);

        printf("Bed Number  : %d\n", patientDays[index]);
        printf("Number of Days   : %d\n", patientDays[index]);
    }
    else
    {
        printf("Admission  : Outpatient\n");
    }

    printf("Final Bill   : Rs. %.2f\n", patientBill[index]);
    printf("=====================================\n");
}

void registerPatient()
{
    int index;

    index = patientCount;

    patientID[index] = patientCount + 1;

    getPatientInput(index);

    assignBed(index);

    saveBedStatus();

    patientBill[index] = calculateBill(index);

    printReceipt(index);

    specialtyQueueCount[patientSpecialty[index] - 1]++;

    patientCount++;
}

void displayPatientsByPriority()
{
    int priority;
    int i;

    printf("\nPatients by Priority\n");

    for (priority = 3; priority >= 1; priority--)
    {
        for (i = 0; i < patientCount; i++)
        {
            if (patientUrgency[i] == priority)
            {
                printf("\nPatient ID: %d\n", patientID[i]);
                printf("Patient Name: %s\n", patientName[i]);

                if (priority == 3)
                {
                    printf("Urgency: Critical\n");
                }
                else if (priority == 2)
                {
                    printf("Urgency: Urgent\n");
                }
                else
                {
                    printf("Urgency: Normal\n");
                }
            }
        }
    }
}

void showUrgencyReport()
{
    int normalCount = 0;
    int urgentCount = 0;
    int criticalCount = 0;
    int i;

    for (i = 0; i < patientCount; i++)
    {
        if (patientUrgency[i] == 1)
        {
            normalCount++;
        }
        else if (patientUrgency[i] == 2)
        {
            urgentCount++;
        }
        else if (patientUrgency[i] == 3)
        {
            criticalCount++;
        }
    }

    printf("\n===== URGENCY REPORT =====\n");
    printf("Normal Patients   : %d\n", normalCount);
    printf("Urgent Patients   : %d\n", urgentCount);
    printf("Critical Patients : %d\n", criticalCount);
}

void showRevenueReport()
{
    double totalRevenue = 0.0;
    double totalDiscounts = 0.0;
    int i;

    for (i = 0; i < patientCount; i++)
    {
        double grossAmount;
        double discount;

        grossAmount = patientBill[i];

        if (patientAge[i] < 5 || patientAge[i] > 65)
        {
            discount = grossAmount * 0.15;
        }
        else
        {
            discount = 0.0;
        }

        totalRevenue = totalRevenue + patientBill[i];
        totalDiscounts = totalDiscounts + discount;
    }

    printf("\n===== REVENUE REPORT =====\n");
    printf("Total Revenue   : Rs. %.2f\n", totalRevenue);
    printf("Total Discounts : Rs. %.2f\n", totalDiscounts);
}

void showBedOccupancyReport()
{
    int ward;
    int bed;
    int occupiedBeds;
    double occupancyPercentage;

    printf("\n===== BED OCCUPANCY REPORT =====\n");

    for (ward = 0; ward < WARD_COUNT; ward++)
    {
        occupiedBeds = 0;

        for (bed = 0; bed < wardCapacities[ward]; bed++)
        {
            if (bedOccupancy[ward][bed] == 1)
            {
                occupiedBeds++;
            }
        }

        occupancyPercentage = (occupiedBeds * 100.0) / wardCapacities[ward];

        printf("\n%s\n", wardNames[ward]);
        printf("Occupied Beds : %d\n", occupiedBeds);
        printf("Capacity      : %d\n", wardCapacities[ward]);
        printf("Occupancy     : %.2f%%\n", occupancyPercentage);
    }
}

void showHighestPayingPatient()
{
    int i;
    int highestIndex;
    double highestBill;

    if (patientCount == 0)
    {
        printf("\nNo patients registered.\n");
        return;
    }

    highestIndex = 0;
    highestBill = patientBill[0];

    for (i = 1; i < patientCount; i++)
    {
        if (patientBill[i] > highestBill)
        {
            highestBill = patientBill[i];
            highestIndex = i;
        }
    }

    printf("\n===== HIGHEST PAYING PATIENT =====\n");
    printf("Patient Name : %s\n", patientName[highestIndex]);
    printf("Patient Bill : Rs. %.2f\n", patientBill[highestIndex]);
}

void saveBedStatus()
{
    FILE *file;
    int ward;
    int bed;

    file = fopen("beds_status.txt", "w");

    if (file == NULL)
    {
        printf("Unable to save bed status.\n");
        return;
    }

    for (ward = 0; ward < WARD_COUNT; ward++)
    {
        for (bed = 0; bed < wardCapacities[ward]; bed++)
        {
            fprintf(file, "%d ", bedOccupancy[ward][bed]);
        }

        fprintf(file, "\n");
    }

    fclose(file);

    printf("Bed status saved successfully.\n");
}

int main()
{
    int choice;
    printf("Smart Hospital Resource Allocation System\n");

     while (1)
    {
        printf("\n1. Register Patient\n");
        printf("2. View Patients by Priority\n");
        printf("3. Urgency Report\n");
        printf("4. Revenue Report\n");
        printf("5. Bed Occupancy Report\n");
        printf("6. Highest-Paying Patient\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            registerPatient();
        }


         else if (choice == 2)
        {
            displayPatientsByPriority();
        }

         else if (choice == 3)
        {
           showUrgencyReport();
        }

        else if  (choice == 4)
        {
            showRevenueReport();

        }

        else if  (choice == 5)
        {
           showBedOccupancyReport();
        }

         else if  (choice == 6)
        {
          showHighestPayingPatient();
        }

        else if  (choice == 7)
        {
            printf("Thank you.\n");
            break;
        }

        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
