#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>

// Simple ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Structure definitions
typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
} FoodItem;

typedef struct {
    int id;
    char name[50];
    FoodItem menu[20];
    int menuSize;
} Cafeteria;

typedef struct {
    int orderId;
    int studentId;
    char studentName[50];
    char studentDept[50];
    char studentSession[20];
    int cafeteriaId;
    int foodItemId;
    int quantity;
    float totalPrice;
    time_t orderTime;
    int isCancelled;
} Order;

typedef struct {
    int id;
    char username[50];
    char password[50];
    char name[50];
    char dept[50];
    char session[20];
    int isAdmin;
} User;

// Global variables
Cafeteria cafeterias[3];
Order orders[100];
int orderCount = 0;
User users[100];
int userCount = 0;
User currentUser;

// Function prototypes
void initializeCafeterias();
void displayMainMenu();
void displayCafeteriaMenu(int cafeteriaId);
void placeOrder();
void viewOrders();
void cancelOrder();
void adminMenu();
void saveDataToFiles();
void loadDataFromFiles();
void clearScreen();
void displayHeader(const char* title);
void displaySuccessMessage(const char* message);
void displayErrorMessage(const char* message);
void displayInfoMessage(const char* message);
void studentRegistration();
void adminRegistration();
bool loginUser(bool isAdmin);
bool isUsernameTaken(const char* username);
bool isStudentIdTaken(int id);
void saveUsersToFile();
void loadUsersFromFile();
void pressAnyKeyToContinue();
void displayAdminDashboard();
void displayStudentDashboard();

int main() {
    initializeCafeterias();
    loadDataFromFiles();

    while(1) {
        clearScreen();
        displayHeader("BUP Cafeteria Management System");

        printf("%s%sMAIN MENU%s\n", BOLD, BLUE, RESET);
        printf("\n1. Student Registration");
        printf("\n2. Student Login");
        printf("\n3. Admin Registration");
        printf("\n4. Admin Login");
        printf("\n5. Exit");

        printf("\n\n%sEnter your choice: %s", BOLD, RESET);

        char choice = getch();
        printf("%c\n", choice);

        switch(choice) {
            case '1':
                studentRegistration();
                break;
            case '2':
                if (loginUser(false)) {
                    displayStudentDashboard();
                }
                break;
            case '3':
                adminRegistration();
                break;
            case '4':
                if (loginUser(true)) {
                    displayAdminDashboard();
                }
                break;
            case '5':
                saveDataToFiles();
                clearScreen();
                printf("%sThank you for using BUP Cafeteria Management System.%s\n", GREEN, RESET);
                exit(0);
            default:
                displayErrorMessage("Invalid choice. Please try again.");
                pressAnyKeyToContinue();
        }
    }
    return 0;
}

void initializeCafeterias() {
    // Vista Cafeteria
    cafeterias[0].id = 1;
    strcpy(cafeterias[0].name, "Vista");
    cafeterias[0].menuSize = 5;
    cafeterias[0].menu[0] = (FoodItem){1, "Chicken Burger", 120.0, 50};
    cafeterias[0].menu[1] = (FoodItem){2, "Beef Burger", 150.0, 40};
    cafeterias[0].menu[2] = (FoodItem){3, "French Fries", 60.0, 100};
    cafeterias[0].menu[3] = (FoodItem){4, "Chicken Nuggets", 80.0, 60};
    cafeterias[0].menu[4] = (FoodItem){5, "Soft Drink", 30.0, 200};

    // Ameti Cafeteria
    cafeterias[1].id = 2;
    strcpy(cafeterias[1].name, "Ameti");
    cafeterias[1].menuSize = 5;
    cafeterias[1].menu[0] = (FoodItem){1, "Chicken Biryani", 130.0, 40};
    cafeterias[1].menu[1] = (FoodItem){2, "Beef Kacchi", 180.0, 30};
    cafeterias[1].menu[2] = (FoodItem){3, "Chicken Tehari", 110.0, 45};
    cafeterias[1].menu[3] = (FoodItem){4, "Morog Polao", 100.0, 50};
    cafeterias[1].menu[4] = (FoodItem){5, "Borhani", 40.0, 100};

    // 3rd Place Cafeteria
    cafeterias[2].id = 3;
    strcpy(cafeterias[2].name, "3rd Place");
    cafeterias[2].menuSize = 5;
    cafeterias[2].menu[0] = (FoodItem){1, "Fried Rice", 90.0, 60};
    cafeterias[2].menu[1] = (FoodItem){2, "Chicken Chili", 100.0, 50};
    cafeterias[2].menu[2] = (FoodItem){3, "Egg Chowmein", 70.0, 70};
    cafeterias[2].menu[3] = (FoodItem){4, "Soup", 50.0, 80};
    cafeterias[2].menu[4] = (FoodItem){5, "Mineral Water", 20.0, 200};
}

