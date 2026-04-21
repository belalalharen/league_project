#include <iostream>
#include <string>

using namespace std;

const int MAX_USERS = 100;
const int MAX_TEAMS = 20;
const int MAX_PLAYERS = 34;
const int MAX_MATCHES = 190;
const int MAX_LEAGUES = 2;
const int MAX_TABLE_RECORDS = 20;



struct User {
    int ID;
    string Name;
    string Password;
    bool isAdmin;
};

struct Player {
    int ID;
    string Name;
    int BirthDate[3];
    string Nationality;
};

struct Team {
    int ID;
    string Name;
    Player Players[34];
    bool Ready;
};

struct Result {
    int ID;
    int TeamHomeGoals;
    int TeamAwayGoals;
};

struct Match {
    int ID;
    Team TeamHome;
    Team TeamAway;
    time_t StartDate;
    Result result;
};

struct TableRecords {
    int ID;
    int Standing;
    string TeamName;
    int Played;
    int Wins;
    int Draws;
    int Losses;
    int GoalsFor;
    int GoalsAgainst;
    int Points;
};

struct League {
    string Name;
    int ID;
    Team Teams[20];
    Match Matches[190];
    TableRecords Table[20];
    int StartDate[3];
    bool leaguestarted;
};

// Function declarations
int Option();
void LogIn();
void SignUp();
bool LogOut();
void userAccess(int userID);
void viewPlayerDetails(int leagueID, int teamID, int playerID);
void viewPlayerDetails(int leagueID);
void viewTeamDetails(int leagueID, int teamID);
void viewTeamDetails(int leagueID);
void userPage();
void adminPage();
bool addTeam(int leagueID);
bool addPlayer(int leagueID, int teamID);
bool generateMatchSchedule(int leagueID);
void viewSchedule(int leagueID);
void insertMatchResults(int leagueID);
void rotateArray(Team(&teams)[20]);
void viewMatchDetails(int leagueID);
void viewTable(int leagueID);
void setTable(int leagueID);
void sortTeamsByPoints(int leagueID);
// Global variables
League leagues[MAX_LEAGUES] = {
    {"Laliga", 1,
        {{1, "Real Madrid"}, {2, "Barcelona"}, {3, "Atletico Madrid"}, {4, "Real Sociedad"},
        {5, "Athletic Club"}, {6, "Real Betis"}, {7, "Villarreal"}, {8, "Valencia"},
        {9, "Sevilla"}, {10, "Getafe"}, {11, "Celta Vigo"}, {12, "Osasuna"},
        {13, "Alaves"}, {14, "Las Palmas"}, {15, "Rayo Vallecano"}, {16, "Mallorca"},
        {17, "Granada"}, {18, "Cadiz"}, {19, "Girona"}, {20, "Almeria"}},
        {}, {}}, // League 1 (LaLiga)
    {"Premier League", 2, {}, {}, {}}  // League 2 (Premier League)
};

User users[MAX_USERS] = {
    {1, "A", "a", true},  // Admin user
    {2, "User1", "password1", false},    // Regular user
    {3, "User2", "password2", false}     // Regular user
};
// Main function - 

int main() {

   
    return 0;
}

