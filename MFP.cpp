/**
 *
 * Solution to course project # 9
 * Introduction to programming course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2024/2025
 *
 * @author Antonio Donkov
 * @idnumber 0MI0600449
 * @compiler GCC
 *
 * <file with helper functions>
 *
 */

#include <iostream>
#include <vector>
#include <fstream>

int input;
std::string usersFilePath = "users.txt";
std::string usersInfoFilePath = "usersInfo.txt";

struct Consumer {
    std::string typeAccount;
    std::string username;
    std::string password;
    int age;
    std::string gender;
    int height;
    int weight;
    int activityLevel;
    std::string goal;

    Consumer (std::string typeAccount_val,
              std::string username_val,
              std::string password_val,
              int age_val, std::string gender_val,
              int height_val,
              int weight_val,
              int activityLevel_val,
              std::string goal_val) {
        typeAccount = typeAccount_val;
        username = username_val;
        password = password_val;
        age = age_val;
        gender = gender_val;
        height = height_val;
        weight = weight_val;
        activityLevel = activityLevel_val;
        goal = goal_val;
    }
};

/**
 * @brief Function for getting value of field
 *
 * @param line Line with data (usersInfo.txt format)
 * @param fieldType the name of the fiels
 * @return string Value of field
 *
 */
std::string getField (std::string line, std::string fieldType) {
    std::string fromTheFieldType = line.substr(line.find(fieldType) + fieldType.size() + 5);
    return fromTheFieldType.substr(0, fromTheFieldType.find_first_of("\""));
}

/**
 * @brief Function for getting password of user
 *
 * @param username Username to be found
 * @return string Password if user exists and nullptr if not
 *
 */
std::string getPasswordByUsername (std::string username) {
    std::ifstream usersFile(usersFilePath);
    std::string line, searchLine = "\"username\" : \"" + username + "\"";

    while (std::getline(usersFile, line)) {
        if (line.find(searchLine) != std::string::npos) {
            usersFile.close();
            std::string passD = line.substr(line.find("\"password\" : \"") + 14);
            return passD.substr(0, passD.size() - 2);
        }
    }

    usersFile.close();

    return "";
}

/**
 * @brief Function for saving user info
 *
 * @param newConsumer Struct of new consumer to be added
 *
 */
void saveNewUserInfo (Consumer newConsumer) {
    std::ofstream usersInfoFile(usersInfoFilePath, std::ios::app);

    usersInfoFile<<"{\"username\" : \""<<newConsumer.username<<"\","<<
    " \"typeAccount\" : \""<<newConsumer.typeAccount<<"\", "<<
    " \"age\" : \""<<newConsumer.age<<"\", "<<
    " \"gender\" : \""<<newConsumer.gender<<"\", "<<
    " \"height\" : \""<<newConsumer.height<<"\", "<<
    " \"weight\" : \""<<newConsumer.weight<<"\", "<<
    " \"activityLevel\" : \""<<newConsumer.activityLevel<<"\", "<<
    " \"goal\" : \""<<newConsumer.goal<<"\"}";

    usersInfoFile.close();
}

/**
 * @brief Function for adding new consumer
 *
 * @param newConsumer Struct of new consumer to be added
 *
 */
void saveNewUser (Consumer newConsumer) {
    std::ofstream usersFile(usersFilePath, std::ios::app);

    usersFile<<"{\"username\" : \""<<newConsumer.username<<"\", \"password\" : \""<<newConsumer.password<<"\"}\n";
    saveNewUserInfo(newConsumer);

    usersFile.close();
}

/**
 * @brief Function for validating string input by possible options
 *
 * @param possibilities List of possible options
 * @param data String data
 * @return bool If the data is correct
 *
 */
bool validateDataString (std::vector<std::string> possibilities, std::string* dataString) {
    for (int i = 0; i < possibilities.size(); i++) {
        if (possibilities[i] == *dataString) {
            return true;
        }
    }

    std::cout<<"Invalid option! Please enter valid option from the list: ";
    std::cin>>*dataString;
    validateDataString(possibilities, dataString);
}

/**
 * @brief Function for validating int input by max and min value
 *
 * @param min_val Minimal value for the data
 * @param max_val Maximal value for the data
 * @return bool If the data is correct
 *
 */
bool validateDataInt (int min_val, int max_val, int* dataInt) {
    if (*dataInt >= min_val && *dataInt <= max_val) {
        return true;
    }

    std::cout<<"Invalid option! Please enter valid option in a acceptable range: ";
    std::cin>>*dataInt;
    validateDataInt(min_val, max_val, dataInt);
}

/**
 * @brief Function for getting line with info for username
 *
 * @param username Username of consumer
 *
 */
std::string getLineInfoForUsername (std::string username) {
    std::ifstream usersInfoFile(usersInfoFilePath);
    std::string line;

    while (std::getline(usersInfoFile, line)) {
        if (getField(line, "username") == username) {
                return line;
        }
    }

    return "not found";
}

/**
 * @type window
 * @brief Function for singing up a new consumer
 *
 * @param loggedConsumer
 */
void home (Consumer consumer) {
    system("cls");

    std::cout<<"Hello, "<<consumer.username<<"!";
    std::cin>>input;
}

/**
 * @type window
 * @brief Function for singing up a new consumer
 *
 */
