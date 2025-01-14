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

int input;

struct Consumer {
    std::string username;
    std::string password;
    int age;
    std::string gender;
    int height;
    int weight;
    int activityLevel;
    std::string goal;

    Consumer (std::string username_val,
              std::string password_val,
              int age_val, std::string gender_val,
              int height_val,
              int weight_val,
              int activityLevel_val,
              std::string goal_val) {
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
 * @brief Function for singing up a new consumer
 *
 * @param hostWindow Name of the window, coming the options from
 * @param input The chosen option
 *
 */
void signUp () {
    int age, height, weight, activityLevel;
    std::string gender, goal, username, password;

    std::cout<<"Enter username: ";
    std::cin>>username;
    //TO DO - check if its unique
    std::cout<<"Enter password: ";
    std::cin>>password;
    std::cout<<"Enter your age: ";
    std::cin>>age;
    std::cout<<"Enter your gender (m \\ f): ";
    std::cin>>gender;
    std::cout<<"Enter your height (cm): ";
    std::cin>>height;
    std::cout<<"Enter your weight (kg): ";
    std::cin>>weight;
    std::cout<<"\n1 - Sedentary: little or no exercise"<<
               "\n2 - Light: exercise 1-3 times/week"<<
               "\n3 - Moderate: exercise 4-5 times/week"<<
               "\n4 - Active: daily exercise or intense exercise 3-4 times/week"<<
               "\n5 - Very Active: intense exercise 6-7 times/week"<<
               "\n\nEnter your activity level: ";
    std::cin>>activityLevel;
    std::cout<<"\nd - calorie deficit\nm - maintenance\ns - calorie surplus\n\nEnter your goal: ";
    std::cin>>goal;

    Consumer consumer (username, password, age, gender, height, weight, activityLevel, goal);
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