void displayStudentDashboard() {
    while(1) {
        clearScreen();
        displayHeader("Student Dashboard");
        printf("%sLogged in as: %s (ID: %d, %s, %s)%s\n", BOLD, currentUser.name, currentUser.id, currentUser.dept, currentUser.session, RESET);

        printf("\n%sSTUDENT MENU%s\n", BOLD, RESET);
        printf("\n1. View Cafeteria Menus");
        printf("\n2. Place Order");
        printf("\n3. View My Orders");
        printf("\n4. Cancel Order");
        printf("\n5. Logout");

        printf("\n\n%sEnter your choice: %s", BOLD, RESET);

        char choice = getch();
        printf("%c\n", choice);

        switch(choice) {
            case '1': {
                clearScreen();
                displayHeader("Available Cafeterias");
                printf("\n1. Vista");
                printf("\n2. Ameti");
                printf("\n3. 3rd Place");
                printf("\n0. Back");

                printf("\n\n%sEnter choice: %s", BOLD, RESET);
                int cafeChoice;
                scanf("%d", &cafeChoice);

                if(cafeChoice >= 1 && cafeChoice <= 3) {
                    clearScreen();
                    char title[50];
                    sprintf(title, "%s Menu", cafeterias[cafeChoice-1].name);
                    displayHeader(title);
                    displayCafeteriaMenu(cafeChoice);
                }
                pressAnyKeyToContinue();
                break;
            }
            case '2':
                clearScreen();
                displayHeader("Place Order");
                placeOrder();
                pressAnyKeyToContinue();
                break;
            case '3':
                clearScreen();
                displayHeader("My Orders");
                viewOrders();
                pressAnyKeyToContinue();
                break;
            case '4':
                clearScreen();
                displayHeader("Cancel Order");
                cancelOrder();
                pressAnyKeyToContinue();
                break;
            case '5':
                return;
            default:
                displayErrorMessage("Invalid choice. Please try again.");
                pressAnyKeyToContinue();
        }
    }
}

