#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <regex>
#include <string.h>
#include <algorithm>
using namespace std;

void clearScreen();
void sleepForMilliseconds(int milliseconds);
string encrypter(string,int);
string decrypter(string,int);
string concat(string a,string b);
void removeDuplicatesFromFile(const std::string& filename);
class User
{
    bool loggedIn;
    string userName;
    string firstName;
    string lastName;
    string password;
    int age;
    vector<string> friends;
    vector<int> personality;
    bool quizDone;
    string personalityDominant;
    vector<string> options;
    public:
    User();
    void startApp();
    void showPersonalityOptions();
    void login();
    void signUp();
    bool checkPassword();
    bool checkDuplicate();
    void dashboard();
    void quiz();
    void personalitySelect();
    void chat();
    void addFriends();
};
void User::addFriends()
{
    //Checks the file friends/username.txt and saves that list of friends to the vector friends in this class
    string command = "friends/"+userName+".txt";
    removeDuplicatesFromFile(command);
    ifstream contents(command.c_str(),ios::app);
    string name;
    while (getline(contents, name))
    {
        friends.push_back(name);
    }   
}
User::User() {
  // Initialize user data
  quizDone = false; // Flag to indicate if user has completed the personality quiz

  // Initialize personality traits array with default values
  for (int i = 0; i < 4; i++) {
    personality.push_back(0); // Assign 0 to the first 4 personality traits (implement logic for assigning values)
  }

  // Read friend list from file (if username is available)
  if (!userName.empty()) {  // Check if username is set
    string command = "friends/" + userName + ".txt";
    ifstream contents(command.c_str(), ios::app); // Open file for reading in append mode (check for potential issues with append mode)

    if (contents.is_open()) { // Check if file opened successfully
      string name;
      while (getline(contents, name)) {
        friends.push_back(name); // Add friend names from the file to the user's friend list
      }
      contents.close();
      
    } else {
      cerr << "Failed to open friend list file: " << command << endl; // Handle case where file fails to open
    }
  } else {
    // Handle scenario where username is not available (e.g., new user registration)
  }
}

void User::chat()
{
    friends.clear();
    addFriends();
    cout << "You have "<<friends.size()<<" friends"<<endl;
    string command = "friends/"+userName+".txt";
    //We have to read every line and push it to a vector
    ifstream contents(command.c_str(),ios::app);
    string name;
    while (getline(contents, name))
    {
        friends.push_back(name);
    }
    //Asking which one the person wants to talk to
    cout << "Press the number of the person you want to talk to"<<endl;
    for(int i=0;i<friends.size();i++)
    {
        cout << i<<". ";
        cout << friends[i]<<endl;
    }
    cout << "Press "<<friends.size()<<" to go back"<<endl;
    int choice;
    cin >> choice;
    if(choice==friends.size())
    {
        return;
    }
    string command2 = "chats/"+concat(friends[choice],userName)+".txt";
    ifstream contents2(command2.c_str(),ios::app);
    string line;
    while (getline(contents2, line))
    {
        cout << line<<endl;
    }
    //We will display the last 10 messages
    //We will ask the user to enter a message
    mess:
    cout << "Enter your message"<<endl;
    string message;
    //We will make it possible for multiple words to be entered
    cin.ignore();
    getline(cin,message);
    ofstream contents3(command2.c_str(),ios::app);
    contents3<<userName<<": "<<message<<endl;
    contents3.close();
    //option to go back
    cout << "Press 1 to go back, Press 0 to enter any other message"<<endl;
    int choice2;
    cin >> choice2;
    if(choice2==1)
    {
        return;
    }
    else
    {
        goto mess;
    }

    
}
//A function that takes two strings and concats them, keeping the alphabetically smaller string frist
string concat(string a,string b)
{
    if(a<b)
    {
        return a+b;
    }
    else
    {
        return b+a;
    }
}
void User::startApp() {
  // Welcome message and introduction
  cout << "Welcome to FriendFinder!" << endl;
  cout << "This application helps you connect with people who share similar interests." << endl;

  // Main program loop
  while (true) {
    // Clear screen for a clean user experience (consider platform-specific methods)
    clearScreen();

    // Display login/signup options
    cout << "Please choose an option:" << endl;
    cout << "1. Login (if you already have an account)" << endl;
    cout << "2. Sign Up (create a new account)" << endl;

    // Get user choice with input validation
    int choice;
    bool validChoice = false;
    while (!validChoice) {
      cout << "Enter your choice (1 or 2): ";
      cin >> choice;
      if (cin.fail()) {
        cin.clear(); // Clear error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter a number (1 or 2)." << endl;
      } else if (choice < 1 || choice > 2) {
        cout << "Invalid choice. Please enter 1 or 2." << endl;
      } else {
        validChoice = true;
      }
    }

    // Handle user choice
    switch (choice) {
      case 1:
        login(); // Handle login process (already implemented, can be enhanced)
        break;
      case 2:
        signUp(); // Handle signup process (already implemented, can be enhanced)
        break;
      default:
        cerr << "Unexpected error: Invalid choice detected." << endl;
        // Handle unexpected errors gracefully (e.g., log the error and provide user feedback)
        break;
    }

    // Check login status and proceed to dashboard if successful
    if (loggedIn) {
      dashboard(); // Display dashboard and allow user interaction (already implemented, can be enhanced)
    } else {
      // Ask user if they want to try again or exit
      cout << "Login unsuccessful. Would you like to try again (y/n)?" << endl;
      char answer;
      cin >> answer;
      if (tolower(answer) != 'y') {
        break; // Exit the loop if user chooses not to try again
      }
    }
  }

  // Exit message
  cout << "Thank you for using FriendFinder!" << endl;
}