// function definitions  
void viewSchedule(int leagueID) {
    // Validate league ID
    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league ID." << endl;
        return;
    }

    cout << "==================== " << leagues[leagueID - 1].Name << " Schedule ====================" << endl << endl;

    bool matchesExist = false;
    int round = 0;

    // Table header
    cout << "Match ID | Home Team                | vs | Away Team                | Date       | Time   | Result " << endl;
    cout << "-----------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < MAX_MATCHES; i++) {

        //banner for round number
        round = (i / (MAX_TEAMS / 2)) + 1;

        //when at a number factor of 10, print the round number
        if (i % (10) == 0) {
            cout << "============================== Round " << round << " =============================" << endl << endl;
        }


        if (leagues[leagueID - 1].Matches[i].ID != 0) {
            matchesExist = true;

            // Convert time_t to struct tm for formatted output
            tm timeStruct;
            localtime_s(&timeStruct, &leagues[leagueID - 1].Matches[i].StartDate);

            // Format the match ID (pad with spaces for alignment)
            cout << leagues[leagueID - 1].Matches[i].ID;
            if (leagues[leagueID - 1].Matches[i].ID < 10) {
                cout << "        | ";
            }
            else if (leagues[leagueID - 1].Matches[i].ID < 100) {
                cout << "       | ";
            }
            else {
                cout << "      | ";
            }

            // Home team (pad with spaces to 25 characters)
            string homeTeam = leagues[leagueID - 1].Matches[i].TeamHome.Name;
            cout << homeTeam;
            for (int j = 0; j < 25 - homeTeam.length(); j++) {
                cout << " ";
            }

            cout << "| vs | ";

            // Away team (pad with spaces to 25 characters)
            string awayTeam = leagues[leagueID - 1].Matches[i].TeamAway.Name;
            cout << awayTeam;
            for (int j = 0; j < 25 - awayTeam.length(); j++) {
                cout << " ";
            }

            cout << "| ";

            // Format date: DD/MM/YYYY
            cout << (timeStruct.tm_mday < 10 ? "0" : "") << timeStruct.tm_mday << "/";
            cout << ((timeStruct.tm_mon + 1) < 10 ? "0" : "") << (timeStruct.tm_mon + 1) << "/";
            cout << (1900 + timeStruct.tm_year) << " | ";

            // Format time: HH:MM
            cout << (timeStruct.tm_hour < 10 ? "0" : "") << timeStruct.tm_hour << ":";
            cout << (timeStruct.tm_min < 10 ? "0" : "") << timeStruct.tm_min << " | ";

            // Add result column
            if (leagues[leagueID - 1].Matches[i].result.ID != 0) {
                cout << leagues[leagueID - 1].Matches[i].result.TeamHomeGoals << " - "
                    << leagues[leagueID - 1].Matches[i].result.TeamAwayGoals;
            }
            else {
                cout << "N/A";
            }

            cout << endl;
        }
    }

    if (!matchesExist) {
        cout << "No matches scheduled yet for this league." << endl;
    }

    cout << endl << "-----------------------------------------------------------------------------------------------" << endl;
}
// === Public interface ===

