/*
    Made By: Ankit Agrawal
    Roll No.: IMT2019010
*/

/*
    Things to notice: PC is 12 bits, although 10 bits are sufficient. Probably because MAR is 12 bits
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Main Memory M
long long int Memory[1000][40]; // Create 1000 x 40 memory with all bits set to 0

// Program Control Unit
long long int MAR[12]; // Create 12 bit MAR with all bits set to 0
long long int IR[8];   // Create 8 bit IR with all bits set to 0
long long int IBR[20];  // Create 20 bit IBR with all bits set to 0
long long int PC[12];   // Create 12 bits (addressing 1000 memory locations) with all bits set to 0

// Arithmetic-logic unit (ALU)
long long int AC[40];  // Create 40 bit AC with all bits set to 0
long long int MQ[40];  // Create 40 bit MQ with all bits set to 0
long long int MBR[40]; // Create 40 bit MBR with all bits set to 0

// Flags to keep track of whether the left instruction has been executed or not
long long int Flag = 1; // 1 for LHS, 2 for RHS (no Deviation) and 3 for RHS (Deviation)

void display(long long int v[], int size)
{
    for (int j = 0; j < size; j++)
    {
        printf("%lld", v[j]);
    }
    printf("\n");
}

void display_2(long long int v[], int size)
{
    for (int j = 0; j < size; j++)
    {
        printf("%lld", v[j]);
    }
}

void display_IAS()
{
    printf("MAR : ");
    display(MAR, 12);
    printf("IR : ");
    display(IR, 8);
    printf("IBR : ");
    display(IBR, 20);
    printf("PC : ");
    display(PC, 12);
    printf("AC : ");
    display(AC, 40);
    printf("MQ : ");
    display(MQ, 40);
    printf("MBR : ");
    display(MBR, 40);
    printf("Flag : %lld\n", Flag);
}

void Program_Memory(long long int row, char *s)
{
    for (int j = 0; j < 40; j++)
    {
        Memory[row][j] = s[j] - '0';
    }
}

long long int Binary_to_Decimal(long long int Binary_Vector[], long long int Ending_Point)
{
    long long int to_return = 0;
    long long int power_2 = 1;
    for (int j = Ending_Point; j >= 0; j--)
    {
        to_return += Binary_Vector[j] * power_2;
        power_2 *= 2;
    }
    return to_return;
}

void Decimal_to_Binary(long long int number, long long int to_return[], int *size)
{
    if (number == 0) // This is a special case
    {
        to_return[0] = 0;
        *size = 1;
        return;
    }

    if (number < 0)
    {
        number = number * -1;
    }

    *size = 0;
    while (number / 2 != 0)
    {
        to_return[*size] = number % 2;
        (*size)++;
        number = number / 2;
    }
    to_return[*size] = 1;
    (*size)++;
    // Reverse the array
    for (int i = 0; i < *size / 2; i++)
    {
        long long int temp = to_return[i];
        to_return[i] = to_return[*size - i - 1];
        to_return[*size - i - 1] = temp;
    }
}

long long int OPcode_Match(char *s)
{
    if (strlen(s) != 8)
    {
        return 0;
    }
    else
    {
        long long int is_identical = 1;
        for (int j = 0; j < 8; j++)
        {
            if (IR[j] != s[j] - '0')
            {
                is_identical = 0;
                break;
            }
        }
        return is_identical;
    }
}

void Reset_IAS()
{
    // Resetting PC to 0
    for (int j = 0; j < 12; j++)
    {
        PC[j] = 0;
    }

    // Resetting MAR
    for (int j = 0; j < 12; j++)
    {
        MAR[j] = 0;
    }

    // Resetting IR
    for (int j = 0; j < 8; j++)
    {
        IR[j] = 0;
    }

    // Resetting IBR
    for (int j = 0; j < 20; j++)
    {
        IBR[j] = 0;
    }

    // Resetting AC
    for (int j = 0; j < 40; j++)
    {
        AC[j] = 0;
    }

    // Resetting MQ
    for (int j = 0; j < 40; j++)
    {
        MQ[j] = 0;
    }

    // Resetting MBR
    for (int j = 0; j < 40; j++)
    {
        MBR[j] = 0;
    }

    // We start IAS from the LHS instruction of Memory Address 0 (in Decimal)
    Flag = 1;
}

void Increment_PC()
{
    long long int PC_Value = Binary_to_Decimal(PC, 0);
    PC_Value = PC_Value + 1;
    long long int New_PC[12];
    int size;
    Decimal_to_Binary(PC_Value, New_PC, &size);
    int New_PC_Pointer = size - 1;

    for (int j = 11; j >= 0; j--)
    {
        if (New_PC_Pointer < 0)
        {
            break;
        }
        else
        {
            PC[j] = New_PC[New_PC_Pointer];
            New_PC_Pointer -= 1;
        }
    }

    for (int j = 0; j < 12 - size; j++)
    {
        PC[j] = 0;
    }
}

long long int* ALU(long long int Type_Of_Operation)
{
    static long long int Temporary_Register[40]; // Static array to store the result

    if (Type_Of_Operation == 1) // LOAD -M(X)
    {
        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = MBR[i];
        }

        if (Temporary_Register[0] == 0)
        {
            Temporary_Register[0] = 1;
        }
        else
        {
            Temporary_Register[0] = 0;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 2) // LOAD |M(X)|
    {
        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = MBR[i];
        }

        if (Temporary_Register[0] == 1)
        {
            Temporary_Register[0] = 0;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 3) // LOAD -|M(X)|
    {
        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = MBR[i];
        }

        if (Temporary_Register[0] == 1) // First we calculate |M(X)|
        {
            Temporary_Register[0] = 0;
        }

        if (Temporary_Register[0] == 0) // Then we change its sign if it's positive
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 4) // ADD M(X)
    {
        // Notice that the number to be added is in the MBR already!
        // So we have to perform: MBR + AC;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }
        if (MBR[0] == 1)
        {
            Number_MBR = Number_MBR * -1;
        }

        long long int Result = Number_AC + Number_MBR;

        int size;
        Decimal_to_Binary(Result, Temporary_Register, &size);

        if (Result < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 5) // ADD |M(X)|)
    {
        // So we have to perform : |MBR| + AC;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        // First we will take Absolute value of MBR
        long long int Temporary_MBR_Value[40];
        for (int i = 0; i < 40; i++)
        {
            Temporary_MBR_Value[i] = MBR[i];
        }
        if (Temporary_MBR_Value[0] == 1)
        {
            Temporary_MBR_Value[0] = 0;
        }

        // Next, we calculate addition as usual
        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(Temporary_MBR_Value, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }

        // Since we know that Temporary_MBR_Value is always +ve, we do not need to check the sign bit of Temporary_MBR_Value

        long long int Result = Number_AC + Number_MBR;
        int size;
        Decimal_to_Binary(Result, Temporary_Register, &size);

        if (Result < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 6) // SUB M(X)
    {
        // Notice that the number to be subtracted is in the MBR already!
        // So we have to perform : AC - MBR
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }
        if (MBR[0] == 1)
        {
            Number_MBR = Number_MBR * -1;
        }

        long long int Result = Number_AC - Number_MBR;
        int size;
        Decimal_to_Binary(Result, Temporary_Register, &size);

        if (Result < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 7) // SUB |M(X)|
    {
        // So we have to perform : AC - |MBR|;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = 0;
        }

        long long int Temporary_MBR[40];
        for (int i = 0; i < 40; i++)
        {
            Temporary_MBR[i] = MBR[i];
        }

        if (Temporary_MBR[0] == 1)
        {
            Temporary_MBR[0] = 0;
        }

        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(Temporary_MBR, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }

        // Since we know that Temporary_MBR is always positive, we do not need to check the sign bit of Temporary_MBR

        long long int Result = Number_AC - Number_MBR;
        int size;
        Decimal_to_Binary(Result, Temporary_Register, &size);

        if (Result < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }
    else if (Type_Of_Operation == 8) // MUL M(X)
    {
        // Notice that the number to be multiplied is in the MBR already!
        // So we have to perform : MQ * MBR;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        long long int Number_MQ = Binary_to_Decimal(MQ, 1);
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);

        if (MQ[0] == 1)
        {
            Number_MQ = Number_MQ * -1;
        }
        if (MBR[0] == 1)
        {
            Number_MBR = Number_MBR * -1;
        }

        long long int Result = Number_MQ * Number_MBR;

        int size;
        Decimal_to_Binary(Result, Temporary_Register, &size);

        if (Result < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 9) // DIV M(X) (Performing Remainder Part)
    {
        // Notice that the divisor is in the MBR already!
        // So we have to perform : AC % MBR;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = 0;
        }

        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }
        if (MBR[0] == 1)
        {
            Number_MBR = Number_MBR * -1;
        }

        long long int Remainder = Number_AC % Number_MBR;

        if (Remainder < 0) // In case Remainder is negative, we make it positive
        {
            Remainder = Remainder + Number_MBR;
        }

        int size;
        Decimal_to_Binary(Remainder, Temporary_Register, &size);

        if (Remainder < 0) // Setting the Sign Bit
        {
            Temporary_Register[0] = 1;
        }

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 10) // DIV M(X) (Performing Quotient Part)
    {
        // Notice that the divisor is in the MBR already!
        // So we have to perform : AC / MBR;
        // We don't care about overflow - no flags as of now, we just truncate extra bits

        for (int i = 0; i < 40; i++)
        {
            Temporary_Register[i] = 0;
        }

        long long int Number_AC = Binary_to_Decimal(AC, 1);
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);

        if (AC[0] == 1)
        {
            Number_AC = Number_AC * -1;
        }
        if (MBR[0] == 1)
        {
            Number_MBR = Number_MBR * -1;
        }

        long long int Quotient = Number_AC / Number_MBR; // The Quotient CAN be negative

        if (Quotient < 0) // If Quotient is negative, then we must reflect it in the sign bit as well!
        {
            Temporary_Register[0] = 1;
        }

        int size;
        Decimal_to_Binary(Quotient, Temporary_Register, &size);

        return Temporary_Register;
    }

    else if (Type_Of_Operation == 11) // LSH
    {
        for (long long int j = 1; j <= 38; j++)
        {
            Temporary_Register[j] = Temporary_Register[j + 1];
        }
        Temporary_Register[39] = 0;
        return Temporary_Register;
    }

    else if (Type_Of_Operation == 12) // RHS
    {
        for (long long int j = 39; j >= 2; j--)
        {
            Temporary_Register[j] = Temporary_Register[j - 1];
        }
        Temporary_Register[1] = 0;
        return Temporary_Register;
    }

    else if (Type_Of_Operation == 13) // STOR M(X, 8:19)
    {
        for (long long int j = 8; j < 20; j++)
        {
            Temporary_Register[j] = Temporary_Register[j + 20];
        }
        return Temporary_Register;
    }

    else if (Type_Of_Operation == 14) // STOR M(X, 28:39)
    {
        for (long long int j = 28; j < 40; j++)
        {
            Temporary_Register[j] = Temporary_Register[j];
        }
        return Temporary_Register;
    }

    else if (Type_Of_Operation == 15) // Before finding a/b and a%b, check if b is equal to 0 or not
    {
        static long long int Zero_Check[40]; // Zero_Check[39] == 0 if b is equal to 0, else 1
        long long int Number_MBR = Binary_to_Decimal(MBR, 1);
        if (Number_MBR != 0)
        {
            Zero_Check[39] = 1;
        }
        return Zero_Check;
    }

    else // This will never be reached, but to prevent compiler warnings, we need to return something
    {
        static long long int to_return[40]; // We return an empty array of 40 bits
        return to_return; // Note: This will never be reached!
    }
}

void Fetch_Cycle_Left()
{
    MAR = PC;
    long long int X = Binary_to_Decimal(MAR, 0);
    MBR = Memory[X];

    for(long long int j = 20; j < 40; j++)    //Copy RHS Instruction to IBR
    {
        IBR[j-20] = MBR[j];
    }

    for(long long int j = 0 ; j < 8; j++)     //Copy OpCode to IR
    {
        IR[j] = MBR[j];
    }
    for(long long int j = 8; j < 20; j++)     //Copy Address to MAR
    {
        MAR[j-8] = MBR[j];
    }
}

void Fetch_Cycle_Right_No_Deviation()
{
    for(long long int j = 0 ; j < 8; j++)     //Copy OpCode of RHS Instruction to IR
    {
        IR[j] = IBR[j];
    }
    for(long long int j = 8; j < 20; j++)     //Copy Address of RHS Instruction to MAR
    {
        MAR[j-8] = IBR[j];
    }
    Increment_PC();
}

void Fetch_Cycle_Right_Deviation()
{
    MAR = PC;
    long long int X = Binary_to_Decimal(MAR, 0);
    MBR = Memory[X];

    for(long long int j = 20; j < 28; j++)    //Copy OpCode of RHS Instruction to IR
    {
        IR[j-20] = MBR[j];
    }
    for(long long int j = 28; j < 40; j++)    //Copy Address of RHS Instruction to MAR
    {
        MAR[j-28] = MBR[j];
    }

    Increment_PC();
}

/*
    FOR THE EXECUTION CYCLE, THE RETURN VALUES OF THE FUNCTION ARE: ==

    --> 0 : The instruction has executed successfully.

    --> 1 : Start Fetch_Cycle_Left(), i.e. we have to execute left instruction

    --> 2 : Start Fetch_Cycle_Right_No_Deviation(), i.e. we have to execute
            right instruction after left instruction of same memory address
            has been executed

    --> 3 : Start Fetch_Cycle_Right_Deviation(), i.e. we have to execute the
            the right instruction but this right instruction is NOT the
            Right Instruction of the address whose Left instruction was
            executed earlier

    --> -1 : STOP! The HALT has been issued, or something wrong has happened
             (for example, JUMP M(X,0:19) or JUMP M(X,0:19) has been issued
              but M(X) contains data (and NOT instruction) :(
*/