void User::showPersonalityOptions()
{
    string command = personalityDominant+".txt";
    ifstream contents(command.c_str(),ios::app);
    string name;
    while (getline(contents, name))
    {
        if(name!=userName)
        {
            options.push_back(name);
        }
    }

}
void User::login() {
  // Get login credentials from user
  cout << "Enter your Username" << endl;
  cin >> userName;
  cout << "Enter your Password" << endl;
  cin >> password;

  // Check if usernames.txt file exists (stores usernames)
  string command = "usernames.txt";
  ifstream name(command.c_str());
  if (!name) {
    cerr << "Failed to open file: " << command << endl;
    return; // Exit function if file fails to open
  }

  // Search for username in usernames.txt
  bool foundName = false;
  string line;
  while (getline(name, line)) {
    if (line == userName) {
      foundName = true;
      break; // Username found, exit loop
    }
  }
  name.close();

  // Handle case where username is not found
  if (!foundName) {
    cout << "Username not found" << endl;
    return;
  }

  // Check if user data file exists (based on username)
  command = "people/" + userName;
  ifstream file(command.c_str());
  if (!file) {
    cerr << "Failed to open file: " << command << endl;
    return; // Exit function if file fails to open
  }

  // Read user data lines into a vector
  vector<string> lines4;
  string line4;
  while (getline(file, line4)) {
    lines4.push_back(line4);
  }
  file.close();

  // Validate username again (ensure data integrity)
  if (lines4[0] != userName) {
    cout << "Invalid Username" << endl;
    return;
  }

  // Extract key for password decryption (improve encryption method)
  int key = lines4[1].size() + stoi(lines4[3]); // Key based on first name size and age

  // Decrypt password and compare with entered password
  if (lines4[4] != encrypter(password, key)) {
    cout << "Invalid Password" << endl;
    // **Security Risk:** Do not print the decrypted password for security reasons.
    return;
  }

  // Login successful message and update user information
  cout << "You have been logged in" << endl;
  loggedIn = true;
  firstName = lines4[1];
  lastName = lines4[2];
  age = stoi(lines4[3]);
personalityDominant = lines4[5];
  quizDone = true;

  // Simulate login delay (optional)
  sleepForMilliseconds(2000); // Replace with appropriate delay function (if needed)

  // Clear screen after successful login (optional)
  clearScreen(); // Replace with appropriate screen clearing function (if needed)
}

