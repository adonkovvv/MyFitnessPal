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
#include <ctime>

int input;
std::string usersFilePath = "users.txt";
std::string usersInfoFilePath = "usersInfo.txt";
std::string mealInfoPath = "mealInfo.txt";
std::time_t t = std::time(nullptr);
std::tm* localTime = std::localtime(&t);

struct Consumer;
void updateDailyCalories (Consumer consumer);
std::string todaysDate ();
std::string getLineInfoForUsername (std::string username);
void setField (std::string line, std::string fieldType, std::string newValue);
void showHistory (Consumer consumer);
std::string getField (std::string line, std::string fieldType);
void displayOptions (std::vector<std::string> options, Consumer* consumer);
void deleteHistoryForDate (Consumer consumer);
void home(Consumer consumer);
std::string todaysDate ();
int getNumberOfRowsWithMeals ();
void helloMenu();

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
    int dailyCalories;
    int eatenCalories;
    int burntCalories;

    Consumer (std::string typeAccount_val,
              std::string username_val,
              std::string password_val,
              int age_val, std::string gender_val,
              int height_val,
              int weight_val,
              int activityLevel_val,
              std::string goal_val,
              int dailyCalories_val = 0,
              int eatenCalories_val = 0,
              int burntCalories_val = 0) {
        typeAccount = typeAccount_val;
        username = username_val;
        password = password_val;
        age = age_val;
        gender = gender_val;
        height = height_val;
        weight = weight_val;
        activityLevel = activityLevel_val;
        goal = goal_val;
        dailyCalories = dailyCalories_val;
        eatenCalories = eatenCalories_val;
        burntCalories = burntCalories_val;

        if (dailyCalories_val == 0) {
            setDailyCalories();
        }
    }

    void setDailyCalories () {
        int dailyCalories_prev = dailyCalories;
        double bmr;
        if (gender == "m") {
                bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
        }
        else {
            bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
        }

        switch (activityLevel) {
            case 1: bmr *= 1.2;
                break;
            case 2: bmr *= 1.375;
                break;
            case 3: bmr *= 1.55;
                break;
            case 4: bmr *= 1.725;
                break;
            case 5: bmr *= 1.9;
                break;
        }

        if (goal == "d") {
            dailyCalories = bmr - 500;
        }
        else if (goal == "m") {
            dailyCalories = bmr;
        }
        else {
            dailyCalories = bmr + 500;
        }

        updateDailyCalories(*this);
        if (dailyCalories_prev != 0) {
            setField(getLineInfoForUsername(username), "dailyCalories", std::to_string(dailyCalories));
        }
    }

};

/**
 * @brief Function for updating daily calories of consumer
 *
 * @param consumer
 *
 */
void updateDailyCalories (Consumer consumer) {
    std::string lineFromFile, content = "";
    std::ifstream mealInfo(mealInfoPath);

    while (std::getline(mealInfo, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            lineFromFile.find(todaysDate()) != std::string::npos) {
            lineFromFile = lineFromFile.substr(0, lineFromFile.find("\"dailyCalories\"") + 19) + std::to_string(consumer.dailyCalories) +
            lineFromFile.substr(lineFromFile.find("\"dailyCalories\"") + 19 + std::to_string(consumer.dailyCalories).size(), lineFromFile.size());
        }
        content += lineFromFile + "\n";
    }

    mealInfo.close();


    std::ofstream mealInfoNew(mealInfoPath, std::ios::trunc);

    mealInfoNew<<content;

    mealInfoNew.close();
}

bool isThereRowForConsumerAndTypeForToday(Consumer consumer, std::string type) {
    std::ifstream mealFile(mealInfoPath);
    std::string line;

    while (std::getline(mealFile, line)) {
            if (line.find(consumer.username) != std::string::npos &&
                line.find(type) != std::string::npos &&
                line.find(todaysDate()) != std::string::npos) {
                return true;
            }
    }

    return false;
}

/**
 * @brief Function for adding rows to mealInfo
 *
 * @param consumer
 * @param type eatenCalories\burntCalories
 *
 */
