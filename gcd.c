#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h> 


// Functions
int isValidInteger(const char *str);
int stringToLongLong(const char *str, long long *result);
long long calculateDivisors(long long n, long long **divisors);
long long calculateCommonDivisors(long long *div1, long long count1, long long *div2, long long count2, long long **common);
long long gcdEuclidean(long long a, long long b);
void printTable(long long num1, long long num2, long long *div1, long long count1, long long *div2, long long count2, long long *common, long long commonCount, long long gcd);
void freeMemory(long long *div1, long long *div2, long long *common);
void banner();


void banner(){
    printf("                   .___    ___                      ____                           ________   ___    \n");
    printf("   ____   ____   __| _/   /  /   ____  __ __  _____/_   |      ____  __ __  _____  \\_____  \\  \\  \\   \n");
    printf("  / ___\\_/ ___\\ / __ |   /  /   /    \\|  |  \\/     \\|   |     /    \\|  |  \\/     \\  /  ____/   \\  \\  \n");
    printf(" / /_/  >  \\___/ /_/ |  (  (   |   |  \\  |  /  Y Y  \\   |    |   |  \\  |  /  Y Y  \\/       \\    )  ) \n");
    printf(" \\___  / \\___  >____ |   \\  \\  |___|  /____/|__|_|  /___| /\\ |___|  /____/|__|_|  /\\_______ \\  /  /  \n");
    printf("/_____/      \\/     \\/    \\__\\      \\/            \\/      )/      \\/            \\/         \\/ /__/   \n");
    printf("\n");
}

