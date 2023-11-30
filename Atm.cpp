#include <iostream>
#include <vector>

class Transaction {
public:
    enum Type { WITHDRAW, DEPOSIT, TRANSFER };
    
    Transaction(Type type, double amount) : type(type), amount(amount) {}
    
    Type getType() const {
        return type;
    }
    
    double getAmount() const {
        return amount;
    }
    
    std::string getTypeString() const {
        switch (type) {
            case WITHDRAW:
                return "Withdraw";
            case DEPOSIT:
                return "Deposit";
            case TRANSFER:
                return "Transfer";
            default:
                return "Unknown";
        }
    }
    
private:
    Type type;
    double amount;
};

class Account {
public:
    Account(int id, int pin) : id(id), pin(pin), balance(0.0) {}
    
    int getId() const {
        return id;
    }
    
    int getPin() const {
        return pin;
    }
    
    double getBalance() const {
        return balance;
    }
    
    void setBalance(double balance) {
        this->balance = balance;
    }
    
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }
    
    const std::vector<Transaction>& getTransactions() const {
        return transactions;
    }
    
private:
    int id;
    int pin;
    double balance;
    std::vector<Transaction> transactions;
};

class ATM {
public:
    ATM(const std::vector<Account>& accounts) : accounts(accounts) {}
    
    void start() {
        int id, pin;
        
        std::cout << "Enter user ID: ";
        std::cin >> id;
        
        std::cout << "Enter PIN: ";
        std::cin >> pin;
        
        bool validCredentials = false;
        Account* currentAccount = nullptr;
        
        for (Account& account : accounts) {
            if (account.getId() == id && account.getPin() == pin) {
                validCredentials = true;
                currentAccount = &account;
                break;
            }
        }
        
        if (validCredentials) {
            std::cout << "Welcome to the ATM!" << std::endl;
            
            while (true) {
                std::cout << std::endl;
                displayMenu();
                
                int choice;
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        withdraw(currentAccount);
                        break;
                    case 2:
                        deposit(currentAccount);
                        break;
                    case 3:
                        transfer(currentAccount);
                        break;
                    case 4:
                        displayTransactionHistory(currentAccount);
                        break;
                    case 5:
                        std::cout << "Thank you for using the ATM. Goodbye!" << std::endl;
                        return;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        } else {
            std::cout << "Invalid user ID or PIN. Exiting..." << std::endl;
        }
    }
    
private:
    std::vector<Account> accounts;
    
    void displayMenu() const {
        std::cout << "ATM INTERFACE" << std::endl;
        std::cout << "1. Withdraw" << std::endl;
        std::cout << "2. Deposit" << std::endl;
        std::cout << "3. Transfer" << std::endl;
        std::cout << "4. Transaction History" << std::endl;
        std::cout << "5. Quit" << std::endl;
        std::cout << "Enter your choice: ";
    }
    
    void withdraw(Account* account) {
        double amount;
        std::cout << "Enter amount to withdraw: ";
        std::cin >> amount;
        
        if (amount > 0 && amount <= account->getBalance()) {
            account->setBalance(account->getBalance() - amount);
            account->addTransaction(Transaction(Transaction::WITHDRAW, amount));
            std::cout << "Withdrawal success!" << std::endl;
        } else {
            std::cout << "Invalid amount. Withdrawal failed." << std::endl;
        }
    }
    
    void deposit(Account* account) {
        double amount;
        std::cout << "Enter amount to deposit: ";
        std::cin >> amount;
        
        if (amount > 0) {
            account->setBalance(account->getBalance() + amount);
            account->addTransaction(Transaction(Transaction::DEPOSIT, amount));
            std::cout << "Deposit success!" << std::endl;
        } else {
            std::cout << "Invalid amount. Deposit failed." << std::endl;
        }
    }
    
    void transfer(Account* account) {
        int recipientId;
        double amount;
        
        std::cout << "Enter recipient ID: ";
        std::cin >> recipientId;
        
        std::cout << "Enter amount to transfer: ";
        std::cin >> amount;
        
        if (amount > 0 && amount <= account->getBalance()) {
            // Find recipient account
            Account* recipientAccount = nullptr;
            
            for (Account& account : accounts) {
                if (account.getId() == recipientId) {
                    recipientAccount = &account;
                    break;
                }
            }
            
            if (recipientAccount != nullptr) {
                account->setBalance(account->getBalance() - amount);
                recipientAccount->setBalance(recipientAccount->getBalance() + amount);
                
                account->addTransaction(Transaction(Transaction::TRANSFER, -amount));
                recipientAccount->addTransaction(Transaction(Transaction::TRANSFER, amount));
                
                std::cout << "Transfer success!" << std::endl;
            } else {
                std::cout << "Recipient account not found. Transfer failed." << std::endl;
            }
        } else {
            std::cout << "Invalid amount. Transfer failed." << std::endl;
        }
    }
    
    void displayTransactionHistory(Account* account) const {
        std::cout << "TRANSACTION HISTORY" << std::endl;
        
        const std::vector<Transaction>& transactions = account->getTransactions();
        
        for (const Transaction& transaction : transactions) {
            std::cout << transaction.getTypeString() << " - " << transaction.getAmount() << std::endl;
        }
    }
};

int main() {
    std::vector<Account> accounts = {
        Account(123456, 1234),
        Account(987654, 4321)
    };
    
    ATM atm(accounts);
    atm.start();
    
    return 0;
}