void addRowsToMealInfo (Consumer consumer, std::string type) {
    std::ofstream mealInfo;

    if (getNumberOfRowsWithMeals() == 0) {
        mealInfo.open(mealInfoPath, std::ios::trunc);
    }
    else {
        mealInfo.open(mealInfoPath, std::ios::app);
    }

    if (!isThereRowForConsumerAndTypeForToday(consumer, type)) {
        mealInfo<<"{\"username\" : \"" + consumer.username + "\", \"type\" : \"" + type + "\", \"dailyCalories\" : \"" +
        std::to_string(consumer.dailyCalories) + "\", \"caloriesSum\" : \"0\", \"date\" : \"" + todaysDate() + "\", }";
    }

    mealInfo.close();
}

/**
 * @brief Function for displaying meals and trainings info
 *
 * @param consumer
 * @param date format d-m-y (31-1-2000)
 *
 */
void displayMealsAndTrainingsForDate (Consumer consumer, std::string date = todaysDate()) {
    std::string lineFromFile, searchType = "eatenCalories";
    std::ifstream mealInfo(mealInfoPath);

    while (std::getline(mealInfo, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            getField(lineFromFile, "date") == date &&
            lineFromFile.find("eatenCalories") != std::string::npos) {
            std::cout<<"The meals with their calories you have consumed are:\n\n";
            std::string mealsLine = lineFromFile.substr(lineFromFile.find(date) + date.size() + 3, lineFromFile.size());

            for (int i = 0; i < mealsLine.size(); i++) {
                if (mealsLine[i] == '"') {
                    i++;
                    std::string meal = "", calories = "";
                    while (mealsLine[i] != '"') {
                        meal += mealsLine[i];
                        i++;
                    }
                    i += 5;
                    while (mealsLine[i] != '"') {
                        calories += mealsLine[i];
                        i++;
                    }
                    i += 2;
                    std::cout<<"\t"<<meal<<" - "<<calories<<" calories\n";
                }
            }
            std::cout<<"\n";
        }

        if (lineFromFile.find(consumer.username) != std::string::npos &&
            lineFromFile.find(date) != std::string::npos &&
            lineFromFile.find("burntCalories") != std::string::npos) {
            std::cout<<"The trainings with their calories you have done are:\n\n";
            std::string mealsLine = lineFromFile.substr(lineFromFile.find(date) + date.size() + 3, lineFromFile.size());

            for (int i = 0; i < mealsLine.size(); i++) {
                if (mealsLine[i] == '"') {
                    i++;
                    std::string meal = "", calories = "";
                    while (mealsLine[i] != '"') {
                        meal += mealsLine[i];
                        i++;
                    }
                    i += 5;
                    while (mealsLine[i] != '"') {
                        calories += mealsLine[i];
                        i++;
                    }
                    i += 2;
                    std::cout<<"\t"<<meal<<" - "<<calories<<" calories\n";
                }
            }
            std::cout<<"\n";
        }
    }

    mealInfo.close();
}

std::string todaysDate () {
    return std::to_string(localTime->tm_mday) + "-" + std::to_string(localTime->tm_mon + 1) + "-" + std::to_string(localTime->tm_year + 1900);
}

/**
 * @brief Function for getting count of consumers registered
 *
 * @return int Count of consumers
 *
 */
int getNumberOfConsumers () {
    std::ifstream usersFile(usersFilePath);
    std::string line;
    int countOfConsumers = 0;

    while (std::getline(usersFile, line)) {
            if (line.size() > 0) {
                countOfConsumers++;
            }
    }

    usersFile.close();

    return countOfConsumers;
}

/**
 * @brief Function for getting count of rows in mealsInfo.txt
 *
 * @return int Count of rows
 *
 */
int getNumberOfRowsWithMeals () {
    std::ifstream mealFile(mealInfoPath);
    std::string line;
    int countOfRows = 0;

    while (std::getline(mealFile, line)) {
            if (line.size() > 0) {
                countOfRows++;
            }
    }

    mealFile.close();

    return countOfRows;
}

/**
 * @brief Function for getting value of field
 *
 * @param line Line with data (usersInfo.txt format)
 * @param fieldType the name of the fields
 * @return string Value of field
 *
 */
std::string getField (std::string line, std::string fieldType) {
    std::string fromTheFieldType = line.substr(line.find("\"" + fieldType + "\"") + fieldType.size() + 6);
    return fromTheFieldType.substr(0, fromTheFieldType.find_first_of("\""));
}

