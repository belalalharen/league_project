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


int main() {

   
    return 0;
}

// function definitions  
void viewSchedule(int leagueID) {
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
// this functoin is used to view the details of a match, including the teams, goals, date, and status (played or not played). It prompts the user to enter a match ID and displays the relevant information for that match. The function also handles invalid match IDs and allows the user to exit by entering 0.
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