// Method to check if the password is valid
bool User::checkPassword()
{
    // Initialize flags for each condition to false
    bool hasDigit = false;
    bool hasLower = false;
    bool hasUpper = false;

    // Iterate over each character in the password
    for (char c : password)
{
        // If the character is a digit, set hasDigit to true
        if (std::isdigit(c))
{
            hasDigit = true;
        }
        // If the character is a lowercase letter, set hasLower to true
        else if (std::islower(c))
{
            hasLower = true;
        }
        // If the character is an uppercase letter, set hasUpper to true
        else if (std::isupper(c))
{
            hasUpper = true;
        }
    }

    // If the password is empty, return false
    if (password.empty())
{
        return false;
    }
    // If the password does not contain at least one digit, return false
    if (!hasDigit)
{
        return false;
    }
    // If the password does not contain at least one lowercase letter, return false
    if (!hasLower)
{
        return false;
    }
    // If the password does not contain at least one uppercase letter, return false
    if (!hasUpper)
{
        return false;
    }
    return true;
}
void User::signUp()
{
    quizDone=false;
  // Get user information
  cout << "Enter Your First Name" << endl;
  cin >> firstName;
  cout << "Enter your Last Name" << endl;
  cin >> lastName;
  cout << "Enter your Age" << endl;
  cin >> age;

  // Check if user is above minimum age requirement
  if (age < 16)
{
    throw "Below Minimum Age"; // Throw an exception if age is less than 16
  }

  // Loop until a unique username is entered
  while (true)
{
    cout << "Enter a valid Username, make it unique" << endl;
    cin >> userName;
    if (checkDuplicate())
{
      break; // Exit loop if username is unique (implement checkDuplicate() function)
    } else {
      cout << "That username already exists" << endl;
    }
  }

  // Write username to usernames.txt file
  ofstream contents("usernames.txt", ios::app);
  contents << userName << endl;
  contents.close();

  // Create a new file for user data
  string command = "people/" + userName;
  ofstream name(command.c_str());
  if (!name)
{
    cerr << "Failed to create file: " << command << endl;
    return; // Exit function if file creation fails
  }
  name << userName; // Write username to the new file

  // Loop until a secure password is entered
  while (true)
{
    cout << "Enter a Secure Password" << endl;
    cout << "The password should be atleast 5 digits long" << endl;
    cout << "The password must include atleast one Digit" << endl;
    cout << "The password must include atleast one letter,both upper and lowercase" << endl;
    cin >> password;
    if (checkPassword())
{ // Check password strength (implement checkPassword() function)
      break; // Exit loop if password is secure
    }
    clearScreen(); // Clear the screen after failed password attempt (implement clearScreen() function)
  }

  // User registration successful message
  cout << "You have been Registered!" << endl;
  cout << "Filling your final details" << endl;

  // Write user details to the new file
  name << endl << firstName << endl;
  name << lastName << endl;
  name << age << endl;

  // Encrypt password using first name size and age as a key (improve encryption method)
  string encrypt = encrypter(password, firstName.size() + age);
  name << encrypt;

  // Write encrypted password length for decryption (improve encryption method)
  name << endl << firstName.size() + age;

  name.close();

  loggedIn = true; // Set logged in status
}