/**
 * @brief Function for setting value of field
 *
 * @param line Line with data (usersInfo.txt format)
 * @param fieldType the name of the fields
 * @param newValue Data to be saved
 *
 */
void setField (std::string line, std::string fieldType, std::string newValue) {
    std::string value = getField(line, fieldType), lineFromFile, content = "";

    std::ifstream usersInfoFile(usersInfoFilePath);


    while (std::getline(usersInfoFile, lineFromFile)) {
        if (lineFromFile == line) {
            std::string toBeReplaced = "\"" + fieldType + "\" : \"" + value + "\",",
            replacement = "\"" + fieldType + "\" : \"" + newValue + "\",";
            lineFromFile = lineFromFile.substr(0, line.find(toBeReplaced)) + replacement + lineFromFile.substr(line.find(toBeReplaced) + toBeReplaced.size(), lineFromFile.size());
        }
        content += lineFromFile + "\n";
    }

    usersInfoFile.close();

    std::ofstream usersInfoFileNew(usersInfoFilePath, std::ios::trunc);

    usersInfoFileNew<<content;

    usersInfoFileNew.close();

}

/**
 * @brief Function for setting value of field
 *
 * @param line Line with data (usersInfo.txt format)
 * @param fieldType the name of the fields
 * @param newValue Data to be saved
 *
 */
std::string setFieldForMealInfo (std::string line, std::string fieldType, std::string newValue) {
    std::string lineFromFile, content = "", returnLine = "";

    std::ifstream mealInfoFile(mealInfoPath);


    while (std::getline(mealInfoFile, lineFromFile)) {
        if (lineFromFile == line) {
            std::string toBeReplaced = fieldType,
            replacement = newValue;
            lineFromFile = lineFromFile.substr(0, line.find(toBeReplaced)) + replacement + lineFromFile.substr(line.find(toBeReplaced) + toBeReplaced.size(), lineFromFile.size());
            returnLine = lineFromFile;
        }
        content += lineFromFile + "\n";
    }

    mealInfoFile.close();

    std::ofstream mealInfoFileNew(mealInfoPath, std::ios::trunc);

    mealInfoFileNew<<content;

    mealInfoFileNew.close();

    return returnLine;

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
    std::ofstream usersInfoFile;

    if (getNumberOfConsumers() == 0) {
        usersInfoFile.open(usersInfoFilePath, std::ios::trunc);
    }
    else {
        usersInfoFile.open(usersInfoFilePath, std::ios::app);
    }

    usersInfoFile<<"{\"username\" : \""<<newConsumer.username<<"\", "<<
    "\"typeAccount\" : \""<<newConsumer.typeAccount<<"\", "<<
    "\"age\" : \""<<newConsumer.age<<"\", "<<
    "\"gender\" : \""<<newConsumer.gender<<"\", "<<
    "\"height\" : \""<<newConsumer.height<<"\", "<<
    "\"weight\" : \""<<newConsumer.weight<<"\", "<<
    "\"activityLevel\" : \""<<newConsumer.activityLevel<<"\", "<<
    "\"dailyCalories\" : \""<<newConsumer.dailyCalories<<"\", "<<
    "\"goal\" : \""<<newConsumer.goal<<"\", }\n";

    usersInfoFile.close();
}

/**
 * @brief Function for adding new consumer
 *
 * @param newConsumer Struct of new consumer to be added
 *
 */
void saveNewUser (Consumer newConsumer) {
    std::ofstream usersFile;

    if (getNumberOfConsumers() == 0) {
        usersFile.open(usersFilePath, std::ios::trunc);
    }
    else {
        usersFile.open(usersFilePath, std::ios::app);
    }

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
    return validateDataString(possibilities, dataString);
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
    return validateDataInt(min_val, max_val, dataInt);
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
        if (line != "" &&
            getField(line, "username") == username) {
                usersInfoFile.close();
                return line;
        }
    }

    usersInfoFile.close();
    return "not found";
}

/**
 * @brief Function for getting line with meal info for username and date
 *
 * @param username Username of consumer
 * @param date Format (17-1-2025)
 * @param type eatenCalories/burntCalories
 *
 */