int main(){
    // declare
    char input[256];
    char *comma_position;
    char first_num_str[128];
    char second_num_str[128];
    long long num1, num2;

    banner();
    printf("[!] Enter two numbers seperated by a comma (eg: 12,33) >");

    // Read inpt 
    if (fgets(input, sizeof(input), stdin) == NULL){
        printf("\n[!] Failed to read input ..");
        return 1;
    }

    // Remove newline char 
    size_t len = strlen(input);

    if (len > 0 && input[len-1] == '\n'){
        input[len-1] = '\0';
        len--;
    }

    // check if input is empty 
    if (len=0){
        printf("\n[!] Input is empty");
    }

    // check for comma chr (1)
    comma_position = strchr(input, ',');
    if (comma_position == NULL){
        printf("\n[!]No comma found in input");
        return 1;
    }

    // check for commas chrs (1+)
    if (strchr(comma_position +1, ',') !=NULL){
        printf("\n[!] Only enter one comma in the middle of the numbers please");
        return 1;
    }

    // Extract first number 
    size_t first_len = comma_position - input;
    if (first_len == 0){
        printf("\n[!] You didn't enter the first number");
        return 1;
    }

    // Too big , should probably change long long 
    if (first_len >= sizeof(first_num_str)){
        printf("\n[!] First number you entered is too long");
        return 1;
    }

    strncpy(first_num_str, input, first_len);
    first_num_str[first_len] = '\0';

    // validate first number
        for (size_t i = 0; i < first_len; i++) {
        if (!isdigit((unsigned char)first_num_str[i]) && 
            first_num_str[i] != '+' && 
            first_num_str[i] != '-') {
            printf("\n[!] First number contains invalid character.");
            return 1;
        }
    }

    // check if input is a valid integer by using isValidInteger function
    if (!isValidInteger(first_num_str)) {
        printf("\n[!] First number is not a valid integer.");
        return 1;
    }

    // extract second number 
    const char *second_start = comma_position + 1;
    size_t second_len = strlen(second_start);
    
    if (second_len == 0) {
        printf("\n[!]Second number is missing.");
        return 1;
    }
    
    if (second_len >= sizeof(second_num_str)) {
        printf("\n[!] Second number is too long.");
        return 1;
    }
    
    strcpy(second_num_str, second_start);

    // validate second number 
        for (size_t i = 0; i < second_len; i++) {
        if (!isdigit((unsigned char)second_num_str[i]) && 
            second_num_str[i] != '+' && 
            second_num_str[i] != '-') {
            printf("\n[!]Second number contains invalid character.");
            return 1;
        }
    }
    if (!isValidInteger(second_num_str)) {
        printf("\n[!]Second number is not a valid integer.");
        return 1;
    }

    // convert to numbers 
    if (!stringToLongLong(first_num_str, &num1)) {
        printf("\n[!]First number is out of range.");
        return 1;
    }
    
    if (!stringToLongLong(second_num_str, &num2)) {
        printf("\n[!] Second number is out of range.");
        return 1;
    }

    //take absolute values for div calc
    long long abs_num1 = llabs(num1);
    long long abs_num2 = llabs(num2);
    

    printf("\nMETHOD 1: ASCII INTERFACE\n");
    printf("==========================================\n");

    long long *divisors1 = NULL, *divisors2 = NULL, *commonDivisors = NULL;
    long long count1 = 0, count2 = 0, commonCount = 0;
    
    if (abs_num1 == 0 && abs_num2 == 0) {
        printf("Note: Both numbers are 0. GCD(0,0) is undefined.\n");
    } else {
        // Get divisors
        count1 = calculateDivisors(abs_num1, &divisors1);
        count2 = calculateDivisors(abs_num2, &divisors2);
        
        // gcd 
        commonCount = calculateCommonDivisors(divisors1, count1, divisors2, count2, &commonDivisors);
        long long gcd = gcdEuclidean(num1, num2);
        printTable(num1, num2, divisors1, count1, divisors2, count2, commonDivisors, commonCount, gcd);
    }


    // Method 2: Euclidean Algorithm
    printf("\n\nMETHOD 2: EUCLIDEAN ALGORITHM\n");
    printf("==========================================\n");
    
    long long gcd = gcdEuclidean(num1, num2);
    printf("GCD(%lld, %lld) = %lld\n", num1, num2, gcd);
    
    // show calc steps 
    printf("\nSteps:\n");
    printf("------------------------------------------\n");

    long long a = llabs(num1);
    long long b = llabs(num2);

    if (a == 0 && b == 0){
        printf("Both numbers are 0. GCD is undefined \n");
    
    } 

    else if (a==0){
        printf("GCD(0, %lld) = %lld\n", b ,b);
    }

    else if (b==0){
        printf("GCD(0, %lld) = %lld\n", a ,a);
    }

    else {
        printf("1. Start with a = %lld, b = %lld\n", a, b);
        int step = 2;
        while (b != 0) {
            long long remainder = a % b;
            printf("%d. %lld %% %lld = %lld\n", step, a, b, remainder);
            printf("   Now a = %lld, b = %lld\n", b, remainder);
            a = b;
            b = remainder;
            step++;
        }
        printf("\nFinal result: GCD = %lld\n", a);
    }

    // show reduced fraction 
    if (gcd != 0 && (num1 != 0 || num2 != 0)) {
        printf("\nReduced fraction: %lld/%lld\n", num1 / gcd, num2 / gcd);
        if (gcd == 1 || gcd == -1) {
            printf("The numbers are COPRIME (relatively prime).\n");
        }   
    }
    printf("==========================================\n");
    
    //free mem 
    freeMemory(divisors1, divisors2, commonDivisors);
    return 0;
}


// valid integer function 
int isValidInteger(const char * str){
    if (str == NULL || *str == '\0'){
        return 0;
    }

    int i = 0;
    if (str[i] == '+' || str[i] == '-'){
        i++;
    }

    if (str[i] == '\0'){
        return 0;
    }

    while (str[i] != '\0'){
        if (!isdigit((unsigned char)str[i])){
            return 0;
        }
        i++;
    }
    return 1;
}


// funcion to convert string to long long w overflow checking
int stringToLongLong(const char *str, long long *result) {
    char *endptr;
    long long val;
    errno = 0;
    val = strtoll(str, &endptr, 10);
    
    if (errno == ERANGE) {
        return 0;
    }
    if (*endptr != '\0') {
        return 0;
    }

    *result = val;
    return 1;
}