void insertMatchResults(int leagueID) {
    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league ID." << endl;
        return;
    }

    int currentRound = 0;
    // Determine the current round based on unplayed matches
    for (int i = 0; i < MAX_MATCHES; i++) {
        if (leagues[leagueID - 1].Matches[i].ID != 0 && leagues[leagueID - 1].Matches[i].result.ID == 0) {
            currentRound = (i / (MAX_TEAMS / 2)) + 1;
            break;
        }
    }

    while (true) {
        viewSchedule(leagueID);
        cout << "Enter Match ID to add result (0 to stop): ";
        int matchID;
        cin >> matchID;
        if (matchID == 0) break;

        // Validate match ID
        if (matchID < 1 || matchID > MAX_MATCHES || leagues[leagueID - 1].Matches[matchID - 1].ID == 0) {
            cout << "Invalid match ID." << endl;
            continue;
        }

        int matchRound = (matchID - 1) / (MAX_TEAMS / 2) + 1;
        if (matchRound > currentRound) {
            cout << "Complete all matches in round " << currentRound << " first." << endl;
            continue;
        }

        if (leagues[leagueID - 1].Matches[matchID - 1].result.ID != 0) {
            cout << "Result already added." << endl;
            continue;
        }

        // Input match results
        cout << "Enter home team goals: ";
        cin >> leagues[leagueID - 1].Matches[matchID - 1].result.TeamHomeGoals;
        cout << "Enter away team goals: ";
        cin >> leagues[leagueID - 1].Matches[matchID - 1].result.TeamAwayGoals;

        leagues[leagueID - 1].Matches[matchID - 1].result.ID = matchID;

        int homeGoals = leagues[leagueID - 1].Matches[matchID - 1].result.TeamHomeGoals;
        int awayGoals = leagues[leagueID - 1].Matches[matchID - 1].result.TeamAwayGoals;
        int homeID = leagues[leagueID - 1].Matches[matchID - 1].TeamHome.ID - 1;
        int awayID = leagues[leagueID - 1].Matches[matchID - 1].TeamAway.ID - 1;

        // Update table records for both teams
        leagues[leagueID - 1].Table[homeID].Played++;
        leagues[leagueID - 1].Table[awayID].Played++;
        leagues[leagueID - 1].Table[homeID].GoalsFor += homeGoals;
        leagues[leagueID - 1].Table[awayID].GoalsFor += awayGoals;
        leagues[leagueID - 1].Table[homeID].GoalsAgainst += awayGoals;
        leagues[leagueID - 1].Table[awayID].GoalsAgainst += homeGoals;

        if (homeGoals > awayGoals) {
            leagues[leagueID - 1].Table[homeID].Wins++;
            leagues[leagueID - 1].Table[awayID].Losses++;
            leagues[leagueID - 1].Table[homeID].Points += 3;
        }
        else if (awayGoals > homeGoals) {
            leagues[leagueID - 1].Table[awayID].Wins++;
            leagues[leagueID - 1].Table[homeID].Losses++;
            leagues[leagueID - 1].Table[awayID].Points += 3;
        }
        else {
            leagues[leagueID - 1].Table[homeID].Draws++;
            leagues[leagueID - 1].Table[awayID].Draws++;
            leagues[leagueID - 1].Table[homeID].Points++;
            leagues[leagueID - 1].Table[awayID].Points++;
        }

        // Check if all matches in the current round are completed
        bool allMatchesCompleted = true;
        for (int i = (currentRound - 1) * (MAX_TEAMS / 2); i < currentRound * (MAX_TEAMS / 2); i++) {
            if (leagues[leagueID - 1].Matches[i].result.ID == 0) {
                allMatchesCompleted = false;
                break;
            }
        }
        if (allMatchesCompleted) currentRound++;
    }

    // Sort the table after updating results
    sortTeamsByPoints(leagueID);
}

void viewMatchDetails(int leagueID) {
    
    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league ID." << endl;
        return;
    }

    int matchID;
    viewSchedule(leagueID);

    do {
        cout << "=======Match Details=======" << endl << endl;



        cout << "Enter Match ID(0 for exiting): "; cin >> matchID;

        if (matchID > MAX_MATCHES) {
            cout << "Invalid match ID." << endl;
            continue;

        }
        else if (matchID == 0) {


            cout << "Exiting..." << endl;
            break;
        }

        //make for loop to determine round


        int matchRound = (matchID - 1) / (MAX_TEAMS / 2) + 1;
        int homeTeamGoals = leagues[leagueID - 1].Matches[matchID - 1].result.TeamHomeGoals;
        int awayTeamGoals = leagues[leagueID - 1].Matches[matchID - 1].result.TeamAwayGoals;

        cout << "Match ID: " << leagues[leagueID - 1].Matches[matchID - 1].ID;
        cout << " | Match Round: " << matchRound << endl;

        if (leagues[leagueID - 1].Matches[matchID - 1].result.ID != 0) {
            cout << "(Home)" << leagues[leagueID - 1].Matches[matchID - 1].TeamHome.Name << " ["
                << leagues[leagueID - 1].Matches[matchID - 1].result.TeamHomeGoals << "] vs "
                << "(Away) " << leagues[leagueID - 1].Matches[matchID - 1].TeamAway.Name << " ["
                << leagues[leagueID - 1].Matches[matchID - 1].result.TeamAwayGoals << "]" << endl;
            cout << "Match Date: ";
            viewDate(leagues[leagueID - 1].Matches[matchID - 1].StartDate);
            cout << "Status: Played" << endl;
        }
        else {
            cout << leagues[leagueID - 1].Matches[matchID - 1].TeamHome.Name << " vs "
                << leagues[leagueID - 1].Matches[matchID - 1].TeamAway.Name << endl;
            cout << "Match Date: ";
            viewDate(leagues[leagueID - 1].Matches[matchID - 1].StartDate);
            cout << "Status: Not Played" << endl;
        }

    } while (matchID != 0);





}