std::string getLineMealInfo (std::string username, std::string date, std::string type) {
    std::ifstream usersMealInfoFile(mealInfoPath);
    std::string line;

    while (std::getline(usersMealInfoFile, line)) {
        if (line != "" &&
            getField(line, "username") == username &&
            getField(line, "date") == date &&
            getField(line, "type") == type) {
                usersMealInfoFile.close();
                return line;
        }
    }


    usersMealInfoFile.close();
    return "not found";
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
    int eatenCalories = 0, burntCalories = 0;
    if (getLineMealInfo(username, todaysDate(), "eatenCalories") != "not found") {
        eatenCalories = stringToInt(getField(getLineMealInfo(username, todaysDate(), "eatenCalories"), "caloriesSum"));
        burntCalories = stringToInt(getField(getLineMealInfo(username, todaysDate(), "burntCalories"), "caloriesSum"));
    }

    Consumer loggedInConsumer(getField(lineInfo, "typeAccount"),
                     getField(lineInfo, "username"),
                     getField(lineInfo, "password"),
                     stringToInt(getField(lineInfo, "age")),
                     getField(lineInfo, "gender"),
                     stringToInt(getField(lineInfo, "height")),
                     stringToInt(getField(lineInfo, "weight")),
                     stringToInt(getField(lineInfo, "activityLevel")),
                     getField(lineInfo, "goal"),
                     stringToInt(getField(lineInfo, "dailyCalories")),
                     eatenCalories,
                     burntCalories);
    home(loggedInConsumer);
}

/**
 * @type window
 * @brief Function for saving new meal to file
 *
 * @param consumer
 * @param date Today's data
 * @param name
 * @param calories
 * @param type meal/training
 *
 */
void saveMealOrTraining (Consumer consumer, std::string date, std::string name, int calories, std::string type, int oldSum) {
    std::string lineFromFile, content = "", searchType = "eatenCalories";
    int calType = consumer.eatenCalories;
    std::ifstream mealInfo(mealInfoPath);

    if (type == "training") {
        searchType = "burntCalories";
        calType = consumer.burntCalories;
    }

    while (std::getline(mealInfo, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            lineFromFile.find(date) != std::string::npos &&
            lineFromFile.find(searchType) != std::string::npos) {
            lineFromFile = lineFromFile.substr(0, lineFromFile.size() - 1) + "\"" + name + "\" : \"" + std::to_string(calories) + "\", }";
            lineFromFile = lineFromFile.replace(lineFromFile.find("caloriesSum") + 16, std::to_string(oldSum).size(), std::to_string(calType));
        }
        content += lineFromFile + "\n";
    }

    mealInfo.close();


    std::ofstream mealInfoNew(mealInfoPath, std::ios::trunc);

    mealInfoNew<<content;

    mealInfoNew.close();
}

/**
 * @type window
 * @brief Function for adding new meal
 *
 * @param consumer
 *
 */
void addMeal (Consumer consumer) {
    std::string name;
    int calories;

    std::cout<<"Enter name of the meal: ";
    std::cin>>name;
    std::cout<<"Enter calories of the meal: ";
    std::cin>>calories;

    int old = consumer.eatenCalories;
    consumer.eatenCalories += calories;
    addRowsToMealInfo(consumer, "eatenCalories");
    saveMealOrTraining(consumer, todaysDate(), name, calories, "meal", old);
    home(consumer);
}

/**
 * @type window
 * @brief Function for adding new training
 *
 * @param consumer
 *
 */
void addTraining (Consumer consumer) {
    std::string name;
    int calories;

    std::cout<<"Enter name of the training: ";
    std::cin>>name;
    std::cout<<"Enter burnt calories from the training: ";
    std::cin>>calories;

    int old = consumer.burntCalories;
    consumer.burntCalories += calories;
    addRowsToMealInfo(consumer, "burntCalories");
    saveMealOrTraining(consumer, todaysDate(), name, calories, "training", old);
    home(consumer);
}

/**
 * @type window
 * @brief Function for updating age
 *
 * @param consumer
 *
 */
