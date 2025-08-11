#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME "bank_data.txt"

// Structure to store account information
struct Account {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char accountType[20];
    float balance;
    char dateCreated[20];
};

// Global variables
struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function prototypes
void mainMenu();
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void searchAccount();
void deleteAccount();
void modifyAccount();
void saveToFile();
void loadFromFile();
void getCurrentDate(char *date);
int generateAccountNumber();
int findAccount(int accountNumber);
void clearScreen();
void pressEnterToContinue();

int main() {
    loadFromFile();
    
    printf("\n\t\t=== WELCOME TO BANKING MANAGEMENT SYSTEM ===\n");
    printf("\t\t\t    Developed in C\n");
    printf("\t\t==========================================\n");
    
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    
    while(1) {
        clearScreen();
        printf("\n\t\t=== BANKING MANAGEMENT SYSTEM ===\n");
        printf("\t\t==================================\n");
        printf("\n\t\t1. Create New Account");
        printf("\n\t\t2. Deposit Money");
        printf("\n\t\t3. Withdraw Money");
        printf("\n\t\t4. Check Balance");
        printf("\n\t\t5. Display All Accounts");
        printf("\n\t\t6. Search Account");
        printf("\n\t\t7. Modify Account");
        printf("\n\t\t8. Delete Account");
        printf("\n\t\t9. Exit");
        printf("\n\t\t==================================\n");
        printf("\n\t\tEnter your choice (1-9): ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                displayAllAccounts();
                break;
            case 6:
                searchAccount();
                break;
            case 7:
                modifyAccount();
                break;
            case 8:
                deleteAccount();
                break;
            case 9:
                saveToFile();
                printf("\n\t\tThank you for using Banking Management System!");
                printf("\n\t\tGoodbye!\n\n");
                exit(0);
            default:
                printf("\n\t\tInvalid choice! Please try again.");
                pressEnterToContinue();
        }
    }
}