// calc all divisors of a num 
long long calculateDivisors(long long n, long long **divisors){
    if (n==0){
        *divisors = (long long*)malloc(sizeof(long long));
        if (*divisors) {
            **divisors = 0;
            return 1;
        }
        return 0;
    }

    // count divisors 
    long long count = 0;
    for (long long i = 1; i * i <= n; i++ ){
        if (n % i == 0){
            count++;
            if (i != n/i){
                count++;
            }
        }
    }

    // alloc mem for divisors 
    *divisors = (long long*)malloc(count * sizeof(long long));
    if (*divisors == NULL) {
        return 0;
    }

    // fill divisors
    long long index = 0;
    for (long long i = 1; i*i <= n; i++){
        if (n % i == 0){
            (*divisors)[index++] = i;
            if (i !=n/i){
                (*divisors)[index++] = n/1;
            }
        }
    }

    // sort divisors with bubble sort
        for (long long i = 0; i < count - 1; i++) {
        for (long long j = 0; j < count - i - 1; j++) {
            if ((*divisors)[j] > (*divisors)[j + 1]) {
                long long temp = (*divisors)[j];
                (*divisors)[j] = (*divisors)[j + 1];
                (*divisors)[j + 1] = temp;
            }
        }
    }
    
    return count;

}

// function to calc common divisors 
long long calculateCommonDivisors(long long *div1, long long count1, long long *div2, long long count2, long long **common) {
    if (div1 == NULL || div2 == NULL) {
        *common = NULL;
        return 0;
    }
    
    // count common divisors 
    long long commonCount = 0;
    for (long long i = 0; i < count1; i++){
        for (long long j=0; j < count2; j++){
            if (div1[i] == div2[j]){
                commonCount++;
                break;
            }
        }
    }

    // alloc mem
    *common = (long long*)malloc(commonCount * sizeof(long long));
    if (*common == NULL) {
        return 0;
    }

    // fill common divisors 
    long long index = 0;
    for (long long i = 0; i < count1; i++) {
        for (long long j = 0; j < count2; j++) {
            if (div1[i] == div2[j]) {
                (*common)[index++] = div1[i];
                break;
            }
        }
    }
    return commonCount;

}


// calculate gcd using euclidean algorithm 
long long gcdEuclidean(long long a, long long b){
    a = llabs(a);
    b = llabs(b);

    if (a == 0 && b == 0){
        return 0;
    }
    if (a ==0) return b;
    if (b==0) return a;
    
    while (b !=0 ){
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;

}

// ascii table for gcd 
void printTable(long long num1, long long num2, long long *div1, long long count1, long long *div2, long long count2, long long *common, long long commonCount, long long gcd) {
    printf("+----------------+----------------------+----------------------+\n");
    printf("|                | %-20lld | %-20lld |\n", num1, num2);
    printf("+----------------+----------------------+----------------------+\n");
    printf("| Divisors       | ");

    if (count1 >0){
        for (long long i=0; i<count1; i++){
            printf("%lld", div1[i]);
            if (i < count1 - 1) printf(", ");
        }
    } 
    else {
        printf("0");
    }
    printf(" | ");

    if (count2 > 0){
        for (long long i=0; i<count2; i++){
            printf("%lld", div2[i]);
            if (i < count2 -1) printf(", ");
        
        }
        
    }
    else{
        printf("0");
    }

    printf(" |\n");
    printf("+----------------+----------------------+----------------------+\n");

    // print common divisors
    printf("| Common Divisors| ");
        if (commonCount > 0) {
        for (long long i = 0; i < commonCount; i++) {
            printf("%lld", common[i]);
            if (i < commonCount - 1) printf(", ");
        }
    } else {
        printf("None");
    }
    printf(" |                      |\n");
    printf("+----------------+----------------------+----------------------+\n");
    printf("| GCD            | %-20lld |                      |\n", gcd);
    printf("+----------------+----------------------+----------------------+\n");
}


// free alloc mem
void freeMemory(long long *div1, long long *div2, long long *common) {
    if (div1) free(div1);
    if (div2) free(div2);
    if (common) free(common);
}
