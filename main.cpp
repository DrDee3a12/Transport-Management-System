#include <iostream>
#include <mysql.h>
#include <string>
#include <algorithm>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using namespace std;

void addVehicle(MYSQL* conn) {
    string number, type;

    cout << "\nEnter vehicle number: ";
    cin >> number;
    cout << "Enter vehicle type (e.g. truck, van): ";
    cin >> type;

    string query = "INSERT INTO vehicles (vehicle_number, vehicle_type) VALUES ('" + number + "', '" + type + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Vehicle added successfully.\n";
    } else {
        cout << "Failed to add vehicle: " << mysql_error(conn) << endl;
    }
}

void deleteVehicle(MYSQL* conn) {
    int vehicleId;
    cout << "\nEnter Vehicle ID to delete: ";
    cin >> vehicleId;

    // Check if the vehicle is assigned to any order
    string checkQuery = "SELECT COUNT(*) FROM orders WHERE vehicle_id = " + to_string(vehicleId);
    if (mysql_query(conn, checkQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row && atoi(row[0]) > 0) {
            cout << "? Cannot delete vehicle. It is assigned to an order.\n";
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);
    }

    string query = "DELETE FROM vehicles WHERE vehicle_id = " + to_string(vehicleId);
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "? Vehicle deleted successfully.\n";
    } else {
        cout << "? Failed to delete vehicle: " << mysql_error(conn) << endl;
    }
}

void viewAllVehicles(MYSQL* conn) {
    string query = "SELECT vehicle_id, vehicle_number, vehicle_type, status FROM vehicles";

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- All Vehicles ---\n";
        cout << "ID\tNumber\t\tType\t\tStatus\n";

        while ((row = mysql_fetch_row(res))) {
            cout << row[0] << "\t" << row[1] << "\t\t" << row[2] << "\t\t" << row[3] << "\n";
        }

        mysql_free_result(res);
    } else {
        cout << "? Failed to fetch vehicles: " << mysql_error(conn) << endl;
    }
}


void addDriver(MYSQL* conn) {
    string name, license, phone;

    cout << "\nEnter driver name: ";
    cin.ignore(); // clear newline before getline
    getline(cin, name);
    cout << "Enter license number: ";
    cin >> license;
    cout << "Enter phone number: ";
    cin >> phone;

    string query = "INSERT INTO drivers (name, license_number, phone) VALUES ('" + name + "', '" + license + "', '" + phone + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Driver added successfully.\n";
    } else {
        cout << "Failed to add driver: " << mysql_error(conn) << endl;
    }
}

void deleteDriver(MYSQL* conn) {
    int driverId;
    cout << "\nEnter Driver ID to delete: ";
    cin >> driverId;

    // Check if the driver is assigned to any order
    string checkQuery = "SELECT COUNT(*) FROM orders WHERE driver_id = " + to_string(driverId);
    if (mysql_query(conn, checkQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row && atoi(row[0]) > 0) {
            cout << "? Cannot delete driver. They are assigned to an order.\n";
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);
    }

    string query = "DELETE FROM drivers WHERE driver_id = " + to_string(driverId);
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "? Driver deleted successfully.\n";
    } else {
        cout << "? Failed to delete driver: " << mysql_error(conn) << endl;
    }
}

void viewAllDrivers(MYSQL* conn) {
    string query = "SELECT driver_id, name, license_number, phone FROM drivers";

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- All Drivers ---\n";
        cout << "ID\tName\t\tLicense\t\tPhone\n";

        while ((row = mysql_fetch_row(res))) {
            cout << row[0] << "\t" << row[1] << "\t\t" << row[2] << "\t\t" << row[3] << "\n";
        }

        mysql_free_result(res);
    } else {
        cout << "? Failed to fetch drivers: " << mysql_error(conn) << endl;
    }
}

