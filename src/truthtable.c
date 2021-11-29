#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum {AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER} type;

typedef struct {
    type gateName; 
    int* inputVariables;
    int* outputVariables;
    int* selectorVariables;

    int inputNumber;
    int outputNumber;
    int selectingNumber;
    int decodingNumber;
} gate;

// This prints out the contents of int* Variables.
void printIntPointer (int* intPointer, int length) {
    for (int i=0; i<length; i++) {
        printf("%d | ", intPointer[i]);
    }
    printf("\n");
}

// This prints out the contents of uniqueVariables.
void printUniqueVariables (char** uniqueVariables, int totalUniqueVariables) {
    for (int i=0; i<totalUniqueVariables; i++) {
        printf("%s | ", uniqueVariables[i]);
    }
    printf("\n");
}

// This prints the input side of the truth table.
void printInputSideTruthTable(int* truthTable, int numInputs) {
    for (int i=2; i<numInputs+2; i++) {printf("%d ", truthTable[i]);}
}

// This prints the output side of the truth table. 
void printOutputSideTruthTable(int* truthTable, int numInputs, int numOutputs) {
    for (int i=0; i<numOutputs; i++) {
        if (i < (numOutputs-1)) {printf("%d ", truthTable[numInputs+2+i]);}
        else {printf("%d", truthTable[numInputs+2+i]);}
    }
}

// This function takes input1, input2 and outputs output1 for andGate with a truthTable.
void andLogicGate (int input1, int input2, int output1, int* truthTable) {
    truthTable[output1] = truthTable[input1] && truthTable[input2];
}

// This function takes input1, input2, and outputs output1 for orGate with a truthTable.
void orLogicGate (int input1, int input2, int output1, int* truthTable) {
    truthTable[output1] = truthTable[input1] || truthTable[input2];
}

// This function takes input1, input2, and outputs output1 for nandGate with a truthTable.
void nandLogicGate (int input1, int input2, int output1, int* truthTable) {
    if (truthTable[input1] == 1 && truthTable[input2] == 1) {truthTable[output1] = 0;}
    else {truthTable[output1]=1;}
}

// This function takes input1, input2, and outputs output1 for norGate with a truthTable.
void norLogicGate (int input1, int input2, int output1, int* truthTable) {
    if (truthTable[input1] == 0 && truthTable[input2] == 0) {truthTable[output1] = 1;}
    else {truthTable[output1]=0;}
}

// This function takes input1, input2, and outputs output1 for xorGate with a truthTable.
void xorLogicGate (int input1, int input2, int output1, int* truthTable) {
    if (truthTable[input1] != truthTable[input2]) {
        truthTable[output1] = 1;
    }
    else {
        truthTable[output1] = 0;
    }
}

// This function takes input1 and outputs output1 for notGate with a truthTable.
void notLogicGate (int input1, int output1, int* truthTable) {
    if (truthTable[input1] == 0) {truthTable[output1] = 1;}
    else {truthTable[output1] = 0;}
}

// This function takes input1 and outputs1 for passGate with a truthTable. 
void passLogicGate (int input1, int output1, int* truthTable) {
    truthTable[output1] = truthTable[input1];
}

// This function takes in pointers to input and yields output through the decoderNum and truthTable.  
void decoderLogicGate (int* inputsPointer, int* outputsPointer, int* truthTable, int decoderNum) {

    int maxNum = pow(2, decoderNum);
    for (int i=0; i < maxNum; i++) {
        truthTable[outputsPointer[i]] = 0;
    }

    int numValue = 0;
    for (int i=0; i < decoderNum; i++) {
        if (truthTable[inputsPointer[i]] == 1) {
            numValue += pow(2, decoderNum-i-1);
        }
    }
    
    truthTable[outputsPointer[numValue]] = 1;
}

// This function takes in pointers to inputs, outputs, and selectors to create the multiplexer. 
void multiplexerLogicGate (int* inputsPointer, int* outputsPointer, int* selectorsPointer, int* truthTable, int selectNumber) {

    int jumpToInputNumber = 0;

    for (int i=0; i < selectNumber; i++) {
        if (truthTable[selectorsPointer[i]] == 1) {
            jumpToInputNumber += pow(2, selectNumber-i-1);
            //printf("JumpToInputNumber: %d\n", jumpToInputNumber);
            
        }
    }

    //printf("Final JumpToInputNumber: %d", jumpToInputNumber);
    
    truthTable[outputsPointer[0]] = truthTable[inputsPointer[jumpToInputNumber]];
}