void updateAge (Consumer consumer) {
    int age;

    std::cout<<"Old age: "<<consumer.age<<"\n\n";
    std::cout<<"Enter new age: ";
    std::cin>>age;
    validateDataInt(0, 120, &age);

    consumer.age = age;
    setField(getLineInfoForUsername(consumer.username), "age", std::to_string(consumer.age));
    consumer.setDailyCalories();
    home(consumer);
}

/**
 * @type window
 * @brief Function for updating height
 *
 * @param consumer
 *
 */
void updateHeight (Consumer consumer) {
    int height;

    std::cout<<"Old height: "<<consumer.height<<"\n\n";
    std::cout<<"Enter new height: ";
    std::cin>>height;
    validateDataInt(0, 300, &height);

    consumer.height = height;
    setField(getLineInfoForUsername(consumer.username), "height", std::to_string(consumer.height));
    consumer.setDailyCalories();
    home(consumer);
}

/**
 * @type window
 * @brief Function for updating weight
 *
 * @param consumer
 *
 */
void updateWeight (Consumer consumer) {
    int weight;

    std::cout<<"Old weight: "<<consumer.weight<<"\n\n";
    std::cout<<"Enter new weight: ";
    std::cin>>weight;
    validateDataInt(0, 350, &weight);

    consumer.weight = weight;
    setField(getLineInfoForUsername(consumer.username), "weight", std::to_string(consumer.weight));
    consumer.setDailyCalories();
    home(consumer);
}

/**
 * @type window
 * @brief Function for updating activity level
 *
 * @param consumer
 *
 */
void updateActivityLevel (Consumer consumer) {
    int activityLevel;
    std::cout<<"Old activity level: "<<consumer.activityLevel<<"\n";
    std::cout<<"\n\t1 - Sedentary: little or no exercise"<<
                   "\n\t2 - Light: exercise 1-3 times/week"<<
                   "\n\t3 - Moderate: exercise 4-5 times/week"<<
                   "\n\t4 - Active: daily exercise or intense exercise 3-4 times/week"<<
                   "\n\t5 - Very Active: intense exercise 6-7 times/week"<<
                   "\n\nEnter new activity level: ";
    std::cin>>activityLevel;
    validateDataInt(1, 5, &activityLevel);

    consumer.activityLevel = activityLevel;
    setField(getLineInfoForUsername(consumer.username), "activityLevel", std::to_string(consumer.activityLevel));
    consumer.setDailyCalories();
    home(consumer);
}

/**
 * @type window
 * @brief Function for changing goal
 *
 * @param consumer
 *
 */
void changeGoal (Consumer consumer) {
    std::string goal;
    std::cout<<"Old goal: "<<consumer.goal<<"\n";
    std::cout<<"\n\td - calorie deficit\n\tm - maintenance\n\ts - calorie surplus\n\nEnter your goal: ";
    std::cin>>goal;
    validateDataString({"d", "m", "s"}, &goal);

    consumer.goal = goal;
    setField(getLineInfoForUsername(consumer.username), "goal", consumer.goal);
    consumer.setDailyCalories();
    home(consumer);
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
    home(consumer);
}

/**
 * @brief Function for displaying macros for premium
 *
 * @param consumer
 *
 */
void displayMacrosForPremium (Consumer consumer) {
    if(consumer.typeAccount == "p") {
        int p, c, f;
        if (consumer.goal == "d") {
            p = consumer.dailyCalories * 0.35;
            c = consumer.dailyCalories * 0.35;
            f = consumer.dailyCalories * 0.30;
        }
        else if (consumer.goal == "d") {
            p = consumer.dailyCalories * 0.25;
            c = consumer.dailyCalories * 0.45;
            f = consumer.dailyCalories * 0.30;
        }
        else {
            p = consumer.dailyCalories * 0.40;
            c = consumer.dailyCalories * 0.35;
            f = consumer.dailyCalories * 0.25;
        }
        std::cout<<"\nof which the macronutrients are preffered to be:"<<
        "\n\n\tprotein: " + std::to_string(p) + " calories - " + std::to_string(p / 4) + " grams" +
        "\n\tcarbohydrates: " + std::to_string(c) + " calories - " + std::to_string(c / 4) + " grams" +
        "\n\tfats: " + std::to_string(f) + " calories - " + std::to_string(f / 9) + " grams\n";
    }
}