bool User::checkDuplicate()
{
    // Create a vector to hold the lines
    vector<string> lines;

    // Open the file
    ifstream MyFile("usernames.txt");

    // Check if the file was opened successfully
    if (!MyFile)
{
        cerr << "Unable to open file usernames.txt";
    }

    // Read the lines into the vector
    string line;
    while (getline(MyFile, line))
{
        lines.push_back(line);
    }

    // Close the file
    MyFile.close();
    for(int i=0;i<lines.size();i++)
    {
        if(lines[i]==userName)
            return false;
    }
    return true;

}
void User::dashboard() {
  // Clear the screen and display welcome message
  clearScreen();
  cout << "Welcome to FriendFinder, " << firstName << endl;

  // Check if user has completed the personality quiz
  if (!quizDone) {
    cout << "Before using the application, you have to give a personality quiz" << endl;
    quiz(); // Call function to take the personality quiz
    personalitySelect(); // Call function to process quiz results (implement this function)
    quizDone = true;
  }

  // Main dashboard loop
  while (true) {
    // Display menu options
    cout << "Press 1 to view Friends" << endl;
    cout << "Press 2 to view Recommendations" << endl;
    cout << "Press 3 to quit" << endl;

    // Get user choice
    int choice;
    cin >> choice;

    // Handle user choices
    if (choice == 1) {
      chat(); // Call function to chat with friends (implement this function)
    } else if (choice == 2) {
      // Recommend potential friends
      cout << "Recommended People:" << endl;
      options.clear(); // Clear any previous recommendations
      showPersonalityOptions(); // Call function to find compatible users (implement this function)

      // Display recommended users (if any)
      for (int i = 0; i < options.size(); i++) {
        if (options[i] != userName && // Exclude yourself
            (std::find(friends.begin(), friends.end(), options[i]) == friends.end())) { // Exclude existing friends
          cout << i << ". ";
          cout << options[i] << endl;
        }
      }

      // Handle no recommendations case
      if (options.size() == 0) {
        cout << "No Matches Found, Try Again later";
      }

      // Option to quit recommendations or send friend request
      cout << "Press " << options.size() << " To Quit" << endl;
      cin >> choice;

      if (choice >= options.size()) {
        clearScreen(); // Clear screen if user quits recommendations
      } else {
        // Send friend request to chosen user
        string command = "friends/" + options[choice] + ".txt";
        ofstream contents(command.c_str(), ios::app);
        contents << userName << endl; // Add username to friend request file
        contents.close();
        //Adding friend both sides
        string command2 = "friends/" + userName + ".txt";
        ofstream contents2(command2.c_str(), ios::app);
        contents2 << options[choice] << endl; // Add username to friend request file
        contents2.close();
        cout << "Friend Request Sent" << endl;

        // Create a friend list file for the new connection (if successful)
        string file = "chats/" + concat(userName, options[choice]) + ".txt";
        ofstream contents3(file.c_str(), ios::app);
        contents3.close();
      }
    } else {
      // User chose to quit
      loggedIn = false;
      break;
    }
  }
}