long long int Execution_Cycle()
{
    if (OPcode_Match("00001010") == 1) // LOAD MQ
    {
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = MQ[j];
        }
        return 0;
    }

    else if (OPcode_Match("00001001") == 1) // LOAD MQ,M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0); // Decode the Address
        MBR = Memory[X];                            // Transfer M(X) to MBR
        for (long long int j = 0; j < 40; j++)
        {
            MQ[j] = MBR[j]; // Transfer contents of MBR to MQ
        }
        return 0;
    }

    else if (OPcode_Match("00100001") == 1) // STOR M(X)
    {
        for (long long int j = 0; j < 40; j++)
        {
            MBR[j] = AC[j]; // Transfer contents of AC to MBR (AC has NO direct connection to MEMORY)
        }
        long long int X = Binary_to_Decimal(MAR, 0);
        for (long long int j = 0; j < 40; j++)
        {
            Memory[X][j] = MBR[j];
        }
        return 0;
    }

    else if (OPcode_Match("00000001") == 1) // LOAD M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = MBR[j]; // This was added during Testing Phase
        }
        return 0;
    }

    else if (OPcode_Match("00000010") == 1) // LOAD -M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        // Now data of MBR is sent to ALU, which calculates -M(X) and stores it in AC
        long long int *result = ALU(1);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if (OPcode_Match("00000011") == 1) // LOAD |M(X)|
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        // Now data of MBR is sent to ALU, which calculates |M(X)| and stores it in AC
        long long int *result = ALU(2);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if (OPcode_Match("00000100") == 1) // LOAD -|M(X)|
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        // Now data of MBR is sent to ALU, which calculates -|M(X)| and stores it in AC
        long long int *result = ALU(3);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if (OPcode_Match("00001101") == 1) // JUMP M(X,0:19)
    {
        for (long long int j = 0; j < 12; j++)
        {
            PC[j] = MAR[j];
        }
        return 1;
    }

    else if (OPcode_Match("00001110") == 1) // JUMP M(X,20:39)
    {
        for (long long int j = 0; j < 12; j++)
        {
            PC[j] = MAR[j + 20];
        }
        return 3; // We are jumping to RHS instruction - We need to deviate flow!
    }

    else if (OPcode_Match("00001111") == 1) // JUMP+ M(X,0:19)
    {
        if (AC[0] == 0) // Since the number in AC is non-negative, we take the next instruction from the left half of M(X)
        {
            for (long long int j = 0; j < 12; j++)
            {
                PC[j] = MAR[j];
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }

    else if (OPcode_Match("00010000") == 1) // JUMP+ M(X,20:39)
    {
        if (AC[0] == 0) // Since the number in AC is non-negative, we take the next instruction from the right half of M(X)
        {
            for (long long int j = 0; j < 12; j++)
            {
                PC[j] = MAR[j + 20];
            }
            return 3; // We are jumping to RHS instruction - We need to deviate flow!
        }
        else
        {
            return 0;
        }
    }

    else if (OPcode_Match("00000101") == 1) // ADD M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X]; // Perform Memory Read
        long long int *result = ALU(4); // Perform Addition and store the result in ALU
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if (OPcode_Match("00000111") == 1) // ADD |M(X)|
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X]; // Perform Memory Read
        long long int *result = ALU(5);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if (OPcode_Match("00000110") == 1) // SUB M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        long long int *result = ALU(6);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }
    else if (OPcode_Match("00001000") == 1) // SUB |M(X)|
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        long long int *result = ALU(7);
        for (long long int j = 0; j < 40; j++)
        {
            AC[j] = result[j];
        }
        return 0;
    }

    else if(OPcode_Match("00001011") == 1)      //MUL M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        vector <long long int> Result = ALU(8);

        /*
          The result is 80 bits (including sign bit)
          out of which the first 40 MSBs would be
          stored in AC, while the remaining 40 bits
          would be stored in MQ
        */

        //Copying first 40 bits to AC
        for(long long int j = 0 ; j < 40; j++)
        {
            AC[j] = Result[j];
        }

        //Copying next 40 bits to MQ
        for(long long int j = 40; j < 80; j++)
        {
            MQ[j-40] = Result[j];
        }
        return 0;
    }

    else if(OPcode_Match("00001100") == 1)     //DIV M(X)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];

        /*
          For this operation, we have two result,
          the Quotient is stored in MQ, while the
          remainder is stored in AC, i.e.

          MQ = AC/MBR
          AC = AC % MBR

          Before doing the actual operations,
          we must check if divisor is 0 or not
        */

        vector <long long int> Check_Divisor = ALU(15);
        if(Check_Divisor[39] == 0)  //So that means that divisor is 0! We can't divide anything by 0
        {
            return -1;      //HALT the machine!
        }

        MQ = ALU(10);       //Loading MQ with the Quotient
        AC = ALU(9);       //Loading AC with the Remainder
        return 0;
    }

    else if(OPcode_Match("00010100") == 1)      //LSH
    {
        AC = ALU(11);
        return 0;
    }

    else if(OPcode_Match("00010101") == 1)      //RHS
    {
        AC = ALU(12);
        return 0;
    }

    else if(OPcode_Match("00010010") == 1)      //STOR M(X, 8:19)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        MBR = ALU(13);
        Memory[X] = MBR;
        return 0;
    }

    else if(OPcode_Match("00010011") == 1)      //STOR M(X, 28:39)
    {
        long long int X = Binary_to_Decimal(MAR, 0);
        MBR = Memory[X];
        MBR = ALU(14);
        Memory[X] = MBR;
        return 0;
    }

    else if(OPcode_Match("11111111") == 1)      //HALT (Custom Instruction)
    {
        return -1;
    }

    else if(OPcode_Match("00000000") == 1)      //There is no left instruction, but there might be right instruction
    {
        return 0;       //We allow for normal execution since there might be right instruction (and that might be probably HALT!)
    }

    else        //Although this will never be reached (unless someone programs memory incorrectly!)
    {
        return 0;      //We will still continue on, as there is nothing to be done...
    }
}