/**
 * @brief Function for editing meals and trainings for today
 *
 * @param consumer
 * @param type meal\training
 *
 */
void editMealTraining (Consumer consumer, std::string type) {
    std::string lineFromFile, newName, newCalories;
    std::ifstream mealInfo(mealInfoPath);
    std::cout<<"Choose which "<<type<<" would you like to edit\n\n";
    std::vector<std::string> products;

    type = (type == "meal") ? "eatenCalories" : "burntCalories";

    while (std::getline(mealInfo, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            getField(lineFromFile, "date") == todaysDate() &&
            lineFromFile.find(type) != std::string::npos) {
            std::string thingsLine = lineFromFile.substr(lineFromFile.find(todaysDate()) + todaysDate().size() + 3, lineFromFile.size());

            for (int i = 0; i < thingsLine.size(); i++) {
                if (thingsLine[i] == '"') {
                    i++;
                    std::string thing = "", calories = "";
                    while (thingsLine[i] != '"') {
                        thing += thingsLine[i];
                        i++;
                    }
                    i += 5;
                    while (thingsLine[i] != '"') {
                        calories += thingsLine[i];
                        i++;
                    }
                    i += 2;
                    products.push_back(thing + " - " + calories);
                }
            }
            break;
        }
    }

    for(int i = 0; i < products.size(); i++) {
        std::cout<<"\t"<<i+1<<". "<<products[i]<<"\n";
    }

    std::cout<<"Your choice: ";
    std::cin>>input;
    std::string oldName = products[input - 1].substr(0, products[input - 1].find(" - ")),
    oldCalories = products[input - 1].substr(products[input - 1].find(" - ") + 3, products[input - 1].size()),
    oldCaloriesSaved = getField(lineFromFile, "caloriesSum");

    std::cout<<"Enter new name: ";
    std::cin>>newName;
    std::cout<<"Enter new calories: ";
    std::cin>>newCalories;
    int caloriesToBeSaved = stringToInt(getField(lineFromFile, "caloriesSum")) + stringToInt(newCalories) - stringToInt(oldCalories);


    lineFromFile = setFieldForMealInfo(lineFromFile, "\"" + oldName + "\" : \"" + oldCalories + "\"", "\"" + newName + "\" : \"" + newCalories + "\"");
    setFieldForMealInfo(lineFromFile, "\"caloriesSum\" : \"" + oldCaloriesSaved, "\"caloriesSum\" : \"" +
                        std::to_string(caloriesToBeSaved));

    while (std::getline(mealInfo, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            getField(lineFromFile, "date") == todaysDate() &&
            lineFromFile.find(type) != std::string::npos) {

        }
    }

    (type == "eatenCalories") ? consumer.eatenCalories = caloriesToBeSaved : consumer.burntCalories = caloriesToBeSaved;

    mealInfo.close();
    home(consumer);
}

/**
 * @brief Function for displaying a menu of options
 *
 * @param option Description of the chosen option
 *
 */
void manageOptions (std::string option, Consumer* consumer = nullptr) {

    if (option == "Create new account") {
        signUp();
    }
    else if (option == "Log in to existing account") {
        logIn();
    }
    else if (option == "Logout") {
        helloMenu();
    }
    else if (option == "Add new meal") {
        addMeal(*consumer);
    }
    else if (option == "Update age") {
        updateAge(*consumer);
    }
    else if (option == "Update height") {
        updateHeight(*consumer);
    }
    else if (option == "Update weight") {
        updateWeight(*consumer);
    }
    else if (option == "Update activity level") {
        updateActivityLevel(*consumer);
    }
    else if (option == "Change goal") {
        changeGoal(*consumer);
    }
    else if (option == "Add new training") {
        addTraining(*consumer);
    }
    else if (option == "Show history for date") {
        showHistory(*consumer);
    }
    else if (option == "Go back to main menu") {
        home(*consumer);
    }
    else if (option == "Delete history for date") {
        deleteHistoryForDate(*consumer);
    }
    else if (option == "Edit meal for today") {
        editMealTraining(*consumer, "meal");
    }
    else if (option == "Edit training for today") {
        editMealTraining(*consumer, "training");
    }
    else {
        std::cout<<"Error: There is no such an option! Please contact administrator!";
    }
}