void createAccount() {
    clearScreen();
    
    if(accountCount >= MAX_ACCOUNTS) {
        printf("\n\t\tMaximum account limit reached!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n\t\t=== CREATE NEW ACCOUNT ===\n");
    printf("\t\t===========================\n");
    
    struct Account newAccount;
    
    // Generate account number
    newAccount.accountNumber = generateAccountNumber();
    
    printf("\n\t\tAccount Number: %d", newAccount.accountNumber);
    
    // Get account holder name
    printf("\n\t\tEnter Account Holder Name: ");
    getchar(); // Clear buffer
    fgets(newAccount.name, MAX_NAME_LENGTH, stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = 0; // Remove newline
    
    // Get address
    printf("\t\tEnter Address: ");
    fgets(newAccount.address, MAX_ADDRESS_LENGTH, stdin);
    newAccount.address[strcspn(newAccount.address, "\n")] = 0;
    
    // Get account type
    printf("\t\tEnter Account Type (Savings/Current): ");
    fgets(newAccount.accountType, 20, stdin);
    newAccount.accountType[strcspn(newAccount.accountType, "\n")] = 0;
    
    // Get initial deposit
    printf("\t\tEnter Initial Deposit Amount: $");
    scanf("%f", &newAccount.balance);
    
    if(newAccount.balance < 0) {
        printf("\n\t\tInitial deposit cannot be negative!");
        pressEnterToContinue();
        return;
    }
    
    // Get current date
    getCurrentDate(newAccount.dateCreated);
    
    // Add account to array
    accounts[accountCount] = newAccount;
    accountCount++;
    
    printf("\n\t\t=== ACCOUNT CREATED SUCCESSFULLY ===");
    printf("\n\t\tAccount Number: %d", newAccount.accountNumber);
    printf("\n\t\tAccount Holder: %s", newAccount.name);
    printf("\n\t\tAccount Type: %s", newAccount.accountType);
    printf("\n\t\tInitial Balance: $%.2f", newAccount.balance);
    printf("\n\t\tDate Created: %s", newAccount.dateCreated);
    
    saveToFile();
    pressEnterToContinue();
}

void depositMoney() {
    clearScreen();
    int accountNumber;
    float amount;
    int index;
    
    printf("\n\t\t=== DEPOSIT MONEY ===\n");
    printf("\t\t====================\n");
    
    printf("\n\t\tEnter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    
    if(index == -1) {
        printf("\n\t\tAccount not found!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n\t\tAccount Holder: %s", accounts[index].name);
    printf("\n\t\tCurrent Balance: $%.2f", accounts[index].balance);
    
    printf("\n\t\tEnter Amount to Deposit: $");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\n\t\tInvalid amount! Amount must be positive.");
        pressEnterToContinue();
        return;
    }
    
    accounts[index].balance += amount;
    
    printf("\n\t\t=== DEPOSIT SUCCESSFUL ===");
    printf("\n\t\tAmount Deposited: $%.2f", amount);
    printf("\n\t\tNew Balance: $%.2f", accounts[index].balance);
    
    saveToFile();
    pressEnterToContinue();
}

void withdrawMoney() {
    clearScreen();
    int accountNumber;
    float amount;
    int index;
    
    printf("\n\t\t=== WITHDRAW MONEY ===\n");
    printf("\t\t=====================\n");
    
    printf("\n\t\tEnter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    
    if(index == -1) {
        printf("\n\t\tAccount not found!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n\t\tAccount Holder: %s", accounts[index].name);
    printf("\n\t\tCurrent Balance: $%.2f", accounts[index].balance);
    
    printf("\n\t\tEnter Amount to Withdraw: $");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\n\t\tInvalid amount! Amount must be positive.");
        pressEnterToContinue();
        return;
    }
    
    if(amount > accounts[index].balance) {
        printf("\n\t\tInsufficient balance!");
        printf("\n\t\tAvailable Balance: $%.2f", accounts[index].balance);
        pressEnterToContinue();
        return;
    }
    
    accounts[index].balance -= amount;
    
    printf("\n\t\t=== WITHDRAWAL SUCCESSFUL ===");
    printf("\n\t\tAmount Withdrawn: $%.2f", amount);
    printf("\n\t\tRemaining Balance: $%.2f", accounts[index].balance);
    
    saveToFile();
    pressEnterToContinue();
}

void checkBalance() {
    clearScreen();
    int accountNumber;
    int index;
    
    printf("\n\t\t=== CHECK BALANCE ===\n");
    printf("\t\t====================\n");
    
    printf("\n\t\tEnter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    
    if(index == -1) {
        printf("\n\t\tAccount not found!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n\t\t=== ACCOUNT DETAILS ===");
    printf("\n\t\tAccount Number: %d", accounts[index].accountNumber);
    printf("\n\t\tAccount Holder: %s", accounts[index].name);
    printf("\n\t\tAccount Type: %s", accounts[index].accountType);
    printf("\n\t\tCurrent Balance: $%.2f", accounts[index].balance);
    printf("\n\t\tDate Created: %s", accounts[index].dateCreated);
    
    pressEnterToContinue();
}

void displayAllAccounts() {
    clearScreen();
    
    printf("\n\t\t=== ALL ACCOUNTS ===\n");
    printf("\t\t===================\n");
    
    if(accountCount == 0) {
        printf("\n\t\tNo accounts found!");
        pressEnterToContinue();
        return;
    }
    
    printf("\n%-12s %-20s %-15s %-12s %s\n", "Acc No.", "Name", "Type", "Balance", "Date Created");
    printf("------------------------------------------------------------------------\n");
    
    for(int i = 0; i < accountCount; i++) {
        printf("%-12d %-20s %-15s $%-11.2f %s\n", 
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].accountType,
               accounts[i].balance,
               accounts[i].dateCreated);
    }
    
    printf("\n\t\tTotal Accounts: %d", accountCount);
    pressEnterToContinue();
}