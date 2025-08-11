#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define FILENAME "bank_data.txt"

struct Account {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    float balance;
};

struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function prototypes
void mainMenu();
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void saveToFile();
void loadFromFile();
int generateAccountNumber();
int findAccount(int accountNumber);

int main() {
    loadFromFile();
    printf("\n=== BANKING MANAGEMENT SYSTEM ===\n");
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    
    while(1) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display All Accounts\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: displayAllAccounts(); break;
            case 6:
                saveToFile();
                printf("Thank you for using Banking System!\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

void createAccount() {
    if(accountCount >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached!\n");
        return;
    }
    
    printf("\n=== CREATE ACCOUNT ===\n");
    
    struct Account newAccount;
    newAccount.accountNumber = generateAccountNumber();
    
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Enter Name: ");
    getchar(); // Clear buffer
    fgets(newAccount.name, MAX_NAME_LENGTH, stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = 0; // Remove newline
    
    printf("Initial Deposit: TK ");
    scanf("%f", &newAccount.balance);
    
    if(newAccount.balance < 0) {
        printf("Initial deposit cannot be negative!\n");
        return;
    }
    
    accounts[accountCount] = newAccount;
    accountCount++;
    
    printf("\nSavings Account created successfully!\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
    printf("Name: %s\n", newAccount.name);
    printf("Balance: TK %.2f\n", newAccount.balance);
    
    saveToFile();
}

void depositMoney() {
    int accountNumber;
    float amount;
    int index;
    
    printf("\n=== DEPOSIT MONEY ===\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: TK %.2f\n", accounts[index].balance);
    printf("Enter Deposit Amount: TK ");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("Amount must be positive!\n");
        return;
    }
    
    accounts[index].balance += amount;
    printf("Deposit successful! New Balance: TK %.2f\n", accounts[index].balance);
    saveToFile();
}

void withdrawMoney() {
    int accountNumber;
    float amount;
    int index;
    
    printf("\n=== WITHDRAW MONEY ===\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: TK %.2f\n", accounts[index].balance);
    printf("Enter Withdrawal Amount: TK ");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("Amount must be positive!\n");
        return;
    }
    
    if(amount > accounts[index].balance) {
        printf("Insufficient balance! Available: TK %.2f\n", accounts[index].balance);
        return;
    }
    
    accounts[index].balance -= amount;
    printf("Withdrawal successful! Remaining Balance: TK %.2f\n", accounts[index].balance);
    saveToFile();
}

void checkBalance() {
    int accountNumber;
    int index;
    
    printf("\n=== CHECK BALANCE ===\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    index = findAccount(accountNumber);
    if(index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("\n=== ACCOUNT DETAILS ===\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Name: %s\n", accounts[index].name);
    printf("Account Type: Savings\n");
    printf("Balance: TK %.2f\n", accounts[index].balance);
}

void displayAllAccounts() {
    printf("\n=== ALL ACCOUNTS ===\n");
    
    if(accountCount == 0) {
        printf("No accounts found!\n");
        return;
    }
    
    printf("%-10s %-20s %s\n", "Acc No.", "Name", "Balance");
    printf("----------------------------------------\n");
    
    for(int i = 0; i < accountCount; i++) {
        printf("%-10d %-20s TK %.2f\n", 
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].balance);
    }
    
    printf("\nTotal Accounts: %d\n", accountCount);
}

void saveToFile() {
    FILE *file = fopen(FILENAME, "w");
    if(file == NULL) {
        printf("Error saving to file!\n");
        return;
    }
    
    fprintf(file, "%d\n", accountCount);
    
    for(int i = 0; i < accountCount; i++) {
        fprintf(file, "%d\n%s\n%.2f\n",
                accounts[i].accountNumber,
                accounts[i].name,
                accounts[i].balance);
    }
    
    fclose(file);
}

int generateAccountNumber() {
    int maxAccountNumber = 1000;
    for(int i = 0; i < accountCount; i++) {
        if(accounts[i].accountNumber > maxAccountNumber) {
            maxAccountNumber = accounts[i].accountNumber;
        }
    }
    return maxAccountNumber + 1;
}

void loadFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if(file == NULL) {
        return; // File doesn't exist yet
    }
    
    fscanf(file, "%d", &accountCount);
    
    for(int i = 0; i < accountCount; i++) {
        fscanf(file, "%d", &accounts[i].accountNumber);
        fgetc(file); // consume newline
        fgets(accounts[i].name, MAX_NAME_LENGTH, file);
        accounts[i].name[strcspn(accounts[i].name, "\n")] = 0;
        fscanf(file, "%f", &accounts[i].balance);
    }
    
    fclose(file);
}

int findAccount(int accountNumber) {
    for(int i = 0; i < accountCount; i++) {
        if(accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}