// This flips the bit of one of the variables for the truthtable row-to-row.
void flipBitTruthTable (int* truthTable, int numInputs, int numOutputs) {
    for (int i=numInputs+1; i >= 0; i--) {
        truthTable[i] = !truthTable[i];
        if (truthTable[i] == 1) {break;}
    }
}

int main (int argc, char* argv[]) {

    // Reads in the file and checks validity.
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {return EXIT_FAILURE;}

    // Main variables. 
    char** uniqueVariables = NULL; int totalUniqueVariables = 2;

    // Scanning variables. 
    int numInputs; int numOutputs; char directiveName[20];

    // Reads in the input and dynamically allocates space. 
    fscanf(file, "%s %d", directiveName, &numInputs);
    totalUniqueVariables += numInputs;
    uniqueVariables = malloc (sizeof(char*) * totalUniqueVariables);

    // Accounts for the case when the inputs are 0/1.
    uniqueVariables[0] = "0"; uniqueVariables[1] = "1"; 

    // Stores the input variables inside of uniqueVariables.
    for (int i=2; i<totalUniqueVariables; i++) {
        uniqueVariables[i] = malloc(sizeof(char) * 20);
        fscanf(file, "%s", uniqueVariables[i]);
    }

    // Reads in the output and dynamically reallocates space. 
    fscanf(file, "%s %d", directiveName, &numOutputs);
    totalUniqueVariables += numOutputs; 
    uniqueVariables = realloc(uniqueVariables, sizeof(char*) * totalUniqueVariables);

    // Stores the output variables inside of uniqueVariables. 
    for (int i=2+numInputs; i<totalUniqueVariables; i++) {
        uniqueVariables[i] = malloc(sizeof(char) * 20);
        fscanf(file, "%s", uniqueVariables[i]);
    }

    // Debugging Check: Print out the contents of uniqueVariables.
    // printf("\nDebugging - Contents of uniqueVariables: ");
    // printUniqueVariables(uniqueVariables, totalUniqueVariables);

    // Creates a truthTable for printing out the row of each TruthTable.
    int* truthTable = calloc(totalUniqueVariables, sizeof(int));
    truthTable[1] = 1;
    int numRows = pow(2, numInputs); int rowIterator = 0;

    // Relevant variables for combinationCircuit.
    int numLogicGates = 0; gate* combinationCircuit;

    // This will build the combinationCircuit.
    while (!feof(file)){

        // This allocates/reallocates space for the combinationCircuit.
        if (numLogicGates == 0) {
            combinationCircuit = malloc(sizeof(gate));
        }
        else {
            combinationCircuit = realloc(combinationCircuit, sizeof(gate) * (numLogicGates+1));
        }  

        // Sets up parameters of the particular logic gate that we are using.
        combinationCircuit[numLogicGates].decodingNumber = 0;
        combinationCircuit[numLogicGates].selectingNumber = 0;
        combinationCircuit[numLogicGates].inputVariables = NULL;
        combinationCircuit[numLogicGates].outputVariables = NULL;
        combinationCircuit[numLogicGates].selectorVariables = NULL;


        int numDirectiveInputs = 2; int numDirectiveOutputs = 1;
        char charReader[20];

        // Reads in the directive name from the file.
        fscanf(file, "%s", charReader);

        // Determines what type of gate the combinationCircuit[logicGate] is.
        if (strcmp("AND", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 0; 
        }
        else if (strcmp("OR", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 1;
        }
        else if (strcmp("NAND", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 2;
        }
        else if (strcmp("NOR", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 3;
        }
        else if (strcmp("XOR", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 4;
        }
        else if (strcmp("NOT", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 5;
            numDirectiveInputs = 1; 
        }
        else if (strcmp("PASS", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 6;
            numDirectiveInputs = 1;
        }
        else if (strcmp("DECODER", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 7;
            fscanf(file, "%d", &numDirectiveInputs);
            combinationCircuit[numLogicGates].decodingNumber = numDirectiveInputs;
            numDirectiveOutputs = pow(2, numDirectiveInputs);
        }
        else if (strcmp("MULTIPLEXER", charReader) == 0) {
            combinationCircuit[numLogicGates].gateName = 8;
            fscanf(file, "%d", &numDirectiveInputs);
            combinationCircuit[numLogicGates].selectingNumber = numDirectiveInputs;
            numDirectiveInputs = pow(2, numDirectiveInputs);
        }


        // Updates fields for the struct.
        combinationCircuit[numLogicGates].inputNumber = numDirectiveInputs;
        combinationCircuit[numLogicGates].outputNumber = numDirectiveOutputs;

        // Dynamically allocate space for numInputDirectives, numOutputDirectives, numSelectorVariables.
        combinationCircuit[numLogicGates].inputVariables = malloc(sizeof(int) * numDirectiveInputs);
        combinationCircuit[numLogicGates].outputVariables = malloc(sizeof(int) * numDirectiveOutputs);
        combinationCircuit[numLogicGates].selectorVariables = malloc(sizeof(int) * combinationCircuit[numLogicGates].selectingNumber);

        // Store the directiveInputs in combinationCircuit[gateNum] struct.
        for (int i=0; i < numDirectiveInputs; i++) {
            fscanf(file, "%s ", charReader);
            int switchNum = 0;
            for (int j=0; j<totalUniqueVariables; j++) {
                if (strcmp(uniqueVariables[j], charReader) == 0) {
                    combinationCircuit[numLogicGates].inputVariables[i] = j;
                    switchNum = 1;
                    break;  
                }
            }

            // Reallocate space for temporary variables if it is necessary.
            if (switchNum == 0) {
                totalUniqueVariables++;
                truthTable = realloc(truthTable, sizeof(int) * totalUniqueVariables);
                truthTable[totalUniqueVariables-1] = 0;
                uniqueVariables = realloc(uniqueVariables, sizeof(char*) * totalUniqueVariables);
                uniqueVariables[totalUniqueVariables-1] = malloc(sizeof(char) * 20);
                strcpy(uniqueVariables[totalUniqueVariables-1], charReader);
                combinationCircuit[numLogicGates].inputVariables[i] = totalUniqueVariables-1;
            }
        }

        // Store the selectorVariables in combinationCircuit[gateNum] struct.
        for (int i=0; i < combinationCircuit[numLogicGates].selectingNumber; i++) {
            fscanf(file, "%s ", charReader);
            for (int j=0; j<totalUniqueVariables; j++) {
                if (strcmp(uniqueVariables[j], charReader) == 0) {
                    combinationCircuit[numLogicGates].selectorVariables[i] = j;
                    break;  
                }
            }
        }

        // Store the directiveOutputs in combinationCircuit[gateNum] struct. 
        for (int i=0; i < numDirectiveOutputs; i++) {
            fscanf(file, "%s ", charReader);
            int switchNum = 0;
            for (int j=0; j<totalUniqueVariables; j++) {
                if (strcmp(uniqueVariables[j], charReader) == 0) {
                    combinationCircuit[numLogicGates].outputVariables[i] = j;
                    switchNum = 1;
                    break;  
                }
            }
        
            // Reallocate space for temporary variables if it is necessary.
            if (switchNum == 0) {
                totalUniqueVariables++;
                truthTable = realloc(truthTable, sizeof(int) * totalUniqueVariables);
                truthTable[totalUniqueVariables-1] = 0;
                uniqueVariables = realloc(uniqueVariables, sizeof(char*) * totalUniqueVariables);
                uniqueVariables[totalUniqueVariables-1] = malloc(sizeof(char) * 20);
                strcpy(uniqueVariables[totalUniqueVariables-1], charReader);
                combinationCircuit[numLogicGates].outputVariables[i] = totalUniqueVariables-1;
            }
        }
        numLogicGates++;
    }

    // printf("Unique Variables List: "); printUniqueVariables(uniqueVariables, totalUniqueVariables);
    
    /*
    // Debugging Check: Print out the parameters of each logic gate.
    for (int i=0; i < numLogicGates; i++) {
        printf("\nLogic Gate %d:\n", i);
        printf("Logic Gate Type: (%d)\n", combinationCircuit[i].gateName);
        printf("Input Number: %d \tOutput Number: %d\n", combinationCircuit[i].inputNumber, combinationCircuit[i].outputNumber);
        printf("Selecting Number: %d\tDecoding Number: %d\n", combinationCircuit[i].selectingNumber, combinationCircuit[i].decodingNumber);
        printf("Input Variables: "); printIntPointer(combinationCircuit[i].inputVariables, combinationCircuit[i].inputNumber);
        printf("Output Variables: "); printIntPointer(combinationCircuit[i].outputVariables, combinationCircuit[i].outputNumber);
        printf("Selector Variables: "); printIntPointer(combinationCircuit[i].selectorVariables, combinationCircuit[i].selectingNumber);
        printf("Truth Table: "); printIntPointer(truthTable, totalUniqueVariables);
        printf("------------------------------------------------------------------------\n\n");
    } */
    

    // This prints the inputs/outputs for the truthTable.
    while (rowIterator < numRows) {      

        for (int i=0; i < numLogicGates; i++) {
            //printf("\n");
            
            // printf("truth[%d] = truth[%d] && truth[%d] = %d && %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]]);
            // truthTable[combinationCircuit[i].outputVariables[0]] = truthTable[combinationCircuit[i].inputVariables[0]] && truthTable[combinationCircuit[i].inputVariables[1]];
            

            if (combinationCircuit[i].gateName == 0) {
                andLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], combinationCircuit[i].outputVariables[0], truthTable);
                //printf("truth[%d] = truth[%d] AND truth[%d] = %d AND %d = %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]], truthTable[combinationCircuit[i].outputVariables[0]]);
                
            }

            if (combinationCircuit[i].gateName == 1) {
                orLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], combinationCircuit[i].outputVariables[0], truthTable);
                //printf("truth[%d] = truth[%d] OR truth[%d] = %d OR %d = %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]], truthTable[combinationCircuit[i].outputVariables[0]]);
                
            }

            if (combinationCircuit[i].gateName == 2) {
                nandLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], combinationCircuit[i].outputVariables[0], truthTable);
                //printf("truth[%d] = truth[%d] NAND truth[%d] = %d NAND %d = %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]], truthTable[combinationCircuit[i].outputVariables[0]]);
                
            }

            if (combinationCircuit[i].gateName == 3) {
                norLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], combinationCircuit[i].outputVariables[0], truthTable);
                //printf("truth[%d] = truth[%d] NOR truth[%d] = %d NOR %d = %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]], truthTable[combinationCircuit[i].outputVariables[0]]);
            }

            if (combinationCircuit[i].gateName == 4) {
                //printf("truth[%d] = truth[%d] XOR truth[%d] = %d XOR %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]]);
                xorLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], combinationCircuit[i].outputVariables[0], truthTable);
            }
            if (combinationCircuit[i].gateName == 5) {
                //printf("truth[%d] = truth[%d] XOR truth[%d] = %d XOR %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]]);
                notLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].outputVariables[0], truthTable);
            }

            if (combinationCircuit[i].gateName == 6) {
                //printf("truth[%d] = truth[%d] XOR truth[%d] = %d XOR %d\n", combinationCircuit[i].outputVariables[0], combinationCircuit[i].inputVariables[0], combinationCircuit[i].inputVariables[1], truthTable[combinationCircuit[i].inputVariables[0]], truthTable[combinationCircuit[i].inputVariables[1]]);
                passLogicGate(combinationCircuit[i].inputVariables[0], combinationCircuit[i].outputVariables[0], truthTable);
            }

            if (combinationCircuit[i].gateName == 7) {
                decoderLogicGate(combinationCircuit[i].inputVariables, combinationCircuit[i].outputVariables, truthTable, combinationCircuit[i].decodingNumber);
            }

            if (combinationCircuit[i].gateName == 8) {
                multiplexerLogicGate(combinationCircuit[i].inputVariables, combinationCircuit[i].outputVariables, combinationCircuit[i].selectorVariables, truthTable, combinationCircuit[i].selectingNumber);
            }

            //printf("\nTruth Table: "); printIntPointer(truthTable, totalUniqueVariables); printf("\n");
        }

        //printf("\n");

        //printf("\nTruth Table: "); printIntPointer(truthTable, totalUniqueVariables); printf("\n");

        // This prints input side of truthTable.
        printInputSideTruthTable(truthTable, numInputs);
        printf("| ");   

        // This prints output side of truthTable.
        printOutputSideTruthTable(truthTable, numInputs, numOutputs); 
        printf("\n");

        //printf("-------------------------------------------------------------------\n");

        // This resets truthTable for future iterations.
        flipBitTruthTable(truthTable, numInputs, numOutputs);     
        rowIterator++; 
    }

    // Deallocates dynamically allocated space. 
    for (int i=0; i<numLogicGates; i++) {
        free(combinationCircuit[i].inputVariables);
        free(combinationCircuit[i].outputVariables);
        free(combinationCircuit[i].selectorVariables);
    }

    for (int i=2; i<(totalUniqueVariables); i++) {
        free(uniqueVariables[i]);
    }

    free(combinationCircuit);
    free(uniqueVariables);
    free(truthTable);

    return EXIT_SUCCESS; 
}