void viewSpecificDriver(MYSQL* conn) {
    string input;
    cout << "\nEnter driver ID or Name: ";
    cin.ignore();
    getline(cin, input);

    string query;
    if (all_of(input.begin(), input.end(), ::isdigit)) {
        query = "SELECT * FROM drivers WHERE driver_id = " + input;
    } else {
        query = "SELECT * FROM drivers WHERE name LIKE '%" + input + "%'";
    }

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        if (mysql_num_rows(res) > 0) {
            cout << "\n--- Driver Info ---\n";
            while ((row = mysql_fetch_row(res))) {
                cout << "ID: " << row[0] << "\nName: " << row[1]
                     << "\nLicense: " << row[2] << "\nPhone: " << row[3] << "\n\n";
            }
        } else {
            cout << "? No driver found with the given input.\n";
        }

        mysql_free_result(res);
    } else {
        cout << "? Query failed: " << mysql_error(conn) << endl;
    }
}


void createOrder(MYSQL* conn) {
    string customer, destination, cargo;

    cout << "\nEnter customer name: ";
    cin.ignore(); // to flush newline
    getline(cin, customer);
    cout << "Enter destination: ";
    getline(cin, destination);
    cout << "Enter cargo details: ";
    getline(cin, cargo);

    string query = "INSERT INTO orders (customer_name, destination, cargo_details) VALUES ('" +
                   customer + "', '" + destination + "', '" + cargo + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Order created successfully.\n";
    } else {
        cout << "Failed to create order: " << mysql_error(conn) << endl;
    }
}

void viewOrders(MYSQL* conn) {
    cout << "\n--- All Orders ---\n";

    string query =
    "SELECT o.order_id, o.customer_name, o.destination, o.cargo_details, "
    "d.name AS driver_name, v.vehicle_number AS vehicle_number "
    "FROM orders o "
    "LEFT JOIN drivers d ON o.driver_id = d.driver_id "
    "LEFT JOIN vehicles v ON o.vehicle_id = v.vehicle_id";


    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res))) {
            cout << "Order ID: " << row[0]
                 << "\nCustomer: " << row[1]
                 << "\nDestination: " << row[2]
                 << "\nCargo: " << row[3]
                 << "\nDriver: " << (row[4] ? row[4] : (char*)"Not Assigned")
                 << "\nVehicle: " << (row[5] ? row[5] : (char*)"Not Assigned")
                 << "\n--------------------------\n";
        }
        mysql_free_result(res);
    } else {
        cout << "Failed to fetch orders: " << mysql_error(conn) << endl;
    }
}

void viewAvailableResources(MYSQL* conn) {
    cout << "\n--- Available Vehicles ---\n";
    if (mysql_query(conn, "SELECT vehicle_id, vehicle_number, vehicle_type FROM vehicles WHERE status = 'available'") == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << " | Number: " << row[1] << " | Type: " << row[2] << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "Failed to fetch vehicles: " << mysql_error(conn) << endl;
    }

    cout << "\n--- Available Drivers ---\n";
    if (mysql_query(conn, "SELECT driver_id, name, license_number FROM drivers WHERE driver_id NOT IN (SELECT driver_id FROM orders WHERE driver_id IS NOT NULL)") == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << " | Name: " << row[1] << " | License: " << row[2] << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "Failed to fetch drivers: " << mysql_error(conn) << endl;
    }
}