// Rotate the teams in the array
void rotateArray(Team(&teams)[20]) {
    int size = 20;
    Team temp = teams[size - 1];

    for (int i = size - 1; i > 0; i--) {
        teams[i] = teams[i - 1];
    }

    teams[0] = temp; // rotate all except index 0
}


void viewDate(time_t date) {
    tm timeStruct;
    localtime_s(&timeStruct, &date);

    // Print date using dot operator

    cout << 1900 + timeStruct.tm_year << "-";
    cout << 1 + timeStruct.tm_mon << "-";
    cout << timeStruct.tm_mday << " ";
    cout << timeStruct.tm_hour << ":";
    cout << timeStruct.tm_min << ":";
    cout << timeStruct.tm_sec << endl;

}
// Function to handle user login
void LogIn() {
    int userID = 0;
    string username, password;
    bool isValid = false;

    cout << "=======Log In=======" << endl << endl;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;
    cout << endl;

    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].ID != 0 && users[i].Name == username && users[i].Password == password) {
            isValid = true;
            userID = users[i].ID;
            break;
        }
    }

    if (isValid) {
        cout << "=======Pro Leagues=======" << endl << endl;
        userAccess(userID);
    }
    else {
        cout << "Invalid username or password" << endl;
    }
}
// Function to determine user access level and direct to appropriate page
void userAccess(int userID) {
    if (userID <= 0 || userID > MAX_USERS) {
        cout << "Invalid user ID" << endl;
        return;
    }

    if (users[userID - 1].isAdmin) {
        cout << "=======Admin Page=======" << endl << endl;
        cout << "You are logged in as " << users[userID - 1].Name << endl;
        adminPage();
    }
    else {
        cout << "=======User Page=======" << endl << endl;
        cout << "You are logged in as " << users[userID - 1].Name << endl;
        userPage();
    }
}
// Function to get teamID from a list of teams in a league and return the teamID
int getTeamID(int leagueID) {
    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league ID." << endl;
        return -1;
    }

    cout << "=======Teams in League=======" << endl << endl;
    bool teamsExist = false;

    for (int i = 0; i < MAX_TEAMS; i++) {
        if (leagues[leagueID - 1].Teams[i].ID != 0) {
            cout << leagues[leagueID - 1].Teams[i].ID << "-" << leagues[leagueID - 1].Teams[i].Name << endl;
            teamsExist = true;
        }
    }

    if (!teamsExist) {
        cout << "No teams available in this league." << endl;
        return -1;
    }

    cout << "\nEnter Team Number: ";
    int teamID;
    cin >> teamID;

    if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
        cout << "Invalid team ID or team doesn't exist." << endl;
        return -1;
    }

    return teamID;
}

void SignUp() {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].ID == 0) {
            cout << "=======Sign Up=======" << endl << endl;
            cout << "Enter your username: ";
            cin >> users[i].Name;

            // Check if username already exists
            bool nameExists = false;
            for (int j = 0; j < MAX_USERS; j++) {
                if (j != i && users[j].ID != 0 && users[j].Name == users[i].Name) {
                    nameExists = true;
                    break;
                }
            }

            if (nameExists) {
                cout << "Username already exists. Please try another one." << endl;
                return;
            }

            cout << "Enter your password: ";
            cin >> users[i].Password;
            cout << endl;

            users[i].ID = i + 1;
            users[i].isAdmin = false;  // Default to regular user

            cout << "Sign up successful! You can now log in." << endl;
            return;
        }
    }

    cout << "Cannot register more users. Maximum limit reached." << endl;
}

bool LogOut() {
    cout << "You are logged out." << endl;
    return true;
}