/**
 * @brief Function showing history for a certain date
 *
 * @param consumer
 *
 */
void showHistory (Consumer consumer) {
    std::string date;

    std::cout<<"Enter date for showing the history\nformat: d-m-y (example: 31-1-2000): ";
    std::cin>>date;

    std::string lineEaten = getLineMealInfo(consumer.username, date, "eatenCalories");
    std::string lineBurnt = getLineMealInfo(consumer.username, date, "burntCalories"), dailyCal, balanceCal;

    if (lineEaten == "not found" && lineBurnt != "not found") {
        dailyCal = getField(lineBurnt, "dailyCalories");
        balanceCal = "-" + getField(lineBurnt, "caloriesSum");
    }
    else if (lineEaten != "not found" && lineBurnt == "not found") {
        dailyCal = getField(lineEaten, "dailyCalories");
        balanceCal = getField(lineEaten, "caloriesSum");
    }
    else if (lineEaten == "not found" && lineBurnt == "not found") {
        std::cout<<"There is no information for the date entered!\n";
        displayOptions({"Show history for date", "Go back to main menu"}, &consumer);
    }
    else {
        dailyCal = getField(lineBurnt, "dailyCalories");
        balanceCal = std::to_string(stringToInt(getField(lineEaten, "caloriesSum")) - stringToInt(getField(lineBurnt, "caloriesSum")));
        std::cout<<"Your daily calories for the day were "<<dailyCal<<"\nThe calorie intake for the day was "<<balanceCal<<"\n";
        displayMealsAndTrainingsForDate(consumer, date);
    }

    displayOptions({"Go back to main menu"}, &consumer);
}

/**
 * @brief Function for deleting history for a certain date
 *
 * @param consumer
 * @param date
 *
 */
void deleteHistoryForDate (Consumer consumer) {
    std::string date, content = "", lineFromFile;
    int br = 0;

    std::cout<<"Enter date for deleting the history for\nformat: d-m-y (example: 31-1-2000): ";
    std::cin>>date;

    std::ifstream mealInfoFile(mealInfoPath);

    while (std::getline(mealInfoFile, lineFromFile)) {
        if (lineFromFile.find(consumer.username) != std::string::npos &&
            lineFromFile.find("\"" + date + "\"") != std::string::npos) {
                br++;
            continue;
        }
        content += lineFromFile + "\n";
    }

    mealInfoFile.close();

    std::ofstream mealInfoNew(mealInfoPath, std::ios::trunc);

    mealInfoNew<<content;

    mealInfoNew.close();

    if (date == todaysDate()) {
        consumer.eatenCalories = 0;
        consumer.burntCalories = 0;
    }

    (br > 0) ? std::cout<<"All the history for "<<date<<" is deleted!\n\n" : std::cout<<"There is no history found for that date!\n\n";
    displayOptions({"Delete history for date", "Go back to main menu"}, &consumer);
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

    system("cls");
}

/**
 * @brief Function for displaying a menu of options
 *
 * @param options List of options
 *
 */
void displayOptions (std::vector<std::string> options, Consumer* consumer = nullptr) {

    for (int i = 0; i < options.size(); i++) {
        std::cout<<"\t"<<i + 1<<" - "<<options[i]<<"\n";
    }

    std::cout<<"\n";

    chooseOption(options.size());
    manageOptions(options[input - 1], consumer);
}

/**
 * @type window
 * @brief Function for singing up a new consumer
 *
 * @param loggedConsumer
 */
void home (Consumer consumer) {
    system("cls");

    std::cout<<"Hello, "<<consumer.username<<"!\nYour daily calories are "<<consumer.dailyCalories;
    displayMacrosForPremium(consumer);
    std::cout<<"\nYour calorie intake for the day is "<<consumer.eatenCalories - consumer.burntCalories<<" calories!\n\n";
    displayMealsAndTrainingsForDate(consumer);

    displayOptions({"Add new meal", "Edit meal for today", "Add new training",
                   "Edit training for today", "Show history for date", "Delete history for date",
                   "Update age" , "Update height", "Update weight", "Update activity level",
                   "Change goal", "Logout"}, &consumer);
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

    helloMenu();

    return 0;
}