void assignDriverVehicleToOrder(MYSQL* conn) {
    int orderId, driverId, vehicleId;

    cout << "\n--- Assign Driver & Vehicle to Order ---\n";

    // Show orders without assigned driver or vehicle
    string orderQuery = "SELECT order_id, customer_name FROM orders WHERE driver_id IS NULL OR vehicle_id IS NULL";
    if (mysql_query(conn, orderQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        if (mysql_num_rows(res) == 0) {
            cout << "All orders already have drivers and vehicles assigned.\n";
            mysql_free_result(res);
            return;
        }

        cout << "Unassigned Orders:\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "Order ID: " << row[0] << " | Customer: " << row[1] << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "Failed to fetch orders: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Enter Order ID to assign: ";
    cin >> orderId;

    // Show available drivers
    string driverQuery = "SELECT driver_id, name FROM drivers WHERE driver_id NOT IN (SELECT driver_id FROM orders WHERE driver_id IS NOT NULL)";
    if (mysql_query(conn, driverQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "Available Drivers:\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "Driver ID: " << row[0] << " | Name: " << row[1] << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "? Failed to fetch drivers: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Enter Driver ID to assign: ";
    cin >> driverId;

    // Show available vehicles
    string vehicleQuery = "SELECT vehicle_id, vehicle_number FROM vehicles WHERE status = 'available'";
    if (mysql_query(conn, vehicleQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "Available Vehicles:\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "Vehicle ID: " << row[0] << " | Number: " << row[1] << endl;
        }
        mysql_free_result(res);
    } else {
        cout << "Failed to fetch vehicles: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Enter Vehicle ID to assign: ";
    cin >> vehicleId;

    // Update the order
    string updateQuery = "UPDATE orders SET driver_id = " + to_string(driverId) +
                         ", vehicle_id = " + to_string(vehicleId) +
                         " WHERE order_id = " + to_string(orderId);

    if (mysql_query(conn, updateQuery.c_str()) == 0) {
        // Mark vehicle as assigned
        string updateVehicle = "UPDATE vehicles SET status = 'assigned' WHERE vehicle_id = " + to_string(vehicleId);
        mysql_query(conn, updateVehicle.c_str());

        cout << "Driver and vehicle assigned successfully to order.\n";
    } else {
        cout << "Failed to assign: " << mysql_error(conn) << endl;
    }
}

void viewSpecificVehicle(MYSQL* conn) {
    string input;
    cout << "Enter vehicle number or ID: ";
    cin >> input;

    string query;

    // Check if the input is a number (assume it's an ID)
    if (all_of(input.begin(), input.end(), ::isdigit)) {
        query = "SELECT * FROM vehicles WHERE vehicle_id = " + input;
    } else {
        query = "SELECT * FROM vehicles WHERE vehicle_number = '" + input + "'";
    }

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            MYSQL_ROW row;
            cout << "\nVehicle Details:\n";
            cout << "ID\tNumber\t\tType\t\tStatus\n";
            while ((row = mysql_fetch_row(res))) {
                cout << row[0] << "\t" << row[1] << "\t\t" << row[2] << "\t\t" << row[3] << "\n";
            }
        } else {
            cout << "No vehicle found with given input.\n";
        }
        mysql_free_result(res);
    } else {
        cout << "Query failed: " << mysql_error(conn) << endl;
    }
}

void deleteOrder(MYSQL* conn) {
    int orderId;
    cout << "\nEnter Order ID to delete: ";
    cin >> orderId;

    // 1. Fetch associated driver_id and vehicle_id
    string fetchQuery = "SELECT driver_id, vehicle_id FROM orders WHERE order_id = " + to_string(orderId);
    if (mysql_query(conn, fetchQuery.c_str()) != 0) {
        cout << "? Failed to fetch order: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "? Order not found.\n";
        if (res) mysql_free_result(res);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    string driverId = row[0] ? row[0] : "NULL";
    string vehicleId = row[1] ? row[1] : "NULL";
    mysql_free_result(res);

    // 2. Set vehicle status back to 'available' (if assigned)
    if (vehicleId != "NULL") {
        string updateVehicle = "UPDATE vehicles SET status = 'available' WHERE vehicle_id = " + vehicleId;
        mysql_query(conn, updateVehicle.c_str());
    }

    // 3. Delete the order
    string deleteQuery = "DELETE FROM orders WHERE order_id = " + to_string(orderId);
    if (mysql_query(conn, deleteQuery.c_str()) == 0) {
        cout << "? Order deleted successfully.\n";
    } else {
        cout << "? Failed to delete order: " << mysql_error(conn) << endl;
    }
}

void aboutProgram() {
    system("clear");
    cout << CYAN << "????????????????????????????????????\n";
    cout << "?         About This Program       ?\n";
    cout << "????????????????????????????????????\n" << RESET;
    cout << BOLD << "Project: " << RESET << "Transport Management System\n";
    cout << BOLD << "Developer: " << RESET << "Muhammad Zalan Khan\n";
    cout << BOLD << "Other Team Members: " << RESET << "Huzaifa Khan and Tawqeer Zia\n";
    cout << BOLD << "Version: " << RESET << "1.0\n";
    cout << BOLD << "Language: " << RESET << "C++ (MySQL Integration)\n";
    cout << BOLD << "Platform: " << RESET << "Linux and Windows\n";
    cout << BOLD << "Description: " << RESET << "This system manages vehicles, drivers, and orders for a textile company's transport department.\n\n";
    cout << YELLOW << "Press Enter to return..." << RESET;
    cin.ignore(); cin.get();
}


void createAdmin(MYSQL* conn) {
    string newUser, newPass, newRole;
    cout << "\nEnter new username: ";
    cin >> newUser;
    cout << "Enter new password: ";
    cin >> newPass;
    cout << "Role (admin/superadmin): ";
    cin >> newRole;

    if (newRole != "admin" && newRole != "superadmin") {
        cout << RED << "? Invalid role. Must be 'admin' or 'superadmin'.\n" << RESET;
        return;
    }

    string query = "INSERT INTO admin (username, password, role) VALUES ('" +
                   newUser + "', '" + newPass + "', '" + newRole + "')";

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << GREEN << "? New admin user created.\n" << RESET;
    } else {
        cout << RED << "? Failed to create user: " << mysql_error(conn) << endl << RESET;
    }
}


int main() {
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "tms", 3306, NULL, 0);

    if (!conn) {
        cout << RED << "? Connection failed: " << mysql_error(conn) << RESET << endl;
        return 1;
    }

    // Login Loop
    string username, password;
    bool loggedIn = false;
    string role;  // add this line above while loop
while (!loggedIn) {
    system("clear");
    cout << CYAN << "????????????????????????????????\n";
    cout << "?      Admin Login Panel       ?\n";
    cout << "????????????????????????????????" << RESET << endl;

    cout << BOLD << "Username: " << RESET;
    cin >> username;
    cout << BOLD << "Password: " << RESET;
    cin >> password;

    string query = "SELECT role FROM admin WHERE username='" + username + "' AND password='" + password + "'";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            MYSQL_ROW row = mysql_fetch_row(res);
            role = row[0]; // get role (admin or superadmin)
            cout << GREEN << "? Login successful. Welcome, " << username << "!\n" << RESET;
            loggedIn = true;
        } else {
            cout << RED << "? Invalid username or password.\n" << RESET;
        }
        mysql_free_result(res);
    } else {
        cout << RED << "? Query error: " << mysql_error(conn) << RESET << endl;
        return 1;
    }

    cout << YELLOW << "Press Enter to continue..." << RESET;
    cin.ignore(); cin.get();
}


    // Main menu
    int mainChoice;
    do {
        system("clear");
        cout << CYAN << "???????????????????????????????????????\n";
        cout << "?      Transport Management System    ?\n";
        cout << "???????????????????????????????????????\n" << RESET;
        cout << BOLD << "1." << RESET << " Manage Vehicles\n";
        cout << BOLD << "2." << RESET << " Manage Drivers\n";
        cout << BOLD << "3." << RESET << " Manage Orders\n";
        cout << BOLD << "4." << RESET << " View Available Resources\n";
        cout << BOLD << "5." << RESET << " About the Program\n";
        if (role == "superadmin") {
    cout << BOLD << "6." << RESET << " Create Admin Account\n";
}
cout << BOLD << "7." << RESET << " Exit\n";
        cout << YELLOW << "Enter choice: " << RESET;
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                int vChoice;
                do {
                    system("clear");
                    cout << BLUE << "????????????????????????\n";
                    cout << "?   Vehicle Management  ?\n";
                    cout << "????????????????????????\n" << RESET;
                    cout << BOLD << "1." << RESET << " Add Vehicle\n";
					cout << BOLD << "2." << RESET << " View All Vehicles\n";
					cout << BOLD << "3." << RESET << " View Specific Vehicle\n";
					cout << BOLD << "4." << RESET << " Delete Vehicle\n";
					cout << BOLD << "5." << RESET << " Back\n";

                    cout << YELLOW << "Enter choice: " << RESET;
                    cin >> vChoice;

                    switch (vChoice) {
						case 1: addVehicle(conn); break;
						case 2: viewAllVehicles(conn); break;
						case 3: viewSpecificVehicle(conn); break;
						case 4: deleteVehicle(conn); break;
						case 5: break;
						default: cout << RED << "Invalid choice.\n" << RESET;
					}



                    if (vChoice != 5) {
                        cout << YELLOW << "Press Enter to return..." << RESET;
                        cin.ignore(); cin.get();
                    }

                } while (vChoice != 5);
                break;
            }

            case 2: {
                int dChoice;
                do {
                    system("clear");
                    cout << MAGENTA << "????????????????????????\n";
                    cout << "?   Driver Management   ?\n";
                    cout << "????????????????????????\n" << RESET;
                    cout << BOLD << "1." << RESET << " Add Driver\n";
					cout << BOLD << "2." << RESET << " View All Drivers\n";
					cout << BOLD << "3." << RESET << " Search Specific Driver\n";
					cout << BOLD << "4." << RESET << " Delete Driver\n";
					cout << BOLD << "5." << RESET << " Back\n";

                    cout << YELLOW << "Enter choice: " << RESET;
                    cin >> dChoice;

                    switch (dChoice) {
						case 1: addDriver(conn); break;
						case 2: viewAllDrivers(conn); break;
						case 3: viewSpecificDriver(conn); break;
						case 4: deleteDriver(conn); break;
						case 5: break;
						default: cout << RED << "Invalid choice.\n" << RESET;
					}




                    if (dChoice != 5) {
                        cout << YELLOW << "Press Enter to return..." << RESET;
                        cin.ignore(); cin.get();
                    }

                } while (dChoice != 5);
                break;
            }

            case 3: {
                int oChoice;
                do {
                    system("clear");
                    cout << GREEN << "????????????????????????\n";
                    cout << "?   Order Management    ?\n";
                    cout << "????????????????????????\n" << RESET;
                    cout << BOLD << "1." << RESET << " Create Order\n";
                    cout << BOLD << "2." << RESET << " View Orders\n";
                    cout << BOLD << "3." << RESET << " Delete Order\n";
                    cout << BOLD << "4." << RESET << " Assign Vehicle & Driver\n";
                    cout << BOLD << "5." << RESET << " Back\n";
                    cout << YELLOW << "Enter choice: " << RESET;
                    cin >> oChoice;

                    switch (oChoice) {
                        case 1: createOrder(conn); break;
                        case 2: viewOrders(conn); break;
                        case 3: deleteOrder(conn); break;
                        case 4: assignDriverVehicleToOrder(conn); break;
                        case 5: break;
                        default: cout << RED << "Invalid choice.\n" << RESET;
                    }

                    if (oChoice != 5) {
                        cout << YELLOW << "Press Enter to return..." << RESET;
                        cin.ignore(); cin.get();
                    }

                } while (oChoice != 5);
                break;
            }

            case 4:
                viewAvailableResources(conn);
                cout << YELLOW << "Press Enter to return..." << RESET;
                cin.ignore(); cin.get();
                break;

            case 5:
				aboutProgram();
				break;
				case 6:
    if (role == "superadmin") {
        createAdmin(conn);
    } else {
        cout << RED << "Unauthorized option.\n" << RESET;
    }
    cout << YELLOW << "Press Enter to return..." << RESET;
    cin.ignore(); cin.get();
    break;

			case 7:
				cout << GREEN << "Exiting program. Goodbye!\n" << RESET;
				break;


            default:
                cout << RED << "Invalid choice.\n" << RESET;
                cout << YELLOW << "Press Enter to continue..." << RESET;
                cin.ignore(); cin.get();
        }

    } while (mainChoice != 7);

    mysql_close(conn);
    return 0;
}