void signUp () {
    int age, height, weight, activityLevel;
    std::string gender, goal, username, password, typeAccount;


    std::cout<<"s - Standart (base functionalities)"<<
    "\np - Premium (additional access to macronutrients)"<<
    "\nChoose type of account: ";
    std::cin>>typeAccount;
    validateDataString({"s", "p"}, &typeAccount);
    std::cout<<"Choose username: ";
    std::cin>>username;
    //TO DO - check if its unique
    while (getPasswordByUsername(username) != "") {
        std::cout<<"User with the same name already exists! Try with another one: ";
        std::cin>>username;
    }
    std::cout<<"Choose password: ";
    std::cin>>password;
    while (password.size() == 0) {
        std::cout<<"Password must be atleast 1 character long! Try with another one: ";
        std::cin>>password;
    }
    std::cout<<"Enter your age: ";
    std::cin>>age;
    validateDataInt(0, 120, &age);
    std::cout<<"Enter your gender (m \\ f): ";
    std::cin>>gender;
    validateDataString({"m", "f"}, &gender);
    std::cout<<"Enter your height (cm): ";
    std::cin>>height;
    validateDataInt(0, 300, &height);
    std::cout<<"Enter your weight (kg): ";
    std::cin>>weight;
    validateDataInt(0, 350, &weight);
    std::cout<<"\n1 - Sedentary: little or no exercise"<<
               "\n2 - Light: exercise 1-3 times/week"<<
               "\n3 - Moderate: exercise 4-5 times/week"<<
               "\n4 - Active: daily exercise or intense exercise 3-4 times/week"<<
               "\n5 - Very Active: intense exercise 6-7 times/week"<<
               "\n\nEnter your activity level: ";
    std::cin>>activityLevel;
    validateDataInt(1, 5, &activityLevel);
    std::cout<<"\nd - calorie deficit\nm - maintenance\ns - calorie surplus\n\nEnter your goal: ";
    std::cin>>goal;
    validateDataString({"d", "m", "s"}, &goal);

    Consumer consumer (typeAccount, username, password, age, gender, height, weight, activityLevel, goal);
    saveNewUser(consumer);
}

int stringToInt(std::string stringValue) {
    int intValue = 0, tenHandler = 1;

    for (int i = 0; i < stringValue.size(); i++) {
            tenHandler = 1;
            for (int j = i + 1; j < stringValue.size(); j++) {
                tenHandler *= 10;
            }
        intValue += (int) (stringValue[i] - '0') * tenHandler;
    }

    return intValue;
}

/**
 * @type window
 * @brief Function for logging in an already existing consumer
 *
 */
void logIn () {
    std::string username, password;

    std::cout<<"Enter username: ";
    std::cin>>username;
    while (getPasswordByUsername(username) == "") {
        std::cout<<"Customer does not exist! Try again: ";
        std::cin>>username;
    }
    std::cout<<"Enter password: ";
    std::cin>>password;
    while (password != getPasswordByUsername(username)) {
        std::cout<<"Wrong password! Try again: ";
        std::cin>>password;
    }

    std::string lineInfo = getLineInfoForUsername(username);
    Consumer loggedInConsumer(getField(lineInfo, "typeAccount"),
                     getField(lineInfo, "username"),
                     getField(lineInfo, "password"),
                     stringToInt(getField(lineInfo, "age")),
                     getField(lineInfo, "gender"),
                     stringToInt(getField(lineInfo, "height")),
                     stringToInt(getField(lineInfo, "weight")),
                     stringToInt(getField(lineInfo, "activityLevel")),
                     getField(lineInfo, "goal"));
    home(loggedInConsumer);
}

/**
 * @brief Function for reading an option, chosen from the consumer
 * and validating it, based on the number of possibly chosen
 * options for the given host menu
 *
 * @param numberOfOptions A number of the given options in list
 *
 */
void chooseOption(int numberOfOptions) {

    std::cout<<"Enter your option: ";
    std::cin>>input;

    while (input < 1 || input > numberOfOptions) {
        std::cout<<"Invalid option! Please enter valid option from the list: ";
        std::cin>>input;
    }

    //std::cout<<"\n";
    system("cls");
}

/**
 * @brief Function for displaying a menu of options
 *
 * @param option Description of the chosen option
 *
 */
void manageOptions (std::string option) {

    if (option == "Create new account") {
        signUp();
    }
    else if (option == "Log in to existing account") {
        logIn();
    }
    else {
        std::cout<<"in progress...";
    }
}

/**
 * @brief Function for displaying a menu of options
 *
 * @param options List of options
 *
 */
void displayOptions (std::vector<std::string> options) {

    for (int i = 0; i < options.size(); i++) {
        std::cout<<"\t"<<i + 1<<" - "<<options[i]<<"\n";
    }

    std::cout<<"\n";

    chooseOption(options.size());
    manageOptions(options[input - 1]);
}

/**
 * @type window
 * @brief Function for the starting menu
 *
 */
void helloMenu () {
    std::cout<<"Hello to My Fitness Pal"<<"\n"<<"\n";
    std::cout<<"Choose an option from the menu below:"<<"\n"<<"\n";

    displayOptions({"Create new account", "Log in to existing account"});
}


int main() {
    //std::cout<<getField("{\"username\" : \"antonio\", \"typeAccount\" : \"Standart\", \"age\" : \"19\", \"gender\" : \"male\", \"height\" : \"176\", \"weight\" : \"70\", \"activityLevel\" : \"4\", \"goal\" : \"s\"}", "gender");

    std::cout<<stringToInt("19");
    helloMenu();

    return 0;
}