// User page function to display league options and allow users to view details based on their selection
void userPage() {
    cout << "=======Leagues List=======" << endl << endl;
    cout << "1-LaLiga" << endl;
    cout << "2-Premier League" << endl;
    cout << "Choose a league: ";
    int leagueID;
    cin >> leagueID;
    cout << endl;

    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league selection." << endl;
        return;
    }

    if (leagueID == 1) {
        cout << "=======LaLiga=======" << endl << endl;
    }
    else if (leagueID == 2) {
        cout << "=======Premier League=======" << endl << endl;
    }

    bool logout = false;
    while (!logout) {
        cout << "1-View Player Details" << endl;
        cout << "2-View Team Details" << endl;
        cout << "3-View Match Details & Results" << endl;
        cout << "4-View Table" << endl;
        cout << "5-View Schedule" << endl;
        cout << "0-Log-Out" << endl << endl;

        cout << "Choose an option: ";
        int option;
        cin >> option;
        cout << endl;

        switch (option) {
        case 1: {
            int playerID, teamID, choice;
            do {
                cout << "1-View Player Details By Search" << endl;
                cout << "2-View Player Details By Navigation" << endl;
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "=======Player Details=======" << endl;
                    cout << "Enter Team ID: ";
                    cin >> teamID;

                    if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
                        cout << "Invalid team ID or team doesn't exist." << endl;
                        break;
                    }

                    cout << "Enter Player ID: ";
                    cin >> playerID;

                    if (playerID < 1 || playerID > MAX_PLAYERS || leagues[leagueID - 1].Teams[teamID - 1].Players[playerID - 1].ID == 0) {
                        cout << "Invalid player ID or player doesn't exist." << endl;
                        break;
                    }

                    viewPlayerDetails(leagueID, teamID, playerID);
                    break;

                case 2:
                    viewPlayerDetails(leagueID);
                    break;

                default:
                    cout << "Invalid choice. Please select 1 or 2." << endl;
                }
            } while (choice != 1 && choice != 2);
            break;
        }

        case 2: {
            int teamID, choice;
            do {
                cout << "1-View Team Details By Search" << endl;
                cout << "2-View Team Details By Navigation" << endl;
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "=======Team Details=======" << endl;
                    cout << "Enter Team ID: ";
                    cin >> teamID;

                    if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
                        cout << "Invalid team ID or team doesn't exist." << endl;
                        break;
                    }

                    viewTeamDetails(leagueID, teamID);
                    break;

                case 2:
                    viewTeamDetails(leagueID);
                    break;

                default:
                    cout << "Invalid choice. Please select 1 or 2." << endl;
                }
            } while (choice != 1 && choice != 2);
            break;
        }

        case 3:
            cout << "=======Match Details=======" << endl << endl;
            cout << "This feature is not yet implemented." << endl;
            break;

        case 4:
            cout << "=======League Table=======" << endl << endl;
            cout << "This feature is not yet implemented." << endl;
            break;

        case 5:
            cout << "=======League Schedule=======" << endl << endl;
            viewSchedule(leagueID);
            break;

        case 0:
            logout = LogOut();
            break;

        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }
}
// Admin page function to display league options and allow admins to perform various functions based on their selection
void adminPage() {
    cout << "1-LaLiga" << endl;
    cout << "2-Premier League" << endl;
    cout << "Choose a league: ";
    int leagueID;
    cin >> leagueID;

    if (leagueID < 1 || leagueID > MAX_LEAGUES) {
        cout << "Invalid league selection." << endl;
        return;
    }

    cout << endl;
    if (leagueID == 1) {
        cout << "=======LaLiga=======" << endl << endl;
    }
    else if (leagueID == 2) {
        cout << "=======Premier League=======" << endl << endl;
    }

    bool logout = false;
    while (!logout) {
        cout << "=======Admin Page=======" << endl << endl;
        cout << "=== Admin Functions ===" << endl;
        cout << "1-Add Team" << endl;
        cout << "2-Add Player" << endl;
        cout << "3-Generate Match Schedule" << endl;
        cout << "4-Add Match Results" << endl;
        cout << "5-View Player Details" << endl;
        cout << "6-View Team Details" << endl;
        cout << "7-View Match Details & Results" << endl;
        cout << "8-View Table" << endl;
        cout << "9-View Schedule" << endl;
        cout << "0-Log-Out" << endl;
        cout << "=================" << endl;
        cout << "Choose an option: ";
        int option;
        cin >> option;
        cout << endl;

        switch (option) {
        case 1:
            cout << "=======Add Team=======" << endl << endl;
            if (leagues[leagueID - 1].leaguestarted) {
                cout << "League already started. Cannot add teams." << endl;
                break;
            }
            addTeam(leagueID);
            break;

        case 2: {
            int teamID;
            cout << "=======Add Player=======" << endl << endl;
            if (leagues[leagueID - 1].leaguestarted) {
                cout << "League already started. Cannot add players." << endl;
                break;
            }

            cout << leagues[leagueID - 1].Name << " :";
            bool teamsExist = false;
            // Display teams in the league
            for (int i = 0; i < MAX_TEAMS; i++) {
                if (leagues[leagueID - 1].Teams[i].ID != 0) {
                    cout << endl << leagues[leagueID - 1].Teams[i].ID << "-" << leagues[leagueID - 1].Teams[i].Name << endl;
                    teamsExist = true;
                }
            }
            // Check if there are any teams in the league
            if (!teamsExist) {
                cout << " is empty. Please add a team first." << endl;
                break;
            }

            do {
                cout << "Enter Choice: ";
                cin >> teamID;

                if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
                    cout << "Invalid choice or team doesn't exist." << endl;
                }
                else {
                    break;
                }

            } while (true);

            addPlayer(leagueID, teamID);
            break;
        }

        case 3:
            cout << "=======Generate Match Schedule=======" << endl << endl;
            if (leagues[leagueID - 1].leaguestarted) {
                cout << "League already started. Cannot generate schedule." << endl;
                break;
            }
            generateMatchSchedule(leagueID);

            break;

        case 4:
            cout << "=======Add Match Results=======" << endl << endl;
            insertMatchResults(leagueID);
            break;

            // User functions (copied from userPage with minor adaptations)
        case 5: {
            int playerID, teamID, choice;
            do {
                cout << "1-View Player Details By Search" << endl;
                cout << "2-View Player Details By Navigation" << endl;
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "=======Player Details=======" << endl;
                    cout << "Enter Team ID: ";
                    cin >> teamID;

                    if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
                        cout << "Invalid team ID or team doesn't exist." << endl;
                        break;
                    }

                    cout << "Enter Player ID: ";
                    cin >> playerID;

                    if (playerID < 1 || playerID > MAX_PLAYERS || leagues[leagueID - 1].Teams[teamID - 1].Players[playerID - 1].ID == 0) {
                        cout << "Invalid player ID or player doesn't exist." << endl;
                        break;
                    }

                    viewPlayerDetails(leagueID, teamID, playerID);
                    break;

                case 2:
                    viewPlayerDetails(leagueID);
                    break;

                default:
                    cout << "Invalid choice. Please select 1 or 2." << endl;
                }
            } while (choice != 1 && choice != 2);
            break;
        }

        case 6: {
            int teamID, choice;
            do {
                cout << "1-View Team Details By Search" << endl;
                cout << "2-View Team Details By Navigation" << endl;
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "=======Team Details=======" << endl;
                    cout << "Enter Team ID: ";
                    cin >> teamID;

                    if (teamID < 1 || teamID > MAX_TEAMS || leagues[leagueID - 1].Teams[teamID - 1].ID == 0) {
                        cout << "Invalid team ID or team doesn't exist." << endl;
                        break;
                    }

                    viewTeamDetails(leagueID, teamID);
                    break;

                case 2:
                    viewTeamDetails(leagueID);
                    break;

                default:
                    cout << "Invalid choice. Please select 1 or 2." << endl;
                }
            } while (choice != 1 && choice != 2);
            break;
        }

        case 7:
            cout << "=======Match Details=======" << endl << endl;
            viewMatchDetails(leagueID);

            break;

        case 8:
            cout << "=======League Table=======" << endl << endl;

            if (leagues[leagueID - 1].leaguestarted) {
                viewTable(leagueID);
                break;
            }
            cout << "League has not started yet. Cannot view table." << endl;

            break;

        case 9:
            cout << "=======League Schedule=======" << endl << endl;
            viewSchedule(leagueID);
            break;

        case 0:
            logout = LogOut();
            break;

        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }
}