int main()
{
    long long int Display_Memory_Start = 0;     //Once the IAS machine is executed, starting from this address, value of memory at this address will be displayed
    long long int Display_Memory_End = 999;     //The value of memory till this address will be displayed once the IAS machine is executed

    //Preprogram the memory
    //Programming the memory for calculating the sum of first 10 natural numbers

    Program_Memory(0, "0000000100000110011000000101000001100100");
    Program_Memory(1, "0010000100000110011000000001000001100100");
    Program_Memory(2, "0000011000000110010100100001000001100100");
    Program_Memory(3, "0000111100000000000011111111000000000000");

    Program_Memory(100, "0000000000000000000000000000000000001010");
    Program_Memory(101, "0000000000000000000000000000000000000001");
    Program_Memory(102, "0000000000000000000000000000000000000000");

    Display_Memory_Start = 102;
    Display_Memory_End = 102;

    //Reset the IAS
    Reset_IAS();

    while(1)
    {
        if(Flag == 1)      //So the left instruction has not been executed and we've to execute it
        {
            //Start the Fetch Cycle for RHS Instruction
            Fetch_Cycle_Left();

            //Start the execute Cycle, and store the result
            long long int Result = Execution_Cycle();

            if(Result == 0)     //Normal execution of LHS instruction, we know that we will Start the cycle for RHS instruction
            {
                Flag = 2;  //Left instruction has been executed successfully
            }
            else if(Result == 1)        //So the instruction that was executed was JUMP M(X,0:19)
            {
                Flag = 1;      //We will again start the Fetch Cycle for a new LHS instruction
            }
            else if(Result == 3)        //So the instruction that was executed was JUMP M(X,20:39)
            {
                Flag = 3;       //We will start the Fetch Cycle for RHS instruction but with deviation
            }
            else if(Result == -1)   //HALT the Machine!!
            {
                break;
            }
            else        //Although this will never be reached, yet as a default case, we will HALT the IAS
            {
                break;
            }
        }
        else if(Flag == 2)
        {
            //Start Fetch Cycle for RHS instruction (Without deviation)
            Fetch_Cycle_Right_No_Deviation();

            //Start the Execute Cycle, and store the result
            long long int Result = Execution_Cycle();

            if(Result == 0)     //Normal execution of RHS instruction, we know that we will Start the cycle for LHS instruction of next address
            {
                Flag = 1;
            }
            else if(Result == 1)       //So the instruction that was executed was JUMP M(X,0:19)
            {
                Flag = 1;      //We will again start the Fetch Cycle for a new LHS instruction
            }
            else if(Result == 3)        //So the instruction that was executed was JUMP M(X,20:39)
            {
                Flag = 3;       //We will start the Fetch Cycle for RHS instruction but with deviation
            }
            else if(Result == -1)   //HALT the Machine!!
            {
                break;
            }
            else        //Although this will never be reached, yet as a default case, we will HALT the IAS
            {
                break;
            }
        }
        else if(Flag == 3)
        {
            //Start Fetch Cycle for RHS instruction (With deviation)
            Fetch_Cycle_Right_Deviation();

            //Start the Execute Cycle, and store the result
            long long int Result = Execution_Cycle();

            if(Result == 0)     //Normal execution of RHS instruction, we know that we will Start the cycle for LHS instruction of next address
            {
                Flag = 1;
            }
            else if(Result == 1)       //So the instruction that was executed was JUMP M(X,0:19)
            {
                Flag = 1;      //We will again start the Fetch Cycle for a new LHS instruction
            }
            else if(Result == 3)        //So the instruction that was executed was JUMP M(X,20:39)
            {
                Flag = 3;       //We will start the Fetch Cycle for RHS instruction but with deviation
            }
            else if(Result == -1)   //HALT the Machine!!
            {
                break;
            }
            else        //Although this will never be reached, yet as a default case, we will HALT the IAS
            {
                break;
            }
        }
        else
        {
            break;
        }
        //display_IAS();  //This piece of code displays the State of IAS machine after each instruction has been executed
        //cout<<"----------------------------------------"<<"\n";
    }

    //This part of the program displays the values in Memory
    for(long long int j = Display_Memory_Start; j <= Display_Memory_End; j++)
    {
        cout<<" Displaying Memory "<<j<<" : ";
        display_2(Memory[j]);
        cout<<" : Decimal Value : "<<Binary_to_Decimal(Memory[j], 1);
        cout<<"\n";
    }
    return 0;
}