void displayAdminDashboard() {
    while(1) {
        clearScreen();
        displayHeader("Admin Dashboard");
        printf("%sLogged in as: %s (Admin ID: %d)%s\n", BOLD, currentUser.name, currentUser.id, RESET);

        printf("\n%sADMIN MENU%s\n", BOLD, RESET);
        printf("\n1. View All Orders");
        printf("\n2. View Cafeteria Stock");
        printf("\n3. Restock Items");
        printf("\n4. Add New Food Item");
        printf("\n5. Manage Users");
        printf("\n6. Logout");

        printf("\n\n%sEnter your choice: %s", BOLD, RESET);

        char choice = getch();
        printf("%c\n", choice);

        switch(choice) {
            case '1': {
                clearScreen();
                displayHeader("All Orders");
                if(orderCount == 0) {
                    displayInfoMessage("No orders found.");
                } else {
                    printf("\n%-10s %-10s %-20s %-15s %-20s %-5s %-10s\n",
                           "OrderID", "StudentID", "Name", "Cafeteria", "Item", "Qty", "Total");
                    printf("---------------------------------------------------------------\n");
                    for(int i = 0; i < orderCount; i++) {
                        char cafeteriaName[50];
                        for(int j = 0; j < 3; j++) {
                            if(cafeterias[j].id == orders[i].cafeteriaId) {
                                strcpy(cafeteriaName, cafeterias[j].name);
                                break;
                            }
                        }

                        char itemName[50];
                        for(int j = 0; j < 3; j++) {
                            if(cafeterias[j].id == orders[i].cafeteriaId) {
                                for(int k = 0; k < cafeterias[j].menuSize; k++) {
                                    if(cafeterias[j].menu[k].id == orders[i].foodItemId) {
                                        strcpy(itemName, cafeterias[j].menu[k].name);
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        printf("%-10d %-10d %-20s %-15s %-20s %-5d %-10.2f\n",
                               orders[i].orderId, orders[i].studentId, orders[i].studentName,
                               cafeteriaName, itemName, orders[i].quantity, orders[i].totalPrice);
                    }
                }
                pressAnyKeyToContinue();
                break;
            }
            case '2': {
                clearScreen();
                displayHeader("Cafeteria Stock Levels");
                for(int i = 0; i < 3; i++) {
                    printf("\n%s%s Cafeteria:%s\n", BOLD, cafeterias[i].name, RESET);
                    printf("\n%-5s %-20s %-10s %-10s\n", "ID", "Item Name", "Price", "Stock");
                    printf("----------------------------------------\n");
                    for(int j = 0; j < cafeterias[i].menuSize; j++) {
                        printf("%-5d %-20s %-10.2f %-10d\n",
                               cafeterias[i].menu[j].id, cafeterias[i].menu[j].name,
                               cafeterias[i].menu[j].price, cafeterias[i].menu[j].stock);
                    }
                }
                pressAnyKeyToContinue();
                break;
            }
            case '3': {
                clearScreen();
                displayHeader("Restock Items");
                printf("\n1. Vista");
                printf("\n2. Ameti");
                printf("\n3. 3rd Place");
                printf("\n0. Back");

                printf("\n\n%sEnter cafeteria choice: %s", BOLD, RESET);
                int cafeId;
                scanf("%d", &cafeId);

                if(cafeId < 1 || cafeId > 3) {
                    displayErrorMessage("Invalid cafeteria.");
                    pressAnyKeyToContinue();
                    break;
                }

                clearScreen();
                char title[50];
                sprintf(title, "%s Menu", cafeterias[cafeId-1].name);
                displayHeader(title);
                displayCafeteriaMenu(cafeId);

                printf("\n%sEnter item ID to restock: %s", BOLD, RESET);
                int itemId;
                scanf("%d", &itemId);

                printf("%sEnter quantity to add: %s", BOLD, RESET);
                int quantity;
                scanf("%d", &quantity);

                if(quantity <= 0) {
                    displayErrorMessage("Quantity must be positive.");
                    pressAnyKeyToContinue();
                    break;
                }

                int found = 0;
                for(int i = 0; i < cafeterias[cafeId-1].menuSize; i++) {
                    if(cafeterias[cafeId-1].menu[i].id == itemId) {
                        cafeterias[cafeId-1].menu[i].stock += quantity;
                        found = 1;

                        clearScreen();
                        displayHeader("Restock Confirmation");
                        printf("%sRestocked successfully!%s\n", GREEN, RESET);
                        printf("\nItem: %s\n", cafeterias[cafeId-1].menu[i].name);
                        printf("Quantity Added: %d\n", quantity);
                        printf("New Stock Level: %d\n", cafeterias[cafeId-1].menu[i].stock);
                        break;
                    }
                }

                if(!found) {
                    displayErrorMessage("Item not found.");
                }
                pressAnyKeyToContinue();
                break;
            }
            case '4': {
                clearScreen();
                displayHeader("Add New Food Item");
                printf("\n1. Vista");
                printf("\n2. Ameti");
                printf("\n3. 3rd Place");
                printf("\n0. Back");

                printf("\n\n%sEnter cafeteria choice: %s", BOLD, RESET);
                int cafeId;
                scanf("%d", &cafeId);

                if(cafeId < 1 || cafeId > 3) {
                    displayErrorMessage("Invalid cafeteria.");
                    pressAnyKeyToContinue();
                    break;
                }

                if(cafeterias[cafeId-1].menuSize >= 20) {
                    displayErrorMessage("Menu is full. Cannot add more items.");
                    pressAnyKeyToContinue();
                    break;
                }

                FoodItem newItem;
                printf("\n%sEnter item name: %s", BOLD, RESET);
                scanf(" %49[^\n]", newItem.name);
                printf("%sEnter item price: %s", BOLD, RESET);
                scanf("%f", &newItem.price);
                printf("%sEnter initial stock: %s", BOLD, RESET);
                scanf("%d", &newItem.stock);

                int maxId = 0;
                for(int i = 0; i < cafeterias[cafeId-1].menuSize; i++) {
                    if(cafeterias[cafeId-1].menu[i].id > maxId) {
                        maxId = cafeterias[cafeId-1].menu[i].id;
                    }
                }
                newItem.id = maxId + 1;

                cafeterias[cafeId-1].menu[cafeterias[cafeId-1].menuSize] = newItem;
                cafeterias[cafeId-1].menuSize++;

                clearScreen();
                displayHeader("Item Added");
                printf("%sItem added successfully!%s\n", GREEN, RESET);
                printf("\nItem ID: %d\n", newItem.id);
                printf("Item Name: %s\n", newItem.name);
                printf("Price: %.2f\n", newItem.price);
                printf("Initial Stock: %d\n", newItem.stock);

                pressAnyKeyToContinue();
                break;
            }
            case '5': {
                clearScreen();
                displayHeader("Manage Users");
                printf("\n1. View All Users");
                printf("\n2. Delete User");
                printf("\n0. Back");

                printf("\n\n%sEnter choice: %s", BOLD, RESET);
                char userChoice = getch();
                printf("%c\n", userChoice);

                if(userChoice == '1') {
                    clearScreen();
                    displayHeader("All Users");
                    if(userCount == 0) {
                        displayInfoMessage("No users found.");
                    } else {
                        printf("\n%-5s %-15s %-20s %-15s %-10s %-10s\n",
                               "ID", "Username", "Name", "Department", "Session", "Role");
                        printf("---------------------------------------------------------------\n");
                        for(int i = 0; i < userCount; i++) {
                            printf("%-5d %-15s %-20s %-15s %-10s %-10s\n",
                                   users[i].id, users[i].username, users[i].name,
                                   users[i].dept, users[i].session,
                                   users[i].isAdmin ? "Admin" : "Student");
                        }
                    }
                    pressAnyKeyToContinue();
                } else if(userChoice == '2') {
                    clearScreen();
                    displayHeader("Delete User");
                    printf("%sEnter user ID to delete: %s", BOLD, RESET);
                    int userId;
                    scanf("%d", &userId);

                    if(userId == currentUser.id) {
                        displayErrorMessage("You cannot delete your own account!");
                        pressAnyKeyToContinue();
                        break;
                    }

                    int found = 0;
                    for(int i = 0; i < userCount; i++) {
                        if(users[i].id == userId) {
                            found = 1;
                            clearScreen();
                            displayHeader("Confirm Deletion");
                            printf("%sAre you sure you want to delete this user?%s\n", RED, RESET);
                            printf("\nID: %d", users[i].id);
                            printf("\nName: %s", users[i].name);
                            printf("\nUsername: %s", users[i].username);
                            printf("\nRole: %s", users[i].isAdmin ? "Admin" : "Student");

                            printf("\n\n%sConfirm deletion (y/n): %s", BOLD, RESET);
                            char confirm = getch();
                            printf("%c\n", confirm);

                            if(confirm == 'y' || confirm == 'Y') {
                                for(int j = i; j < userCount - 1; j++) {
                                    users[j] = users[j + 1];
                                }
                                userCount--;
                                saveUsersToFile();
                                displaySuccessMessage("User deleted successfully.");
                            } else {
                                displayInfoMessage("Deletion cancelled.");
                            }
                            break;
                        }
                    }

                    if(!found) {
                        displayErrorMessage("User not found.");
                    }
                    pressAnyKeyToContinue();
                }
                break;
            }
            case '6':
                return;
            default:
                displayErrorMessage("Invalid choice. Please try again.");
                pressAnyKeyToContinue();
        }
    }
}

void displayCafeteriaMenu(int cafeteriaId) {
    printf("\n%-5s %-20s %-10s %-10s\n", "ID", "Item Name", "Price", "Stock");
    printf("----------------------------------------\n");
    for(int i = 0; i < cafeterias[cafeteriaId-1].menuSize; i++) {
        printf("%-5d %-20s %-10.2f %-10d\n",
               cafeterias[cafeteriaId-1].menu[i].id,
               cafeterias[cafeteriaId-1].menu[i].name,
               cafeterias[cafeteriaId-1].menu[i].price,
               cafeterias[cafeteriaId-1].menu[i].stock);
    }
}

void placeOrder() {
    printf("\nAvailable Cafeterias\n");
    printf("\n1. Vista");
    printf("\n2. Ameti");
    printf("\n3. 3rd Place");
    printf("\n0. Cancel");

    printf("\n\n%sSelect cafeteria: %s", BOLD, RESET);
    int cafeteriaChoice;
    scanf("%d", &cafeteriaChoice);

    if(cafeteriaChoice < 1 || cafeteriaChoice > 3) {
        displayInfoMessage("Order cancelled.");
        return;
    }

    clearScreen();
    char title[50];
    sprintf(title, "%s Menu", cafeterias[cafeteriaChoice-1].name);
    displayHeader(title);
    displayCafeteriaMenu(cafeteriaChoice);

    printf("\n%sEnter food item ID: %s", BOLD, RESET);
    int itemChoice;
    scanf("%d", &itemChoice);

    int validItem = 0;
    FoodItem selectedItem;

    for(int i = 0; i < cafeterias[cafeteriaChoice-1].menuSize; i++) {
        if(cafeterias[cafeteriaChoice-1].menu[i].id == itemChoice) {
            validItem = 1;
            selectedItem = cafeterias[cafeteriaChoice-1].menu[i];
            break;
        }
    }

    if(!validItem) {
        displayErrorMessage("Invalid item ID.");
        return;
    }

    printf("%sEnter quantity: %s", BOLD, RESET);
    int quantity;
    scanf("%d", &quantity);

    if(quantity <= 0) {
        displayErrorMessage("Quantity must be positive.");
        return;
    }

    if(quantity > selectedItem.stock) {
        printf("%sNot enough stock available. Only %d available.%s\n", RED, selectedItem.stock, RESET);
        return;
    }

    Order newOrder;
    newOrder.orderId = orderCount + 1;
    newOrder.studentId = currentUser.id;
    strcpy(newOrder.studentName, currentUser.name);
    strcpy(newOrder.studentDept, currentUser.dept);
    strcpy(newOrder.studentSession, currentUser.session);
    newOrder.cafeteriaId = cafeteriaChoice;
    newOrder.foodItemId = itemChoice;
    newOrder.quantity = quantity;
    newOrder.totalPrice = selectedItem.price * quantity;
    newOrder.orderTime = time(NULL);
    newOrder.isCancelled = 0;

    orders[orderCount] = newOrder;
    orderCount++;

    for(int i = 0; i < cafeterias[cafeteriaChoice-1].menuSize; i++) {
        if(cafeterias[cafeteriaChoice-1].menu[i].id == itemChoice) {
            cafeterias[cafeteriaChoice-1].menu[i].stock -= quantity;
            break;
        }
    }

    clearScreen();
    displayHeader("Order Confirmation");
    printf("%sOrder placed successfully!%s\n", GREEN, RESET);
    printf("\n%sOrder Details:%s\n", BOLD, RESET);
    printf("----------------------------\n");
    printf("Order ID: %d\n", newOrder.orderId);
    printf("Item: %s\n", selectedItem.name);
    printf("Quantity: %d\n", quantity);
    printf("Total Price: %s%.2f%s\n", GREEN, newOrder.totalPrice, RESET);
    printf("Order Time: %s", ctime(&newOrder.orderTime));
}

void viewOrders() {
    printf("\n%sYour Orders:%s\n", BOLD, RESET);
    int found = 0;

    printf("\n%-10s %-15s %-20s %-10s %-10s %-20s\n",
           "OrderID", "Cafeteria", "Item", "Qty", "Total", "Time");
    printf("---------------------------------------------------------------\n");

    for(int i = 0; i < orderCount; i++) {
        if(orders[i].studentId == currentUser.id && !orders[i].isCancelled) {
            found = 1;
            char cafeteriaName[50];
            for(int j = 0; j < 3; j++) {
                if(cafeterias[j].id == orders[i].cafeteriaId) {
                    strcpy(cafeteriaName, cafeterias[j].name);
                    break;
                }
            }

            char itemName[50];
            for(int j = 0; j < 3; j++) {
                if(cafeterias[j].id == orders[i].cafeteriaId) {
                    for(int k = 0; k < cafeterias[j].menuSize; k++) {
                        if(cafeterias[j].menu[k].id == orders[i].foodItemId) {
                            strcpy(itemName, cafeterias[j].menu[k].name);
                            break;
                        }
                    }
                    break;
                }
            }

            char timeStr[20];
            strncpy(timeStr, ctime(&orders[i].orderTime), 16);
            timeStr[16] = '\0';

            printf("%-10d %-15s %-20s %-10d %-10.2f %-20s\n",
                   orders[i].orderId, cafeteriaName, itemName,
                   orders[i].quantity, orders[i].totalPrice, timeStr);
        }
    }

    if(!found) {
        displayInfoMessage("No orders found.");
    }
}

void cancelOrder() {
    printf("\n%sYour Active Orders:%s\n", BOLD, RESET);
    int found = 0;

    printf("\n%-10s %-15s %-20s %-10s %-10s %-20s\n",
           "OrderID", "Cafeteria", "Item", "Qty", "Total", "Time");
    printf("---------------------------------------------------------------\n");

    for(int i = 0; i < orderCount; i++) {
        if(orders[i].studentId == currentUser.id && !orders[i].isCancelled) {
            found = 1;
            char cafeteriaName[50];
            for(int j = 0; j < 3; j++) {
                if(cafeterias[j].id == orders[i].cafeteriaId) {
                    strcpy(cafeteriaName, cafeterias[j].name);
                    break;
                }
            }

            char itemName[50];
            for(int j = 0; j < 3; j++) {
                if(cafeterias[j].id == orders[i].cafeteriaId) {
                    for(int k = 0; k < cafeterias[j].menuSize; k++) {
                        if(cafeterias[j].menu[k].id == orders[i].foodItemId) {
                            strcpy(itemName, cafeterias[j].menu[k].name);
                            break;
                        }
                    }
                    break;
                }
            }

            char timeStr[20];
            strncpy(timeStr, ctime(&orders[i].orderTime), 16);
            timeStr[16] = '\0';

            printf("%-10d %-15s %-20s %-10d %-10.2f %-20s\n",
                   orders[i].orderId, cafeteriaName, itemName,
                   orders[i].quantity, orders[i].totalPrice, timeStr);
        }
    }

    if(!found) {
        displayInfoMessage("No orders found to cancel.");
        return;
    }

    printf("\n%sEnter Order ID to cancel (0 to go back): %s", BOLD, RESET);
    int orderId;
    scanf("%d", &orderId);

    if(orderId == 0) {
        return;
    }

    int orderIndex = -1;
    for(int i = 0; i < orderCount; i++) {
        if(orders[i].orderId == orderId && orders[i].studentId == currentUser.id && !orders[i].isCancelled) {
            orderIndex = i;
            break;
        }
    }

    if(orderIndex == -1) {
        displayErrorMessage("Invalid Order ID or order already cancelled.");
        return;
    }

    for(int i = 0; i < cafeterias[orders[orderIndex].cafeteriaId-1].menuSize; i++) {
        if(cafeterias[orders[orderIndex].cafeteriaId-1].menu[i].id == orders[orderIndex].foodItemId) {
            cafeterias[orders[orderIndex].cafeteriaId-1].menu[i].stock += orders[orderIndex].quantity;
            break;
        }
    }

    orders[orderIndex].isCancelled = 1;
    displaySuccessMessage("Order cancelled successfully.");
}

void studentRegistration() {
    clearScreen();
    displayHeader("Student Registration");

    User newUser;
    newUser.isAdmin = 0;

    printf("%sEnter your student ID: %s", BOLD, RESET);
    scanf("%d", &newUser.id);

    if(isStudentIdTaken(newUser.id)) {
        displayErrorMessage("This student ID is already registered.");
        pressAnyKeyToContinue();
        return;
    }

    printf("%sEnter your full name: %s", BOLD, RESET);
    scanf(" %49[^\n]", newUser.name);

    printf("%sEnter your department: %s", BOLD, RESET);
    scanf(" %49[^\n]", newUser.dept);

    printf("%sEnter your session (e.g., 2021-22): %s", BOLD, RESET);
    scanf(" %19[^\n]", newUser.session);

    char username[50];
    do {
        printf("%sEnter username: %s", BOLD, RESET);
        scanf("%49s", username);

        if(isUsernameTaken(username)) {
            displayErrorMessage("Username already taken. Please choose another.");
        }
    } while(isUsernameTaken(username));

    strcpy(newUser.username, username);

    printf("%sEnter password: %s", BOLD, RESET);
    char password[50];
    scanf("%49s", password);
    strcpy(newUser.password, password);

    users[userCount] = newUser;
    userCount++;
    saveUsersToFile();

    displaySuccessMessage("\nRegistration successful!");
    printf("\nStudent ID: %d", newUser.id);
    printf("\nName: %s", newUser.name);
    printf("\nDepartment: %s", newUser.dept);
    printf("\nSession: %s", newUser.session);
    printf("\nUsername: %s", newUser.username);

    pressAnyKeyToContinue();
}

void adminRegistration() {
    clearScreen();
    displayHeader("Admin Registration");

    User newUser;
    newUser.isAdmin = 1;

    printf("%sEnter admin ID: %s", BOLD, RESET);
    scanf("%d", &newUser.id);

    if(isStudentIdTaken(newUser.id)) {
        displayErrorMessage("This ID is already registered.");
        pressAnyKeyToContinue();
        return;
    }

    printf("%sEnter your full name: %s", BOLD, RESET);
    scanf(" %49[^\n]", newUser.name);

    strcpy(newUser.dept, "Administration");
    strcpy(newUser.session, "N/A");

    char username[50];
    do {
        printf("%sEnter username: %s", BOLD, RESET);
        scanf("%49s", username);

        if(isUsernameTaken(username)) {
            displayErrorMessage("Username already taken. Please choose another.");
        }
    } while(isUsernameTaken(username));

    strcpy(newUser.username, username);

    printf("%sEnter password: %s", BOLD, RESET);
    char password[50];
    scanf("%49s", password);
    strcpy(newUser.password, password);

    users[userCount] = newUser;
    userCount++;
    saveUsersToFile();

    displaySuccessMessage("\nAdmin registration successful!");
    printf("\nAdmin ID: %d", newUser.id);
    printf("\nName: %s", newUser.name);
    printf("\nUsername: %s", newUser.username);

    pressAnyKeyToContinue();
}

bool loginUser(bool isAdmin) {
    clearScreen();
    displayHeader(isAdmin ? "Admin Login" : "Student Login");

    char identifier[50];
    char password[50];

    printf("%sEnter %s ID or username: %s", BOLD, isAdmin ? "admin" : "student", RESET);
    scanf("%49s", identifier);

    printf("%sEnter password: %s", BOLD, RESET);
    scanf("%49s", password);

    for(int i = 0; i < userCount; i++) {
        int id;
        if(sscanf(identifier, "%d", &id) == 1) {
            if(users[i].id == id && strcmp(users[i].password, password) == 0 && users[i].isAdmin == isAdmin) {
                currentUser = users[i];
                displaySuccessMessage("\nLogin successful!");
                printf("Welcome, %s!\n", users[i].name);
                pressAnyKeyToContinue();
                return true;
            }
        }

        if(strcmp(users[i].username, identifier) == 0 && strcmp(users[i].password, password) == 0 && users[i].isAdmin == isAdmin) {
            currentUser = users[i];
            displaySuccessMessage("\nLogin successful!");
            printf("Welcome, %s!\n", users[i].name);
            pressAnyKeyToContinue();
            return true;
        }
    }

    displayErrorMessage("\nInvalid credentials. Please try again.");
    pressAnyKeyToContinue();
    return false;
}

bool isUsernameTaken(const char* username) {
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0) {
            return true;
        }
    }
    return false;
}

bool isStudentIdTaken(int id) {
    for(int i = 0; i < userCount; i++) {
        if(users[i].id == id) {
            return true;
        }
    }
    return false;
}

void saveDataToFiles() {
    FILE *file;

    file = fopen("orders.dat", "wb");
    if(file != NULL) {
        fwrite(&orderCount, sizeof(int), 1, file);
        fwrite(orders, sizeof(Order), orderCount, file);
        fclose(file);
    }

    file = fopen("cafeterias.dat", "wb");
    if(file != NULL) {
        for(int i = 0; i < 3; i++) {
            fwrite(&cafeterias[i], sizeof(Cafeteria), 1, file);
        }
        fclose(file);
    }

    saveUsersToFile();
}

void loadDataFromFiles() {
    FILE *file;

    file = fopen("orders.dat", "rb");
    if(file != NULL) {
        fread(&orderCount, sizeof(int), 1, file);
        fread(orders, sizeof(Order), orderCount, file);
        fclose(file);
    }

    file = fopen("cafeterias.dat", "rb");
    if(file != NULL) {
        for(int i = 0; i < 3; i++) {
            fread(&cafeterias[i], sizeof(Cafeteria), 1, file);
        }
        fclose(file);
    }

    loadUsersFromFile();
}

void saveUsersToFile() {
    FILE *file = fopen("users.dat", "wb");
    if(file != NULL) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void loadUsersFromFile() {
    FILE *file = fopen("users.dat", "rb");
    if(file != NULL) {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void clearScreen() {
    system("cls || clear");
}

void displayHeader(const char* title) {
    printf("\n%s%s%s\n", BLUE, title, RESET);
    printf("----------------------------------------\n");
}

void displaySuccessMessage(const char* message) {
    printf("%s✓ %s%s\n", GREEN, message, RESET);
}

void displayErrorMessage(const char* message) {
    printf("%s✗ %s%s\n", RED, message, RESET);
}

void displayInfoMessage(const char* message) {
    printf("%sℹ %s%s\n", CYAN, message, RESET);
}

void pressAnyKeyToContinue() {
    printf("%s\nPress any key to continue...%s", YELLOW, RESET);
    getch();
}