void User::quiz()
{   int z=1;
    try{
    int choice;
    //0 means red, 1 means blue, 2 means green, 3 means yellow
    cout << endl<<"You will be shown 4 adjectives, And have to tell which one matches you best"<<endl;
    cout << "Press 1 for the first one, 2 for the second and so on."<<endl;
    clearScreen();//1
    cout << "self-confident  structured  sensitive  trusting"<<endl;//2
    cin>>choice;//3
    clearScreen();//1
    personality[choice]++;//bas ye kardena//4
     cout << "spontaneous  checks with others  dreamer  analytical"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "likes involvement  likes organization  likes being straightforward  likes to explore"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "stubborn  dictatorial  rebellious  easily offended"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "demanding  nurturing  persistent  quiet"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "joiner  likes to brainstorm  resists change  takes charge"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "cautious  overgenerous  harmonious  energetic"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "caring/helpful  outspoken  steadfast-behaviour  mild mannered"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "believable  forceful  disciplined  possessive"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "daring  idealist  dutiful  playful"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "logical  contented  friendly  bold/audacious"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "“eager beaver”  imaginative  accurate/precise  well-liked"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "reserved  inventive  charismatic  optimistic"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "authoritative  team-worker  independent  traditional"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "talkative  restless  conscientious  modest/unassuming"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "leader  counsellor  designer  controller"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "meticulous  workaholic  supportive  self-directed"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "industrious  attentive-to-details mental-imager positive-thinker"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "task-oriented  people-oriented  idea-oriented  result-oriented"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "emotional  flexible/adaptable  likes-recognition  particular"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "irritable  rigid  easily-slighted  easily-threatened"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "indirect  frank/candid  careful  strict"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "goal-oriented  capable  volunteers  schedule-oriented"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "excels-in-crisis  likes-compliments  dry-humour  avoids-causing-attention"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "likes-watching-people  strong-willed  enthusiastic  sets-very-high-standards"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "very-self-confident  cautious-in-friends  thorough  dresses-in-trendy-ways"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;
     cout << "neat-and-tidy  looks-good-on-outside  avoids-conflicts  usually-right"<<endl;//2
    cin>>choice;//3
clearScreen();
    personality[choice]++;

    if(choice!=1||choice!=2||choice!=3||choice!=4){
        throw z;
    }

    //5
    //Other questions
    //Aur error handling can be added here
    //About 100 lines added from here
}
    catch(int z){
        cout<<"invalid input\nplease enter a valid input"<<endl;
    }
    clearScreen();
}
// Method to select the dominant personality
void User::personalitySelect()
{
    // Initialize the maximum value and its index
    int max = 0, maxOne = 0;

    // Iterate over the personality vector
    for(int i=0; i<personality.size(); i++)
    {
        // If the current value is greater than the maximum value
        if(personality[i] > max)
        {
            // Update the maximum value and its index
            max = personality[i];
            maxOne = i;
        }
    }
    // Now we know maxOne is the index of the largest value
    // Switch on the index of the maximum value
    switch(maxOne)
    {
        // If the index is 1, the dominant personality is blue
        case 1:
            personalityDominant = "blue";
            break;
        // If the index is 2, the dominant personality is green
        case 2:
            personalityDominant = "green";
            break;
        // If the index is 3, the dominant personality is yellow
        case 3:
            personalityDominant = "yellow";
            break;
        // If the index is 0, the dominant personality is red
        case 0:
            personalityDominant = "red";
            break;
    }
    string command = userName;
    //saving the personality in the file
    ofstream user(command.c_str(),ios::app);
    user<<personalityDominant<<endl;
    user.close();
    // Open a file with the name of the dominant personality in append mode
    ofstream blue((personalityDominant+".txt").c_str(), ios::app);

    // Write the user's name to the file
    blue << userName << endl;

    // Close the file
    blue.close();
}
void clearScreen()
{
    #ifdef _WIN32
        system("CLS");
    #else
        // Assume POSIX
        system("clear");
    #endif
}
void sleepForMilliseconds(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}
// These functions currently do not provide actual encryption

string encrypter(string word, int key) {
  // This function does not perform any encryption
  // It simply iterates through the string and adds 0 to each character (no change)

  string k = word;  // Create a copy of the word
  for (int i = 0; i < k.size(); i++) {
    k[i] += 0;  // Add 0 to each character (effectively does nothing)
  }
  return k;
}

string decrypter(string word, int key) {
  // This function does not perform decryption as there was no real encryption

  string k = word;  // Create a copy of the word
  for (int i = 0; i < k.size(); i++) {
    k[i] -= key;  // Subtract key from each character (with no prior encryption, this is meaningless)
  }
  return k;
}
void removeDuplicatesFromFile(const std::string& filename) {
    std::ifstream input(filename);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    input.close();

    std::sort(lines.begin(), lines.end());
    lines.erase(std::unique(lines.begin(), lines.end()), lines.end());

    std::ofstream output(filename);

    for (const auto& uniqueLine : lines) {
        output << uniqueLine << "\n";
    }
}
class tutorial
{
    public:
    void startTutorial();
};
void tutorial::startTutorial()
{
    cout<<"Welcome to the tutorial of Friend Finder"<<endl;
    //Write the whole tutorial here<
    cout << "You can sign up or log in"<<endl;
    
}
// The main function of the program
int main()
{
    try
    {
        while(true)
        {
        // Create a User object named A
        User A;
        // Start the application for user A
        A.startApp();
        // Open the dashboard for user A
        A.dashboard();
    }
    }
    // Catch any exceptions of type char const*
    catch(char const* a)
    {
        // Print "Error" to the console
        cout << "Error" << endl;
        // Print the exception message to the console
        cout << a << endl;
    }
}