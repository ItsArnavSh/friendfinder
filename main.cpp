#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <regex>
#include <string.h>
using namespace std;

void clearScreen();
void sleepForMilliseconds(int milliseconds);
string encrypter(string,int);
string decrypter(string,int);
class User
{
    bool loggedIn;
    string userName;
    string firstName;
    string lastName;
    string password;
    int age;
    vector<User> friends;
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
    void showFriends();
};
User::User()
{
    quizDone = false;
    for(int i=0;i<4;i++)
        personality.push_back(0);
}
void User::startApp()
{
    back:
    cout << "Press 1 to Login and 2 to SignUp"<<endl;
    int choice;
    cin >> choice;
    switch(choice)
    {
        case 1:
        login();
        break;
        case 2:
        signUp();
        break;
        default:
        throw  "Enter a valid Number";
    }
    if(loggedIn)
    dashboard();
    else
    goto back;
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
void User::login()
{
    cout << "Enter your Username"<<endl;
    cin >> userName;
    cout << "Enter your Password"<<endl;
    cin >> password;
    string command = "usernames.txt";
    ifstream name(command.c_str());
    if (!name)
    {
        cerr << "Failed to open file: " << command << endl;
        return;
    }
    bool foundName = false;
    string line;
    while (getline(name, line)) {
        if(line==userName)
        {
            foundName=true;
            break;
        }
    }
    if(foundName==false)
    {
        cout << "Username not found";
        return;
    }
    name.close();
    command = "people/"+userName;
    ifstream file(command.c_str());
    if (!file)
    {
        cerr << "Failed to open file: " << command << endl;
        return;
    }
    string line4;
    vector<string> lines4;
    while (getline(file, line4)) {
        lines4.push_back(line4);
    }
    file.close();
    if(lines4[0]!=userName)
    {
        cout << "Invalid Username"<<endl;
        return;
    }
    int key = lines4[1].size()+stoi(lines4[3]);
    cout << (key);
    if((lines4[4])!=encrypter(password,key))
    {
        cout << "Invalid Password"<<endl;
        cout << encrypter(password,key);
        return;
    }
    cout << "You have been logged in"<<endl;
    loggedIn = true;
    firstName = lines4[1];
    lastName = lines4[2];
    age = stoi(lines4[3]);
    sleepForMilliseconds(2000);
    clearScreen();
}
bool User::checkPassword()
{
    //We will use regular expressions for the password checking
    regex num("\\d");
    if(!regex_search(password,num))
    {
        return false;
    }
    regex lower("[a-z]");
    if(!regex_search(password,lower))
    {
        return false;
    }
    regex larger("[A-Z]");
    if(!regex_search(password,larger))
    {
        return false;
    }
    return true;
}
void User::signUp()
{
    cout << "Enter Your First Name"<<endl;
    cin >> firstName;
    cout << "Enter your Last Name"<<endl;
    cin >> lastName;
    cout << "Enter your Age"<<endl;
    cin >>age;
    if(age<16)
    {
        throw "Below Minimum Age";
    }
    while(true)
    {
    cout << "Enter a valid Username, make it unique"<<endl;
    cin >> userName;
    if(checkDuplicate())
    {
        break;
    }
    else
    {
        cout << "That username already exists"<<endl;
    }
    }
    ofstream contents("usernames.txt",ios::app);
    contents<<userName<<endl;
    contents.close();
    string command = "people/"+userName;
    ofstream name(command.c_str());
    if (!name)
    {
        cerr << "Failed to create file: " << command << endl;
        return;
    }
    name<<userName;
    while(true)
    {
    cout << "Enter a Secure Password"<<endl;
    cout << "The password should be atleast 5 digits long"<<endl;
    cout << "The password must include atleast one Digit"<<endl;
    cout << "The password must include atleast one letter,both upper and lowercase"<<endl;
    cin >>password;
    if(checkPassword());
    break;
    clearScreen();
    }
    cout << "You have been Registered!"<<endl;
    cout << "Filling your final details"<<endl;
    name<<endl<<firstName<<endl;
    name<<lastName<<endl;
    name<<age<<endl;
    string encrypt = encrypter(password,firstName.size()+age);
    name<<encrypt;
    name<<endl<<firstName.size()+age;
    name.close();
    loggedIn = true;
}
bool User::checkDuplicate()
{
    // Create a vector to hold the lines
    vector<string> lines;

    // Open the file
    ifstream MyFile("usernames.txt");

    // Check if the file was opened successfully
    if (!MyFile) {
        cerr << "Unable to open file usernames.txt";
    }

    // Read the lines into the vector
    string line;
    while (getline(MyFile, line)) {
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
void User::dashboard()
{
    clearScreen();
    cout << "Welcome to FriendFinder, "<< firstName<<endl;
    if(!quizDone)
    {
        cout << "Before using the application, you have to give a personality quiz"<<endl;
        quiz();
    }
    cout<< "Press 1 to view Friends"<<endl;
    cout << "Press 2 to view Recommendations"<<endl;
    cout << "Press 3 to quit"<<endl;
    int choice;
    cin >> choice;
    if(choice==1)
    {
        //showFriends();
    }
    else if(choice==2)
    {
        personalitySelect();
        cout << "Recommended People: "<<endl;
        for(int i=0;i<options.size();i++)
        {
            cout << i<<". ";
            cout << options[i]<<endl;
        }
        if(options.size()==0)
        {
            cout << "No Matches Found, Try Again later";
        }
        cout << "Press "<<options.size()<<"To Quit"<<endl;
        int choice;
    }
    else
    {

    }


}
void User::quiz()
{
    int choice;
    //0 means red, 1 means blue, 2 means green, 3 means yellow
    cout << "You will be shown 4 adjectives, And have to tell which one matches you best"<<endl;
    cout << "Press 1 for the first one, 2 for the second and so on."<<endl;
    cout << "Passionate Helpful MindFull Tranquil"<<endl;
    cin>>choice;
    personality[choice]++;//bas ye kardena
    //Other questions
    //Aur error handling can be added here
    //About 100 lines added from here
}
void User::personalitySelect()
{
    int max = 0,maxOne = 0;
    for(int i=0;i<personality.size();i++)
    {
        if(personality[i]>max)
        {
            max = personality[i];
            maxOne = i;
        }
    }
    //Now we know maxOne is the largest
    switch(maxOne)
    {
        case 1:
        personalityDominant = "blue";
        break;
        case 2:
        personalityDominant = "green";
        break;
        case 3:
        personalityDominant = "yellow";
        break;
        case 0:
        personalityDominant = "red";
        break;
    }
}
void clearScreen() {
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
string encrypter(string word,int key)
{
    string k = word;
    for(int i=0;i<k.size();i++)
    {
        k[i]+=0;
    }
    return k;
}
string decrypter(string word,int key)
{
    string k = word;
    for(int i=0;i<k.size();i++)
    {
        k[i]-=key;
    }
    return k;
}

int main()
{
    try
    {
    User A;
    A.startApp();
    A.dashboard();
    }
    catch(char const* a)
    {
        cout << "Error"<<endl;
        cout << a<<endl;
    }
}